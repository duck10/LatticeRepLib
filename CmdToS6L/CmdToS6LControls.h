#ifndef INPUTHANDLER_CMD_TO_S6L_FEATURES_H
#define INPUTHANDLER_CMD_TO_S6L_FEATURES_H

#include "BaseControlVariables.h"
#include "BlockUtils.h"
#include "InputHandler.h"
#include "LRL_StringTools.h"

#include <sstream>
#include <string>

class CmdToS6LControlVariables : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CmdToS6LControlVariables& ctrl) {
      os << "; CmdToS6LControlVariables\n";
      os << ";   format     " << ctrl.format << std::endl;
      os << ";   from the web " << ((ctrl.getHasWebInput()) ? "true" : "false");
      return os;
   }

   CmdToS6LControlVariables() {


      // Register handler for FORMAT command
      InputHandler::registerHandler("FORMAT", 0.35,
         [this](const BaseControlVariables&, const std::string& value) {
            format = LRL_StringTools::strToupper(value);
            if (format.empty()) format = "RS"; // Default to both if empty
            else if (format.find('R') == std::string::npos && format.find('S') == std::string::npos) {
               std::cout << ";Warning: Invalid format. Using default 'RS'" << std::endl;
               format = "RS";
            }
         });

      // Register handler for FORMAT command
      InputHandler::registerHandler("TYPE", 0.35,
         [this](const BaseControlVariables&, const std::string& value) {
            format = LRL_StringTools::strToupper(value);
            if (format.empty()) format = "RS"; // Default to both if empty
            else if (format.find('R') == std::string::npos && format.find('S') == std::string::npos) {
               std::cout << ";Warning: Invalid format. Using default 'RS'" << std::endl;
               format = "RS";
            }
         });

      // Also register handler for ROOT and LINEAR commands for compatibility
      InputHandler::registerHandler("ROOT", 0.35,
         [this](const BaseControlVariables&, const std::string& value) {
            format = "R";
         });

      InputHandler::registerHandler("LINEAR", 0.35,
         [this](const BaseControlVariables&, const std::string& value) {
            format = "S";
         });
   }

   bool hasRoot() const { return format.find('R') != std::string::npos; }
   bool hasLinear() const { return format.find('S') != std::string::npos; }
   void setFormat(const std::string& s) { format = s; }
   std::string getFormat() const { return format; }

private:
   std::string prefix = "S6L";
   std::string format = "RS";  // Default to both Root and Linear
};

#endif // INPUTHANDLER_CMD_TO_S6L_FEATURES_H