// BaseControlVariables.h
#ifndef BASE_CONTROL_VARIABLES_H
#define BASE_CONTROL_VARIABLES_H

#include "ControlFeature.h"
#include <vector>
#include <memory>
#include <ostream>

class BaseControlVariables {
protected:
   std::vector<std::unique_ptr<ControlFeature>> features;

public:
   virtual ~BaseControlVariables() = default;

   template<typename T>
   T* getFeature() {
      for (auto& feature : features) {
         if (auto* p = dynamic_cast<T*>(feature.get())) {
            return p;
         }
      }
      return nullptr;
   }

   friend std::ostream& operator<<(std::ostream& os, const BaseControlVariables& cv);
};

#endif // BASE_CONTROL_VARIABLES_H
