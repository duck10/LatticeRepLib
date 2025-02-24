#ifndef FOLLOW_CONTROLS_H
#define FOLLOW_CONTROLS_H

#include "BaseControlVariables.h"
#include "BlockUtils.h"
#include "DistanceTypes.h"
#include "DistanceTypesUtils.h"
#include "FollowerMode.h" 
#include "FollowerPointsUtils.h"
#include "GlitchUtils.h"
#include "InputHandler.h"
#include "PerturbationUtils.h"
#include "LRL_StringTools.h"

#include <memory>
#include <string>
#include <vector>
#include <atomic>
#include <set>
#include <sstream>
#include <unordered_set>

class FollowControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const FollowControls& fc) {
      os << fc.getState();
      return os;
   }

   static std::atomic<bool> g_running;
   static std::atomic<bool> g_in_input;

   FollowControls() {
      InputHandler::registerHandler("BLOCKSIZE", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            setBlockSize(std::stoul(value));
         });

      InputHandler::registerHandler("BLOCKSTART", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            setBlockStart(std::stoul(value));
         });
      InputHandler::registerHandler("TYPE", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            handleDistanceType(LRL_StringTools::strToupper(value));
         });

      InputHandler::registerHandler("ENABLE", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            handleEnableDistance(LRL_StringTools::strToupper(value));
         });

      InputHandler::registerHandler("DISABLE", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            handleDisableDistance(LRL_StringTools::strToupper(value));
         });

      InputHandler::registerHandler("MODE", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            followerMode = FollowerModeUtils::fromString(value);
         });

      InputHandler::registerHandler("FOLLOWERMODE", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            followerMode = FollowerModeUtils::fromString(value);
         });

      InputHandler::registerHandler("PERTURBATIONS", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            perturbations = PerturbationUtils::validatePerturbCount(value);
         });

      InputHandler::registerHandler("PERTURBBY", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            perturbBy = PerturbationUtils::validatePerturbAmount(value);
         });

      InputHandler::registerHandler("POINTS", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            numFollowerPoints = FollowerPointsUtils::validatePointCount(stoi(value));
         });

      InputHandler::registerHandler("NUMFOLLOWERPOINTS", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            numFollowerPoints = FollowerPointsUtils::validatePointCount(stoi(value));
         });

      InputHandler::registerHandler("PRINTDATA", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            printDistanceData = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

      InputHandler::registerHandler("PRINTDISTANCEDATA", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            printDistanceData = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

      InputHandler::registerHandler("GLITCHTHRESHOLD", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            glitchThresholdPercent = GlitchUtils::validateGlitchThresholdPercent(value);
         });
      InputHandler::registerHandler("GLITCHESONLY", .35,
         [this](BaseControlVariables&, const std::string& value) {
            glitchesOnly = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         }
      );

      InputHandler::registerHandler("DETECTGLITCHES", .35,
         [this](BaseControlVariables&, const std::string& value) {
            shouldDetectGlitches = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         }
      );

      InputHandler::registerHandler("SHOWMARKERS", .35,
         [this](BaseControlVariables&, const std::string& value) {
            showDataMarkers = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         }
      );

      InputHandler::registerHandler("SHOWDATAMARKERS", .35,
         [this](BaseControlVariables&, const std::string& value) {
            showDataMarkers = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         }
      );

   }

   std::string  getFilePrefix() const {return prefix;}

   DistanceTypes getDistanceTypes() const { 
      DistanceTypes dt;
      dt.setEnabledTypes(getEnabledTypes());
      return dt;
      }


   void setBlockSize(int size) {
      long long val = static_cast<long long>(size);
      if (val <= 0) {
         std::cout << ";Warning: Blocksize must be positive, using "
            << DEFAULT_BLOCKSIZE << std::endl;
         blocksize = DEFAULT_BLOCKSIZE;
      }
      else if (webRun && val > MAX_BLOCKSIZE) {
         std::cout << ";Warning: Blocksize exceeds web limit, using "
            << MAX_BLOCKSIZE << std::endl;
         blocksize = MAX_BLOCKSIZE;
      }
      else {
         blocksize = static_cast<size_t>(val);
      }
   }

   void setBlockStart(int start) {
      long long val = static_cast<long long>(start);
      if (val < 0) {
         std::cout << ";Warning: Blockstart cannot be negative, using 0" << std::endl;
         blockstart = 0;
      }
      else {
         blockstart = start;
      }
   }

   std::string replaceCharacters(const std::string& input, const std::string& targets, char replacement) {
      std::unordered_set<char> targetSet(targets.begin(), targets.end()); // Use a set for efficient look-up
      std::string result = input;

      for (char& c : result) {
         if (targetSet.find(c) != targetSet.end()) {
            c = replacement;
         }
      }

      return result;
   }

   // Distance type methods
   void handleDistanceType(const std::string& value) {
      std::istringstream iss(value);
      std::string type;
      while (iss >> type) {
         if (isValidDistanceType(type)) {
            enabledTypes.insert(type);
         }
         else {
            std::cout << ";Warning: Invalid distance type: " << type << std::endl;
         }
      }
   }

   void handleEnableDistance(const std::string& value) {
      handleDistanceType(value);  // Same logic as TYPE
   }

   void handleDisableDistance(const std::string& value) {
      std::istringstream iss(value);
      std::string type;
      while (iss >> type) {
         if (isValidDistanceType(type)) {
            enabledTypes.erase(type);
         }
         else {
            std::cout << ";Warning: Invalid distance type: " << type << std::endl;
         }
      }
   }

   bool isValidDistanceType(const std::string& type) const {
      return std::find(DistanceTypesUtils::VALID_TYPES.begin(),
         DistanceTypesUtils::VALID_TYPES.end(),
         type) != DistanceTypesUtils::VALID_TYPES.end();
   }

   // Public accessors
   int getVectorsPerTrial() const { return FollowerModeUtils::getVectorsNeeded(followerMode); }
   int getPerturbations() const { return perturbations; }
   double getPerturbBy() const { return perturbBy; }
   FollowerMode getMode() const { return followerMode; }

   bool shouldPrintDistanceData() const { return printDistanceData; }
   bool shouldShowOnlyGlitches() const { return glitchesOnly; }
   bool shouldShowControls() const { return showControls; }
   bool isGlitchDetectionEnabled() const { return shouldDetectGlitches; }
   double getGlitchThreshold() const { return glitchThresholdPercent; }
   bool shouldShowMarkers() const { return showDataMarkers; }
   int getNumPoints() const { return numFollowerPoints; }

   size_t getBlockStart() const { return blockstart; }
   size_t getBlockSize() const { return blocksize; }
   std::string getPrefix() const { return prefix; }
   const std::set<std::string>& getEnabledTypes() const { return enabledTypes; }

   void updateFilenames(const std::vector<std::string>& names) { filenames = names; }
   const std::vector<std::string>& getFilenames() const { return filenames; }

   std::string getState() const {
      std::ostringstream oss;
      if (perturbations == 1) {
         oss << ";Perturbations: no perturbations\n";
      }
      else {
         oss << ";Perturbations: " << perturbations << "\n"
            << ";Perturb By: " << perturbBy << "\n";
      }
      oss << ";Follower Mode: " << FollowerModeUtils::toString(followerMode) << "\n"
          << ";Number of Follower Points: " << numFollowerPoints << "\n"
         << ";Print Distance Data: " << (printDistanceData ? "Yes" : "No") << "\n"
         << ";Glitches Only: " << (glitchesOnly ? "Yes" : "No") << "\n"
         << ";Should Detect Glitches: " << (shouldDetectGlitches ? "Yes" : "No") << "\n"
         << ";Glitch Threshold Percent: " << glitchThresholdPercent << "\n"
         << ";Show Data Markers: " << (showDataMarkers ? "Yes" : "No") << "\n"
         << ";File Prefix: " << prefix << "\n\n"
         << ";Web run: " << ((this->getHasWebInput()) ? "true" : "false") << "\n"
         << ";Blockstart: " << blockstart << "\n"
         << ";Blocksize: " << blocksize << "\n\n"
         << ";Enabled Distances: ";
      for (const auto& type : enabledTypes) {
         oss << type << " ";
      }
      oss << "\n\n";
      return oss.str();
   }

private:
   // Block processing members
   static constexpr size_t MIN_BLOCKSIZE = BlockUtils::MIN_BLOCKSIZE;
   static constexpr size_t MAX_BLOCKSIZE = BlockUtils::MAX_BLOCKSIZE;
   static constexpr size_t DEFAULT_BLOCKSIZE = BlockUtils::MAX_BLOCKSIZE;
   size_t blockstart = 0;
   size_t blocksize = DEFAULT_BLOCKSIZE;

   // File prefix member
   const std::string prefix = "FOL";

   // Distance types member
   std::set<std::string> enabledTypes{ "CS", "NC" };

   // Existing members
   FollowerMode followerMode = FollowerMode::POINT;
   int perturbations = 1;
   double perturbBy = 0.1;
   int numFollowerPoints = 100;
   bool printDistanceData = false;
   bool glitchesOnly = false;
   bool shouldDetectGlitches = true;
   double glitchThresholdPercent = 4.0;
   bool showDataMarkers = true;
   std::vector<std::string> filenames;
};


#endif // FOLLOW_CONTROLS_H

