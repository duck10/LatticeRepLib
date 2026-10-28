// FlexibleSellaTestMode.h
//
// Generalized, expandable regression-test framework for CmdSella, modeled
// directly on CmdLMP3's FlexibleTestMode.h (FlexibleTestCase /
// FlexibleTestRunner). See SELLA_handoff.md, item 5, for the design
// discussion this implements.
//
// Design note carried over unchanged from the handoff: individual-cell and
// comparison-mode tests are NOT a fork. FlexibleSellaTestCase::inputCells
// is a vector; a single-cell test is just the size()==1 case, and a
// comparison test (e.g. the O1B/O2/O3-style side-by-side case) is the
// size()>1 case. One expectation can be checked per cell, or across cells,
// depending on which expectation type is used -- see RankStabilityExpectation
// below for the one comparison-only case.
//
// What's SELLA-specific (unlike CmdLMP3's P3-distance/S6-angle/determinant
// triple) is the set of expectation types, since there is no "matrix" or
// "P3 distance" here -- the thing under test is a set of Delone/Bravais fit
// values and Grimmer-chain pass/fail state per input cell:
//
//   FitRangeExpectation        -- named type's raw fit falls in [min, max]
//                                  for one input cell.
//   HasFailureExpectation      -- GrimmerChains::HasFailure() (after
//                                  remediation) equals an expected bool, for
//                                  one input cell.
//   NestingInequalityExpectation
//                              -- fit(parentType) <= fit(childType) +
//                                  tolerance, for one input cell. This is
//                                  the literal certificate for a boundary
//                                  stratum genuinely nesting inside a
//                                  parent type (see SELLA_handoff.md item 1)
//                                  -- NOT a claim about which types nest in
//                                  which; the caller supplies the pair.
//   RankStabilityExpectation   -- comparison-only (needs >=2 input cells):
//                                  no named type's best-vs-second-best rank
//                                  (by raw fit, across the named type set)
//                                  should flip between consecutive input
//                                  cells. Direct check for the O1B/O2/O3
//                                  instability that motivated this session.
//
// A test with no expectations attached is a MANUAL test: it runs and
// prints results for human inspection, and neither passes nor fails
// automatically (matches CmdLMP3's test4 "ManualCheck" convention).
//
// This file includes DeloneFitComputation.h for the shared
// DeloneFitComputation struct / ComputeDeloneFits declaration -- do not
// redeclare either here (see DeloneFitComputation.h for why: CmdSella.cpp
// #includes this file directly, so a second copy in the same TU is a
// redefinition, not a harmless cross-TU duplicate).

#ifndef FLEXIBLE_SELLA_TEST_MODE_H
#define FLEXIBLE_SELLA_TEST_MODE_H

#include "CheckABReflection.h"
#include "CmdSellaControls.h"
#include "DeloneFitComputation.h"
#include "DeloneFitResults.h"
#include "GrimmerTree.h"
#include "LatticeCell.h"
#include "MatS6.h"
#include "S6.h"
#include "Sella.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class FlexibleSellaTestCase {
public:

   struct FitRangeExpectation {
      std::string typeName;   // GetGeneralType() value, e.g. "O4"
      double minFit;
      double maxFit;
      size_t cellIndex;       // which inputCells[] this applies to
      std::string description;

      FitRangeExpectation(const std::string& type, const double lo, const double hi,
         const size_t cell = 0, const std::string& desc = "")
         : typeName(type), minFit(lo), maxFit(hi), cellIndex(cell), description(desc) {
      }
   };

   struct HasFailureExpectation {
      bool expectedHasFailure;
      size_t cellIndex;
      std::string description;

      HasFailureExpectation(const bool expected, const size_t cell = 0,
         const std::string& desc = "")
         : expectedHasFailure(expected), cellIndex(cell), description(desc) {
      }
   };

   struct NestingInequalityExpectation {
      std::string parentType;
      std::string childType;
      double tolerance;
      size_t cellIndex;
      std::string description;

      NestingInequalityExpectation(const std::string& parent, const std::string& child,
         const double tol = 1.0E-6, const size_t cell = 0, const std::string& desc = "")
         : parentType(parent), childType(child), tolerance(tol), cellIndex(cell), description(desc) {
      }
   };

   struct RankStabilityExpectation {
      std::vector<std::string> typeNames; // the candidate set to rank
      std::string description;

      RankStabilityExpectation(const std::vector<std::string>& types, const std::string& desc = "")
         : typeNames(types), description(desc) {
      }
   };

   // ReflectionRelationExpectation -- AUTOMATED check, wraps
   // CheckABReflectionRelation() (see CheckABReflection.h). Use only once
   // real evidence exists for what the answer should be (i.e. someone has
   // actually run the check and confirmed sameLattice true/false) --
   // otherwise use ReflectionCheck below (no asserted expectation) until
   // that evidence exists. candidateA/candidateB are independent of
   // inputCells -- this does not go through ComputeDeloneFits/Selling
   // reduction at all, just the raw 24-reflection comparison.
   struct ReflectionRelationExpectation {
      S6 candidateA;
      S6 candidateB;
      std::string labelA;
      std::string labelB;
      bool expectedSameLattice;
      double tolerance;
      std::string description;

      ReflectionRelationExpectation(const S6& a, const S6& b, const bool expectSame,
         const std::string& la = "A", const std::string& lb = "B",
         const double tol = 1.0E-3, const std::string& desc = "")
         : candidateA(a), candidateB(b), labelA(la), labelB(lb),
         expectedSameLattice(expectSame), tolerance(tol), description(desc) {
      }
   };

   // ReflectionCheck -- MANUAL companion to ReflectionRelationExpectation:
   // runs CheckABReflectionRelation() and prints its output for human
   // inspection, but asserts nothing (no known-correct answer yet). Not
   // counted by hasAnyExpectation(), so a test with only ReflectionChecks
   // (and no other expectations) still falls into the manual-dump path in
   // runTest() -- this list is drained there, alongside the fit dump.
   struct ReflectionCheck {
      S6 candidateA;
      S6 candidateB;
      std::string labelA;
      std::string labelB;
      double tolerance;
      std::string description;

      ReflectionCheck(const S6& a, const S6& b,
         const std::string& la = "A", const std::string& lb = "B",
         const double tol = 1.0E-3, const std::string& desc = "")
         : candidateA(a), candidateB(b), labelA(la), labelB(lb),
         tolerance(tol), description(desc) {
      }
   };

   std::string name;
   std::string description;
   std::vector<LatticeCell> inputCells;

   std::vector<FitRangeExpectation> fitRangeExpectations;
   std::vector<HasFailureExpectation> hasFailureExpectations;
   std::vector<NestingInequalityExpectation> nestingExpectations;
   std::vector<RankStabilityExpectation> rankStabilityExpectations;
   std::vector<ReflectionRelationExpectation> reflectionRelationExpectations;
   std::vector<ReflectionCheck> reflectionChecks;   // manual, not in hasAnyExpectation()

   FlexibleSellaTestCase(const std::string& testName, const std::string& desc)
      : name(testName), description(desc) {
   }

   void addInputCell(const LatticeCell& cell) { inputCells.push_back(cell); }

   void addFitRangeExpectation(const FitRangeExpectation& e) { fitRangeExpectations.push_back(e); }
   void addHasFailureExpectation(const HasFailureExpectation& e) { hasFailureExpectations.push_back(e); }
   void addNestingExpectation(const NestingInequalityExpectation& e) { nestingExpectations.push_back(e); }
   void addRankStabilityExpectation(const RankStabilityExpectation& e) { rankStabilityExpectations.push_back(e); }
   void addReflectionRelationExpectation(const ReflectionRelationExpectation& e) { reflectionRelationExpectations.push_back(e); }
   void addReflectionCheck(const ReflectionCheck& c) { reflectionChecks.push_back(c); }

   bool hasAnyExpectation() const {
      return !fitRangeExpectations.empty() || !hasFailureExpectations.empty()
         || !nestingExpectations.empty() || !rankStabilityExpectations.empty()
         || !reflectionRelationExpectations.empty();
      // NOTE: reflectionChecks deliberately excluded -- see its comment above.
   }
};

class FlexibleSellaTestRunner {
private:
   std::vector<FlexibleSellaTestCase> testCases;
   int passedTests = 0;
   int failedTests = 0;
   int manualTests = 0;

   static double FindRawFit(const std::vector<DeloneFitResults>& fits, const std::string& typeName,
      bool& found) {
      double best = 0.0;
      found = false;
      for (const auto& f : fits) {
         if (f.GetGeneralType() == typeName) {
            if (!found || f.GetRawFit() < best) best = f.GetRawFit();
            found = true;
         }
      }
      return best;
   }

   void displayTestInputs(const FlexibleSellaTestCase& test) const {
      std::cout << "Input cells:" << std::endl;
      for (size_t i = 0; i < test.inputCells.size(); ++i) {
         std::cout << "  Cell " << (i + 1) << ": " << test.inputCells[i].GetInput() << std::endl;
      }
   }

   // Computes the fit results for every input cell once, up front, so each
   // expectation type can be checked against the same computation without
   // re-running Selling reduction/SellaFit/remediation per expectation.
   std::vector<DeloneFitComputation> computeAll(
      const FlexibleSellaTestCase& test, const CmdSellaControls& controls, Sella& sella) const {
      std::vector<DeloneFitComputation> out;
      out.reserve(test.inputCells.size());
      for (const auto& cell : test.inputCells) {
         out.push_back(ComputeDeloneFits(cell, controls, sella));
      }
      return out;
   }

