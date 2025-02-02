#ifndef CMD_TO_V7_CONTROLS_H
#define CMD_TO_V7_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class CmdToV7Controls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CmdToV7Controls& ctrl) {
      os << "; CmdToV7Controls\n";
      return os;
   }

   CmdToV7Controls() {
   }

   bool shouldShowControls() const { return showControls; }
};

#endif // CMD_TO_V7_CONTROLS_H