#ifndef CMD_PLOT_C3_CONTROLS_H
#define CMD_PLOT_C3_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class CmdPlotC3Controls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CmdPlotC3Controls& ctrl) {
      os << "; CmdPlotC3Controls\n";
      os << ";   file prefix " << ctrl.prefix << std::endl;
      return os;
   }

   CmdPlotC3Controls() {

   }

   bool shouldShowControls() const { return showControls; }
   std::string getPrefix() const { return prefix; }

private:

   const std::string prefix = "PLT";
};

#endif // CMD_PLOT_C3_CONTROLS_H
