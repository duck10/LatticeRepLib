#ifndef CONTROL_VARIABLES_H
#define CONTROL_VARIABLES_H

#include <string>
#include <vector>
#include <set>
#include <atomic>
#include <iostream>
#include <sstream>
#include "G6.h"  // For inputPoints type

enum class FollowerMode {
   POINT,
   LINE,
   CHORD,
   CHORD3,
   TRIANGLE
};

class ControlVariables {
public:
   friend std::ostream& operator<<(std::ostream& os, const ControlVariables& cv);

   ControlVariables();

   void updateFilenames(int totalRuns) {
      filenames.clear();
      for (int i = 0; i < totalRuns; ++i) {
         filenames.push_back(filePrefix + "_" + std::to_string(i + 1) + ".svg");
      }
   }

   void updateFilenames(const std::vector<std::string>& names) {
      filenames = names;
   }


   // Static control flags
   static std::atomic<bool> g_running;
   static std::atomic<bool> g_in_input;

   // Static variables (set at startup)
   int perturbations;  // Renamed from trials
   double perturbBy;
   double glitchLevel;
   int numFollowerPoints;
   bool printDistanceData;
   bool glitchesOnly;
   bool niggliSellingFirst;
   bool fixRandomSeed;
   bool timestamp;
   int randomSeed;
   bool shouldDetectGlitches;
   double glitchThreshold;
   bool showDataPoints;
   std::string filePrefix;
   std::set<std::string> enabledDistances;

   // Dynamic variables
   FollowerMode followerMode;
   std::vector<std::string> filenames;
   std::string currentFilename;

   // Member functions
   void setFollowerMode(FollowerMode mode);
   void ensureDefaultTypesEnabled();
   void enableDistance(const std::string& distName);
   void disableDistance(const std::string& distName);
   void setCurrentFilename(size_t index);
   std::string getControlVariablesAsString() const;
   void setDistanceTypes(const std::string& types, bool clearExisting = false);
   std::string getDistanceTypes() const;
   void toggleDistanceType(const std::string& type, bool enable);
   static const std::vector<std::string> VALID_DISTANCE_TYPES;

   bool isValidDistanceType(const std::string& type) const;
   void printControlVariables(std::ostream& out = std::cout) const;
   size_t getCurrentFilenameIndex() const;
   int getVectorsPerTrial() const;
   std::string getFollowerMode();

   // Color constants for SVG plots
   static const std::string CS6_COLOR;
   static const std::string NC_COLOR;
};

// Implementation of member functions

inline void ControlVariables::ensureDefaultTypesEnabled() {
   if (enabledDistances.empty()) {
      enabledDistances.insert("CS");
      enabledDistances.insert("NC");
   }
}

inline void ControlVariables::enableDistance(const std::string& distName) {
   enabledDistances.insert(distName);
}

inline void ControlVariables::disableDistance(const std::string& distName) {
   enabledDistances.erase(distName);
}

inline void ControlVariables::setCurrentFilename(size_t index) {
   if (index < filenames.size()) {
      currentFilename = filenames[index];
   }
   else {
      currentFilename = "default_" + std::to_string(index) + ".svg";
   }
}

#endif // CONTROL_VARIABLES_H
