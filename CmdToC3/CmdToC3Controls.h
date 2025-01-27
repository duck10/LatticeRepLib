#ifndef CMD_TO_C3_CONTROLS_H
#define CMD_TO_C3_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class CmdToC3Controls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CmdToC3Controls& ctrl) {
      os << "; CmdToC3Controls\n";
      return os;
   }

   CmdToC3Controls() {
      InputHandler::registerHandler("SHOW", .5,
         [this](BaseControlVariables&, const std::string& value) {
            showControls = (value == "1" || value == "TRUE" || value.empty());
         }
      );
   }

   bool shouldShowControls() const { return showControls; }
};

#endif // CMD_TO_C3_CONTROLS_H