#ifndef CMD_DELONE_CONTROLS_H
#define CMD_DELONE_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class CmdDeloneControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CmdDeloneControls& ctrl) {
      os << "; CmdDeloneControls\n";
      return os;
   }

   bool shouldShowControls() const { return showControls; }

};

#endif // CMD_DELONE_CONTROLS_H
