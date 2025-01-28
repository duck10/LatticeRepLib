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
      InputHandler::registerHandler("SHOW", .5,
         [this](BaseControlVariables&, const std::string& value) {
            showControls = (value == "1" || value == "TRUE" || value.empty());
         }
      );

      InputHandler::registerHandler("PREFIX", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            setPrefix(value);
         }
      );
   }

   bool shouldShowControls() const { return showControls; }
   std::string getPrefix() const { return prefix; }

private:
   void setPrefix(const std::string& newPrefix) {
      prefix = newPrefix;
   }

   std::string prefix = "PPL";
};

#endif // CMD_PLOT_POLAR_CONTROLS_H
