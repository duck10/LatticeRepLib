// CmdSellaControls.h
#ifndef INPUTHANDLER_CMD_SELLA_FEATURES_H
#define INPUTHANDLER_CMD_SELLA_FEATURES_H

#include "BaseControlVariables.h"
#include "BlockUtils.h"
#include "InputHandler.h"
#include "LRL_StringTools.h"

#include <sstream>

class CmdSellaControls : public BaseControlVariables {
public:

   friend std::ostream& operator<< (std::ostream& os, const CmdSellaControls& csc) {
      os << "; CmdSellaControls\n";
      os << ";   file prefix " << csc.prefix << std::endl;
      os << ";   blockstart " << csc.blockstart << std::endl;
      os << ";   blocksize  " << csc.blocksize << std::endl;
      os << ";   from the web " << ((csc.webRun) ? "true" : "false");
      return os;
   }

   CmdSellaControls() {

   InputHandler::registerHandler("BLOCKSIZE", 0.34,
      [this](const BaseControlVariables&, const std::string& value) {
         setBlockSize(std::stoul(value));
      });

   InputHandler::registerHandler("BLOCKSTART", 0.35,
      [this](const BaseControlVariables&, const std::string& value) {
         setBlockStart(std::stoul(value));
      });

   InputHandler::registerHandler("DOGRAPHICS", .5,
      [this](BaseControlVariables&, const std::string& value) {
         doGraphics = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
      }
   );

   }

   int getBlockSize() const { return  static_cast<int>(blocksize); }
   int getBlockStart() const { return static_cast<int>(blockstart); }
   std::string getPrefix() const { return prefix; }
   bool DoGraphics() const { return doGraphics; }
private:

   void setBlockSize(int size) {
      long long val = static_cast<long long>(size);
      if (val <= 0) {
         std::cerr << ";Warning: Blocksize must be positive, using "
            << DEFAULT_BLOCKSIZE << std::endl;
         blocksize = DEFAULT_BLOCKSIZE;
      }
      else if (webRun && val > MAX_BLOCKSIZE) {
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
   static constexpr size_t MAX_BLOCKSIZE = BlockUtils::MAX_BLOCKSIZE/2; // divided by 2 because there are 
   // two output files !!!!!!!!!!!!!!!!!!
   static constexpr size_t DEFAULT_BLOCKSIZE = MAX_BLOCKSIZE;

   size_t blockstart = BlockUtils::MIN_BLOCKSTART;
   size_t blocksize = MAX_BLOCKSIZE;

   // File prefix member
   const std::string prefix = "SEL";
   bool doGraphics = true;


};

#endif // INPUTHANDLER_CMD_SELLA_FEATURES_H
