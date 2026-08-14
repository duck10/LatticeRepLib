// CmdSellaTests.cpp
//
// Built-in regression tests for CmdSella, wired to the TEST control
// (mirroring CmdLMP3's FlexibleTestRunner: `cmdsella` -> type `test` ->
// `end` -> runs this suite instead of normal cell processing).
//
// Each test is arrange/act/assert, named for exactly what it checks, and
// prints PASS/FAIL with a short reason. RunCmdSellaTests() is the single
// entry point called from main().

#include "CmdSellaControls.h"
#include "DeloneFitResults.h"
#include "GrimmerTree.h"
#include "LatticeCell.h"
#include "Sella.h"

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

// Declared in CmdSella.cpp; there's no header for these yet -- see the
// note in the original CmdSellaCompareTests.cpp draft about moving these
// declarations into a proper CmdSella.h at some point.
struct DeloneFitComputation {
   std::vector<DeloneFitResults> fits;
   GrimmerChains chains;
};
DeloneFitComputation ComputeDeloneFits(
   const LatticeCell& input,
   const CmdSellaControls& controls,
   Sella& sella);
std::vector<std::pair<std::string, std::vector<DeloneFitResults>>> BuildCompareResults(
   const std::vector<LatticeCell>& inputList,
   const CmdSellaControls& controls,
   Sella& sella);


namespace {

   int g_failures = 0;

   void Check(const bool condition, const std::string& testName, const std::string& detail) {
      if (condition) {
         std::cout << "PASS: " << testName << std::endl;
      }
      else {
         std::cout << "FAIL: " << testName << " -- " << detail << std::endl;
         ++g_failures;
      }
   }

   // Count how many DeloneFitResults entries in `fits` report the given
   // GetGeneralType() name -- used to check for the duplicate-row bug that
   // the remediation-loop progress check fixed (a type appearing once should
   // stay appearing once, not once per remediation pass).
   size_t CountEntriesForType(const std::vector<DeloneFitResults>& fits, const std::string& typeName) {
      size_t count = 0;
      for (const auto& f : fits) {
         if (f.GetGeneralType() == typeName) ++count;
      }
      return count;
   }

   // One named, runnable test. Numbered by position in g_registry (1-based),
   // matching CmdLMP3's TEST/TEST-N convention: TEST alone runs everything,
   // TEST N runs only test number N.
   struct NamedTest {
      std::string name;
      bool (*fn)();
   };

} // namespace


// ---------------------------------------------------------------------
// Test: a cell with a genuine, single Grimmer-chain violation resolves
// after remediation. Fixture confirmed live: P 7.062 16.933 17.019
// 144.338 90.898 113.499 -- before this session's GrimmerTree.cpp fix
// (CheckAllGrimmerChains clearing m_GrimmerFailures, ReplaceRemediation
// re-running the check), HasFailure() stayed stuck true forever after any
// violation, regardless of whether remediation actually fixed it.
// ---------------------------------------------------------------------
bool Test_SingleViolation_ResolvesAfterRemediation() {
   const std::string testName = "Test_SingleViolation_ResolvesAfterRemediation";
   const LatticeCell fixture("P 7.062 16.933 17.019 144.338 90.898 113.499");

   CmdSellaControls controls;
   Sella sella;

   const DeloneFitComputation computation = ComputeDeloneFits(fixture, controls, sella);

   const bool passed = !computation.chains.HasFailure();
   Check(passed, testName,
      "expected HasFailure() == false after remediation, got true");
   return passed;
}

