// CmdSellaControls.h
#ifndef INPUTHANDLER_CMD_SELLA_FEATURES_H
#define INPUTHANDLER_CMD_SELLA_FEATURES_H

#include "BaseControlVariables.h"
#include "BlockUtils.h"
#include "InputHandler.h"
#include "LRL_StringTools.h"
#include "S6.h"

#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

class CmdSellaControls : public BaseControlVariables {
public:

   friend std::ostream& operator<< (std::ostream& os, const CmdSellaControls& csc) {
      os << "; CmdSellaControls\n";
      os << ";   file prefix " << csc.prefix << std::endl;
      os << ";   blockstart " << csc.blockstart << std::endl;
      os << ";   blocksize  " << ((csc.blocksize == std::numeric_limits<size_t>::max())
         ? std::string("all") : std::to_string(csc.blocksize)) << std::endl;
      os << ";   from the web " << ((csc.webRun) ? "true" : "false") << std::endl;
      os << ";   test mode " << ((csc.testMode) ? "true" : "false") << std::endl;
      os << ";   compare mode " << ((csc.compareMode) ? "true" : "false") << std::endl;
      os << ";   max delta for match " << csc.maxDeltaForMatch << std::endl;
      os << ";   nesting delta " << csc.nestingDelta << std::endl;
      os << ";   max boundary walk cycles " << csc.maxBoundaryWalkCycles << std::endl;
      os << ";   check nesting " << ((csc.checkNesting) ? "true" : "false") << std::endl;
      os << ";   requested types ";
      if (csc.requestedTypes.empty()) os << "(all)";
      else for (const auto& t : csc.requestedTypes) os << t << " ";
      os << std::endl;
      os << ";   seed boundaries mode " << (csc.seedBoundariesMode.empty() ? "(off)" : csc.seedBoundariesMode) << std::endl;
      os << ";   test transforms " << ((csc.testTransforms) ? "true" : "false") << std::endl;
      os << ";   random seed " << csc.getRandomSeed() << std::endl;

      os << ";   face crossed " << ((csc.faceCrossed) ? "true" : "false") << std::endl;
      os << ";   remediation " << ((csc.remediation) ? "true" : "false") << std::endl;
      os << ";   cone fit " << ((csc.coneFit) ? "true" : "false") << std::endl;
      os << ";   check remediation " << ((csc.checkRemediation) ? "true" : "false") << std::endl;
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

      // TEST -- runs the regression test suite (Grimmer-chain invariant checks,
      // reflection-orbit completeness, Remediation before/after fixtures, etc.)
      // instead of / in addition to normal single-cell processing.
      //
      // Accepts, matching CmdLMP3's TEST/getTestNumbers() convention:
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
            if (value.empty() || value == "0" || upper == "TRUE") {
               return; // run-all: testNumbers stays empty
            }
            if (upper == "FALSE") {
               testMode = false;
               return;
            }
            std::istringstream istr(value);
            int n;
            while (istr >> n) testNumbers.push_back(n);
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

      // MAXDELTA -- relative-fit-vs-lattice-norm threshold used by
      // ListMatchingTypes to decide whether a candidate type counts as a
      // match at all. Was the file-scope global g_maxDeltaForMatch in
      // CmdSella.cpp (default 0.02); now settable so it doesn't require a
      // recompile to try a different value.
      InputHandler::registerHandler("MAXDELTA", .35,
         [this](BaseControlVariables&, const std::string& value) {
            setMaxDeltaForMatch(std::stod(value));
         }
      );

      // NESTINGDELTA -- relative-to-lattice-norm tolerance for the
      // NestingInequalityExpectation check (fit(parentType) <=
      // fit(childType) + NESTINGDELTA * LatI.norm()), e.g. the O1B/O2/O3
      // stage-1 check from SELLA_handoff.md item 1. Deliberately a separate,
      // much tighter control from MAXDELTA -- see the epsilon-design
      // discussion this session for why a fixed/shared value is wrong: this
      // check asserts something that should hold near-exactly, not "is this
      // plausibly a match."
      InputHandler::registerHandler("NESTINGDELTA", .35,
         [this](BaseControlVariables&, const std::string& value) {
            setNestingDelta(std::stod(value));
         }
      );

      // CHECKNESTING -- runs the general, name-free nesting-consistency
      // check (RunNestingConsistencyCheck in CmdSella.cpp) instead of
      // normal cell processing: for every pair of Delone types (P, C)
      // where C has strictly fewer free parameters than P, tests via
      // TestDeloneParentage whether C genuinely nests inside P, and for
      // every confirmed pair checks fit(P) <= fit(C) + NESTINGDELTA with 
      // the full fit pipeline.
      // This is the generalized form of the O1B/O2/O3 diagnostic that
      // started this investigation -- covers all 24 Delone types, not
      // just the three named degenerate boundary types (O3, M2B, M3), and
      // needs no hand-picked list. Off by default; a diagnostic mode, not
      // part of normal use.
      InputHandler::registerHandler("CHECKNESTING", .35,
         [this](BaseControlVariables&, const std::string& value) {
            checkNesting = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         }
      );

      // TYPES -- restricts the "Fits for Types" and COMPARE printouts to a
      // chosen subset of Delone type names (e.g. "TYPES O1B O2 O3"),
      // rather than always showing all 24. Motivated directly by
      // generate-then-inspect workflows like `cmdgen 10 O3 | cmdsella`,
      // where scanning past 21 irrelevant lines per input to compare two
      // or three types of interest gets unwieldy fast. Empty (the
      // default) means show everything, same convention as TEST's
      // empty-means-all. Filtering happens at PRINT time only -- every
      // type's fit is still computed exactly as before (Grimmer-chain
      // logic elsewhere depends on having all 24), only which ones get
      // displayed changes, so this cannot affect any existing behavior
      // beyond what's shown on screen.
      InputHandler::registerHandler("TYPES", .35,
         [this](BaseControlVariables&, const std::string& value) {
            requestedTypes.clear();
            std::istringstream istr(value);
            std::string token;
            while (istr >> token) requestedTypes.push_back(token);
         }
      );

      InputHandler::registerHandler("SIGMAS", .35,
         [this](BaseControlVariables&, const std::string& value) {
            sigmas.clear();
            std::istringstream istr(value);
            std::string token;
            while (istr >> token) sigmas.push_back(std::stod(token));
         }

      );

      InputHandler::registerHandler("PRECISION", .35,
         [this](BaseControlVariables&, const std::string& value) {
            std::istringstream istr(value);
            int n = 0;
            istr >> n;
            outputPrecision = (n < 0) ? 0 : n;
         });


      // FACECROSSED -- on (default): adds the 143 face arrangements (SELLA
      // white paper v5, Appendix B) for 11 Delone types to the projector
      // lists used for fitting, each borrowed from a more symmetric type
      // (e.g. O2 gets three of O3's). These cover reduced forms of special
      // lattices on cone faces that the type's own list misses, so nesting
      // such as fit(O2) <= fit(O3) holds by construction. Off: the type's
      // own list only, for comparison (e.g. COMPARE with FACECROSSED 0 vs 1).
      InputHandler::registerHandler("FACECROSSED", .35,
         [this](BaseControlVariables&, const std::string& value) {
            faceCrossed = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         }
      );

      // REMEDIATION -- on (default): every type's fit gets the full
      // Remediation168 search (boundary-transform and reflection candidates,
      // iterated, cross-seeded; DeloneFitComputation.cpp). Off: each type's
      // fit is the direct fit of the reduced cell, SellaFitXXXXXX with its
      // projector list (plus the face arrangements when FACECROSSED is on).
      // Added 2026-11-06 to test whether Remediation is still needed now
      // that the face arrangements are in place.
      InputHandler::registerHandler("REMEDIATION", .35,
         [this](BaseControlVariables&, const std::string& value) {
            remediation = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         }
      );

      // CONEFIT -- on (default): Sella::SellaFitXXXXXX clamps each projection
      // to the reduced cone (each letter group gets min(0, group mean), the
      // white paper's constrained projection) before taking the distance.
      // Changes nothing for reduced input; corrects Remediation's
      // candidates outside the cone, which could otherwise report fits
      // that are too small (CHECKREMEDIATION "ARTIFACT" lines, 2026-11-06).
      // Off: the plain subspace projection, as before.
      InputHandler::registerHandler("CONEFIT", .35,
         [this](BaseControlVariables&, const std::string& value) {
            coneFit = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         }
      );

      // CHECKREMEDIATION -- diagnostic (RunRemediationCheck,
      // DeloneFitComputation.cpp): for every input cell and every type,
      // computes the direct fit and the remediated fit, and reports each
      // type where Remediation found a lower fit, with the winning
      // candidate's largest scalar (> 0 means outside the reduced cone) and
      // its cone fit (projection with each group value clamped to <= 0).
      // Prints a summary and exits. Off by default.
      InputHandler::registerHandler("CHECKREMEDIATION", .35,
         [this](BaseControlVariables&, const std::string& value) {
            checkRemediation = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         }
      );

      // SEED / RANDOMSEED moved to BaseControlVariables.h (2026-08-29) --
      // registered there instead of here so every program built on this
      // framework gets it, not just CmdSella. randomSeed/getRandomSeed()
      // are now inherited from the base class too.
   }

