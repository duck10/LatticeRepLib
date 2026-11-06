#ifndef CMDBGAOL_CONTROLS_H
#define CMDBGAOL_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

#include <ostream>

class BgaolControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const BgaolControls& ctrl) {
      os << "; CmdBGAOL Controls\n";
      os << ";   from the web " << ((ctrl.hasWebInput) ? "true" : "false") << std::endl;
      os << "; showDetails " << ((ctrl.showDetails) ? "true" : "false") << std::endl;
      os << "; blocksize = " << ctrl.blocksize << std::endl;
      os << "; blockstart = " << ctrl.blockstart << std::endl;

      return os;
   }

   BgaolControls() {
      InputHandler::registerHandler("SHOW", .5,
         [this](BaseControlVariables& controls, const std::string& value) {
            showControls = (value == "1" || value == "TRUE" || value.empty());
         }
      );

      InputHandler::registerHandler("DETAILS", .35,
         [this](BaseControlVariables&, const std::string& value) {
            showDetails = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         }
      );

   }

   bool shouldShowDetails() const { return showDetails; }
   bool getHasWebInput() const { return hasWebInput; }
   void setWebRun(bool webInput) { hasWebInput = webInput; }

private:
   size_t blocksize = 0;
   size_t blockstart = 0;
   bool showDetails = false;
   bool hasWebInput = false;
};

#endif // CMDBGAOL_CONTROLS_H

