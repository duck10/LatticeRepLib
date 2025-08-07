#ifndef CMD_P3_DISTS_CONTROLS_H
#define _DISTS_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class CmdP3DistsControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CmdP3DistsControls& ctrl) {
      os << "; CmdP3DistsControls\n";
      return os;
   }

   bool shouldShowControls() const { return showControls; }
};

#endif // CMD_P3_DISTS_CONTROLS_H