   // size_t, not int (Oct 2026): the unlimited default is the largest size_t,
   // which would become -1 if cast to int.
   size_t getBlockSize() const { return blocksize; }
   size_t getBlockStart() const { return blockstart; }
   std::string getPrefix() const { return prefix; }
   bool DoGraphics() const { return doGraphics; }
   bool shouldDebug() const { return debug; }
   bool shouldPrintMatrices() const { return printMatrices; }
   bool shouldRunTests() const { return testMode; }
   // Matches FlexibleTestMode.h's MultiTransformFinderControls convention.
   // Empty vector means "run all" (or list, if 999 is present).
   std::vector<int> getTestNumbers() const { return testNumbers; }
   bool shouldCompare() const { return compareMode; }
   double getMaxDeltaForMatch() const { return maxDeltaForMatch; }
   double getNestingDelta() const { return nestingDelta; }
   size_t getMaxBoundaryWalkCycles() const { return maxBoundaryWalkCycles; }
   bool shouldCheckNesting() const { return checkNesting; }
   std::vector<std::string> getRequestedTypes() const { return requestedTypes; }
   // true if this type should be shown -- either nothing was requested
   // (show everything, the default) or this specific type was.
   bool shouldShowType(const std::string& generalType) const {
      if (requestedTypes.empty()) return true;
      for (const auto& t : requestedTypes) if (t == generalType) return true;
      return false;
   }
   std::string getSeedBoundariesMode() const { return seedBoundariesMode; }

