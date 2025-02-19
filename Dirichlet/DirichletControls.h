// DirichletControls.h
#ifndef INPUTHANDLER_DIRICHLET_FEATURES_H
#define INPUTHANDLER_DIRICHLET_FEATURES_H

#include "BaseControlVariables.h"
#include "BlockUtils.h"
#include "InputHandler.h"
#include "LRL_StringTools.h"

#include <sstream>

class DirichletControls : public BaseControlVariables {
public:

   friend std::ostream& operator<< (std::ostream& os, const DirichletControls& csc) {
      os << "; DirichletControls\n";
      os << ";   file prefix " << csc.prefix << std::endl;
      os << ";   blockstart " << csc.blockstart << std::endl;
      os << ";   blocksize  " << csc.blocksize << std::endl;
      os << ";   from the web " << ((csc.webRun) ? "true" : "false");
      return os;
   }

   DirichletControls() {

      InputHandler::registerHandler("BLOCKSIZE", 0.35,
         [this](const BaseControlVariables&, const std::string& value) {
            setBlockSize(std::stoul(value));
         });

      InputHandler::registerHandler("BLOCKSTART", 0.35,
         [this](const BaseControlVariables&, const std::string& value) {
            setBlockStart(std::stoul(value));
         });

   }

   int getBlockSize() const { return  static_cast<int>(blocksize); }
   int getBlockStart() const { return static_cast<int>(blockstart); }
   std::string getPrefix() const { return prefix; }

private:

   void setBlockSize(int size) {
      long long val = static_cast<long long>(size);
      if (val <= 0) {
         std::cout << ";Warning: Blocksize must be positive, using "
            << DEFAULT_BLOCKSIZE << std::endl;
         blocksize = DEFAULT_BLOCKSIZE;
      }
      else if (webRun && val > MAX_BLOCKSIZE) {
         std::cout << ";Warning: Blocksize exceeds web limit, using "
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
         std::cout << ";Warning: Blockstart cannot be negative, using 0" << std::endl;
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

   // File prefix member
   const std::string prefix = "DIR";

};

#endif // INPUTHANDLER_DIRICHLET_FEATURES_H
