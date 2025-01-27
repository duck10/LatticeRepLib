#ifndef CMD_PATH_CONTROLS_H
#define CMD_PATH_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"
#include "LRL_StringTools.h"

#include <sstream>
#include <string>

class CmdPathControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CmdPathControls& ctrl) {
      os << "; CmdPathControls\n";
      os << ";   path length " << ctrl.pathLength << std::endl;
      return os;
   }

   CmdPathControls(int commandLinePathLength = 20) : pathLength(commandLinePathLength) {
      InputHandler::registerHandler("SHOW", .5,
         [this](BaseControlVariables&, const std::string& value) {
            showControls = (value == "1" || value == "TRUE" || value.empty());
         }
      );

      InputHandler::registerHandler("LENGTH", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            setPathLength(std::stoi(value));
         }
      );

      InputHandler::registerHandler("POINTS", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            setPathLength(std::stoi(value));
         }
      );

      InputHandler::registerHandler("COUNT", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            setPathLength(std::stoi(value));
         }
      );
   }

   int getPathLength() const { return pathLength; }
   bool shouldShowControls() const { return showControls; }

private:
   void setPathLength(int length) {
      if (length <= 0) {
         std::cerr << ";Warning: Path length must be positive, using "
            << DEFAULT_PATH_LENGTH << std::endl;
         pathLength = DEFAULT_PATH_LENGTH;
      }
      else {
         pathLength = length;
      }
   }

   static constexpr int DEFAULT_PATH_LENGTH = 20;
   int pathLength;
};

#endif // CMD_PATH_CONTROLS_H