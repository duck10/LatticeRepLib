#ifndef CMD_TO_P3_CONTROLS_H
#define CMD_TO_P3_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class CmdToP3Controls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CmdToP3Controls& ctrl) {
      os << "; CmdToP3Controls\n";
      return os;
   }

   CmdToP3Controls() {
      InputHandler::registerHandler("SHOW", .5,
         [this](BaseControlVariables&, const std::string& value) {
            showControls = (value == "1" || value == "TRUE" || value.empty());
         }
      );
   }

   bool shouldShowControls() const { return showControls; }
};

#endif // CMD_TO_P3_CONTROLS_H
