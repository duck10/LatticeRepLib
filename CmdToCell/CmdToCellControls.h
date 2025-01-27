#ifndef CMD_TO_CELL_CONTROLS_H
#define CMD_TO_CELL_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class CmdToCellControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CmdToCellControls& ctrl) {
      os << "; CmdToCellControls\n";
      return os;
   }

   CmdToCellControls() {
      InputHandler::registerHandler("SHOW", .5,
         [this](BaseControlVariables&, const std::string& value) {
            showControls = (value == "1" || value == "TRUE" || value.empty());
         }
      );
   }

   bool shouldShowControls() const { return showControls; }
};

#endif // CMD_TO_CELL_CONTROLS_H