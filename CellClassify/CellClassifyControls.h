#ifndef CELLCLASSIFY_CONTROLS_H
#define CELLCLASSIFY_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"
#include "LRL_StringTools.h"

#include <iostream>
#include <ostream>
#include <sstream>
#include <vector>

class CellClassifyControls : public BaseControlVariables {
public:

   friend std::ostream& operator<< (std::ostream& os, const CellClassifyControls& ccc) {
      os << ";CellClassify controls\n";
      os << ";samples retained per key (NMAX): " << ccc.nmax << std::endl;
      os << ";near-tie detection threshold (TIETOL): " << ccc.tieTol << std::endl;
      return os;
   }

   CellClassifyControls() {

      InputHandler::registerHandler("NMAX", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            const int n = std::stoi(value);
            if (n < 1) {
               std::cout << ";Warning: NMAX must be positive, using "
                  << DEFAULT_NMAX << std::endl;
               nmax = DEFAULT_NMAX;
            }
            else {
               nmax = static_cast<size_t>(n);
            }
         });

      InputHandler::registerHandler("TIETOL", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            const double t = std::stod(value);
            if (t <= 0.0) {
               std::cout << ";Warning: TIETOL must be positive, using "
                  << DEFAULT_TIETOL << std::endl;
               tieTol = DEFAULT_TIETOL;
            }
            else {
               tieTol = t;
            }
         });

      // TEST -- runs CellClassify's own regression suite (currently: the
      // 16-cell historical set, checking totalProcessed/totalFailed and
      // the failure-signature store's contents against known answers)
      // instead of normal batch processing. Copied verbatim from
      // CmdSellaControls.h's TEST handler -- matching CmdLMP3's
      // TEST/getTestNumbers() convention, same as CmdSella already does,
      // so the same TEST / TEST 1 / TEST 1 3 5 / TEST 999 vocabulary
      // works identically in both tools.
      //
      //   TEST            -- run the whole suite (testNumbers left empty)
      //   TEST 1          -- run only test 1
      //   TEST 1 3 5      -- run tests 1, 3, and 5 (whitespace-separated)
      //   TEST 999        -- list available tests, run nothing
      // "1" and "TRUE" are kept as bare on/off synonyms (run-all) for
      // backward compatibility with the original boolean-only control.
      InputHandler::registerHandler("TEST", .35,
         [this](BaseControlVariables&, const std::string& value) {
            testMode = true;
            testNumbers.clear();
            const std::string upper = LRL_StringTools::strToupper(value);
            if (value.empty() || value == "1" || upper == "TRUE") {
               return; // run-all: testNumbers stays empty
            }
            if (upper == "0" || upper == "FALSE") {
               testMode = false;
               return;
            }
            std::istringstream istr(value);
            int n;
            while (istr >> n) testNumbers.push_back(n);
         }
      );
      // DETAILS -- per-cell diagnostic printing (cell string, every
      // filtered type's raw fit, best/second/tie status, failure
      // signature if any), OFF by default since CellClassify's whole
      // design point is bounded output regardless of N. Explicit opt-in
      // for exactly the kind of investigation that motivated adding this:
      // seeing per-cell fits directly, rather than needing a separate
      // CmdSella `details` round-trip on cells suspected of anything
      // wrong. Same bare on/off vocabulary as TEST's run-all synonyms.
      InputHandler::registerHandler("DETAILS", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            const std::string upper = LRL_StringTools::strToupper(value);
            showDetails = value.empty() || value == "1" || upper == "TRUE";
         });
   }

   size_t getNmax() const { return nmax; }
   double getTieTol() const { return tieTol; }
   bool shouldRunTests() const { return testMode; }
   std::vector<int> getTestNumbers() const { return testNumbers; }
   bool shouldShowDetails() const { return showDetails; }

private:
   // DEFAULT_NMAX=3 per this session's discussion -- keeps output readable
   // regardless of how many cells are run (1000 or 1,000,000): only the
   // number of DISTINCT keys that occur controls output size, and this
   // caps how many example cells are kept per key. StoreResults itself
   // has no maximum -- m_countPerKey keeps an exact unbounded tally
   // regardless of NMAX; this only caps sample retention/display.
   static constexpr size_t DEFAULT_NMAX = 3;
   // DEFAULT_TIETOL is a placeholder, not a judged value -- what counts
   // as "close enough" between two Delone types' normalized raw fits to
   // log as a near-tie candidate is a real physical judgment call this
   // session deferred. Left controllable (TIETOL keyword) rather than
   // hardcoded so that judgment can be made per-run without a rebuild.
   // Units: raw fit / lattice norm, i.e. |normalizedFit_1st -
   // normalizedFit_2nd| < TIETOL for the two smallest-fit types.
   static constexpr double DEFAULT_TIETOL = 0.01;

   size_t nmax = DEFAULT_NMAX;
   double tieTol = DEFAULT_TIETOL;
   bool testMode = false;
   std::vector<int> testNumbers;
   bool showDetails = false;
};
#endif // CELLCLASSIFY_CONTROLS_H
