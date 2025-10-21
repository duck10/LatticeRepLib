#ifndef TESTBED_CONTROLS_H
#define TESTBED_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class TestbedControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const TestbedControls& ctrl) {
      os << "; CTestbedControls\n";
      return os;
   }

   TestbedControls() {
   }

   bool shouldShowControls() const { return showControls; }
};

#endif // TESTBED_CONTROLS_H