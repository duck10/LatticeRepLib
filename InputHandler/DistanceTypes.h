#ifndef INPUTHANDLER_DISTANCE_TYPES_H
#define INPUTHANDLER_DISTANCE_TYPES_H

#include "ControlFeature.h"
#include "DistanceTypesUtils.h"
#include "InputHandler.h"

#include <set>
#include <string>
#include <sstream>

class DistanceTypes : public ControlFeature {
public:
   DistanceTypes() {
      // Initialize with default types
      enabledTypes.insert("CS");
      enabledTypes.insert("NC");

      InputHandler::registerHandler("TYPE", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            handleInput("TYPE", value);
         });

      InputHandler::registerHandler("ENABLE", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            handleInput("ENABLE", value);
         });

      InputHandler::registerHandler("DISABLE", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            handleInput("DISABLE", value);
         });
   }

   bool handleInput(const std::string& command, const std::string& value) override {
      if (command == "ENABLE" || command == "TYPE") {
         std::istringstream iss(value);
         std::string type;
         while (iss >> type) {
            std::string upperType = type;
            std::transform(upperType.begin(), upperType.end(), upperType.begin(), ::toupper);

            bool valid = false;
            for (const auto& validType : DistanceTypesUtils::VALID_TYPES) {
               std::string upperValid = validType;
               std::transform(upperValid.begin(), upperValid.end(), upperValid.begin(), ::toupper);
               if (upperType == upperValid) {
                  enabledTypes.insert(validType);  // Insert canonical case version
                  valid = true;
                  break;
               }
            }
            if (!valid) {
               std::cerr << ";Warning: Invalid distance type: " << type << std::endl;
            }
         }
         return true;
      }
      else if (command == "DISABLE") {
         std::istringstream iss(value);
         std::string type;
         while (iss >> type) {
            std::string upperType = type;
            std::transform(upperType.begin(), upperType.end(), upperType.begin(), ::toupper);

            bool valid = false;
            for (const auto& validType : DistanceTypesUtils::VALID_TYPES) {
               std::string upperValid = validType;
               std::transform(upperValid.begin(), upperValid.end(), upperValid.begin(), ::toupper);
               if (upperType == upperValid) {
                  enabledTypes.erase(validType);  // Erase canonical case version
                  valid = true;
                  break;
               }
            }
            if (!valid) {
               std::cerr << ";Warning: Invalid distance type: " << type << std::endl;
            }
         }
         return true;
      }
      return false;
   }

   std::string getFeatureState() const override {
      std::ostringstream oss;
      oss << ";Enabled Distances: ";
      for (const auto& type : enabledTypes) {
         oss << type << " ";
      }
      oss << "\n";
      return oss.str();
   }

   bool isTypeEnabled(const std::string& type) const {
      return enabledTypes.find(type) != enabledTypes.end();
   }

   const std::set<std::string>& getEnabledTypes() const { return enabledTypes; }

   void writeToStream(std::ostream& os) const override {
      os << getFeatureState();
   }

private:
   std::set<std::string> enabledTypes;
};

#endif  // INPUTHANDLER_DISTANCE_TYPES_H