// BaseControlVariables.h
#ifndef BASE_CONTROL_VARIABLES_H
#define BASE_CONTROL_VARIABLES_H

#include "ControlFeature.h"
#include <vector>
#include <memory>
#include <ostream>
#include <sstream>

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

   virtual bool handleInput(const std::string& command, const std::string& value) {
      for (auto& feature : features) {
         if (feature->handleInput(command, value)) {
            return true;
         }
      }
      return false;
   }

   virtual std::string getState() const {
      std::ostringstream oss;
      for (const auto& feature : features) {
         oss << feature->getFeatureState();
      }
      return oss.str();
   }

   friend std::ostream& operator<<(std::ostream& os, const BaseControlVariables& cv);
};

#endif // BASE_CONTROL_VARIABLES_H