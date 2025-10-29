#ifndef TEMPLATE_CONTROLS_H
#define TEMPLATE_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class TestControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const TestControls& ctrl) {
      os << "; CmdToB4Controls\n";
      return os;
   }

   TestControls() {
   }

   bool shouldShowControls() const { return showControls; }
};

#endif // TEMPLATE_CONTROLS_H

