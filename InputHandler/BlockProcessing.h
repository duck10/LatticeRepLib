#ifndef INPUTHANDLER_BLOCK_PROCESSING_H
#define INPUTHANDLER_BLOCK_PROCESSING_H

#include "ControlFeature.h"
#include "InputHandler.h"

#include "functional"
#include <string>
#include <sstream>
#include <iostream>

class BlockProcessing : public ControlFeature {
public:
   static constexpr size_t MIN_BLOCKSIZE = 1;
   static constexpr size_t MAX_BLOCKSIZE = 20;
   static constexpr size_t DEFAULT_BLOCKSIZE = MAX_BLOCKSIZE;

   BlockProcessing()
      : blockstart(0)
      , blocksize(DEFAULT_BLOCKSIZE)
      , webRun(false) {

      InputHandler::registerHandler("BLOCKSIZE", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            setBlockSize(std::stoll(value));
         });

      InputHandler::registerHandler("BLOCKSTART", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            setBlockStart(std::stoll(value));
         });
   }

   bool handleInput(const std::string& command, const std::string& value) override {
      try {
         if (command == "BLOCKSIZE") {
            long long val = std::stoll(value);
            if (val < 1) {
               std::cerr << ";Warning: Blocksize must be at least 1, using 1" << std::endl;
               blocksize = 1;
            }
            else if (webRun && val > MAX_BLOCKSIZE) {
               std::cerr << ";Warning: Blocksize exceeds web limit, using "
                  << MAX_BLOCKSIZE << std::endl;
               blocksize = MAX_BLOCKSIZE;
            }
            else {
               blocksize = static_cast<size_t>(val);
            }
            return true;
         }
         else if (command == "BLOCKSTART") {
            long long val = std::stoll(value);
            if (val < 0) {
               std::cerr << ";Warning: Blockstart cannot be negative, using 0" << std::endl;
               blockstart = 0;
            }
            else {
               blockstart = static_cast<size_t>(val);
            }
            return true;
         }
      }
      catch (const std::exception&) {
         std::cerr << ";Warning: Invalid numeric value '" << value << "'" << std::endl;
      }
      return false;
   }

   std::string getFeatureState() const override {
      std::ostringstream oss;
      oss << ";Blockstart: " << blockstart << "\n"
         << ";Blocksize: " << blocksize << "\n";
      return oss.str();
   }

   void writeToStream(std::ostream& os) const override {
      os << getFeatureState();
   }

   size_t getBlockStart() const { return blockstart; }
   size_t getBlockSize() const { return blocksize; }
   void setWebRun(bool web) { webRun = web; }

   void updateLimits(size_t totalFiles, bool hasWebInstructions,
      std::function<size_t(size_t)> getTotalFiles) {
      size_t totalOutputFiles = getTotalFiles(totalFiles);
      if (blocksize > totalOutputFiles) {
         blocksize = totalOutputFiles;
      }
   }

   void setBlockSize(size_t size) {
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

   void setBlockStart(size_t start) {
      long long val = static_cast<long long>(start);
      if (val < 0) {
         std::cerr << ";Warning: Blockstart cannot be negative, using 0" << std::endl;
         blockstart = 0;
      }
      else {
         blockstart = start;
      }
   }

private:
   size_t blockstart;
   size_t blocksize;
   bool webRun;
};

#endif // INPUTHANDLER_BLOCK_PROCESSING_H
