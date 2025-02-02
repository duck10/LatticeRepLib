#ifndef CMD_TO_RI_CONTROLS_H
#define CMD_TO_RI_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"
#include <sstream>

class CmdToRiControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CmdToRiControls& ctrl) {
      os << "; CmdToRIControls\n";
      os << "; Show Contols " << ((ctrl.showControls) ? " true" : "; false") << std::endl;;
      os << "; webRun " << ((ctrl.webRun) ? " true" : " false") << std::endl;
      return os;
   }

   CmdToRiControls() {
   }

private:

};

#endif // CMD_TO_RI_CONTROLS_H