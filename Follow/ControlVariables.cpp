#include "ControlVariables.h"
#include <iostream>
#include <iomanip>
#include <sstream>

// Initialize static members
std::atomic<bool> ControlVariables::g_running(true);
std::atomic<bool> ControlVariables::g_in_input(false);

ControlVariables::ControlVariables()
   : followerMode(FollowerMode::POINT),
   perturbations(1),
   perturbBy(0.1),
   numFollowerPoints(100),
   printDistanceData(false),
   glitchesOnly(false),
   fixRandomSeed(false),
   timestamp(true),
   randomSeed(42),
   shouldDetectGlitches(true),
   glitchThresholdPercent(4.0),
   showDataMarkers(true),
   blockstart(0),
   blocksize(BLOCKSIZE_LIMIT),
   filePrefix("FOL")
{
   ensureDefaultTypesEnabled();
}

std::string ControlVariables::getControlVariablesAsString() const {
   std::ostringstream oss;
   oss << *this;
   return oss.str();
}

void ControlVariables::printControlVariables(std::ostream& out) const {
   out << ";Control Variables:\n" << getControlVariablesAsString();
}

std::ostream& operator<<(std::ostream& os, const ControlVariables& cv) {
   os << ";Control Variables:\n";
   os << ";Follower Mode: ";
   os << cv.getFollowerMode();
   os << "\n";
   if (cv.perturbations == 1) {
      os << ";Perturbations: no perturbations\n";
   }
   else{
   os << ";Perturbations: " << cv.perturbations << "\n";
   os << ";Perturb By: " << cv.perturbBy << "\n";
   }
   os << ";Number of Follower Points: " << cv.numFollowerPoints << "\n";
   os << ";Print Distance Data: " << (cv.printDistanceData ? "Yes" : "No") << "\n";
   os << ";Glitches Only: " << (cv.glitchesOnly ? "Yes" : "No") << "\n";
   os << ";Should Detect Glitches: " << (cv.shouldDetectGlitches ? "Yes" : "No") << "\n";
   os << ";Glitch Threshold Percent: " << cv.glitchThresholdPercent << "\n";
   os << ";Show Data Markers: " << (cv.showDataMarkers ? "Yes" : "No") << "\n";
   os << ";File Prefix: " << cv.filePrefix << "\n";
   os << ";Blockstart: " << cv.blockstart << "\n";
   os << ";Blocksize: " << cv.blocksize << "\n";
   os << ";Enabled Distances: ";

   for (const auto& dist : cv.enabledDistances) {
      os << dist << " ";
   }
   os << "\n";

   if (!cv.pathStart.empty()) os << ";Starting Point of Path: ";
   for (const auto& point : cv.pathStart) {
      os << point.getLatticeType() << " " << S6(point.getCell()) << "\n";
   }
   os << "\n";

   if (!cv.currentFilename.empty()) os << ";Current Filename: " << cv.currentFilename << "\n";
   return os;
}

std::string ControlVariables::getFollowerMode() const {
   switch (followerMode) {
   case FollowerMode::POINT: return "POINT";
   case FollowerMode::LINE: return "LINE";
   case FollowerMode::CHORD: return "CHORD";
   case FollowerMode::CHORD3: return "CHORD3";;
   case FollowerMode::TRIANGLE: return "TRIANGLE";
   }
   return "UNKNOWN FOLLOWER MODE";
}


int ControlVariables::getVectorsPerTrial() const {
   switch (followerMode) {
   case FollowerMode::POINT: return 1;
   case FollowerMode::LINE: return 2;
   case FollowerMode::CHORD: return 2;
   case FollowerMode::CHORD3: return 3;
   case FollowerMode::TRIANGLE: return 3;
   default: return 1; // Default to 1 if unknown mode
   }
}

const std::vector<std::string> ControlVariables::VALID_DISTANCE_TYPES = {
    "CS", "NC", "V7", "R9"
};

void ControlVariables::disableDistanceType(const std::string& type) {
   enabledDistances.erase(type);
}

void ControlVariables::setDistanceTypes(const std::string& types, bool clearExisting ) {
   if (clearExisting) {
      enabledDistances.clear();
   }
   std::istringstream iss(types);
   std::string type;
   while (iss >> type) {
      if (isValidDistanceType(type)) {
         enabledDistances.insert(type);
      }
   }
   if (enabledDistances.empty()) {
      ensureDefaultTypesEnabled();
   }
}

void ControlVariables::toggleDistanceType(const std::string& type, bool enable) {
   if (isValidDistanceType(type)) {
      if (enable) {
         enabledDistances.insert(type);
      }
      else {
         enabledDistances.erase(type);
      }
      if (enabledDistances.empty()) {
         ensureDefaultTypesEnabled();
      }
   }
}

std::string ControlVariables::getDistanceTypes() const {
   std::ostringstream oss;
   for (const auto& type : enabledDistances) {
      oss << type << " ";
   }
   return oss.str();
}

bool ControlVariables::isValidDistanceType(const std::string& type) const {
   return std::find(VALID_DISTANCE_TYPES.begin(),
      VALID_DISTANCE_TYPES.end(),
      type) != VALID_DISTANCE_TYPES.end();
}

void ControlVariables::updateBlock(const size_t numFiles, const bool isWebMode) {
   // Convert any negative values to 0 for safety
   blockstart = std::max(size_t(0), blockstart);

   if (isWebMode) {
      // Web mode: enforce maximum blocksize of 20
      const size_t MAX_WEB_BLOCKSIZE = 20;

      // If user hasn't set blocksize, use default
      if (blocksize == 0) {
         blocksize = std::min(MAX_WEB_BLOCKSIZE, numFiles);
      }
      else {
         blocksize = std::min(blocksize, MAX_WEB_BLOCKSIZE);
      }

      // If user hasn't set blockstart, start from beginning
      if (blockstart == 0) {
         blockstart = 0;
      }

   }
   else {
      // Non-web mode: use full file list unless user specified otherwise
      if (blocksize == 0) {
         blocksize = numFiles;
      }
      if (blockstart == 0) {
         blockstart = 0;
      }
   }

   // Final bounds checking to prevent indexing beyond array
   if (blockstart >= numFiles) {
      blockstart = numFiles > 0 ? numFiles - 1 : 0;
   }

   // Adjust blocksize if it would go past end of array
   const size_t remainingFiles = numFiles - blockstart;
   if (blocksize > remainingFiles) {
      blocksize = remainingFiles;
   }
}