   bool checkFitRange(const FlexibleSellaTestCase::FitRangeExpectation& e,
      const std::vector<DeloneFitComputation>& computations) const {
      if (e.cellIndex >= computations.size()) {
         std::cout << "  FitRangeExpectation (" << e.description << "): FAIL -- cell index "
            << e.cellIndex << " out of range" << std::endl;
         return false;
      }
      bool found = false;
      const double fit = FindRawFit(computations[e.cellIndex].fits, e.typeName, found);
      if (!found) {
         std::cout << "  FitRangeExpectation (" << e.description << "): FAIL -- type '"
            << e.typeName << "' not found in cell " << (e.cellIndex + 1) << std::endl;
         return false;
      }
      const bool ok = (fit >= e.minFit && fit <= e.maxFit);
      std::cout << "  FitRangeExpectation (" << e.description << "): "
         << (ok ? "PASS" : "FAIL") << " -- " << e.typeName << " fit " << fit
         << " (expected [" << e.minFit << ", " << e.maxFit << "] for cell "
         << (e.cellIndex + 1) << ")" << std::endl;
      return ok;
   }

   bool checkHasFailure(const FlexibleSellaTestCase::HasFailureExpectation& e,
      const std::vector<DeloneFitComputation>& computations) const {
      if (e.cellIndex >= computations.size()) {
         std::cout << "  HasFailureExpectation (" << e.description << "): FAIL -- cell index "
            << e.cellIndex << " out of range" << std::endl;
         return false;
      }
      const bool actual = computations[e.cellIndex].chains.HasFailure();
      const bool ok = (actual == e.expectedHasFailure);
      std::cout << "  HasFailureExpectation (" << e.description << "): "
         << (ok ? "PASS" : "FAIL") << " -- HasFailure()==" << (actual ? "true" : "false")
         << " (expected " << (e.expectedHasFailure ? "true" : "false")
         << ") for cell " << (e.cellIndex + 1) << std::endl;
      return ok;
   }

   bool checkNesting(const FlexibleSellaTestCase::NestingInequalityExpectation& e,
      const std::vector<DeloneFitComputation>& computations) const {
      if (e.cellIndex >= computations.size()) {
         std::cout << "  NestingInequalityExpectation (" << e.description << "): FAIL -- cell index "
            << e.cellIndex << " out of range" << std::endl;
         return false;
      }
      const auto& fits = computations[e.cellIndex].fits;
      bool foundParent = false, foundChild = false;
      const double parentFit = FindRawFit(fits, e.parentType, foundParent);
      const double childFit = FindRawFit(fits, e.childType, foundChild);
      if (!foundParent || !foundChild) {
         std::cout << "  NestingInequalityExpectation (" << e.description << "): FAIL -- "
            << (!foundParent ? e.parentType : e.childType) << " not found in cell "
            << (e.cellIndex + 1) << std::endl;
         return false;
      }
      const bool ok = (parentFit <= childFit + e.tolerance);
      std::cout << "  NestingInequalityExpectation (" << e.description << "): "
         << (ok ? "PASS" : "FAIL") << " -- fit(" << e.parentType << ")=" << parentFit
         << " <= fit(" << e.childType << ")=" << childFit
         << " (tolerance " << e.tolerance << ") for cell " << (e.cellIndex + 1) << std::endl;
      return ok;
   }

   bool checkRankStability(const FlexibleSellaTestCase::RankStabilityExpectation& e,
      const std::vector<DeloneFitComputation>& computations) const {
      if (computations.size() < 2) {
         std::cout << "  RankStabilityExpectation (" << e.description
            << "): FAIL -- needs >= 2 input cells, got " << computations.size() << std::endl;
         return false;
      }

      // Best and second-best (by raw fit) among e.typeNames, per cell.
      std::vector<std::string> bestPerCell, secondPerCell;
      for (const auto& comp : computations) {
         std::vector<std::pair<double, std::string>> ranked;
         for (const auto& t : e.typeNames) {
            bool found = false;
            const double fit = FindRawFit(comp.fits, t, found);
            if (found) ranked.emplace_back(fit, t);
         }
         std::sort(ranked.begin(), ranked.end());
         bestPerCell.push_back(ranked.size() > 0 ? ranked[0].second : "");
         secondPerCell.push_back(ranked.size() > 1 ? ranked[1].second : "");
      }

      bool ok = true;
      for (size_t i = 1; i < bestPerCell.size(); ++i) {
         const bool flipped = (bestPerCell[i] != bestPerCell[i - 1]);
         std::cout << "    cell " << i << " -> " << (i + 1) << ": best "
            << bestPerCell[i - 1] << " -> " << bestPerCell[i]
            << ", second " << secondPerCell[i - 1] << " -> " << secondPerCell[i]
            << (flipped ? "   ** RANK FLIP **" : "") << std::endl;
         if (flipped) ok = false;
      }
      std::cout << "  RankStabilityExpectation (" << e.description << "): "
         << (ok ? "PASS" : "FAIL") << std::endl;
      return ok;
   }

   bool checkReflectionRelation(const FlexibleSellaTestCase::ReflectionRelationExpectation& e) const {
      const ReflectionRelationResult r = CheckABReflectionRelation(
         e.candidateA, e.candidateB, e.labelA, e.labelB, e.tolerance, true);
      const bool ok = (r.sameLattice == e.expectedSameLattice);
      std::cout << "  ReflectionRelationExpectation (" << e.description << "): "
         << (ok ? "PASS" : "FAIL") << " -- sameLattice=" << (r.sameLattice ? "true" : "false")
         << " (expected " << (e.expectedSameLattice ? "true" : "false")
         << "), best residual " << r.bestResidual << " at reflection " << r.bestIndex << std::endl;
      return ok;
   }

   void runTest(const FlexibleSellaTestCase& test, const CmdSellaControls& controls, Sella& sella) {
      std::cout << "\n=== TEST: " << test.name << " ===" << std::endl;
      std::cout << "Description: " << test.description << std::endl;
      displayTestInputs(test);

      if (test.inputCells.empty()) {
         std::cout << "FAIL: Test has no input cells" << std::endl;
         ++failedTests;
         return;
      }

      const std::vector<DeloneFitComputation> computations = computeAll(test, controls, sella);

      if (!test.hasAnyExpectation()) {
         std::cout << "\n; No expectations defined -- manual verification needed" << std::endl;
         for (size_t i = 0; i < computations.size(); ++i) {
            std::cout << "  Cell " << (i + 1) << " fits:" << std::endl;
            for (const auto& f : computations[i].fits) {
               std::cout << "    " << f.GetBravaisType() << " " << f.GetGeneralType()
                  << " " << f.GetRawFit() << std::endl;
            }
            std::cout << "  Cell " << (i + 1) << " HasFailure(): "
               << (computations[i].chains.HasFailure() ? "true" : "false") << std::endl;
         }
         if (!test.reflectionChecks.empty()) {
            std::cout << "\n; Reflection-relation checks (manual, no asserted expectation "
               << "-- no confirmed-correct answer yet):" << std::endl;
            for (const auto& c : test.reflectionChecks) {
               std::cout << "  (" << c.description << ")" << std::endl;
               CheckABReflectionRelation(c.candidateA, c.candidateB, c.labelA, c.labelB, c.tolerance, true);
            }
         }
         std::cout << ";OVERALL: MANUAL CHECK REQUIRED" << std::endl;
         ++manualTests;
         return;
      }

      bool testPassed = true;
      if (!test.fitRangeExpectations.empty()) {
         std::cout << "\nChecking fit-range expectations:" << std::endl;
         for (const auto& e : test.fitRangeExpectations)
            testPassed = checkFitRange(e, computations) && testPassed;
      }
      if (!test.hasFailureExpectations.empty()) {
         std::cout << "\nChecking HasFailure() expectations:" << std::endl;
         for (const auto& e : test.hasFailureExpectations)
            testPassed = checkHasFailure(e, computations) && testPassed;
      }
      if (!test.nestingExpectations.empty()) {
         std::cout << "\nChecking nesting-inequality expectations:" << std::endl;
         for (const auto& e : test.nestingExpectations)
            testPassed = checkNesting(e, computations) && testPassed;
      }
      if (!test.reflectionRelationExpectations.empty()) {
         std::cout << "\nChecking reflection-relation expectations:" << std::endl;
         for (const auto& e : test.reflectionRelationExpectations)
            testPassed = checkReflectionRelation(e) && testPassed;
      }
      if (!test.rankStabilityExpectations.empty()) {
         std::cout << "\nChecking rank-stability expectations:" << std::endl;
         for (const auto& e : test.rankStabilityExpectations)
            testPassed = checkRankStability(e, computations) && testPassed;
      }

      if (testPassed) {
         std::cout << "OVERALL: PASS" << std::endl;
         ++passedTests;
      }
      else {
         std::cout << "OVERALL: FAIL" << std::endl;
         ++failedTests;
      }
   }

public:
   void addTestCase(const FlexibleSellaTestCase& testCase) { testCases.push_back(testCase); }