// ---------------------------------------------------------------------
// Test: remediation does not append a duplicate DeloneFitResults row when
// a pass makes no further progress. Same fixture as above -- before the
// progress-check fix, this cell's "oC" row appeared 10 times (one per
// wasted remediation pass) instead of once (before) + once (after the one
// real improvement).
// ---------------------------------------------------------------------
bool Test_RemediationLoop_DoesNotDuplicateRows() {
   const std::string testName = "Test_RemediationLoop_DoesNotDuplicateRows";
   const LatticeCell fixture("P 7.062 16.933 17.019 144.338 90.898 113.499");

   CmdSellaControls controls;
   Sella sella;

   const DeloneFitComputation computation = ComputeDeloneFits(fixture, controls, sella);

   // Expect exactly 2 "O4" entries: the original (pre-remediation) fit and
   // the one improved-and-accepted remediation result -- not 1 (no
   // remediation ran) and not >2 (duplicate/wasted passes appended).
   const size_t o4Count = CountEntriesForType(computation.fits, "O4");
   const bool passed = (o4Count == 2);
   Check(passed, testName,
      "expected exactly 2 O4 entries (original + one real improvement), got " +
      std::to_string(o4Count));
   return passed;
}

// ---------------------------------------------------------------------
// Characterization tests: these six S6 fixtures are KNOWN, CONFIRMED
// still-failing cases as of this session -- Remediation's search (single
// elementary boundary transform, no 24-reflection-orbit search, no depth
// composition beyond what one transform finds) cannot beat the "oC"
// violation for any of them. These tests currently assert the FAILING
// state on purpose, to document the known gap and catch any accidental
// regression in the OTHER direction (e.g. if a future change makes
// HasFailure() wrongly report false for these).
//
// When Remediation's search is eventually improved (see conversation
// notes: missing reflection-orbit search is the leading suspect), these
// assertions should be INVERTED to expect HasFailure() == false, and the
// TODO comments below removed.
// ---------------------------------------------------------------------
bool Test_KnownUnresolvedViolation(const std::string& s6Line, const std::string& testName) {
   const LatticeCell fixture(s6Line);

   CmdSellaControls controls;
   Sella sella;

   const DeloneFitComputation computation = ComputeDeloneFits(fixture, controls, sella);

   // TODO: flip to !computation.chains.HasFailure() once Remediation's
   // search is improved (missing reflection-orbit / depth-composition --
   // see conversation notes on the O1B/O2/O3 and oC/mP/tP investigations).
   const bool passed = computation.chains.HasFailure();
   Check(passed, testName,
      "expected HasFailure() == true (known unresolved case), got false -- "
      "if this now passes as resolved, Remediation's search may have improved; "
      "update this test to assert resolution instead of failure");
   return passed;
}

bool Test_KnownUnresolvedViolation_1() {
   return Test_KnownUnresolvedViolation(
      "S -240.86238 -24.92865 -8.64775 -7.17380 -34.21200 -8.19574",
      "Test_KnownUnresolvedViolation_1");
}
bool Test_KnownUnresolvedViolation_2() {
   return Test_KnownUnresolvedViolation(
      "S -230.17565 -17.27155 -54.34361 -23.25853 -11.49257 -55.62911",
      "Test_KnownUnresolvedViolation_2");
}
bool Test_KnownUnresolvedViolation_3() {
   return Test_KnownUnresolvedViolation(
      "S -2.78162 -34.43272 -237.34522 -10.96929 -43.32243 -21.83436",
      "Test_KnownUnresolvedViolation_3");
}
bool Test_KnownUnresolvedViolation_4() {
   return Test_KnownUnresolvedViolation(
      "S -11.10601 -81.30216 -222.54748 -5.31460 -54.25901 -27.69384",
      "Test_KnownUnresolvedViolation_4");
}
bool Test_KnownUnresolvedViolation_5() {
   return Test_KnownUnresolvedViolation(
      "S -9.12428 -0.87504 -63.50943 -233.55358 -4.41117 -36.27397",
      "Test_KnownUnresolvedViolation_5");
}
bool Test_KnownUnresolvedViolation_6() {
   return Test_KnownUnresolvedViolation(
      "S -3.00480 -19.63201 -241.04017 -9.76684 -34.86767 -13.93003",
      "Test_KnownUnresolvedViolation_6");
}