   bool shouldTestTransforms() const { return testTransforms; };

   int getOutputPrecision() const { return outputPrecision; }
   bool useExtendedPrecision() const { return outputPrecision > 0; }
   bool shouldUseFaceCrossed() const { return faceCrossed; }
   bool shouldRemediate() const { return remediation; }
   bool shouldUseConeFit() const { return coneFit; }
   bool shouldCheckRemediation() const { return checkRemediation; }

private:

   // The web limit is no longer applied here (Oct 2026): WebIO::ApplyBlockLimits
   // enforces it in CmdSella.cpp, counting the files actually written per cell.
   void setBlockSize(int size) {
      long long val = static_cast<long long>(size);
      if (val <= 0) {
         std::cerr << ";Warning: Blocksize must be positive, using all" << std::endl;
         blocksize = std::numeric_limits<size_t>::max();
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

   void setMaxDeltaForMatch(const double d) {
      if (d <= 0.0) {
         std::cerr << ";Warning: MAXDELTA must be positive, using "
            << maxDeltaForMatch << std::endl;
      }
      else {
         maxDeltaForMatch = d;
      }
   }

   void setNestingDelta(const double d) {
      if (d <= 0.0) {
         std::cerr << ";Warning: NESTINGDELTA must be positive, using "
            << nestingDelta << std::endl;
      }
      else {
         nestingDelta = d;
      }
   }

   // Default block size: no limit (Oct 2026). Previously MAX_BLOCKSIZE / 2
   // (= 10, "because there are two output files" -- there are actually
   // three per cell), which also silently limited desktop runs to 10 cells.
   // Web runs are now capped in CmdSella.cpp by WebIO::ApplyBlockLimits,
   // which counts the three files written per cell.
   size_t blockstart = BlockUtils::MIN_BLOCKSTART;
   size_t blocksize = std::numeric_limits<size_t>::max();

   // File prefix member
   const std::string prefix = "SEL";
   bool doGraphics = true;

   bool buildMatrices = false;
   bool printMatrices = false;
   bool debug = false;
   bool testMode = false;
   std::vector<int> testNumbers; // empty == run all (see TEST handler above)
   bool compareMode = false;
   bool showChainFailures = false;

   // Default 0.02 matches the value the old file-scope global
   // g_maxDeltaForMatch had in CmdSella.cpp -- unchanged behavior until
   // MAXDELTA is actually set.
   double maxDeltaForMatch = 0.02;

   // Default is a placeholder, not calibrated -- see this session's
   // epsilon-design discussion. Needs checking against real fixture data
   // (known-good non-boundary gaps vs the O1B/O2/O3 bug-scale gap) before
   // trusting it for real NestingInequalityExpectation fixtures.
   double nestingDelta = 1.0E-4;

   // Default "tens" per instruction -- adjust once real warning counts are
   // seen against actual boundary types.
   size_t maxBoundaryWalkCycles = 50;
   bool checkNesting = false;
   bool testTransforms = false;
   std::vector<std::string> requestedTypes;  // empty == show all (see TYPES handler above)
   std::vector<double> sigmas;
   std::string seedBoundariesMode;  // "", "PRINT", or "USE" (see SEEDBOUNDARIES handler above)

   int outputPrecision = 0;   // 0 = current formatting
   bool faceCrossed = true;  // see FACECROSSED handler above
   bool remediation = true;  // see REMEDIATION handler above
   bool coneFit = true;  // see CONEFIT handler above
   bool checkRemediation = false;  // see CHECKREMEDIATION handler above
};

#endif // INPUTHANDLER_CMD_SELLA_FEATURES_H
