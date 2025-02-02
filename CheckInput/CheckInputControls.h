#ifndef CMD_CHECK_INPUT_CONTROLS_H
#define CMD_CHECK_INPUT_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class CheckInputControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CheckInputControls& ctrl) {
      os << "; CmdCheckInputControls\n";
      os << "; webRun " << ctrl.getHasWebInput() << std::endl;
      os << "; echo " << (ctrl.getEcho()) ? "true" : "false";
      return os;
   }
};

#endif // CMD_CHECK_INPUT_CONTROLS_H