// ---------------------------------------------------------------------
// Test: COMPARE mode processes every cell in inputList, regardless of
// blockstart/blocksize (that window is a web-batching-only concept and
// has no bearing on which cells belong together in a comparison).
// ---------------------------------------------------------------------
bool Test_CompareMode_ProcessesEntireInputList_IgnoringBlockWindow() {
   const std::string testName = "Test_CompareMode_ProcessesEntireInputList_IgnoringBlockWindow";

   std::vector<LatticeCell> inputList;
   inputList.push_back(LatticeCell("P 10 10 10 90 90 90"));
   inputList.push_back(LatticeCell("P 10 10 11 90 90 90"));
   inputList.push_back(LatticeCell("P 10 11 12 90 90 90"));
   inputList.push_back(LatticeCell("P 10 10 10 90 90 91"));
   inputList.push_back(LatticeCell("P 10 12 14 90 90 90"));

   CmdSellaControls controls;
   // NOTE: InputHandler::commandSystem is static and shared across every
   // CmdSellaControls instance -- see conversation notes. Safe here only
   // because this test constructs, configures, and uses `controls`
   // immediately with nothing else constructing a CmdSellaControls in
   // between.
   InputHandler::handleCommand(controls, "BLOCKSTART", "2");
   InputHandler::handleCommand(controls, "BLOCKSIZE", "1");

   Sella sella;
   const auto compareResults = BuildCompareResults(inputList, controls, sella);

   const bool passed = (compareResults.size() == inputList.size());
   Check(passed, testName,
      "expected " + std::to_string(inputList.size()) + " results, got " +
      std::to_string(compareResults.size()));
   return passed;
}


int RunCmdSellaTests(const CmdSellaControls& controls) {
   g_failures = 0;

   // Registry, numbered 1..N by position -- this is the single place that
   // adds a new test to the suite. TEST alone runs all of these; TEST N
   // runs only the Nth one. Numbers are printed alongside each result so
   // "TEST N" can be used to re-run just a failing one.
   static const std::vector<NamedTest> registry = {
      {"Test_SingleViolation_ResolvesAfterRemediation", &Test_SingleViolation_ResolvesAfterRemediation},
      {"Test_RemediationLoop_DoesNotDuplicateRows", &Test_RemediationLoop_DoesNotDuplicateRows},
      {"Test_CompareMode_ProcessesEntireInputList_IgnoringBlockWindow", &Test_CompareMode_ProcessesEntireInputList_IgnoringBlockWindow},
      {"Test_KnownUnresolvedViolation_1", &Test_KnownUnresolvedViolation_1},
      {"Test_KnownUnresolvedViolation_2", &Test_KnownUnresolvedViolation_2},
      {"Test_KnownUnresolvedViolation_3", &Test_KnownUnresolvedViolation_3},
      {"Test_KnownUnresolvedViolation_4", &Test_KnownUnresolvedViolation_4},
      {"Test_KnownUnresolvedViolation_5", &Test_KnownUnresolvedViolation_5},
      {"Test_KnownUnresolvedViolation_6", &Test_KnownUnresolvedViolation_6},
   };

   const int requested = controls.getTestNumber();
   if (requested != 0) {
      if (requested < 1 || static_cast<size_t>(requested) > registry.size()) {
         std::cout << "; ERROR: TEST " << requested << " out of range (1.."
            << registry.size() << ")" << std::endl;
         return 1;
      }
      std::cout << "; Running CmdSella test " << requested << ": "
         << registry[requested - 1].name << std::endl;
      registry[requested - 1].fn();
   }
   else {
      std::cout << "; Running CmdSella built-in test suite ("
         << registry.size() << " tests)" << std::endl;
      for (size_t i = 0; i < registry.size(); ++i) {
         std::cout << "[" << (i + 1) << "] ";
         registry[i].fn();
      }
   }

   std::cout << "; " << g_failures << " failure(s)" << std::endl;
   return g_failures;
}
