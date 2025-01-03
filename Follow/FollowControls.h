#ifndef FOLLOW_CONTROLS_H
#define FOLLOW_CONTROLS_H

#include "BaseControlVariables.h"
#include "BlockProcessing.h"
#include "DistanceTypes.h"
#include "FilePrefix.h"
#include "FollowerMode.h" 
#include "FollowerPointsUtils.h"
#include "GlitchUtils.h"
#include "PerturbationUtils.h"

#include <memory>
#include <string>
#include <vector>
#include <atomic>

class FollowControls : public BaseControlVariables {
public:
   static std::atomic<bool> g_running;
   static std::atomic<bool> g_in_input;

   FollowControls()
   {
      features.push_back(std::make_unique<FilePrefix>("FOL"));
      features.push_back(std::make_unique<BlockProcessing>());
      features.push_back(std::make_unique<DistanceTypes>());

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
            printDistanceData = (value == "1" || value == "true" || value == "TRUE" || value.empty());
         });

      InputHandler::registerHandler("PRINTDISTANCEDATA", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            printDistanceData = (value == "1" || value == "true" || value == "TRUE" || value.empty());
         });

      // GLITCHTHRESHOLD needs to come before GLITCHESONLY
      InputHandler::registerHandler("GLITCHTHRESHOLD", 0.2,
         [this](BaseControlVariables&, const std::string& value) {
            //std::cerr << ";Debug: GLITCHTHRESHOLD handler called with value: " << value << std::endl;
            glitchThresholdPercent = GlitchUtils::validateGlitchThresholdPercent(value);
         });

      InputHandler::registerHandler("GLITCHESONLY", .2,
         [this](BaseControlVariables&, const std::string& value) {
            glitchesOnly = (value == "1" || value == "true" || value == "TRUE" || value.empty());
         }
      );

      InputHandler::registerHandler("DETECTGLITCHES", .2,
         [this](BaseControlVariables&, const std::string& value) {
            shouldDetectGlitches = (value == "1" || value == "true" || value == "TRUE" || value.empty());
         }
      );

      InputHandler::registerHandler("SHOW", .5,
         [this](BaseControlVariables&, const std::string& value) {
            showControls = (value == "1" || value == "true" || value == "TRUE" || value.empty());
         }
      );
   }

   // Feature accessors
   FilePrefix* getFilePrefix() { return static_cast<FilePrefix*>(features[0].get()); }
   BlockProcessing* getBlockProcessing() { return static_cast<BlockProcessing*>(features[1].get()); }
   DistanceTypes* getDistanceTypes() const { return static_cast<DistanceTypes*>(features[2].get()); }

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

   void updateFilenames(const std::vector<std::string>& names) { filenames = names; }
   const std::vector<std::string>& getFilenames() const { return filenames; }

   std::string getState() const override {
      std::ostringstream oss;
      oss << ";Follower Mode: " << FollowerModeUtils::toString(followerMode) << "\n";
      if (perturbations == 1) {
         oss << ";Perturbations: no perturbations\n";
      }
      else {
         oss << ";Perturbations: " << perturbations << "\n"
            << ";Perturb By: " << perturbBy << "\n";
      }
      oss << ";Number of Follower Points: " << numFollowerPoints << "\n"
         << ";Print Distance Data: " << (printDistanceData ? "Yes" : "No") << "\n"
         << ";Glitches Only: " << (glitchesOnly ? "Yes" : "No") << "\n"
         << ";Should Detect Glitches: " << (shouldDetectGlitches ? "Yes" : "No") << "\n"
         << ";Glitch Threshold Percent: " << glitchThresholdPercent << "\n"
         << ";Show Data Markers: " << (showDataMarkers ? "Yes" : "No") << "\n";

      for (const auto& f : features) {
         oss << f->getFeatureState() << "\n";
      }
      return oss.str();
   }

private:
   FollowerMode followerMode;
   int perturbations = 1;
   double perturbBy = 0.1;
   int numFollowerPoints = 100;
   bool printDistanceData = false;
   bool showControls = false;
   bool glitchesOnly = false;
   bool shouldDetectGlitches = true;
   double glitchThresholdPercent = 4.0;
   bool showDataMarkers = true;
   std::vector<std::string> filenames;
};
#endif // FOLLOW_CONTROLS_H

