#ifndef INPUTHANDLER_CMD_SCALE_FEATURES_H
#define INPUTHANDLER_CMD_SCALE_FEATURES_H

#include "BaseControlVariables.h"
#include "CmdScaleUtils.h"
#include "InputHandler.h"
#include "LRL_StringTools.h"

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

class CmdScaleControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CmdScaleControls& ctrl) {
      os << "; CmdScaleControlVariables\n";
      os << ";   type       " << ctrl.scaleType << std::endl;
      os << ";   from the web " << ((ctrl.hasWebInput) ? "true" : "false");
      return os;
   }

   CmdScaleControls() {

      InputHandler::registerHandler("SHOW", .5,
         [this](BaseControlVariables&, const std::string& value) {
            showControls = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         }
      );

      // Register handler for TYPE command
      InputHandler::registerHandler("TYPE", 0.35,
         [this](const BaseControlVariables&, const std::string& value) {
            scaleType = CmdScaleUtils::verifyScaleType(value);
         });
   }

   bool shouldShowControls() const { return showControls; }
   bool getHasWebInput() const { return hasWebInput; }
   void setWebRun(bool webInput) { hasWebInput = webInput; }
   std::string getScaleType() const { return scaleType; }

   void setScaleType(const std::string& type) {
      std::string upperType = LRL_StringTools::strToupper(type);
      const std::vector<std::string> allowedTypes{CmdScaleUtils::verifyScaleType(upperType)};
      if (std::find(allowedTypes.begin(), allowedTypes.end(), upperType) != allowedTypes.end()) {
         scaleType = upperType;
      }
      else {
         std::cerr << ";Warning: Invalid type. Using default 'VOLUME'" << std::endl;
         scaleType = "VOLUME";
      }
   }

   std::string scaleType = "VOLUME";  // Default to VOLUME
   bool hasWebInput = false;
};

#endif // INPUTHANDLER_CMD_SCALE_FEATURES_H