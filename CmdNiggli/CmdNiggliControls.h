#ifndef CMD_NIGGLI_CONTROLS_H
#define CMD_NIGGLI_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class CmdNiggliControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CmdNiggliControls& ctrl) {
      os << "; CmdNiggliControls\n";
      return os;
   }

   bool shouldShowControls() const { return showControls; }
};

#endif // CMD_NIGGLI_CONTROLS_H
