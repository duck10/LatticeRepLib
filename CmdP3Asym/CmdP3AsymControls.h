#ifndef CMD_P3_ASYM_CONTROLS_H
#define CMD_P3_ASYM_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

class CmdP3AsymControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CmdP3AsymControls& ctrl) {
      os << "; CmdP3AsymControls\n";
      return os;
   }

   CmdP3AsymControls(int CommandStride = 10) : stride(CommandStride) {
      InputHandler::registerHandler("STRIDE", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            stride = std::stoi(value);
         }
      );

   }
   bool shouldShowControls() const { return showControls; }

private:
   int stride = 10;
};

#endif // CMD_P3_ASYM_CONTROLS_H
