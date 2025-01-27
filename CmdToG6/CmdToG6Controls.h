#ifndef CMD_TO_G6_CONTROLS_H
#define CMD_TO_G6_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class CmdToG6Controls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CmdToG6Controls& ctrl) {
      os << "; CmdToG6Controls\n";
      return os;
   }

   CmdToG6Controls() {
      InputHandler::registerHandler("SHOW", .5,
         [this](BaseControlVariables&, const std::string& value) {
            showControls = (value == "1" || value == "TRUE" || value.empty());
         }
      );
   }

   bool shouldShowControls() const { return showControls; }
};

#endif // CMD_TO_G6_CONTROLS_H