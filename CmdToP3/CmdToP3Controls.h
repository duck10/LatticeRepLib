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
   }

   bool shouldShowControls() const { return showControls; }
};

#endif // CMD_TO_P3_CONTROLS_H
