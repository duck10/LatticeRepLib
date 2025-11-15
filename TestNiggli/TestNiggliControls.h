#ifndef TEMPLATE_CONTROLS_H
#define TEMPLATE_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class TemplateControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const TemplateControls& ctrl) {
      os << "; CmdToB4Controls\n";
      return os;
   }

   TemplateControls() {
   }

   bool shouldShowControls() const { return showControls; }
};

#endif // TEMPLATE_CONTROLS_H

