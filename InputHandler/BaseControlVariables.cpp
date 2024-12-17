// BaseControlVariables.cpp
#include "BaseControlVariables.h"

std::ostream& operator<<(std::ostream& os, const BaseControlVariables& cv) {
   os << ";Control Variables:\n";
   for (const auto& feature : cv.features) {
      feature->writeToStream(os);
   }
   return os;
}