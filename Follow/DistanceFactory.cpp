// DistanceFactory.cpp
#include "DistanceFactory.h"

std::vector<std::unique_ptr<Distance>> DistanceFactory::createEnabledDistances(const ControlVariables& controlVars) {
   std::vector<std::unique_ptr<Distance>> distfuncs;
   for (const auto& distName : controlVars.enabledDistances) {
      if (distName == "CS") {
         distfuncs.push_back(std::make_unique<CS6Distance>());
      }
      else if (distName == "NC") {
         distfuncs.push_back(std::make_unique<NCDistance>());
      }
      else if (distName == "V7") {
         distfuncs.push_back(std::make_unique<V7Distance>());
      }
      else if (distName == "R9") {
         distfuncs.push_back(std::make_unique<R9Distance>());
      }
   }
   return distfuncs;
}
