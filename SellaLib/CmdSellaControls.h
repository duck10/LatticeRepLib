// CmdSellaControls.h
#ifndef INPUTHANDLER_CMD_SELLA_FEATURES_H
#define INPUTHANDLER_CMD_SELLA_FEATURES_H

#include "BaseControlVariables.h"
#include "BlockUtils.h"
#include "InputHandler.h"
#include "LRL_StringTools.h"
#include "S6.h"

#include <sstream>
#include <vector>

class CmdSellaControls : public BaseControlVariables {
public:

   friend std::ostream& operator<< (std::ostream& os, const CmdSellaControls& csc) {
      os << "; CmdSellaControls\n";
      os << ";   file prefix " << csc.prefix << std::endl;
      os << ";   blockstart " << csc.blockstart << std::endl;
      os << ";   blocksize  " << csc.blocksize << std::endl;
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
      os << ";   seed boundaries mode " << (csc.seedBoundariesMode.empty() ? "(off)" : csc.seedBoundariesMode);
      os << ";   test transforms " << ((csc.testTransforms) ? "true" : "false") << std::endl;
      os << ";   sequence spec " << (csc.sequenceSpec.empty() ? "(off)" : csc.sequenceSpec) << std::endl;
      os << ";   best-of spec " << (csc.bestOfSpec.empty() ? "(off)" : csc.bestOfSpec) << std::endl;
      os << ";   near-tie epsilon " << csc.nearTieEpsilon << std::endl;
      os << ";   target type " << (csc.targetTypeName.empty() ? "(all 24)" : csc.targetTypeName) << std::endl;
      os << ";   full detail " << (csc.fullDetail ? "true" : "false") << std::endl;
      os << ";   random seed " << csc.getRandomSeed() << std::endl;
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
      InputHandler::registerHandler("HARNESS", .35,
         [this](BaseControlVariables&, const std::string& value) {
            useTestHarness = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
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

      // TESTTRANSFORMS -- runs CompareBoundaryTransformSets (see
// CompareBoundaryTransformsTest.h): generates one random cell and
// checks S6BoundaryTransforms::generateOneBoundaryTransform against
// Selling::GetBoundaryTransforms at all 6 boundaries. Standalone
// diagnostic, not part of the fit-based FlexibleSellaTestMode suite
// (operates on raw S6/MatS6, no LatticeCell input) -- kept as its
// own keyword rather than folded into TEST for that reason. Off by
// default, matching CHECKNESTING's convention.
      InputHandler::registerHandler("TESTTRANSFORMS", .35,
         [this](BaseControlVariables&, const std::string& value) {
            testTransforms = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
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

      // MAXBOUNDARYCYCLES -- per-branch cycle cap for the boundary-tie
      // exploration used when building a type's library near its own
      // boundary strata (see this session's discussion of Selling::Reduce's
      // tie-breaking at O1B/O2/O3). Same role as Selling::Reduce's
      // limitReductionCycles, but for the offline exploratory walk, not
      // production reduction -- default kept small (tens, per instruction)
      // since pathological single paths are known to run to hundreds of
      // cycles or loop forever without a cap.
      InputHandler::registerHandler("MAXBOUNDARYCYCLES", .35,
         [this](BaseControlVariables&, const std::string& value) {
            setMaxBoundaryWalkCycles(std::stoul(value));
         }
      );

      // CHECKNESTING -- runs the general, name-free nesting-consistency
      // check (RunNestingConsistencyCheck in CmdSella.cpp) instead of
      // normal cell processing: for every pair of Delone types (P, C)
      // where C has strictly fewer free parameters than P, tests via
      // TestDeloneParentage whether C genuinely nests inside P, and for
      // every confirmed pair checks fit(P) <= fit(C) + NESTINGDELTA using
      // whatever matrix library is currently active (static or BUILD).
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

      // SEEDBOUNDARIES -- runs SellaBuild::SeedBoundaryGaps during BUILD:
      // for every (parent, child) pair TestDeloneParentage confirms as
      // genuinely nested, seeds parent's raw sample map with
      // representatives reachable by crossing child's own zero-boundaries
      // (ExpandAcrossBoundaryZeros) -- closing exactly the coverage gap
      // this session traced through O1B/O3, O1B/M2B, O1B/M3, R1/C5,
      // M1A/O4, etc. (CHECKNESTING/COMPARE). Off by default (empty
      // string): costs nothing unless requested, matches BUILD's own
      // opt-in shape.
      //   SEEDBOUNDARIES PRINT -- run the scan, report what would be
      //     seeded, change nothing (see chat with Claude, 2026-08-16:
      //     print-vs-use was left as an open choice, not decided here).
      //   SEEDBOUNDARIES USE -- run the scan and actually seed the map.
      // Only meaningful alongside BUILD; seeding a map that never gets
      // used has no effect.
      InputHandler::registerHandler("SEEDBOUNDARIES", .35,
         [this](BaseControlVariables&, const std::string& value) {
            seedBoundariesMode = LRL_StringTools::strToupper(value);
         }
      );

      // CLASSIFYDIRECT -- diverges CmdSella.cpp's main() right after
// input/controls setup into the new from-scratch classification
// rewrite (see chat, Sept 2026), bypassing the existing
// SellaFit/RemediateIfShortlistChanges/GrimmerChains pipeline
// entirely. One-way: prints and exits, existing pipeline below is
// untouched, for side-by-side comparison during the rebuild.
      InputHandler::registerHandler("CLASSIFYDIRECT", .35,
         [this](BaseControlVariables&, const std::string& value) {
            classifyDirect = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         }
      );

      // SEQUENCE -- interpreter #1 for the method-library research pipeline
      // (see chat, Sept 2026): applies the named/numbered methods in
      // GetMethodRegistry() (RemediatedFit.h's 6/25/144/168/3480-candidate
      // libraries) in strict left-to-right order, each stage's output
      // feeding the next stage's input. "168(2)" means apply method 168,
      // then apply method 168 again to that result (2 total rounds), not
      // a parallel/independent second run. Empty by default: costs
      // nothing unless requested, matches SEEDBOUNDARIES/BUILD's
      // opt-in shape.
      InputHandler::registerHandler("SEQUENCE", .35,
         [this](BaseControlVariables&, const std::string& value) {
            sequenceSpec = value;
         }
      );

      // BESTOF -- interpreter #2 for the same method-library pipeline:
      // applies each listed method independently to the SAME starting
      // input, then keeps -- per original input cell -- whichever
      // result(s) scored within NEARTIEEPS of the best fit found across
      // every method tried. Same per-method spec syntax as SEQUENCE
      // (name or number, optional "(n)" repeat count applied before that
      // method's result enters the comparison). Empty by default.
      InputHandler::registerHandler("BESTOF", .35,
         [this](BaseControlVariables&, const std::string& value) {
            bestOfSpec = value;
         }
      );

      // NEARTIEEPS -- shared absolute-fit tolerance used by every method
      // in RemediatedFit.h's registry (see chat, Sept 2026) to decide
      // which of its own candidates -- including the input itself, always
      // in the comparison pool -- survive into its output vector, and by
      // BESTOF to decide which methods' final results count as tied.
      // Deliberately a single shared value for now (Larry's call, chat
      // Sept 2026) rather than a per-method value like NESTINGDELTA;
      // revisit if the method-comparison research shows methods need
      // different bands.
      InputHandler::registerHandler("NEARTIEEPS", .35,
         [this](BaseControlVariables&, const std::string& value) {
            setNearTieEpsilon(std::stod(value));
         }
      );

      // TARGETTYPE -- restricts SEQUENCE/BESTOF to one or several Delone
      // types (e.g. "M4" or "M4 O4" -- SelectTargetTypes in
      // MethodLibrary.h splits on whitespace and calls
      // GenerateDeloneBase::Select once per name, since Select itself
      // takes only one name at a time) instead of looping all 24 (chat,
      // Sept 2026: output volume concern, same pattern as
      // FlexibleSellaTestMode.h's relevantTypeNames). Empty by default
      // (loop all 24). Print detail is controlled separately by
      // FULLDETAIL below, not by whether TARGETTYPE is set -- a batch
      // comparison run wants a handful of types restricted for speed
      // WITHOUT the full per-candidate dump (chat, Sept 2026).
      InputHandler::registerHandler("TARGETTYPE", .35,
         [this](BaseControlVariables&, const std::string& value) {
            targetTypeName = value;
         }
      );

      // FULLDETAIL -- independent of TARGETTYPE (chat, Sept 2026): prints
      // every surviving near-tie candidate per type instead of the
      // compact one-line summary. Off by default, so restricting to a
      // handful of types via TARGETTYPE for a batch/comparison run stays
      // compact unless full detail is explicitly requested too.
      InputHandler::registerHandler("FULLDETAIL", .35,
         [this](BaseControlVariables&, const std::string& value) {
            fullDetail = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         }
      );

      // SEED / RANDOMSEED moved to BaseControlVariables.h (2026-08-29) --
      // registered there instead of here so every program built on this
      // framework gets it, not just CmdSella. randomSeed/getRandomSeed()
      // are now inherited from the base class too.
   }

   int getBlockSize() const { return  static_cast<int>(blocksize); }
   int getBlockStart() const { return static_cast<int>(blockstart); }
   std::string getPrefix() const { return prefix; }
   bool DoGraphics() const { return doGraphics; }
   bool shouldBuildMatrices() const { return buildMatrices; }
   bool shouldDebug() const { return debug; }
   bool shouldPrintMatrices() const { return printMatrices; }
   bool shouldRunTests() const { return testMode; }
   // Matches FlexibleTestMode.h's MultiTransformFinderControls convention.
   // Empty vector means "run all" (or list, if 999 is present).
   std::vector<int> getTestNumbers() const { return testNumbers; }
   bool shouldCompare() const { return compareMode; }
   bool shouldShowChainFailures() const { return showChainFailures; }
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
   bool shouldUseHarness() const { return useTestHarness; };
   bool shouldClassifyDirect() const { return classifyDirect; }
   std::string getSequenceSpec() const { return sequenceSpec; }
   bool shouldRunSequence() const { return !sequenceSpec.empty(); }
   std::string getBestOfSpec() const { return bestOfSpec; }
   bool shouldRunBestOf() const { return !bestOfSpec.empty(); }
   double getNearTieEpsilon() const { return nearTieEpsilon; }
   std::string getTargetTypeName() const { return targetTypeName; }
   bool shouldRestrictTypes() const { return !targetTypeName.empty(); }
   bool shouldShowFullDetail() const { return fullDetail; }

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

   void setMaxBoundaryWalkCycles(const size_t n) {
      if (n == 0) {
         std::cerr << ";Warning: MAXBOUNDARYCYCLES must be positive, using "
            << maxBoundaryWalkCycles << std::endl;
      }
      else {
         maxBoundaryWalkCycles = n;
      }
   }

   void setNearTieEpsilon(const double d) {
      if (d < 0.0) {
         std::cerr << ";Warning: NEARTIEEPS cannot be negative, using "
            << nearTieEpsilon << std::endl;
      }
      else {
         nearTieEpsilon = d;
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
   std::string seedBoundariesMode;  // "", "PRINT", or "USE" (see SEEDBOUNDARIES handler above)
   bool useTestHarness = false;
   bool classifyDirect = true;
   std::string sequenceSpec;   // "" == off (see SEQUENCE handler above)
   std::string bestOfSpec;     // "" == off (see BESTOF handler above)
   std::string targetTypeName; // "" == loop all 24 (see TARGETTYPE handler above)
   bool fullDetail = false;    // see FULLDETAIL handler above

   // Default is a placeholder, not calibrated -- same caveat as
   // nestingDelta above. Needs checking against real method-comparison
   // data once the method-library research (chat, Sept 2026) has some.
   double nearTieEpsilon = 1.0E-4;
};

#endif // INPUTHANDLER_CMD_SELLA_FEATURES_H
