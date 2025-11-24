#ifndef TEMPLATE_CONTROLS_H
#define TEMPLATE_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class TestSelling3DControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const TestSelling3DControls& ctrl) {
      os << "; CmdToB4Controls\n";
      return os;
   }

   TestSelling3DControls() {
   }

   bool shouldShowControls() const { return showControls; }
};

#endif // TEMPLATE_CONTROLS_H

