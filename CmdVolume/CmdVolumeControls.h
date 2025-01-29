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
      InputHandler::registerHandler("SHOW", .5,
         [this](BaseControlVariables&, const std::string& value) {
            showControls = (value == "1" || value == "TRUE" || value.empty());
         }
      );
   }

   bool shouldShowControls() const { return showControls; }
};

#endif // CMD_VOLUME_CONTROLS_H
