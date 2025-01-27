#ifndef CMD_INVERSE_CONTROLS_H
#define CMD_INVERSE_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class CmdInverseControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CmdInverseControls& ctrl) {
      os << "; CmdInverseControls\n";
      return os;
   }

   CmdInverseControls() {
      InputHandler::registerHandler("SHOW", .5,
         [this](BaseControlVariables&, const std::string& value) {
            showControls = (value == "1" || value == "TRUE" || value.empty());
         }
      );
   }

   bool shouldShowControls() const { return showControls; }
};

#endif // CMD_INVERSE_CONTROLS_H
