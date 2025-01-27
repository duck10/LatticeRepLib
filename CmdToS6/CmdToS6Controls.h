#ifndef CMD_TO_S6_CONTROLS_H
#define CMD_TO_S6_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class CmdToS6Controls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CmdToS6Controls& ctrl) {
      os << "; CmdToS6Controls\n";
      return os;
   }

   CmdToS6Controls() {
      InputHandler::registerHandler("SHOW", .5,
         [this](BaseControlVariables&, const std::string& value) {
            showControls = (value == "1" || value == "TRUE" || value.empty());
         }
      );
   }

   bool shouldShowControls() const { return showControls; }
};

#endif // CMD_TO_S6_CONTROLS_H
