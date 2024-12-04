#include "ControlVariables.h"
#include <iostream>
#include <iomanip>
#include <sstream>

// Initialize static members
std::atomic<bool> ControlVariables::g_running(true);
std::atomic<bool> ControlVariables::g_in_input(false);

// In ControlVariables.cpp, define:
const std::string ControlVariables::CS6_COLOR = "#0077BB";  // Blue
const std::string ControlVariables::NC_COLOR = "#EE7733";   // Orange

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
   os << ";Perturbations: " << cv.perturbations << "\n";
   os << ";Perturb By: " << cv.perturbBy << "\n";
   os << ";Number of Follower Points: " << cv.numFollowerPoints << "\n";
   os << ";Print Distance Data: " << (cv.printDistanceData ? "Yes" : "No") << "\n";
   os << ";Glitches Only: " << (cv.glitchesOnly ? "Yes" : "No") << "\n";
   os << ";Should Detect Glitches: " << (cv.shouldDetectGlitches ? "Yes" : "No") << "\n";
   os << ";Glitch Threshold Percent: " << cv.glitchThresholdPercent << "\n";
   os << ";Show Data Markers: " << (cv.showDataMarkers ? "Yes" : "No") << "\n";
   os << ";File Prefix: " << cv.filePrefix << "\n";
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
