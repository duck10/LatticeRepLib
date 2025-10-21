#ifndef TEMPLATE_CONTROLS_H
#define TEMPLATE_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class eISENSTEINControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const eISENSTEINControls& ctrl) {
      os << "; TEMPLATE_Controls\n";
      return os;
   }

   eISENSTEINControls() {
   }

   bool shouldShowControls() const { return showControls; }
};

#endif // TEMPLATE_CONTROLS_H

