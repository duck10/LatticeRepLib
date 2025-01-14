#ifndef CMDGEN_CONTROLS_H
#define CMDGEN_CONTROLS_H

#include "BaseControlVariables.h"
#include "CmdGenControls.h"
#include "CmdGenUtils.h"
#include "InputHandler.h"

#include <iostream>
#include <ostream>

class CmdGenControls : public BaseControlVariables {
public:

   friend std::ostream& operator<< (std::ostream& os, const CmdGenControls& cgc) {
      os << "number of cells of each type to generate: " << cgc.generateCount << std::endl;
      os << "raw types to generate: " << cgc.types << std::endl;
      for (const auto& t : cgc.selectedTypes) {
         os << "  selected type: " << t << std::endl;
      }
      return os;
   }

   CmdGenControls() {
      InputHandler::clearHandlers();

      InputHandler::registerHandler("COUNT", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            generateCount = CmdGenUtils::validatePointCount(stoi(value));
         });




      //InputHandler::registerHandler("PERTURBBY", 0.35,
      //   [](BaseControlVariables& cv, const std::string& value) {
      //      if (auto* followCtrl = cv.getFeature<FollowerControl>()) {
      //         followCtrl->setPerturbBy(std::stod(value));
      //      }
      //   });

      InputHandler::registerHandler("TYPE", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            std::istringstream iss(value);
            std::string token;
            while (iss >> token) {
               const std::string type = CmdGenUtils::validateTypes(token);
               selectedTypes.insert(token);
            }
         });
   }

   int getCount() const { return generateCount; }

   std::set<std::string> getTypes() const { return selectedTypes; }

   std::vector<std::string> getTypesAsVector() const {
      std::vector<std::string> out;
      for (const auto& s : selectedTypes) {
         out.emplace_back(s);
      }
      return out;
   }
   std::string getTypesAsString() const {
      std::string out;
      for (const auto& s : selectedTypes) {
         out += s + " ";
      }
      return out;
   }

private:
   int generateCount = 5;
   std::string types;
   std::set<std::string> selectedTypes;
};
#endif // CMDGEN_CONTROLS_H