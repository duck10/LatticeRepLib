#ifndef CMD_PRIM_CONTROLS_H
#define CMD_PRIM_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class CmdPrimControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CmdPrimControls& ctrl) {
      os << "; CmdPrimControls\n";
      return os;
   }

   CmdPrimControls() {
      InputHandler::registerHandler("SHOW", .5,
         [this](BaseControlVariables&, const std::string& value) {
            showControls = (value == "1" || value == "TRUE" || value.empty());
         }
      );
   }

   bool shouldShowControls() const { return showControls; }
};

#endif // CMD_PRIM_CONTROLS_H