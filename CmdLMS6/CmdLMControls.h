#ifndef CMD_LM_CONTROLS_H
#define CMD_LM_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class CmdLMControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CmdLMControls& ctrl) {
      os << "; CmdLMControls\n";
      return os;
   }


   bool shouldShowControls() const { return showControls; }
};

#endif // CMD_LM_CONTROLS_H
