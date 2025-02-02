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
   }

   bool shouldShowControls() const { return showControls; }
};

#endif // CMD_PRIM_CONTROLS_H