   void runSingleTest(const int testNumber, const CmdSellaControls& controls, Sella& sella) {
      if (testNumber < 1 || testNumber > static_cast<int>(testCases.size())) {
         std::cout << "Error: Test number " << testNumber << " is out of range (1-"
            << testCases.size() << ")" << std::endl;
         return;
      }
      const auto& test = testCases[testNumber - 1];
      std::cout << "\n========================================" << std::endl;
      std::cout << "RUNNING SINGLE TEST: " << testNumber << " (" << test.name << ")" << std::endl;
      std::cout << "========================================" << std::endl;

      passedTests = 0;
      failedTests = 0;
      manualTests = 0;

      const auto t0 = std::chrono::high_resolution_clock::now();
      runTest(test, controls, sella);
      const auto t1 = std::chrono::high_resolution_clock::now();
      std::cout << "; Test " << testNumber << " elapsed: "
         << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()
         << " ms" << std::endl;

      std::cout << "\n========================================" << std::endl;
      std::cout << "SINGLE TEST RESULT" << std::endl;
      std::cout << "========================================" << std::endl;
      if (passedTests > 0) std::cout << "TEST PASSED!" << std::endl;
      else if (failedTests > 0) std::cout << "TEST FAILED!" << std::endl;
      else std::cout << "TEST REQUIRES MANUAL VERIFICATION!" << std::endl;
   }

   void listTests() const {
      std::cout << "\n========================================" << std::endl;
      std::cout << "AVAILABLE TESTS" << std::endl;
      std::cout << "========================================" << std::endl;
      for (size_t i = 0; i < testCases.size(); ++i) {
         std::cout << "Test " << (i + 1) << ": " << testCases[i].name
            << " - " << testCases[i].description << std::endl;
      }
      std::cout << "\nUsage:" << std::endl;
      std::cout << "  TEST            : run all tests" << std::endl;
      std::cout << "  TEST 1-" << testCases.size() << "        : run one specific test" << std::endl;
      std::cout << "  TEST 1 3 5      : run several specific tests" << std::endl;
      std::cout << "  TEST 999        : show this list" << std::endl;
   }

   // Run all tests, or one or more specific tests, based on controls.
   void runAllTests(const CmdSellaControls& controls, Sella& sella) {
      const std::vector<int>& requested = controls.getTestNumbers();

      for (int n : requested) {
         if (n >= 999) { listTests(); return; }
      }

      std::vector<int> validTests;
      for (int n : requested) {
         if (n > 0 && n <= static_cast<int>(testCases.size())) {
            validTests.push_back(n);
         }
         else {
            std::cout << "; Warning: Requested test " << n << " > " << testCases.size()
               << " available tests -- ignored" << std::endl;
         }
      }

      if (!validTests.empty()) {
         for (int testNumber : validTests) runSingleTest(testNumber, controls, sella);
         return;
      }

      std::cout << "\n========================================" << std::endl;
      std::cout << "RUNNING FLEXIBLE SELLA TEST SUITE" << std::endl;
      std::cout << "========================================" << std::endl;

      passedTests = 0;
      failedTests = 0;
      manualTests = 0;

      const auto t0 = std::chrono::high_resolution_clock::now();
      for (size_t i = 0; i < testCases.size(); ++i) {
         std::cout << "\n--- Test " << (i + 1) << " of " << testCases.size() << " ---";
         const auto tt0 = std::chrono::high_resolution_clock::now();
         runTest(testCases[i], controls, sella);
         const auto tt1 = std::chrono::high_resolution_clock::now();
         std::cout << "; Test " << (i + 1) << " elapsed: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(tt1 - tt0).count()
            << " ms" << std::endl;
      }
      const auto t1 = std::chrono::high_resolution_clock::now();

      std::cout << "\n========================================" << std::endl;
      std::cout << "TEST SUMMARY" << std::endl;
      std::cout << "========================================" << std::endl;
      std::cout << "Total time: "
         << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()
         << " ms" << std::endl;
      std::cout << "Passed: " << passedTests << std::endl;
      std::cout << "Failed: " << failedTests << std::endl;
      std::cout << "Manual: " << manualTests << std::endl;
      std::cout << "Total:  " << (passedTests + failedTests + manualTests) << std::endl;

      if (failedTests == 0) {
         std::cout << "ALL AUTOMATED TESTS PASSED!" << std::endl;
         if (manualTests > 0)
            std::cout << "(" << manualTests << " tests require manual verification)" << std::endl;
      }
      else {
         std::cout << "SOME TESTS FAILED!" << std::endl;
      }
   }

