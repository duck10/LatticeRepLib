#ifndef CMD_TO_U_CONTROLS_H
#define CMD_TO_U_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class CmdToUControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CmdToUControls& ctrl) {
      os << "; CmdToUControls\n";
      return os;
   }

   CmdToUControls() {
   }

   bool shouldShowControls() const { return showControls; }
};

#endif // CMD_TO_U_CONTROLS_H
