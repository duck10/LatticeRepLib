#ifndef CMDGEN_CONTROLS_H
#define CMDGEN_CONTROLS_H

#include "BaseControlVariables.h"
#include "CmdGenControls.h"
#include "CmdGenUtils.h"
#include "InputHandler.h"

class CmdGenControls : public BaseControlVariables {
public:
   CmdGenControls() {
      InputHandler::clearHandlers();

      InputHandler::registerHandler("COUNT", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            generateCount = CmdGenUtils::validatePointCount(stoi(value));
         });

      InputHandler::registerHandler("TYPE", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            generateCount = CmdGenUtils::validatePointCount(stoi(value));
         });
   }


private:
   int generateCount = 5;
   std::set<std::string> selectedTypes;
};
#endif // CMDGEN_CONTROLS_H