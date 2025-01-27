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

   CmdDeloneControls() {
      InputHandler::registerHandler("SHOW", .5,
         [this](BaseControlVariables&, const std::string& value) {
            showControls = (value == "1" || value == "TRUE" || value.empty());
         }
      );
   }

   bool shouldShowControls() const { return showControls; }

};

#endif // CMD_DELONE_CONTROLS_H
