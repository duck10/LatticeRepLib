#ifndef CMD_VOLUME_CONTROLS_H
#define CMD_VOLUME_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class CmdVolumeControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CmdVolumeControls& ctrl) {
      os << "; CmdVolumeControls\n";
      return os;
   }

   CmdVolumeControls() {
   }

   bool shouldShowControls() const { return showControls; }
};

#endif // CMD_VOLUME_CONTROLS_H
