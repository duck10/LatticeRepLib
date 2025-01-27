#ifndef CMD_S6REFL_CONTROLS_H
#define CMD_S6REFL_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class CmdS6ReflControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CmdS6ReflControls& ctrl) {
      os << "; CmdS6ReflControls\n";
      os << ";   delta " << ctrl.delta << std::endl;
      return os;
   }

   CmdS6ReflControls(double cmdDelta = 1.0E-6) : delta(cmdDelta) {
      InputHandler::registerHandler("SHOW", .5,
         [this](BaseControlVariables&, const std::string& value) {
            showControls = (value == "1" || value == "TRUE" || value.empty());
         }
      );

      InputHandler::registerHandler("DELTA", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            setDelta(std::stod(value));
         }
      );
   }

   bool shouldShowControls() const { return showControls; }
   double getDelta() const { return delta; }

private:
   void setDelta(double value) {
      if (value > 0.0) {
         delta = value;
      }
      else {
         std::cerr << ";Warning: delta must be positive, using default value" << std::endl;
         delta = DEFAULT_DELTA;
      }
   }

   static constexpr double DEFAULT_DELTA = 1.0E-6;
   double delta;
};

#endif // CMD_S6REFL_CONTROLS_H