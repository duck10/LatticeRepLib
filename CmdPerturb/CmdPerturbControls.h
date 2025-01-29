#ifndef CMD_PERTURB_CONTROLS_H
#define CMD_PERTURB_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"
#include "LRL_StringTools.h"

class CmdPerturbControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CmdPerturbControls& ctrl) {
      os << "; CmdPerturbControls\n";
      os << ";   number of perturbations " << ctrl.perturbCount << std::endl;
      os << ";   perturbation value per thousand " << ctrl.perturbDelta << std::endl;
      return os;
   }

   CmdPerturbControls(int cmdCount = 20, double cmdDelta = 1.0)
      : perturbCount(cmdCount), perturbDelta(cmdDelta) {
      InputHandler::registerHandler("SHOW", .5,
         [this](BaseControlVariables&, const std::string& value) {
            showControls = (value == "1" || value == "TRUE" || value.empty());
         }
      );

      InputHandler::registerHandler("COUNT", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            setPerturbCount(std::stoi(value));
         }
      );

      InputHandler::registerHandler("DELTA", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            setPerturbDelta(std::stod(value));
         }
      );
   }

   int getPerturbCount() const { return perturbCount; }
   double getPerturbDelta() const { return perturbDelta; }
   bool shouldShowControls() const { return showControls; }

private:
   void setPerturbCount(int count) {
      if (count <= 0) {
         std::cerr << ";Warning: Perturbation count must be positive, using "
            << DEFAULT_COUNT << std::endl;
         perturbCount = DEFAULT_COUNT;
      }
      else {
         perturbCount = count;
      }
   }

   void setPerturbDelta(double delta) {
      if (delta <= 0.0) {
         std::cerr << ";Warning: Perturbation delta must be positive, using "
            << DEFAULT_DELTA << std::endl;
         perturbDelta = DEFAULT_DELTA;
      }
      else {
         perturbDelta = delta;
      }
   }

   static constexpr int DEFAULT_COUNT = 20;
   static constexpr double DEFAULT_DELTA = 4.0;

   int perturbCount;
   double perturbDelta;
};

#endif // CMD_PERTURB_CONTROLS_H