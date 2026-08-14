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
      os << ";   from the web " << ((csc.webRun) ? "true" : "false") << std::endl;
      os << ";   test mode " << ((csc.testMode) ? "true" : "false") << std::endl;
      os << ";   compare mode " << ((csc.compareMode) ? "true" : "false");
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


      InputHandler::registerHandler("BUILD", .35,
         [this](BaseControlVariables&, const std::string& value) {
            buildMatrices = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         }
      );

      InputHandler::registerHandler("DETAILS", .35,
         [this](BaseControlVariables&, const std::string& value) {
            debug = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         }
      );
      InputHandler::registerHandler("DEBUG", .35,
         [this](BaseControlVariables&, const std::string& value) {
            debug = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         }
      );

      InputHandler::registerHandler("PRINTMATRICES", .35,
         [this](BaseControlVariables&, const std::string& value) {
            printMatrices = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         }
      );

      // TEST -- runs the regression test suite (Grimmer-chain invariant checks,
      // reflection-orbit completeness, Remediation before/after fixtures, etc.)
      // instead of / in addition to normal single-cell processing.
      // TEST alone runs everything; TEST N runs only test number N (matching
      // CmdLMP3's TEST/TEST-N convention).
      InputHandler::registerHandler("TEST", .35,
         [this](BaseControlVariables&, const std::string& value) {
            testMode = true;
            testNumber = value.empty() ? 0 : std::stoi(value); // 0 = run all
         }
      );

      // COMPARE -- accumulates DeloneFitResults across all input cells in this
      // run (rather than printing each cell's fits immediately/independently)
      // and prints one summary table at the end: candidate types down the
      // rows, labeled input cells across the columns. Intended for exactly the
      // O1B/O2/O3-style side-by-side comparisons.
      InputHandler::registerHandler("COMPARE", .35,
         [this](BaseControlVariables&, const std::string& value) {
            compareMode = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         }
      );

      // SHOWCHAINFAILURES -- prints Grimmer-chain violation detail (which
      // chain, which types, before/after remediation) for each input cell.
      // Split out from DEBUG/DETAILS so chain-failure visibility doesn't
      // require turning on all of DEBUG's other (much noisier) output.
      InputHandler::registerHandler("SHOWCHAINFAILURES", .35,
         [this](BaseControlVariables&, const std::string& value) {
            showChainFailures = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         }
      );

   }

   int getBlockSize() const { return  static_cast<int>(blocksize); }
   int getBlockStart() const { return static_cast<int>(blockstart); }
   std::string getPrefix() const { return prefix; }
   bool DoGraphics() const { return doGraphics; }
   bool shouldBuildMatrices() const { return buildMatrices; }
   bool shouldDebug() const { return debug; }
   bool shouldPrintMatrices() const { return printMatrices; }
   bool shouldRunTests() const { return testMode; }
   int getTestNumber() const { return testNumber; } // 0 = run all
   bool shouldCompare() const { return compareMode; }
   bool shouldShowChainFailures() const { return showChainFailures; }

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
   static constexpr size_t MAX_BLOCKSIZE = BlockUtils::MAX_BLOCKSIZE / 2; // divided by 2 because there are 
   // two output files !!!!!!!!!!!!!!!!!!
   static constexpr size_t DEFAULT_BLOCKSIZE = MAX_BLOCKSIZE;

   size_t blockstart = BlockUtils::MIN_BLOCKSTART;
   size_t blocksize = MAX_BLOCKSIZE;

   // File prefix member
   const std::string prefix = "SEL";
   bool doGraphics = true;

   bool buildMatrices = false;
   bool printMatrices = false;
   bool debug = false;
   bool testMode = false;
   int testNumber = 0; // 0 = run all
   bool compareMode = false;
   bool showChainFailures = false;
};

#endif // INPUTHANDLER_CMD_SELLA_FEATURES_H
