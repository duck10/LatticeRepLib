#ifndef DISTANCEFACTORY_H
#define DISTANCEFACTORY_H

#include <vector>
#include <memory>
#include <string>
#include "ControlVariables.h"
#include "Distance.h"

class DistanceFactory {
public:
   static std::vector<std::unique_ptr<Distance>> createEnabledDistances(const ControlVariables& controlVars);

   // Helper method to get all available distance types
   static std::vector<std::string> getAvailableDistanceTypes() {
      return { "CS", "NC", "V7", "R9" };
   }

   // Helper method to check if a distance type is valid
   static bool isValidDistanceType(const std::string& distName) {
      for (const auto& type : getAvailableDistanceTypes()) {
         if (type == distName) return true;
      }
      return false;
   }
};

#endif // DISTANCEFACTORY_H