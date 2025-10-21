#ifndef TEST_DELTA
#define TEST_DELTA

#include "BaseControlVariables.h"
#include "InputHandler.h"

class TEST_DELTAControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const TEST_DELTAControls& ctrl) {
      os << "; CmdToB4Controls\n";
      return os;
   }

   TEST_DELTAControls() {
   }

   bool shouldShowControls() const { return showControls; }
};

#endif // TEST_DELTA

