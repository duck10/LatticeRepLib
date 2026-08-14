// CmdSellaCompareTests.cpp
//
// Two focused tests for the COMPARE-mode and Remediation changes made to
// CmdSella.cpp / ComputeDeloneFits. Each test checks exactly one behavior,
// named for what it verifies, in the arrange/act/assert shape used
// elsewhere in the test suite (see FlexibleTestMode.h's ContainsExpectation
// convention). Wire these into whatever runner calls the existing
// runXxxTests(controls)-style functions.

#include "CmdSellaControls.h"
#include "DeloneFitResults.h"
#include "InputHandler.h"
#include "LatticeCell.h"
#include "Sella.h"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

// Declared in CmdSella.cpp; not exposed via a header today. If/when this
// file is wired into the real build, move these declarations into a small
// CmdSella.h alongside the .cpp instead of forward-declaring them here.
struct DeloneFitComputation {
   std::vector<DeloneFitResults> fits;
   class GrimmerChains* chainsPlaceholder; // placeholder only -- see note below
};
std::vector<std::pair<std::string, std::vector<DeloneFitResults>>> BuildCompareResults(
   const std::vector<LatticeCell>& inputList,
   const CmdSellaControls& controls,
   Sella& sella);
// (ComputeDeloneFits itself is not needed directly by these two tests.)


// ---------------------------------------------------------------------
// Test 1: COMPARE mode must process every cell in inputList, regardless
// of blockstart/blocksize. This is the direct behavioral requirement from
// this conversation ("blockstart/blocksize should be ignored here...
// COMPARE will simply compare all of inputList") and it's exactly the kind
// of thing that regresses silently if someone later "helpfully" reuses the
// blockstart/blocksize window inside BuildCompareResults.
// ---------------------------------------------------------------------
bool Test_CompareMode_ProcessesEntireInputList_IgnoringBlockWindow() {
   const std::string testName = "Test_CompareMode_ProcessesEntireInputList_IgnoringBlockWindow";

   // Arrange: five distinct input cells, and a controls object whose
   // blockstart/blocksize are driven to a window that would restrict
   // processing to a single cell in the middle of the list, IF anything
   // in the COMPARE path honored them (it should not).
   std::vector<LatticeCell> inputList;
   inputList.push_back(LatticeCell("P 10 10 10 90 90 90"));
   inputList.push_back(LatticeCell("P 10 10 11 90 90 90"));
   inputList.push_back(LatticeCell("P 10 11 12 90 90 90"));
   inputList.push_back(LatticeCell("P 10 10 10 90 90 91"));
   inputList.push_back(LatticeCell("P 10 12 14 90 90 90"));

   CmdSellaControls controls;
   // NOTE: InputHandler::commandSystem is a static member shared by every
   // CmdSellaControls instance; each constructor call re-registers its
   // handler lambdas (capturing `this`) under the same command names in
   // that shared registry. handleCommand(controls, ...) below only reaches
   // THIS controls object correctly because it's the most recently
   // constructed CmdSellaControls when the calls are made. If this test
   // suite ever constructs more than one CmdSellaControls (e.g. running
   // Test 1 and Test 2 with separate controls objects, or in parallel),
   // whichever was constructed last "owns" BLOCKSTART/BLOCKSIZE/etc. for
   // all of them -- a real fragility worth fixing in CmdSellaControls
   // itself at some point, not just working around in tests.
   InputHandler::handleCommand(controls, "BLOCKSTART", "2");
   InputHandler::handleCommand(controls, "BLOCKSIZE", "1");
   assert(controls.getBlockStart() == 2);
   assert(controls.getBlockSize() == 1);

   Sella sella; // NOTE: assumes matrices have already been built onto this
   // Sella instance the same way main() does via SellaBuild,
   // or that a shared pre-built Sella fixture is available to
   // the test suite -- left as a TODO, since building matrices
   // fresh in every test would be needlessly slow and this
   // test doesn't care what the fits actually are, only how
   // many cells got processed.

// Act
   const auto compareResults = BuildCompareResults(inputList, controls, sella);

   // Assert: one result per input cell, not per blockstart/blocksize window
   // (which, if honored, would have limited this to exactly 1 result).
   const bool passed = (compareResults.size() == inputList.size());

   std::cout << (passed ? "PASS: " : "FAIL: ") << testName
      << " -- expected " << inputList.size() << " results, got "
      << compareResults.size() << std::endl;
   return passed;
}


// ---------------------------------------------------------------------
// Test 2: the remediation loop in ComputeDeloneFits must not stop after
// repairing only the first Grimmer-chain violation. This is a direct
// regression test for the if -> while fix: before the fix, a cell sitting
// near a multi-way boundary (like the O1B/O2/O3 case worked through in
// this conversation) could be left with a violation unresolved because
// only gcs.GetFirstFailure() was ever repaired.
//
// This needs a real fixture cell known to trigger more than one
// simultaneous chain violation before the fix. The O3-exact / perturbed
// S6 vectors from the original O1B/O2/O3 table are good candidates, but
// confirming which of them (if any) trips *two* violations at once
// (rather than one) requires running them through the actual pipeline --
// left as a TODO to fill in the fixture below with a confirmed case.
// ---------------------------------------------------------------------
bool Test_ComputeDeloneFits_RemediationLoop_ResolvesAllViolations_NotJustFirst() {
   const std::string testName = "Test_ComputeDeloneFits_RemediationLoop_ResolvesAllViolations_NotJustFirst";

   // Arrange: TODO -- replace with a confirmed multi-violation fixture cell.
   // Placeholder uses the O3-boundary-adjacent style of cell discussed
   // earlier in this conversation (near-tied Selling scalars).
   const LatticeCell fixture("P 10.000 1.000 1.000 60.000 87.134 87.134");

   CmdSellaControls controls;
   Sella sella; // see note in Test 1 re: pre-built matrices

   // Act
   // TODO: call ComputeDeloneFits(fixture, controls, sella) once the
   // fixture above is confirmed to trigger multiple violations, then
   // re-run the same GrimmerChains check-construction used inside
   // ComputeDeloneFits against computation.fits/computation.chains to
   // confirm gcs.HasFailure() == false afterward (all violations
   // resolved), not just that the first one's fit value changed.

   const bool passed = false; // placeholder until the fixture is confirmed
   std::cout << "SKIP (fixture not yet confirmed): " << testName << std::endl;
   return passed;
}


int RunCmdSellaCompareTests() {
   int failures = 0;
   if (!Test_CompareMode_ProcessesEntireInputList_IgnoringBlockWindow()) ++failures;
   // Test 2 intentionally not counted as a hard failure yet -- see its TODO.
   Test_ComputeDeloneFits_RemediationLoop_ResolvesAllViolations_NotJustFirst();
   return failures;
}
