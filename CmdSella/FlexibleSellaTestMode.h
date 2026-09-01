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
#include <sstream>
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
   std::vector<std::string> relevantTypeNames;  // if non-empty, MANUAL per-cell
   // fit printing (see runOneTest) shows only these GetGeneralType()
   // values instead of all 24 -- for tests that are only actually about
   // a small named subset. Empty (default) prints everything, matching
   // prior behavior for every existing test.

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
   void setRelevantTypes(const std::vector<std::string>& types) { relevantTypeNames = types; }

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

   // WrapText -- breaks a long description into lines no wider than
   // width, breaking only at spaces (never mid-word), each continuation
   // line prefixed with indent. Console-friendly rendering for the long
   // free-text description strings tests carry -- these were previously
   // dumped as a single unbroken line regardless of length.
   static void PrintWrapped(const std::string& text, const size_t width = 78,
      const std::string& indent = "") {
      size_t pos = 0;
      const size_t n = text.size();
      bool firstLine = true;
      while (pos < n) {
         size_t remaining = n - pos;
         size_t take = (remaining <= width) ? remaining : width;
         if (take < remaining) {
            size_t lastSpace = text.rfind(' ', pos + take);
            if (lastSpace != std::string::npos && lastSpace > pos) {
               take = lastSpace - pos;
            }
         }
         std::cout << (firstLine ? "" : indent) << text.substr(pos, take) << std::endl;
         pos += take;
         while (pos < n && text[pos] == ' ') ++pos;
         firstLine = false;
      }
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

   // PrintCheckResult -- composes an expectation-check result line the
   // same way every checkXxx function used to (via direct cout chains),
   // then prints it through PrintWrapped instead of as one raw line --
   // these lines combine a fixed label with a free-text description and
   // can run long. detail may be empty (omitted entirely, no trailing
   // " -- ").
   static void PrintCheckResult(const std::string& kind, const std::string& description,
      const std::string& verdict, const std::string& detail) {
      std::ostringstream oss;
      oss << "  " << kind << " (" << description << "): " << verdict;
      if (!detail.empty()) oss << " -- " << detail;
      PrintWrapped(oss.str(), 78, "      ");
   }

   bool checkFitRange(const FlexibleSellaTestCase::FitRangeExpectation& e,
      const std::vector<DeloneFitComputation>& computations) const {
      if (e.cellIndex >= computations.size()) {
         std::ostringstream detail;
         detail << "cell index " << e.cellIndex << " out of range";
         PrintCheckResult("FitRangeExpectation", e.description, "FAIL", detail.str());
         return false;
      }
      bool found = false;
      const double fit = FindRawFit(computations[e.cellIndex].fits, e.typeName, found);
      if (!found) {
         std::ostringstream detail;
         detail << "type '" << e.typeName << "' not found in cell " << (e.cellIndex + 1);
         PrintCheckResult("FitRangeExpectation", e.description, "FAIL", detail.str());
         return false;
      }
      const bool ok = (fit >= e.minFit && fit <= e.maxFit);
      std::ostringstream detail;
      detail << e.typeName << " fit " << fit << " (expected [" << e.minFit << ", "
         << e.maxFit << "] for cell " << (e.cellIndex + 1) << ")";
      PrintCheckResult("FitRangeExpectation", e.description, ok ? "PASS" : "FAIL", detail.str());
      return ok;
   }

   bool checkHasFailure(const FlexibleSellaTestCase::HasFailureExpectation& e,
      const std::vector<DeloneFitComputation>& computations) const {
      if (e.cellIndex >= computations.size()) {
         std::ostringstream detail;
         detail << "cell index " << e.cellIndex << " out of range";
         PrintCheckResult("HasFailureExpectation", e.description, "FAIL", detail.str());
         return false;
      }
      const bool actual = computations[e.cellIndex].chains.HasFailure();
      const bool ok = (actual == e.expectedHasFailure);
      std::ostringstream detail;
      detail << "HasFailure()==" << (actual ? "true" : "false")
         << " (expected " << (e.expectedHasFailure ? "true" : "false")
         << ") for cell " << (e.cellIndex + 1);
      PrintCheckResult("HasFailureExpectation", e.description, ok ? "PASS" : "FAIL", detail.str());
      return ok;
   }

   bool checkNesting(const FlexibleSellaTestCase::NestingInequalityExpectation& e,
      const std::vector<DeloneFitComputation>& computations) const {
      if (e.cellIndex >= computations.size()) {
         std::ostringstream detail;
         detail << "cell index " << e.cellIndex << " out of range";
         PrintCheckResult("NestingInequalityExpectation", e.description, "FAIL", detail.str());
         return false;
      }
      const auto& fits = computations[e.cellIndex].fits;
      bool foundParent = false, foundChild = false;
      const double parentFit = FindRawFit(fits, e.parentType, foundParent);
      const double childFit = FindRawFit(fits, e.childType, foundChild);
      if (!foundParent || !foundChild) {
         std::ostringstream detail;
         detail << (!foundParent ? e.parentType : e.childType) << " not found in cell "
            << (e.cellIndex + 1);
         PrintCheckResult("NestingInequalityExpectation", e.description, "FAIL", detail.str());
         return false;
      }
      const bool ok = (parentFit <= childFit + e.tolerance);
      std::ostringstream detail;
      detail << "fit(" << e.parentType << ")=" << parentFit << " <= fit(" << e.childType
         << ")=" << childFit << " (tolerance " << e.tolerance << ") for cell "
         << (e.cellIndex + 1);
      PrintCheckResult("NestingInequalityExpectation", e.description, ok ? "PASS" : "FAIL", detail.str());
      return ok;
   }

   bool checkRankStability(const FlexibleSellaTestCase::RankStabilityExpectation& e,
      const std::vector<DeloneFitComputation>& computations) const {
      if (computations.size() < 2) {
         std::ostringstream detail;
         detail << "needs >= 2 input cells, got " << computations.size();
         PrintCheckResult("RankStabilityExpectation", e.description, "FAIL", detail.str());
         return false;
      }

      // Best and second-best (by raw fit) among e.typeNames, per cell.
      std::vector<std::string> bestPerCell, secondPerCell;
      std::vector<double> bestFitPerCell, secondFitPerCell;
      for (const auto& comp : computations) {
         std::vector<std::pair<double, std::string>> ranked;
         for (const auto& t : e.typeNames) {
            bool found = false;
            const double fit = FindRawFit(comp.fits, t, found);
            if (found) ranked.emplace_back(fit, t);
         }
         std::sort(ranked.begin(), ranked.end());
         bestPerCell.push_back(ranked.size() > 0 ? ranked[0].second : "");
         bestFitPerCell.push_back(ranked.size() > 0 ? ranked[0].first : 0.0);
         secondPerCell.push_back(ranked.size() > 1 ? ranked[1].second : "");
         secondFitPerCell.push_back(ranked.size() > 1 ? ranked[1].first : 0.0);
      }

      bool ok = true;
      for (size_t i = 1; i < bestPerCell.size(); ++i) {
         const bool flipped = (bestPerCell[i] != bestPerCell[i - 1]);
         std::cout << "    cell " << i << " -> " << (i + 1) << ": best "
            << bestPerCell[i - 1] << " (" << bestFitPerCell[i - 1] << ") -> "
            << bestPerCell[i] << " (" << bestFitPerCell[i] << ")"
            << ", second " << secondPerCell[i - 1] << " (" << secondFitPerCell[i - 1] << ") -> "
            << secondPerCell[i] << " (" << secondFitPerCell[i] << ")"
            << (flipped ? "   ** RANK FLIP **" : "") << std::endl;
         if (flipped) ok = false;
      }
      PrintCheckResult("RankStabilityExpectation", e.description, ok ? "PASS" : "FAIL", "");
      return ok;
   }

   bool checkReflectionRelation(const FlexibleSellaTestCase::ReflectionRelationExpectation& e) const {
      const ReflectionRelationResult r = CheckABReflectionRelation(
         e.candidateA, e.candidateB, e.labelA, e.labelB, e.tolerance, true);
      const bool ok = (r.sameLattice == e.expectedSameLattice);
      std::ostringstream detail;
      detail << "sameLattice=" << (r.sameLattice ? "true" : "false")
         << " (expected " << (e.expectedSameLattice ? "true" : "false")
         << "), best residual " << r.bestResidual << " at reflection " << r.bestIndex;
      PrintCheckResult("ReflectionRelationExpectation", e.description, ok ? "PASS" : "FAIL", detail.str());
      return ok;
   }

   void runTest(const FlexibleSellaTestCase& test, const CmdSellaControls& controls, Sella& sella) {
      std::cout << "\n=== TEST: " << test.name << " ===" << std::endl;
      std::cout << "Description: ";
      PrintWrapped(test.description, 78, "             ");
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
               if (!test.relevantTypeNames.empty()) {
                  const bool isRelevant = std::find(test.relevantTypeNames.begin(),
                     test.relevantTypeNames.end(), f.GetGeneralType()) != test.relevantTypeNames.end();
                  if (!isRelevant) continue;
               }
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

      // Tests 2-7 -- originally the six known-unresolved oC-vs-tP
      // characterization fixtures (SELLA_handoff.md item 3 /
      // Cmdsellatests.cpp Test_KnownUnresolvedViolation_1..6), asserting
      // the CURRENT broken state (HasFailure()==true) as a regression
      // check.
      //
      // FLIPPED 2026-08-27: all 6 now come back HasFailure()==false when
      // run live -- the violation these cells were built to characterize
      // no longer reproduces. Checked directly against the 16-cell
      // historical set (RunHistoricalBadCaseCheck, TestHarness.h,
      // provided by Larry in reduced-S6 form) -- NOT the same cells (no
      // overlap in magnitude or pattern), so this is a second, separate
      // resolved case, not a re-confirmation of that set's already-known
      // "4 of 8" fix. Cause not confirmed -- plausibly the same general
      // 2-hop Remediation search widening (a global change, not
      // cell-specific), but that's inference, not verified via
      // Cmdsellatests.cpp (not available this session). Now asserts the
      // fixed state as a regression guard: if any of these 6 starts
      // reporting HasFailure()==true again, that's worth investigating
      // as a real regression, not expected behavior.
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
               "OC_TP_ChainViolation_" + std::to_string(i + 1) + "_Resolved",
               "This oC-vs-tP chain violation was known-unresolved as of "
               "SELLA_handoff.md item 3, but now resolves -- confirmed "
               "2026-08-27, not the same cells as the other 16-case "
               "historical set, cause not confirmed. Regression guard: "
               "should stay resolved.");
            test.addInputCell(LatticeCell(knownUnresolved[i]));
            test.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
               false, 0, "now resolves -- was known-unresolved, cause unconfirmed"));
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
            "Remediation/transform issue. Expected to keep FAILING until "
            "the library is patched.");
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
            "stable throughout (not part of the instability). Expected "
            "to keep FAILING until Remediation-style correction is "
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
            "unstable pair). Expected to keep FAILING until "
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

      // Test 17 -- R1/R3 near the hR boundary. RESOLVED this session
      // (2026-08-27): NOT a real swap/instability.
      //
      // Larry identified directly from the S6 scalars that cells 2 and 3
      // are related by a boundary transform (their near-zero triples
      // straddle the S2/S5 opposite-scalar pair differently), not a
      // genuine R1/R3 classification ambiguity. Confirmed live: running
      // this test through the normal ComputeDeloneFits path (below)
      // shows R1 and R3 both small and comparable on every cell
      // (roughly 0.08-0.23) -- the same small-fits-on-both-sides pattern
      // Test 21 already established as EXPECTED behavior for cells
      // straddling the R1/R3 boundary, not a defect.
      //
      // NOTE ON THE ORIGINAL ~128/~115 NUMBERS: this test's cell
      // comments originally recorded R3~128 (cells 1/2) and R1~115
      // (cell 3), which read as evidence of a real instability. Running
      // this test live (2026-08-27) through the actual ComputeDeloneFits
      // path shows NO such large values -- R1 and R3 are both small on
      // every cell. An earlier hypothesis this session (that
      // ComputeDeloneFits's reactive remediation gate was leaving these
      // uncorrected) is DISPROVEN by that live output -- the standard
      // path already produces the small, correct fits. Where the
      // original ~128/~115 numbers came from is unresolved -- possibly a
      // different, unremediated measurement (e.g. plain SellaFit with no
      // boundary search), possibly a different code path (CmdSella
      // COMPARE), possibly stale from an earlier build. Not asserted
      // either way; flagged here rather than guessed at further.
      //
      // No expectation is asserted -- MANUAL, like Test 21, since this
      // documents observed behavior rather than a pass/fail claim.
      {
         FlexibleSellaTestCase test17("R1_R3_NearBoundary_NoIssueFound",
            "Cells near the R1/R3 boundary. Resolved: not a swap bug -- "
            "both types fit small on every cell. See source comment for "
            "full history.");
         test17.setRelevantTypes({ "R1", "R3", "C5" });
         test17.addInputCell(LatticeCell("S 0.00000 0.00000 -0.17453 -99.82547 -99.82547 -100.00000"));  // cell 1
         test17.addInputCell(LatticeCell("S -0.10976 -0.07316 -0.09934 -99.66756 -99.67093 -99.57723"));  // cell 2
         test17.addInputCell(LatticeCell("S -0.06282 -0.01222 -99.84618 -99.85396 -0.13101 -99.84497"));  // cell 3
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

      // Test 21 -- R1/R3 spot-check at a DIFFERENT seed than Test 17's:
      // exact cubic P 10 10 10 90 90 90 (1/1000 orthogonal perturbation),
      // vs. Test 17's deliberately-off-cubic "90 90 90.1" seed. The
      // origin cell here sits exactly ON the R1/R3 boundary (C5=0), so a
      // small perturbation landing slightly on the R1 side or the R3
      // side, alternating unpredictably across the sample (R3 wins
      // perturbed cells 1-4 and 9, R1 wins 5-8 and 10), is EXPECTED,
      // CORRECT behavior for a boundary point -- not a failure signature.
      // (Two earlier reads of this same data got this wrong: first as
      // "no failure" by only checking for blow-up/NaN, then as
      // "alternation, inconclusive" by treating the alternation itself
      // as suspicious -- both wrong for the same underlying reason.)
      //
      // CORRECTED 2026-08-27: originally contrasted with what looked like
      // a "confirmed swap" in Test 17 (R1 near-zero vs R3~128). Running
      // Test 17 live disproved that -- see Test 17's own comment. Both
      // tests now show the same thing: small, comparable fits on both
      // sides of the boundary, no real R1/R3 issue anywhere.
      // MANUAL/no expectation asserted: this documents observed boundary
      // behavior for reference, not a pass/fail claim.
      {
         FlexibleSellaTestCase test21("R1_R3_CubicSeed_NoIssueFound",
            "Perturbations of an exact cubic cell, sitting on the R1/R3 "
            "boundary. Both types fit small throughout -- no issue, "
            "consistent with Test 17.");
         test21.setRelevantTypes({ "R1", "R3", "C5" });
         test21.addInputCell(LatticeCell("P 10 10 10 90 90 90"));                          // origin: C5=0.00000 R1=0.00000 R3=0.00000
         test21.addInputCell(LatticeCell("P 9.994 9.992 9.988 89.958 89.937 89.957"));      // C5=0.12544 R1=0.17416 R3=0.10127
         test21.addInputCell(LatticeCell("P 10.002 10.003 9.996 89.957 90.036 90.011"));    // C5=0.16125 R1=0.16022 R3=0.15366
         test21.addInputCell(LatticeCell("P 9.996 9.992 9.993 89.994 89.950 89.946"));      // C5=0.11589 R1=0.08860 R3=0.07979
         test21.addInputCell(LatticeCell("P 9.990 9.999 10.005 90.058 89.971 89.938"));     // C5=0.19775 R1=0.22228 R3=0.19117
         test21.addInputCell(LatticeCell("P 9.997 9.999 9.998 90.051 89.949 89.967"));      // C5=0.14354 R1=0.04598 R3=0.18490
         test21.addInputCell(LatticeCell("P 9.996 9.998 9.990 89.966 89.916 90.033"));      // C5=0.24331 R1=0.19001 R3=0.23734
         test21.addInputCell(LatticeCell("P 10.002 9.993 9.996 89.980 90.028 89.940"));     // C5=0.14211 R1=0.13121 R3=0.13344
         test21.addInputCell(LatticeCell("P 10.002 9.992 9.988 89.931 89.977 89.993"));     // C5=0.16688 R1=0.16221 R3=0.23141
         test21.addInputCell(LatticeCell("P 9.991 9.992 9.991 89.934 89.936 89.982"));      // C5=0.14200 R1=0.17326 R3=0.11044
         test21.addInputCell(LatticeCell("P 10.002 9.998 10.012 90.030 90.042 89.996"));    // C5=0.17102 R1=0.15666 R3=0.16392
         addTestCase(test21);
      }

      // Test 22 -- mP (M4) search-coverage gap. CONFIRMED this session
      // (2026-08-27) via TestHarness.h's RunStabilityTrace: on both cells
      // below, RunStabilityTrace's very first search step lands exactly
      // on the true minimum and holds it for 30 rounds (residual deltas
      // at 1e-14 floating-point-noise scale). At the time, Remediation168
      // reported a distinctly worse fit -- 13.0716 vs. true 9.243 on cell
      // 1, 49.3992 vs. true 34.9305 on cell 2.
      //
      // RESOLVED 2026-08-27, same day, before this test was even first
      // run: M4 now reaches the true minimum on both cells (9.23988,
      // 34.926) -- confirmed live, matching the pattern already seen for
      // Tests 2-7 (oC-vs-tP) and Tests 17/21 (R1/R3): three separate
      // previously-confirmed problems all turned out already fixed when
      // actually tested, none touched deliberately this session. Cause
      // not confirmed -- plausibly the same general 2-hop Remediation
      // search widening (RemediatedFit.h), but that's inference. Nesting
      // itself was always fine (M4 genuinely nests O4); this was a
      // candidate-generation gap in Remediation168's search for M4
      // specifically, apparently since fixed.
      //
      // Cells originally identified in G6 form (RunStabilityTrace
      // output); converted to P form via `checkinput` (Larry,
      // 2026-08-27) since LatticeCell's accepted string prefixes were
      // not confirmed for G6 this session:
      //   G6 36.972 106.683 106.683 18.486 36.972 36.972   -> cell 1 below
      //   G6 139.722 187.104 187.104 69.861 139.722 139.722 -> cell 2 below
      //
      // Bounds stay tight around the TRUE minimum -- now a regression
      // guard: if Remediation168's search regresses and lands short of
      // the true minimum again, this should go back to failing.
      {
         FlexibleSellaTestCase test22("M4_ReachesTrueMinimum_Remediation168",
            "M4's true minimum was missed by Remediation168 as of "
            "2026-08-27 (13.0716 vs true 9.243, 49.3992 vs true 34.9305), "
            "but now reaches it -- confirmed same day. Regression guard: "
            "should stay passing.");
         test22.addInputCell(LatticeCell("P 6.080 10.329 10.329 85.030 72.882 72.882"));    // true M4 min 9.243, was Remediation168-reported 13.0716
         test22.addInputCell(LatticeCell("P 11.820 13.679 13.679 79.240 64.401 64.401"));   // true M4 min 34.9305, was Remediation168-reported 49.3992
         test22.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 9.0, 9.5, 0,
            "should stay at true minimum ~9.243, not regress toward old 13.0716"));
         test22.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 34.5, 35.5, 1,
            "should stay at true minimum ~34.9305, not regress toward old 49.3992"));
         addTestCase(test22);
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