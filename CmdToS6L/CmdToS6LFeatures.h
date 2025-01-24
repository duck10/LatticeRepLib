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
      os << ";   file prefix " << ctrl.prefix << std::endl;
      os << ";   blockstart " << ctrl.blockstart << std::endl;
      os << ";   blocksize  " << ctrl.blocksize << std::endl;
      os << ";   format     " << ctrl.format << std::endl;
      os << ";   from the web " << ((ctrl.hasWebInput) ? "true" : "false");
      return os;
   }

   CmdToS6LControlVariables() {
      InputHandler::registerHandler("BLOCKSIZE", 0.2,
         [this](const BaseControlVariables&, const std::string& value) {
            setBlockSize(std::stoul(value));
         });

      InputHandler::registerHandler("BLOCKSTART", 0.2,
         [this](const BaseControlVariables&, const std::string& value) {
            setBlockStart(std::stoul(value));
         });

      InputHandler::registerHandler("PREFIX", 0.35,
         [this](const BaseControlVariables&, const std::string& value) {
            setPrefix(value);
         });

      InputHandler::registerHandler("SHOW", .5,
         [this](BaseControlVariables&, const std::string& value) {
            showControls = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         }
      );

      // Register handler for FORMAT command
      InputHandler::registerHandler("FORMAT", 0.35,
         [this](const BaseControlVariables&, const std::string& value) {
            format = LRL_StringTools::strToupper(value);
            if (format.empty()) format = "RS"; // Default to both if empty
            else if (format.find('R') == std::string::npos && format.find('S') == std::string::npos) {
               std::cerr << ";Warning: Invalid format. Using default 'RS'" << std::endl;
               format = "RS";
            }
         });

      // Also register handler for ROOT and LINEAR commands for compatibility
      InputHandler::registerHandler("ROOT", 0.35,
         [this](const BaseControlVariables&, const std::string& value) {
            if (format.find('S') != std::string::npos) format = "RS";
            else format = "R";
         });

      InputHandler::registerHandler("LINEAR", 0.35,
         [this](const BaseControlVariables&, const std::string& value) {
            if (format.find('R') != std::string::npos) format = "RS";
            else format = "S";
         });
   }

   int getBlockSize() const { return static_cast<int>(blocksize); }
   int getBlockStart() const { return static_cast<int>(blockstart); }
   std::string getPrefix() const { return prefix; }
   bool getHasWebInput() const { return hasWebInput; }
   void setWebRun(bool webInput) { hasWebInput = webInput; }
   bool hasRoot() const { return format.find('R') != std::string::npos; }
   bool hasLinear() const { return format.find('S') != std::string::npos; }

private:
   void setPrefix(const std::string& newPrefix) {
      prefix = newPrefix;
   }

   void setBlockSize(int size) {
      long long val = static_cast<long long>(size);
      if (val <= 0) {
         std::cerr << ";Warning: Blocksize must be positive, using "
            << DEFAULT_BLOCKSIZE << std::endl;
         blocksize = DEFAULT_BLOCKSIZE;
      }
      else if (hasWebInput && val > MAX_BLOCKSIZE) {
         std::cerr << ";Warning: Blocksize exceeds web limit, using "
            << MAX_BLOCKSIZE << std::endl;
         blocksize = MAX_BLOCKSIZE;
      }
      else {
         blocksize = static_cast<size_t>(val);
      }
   }

   void setBlockStart(int start) {
      long long val = static_cast<long long>(start);
      if (val < 0) {
         std::cerr << ";Warning: Blockstart cannot be negative, using 0" << std::endl;
         blockstart = 0;
      }
      else {
         blockstart = start;
      }
   }

   static constexpr size_t MIN_BLOCKSIZE = BlockUtils::MIN_BLOCKSIZE;
   static constexpr size_t MAX_BLOCKSIZE = BlockUtils::MAX_BLOCKSIZE;
   static constexpr size_t DEFAULT_BLOCKSIZE = BlockUtils::MAX_BLOCKSIZE;

   size_t blockstart = BlockUtils::MIN_BLOCKSTART;
   size_t blocksize = BlockUtils::DEFAULT_BLOCKSIZE;
   std::string prefix = "S6L";
   std::string format = "RS";  // Default to both Root and Linear
   bool hasWebInput = false;
};

#endif // INPUTHANDLER_CMD_TO_S6L_FEATURES_H