   // setupStandardTests -- seeded with the real, confirmed fixtures from
   // this session's handoff (SELLA_handoff.md). Each comment below states
   // where a fixture came from and why it's here; nothing is invented.
   void setupStandardTests() {

      // Test 1 -- the confirmed Grimmer-chain-violation-resolves fixture
      // (SELLA_handoff.md item 5 / this session's GrimmerTree.cpp fix).
      // Mirrors Cmdsellatests.cpp's Test_SingleViolation_ResolvesAfterRemediation,
      // expressed here as the generalized HasFailureExpectation form.
      {
         FlexibleSellaTestCase test1("SingleViolation_ResolvesAfterRemediation",
            "A cell with one genuine Grimmer-chain violation should have "
            "HasFailure()==false after remediation (verified live this session).");
         test1.addInputCell(LatticeCell("P 7.062 16.933 17.019 144.338 90.898 113.499"));
         test1.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
            false, 0, "resolves after remediation"));
         addTestCase(test1);
      }

      // Tests 2-7 -- the six known-unresolved oC-vs-tP characterization
      // fixtures (SELLA_handoff.md item 3 / Cmdsellatests.cpp
      // Test_KnownUnresolvedViolation_1..6). These intentionally assert the
      // CURRENT broken state (HasFailure()==true) as a tripwire; invert to
      // expect false once Remediation's search is improved -- see the open
      // item in SELLA_handoff.md ("Remediation's search is still genuinely
      // inadequate") before flipping these.
      {
         static const char* const knownUnresolved[6] = {
            "S -240.86238 -24.92865 -8.64775 -7.17380 -34.21200 -8.19574",
            "S -230.17565 -17.27155 -54.34361 -23.25853 -11.49257 -55.62911",
            "S -2.78162 -34.43272 -237.34522 -10.96929 -43.32243 -21.83436",
            "S -11.10601 -81.30216 -222.54748 -5.31460 -54.25901 -27.69384",
            "S -9.12428 -0.87504 -63.50943 -233.55358 -4.41117 -36.27397",
            "S -3.00480 -19.63201 -241.04017 -9.76684 -34.86767 -13.93003",
         };
         for (int i = 0; i < 6; ++i) {
            FlexibleSellaTestCase test(
               "KnownUnresolvedViolation_" + std::to_string(i + 1),
               "Known-unresolved oC-vs-tP violation (Remediation's search "
               "cannot currently fix it) -- asserts the failing state as a "
               "tripwire, per SELLA_handoff.md item 3.");
            test.addInputCell(LatticeCell(knownUnresolved[i]));
            test.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
               true, 0, "still fails: known Remediation search gap"));
            addTestCase(test);
         }
      }

      // Test 8 -- the O1B/O2/O3 boundary-point fixture that originally
      // motivated this session (SELLA_handoff.md item 1). Deliberately left
      // as a MANUAL test (no expectations): the handoff explicitly warns
      // against guessing parentage (which type nests inside which) from the
      // Delone-grid SVG's letter patterns -- that was tried twice and got
      // it wrong. Once parentage is established the reliable way (testing
      // whether a candidate parent's projector leaves a candidate child's
      // canonical representative unchanged across the 24-reflection orbit,
      // per SellaBuild::vDeloneTypes / LabeledDeloneTypeMatrices), replace
      // this with a NestingInequalityExpectation(parent, child) -- the
      // machinery for that check is already implemented above.
      {
         FlexibleSellaTestCase test8("O1B_O2_O3_BoundaryPoint",
            "Ready-made regression fixture for the original motivating "
            "issue: at this exact boundary point the raw fit table gives "
            "O1B=10, O2=0, O3=0, apparently violating fit(O1B) <= fit(O3) "
            "if O3 nests inside O1B -- but that parentage has NOT been "
            "verified (see SELLA_handoff.md item 1). Left MANUAL on "
            "purpose: do not add a NestingInequalityExpectation here until "
            "parentage is confirmed via the projector-invariance test, not "
            "by reading the SVG.");
         test8.addInputCell(LatticeCell("S -10 -20 0 -10 -20 0"));
         addTestCase(test8);
      }

      // Test 9 -- O2 within-type library gap, minimal 3-cell fixture
            // (confirmed live this session via CmdSella COMPARE, TYPES O2 O3 O1B).
            // Cell 1 is the exact O3 boundary point (baseline), given in its own
            // Selling-reduced form (see note below -- NOT the form originally
            // typed at the CLI). Cell 2 is a nearby perturbation where O1B/O2
            // behave correctly (O1B large, O2 small). Cell 3 is the confirmed
            // failure point: O1B and O2 swap rank -- O1B drops to ~0.065
            // (should stay ~5.2) while O2 spikes to ~5.25 (should stay
            // ~0.01-0.1). O3 is included in the ranked set (not just O1B/O2)
            // so the fixture also confirms O3 stays stable throughout,
            // matching the earlier 11-point sweep.
            //
            // CELL 1 NOTE: LatticeCell only stores a G6 (see LatticeCell.h),
            // so constructing from a raw "S ..." string round-trips S6 -> G6
            // at construction, then GetInputSellingReducedVectors converts
            // G6 -> S6 again before Selling::Reduce. That round trip preserves
            // the actual lattice but not which S6 slot each component sits
            // in -- for a generic point this doesn't matter, but cell 1's
            // original typed form (-43.40350 -38.13450 0.00000 -38.13450
            // -43.40350 -0.00000) is maximally degenerate (two repeated-value
            // pairs plus an exact zero) and DID land on a components-1-and-2-
            // swapped representative after the round trip (confirmed via
            // DETAILS output this session), spuriously flipping which type
            // read as "best" at cell 1 -- an artifact of relabeling, not a
            // real library-gap finding. Using the already-round-tripped form
            // below (matching GetInputSellingReducedVectors' own output)
            // makes cell 1 stable under the round trip, so the ONLY rank flip
            // Test 9 reports is the real one, between cells 2 and 3.
            //
            // This is a within-type static-library gap (CreatePerps_O2()/
            // CreatePrjs_O2() in LabeledSellaMatrices.cpp -- confirmed NOT
            // touched by the GrimmerTree.cpp Remediation transform-source fix
            // made earlier this session, since COMPARE never calls
            // Remediation). Left as a tripwire (expected to currently FAIL)
            // until CreatePerps_O2()/CreatePrjs_O2() are patched with the
            // missing orientation; flip to a passing expectation only once
            // that fix is confirmed via this exact fixture.
      {
         FlexibleSellaTestCase test9("O2_WithinType_LibraryGap_MinimalFixture",
            "O1B/O2 rank flip at a confirmed failure point -- static "
            "CreatePerps_O2()/CreatePrjs_O2() library gap, not a "
            "Remediation/transform issue. Tripwire: expected to FAIL "
            "until the library is patched.");
         test9.addInputCell(LatticeCell("S -38.13450 -43.40350 0.00000 -38.13450 -43.40350 -0.00000"));
         test9.addInputCell(LatticeCell("S -43.34318 -38.15350 -0.00055 -38.13490 -43.34264 -0.02616"));
         test9.addInputCell(LatticeCell("S -38.11642 -43.36206 -0.01851 -38.17840 -43.42403 -0.04436"));
         test9.addRankStabilityExpectation(FlexibleSellaTestCase::RankStabilityExpectation(
            { "O1B", "O2", "O3" },
            "O1B/O2/O3 best-fit rank must not flip across these three cells"));
         addTestCase(test9);
      }

      // Test 10 -- O1B/O2/O3 triple-point fit sensitivity. ORIGINALLY
      // written as a FitRangeExpectation tripwire predicting an O1B
      // orientation-coverage gap (CreatePrjs_O1B() has only 1 stored
      // entry vs O2's 6). That prediction was tested directly and
      // DISPROVEN, same session: DiagnoseO1BCoverage.h applied all 24
      // reflections to candidate B and got back the IDENTICAL residual
      // (0.064614) for every single one, to 6 decimal places -- meaning
      // O1B's one stored projector is invariant under the full 24-element
      // reflection group. There is no orientation-coverage gap in O1B.
      // In hindsight this makes sense: O1B sits near the top of the
      // Delone hierarchy as a minimally-constrained, highly generic type,
      // so a single orientation-agnostic projector may be exactly correct
      // for it, unlike O2/O3 which have genuine pairwise-equality/zero
      // constraints that reflections can break apart.
      //
      // Left as MANUAL (no automated expectation), same convention as
      // Test 8, because the real explanation for the 5.19875 (A) vs
      // 0.06462 (B) fit(O1B) swing is NOT YET KNOWN. Candidates A, B, and
      // the original cell all sit extremely close to the literal
      // O1B/O2/O3 triple point -- a genuine stratification singularity
      // where fit functions could be legitimately discontinuous as a
      // matter of geometry, not a library or code bug. Alternatively,
      // something in ComputeDeloneFits/SellaFit's handling of points this
      // close to a cusp could still be at fault -- now that the library
      // itself is cleared, that code path is the next place to look, not
      // any static perp/prj table. Do not add an automated expectation
      // here until one of those two explanations is actually confirmed.
      {
         FlexibleSellaTestCase test10("O1B_O2_O3_TriplePoint_FitSensitivity",
            "MANUAL: candidates A and B are physically close (cmddists "
            "NCDist=0.137) but fit(O1B) swings 5.19875 (A) vs 0.06462 (B). "
            "The original orientation-coverage-gap hypothesis was tested "
            "and disproven this session (DiagnoseO1BCoverage.h: identical "
            "residual across all 24 reflections). Real cause not yet "
            "known -- possibly a genuine triple-point stratification "
            "singularity, possibly a ComputeDeloneFits/SellaFit issue near "
            "the cusp. Left manual until one is confirmed.");
         test10.addInputCell(LatticeCell("S -43.34318 -38.15350 -0.00055 -38.13490 -43.34264 -0.02616"));  // candidate A
         test10.addInputCell(LatticeCell("S -38.11642 -43.36206 -0.01851 -38.17840 -43.42403 -0.04436"));  // candidate B
         addTestCase(test10);
      }

      // Test 11 -- M1B/M2A orientation swap near an M3 boundary point.
      // First entry in the boundary-case catalog called for by the
      // 2026-08-19 handoff, Section 4 ("dictionary of tests" for the full
      // Delone-type boundary set). NOT the M2B/M3 pairing originally
      // guessed at -- confirmed via real CmdSella COMPARE data
      // (cmdgen 1 M3 | cmdperturb 20 1 | cmdsella COMPARE
      // TYPES M3 M2A M2B M1A M1B M4) that M3 and M2B are both stable
      // throughout (M3 raw fit stays in [0.00, 0.11]; M2B tracks M3
      // almost exactly, both consistent with M3 nesting under M2B/M2A as
      // expected). The instability is between M1B and M2A instead: in
      // most perturbations M2A fits near-zero and M1B badly (~3.0), but
      // in a confirmed minority (4/20 in the original 20-point sweep)
      // this flips -- M1B near-zero, M2A ~2.97. Same signature as the
      // O1B/O2/O3 case (Section 2 of that handoff): the swap rows and
      // non-swap rows are close in magnitude component-by-component but
      // with two slots interchanged, consistent with orientation
      // ambiguity from Selling::Reduce, not a library gap.
      //
      // Cell 1: exact (unperturbed) M3 point -- NOTE the same round-trip
      // caveat as Test 9's cell 1 applies here (this is a degenerate
      // point: two repeated-value pairs plus an exact zero); not yet
      // confirmed stable under LatticeCell's S6->G6->S6 round trip via
      // DETAILS output the way Test 9's cell 1 was. Treat this cell 1
      // with the same suspicion until checked.
      // Cell 2: a "normal" perturbation (M2A good, M1B bad).
      // Cell 3: a confirmed "swap" perturbation (M1B good, M2A bad).
      //
      // Left as an active (non-manual) tripwire, expected to currently
      // FAIL -- no Remediation-style fix has been attempted for this
      // pair yet (that's the next step, not done in this session).
      //
      // HasFailureExpectations below added after Test 14's manual dump
      // (run this session) showed the swap point (cell 3) has a REAL
      // Grimmer chain violation (HasFailure()==true), while the normal
      // point (cell 2) does not. Confirmed real evidence, not a guess --
      // this ties the rank-flip signature directly to the chain-failure
      // detection mechanism (see also the 2026-08-19 handoff Section 4's
      // note that Remediation was originally written for exactly this
      // kind of chain failure). Cell 1 (the exact, degenerate M3 point)
      // is NOT asserted here -- its HasFailure() status was not part of
      // Test 14's dump and remains unchecked.
      {
         FlexibleSellaTestCase test11("M1B_M2A_OrientationSwap_M3Boundary",
            "M1B/M2A best-fit rank flips near an M3 boundary point, same "
            "signature as the O1B/O2/O3 case. M3 and M2B themselves stay "
            "stable throughout (not part of the instability). Tripwire: "
            "expected to FAIL until Remediation-style correction is "
            "confirmed for this pair (not yet attempted).");
         test11.addInputCell(LatticeCell("S -43.40350 -47.63950 0.00000 -28.62950 -43.40350 -0.00000"));  // cell 1: exact M3 point (round-trip UNCHECKED, see caveat above)
         test11.addInputCell(LatticeCell("S -47.59416 -28.66333 -0.02968 -43.33617 -43.38776 -0.05159"));  // cell 2: normal (M2A=0.06319, M1B=3.01090)
         test11.addInputCell(LatticeCell("S -47.62349 -43.35998 -0.01838 -28.66778 -43.42698 -0.04721"));  // cell 3: swap (M1B=0.05157, M2A=2.96744)
         test11.addRankStabilityExpectation(FlexibleSellaTestCase::RankStabilityExpectation(
            { "M1B", "M2A" },
            "M1B/M2A best-fit rank must not flip across these three cells"));
         test11.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
            false, 1, "normal point (cell 2): no chain violation, confirmed via Test 14"));
         test11.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
            true, 2, "swap point (cell 3): real chain violation, confirmed via Test 14"));
         addTestCase(test11);
      }

      // Test 12 -- M1A/M1B orientation swap near an M2B boundary point.
      // Second entry in the boundary-case catalog. Generated the same way
      // as Test 11 (cmdgen 1 M2B | cmdperturb 20 1 | cmdsella COMPARE
      // TYPES M1A M1B M2A M2B M3 M4). Confirms the swap-partner pair is
      // NOT fixed across boundary cases -- here it is M1A/M1B that trade
      // rank, while M2A stays consistently bad (~3.6-3.8) and M2B stays
      // consistently good (~0.03-0.10) throughout, the mirror image of
      // Test 11 where M2A/M1B were the unstable pair and M3/M2B were
      // stable. (M3 and M4 fit values are large and irrelevant this far
      // from the M3 boundary -- included in the COMPARE run for
      // completeness but not part of this fixture's expectation.)
      //
      // Cell 1: exact (unperturbed) M2B point -- same round-trip caveat
      // as Test 11's cell 1 (degenerate: repeated-value pairs plus an
      // exact zero); UNCHECKED.
      // Cell 2: a "normal" perturbation (M1B good, M1A bad) -- matches
      // cell 1's own pattern.
      // Cell 3: a confirmed "swap" perturbation (M1A good, M1B bad).
      {
         FlexibleSellaTestCase test12("M1A_M1B_OrientationSwap_M2BBoundary",
            "M1A/M1B best-fit rank flips near an M2B boundary point. M2A "
            "and M2B themselves stay stable throughout (mirror image of "
            "Test 11, where M3/M2B were stable and M1B/M2A were the "
            "unstable pair). Tripwire: expected to FAIL until "
            "Remediation-style correction is confirmed for this pair.");
         test12.addInputCell(LatticeCell("S -38.13450 -43.40350 0.00000 -38.13450 -43.40350 -59.44700"));  // cell 1: exact M2B point (round-trip UNCHECKED)
         test12.addInputCell(LatticeCell("S -38.10203 -43.40929 -0.06833 -38.08135 -43.38861 -59.50431"));  // cell 2: normal (M1B=0.02068, M1A=5.30726)
         test12.addInputCell(LatticeCell("S -38.05796 -38.14164 -0.06258 -43.30057 -43.34813 -59.49953"));  // cell 3: swap (M1A=0.06806, M1B=5.22458)
         test12.addRankStabilityExpectation(FlexibleSellaTestCase::RankStabilityExpectation(
            { "M1A", "M1B" },
            "M1A/M1B best-fit rank must not flip across these three cells"));
         addTestCase(test12);
      }

      // Test 13 -- migrates the ORIGINAL O1B/O2/O3 CheckABReflectionRelation
      // check into the harness, now that CheckABReflection.h is
      // parameterized. This is the confirmed result from the earlier
      // session (residual 7.335734 at reflection index 16, not near
      // zero): candidates A and B are NOT the same lattice under any of
      // the 24 reflections. AUTOMATED (not manual) because this answer is
      // already confirmed real evidence, unlike Tests 14/15 below.
      {
         FlexibleSellaTestCase test13("O1B_O2_O3_ReflectionRelation_Confirmed",
            "Confirmed (previous session): O1B/O2/O3 boundary candidates A "
            "and B are NOT related by any of the 24 reflections (best "
            "residual 7.335734, reflection index 16). Migrated from "
            "CheckABReflection.h's original hardcoded main-line check into "
            "the harness so it has an unambiguous test number.");
         test13.addInputCell(LatticeCell("S -43.34318 -38.15350 -0.00055 -38.13490 -43.34264 -0.02616"));
         test13.addInputCell(LatticeCell("S -38.11642 -43.36206 -0.01851 -38.17840 -43.42403 -0.04436"));
         test13.addReflectionRelationExpectation(FlexibleSellaTestCase::ReflectionRelationExpectation(
            S6("-43.34318 -38.15350 -0.00055 -38.13490 -43.34264 -0.02616"),
            S6("-38.11642 -43.36206 -0.01851 -38.17840 -43.42403 -0.04436"),
            false, "O1B/O2/O3 candidate A", "O1B/O2/O3 candidate B",
            1.0E-3, "A and B are not simply reoriented forms of one another"));
         addTestCase(test13);
      }

      // Test 14 -- reflection-relation check for Test 11's M3-boundary
      // pair (M1B/M2A swap). CONFIRMED this session (best residual
      // 5.942014 at reflection index 1, not near zero) -- same signature
      // as Test 13's O1B/O2/O3 pair: not a simple reflection relation.
      // Promoted from the original MANUAL ReflectionCheck once real
      // evidence existed; the ReflectionCheck call is kept alongside so
      // the full residual table still prints for inspection.
      //
      // SEPARATE OPEN ITEM, not this test's concern: running this fixture
      // showed cell 2 (the swap point, S6 -47.62349 -43.35998 -0.01838
      // -28.66778 -43.42698 -0.04721) has HasFailure()==true -- a real
      // Grimmer chain violation, unlike cell 1 (HasFailure()==false).
      // Its fit dump also shows a duplicate "oC O4" entry (28.6634 in
      // the main list, 28.6862 appended after "hP H4") -- looks like a
      // live instance of the "remediation loop appends rather than
      // replaces" behavior flagged in the 2026-08-18 handoff, Section 0d,
      // caught for the first time on an actual swap case rather than a
      // synthetic fixture. Not investigated further here -- worth a
      // dedicated look, and worth checking whether HasFailure()==true
      // correlates with swap points generally (Test 12's cell 3 should
      // be checked the same way) before assuming it's specific to M3.
      {
         FlexibleSellaTestCase test14("M3Boundary_ReflectionRelation",
            "Confirmed this session: M3-boundary M1B/M2A swap candidates "
            "are NOT related by any of the 24 reflections (best residual "
            "5.942014, reflection index 1) -- same signature as Test 13. "
            "Cell 2 (the swap point) also has HasFailure()==true, a "
            "separate finding not covered by this expectation.");
         test14.addInputCell(LatticeCell("S -47.59416 -28.66333 -0.02968 -43.33617 -43.38776 -0.05159"));
         test14.addInputCell(LatticeCell("S -47.62349 -43.35998 -0.01838 -28.66778 -43.42698 -0.04721"));
         test14.addReflectionRelationExpectation(FlexibleSellaTestCase::ReflectionRelationExpectation(
            S6("-47.59416 -28.66333 -0.02968 -43.33617 -43.38776 -0.05159"),
            S6("-47.62349 -43.35998 -0.01838 -28.66778 -43.42698 -0.04721"),
            false, "M3 pair, normal (M2A good)", "M3 pair, swap (M1B good)",
            1.0E-3, "Test 11's cell 2 vs cell 3 -- not simply reoriented forms"));
         addTestCase(test14);
      }

      // Test 15 -- reflection-relation check for Test 12's M2B-boundary
      // pair (M1A/M1B swap). CONFIRMED this session (best residual
      // 7.386464 at reflection index 7, not near zero) -- same signature
      // as Tests 13/14. Promoted from MANUAL for the same reason.
      //
      // HasFailureExpectations below added after Test 16's manual dump
      // showed BOTH cells have HasFailure()==false at THIS specific
      // sampled point -- unlike Test 11's M3 swap point (cell 3 there has
      // a real chain violation). Do NOT read this as "M2B's boundary
      // doesn't produce chain violations" -- that would be concluding
      // something about the whole M2B boundary region from a single
      // negative sample. All this confirms is that chain-violation
      // detection is not a reliable stand-in for the rank-stability check
      // itself: it caught the problem at this one M3 point and missed it
      // at this one M2B point, so it cannot be trusted to enumerate every
      // boundary-swap case on its own (consistent with the 2026-08-19
      // handoff, Section 4: Grimmer chain failures expose SOME boundaries,
      // not necessarily all of them). Other M2B perturbations, not yet
      // generated or tested, may well trip a violation -- that remains
      // open, not ruled out.
      {
         FlexibleSellaTestCase test15("M2BBoundary_ReflectionRelation",
            "Confirmed this session: M2B-boundary M1A/M1B swap candidates "
            "are NOT related by any of the 24 reflections (best residual "
            "7.386464, reflection index 7) -- same signature as Tests "
            "13/14. All three now-confirmed boundary cases (O1B/O2/O3, "
            "M3's M1B/M2A, M2B's M1A/M1B) show the identical pattern: "
            "genuine orientation ambiguity, not a simple reflection "
            "relation. At THIS sampled point, neither cell trips a "
            "Grimmer chain violation (confirmed via Test 16), unlike Test "
            "11's M3 swap point -- a single-sample result, not evidence "
            "that M2B's boundary lacks chain violations generally.");
         test15.addInputCell(LatticeCell("S -38.10203 -43.40929 -0.06833 -38.08135 -43.38861 -59.50431"));
         test15.addInputCell(LatticeCell("S -38.05796 -38.14164 -0.06258 -43.30057 -43.34813 -59.49953"));
         test15.addReflectionRelationExpectation(FlexibleSellaTestCase::ReflectionRelationExpectation(
            S6("-38.10203 -43.40929 -0.06833 -38.08135 -43.38861 -59.50431"),
            S6("-38.05796 -38.14164 -0.06258 -43.30057 -43.34813 -59.49953"),
            false, "M2B pair, normal (M1B good)", "M2B pair, swap (M1A good)",
            1.0E-3, "Test 12's cell 2 vs cell 3 -- not simply reoriented forms"));
         test15.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
            false, 0, "normal point (cell 1): no chain violation at this sampled point, confirmed via Test 16"));
         test15.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
            false, 1, "swap point (cell 2): no chain violation at this sampled point either, confirmed via Test 16"));
         addTestCase(test15);
      }


      // Test 16 -- MANUAL. Same normal/swap pair as Test 15, but Test 15
      // is now AUTOMATED (Test 14 promotion), so it no longer prints the
      // per-cell fit/HasFailure() dump -- that dump only happens on the
      // manual (!hasAnyExpectation()) path in runTest(). This test exists
      // ONLY to surface that dump for the M2B pair, the same way it
      // showed up incidentally for the M3 pair while Test 14 was still
      // manual. No expectations attached on purpose: whether the M2B
      // swap point also has HasFailure()==true (matching Test 11's M3
      // finding) or not (meaning the chain-violation correlation is M3-
      // specific rather than general to all swap points) is not yet
      // known -- read the printed HasFailure() lines for cell 1 (normal)
      // and cell 2 (swap) once run, then report back so this can be
      // promoted to a confirmed HasFailureExpectation the same way
      // Test 11's was.
      {
         FlexibleSellaTestCase test16("M2BBoundary_SwapPoint_HasFailureCheck",
            "MANUAL: does the M2B-boundary M1A/M1B swap point also trip a "
            "real Grimmer chain violation, the way Test 11's M3 swap "
            "point did? Not yet known -- read HasFailure() for cell 1 "
            "(normal) vs cell 2 (swap) below, then report back so this "
            "can be promoted to a confirmed HasFailureExpectation.");
         test16.addInputCell(LatticeCell("S -38.10203 -43.40929 -0.06833 -38.08135 -43.38861 -59.50431"));  // cell 1: normal (M1B=0.02068, M1A=5.30726)
         test16.addInputCell(LatticeCell("S -38.05796 -38.14164 -0.06258 -43.30057 -43.34813 -59.49953"));  // cell 2: swap (M1A=0.06806, M1B=5.22458)
         addTestCase(test16);
      }

      // Test 17 -- R1/R3 orientation swap. Fourth confirmed sibling-swap
      // case, and the first from the (freeParams=2, hR) group (see the
      // sibling-group enumeration this session: {R1, R3} is the only
      // real pair there). Generated via cmdperturb on "P 10 10 10 90 90
      // 90.1" (R1/R3 touch at 90 degrees), then CmdSella COMPARE TYPES
      // R1 R3. Same signature as every other confirmed case: R1 fits
      // near-zero and R3 badly (~128) on most rows, flipping to R1 bad
      // (~115) / R3 near-zero on a confirmed minority (rows 3, 5, 6, 8 of
      // the original 10-point sweep).
      //
      // NOT the same phenomenon as T1/T2's overlap (see discussion this
      // session): T1/T2 meet at an analytically understood shared locus
      // (T1's own s=0 special case), not an orientation-relabeling
      // ambiguity -- T1/T2 deliberately has no entry here.
      //
      // Reflection-relation and HasFailure() checks for this pair have
      // NOT been run yet (unlike Tests 13-16 for the earlier three
      // cases) -- left as a follow-up, not assumed to match.
      {
         FlexibleSellaTestCase test17("R1_R3_OrientationSwap",
            "R1/R3 best-fit rank flips under small perturbation near a "
            "90-degree cell, same signature as O1B/O2, M1B/M2A, and "
            "M1A/M1B. Tripwire: expected to FAIL until Remediation-style "
            "correction is confirmed for this pair (not yet attempted). "
            "Reflection-relation and HasFailure() status for this pair "
            "not yet checked.");
         test17.addInputCell(LatticeCell("S 0.00000 0.00000 -0.17453 -99.82547 -99.82547 -100.00000"));  // cell 1: normal (R1=0.20153, R3=128.82913)
         test17.addInputCell(LatticeCell("S -0.10976 -0.07316 -0.09934 -99.66756 -99.67093 -99.57723"));  // cell 2: normal (R1=0.07976, R3=128.54453)
         test17.addInputCell(LatticeCell("S -0.06282 -0.01222 -99.84618 -99.85396 -0.13101 -99.84497"));  // cell 3: swap (R1=115.19670, R3=0.13176)
         test17.addRankStabilityExpectation(FlexibleSellaTestCase::RankStabilityExpectation(
            { "R1", "R3" },
            "R1/R3 best-fit rank must not flip across these three cells"));
         addTestCase(test17);
      }

      // Test 18 -- MANUAL. M1A vs M1B vs M2A near a DIFFERENT seed than
      // Test 12's (S6 -25.10300 -25.10300 -71.96250 -56.43450 -56.43450
      // -59.44700, not Test 12's M2B-boundary point). In this 10-point
      // sample, M1A wins every row -- M1B (~23.8) and M2A (~27.3) both
      // fit badly and consistently, no rank flip anywhere among the
      // three. Deliberately NOT asserted as a HasFailure/RankStability
      // expectation of "no swap" -- that would overclaim from one small
      // sample near one seed. All this confirms is that THIS region
      // isn't close enough to whatever boundary would expose a swap
      // (exactly as M2B's HasFailure()==false result, Test 16, did not
      // establish that M2B's boundary lacks violations generally). In
      // particular M1A/M2A -- the one pair in the (freeParams=4, mC)
      // sibling group still without a confirmed swap example -- remains
      // genuinely open, not ruled out by this result.
      {
         FlexibleSellaTestCase test18("M1A_M1B_M2A_NoSwapFound_InconclusiveSample",
            "MANUAL, inconclusive: M1A dominates M1B and M2A throughout "
            "this 10-point sample near a different seed than Test 12's. "
            "Does NOT establish that M1A/M2A (the one still-untested pair "
            "in the freeParams=4 mC sibling group) lacks a swap -- only "
            "that this particular region doesn't expose one. A denser or "
            "differently-seeded sweep is needed before concluding "
            "anything about M1A/M2A specifically.");
         test18.addInputCell(LatticeCell("S -25.10300 -25.10300 -71.96250 -56.43450 -56.43450 -59.44700"));
         test18.addInputCell(LatticeCell("S -25.05836 -25.02022 -71.96804 -56.52384 -56.43615 -59.41671"));
         addTestCase(test18);
      }

      // Test 19 -- the 16 logged mP/oC/tP Grimmer-chain-violation cases
      // from this file's CmdSella.cpp header comment (2023-12-23/24),
      // the same set TestHarness.h::RunHistoricalBadCaseCheck loops over
      // as a manual diagnostic. Promoted here to an actual pass/fail
      // regression following this session's LRL_Cell.cpp fix (the
      // LRL_Cell::LRL_Cell(const S6&) constructor was unconditionally
      // overwriting m_valid from uninitialized al/be/ga whenever a
      // boundary-transformed candidate's cosine landed at or beyond +-1
      // -- see this session's chat). Confirmed via `details` mode, cell
      // by cell, this session: all 16 now resolve (HasFailure()==false),
      // with the P 2.778.../P 2.973... cells specifically confirmed to
      // no longer show the corrupted "projected best fits" angles that
      // originally motivated the fix.
      //
      // NOTE: cell 16 (P 3.345 3.397 1.23...) is NOT like the other 15 --
      // it has no Grimmer chain violation at all (HasFailure()==false
      // even before remediation); what changes for it is
      // RemediateIfShortlistChanges widening the reported shortlist, a
      // different code path than the chain-failure repair loop
      // (Remediation168) the other 15 exercise. It's included here for
      // completeness (this group's only claim is HasFailure()==false,
      // which holds for it too either way), and broken out again on its
      // own as Test 20, which asserts on that different behavior
      // specifically -- see Test 20 for why HasFailureExpectation alone
      // doesn't test the interesting part of cell 16.
      {
         FlexibleSellaTestCase test19("HistoricalMPOCTP_16Cases_ResolveAfterRemediation",
            "The 16 historical mP/oC/tP Grimmer-chain-violation cases "
            "logged in CmdSella.cpp's 2023-12-23/24 header comment "
            "should all have HasFailure()==false after remediation, "
            "confirmed this session post the LRL_Cell.cpp S6-validity "
            "fix. Cell 16 is a genuinely different case (see Test 20) "
            "included here only because the group's claim -- "
            "HasFailure()==false -- happens to hold for it too.");
         static const char* const historicalCells[16] = {
            "P 21.775 21.785 27.232  90.028  89.974  90.023",
            "P  1.307  3.134  3.150 144.338  90.898 113.499",
            "P  1.447  2.154  1.448  93.001  93.077 118.555",
            "P  1.511  3.987  3.918 149.756 103.646  94.613",
            "P  1.909  3.372  3.412 140.217  95.846 108.922",
            "P  1.916  4.033  1.656 105.411 101.400  96.056",
            "P  1.918  1.118  1.899  94.267 130.788 116.312",
            "P  2.044  2.594  3.604 121.251  93.338  95.754",
            "P  2.092  3.512  1.736  92.747  95.706 123.113",
            "P  2.212  3.228  3.111 124.239  96.056 120.065",
            "P  2.418  2.421  1.105  91.232 105.455 146.970",
            "P  2.778  1.881  3.996 114.240  97.653  94.507",
            "P  2.891  2.299  3.839  90.545 118.914 100.136",
            "P  2.973  2.869  1.853  93.424 114.956 138.239",
            "P  3.286  1.671  1.295  98.790  90.427 114.783",
            "P  3.345  3.397  1.230  91.705 101.386 151.360"
         };
         for (int i = 0; i < 16; ++i) {
            test19.addInputCell(LatticeCell(historicalCells[i]));
            test19.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
               false, static_cast<size_t>(i), "resolves after remediation"));
         }
         addTestCase(test19);
      }

      // Test 20 -- cell 16 from Test 19, broken out on its own. Unlike
      // the other 15 cells in that group, this one has no Grimmer chain
      // violation to begin with -- HasFailure() is false before AND
      // after remediation, so a HasFailureExpectation alone (as used for
      // the other 15) would be trivially true and wouldn't actually
      // test anything specific to this cell. What's genuinely being
      // exercised here is RemediateIfShortlistChanges (CmdSella.cpp),
      // which runs unconditionally (not gated on HasFailure()) and
      // widens this cell's matched-type shortlist from empty (raw) to
      // M1A/M1B/M2A -- confirmed via `details` mode this session
      // ("remediation changed shortlist for P 3.345 3.397 1.23... --
      // raw:  remediated: M1A M1B M2A"). There is no ShortlistExpectation
      // type in this framework, and ComputeDeloneFits returns only the
      // already-widened result (RemediateIfShortlistChanges replaces the
      // fit vector outright rather than appending, unlike the chain-
      // failure repair loop), so the closest available proxy is a tight
      // FitRangeExpectation on M2A's raw fit -- if a future change
      // regresses the widening (e.g. RemediateIfShortlistChanges stops
      // firing, or reverts to the un-widened value), M2A's raw fit would
      // move well outside this window.
      {
         FlexibleSellaTestCase test20("P3345_ShortlistWidensWithoutChainFailure",
            "Unlike Test 19's other 15 cells, this one has no Grimmer "
            "chain violation at all (HasFailure()==false before and "
            "after remediation) -- what actually changes is "
            "RemediateIfShortlistChanges widening the matched-type "
            "shortlist from empty to M1A/M1B/M2A, a different code path "
            "than the chain-failure repair loop (Remediation168) every "
            "other historical cell exercises. Asserted here via M2A's "
            "widened raw fit, since HasFailureExpectation alone would "
            "be trivially true and wouldn't test the shortlist widening "
            "itself.");
         test20.addInputCell(LatticeCell("P  3.345  3.397  1.230  91.705 101.386 151.360"));
         test20.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
            false, 0, "no chain violation, before or after remediation"));
         test20.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M2A", 0.1240, 0.1250, 0,
            "M2A widened into the shortlist by RemediateIfShortlistChanges (observed 0.124645)"));
         addTestCase(test20);
      }

      //   1. Test 20: replace its FitRangeExpectation (M2A) with the one below.
