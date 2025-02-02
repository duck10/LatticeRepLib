#ifndef CMD_TO_B4_CONTROLS_H
#define CMD_TO_B4_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class CmdToB4Controls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CmdToB4Controls& ctrl) {
      os << "; CmdToB4Controls\n";
      return os;
   }

   CmdToB4Controls() {
   }

   bool shouldShowControls() const { return showControls; }
};

#endif // CMD_TO_B4_CONTROLS_H