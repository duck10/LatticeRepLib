#ifndef CMD_DISTS_CONTROLS_H
#define CMD_DISTS_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class CmdDistsControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CmdDistsControls& ctrl) {
      os << "; CmdDistsControls\n";
      return os;
   }

   CmdDistsControls() {
      InputHandler::registerHandler("SHOW", .5,
         [this](BaseControlVariables&, const std::string& value) {
            showControls = (value == "1" || value == "TRUE" || value.empty());
         }
      );
   }

   bool shouldShowControls() const { return showControls; }
};

#endif // CMD_DISTS_CONTROLS_H