//   2. Test 22: replace the whole block with the one below.
//   3. Tests 24 and 25: paste after Test 23's block, before the closing
//      brace of setupStandardTests().
// ===================================================================

// ---- 1. Test 20: replacement FitRangeExpectation ----
         // 2026-10-10: the old 0.124645 came from a candidate setting with a
         // scalar of +9.97 (10x the 10%-of-|S6| limit of 1.01 for this cell).
      test20.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
         "M2A", 0.2075, 0.2085, 0,
         "M2A inside-cone fit (observed 0.208044); was 0.124645 from a far-outside setting"));

      // ---- 2. Test 22: replacement block ----
            // Test 22 -- M4 on two cells. HISTORY: Remediation168 reported 13.0716
            // and 49.3992 (2026-08-27), later "fixed" to 9.243 and 34.9305. The
            // 2026-10-10 analysis showed those lower values came from points with
            // a scalar of +97 and +152 (cells with |S6| 126 and 180) -- far outside
            // the reduced cone. With far-outside candidate settings rejected, the
            // fits return to the inside-cone values 13.0685 and 49.3934. This test
            // now guards those.
      {
         FlexibleSellaTestCase test22("M4_InsideConeFits_NotFarOutside",
            "M4 must stay at the inside-cone values (13.0685, 49.3934), not "
            "drop to the far-outside values 9.24 / 34.93.");
         test22.addInputCell(LatticeCell("P 6.080 10.329 10.329 85.030 72.882 72.882"));
         test22.addInputCell(LatticeCell("P 11.820 13.679 13.679 79.240 64.401 64.401"));
         test22.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 13.06, 13.08, 0,
            "inside-cone M4 fit 13.0685 (far-outside value was 9.24)"));
         test22.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 49.38, 49.41, 1,
            "inside-cone M4 fit 49.3934 (far-outside value was 34.93)"));
         addTestCase(test22);
      }

      // ---- 3. Tests 24 and 25 ----
            // Test 24 -- 20 M4 cells, each a genuine 10% orthogonal S6 perturbation
            // of a cmdgen M4 cell (cmdgen 20 M4 | cmdperturb 1 100, after the
            // 2026-10-10 CmdPerturb fallback fix; every output verified at exactly
            // 0.100 of its parent's S6 length). Expected values are the 2026-10-10
            // results after rejecting candidate settings whose largest scalar exceeds
            // 10% of |S6| (Remediation168ForOneSubtype). Before that filter, all 20
            // came from points 19 to 99 outside the reduced cone and were 15-40%
            // lower. Each value is also <= the distance to its own known M4 parent.
      {
         FlexibleSellaTestCase test24("M4_Perturbed10pct_InsideConeFits",
            "20 genuine 10% perturbations of cmdgen M4 cells. M4 fit must match "
            "the inside-cone (filtered) value within 0.005 -- guards against "
            "far-outside candidate settings returning.");
         test24.addInputCell(LatticeCell("P     7.531     8.559     8.810    93.967    87.746   122.429"));
         test24.addInputCell(LatticeCell("P     7.613     7.968     8.625    89.643    86.978   114.713"));
         test24.addInputCell(LatticeCell("P     8.175    11.195     1.979    74.018    94.635   115.416"));
         test24.addInputCell(LatticeCell("P     8.216     8.762     6.196    85.817    85.743   145.338"));
         test24.addInputCell(LatticeCell("P     8.247     7.876     5.429    86.058    89.148    91.891"));
         test24.addInputCell(LatticeCell("P     8.971     7.324     5.386    91.526    97.263    95.398"));
         test24.addInputCell(LatticeCell("P     9.128     8.080     2.019   100.035    77.050   107.182"));
         test24.addInputCell(LatticeCell("P     9.182    11.702     6.396    83.638    87.358   132.657"));
         test24.addInputCell(LatticeCell("P     9.432    11.535     8.412    86.898    93.129   124.107"));
         test24.addInputCell(LatticeCell("P     9.696    12.229     6.336    85.283    80.983   126.615"));
         test24.addInputCell(LatticeCell("P     9.852    12.953     3.185    86.241    96.666   123.350"));
         test24.addInputCell(LatticeCell("P    10.033     9.318     3.526    83.034    90.830    99.547"));
         test24.addInputCell(LatticeCell("P    10.580     7.484     2.934    82.709    77.196   119.320"));
         test24.addInputCell(LatticeCell("P    10.652    13.117     2.201    76.330    64.400   129.005"));
         test24.addInputCell(LatticeCell("P    10.702     8.848     4.749    87.214    87.218   114.318"));
         test24.addInputCell(LatticeCell("P    11.025    11.252     5.658    96.462    83.404   103.515"));
         test24.addInputCell(LatticeCell("P    11.328    10.505     3.813    90.328    98.333   101.100"));
         test24.addInputCell(LatticeCell("P    12.099    11.380     4.626    93.780    94.957   131.093"));
         test24.addInputCell(LatticeCell("P    13.159    12.314     6.516    87.034    96.041   111.932"));
         test24.addInputCell(LatticeCell("P    13.321    11.490     4.172    90.883    86.579   126.717"));
         test24.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 3.68373, 3.69373, 0, "inside-cone M4 fit 3.68873"));
         test24.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 3.48305, 3.49305, 1, "inside-cone M4 fit 3.48805"));
         test24.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 1.56883, 1.57883, 2, "inside-cone M4 fit 1.57383"));
         test24.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 5.46865, 5.47865, 3, "inside-cone M4 fit 5.47365"));
         test24.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 1.61565, 1.62565, 4, "inside-cone M4 fit 1.62065"));
         test24.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 5.16097, 5.17097, 5, "inside-cone M4 fit 5.16597"));
         test24.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 1.22990, 1.23990, 6, "inside-cone M4 fit 1.23490"));
         test24.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 8.71929, 8.72929, 7, "inside-cone M4 fit 8.72429"));
         test24.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 4.42245, 4.43245, 8, "inside-cone M4 fit 4.42745"));
         test24.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 11.54077, 11.55077, 9, "inside-cone M4 fit 11.54577"));
         test24.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 2.85765, 2.86765, 10, "inside-cone M4 fit 2.86265"));
         test24.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 3.50486, 3.51486, 11, "inside-cone M4 fit 3.50986"));
         test24.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 2.02185, 2.03185, 12, "inside-cone M4 fit 2.02685"));
         test24.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 2.02646, 2.03646, 13, "inside-cone M4 fit 2.03146"));
         test24.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 3.19755, 3.20755, 14, "inside-cone M4 fit 3.20255"));
         test24.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 7.15999, 7.16999, 15, "inside-cone M4 fit 7.16499"));
         test24.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 6.25909, 6.26909, 16, "inside-cone M4 fit 6.26409"));
         test24.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 5.94767, 5.95767, 17, "inside-cone M4 fit 5.95267"));
         test24.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 6.39601, 6.40601, 18, "inside-cone M4 fit 6.40101"));
         test24.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 2.67634, 2.68634, 19, "inside-cone M4 fit 2.68134"));
         addTestCase(test24);
      }

      // Test 25 -- 22 cells near the O1B/O2/O3 boundary (from the
      // 2026-10-10 COMPARE run). Expected values are PREDICTED for the
      // filtered search (same method that reproduced Test 24's 20 values
      // to 4 decimals); confirm on the first run. O2 rises on 15 of 22
      // cells versus the unfiltered search, whose O2 points were 25-46
      // outside the cone. NOTE: O2 > O3 on several cells is expected until
      // O2's projector list gains its missing face-crossed forms (3 forms,
      // all on O1B/O3 forms) -- do not add an O2 <= O3 nesting check yet.
      {
         FlexibleSellaTestCase test25("O1B_O2_O3_Cluster_InsideConeFits",
            "22 cells near the O1B/O2/O3 boundary. O1B, O2, O3 fits must match "
            "the filtered-search values within 0.005.");
         test25.addInputCell(LatticeCell("S -43.40350 -38.13450 0.00000 -38.13450 -43.40350 -0.00000"));
         test25.addInputCell(LatticeCell("S -35.66501 -32.05498 -0.70909 -37.39343 -38.40192 -4.78523"));
         test25.addInputCell(LatticeCell("S -41.64855 -30.13990 -4.98163 -30.20533 -42.94219 -4.11050"));
         test25.addInputCell(LatticeCell("S -33.37402 -40.51017 -2.93534 -36.06738 -41.04267 -5.46859"));
         test25.addInputCell(LatticeCell("S -37.20884 -31.66894 -4.90828 -35.48589 -38.59513 -8.75683"));
         test25.addInputCell(LatticeCell("S -36.91726 -39.58748 -0.31677 -37.93953 -37.06459 -2.91900"));
         test25.addInputCell(LatticeCell("S -36.12952 -38.66622 -1.81097 -41.96920 -45.78201 -4.02314"));
         test25.addInputCell(LatticeCell("S -29.64020 -36.15816 -5.21508 -38.85461 -36.80094 -1.28571"));
         test25.addInputCell(LatticeCell("S -39.21542 -29.93236 -2.48184 -40.43034 -40.36510 -3.10222"));
         test25.addInputCell(LatticeCell("S -38.23595 -30.62884 -2.52444 -29.30673 -37.94946 -6.44290"));
         test25.addInputCell(LatticeCell("S -40.27396 -36.00550 -0.32174 -46.50095 -38.32976 -5.06335"));
         test25.addInputCell(LatticeCell("S -27.33250 -46.76050 0.00000 -27.33250 -46.76050 0.00000"));
         test25.addInputCell(LatticeCell("S -19.09182 -25.70450 -4.63218 -44.53773 -39.44671 -5.95650"));
         test25.addInputCell(LatticeCell("S -37.68675 -24.61739 -4.36708 -21.31871 -45.39452 -4.70646"));
         test25.addInputCell(LatticeCell("S -25.49638 -48.44328 -5.40932 -24.36373 -47.25758 -3.71168"));
         test25.addInputCell(LatticeCell("S -42.68949 -24.65813 -9.16684 -19.34947 -40.91043 -4.90237"));
         test25.addInputCell(LatticeCell("S -23.77561 -22.99289 -1.25834 -42.36701 -51.16149 -0.92580"));
         test25.addInputCell(LatticeCell("S -23.83560 -43.32917 -1.28589 -31.79002 -49.01062 -2.77500"));
         test25.addInputCell(LatticeCell("S -23.53057 -48.87165 -0.54992 -30.41265 -44.44479 -4.91463"));
         test25.addInputCell(LatticeCell("S -46.47390 -23.85286 -4.98516 -26.83013 -38.21334 -3.32396"));
         test25.addInputCell(LatticeCell("S -43.24616 -19.55483 -4.67494 -23.07062 -37.81048 -5.95627"));
         test25.addInputCell(LatticeCell("S -20.79748 -18.41420 -4.39473 -42.09021 -45.92328 -5.46142"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 0.00000, 0.00100, 0, "O1B filtered fit 0.00000"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 0.00000, 0.00100, 0, "O2 filtered fit 0.00000"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 0.00000, 0.00100, 0, "O3 filtered fit 0.00000"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 4.26656, 4.27656, 1, "O1B filtered fit 4.27156"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 4.29589, 4.30589, 1, "O2 filtered fit 4.30089"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 4.90849, 4.91849, 1, "O3 filtered fit 4.91349"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 3.63467, 3.64467, 2, "O1B filtered fit 3.63967"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 5.06013, 5.07013, 2, "O2 filtered fit 5.06513"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 4.29546, 4.30546, 2, "O3 filtered fit 4.30046"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 2.63651, 2.64651, 3, "O1B filtered fit 2.64151"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 3.18659, 3.19659, 3, "O2 filtered fit 3.19159"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 4.33118, 4.34118, 3, "O3 filtered fit 4.33618"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 5.72876, 5.73876, 4, "O1B filtered fit 5.73376"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 5.68153, 5.69153, 4, "O2 filtered fit 5.68653"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 6.86144, 6.87144, 4, "O3 filtered fit 6.86644"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 1.18617, 1.19617, 5, "O1B filtered fit 1.19117"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 1.20705, 1.21705, 5, "O2 filtered fit 1.21205"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 2.86566, 2.87566, 5, "O3 filtered fit 2.87066"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 4.30534, 4.31534, 6, "O1B filtered fit 4.31034"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 5.15948, 5.16948, 6, "O2 filtered fit 5.16448"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 4.31466, 4.32466, 6, "O3 filtered fit 4.31966"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 4.91203, 4.92203, 7, "O1B filtered fit 4.91703"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 4.99537, 5.00537, 7, "O2 filtered fit 5.00037"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 6.35453, 6.36453, 7, "O3 filtered fit 6.35953"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 6.91610, 6.92610, 8, "O1B filtered fit 6.92110"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 7.25541, 7.26541, 8, "O2 filtered fit 7.26041"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 7.04015, 7.05015, 8, "O3 filtered fit 7.04515"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 2.02020, 2.03020, 9, "O1B filtered fit 2.02520"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 2.69460, 2.70460, 9, "O2 filtered fit 2.69960"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 4.75338, 4.76338, 9, "O3 filtered fit 4.75838"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 5.76823, 5.77823, 10, "O1B filtered fit 5.77323"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 6.67120, 6.68120, 10, "O2 filtered fit 6.67620"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 6.67895, 6.68895, 10, "O3 filtered fit 6.68395"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 0.00000, 0.00100, 11, "O1B filtered fit 0.00000"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 0.00000, 0.00100, 11, "O2 filtered fit 0.00000"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 0.00000, 0.00100, 11, "O3 filtered fit 0.00000"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 7.24503, 7.25503, 12, "O1B filtered fit 7.25003"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 8.37968, 8.38968, 12, "O2 filtered fit 8.38468"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 7.61300, 7.62300, 12, "O3 filtered fit 7.61800"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 6.67940, 6.68940, 13, "O1B filtered fit 6.68440"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 7.35821, 7.36821, 13, "O2 filtered fit 7.36321"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 7.36603, 7.37603, 13, "O3 filtered fit 7.37103"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 1.66395, 1.67395, 14, "O1B filtered fit 1.66895"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 2.05081, 2.06081, 14, "O2 filtered fit 2.05581"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 4.23799, 4.24799, 14, "O3 filtered fit 4.24299"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 7.59032, 7.60032, 15, "O1B filtered fit 7.59532"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 9.98021, 9.99021, 15, "O2 filtered fit 9.98521"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 7.60371, 7.61371, 15, "O3 filtered fit 7.60871"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 6.27245, 6.28245, 16, "O1B filtered fit 6.27745"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 6.30649, 6.31649, 16, "O2 filtered fit 6.31149"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 6.31524, 6.32524, 16, "O3 filtered fit 6.32024"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 6.98675, 6.99675, 17, "O1B filtered fit 6.99175"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 7.06559, 7.07559, 17, "O2 filtered fit 7.07059"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 7.18157, 7.19157, 17, "O3 filtered fit 7.18657"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 6.55285, 6.56285, 18, "O1B filtered fit 6.55785"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 7.24281, 7.25281, 18, "O2 filtered fit 7.24781"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 7.26364, 7.27364, 18, "O3 filtered fit 7.26864"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 7.13478, 7.14478, 19, "O1B filtered fit 7.13978"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 7.95756, 7.96756, 19, "O2 filtered fit 7.96256"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 7.23094, 7.24094, 19, "O3 filtered fit 7.23594"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 5.64135, 5.65135, 20, "O1B filtered fit 5.64635"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 6.53784, 6.54784, 20, "O2 filtered fit 6.54284"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 6.66213, 6.67213, 20, "O3 filtered fit 6.66713"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 5.00497, 5.01497, 21, "O1B filtered fit 5.00997"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 6.32061, 6.33061, 21, "O2 filtered fit 6.32561"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 5.53013, 5.54013, 21, "O3 filtered fit 5.53513"));
         addTestCase(test25);
      }


   }
};

// runFlexibleSellaTestMode -- entry point, analogous to CmdLMP3's
// runFlexibleTestMode(controls). Called from main() when
// controls.shouldRunTests() is true. Builds its own Sella the same way the
// older CmdSellaTests.cpp registry does (each test gets a fresh, default
// Sella; this does NOT depend on controls.shouldBuildMatrices() / the
// dynamic SellaBuild matrices), so `cmdsella` then `test` then `end` works
// standalone regardless of whether BUILD was also given.
inline void runFlexibleSellaTestMode(const CmdSellaControls& controls) {
   std::cout << "; CmdSella -- Flexible Test Mode" << std::endl;

   Sella sella;
   FlexibleSellaTestRunner runner;
   runner.setupStandardTests();
   runner.runAllTests(controls, sella);
}

#endif // FLEXIBLE_SELLA_TEST_MODE_H
