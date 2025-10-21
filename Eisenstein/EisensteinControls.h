#ifndef EISENSTEIN_CONTROLS_H
#define EISENSTEIN_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class EisenControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const EisenControls& ctrl) {
      os << "; CmdToB4Controls\n";
      return os;
   }

   EisenControls() {
   }

   bool shouldShowControls() const { return showControls; }
};

#endif // EISENSTEIN_CONTROLS_H

