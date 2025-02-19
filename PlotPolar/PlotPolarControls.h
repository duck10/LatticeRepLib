#ifndef CMD_PLOT_POLAR_CONTROLS_H
#define CMD_PLOT_POLAR_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class CmdPlotPolarControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CmdPlotPolarControls& ctrl) {
      os << "; CmdPlotPolarControls\n";
      os << ";   file prefix " << ctrl.prefix << std::endl;
      return os;
   }

   CmdPlotPolarControls() {
   }

   bool shouldShowControls() const { return showControls; }
   std::string getPrefix() const { return prefix; }

private:

   const std::string prefix = "PPL";
};

#endif // CMD_PLOT_POLAR_CONTROLS_H
