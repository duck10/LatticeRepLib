#ifndef GENERATE_CENTERED_CELLS_H
#define TEMPLATE_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class GenerateCenteredCellsControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const GenerateCenteredCellsControls& ctrl) {
      os << "; CmdToB4Controls\n";
      return os;
   }

   GenerateCenteredCellsControls() {
   }

   bool shouldShowControls() const { return showControls; }
};

#endif // TEMPLATE_CONTROLS_H

