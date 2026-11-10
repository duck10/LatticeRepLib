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
#include "ConventionalCell.h"
#include "DeloneFitComputation.h"
#include "DeloneFitResults.h"
#include "DeloneParentageTest.h"
#include "FaceCrossedArrangements.h"
#include "G6.h"
#include "GenerateLatticeTypeExamples.h"
#include "GrimmerTree.h"
#include "LatticeCell.h"
#include "LRL_Cell.h"
#include "MatG6.h"
#include "MatS6.h"
#include "RemediatedFit.h"
#include "S6.h"
#include "Sella.h"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <set>
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

   // ParentageTableExpectation -- checks TestDeloneParentage
   // (DeloneParentageTest.h) over every ordered pair of Delone types with
   // child free parameters < parent free parameters, the same pair set
   // CHECKNESTING uses. expectedPairs ("PARENT>CHILD") must be exactly the
   // pairs found nested: a missing pair or an extra pair both fail. Needs
   // no input cells.
   struct ParentageTableExpectation {
      std::set<std::string> expectedPairs;
      std::string description;

      ParentageTableExpectation(const std::set<std::string>& pairs, const std::string& desc = "")
         : expectedPairs(pairs), description(desc) {
      }
   };

   // FaceCrossedLibraryExpectation -- checks FaceCrossedArrangements.h:
   // VerifyFaceCrossedArrangements reports no problems, and each type has
   // the expected number of face arrangements (Appendix B). Needs no
   // input cells.
   struct FaceCrossedLibraryExpectation {
      std::map<std::string, size_t> expectedCounts;
      std::string description;

      FaceCrossedLibraryExpectation(const std::map<std::string, size_t>& counts,
         const std::string& desc = "")
         : expectedCounts(counts), description(desc) {
      }
   };

   // ConventionalCellLibraryExpectation (2026-11-08) -- runs
   // CheckConventionalCellsAllArrangements (ConventionalCell.h): generic
   // members of every library and face arrangement of every Delone type
   // reach the type's canonical arrangement, and their conventional cells
   // have the type's symmetry and centering. Needs no input cells.
   struct ConventionalCellLibraryExpectation {
      size_t expectedArrangements;
      size_t samplesPerArrangement;
      std::string description;

      ConventionalCellLibraryExpectation(const size_t arrangements, const size_t samples,
         const std::string& desc = "")
         : expectedArrangements(arrangements), samplesPerArrangement(samples), description(desc) {
      }
   };

   // ConventionalCellExpectation (2026-11-08) -- for one input cell and one
   // Delone type, the two conventional cells from ConventionalCellsForFit:
   //   ideal -- centering letter, a b c alpha beta gamma, each within tolerance;
   //   input -- sorted edges, and sorted |angle - 90|, each within tolerance
   //            (independent of which axis comes first).
   struct ConventionalCellExpectation {
      std::string typeName;
      size_t cellIndex;
      char centering;
      std::vector<double> idealCell;                 // a b c alpha beta gamma
      std::vector<double> inputSortedEdges;          // 3 values, ascending
      std::vector<double> inputSortedAngleOffsets;   // 3 values of |angle - 90|, ascending
      double edgeTol;
      double angleTol;
      std::string description;
      std::vector<double> idealCellI;   // optional: the I-centered ideal cell (mC, large beta); empty = not checked

      ConventionalCellExpectation(const std::string& type, const size_t cell, const char cent,
         const std::vector<double>& ideal, const std::vector<double>& edges,
         const std::vector<double>& angleOffsets, const double eTol, const double aTol,
         const std::string& desc = "")
         : typeName(type), cellIndex(cell), centering(cent), idealCell(ideal),
         inputSortedEdges(edges), inputSortedAngleOffsets(angleOffsets),
         edgeTol(eTol), angleTol(aTol), description(desc) {
      }
   };

   // TypedBasisExpectation (2026-11-09) -- the 3x3 from the typed cell to each
   // conventional cell (AddTypedBasis, ConventionalCell.h), checked without
   // freezing any matrix, so a later choice of setting (status note item 7)
   // does not break it. The input cells must all be the SAME lattice, typed
   // in different settings. For each cell and each named type:
   //   1. round-trip and Gram errors <= basisTol;
   //   2. det = conventional lattice points / typed lattice points;
   //   3. every row, and every centering vector of the conventional cell,
   //      is a vector of the typed lattice;
   // and the same for the I2/m cell when there is one. Against cell 1: the
   // same fit within fitTol, and the same ideal conventional cell (centering,
   // sorted edges within edgeTol, sorted angles within angleTol).
   struct TypedBasisExpectation {
      std::vector<std::string> typeNames;
      double basisTol;
      double fitTol;
      double edgeTol;
      double angleTol;
      std::string description;

      TypedBasisExpectation(const std::vector<std::string>& types, const double bTol,
         const double fTol, const double eTol, const double aTol, const std::string& desc = "")
         : typeNames(types), basisTol(bTol), fitTol(fTol), edgeTol(eTol), angleTol(aTol),
         description(desc) {
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
   std::vector<ParentageTableExpectation> parentageTableExpectations;
   std::vector<FaceCrossedLibraryExpectation> faceCrossedLibraryExpectations;
   std::vector<ConventionalCellLibraryExpectation> conventionalCellLibraryExpectations;
   std::vector<ConventionalCellExpectation> conventionalCellExpectations;
   std::vector<TypedBasisExpectation> typedBasisExpectations;

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
   void addParentageTableExpectation(const ParentageTableExpectation& e) { parentageTableExpectations.push_back(e); }
   void addFaceCrossedLibraryExpectation(const FaceCrossedLibraryExpectation& e) { faceCrossedLibraryExpectations.push_back(e); }
   void addConventionalCellLibraryExpectation(const ConventionalCellLibraryExpectation& e) { conventionalCellLibraryExpectations.push_back(e); }
   void addConventionalCellExpectation(const ConventionalCellExpectation& e) { conventionalCellExpectations.push_back(e); }
   void addTypedBasisExpectation(const TypedBasisExpectation& e) { typedBasisExpectations.push_back(e); }

   // True if any expectation needs fits computed from input cells. Tests
   // with only library checks (parentage table, face-crossed library)
   // need no cells.
   bool needsInputCells() const {
      return !fitRangeExpectations.empty() || !hasFailureExpectations.empty()
         || !nestingExpectations.empty() || !rankStabilityExpectations.empty()
         || !conventionalCellExpectations.empty() || !typedBasisExpectations.empty()
         || (parentageTableExpectations.empty() && faceCrossedLibraryExpectations.empty()
            && conventionalCellLibraryExpectations.empty()
            && reflectionRelationExpectations.empty());
   }

   bool hasAnyExpectation() const {
      return !fitRangeExpectations.empty() || !hasFailureExpectations.empty()
         || !nestingExpectations.empty() || !rankStabilityExpectations.empty()
         || !reflectionRelationExpectations.empty()
         || !parentageTableExpectations.empty() || !faceCrossedLibraryExpectations.empty()
         || !conventionalCellLibraryExpectations.empty() || !conventionalCellExpectations.empty()
         || !typedBasisExpectations.empty();
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

   bool checkParentageTable(const FlexibleSellaTestCase::ParentageTableExpectation& e) const {
      const auto allTypes = GenerateDeloneBase().Select("");
      std::set<std::string> found;
      size_t pairsChecked = 0;
      for (const auto& parent : allTypes) {
         for (const auto& child : allTypes) {
            if (parent->GetName() == child->GetName()) continue;
            if (child->GetFreeParams() >= parent->GetFreeParams()) continue;
            ++pairsChecked;
            if (TestDeloneParentage(*parent, *child).isNested) {
               found.insert(parent->GetName() + ">" + child->GetName());
            }
         }
      }
      size_t missing = 0;
      size_t extra = 0;
      for (const auto& pr : e.expectedPairs) {
         if (found.count(pr) == 0) {
            ++missing;
            std::cout << "    missing nested pair: " << pr << std::endl;
         }
      }
      for (const auto& pr : found) {
         if (e.expectedPairs.count(pr) == 0) {
            ++extra;
            std::cout << "    unexpected nested pair: " << pr << std::endl;
         }
      }
      const bool ok = (missing == 0 && extra == 0);
      std::ostringstream detail;
      detail << pairsChecked << " pair(s) checked, " << found.size() << " nested (expected "
         << e.expectedPairs.size() << "), " << missing << " missing, " << extra << " unexpected";
      PrintCheckResult("ParentageTableExpectation", e.description, ok ? "PASS" : "FAIL", detail.str());
      return ok;
   }

   bool checkFaceCrossedLibrary(const FlexibleSellaTestCase::FaceCrossedLibraryExpectation& e) const {
      std::ostringstream problemsText;
      const size_t problems = VerifyFaceCrossedArrangements(problemsText);
      if (problems > 0) std::cout << problemsText.str();
      size_t countMismatches = 0;
      size_t expectedTotal = 0;
      for (const auto& entry : e.expectedCounts) {
         expectedTotal += entry.second;
         const size_t actual = FaceCrossedArrangements(entry.first).size();
         if (actual != entry.second) {
            ++countMismatches;
            std::cout << "    " << entry.first << ": " << actual
               << " face arrangement(s), expected " << entry.second << std::endl;
         }
      }
      const size_t total = CountFaceCrossedArrangements();
      const bool ok = (problems == 0 && countMismatches == 0 && total == expectedTotal);
      std::ostringstream detail;
      detail << total << " arrangement(s) (expected " << expectedTotal << "), "
         << problems << " verification problem(s), " << countMismatches
         << " per-type count mismatch(es)";
      PrintCheckResult("FaceCrossedLibraryExpectation", e.description, ok ? "PASS" : "FAIL", detail.str());
      return ok;
   }

   bool checkConventionalCellLibrary(const FlexibleSellaTestCase::ConventionalCellLibraryExpectation& e) const {
      const ConventionalCellCheckCounts counts =
         CheckConventionalCellsAllArrangements(std::cout, e.samplesPerArrangement);
      const bool ok = (counts.failures == 0 && counts.arrangements == e.expectedArrangements
         && counts.samples > 0);
      std::ostringstream detail;
      detail << counts.arrangements << " arrangement(s) (expected " << e.expectedArrangements
         << "), " << counts.samples << " sample(s), " << counts.skippedInvalid
         << " invalid sample(s) skipped, " << counts.failures << " failure(s)";
      PrintCheckResult("ConventionalCellLibraryExpectation", e.description, ok ? "PASS" : "FAIL", detail.str());
      return ok;
   }

   static std::vector<double> SortedValues(const double x, const double y, const double z) {
      std::vector<double> v = { x, y, z };
      std::sort(v.begin(), v.end());
      return v;
   }

   static bool WithinTolerance(const std::vector<double>& actual, const std::vector<double>& expected,
      const double tol) {
      if (actual.size() != expected.size()) return false;
      for (size_t i = 0; i < actual.size(); ++i) {
         if (std::fabs(actual[i] - expected[i]) > tol) return false;
      }
      return true;
   }

   bool checkConventionalCell(const FlexibleSellaTestCase::ConventionalCellExpectation& e,
      const std::vector<DeloneFitComputation>& computations) const {
      if (e.cellIndex >= computations.size()) {
         PrintCheckResult("ConventionalCellExpectation", e.description, "FAIL", "no such input cell");
         return false;
      }
      const DeloneFitResults* fit = nullptr;
      for (const auto& f : computations[e.cellIndex].fits) {
         if (f.GetGeneralType() == e.typeName) {
            fit = &f;
            break;
         }
      }
      if (fit == nullptr) {
         PrintCheckResult("ConventionalCellExpectation", e.description, "FAIL", "type not in the fits");
         return false;
      }
      const ConventionalCellResult r = ConventionalCellsForFit(*fit);
      if (!r.ok) {
         PrintCheckResult("ConventionalCellExpectation", e.description, "FAIL",
            "no conventional cell (canonical arrangement not reached)");
         return false;
      }
      const std::vector<double> ideal = { r.ideal.a, r.ideal.b, r.ideal.c,
         r.ideal.alpha, r.ideal.beta, r.ideal.gamma };
      const std::vector<double> idealEdges(ideal.begin(), ideal.begin() + 3);
      const std::vector<double> idealAngles(ideal.begin() + 3, ideal.end());
      const std::vector<double> expectedEdges(e.idealCell.begin(), e.idealCell.begin() + 3);
      const std::vector<double> expectedAngles(e.idealCell.begin() + 3, e.idealCell.end());
      const std::vector<double> inputEdges = SortedValues(r.input.a, r.input.b, r.input.c);
      const std::vector<double> inputOffsets = SortedValues(std::fabs(r.input.alpha - 90.0),
         std::fabs(r.input.beta - 90.0), std::fabs(r.input.gamma - 90.0));

      bool ok = r.ideal.centering == e.centering && r.input.centering == e.centering
         && WithinTolerance(idealEdges, expectedEdges, e.edgeTol)
         && WithinTolerance(idealAngles, expectedAngles, e.angleTol)
         && WithinTolerance(inputEdges, e.inputSortedEdges, e.edgeTol)
         && WithinTolerance(inputOffsets, e.inputSortedAngleOffsets, e.angleTol);
      std::vector<std::string> cellLines;
      cellLines.push_back("idealized " + FormatConventionalCell(r.ideal));
      cellLines.push_back("input   " + FormatConventionalCell(r.input));
      std::string note;
      if (!e.idealCellI.empty()) {
         if (!r.hasAlternateI) {
            ok = false;
            note = "no I-centered cell";
         }
         else {
            const std::vector<double> idealI = { r.idealI.a, r.idealI.b, r.idealI.c,
               r.idealI.alpha, r.idealI.beta, r.idealI.gamma };
            ok = ok && r.idealI.centering == 'I'
               && WithinTolerance(std::vector<double>(idealI.begin(), idealI.begin() + 3),
                  std::vector<double>(e.idealCellI.begin(), e.idealCellI.begin() + 3), e.edgeTol)
               && WithinTolerance(std::vector<double>(idealI.begin() + 3, idealI.end()),
                  std::vector<double>(e.idealCellI.begin() + 3, e.idealCellI.end()), e.angleTol);
            cellLines.push_back("idealized " + FormatConventionalCell(r.idealI));
            cellLines.push_back("input   " + FormatConventionalCell(r.inputI));
         }
      }
      PrintCheckResult("ConventionalCellExpectation", e.description, ok ? "PASS" : "FAIL", note);
      for (const auto& line : cellLines) std::cout << "      " << line << std::endl;
      return ok;
   }

   // --- TypedBasisExpectation helpers (2026-11-09) ---

   // The entry of a type with the smallest raw fit, or nullptr.
   static const DeloneFitResults* FindBestFit(const std::vector<DeloneFitResults>& fits,
      const std::string& typeName) {
      const DeloneFitResults* best = nullptr;
      for (const auto& f : fits) {
         if (f.GetGeneralType() != typeName) continue;
         if (best == nullptr || f.GetRawFit() < best->GetRawFit()) best = &f;
      }
      return best;
   }

   // Centering of the cell as typed: P, I, A, B, C, F, or R (rhombohedral
   // centering on hexagonal axes). R or H read as rhombohedral axes
   // (LatSymMatG6 gives the unit matrix) is primitive. Other letters (S, G,
   // ...) are primitive representations.
   static char TypedCentering(const LatticeCell& cell) {
      const std::string lattice = cell.getLatticeType();
      if (lattice.empty()) return 'P';
      const char letter = static_cast<char>(std::toupper(static_cast<unsigned char>(lattice[0])));
      if (letter == 'R' || letter == 'H') {
         return LRL_Cell::LatSymMatG6(lattice, cell.getCell()).IsUnit() ? 'P' : 'R';
      }
      if (letter == 'I' || letter == 'A' || letter == 'B' || letter == 'C' || letter == 'F') return letter;
      return 'P';
   }

   static size_t LatticePointsPerCell(const char centering) {
      if (centering == 'P') return 1;
      if (centering == 'F') return 4;
      if (centering == 'R') return 3;
      return 2;   // I, A, B, C
   }

   // Centering translations of a cell, in its own coordinates.
   static std::vector<std::vector<double>> CenteringTranslations(const char centering) {
      if (centering == 'I') return { { 0.5, 0.5, 0.5 } };
      if (centering == 'A') return { { 0.0, 0.5, 0.5 } };
      if (centering == 'B') return { { 0.5, 0.0, 0.5 } };
      if (centering == 'C') return { { 0.5, 0.5, 0.0 } };
      if (centering == 'F') return { { 0.0, 0.5, 0.5 }, { 0.5, 0.0, 0.5 }, { 0.5, 0.5, 0.0 } };
      if (centering == 'R') return { { 2.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0 }, { 1.0 / 3.0, 2.0 / 3.0, 2.0 / 3.0 } };
      return {};
   }

   static bool IsIntegerVector(const double v[3]) {
      const double tol = 1.0E-6;
      for (size_t i = 0; i < 3; ++i) {
         if (std::fabs(v[i] - std::round(v[i])) > tol) return false;
      }
      return true;
   }

   // True if v (in the typed axes) is a lattice vector of the typed cell.
   static bool InTypedLattice(const double v[3], const char typedCentering) {
      if (IsIntegerVector(v)) return true;
      for (const auto& t : CenteringTranslations(typedCentering)) {
         const double w[3] = { v[0] - t[0], v[1] - t[1], v[2] - t[2] };
         if (IsIntegerVector(w)) return true;
      }
      return false;
   }

   // Checks 2 and 3 for one basis matrix M (rows: the conventional axes in
   // the typed axes). Empty if both hold, else what failed.
   static std::string CheckBasisLattice(const Matrix3& M, const char conventionalCentering,
      const char typedCentering) {
      const double expectedDet = static_cast<double>(LatticePointsPerCell(conventionalCentering))
         / static_cast<double>(LatticePointsPerCell(typedCentering));
      const double det = Determinant3(M);
      if (std::fabs(det - expectedDet) > 1.0E-6) {
         std::ostringstream os;
         os << "det " << det << " (expected " << expectedDet << ")";
         return os.str();
      }
      for (size_t i = 0; i < 3; ++i) {
         double row[3];
         MatrixRow(M, i, row);
         if (!InTypedLattice(row, typedCentering)) {
            return "row " + std::to_string(i + 1) + " is not a lattice vector";
         }
      }
      for (const auto& t : CenteringTranslations(conventionalCentering)) {
         double v[3];
         for (size_t j = 0; j < 3; ++j) {
            v[j] = t[0] * M.m[0][j] + t[1] * M.m[1][j] + t[2] * M.m[2][j];
         }
         if (!InTypedLattice(v, typedCentering)) return "a centering vector is not a lattice vector";
      }
      return std::string();
   }

   // Same centering, sorted edges and sorted angles within tolerance.
   static bool SameCellUpToAxisOrder(const ConventionalCellParameters& p,
      const ConventionalCellParameters& q, const double edgeTol, const double angleTol) {
      return p.centering == q.centering
         && WithinTolerance(SortedValues(p.a, p.b, p.c), SortedValues(q.a, q.b, q.c), edgeTol)
         && WithinTolerance(SortedValues(p.alpha, p.beta, p.gamma),
            SortedValues(q.alpha, q.beta, q.gamma), angleTol);
   }

   bool checkTypedBasis(const FlexibleSellaTestCase::TypedBasisExpectation& e,
      const FlexibleSellaTestCase& test, const std::vector<DeloneFitComputation>& computations) const {
      std::vector<std::string> problems;
      size_t matricesChecked = 0;
      double worstError = 0.0;
      for (size_t c = 0; c < computations.size() && c < test.inputCells.size(); ++c) {
         const LatticeCell& cell = test.inputCells[c];
         MatS6 typedToReduced;
         GetInputSellingReducedVectors(cell, typedToReduced);
         const S6 typedS6 = S6(G6(cell.getCell()));
         const char typedCentering = TypedCentering(cell);
         for (const auto& type : e.typeNames) {
            const std::string where = "cell " + std::to_string(c + 1) + " " + type;
            const DeloneFitResults* fit = FindBestFit(computations[c].fits, type);
            if (fit == nullptr) {
               problems.push_back(where + ": type not in the fits");
               continue;
            }
            ConventionalCellResult r = ConventionalCellsForFit(*fit);
            AddTypedBasis(r, *fit, typedToReduced, typedS6);
            if (!r.ok || !r.hasTypedBasis) {
               problems.push_back(where + ": no conventional cell");
               continue;
            }
            worstError = std::max(worstError,
               std::max(r.typedBasisRoundTripError, r.typedBasisGramError));
            if (r.typedBasisRoundTripError > e.basisTol || r.typedBasisGramError > e.basisTol) {
               std::ostringstream os;
               os << where << ": round trip " << r.typedBasisRoundTripError
                  << ", Gram " << r.typedBasisGramError;
               problems.push_back(os.str());
            }
            ++matricesChecked;
            std::string p = CheckBasisLattice(r.typedToConventional, r.ideal.centering, typedCentering);
            if (!p.empty()) problems.push_back(where + ": " + p);
            if (r.hasAlternateI) {
               ++matricesChecked;
               p = CheckBasisLattice(r.typedToConventionalI, 'I', typedCentering);
               if (!p.empty()) problems.push_back(where + " (I2/m): " + p);
            }
            if (c == 0) continue;
            const DeloneFitResults* first = FindBestFit(computations[0].fits, type);
            if (first == nullptr) continue;   // reported for cell 1
            if (std::fabs(fit->GetRawFit() - first->GetRawFit()) > e.fitTol) {
               std::ostringstream os;
               os << where << ": fit " << fit->GetRawFit() << ", cell 1 " << first->GetRawFit();
               problems.push_back(os.str());
            }
            const ConventionalCellResult r0 = ConventionalCellsForFit(*first);
            if (r0.ok && !SameCellUpToAxisOrder(r.ideal, r0.ideal, e.edgeTol, e.angleTol)) {
               problems.push_back(where + ": ideal cell " + FormatConventionalCell(r.ideal)
                  + ", cell 1 " + FormatConventionalCell(r0.ideal));
            }
         }
      }
      const bool ok = problems.empty();
      std::ostringstream detail;
      detail << matricesChecked << " basis matrices over " << computations.size()
         << " cells, largest check error " << worstError << ", " << problems.size() << " problem(s)";
      PrintCheckResult("TypedBasisExpectation", e.description, ok ? "PASS" : "FAIL", detail.str());
      const size_t maxPrinted = 10;
      for (size_t i = 0; i < problems.size() && i < maxPrinted; ++i) {
         std::cout << "      " << problems[i] << std::endl;
      }
      return ok;
   }

   void runTest(const FlexibleSellaTestCase& test, const CmdSellaControls& controls, Sella& sella) {
      std::cout << "\n=== TEST: " << test.name << " ===" << std::endl;
      std::cout << "Description: ";
      PrintWrapped(test.description, 78, "             ");
      displayTestInputs(test);

      if (test.inputCells.empty() && test.needsInputCells()) {
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
      if (!test.parentageTableExpectations.empty()) {
         std::cout << "\nChecking parentage-table expectations:" << std::endl;
         for (const auto& e : test.parentageTableExpectations)
            testPassed = checkParentageTable(e) && testPassed;
      }
      if (!test.faceCrossedLibraryExpectations.empty()) {
         std::cout << "\nChecking face-crossed library expectations:" << std::endl;
         for (const auto& e : test.faceCrossedLibraryExpectations)
            testPassed = checkFaceCrossedLibrary(e) && testPassed;
      }
      if (!test.conventionalCellLibraryExpectations.empty()) {
         std::cout << "\nChecking conventional-cell library expectations:" << std::endl;
         for (const auto& e : test.conventionalCellLibraryExpectations)
            testPassed = checkConventionalCellLibrary(e) && testPassed;
      }
      if (!test.conventionalCellExpectations.empty()) {
         std::cout << "\nChecking conventional-cell expectations:" << std::endl;
         for (const auto& e : test.conventionalCellExpectations)
            testPassed = checkConventionalCell(e, computations) && testPassed;
      }
      if (!test.typedBasisExpectations.empty()) {
         std::cout << "\nChecking typed-cell basis expectations:" << std::endl;
         for (const auto& e : test.typedBasisExpectations)
            testPassed = checkTypedBasis(e, test, computations) && testPassed;
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
      // motivated this session (SELLA_handoff.md item 1). The cell is an
      // exact O3 point whose reduced form has the pattern ab0ab0, which lies
      // on an O1B form and, after a face permutation, on an O2 form. All
      // three fits must therefore be zero (was O1B=10 before the
      // boundary-transform fix).
      // Parentage (checked 2026-10-10 from the projector forms): every O3
      // lattice is O1B and O2, but of O3's 9 forms, 3 lie on O1B forms and
      // 6 on O2 forms, none on both. A general NestingInequalityExpectation
      // (O1B or O2 over O3) is valid only after the missing face-crossed
      // projectors are added (O1B 6, O2 3); until then use exact-fit ranges.
      {
         FlexibleSellaTestCase test8("O1B_O2_O3_BoundaryPoint",
            "Exact O3 point S -10 -20 0 -10 -20 0 (form ab0ab0): O1B, O2 and O3 "
            "must all fit at zero. Original motivating issue: the raw fit table "
            "gave O1B=10, O2=0, O3=0. Now a regression guard.");
         test8.addInputCell(LatticeCell("S -10 -20 0 -10 -20 0"));
         static const char* const o3Family8[3] = { "O1B", "O2", "O3" };
         for (size_t t = 0; t < 3; ++t) {
            test8.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
               o3Family8[t], 0.0, 1.0E-6, 0,
               std::string(o3Family8[t]) + " exactly 0 on exact O3 point"));
         }
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
            // Remediation). Originally a tripwire expected to FAIL until
            // CreatePerps_O2()/CreatePrjs_O2() were patched.
            //
            // 2026-10-29: PASSES. Observed in the full suite run: O1B stays
            // best (0 -> 0.0131635 -> 0.0646143) and O2 second throughout --
            // no rank flip. The change that made it pass was not recorded;
            // the fit values at cell 3 (O1B 0.0646, O2 0.0672) no longer
            // match the failure described above (O1B ~0.065, O2 ~5.25).
            // Kept as a regression guard.
            //
            // 2026-11-06 (CONEFIT): the 2026-10-29 pass came from outside-cone
            // artifacts -- Remediation's candidates projected to points
            // outside the reduced cone, which are not O1B/O2/O3 lattices.
            // With the cone fit the order flips between cells again (O2 best
            // on cell 2, O1B best on cell 3). Nothing requires the best type
            // to keep its rank across two different cells near a triple
            // point, so the rank check is replaced by the nesting bounds
            // O1B <= O3 and O2 <= O3, which must hold, plus fit-range guards
            // on the cone-fit values.
      {
         FlexibleSellaTestCase test9("O2_WithinType_LibraryGap_MinimalFixture",
            "Three cells at a former O2 library-gap point near the O1B/O2/O3 "
            "triple point. O1B <= O3 and O2 <= O3 on every cell, and O1B, O2 "
            "fits at the cone-fit values (2026-11-06). The former rank check "
            "passed only because of outside-cone artifacts.");
         test9.addInputCell(LatticeCell("S -38.13450 -43.40350 0.00000 -38.13450 -43.40350 -0.00000"));
         test9.addInputCell(LatticeCell("S -43.34318 -38.15350 -0.00055 -38.13490 -43.34264 -0.02616"));
         test9.addInputCell(LatticeCell("S -38.11642 -43.36206 -0.01851 -38.17840 -43.42403 -0.04436"));
         test9.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 0.0, 0.001, 0, "cell 1 (exact point): O1B cone fit 0"));
         test9.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 0.0, 0.001, 0, "cell 1 (exact point): O2 cone fit 0"));
         test9.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 0.0237976, 0.0337976, 1, "cell 2: O1B cone fit 0.0287976"));
         test9.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 0.0081692, 0.0181692, 1, "cell 2: O2 cone fit 0.0131692"));
         test9.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 0.0596143, 0.0696143, 2, "cell 3: O1B cone fit 0.0646143"));
         test9.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 0.0646545, 0.0746545, 2, "cell 3: O2 cone fit 0.0696545"));
         for (size_t cell = 0; cell < 3; ++cell) {
            test9.addNestingExpectation(FlexibleSellaTestCase::NestingInequalityExpectation(
               "O1B", "O3", 1.0E-6, cell, "O1B <= O3, cell " + std::to_string(cell + 1)));
            test9.addNestingExpectation(FlexibleSellaTestCase::NestingInequalityExpectation(
               "O2", "O3", 1.0E-6, cell, "O2 <= O3, cell " + std::to_string(cell + 1)));
         }
         addTestCase(test9);
      }

      // Test 10 -- O1B/O2/O3 triple-point fit sensitivity. Both cells sit
      // very close to the O1B/O2/O3 triple point (cmddists NCDist = 0.137
      // between them). Originally fit(O1B) swung 5.19875 (A) vs 0.06462
      // (B) and the test was left MANUAL until the cause was known. The
      // orientation-coverage-gap hypothesis was disproven earlier
      // (DiagnoseO1BCoverage.h: identical residual across all 24
      // reflections).
      //
      // Automated 2026-11-04: the swing is gone. Cell A now gives
      // O1B 0.0131635, O2 0.0131692, O3 0.0287976; cell B gives
      // O1B 0.0646143, O2 0.0671500, O3 0.0696545. The values are the same
      // with FACECROSSED on and off, so the face arrangements did not fix
      // it -- one of the earlier fit-pipeline fixes did. The test is now a
      // regression guard: if the swing returns, the O1B range on cell A
      // fails.
      //
      // 2026-11-06 (CONEFIT): O1B on cell A rises from 0.0131635 to
      // 0.0287976 (= O3, the nesting bound met with equality) and O2 on
      // cell B from 0.06715 to 0.0696545 (= O3). The old values came from
      // Remediation candidates whose projections lay outside the reduced
      // cone, i.e. distances to points that are not O1B/O2 lattices.
      {
         FlexibleSellaTestCase test10("O1B_O2_O3_TriplePoint_FitSensitivity",
            "Two close cells (NCDist 0.137) near the O1B/O2/O3 triple point. "
            "fit(O1B) once swung 5.19875 (A) vs 0.06462 (B); now small on both. "
            "Guards against the swing returning: O1B, O2, O3 ranges on both "
            "cells, plus O1B <= O3 and O2 <= O3 nesting.");
         test10.addInputCell(LatticeCell("S -43.34318 -38.15350 -0.00055 -38.13490 -43.34264 -0.02616"));  // candidate A
         test10.addInputCell(LatticeCell("S -38.11642 -43.36206 -0.01851 -38.17840 -43.42403 -0.04436"));  // candidate B
         test10.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 0.0237976, 0.0337976, 0, "cell A: O1B cone fit 0.0287976 (was 0.0131635)"));
         test10.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 0.0081692, 0.0181692, 0, "cell A: O2 = 0.0131692"));
         test10.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 0.0237976, 0.0337976, 0, "cell A: O3 = 0.0287976"));
         test10.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 0.0596143, 0.0696143, 1, "cell B: O1B = 0.0646143"));
         test10.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 0.0646545, 0.0746545, 1, "cell B: O2 cone fit 0.0696545 (was 0.0671500)"));
         test10.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 0.0646545, 0.0746545, 1, "cell B: O3 = 0.0696545"));
         for (size_t cell = 0; cell < 2; ++cell) {
            test10.addNestingExpectation(FlexibleSellaTestCase::NestingInequalityExpectation(
               "O1B", "O3", 1.0E-6, cell, "O1B <= O3, cell " + std::to_string(cell + 1)));
            test10.addNestingExpectation(FlexibleSellaTestCase::NestingInequalityExpectation(
               "O2", "O3", 1.0E-6, cell, "O2 <= O3, cell " + std::to_string(cell + 1)));
         }
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
            "M1B/M2A near an M3 boundary point, same signature as the O1B/O2/O3 case. "
            "M3 and M2B stay stable throughout. Cell 2 must show no chain violation. "
            "Cell 3 had an oC>tP violation (28.6862 vs 28.6679); it is now resolved by "
            "Bravais containment (oC includes tP's lattices). M1B/M2A best-fit rank "
            "must not flip across the three cells. Now a regression guard.");
         test11.addInputCell(LatticeCell("S -43.40350 -47.63950 0.00000 -28.62950 -43.40350 -0.00000"));  // cell 1: exact M3 point (round-trip UNCHECKED, see caveat above)
         test11.addInputCell(LatticeCell("S -47.59416 -28.66333 -0.02968 -43.33617 -43.38776 -0.05159"));  // cell 2: normal (M2A=0.06319, M1B=3.01090)
         test11.addInputCell(LatticeCell("S -47.62349 -43.35998 -0.01838 -28.66778 -43.42698 -0.04721"));  // cell 3: swap (M1B=0.05157, M2A=2.96744)
         test11.addRankStabilityExpectation(FlexibleSellaTestCase::RankStabilityExpectation(
            { "M1B", "M2A" },
            "M1B/M2A best-fit rank must not flip across these three cells"));
         test11.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
            false, 1, "normal point (cell 2): no chain violation, confirmed via Test 14"));
         // 2026-10-10: cell 3's violation was oC > tP (28.6862 vs 28.6679);
         // resolved by Bravais containment (oC includes tP's lattices).
         test11.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
            false, 2, "swap point (cell 3): oC>tP violation resolved by containment"));
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
      //
      // 2026-10-29: PASSES. Observed in the full suite run: M1A and M1B
      // now fit IDENTICALLY on every cell (0, 0.02068, 0.0680599), so
      // there is no rank flip. The per-cell values in the comments on the
      // input lines below (M1A=5.30726 at cell 2, M1B=5.22458 at cell 3)
      // are the original swap values, no longer what the code produces.
      // Note the pass is by a tie, not by a stable ordering: if M1A and
      // M1B ever diverge again, this fixture is where it will show.
      // The change that produced the tie was not recorded.
      //
      // 2026-11-06 (CONEFIT): the tie came from outside-cone artifacts. With
      // the cone fit the original swap is back: M1B best on cell 2 (0.02068
      // vs M1A 0.0713908), M1A best on cell 3 (0.0680599 vs M1B 0.0924576).
      // A swap between two mC siblings on either side of an M2B point is
      // allowed geometry, so the rank check is replaced by the nesting
      // bounds M1A <= M2B and M1B <= M2B (both are confirmed nested pairs,
      // Test 29) plus fit-range guards on the cone-fit values.
      {
         FlexibleSellaTestCase test12("M1A_M1B_OrientationSwap_M2BBoundary",
            "M1A/M1B near an M2B boundary point. M1A <= M2B and M1B <= M2B on "
            "every cell, and M1A, M1B fits at the cone-fit values "
            "(2026-11-06): M1B better on cell 2, M1A better on cell 3. The "
            "former tie came from outside-cone artifacts.");
         test12.addInputCell(LatticeCell("S -38.13450 -43.40350 0.00000 -38.13450 -43.40350 -59.44700"));  // cell 1: exact M2B point (round-trip UNCHECKED)
         test12.addInputCell(LatticeCell("S -38.10203 -43.40929 -0.06833 -38.08135 -43.38861 -59.50431"));  // cell 2: normal (M1B=0.02068, M1A=5.30726)
         test12.addInputCell(LatticeCell("S -38.05796 -38.14164 -0.06258 -43.30057 -43.34813 -59.49953"));  // cell 3: swap (M1A=0.06806, M1B=5.22458)
         test12.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M1A", 0.0, 0.001, 0, "cell 1 (exact M2B point): M1A cone fit 0"));
         test12.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M1B", 0.0, 0.001, 0, "cell 1 (exact M2B point): M1B cone fit 0"));
         test12.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M1B", 0.01568, 0.02568, 1, "cell 2: M1B cone fit 0.02068"));
         test12.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M1A", 0.0663908, 0.0763908, 1, "cell 2: M1A cone fit 0.0713908"));
         test12.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M1A", 0.0630599, 0.0730599, 2, "cell 3: M1A cone fit 0.0680599"));
         test12.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M1B", 0.0874576, 0.0974576, 2, "cell 3: M1B cone fit 0.0924576"));
         for (size_t cell = 0; cell < 3; ++cell) {
            test12.addNestingExpectation(FlexibleSellaTestCase::NestingInequalityExpectation(
               "M1A", "M2B", 1.0E-6, cell, "M1A <= M2B, cell " + std::to_string(cell + 1)));
            test12.addNestingExpectation(FlexibleSellaTestCase::NestingInequalityExpectation(
               "M1B", "M2B", 1.0E-6, cell, "M1B <= M2B, cell " + std::to_string(cell + 1)));
         }
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
      //
      // 2026-10-29: the HasFailure()==true finding above is out of date.
      // That violation was oC > tP (28.6862 vs 28.6679) and was resolved
      // 2026-10-10 by Bravais containment; Test 11 now asserts
      // HasFailure()==false for this same cell and passes.
      {
         FlexibleSellaTestCase test14("M3Boundary_ReflectionRelation",
            "Confirmed: M3-boundary M1B/M2A swap candidates are NOT related "
            "by any of the 24 reflections (best residual 5.942014, "
            "reflection index 1) -- same signature as Test 13. (The swap "
            "point's former oC>tP chain violation was resolved 2026-10-10 "
            "by containment; see Test 11.)");
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
      //
      // ANSWERED and AUTOMATED 2026-11-06: HasFailure() is false on both
      // cells, so the M2B swap point does not trip a chain violation (the
      // M3 correlation in Test 11 is not general). With the cone fit, M1B
      // is better on cell 1 (0.02068 vs M1A 0.0713908) and M1A on cell 2
      // (0.0680599 vs M1B 0.0924576) -- the swap, which is allowed geometry
      // (see Test 12). Guards: HasFailure false, M1A/M1B fit ranges, and
      // M1A <= M2B, M1B <= M2B.
      {
         FlexibleSellaTestCase test16("M2BBoundary_SwapPoint_HasFailureCheck",
            "M2B-boundary M1A/M1B swap point: no Grimmer chain violation on "
            "either cell (answered 2026-11-06). Guards HasFailure false, the "
            "M1A/M1B cone-fit values, and M1A, M1B <= M2B.");
         test16.addInputCell(LatticeCell("S -38.10203 -43.40929 -0.06833 -38.08135 -43.38861 -59.50431"));  // cell 1: normal (M1B=0.02068, M1A=5.30726)
         test16.addInputCell(LatticeCell("S -38.05796 -38.14164 -0.06258 -43.30057 -43.34813 -59.49953"));  // cell 2: swap (M1A=0.06806, M1B=5.22458)
         test16.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
            false, 0, "cell 1 (normal): no chain violation"));
         test16.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
            false, 1, "cell 2 (swap): no chain violation"));
         test16.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M1B", 0.01568, 0.02568, 0, "cell 1: M1B cone fit 0.02068"));
         test16.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M1A", 0.0663908, 0.0763908, 0, "cell 1: M1A cone fit 0.0713908"));
         test16.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M1A", 0.0630599, 0.0730599, 1, "cell 2: M1A cone fit 0.0680599"));
         test16.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M1B", 0.0874576, 0.0974576, 1, "cell 2: M1B cone fit 0.0924576"));
         for (size_t cell = 0; cell < 2; ++cell) {
            test16.addNestingExpectation(FlexibleSellaTestCase::NestingInequalityExpectation(
               "M1A", "M2B", 1.0E-6, cell, "M1A <= M2B, cell " + std::to_string(cell + 1)));
         }
         for (size_t cell = 0; cell < 2; ++cell) {
            test16.addNestingExpectation(FlexibleSellaTestCase::NestingInequalityExpectation(
               "M1B", "M2B", 1.0E-6, cell, "M1B <= M2B, cell " + std::to_string(cell + 1)));
         }
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
      // AUTOMATED 2026-11-06: was MANUAL (observed behavior only). Now a
      // regression guard on the current cone-fit values of C5, R1 and R3,
      // plus HasFailure false. The R1/R3 <= C5 nesting on these cells is
      // checked in Test 27.
      {
         FlexibleSellaTestCase test17("R1_R3_NearBoundary_NoIssueFound",
            "Cells near the R1/R3 boundary: not a swap bug -- both types fit "
            "small on every cell. Guards the C5/R1/R3 cone-fit values and "
            "HasFailure false (nesting in Test 27).");
         test17.setRelevantTypes({ "R1", "R3", "C5" });
         test17.addInputCell(LatticeCell("S 0.00000 0.00000 -0.17453 -99.82547 -99.82547 -100.00000"));  // cell 1
         test17.addInputCell(LatticeCell("S -0.10976 -0.07316 -0.09934 -99.66756 -99.67093 -99.57723"));  // cell 2
         test17.addInputCell(LatticeCell("S -0.06282 -0.01222 -99.84618 -99.85396 -0.13101 -99.84497"));  // cell 3
         test17.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "C5", 0.220317, 0.230317, 0, "cell 1: C5 0.225317"));
         test17.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R1", 0.19653, 0.20653, 0, "cell 1: R1 0.20153"));
         test17.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R3", 0.220317, 0.230317, 0, "cell 1: R3 0.225317"));
         test17.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
            false, 0, "cell 1: no chain violation"));
         test17.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "C5", 0.176434, 0.186434, 1, "cell 2: C5 0.181434"));
         test17.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R1", 0.0747581, 0.0847581, 1, "cell 2: R1 0.0797581"));
         test17.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R3", 0.176434, 0.186434, 1, "cell 2: R3 0.181434"));
         test17.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
            false, 1, "cell 2: no chain violation"));
         test17.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "C5", 0.136206, 0.146206, 2, "cell 3: C5 0.141206"));
         test17.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R1", 0.136206, 0.146206, 2, "cell 3: R1 0.141206"));
         test17.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R3", 0.114616, 0.124616, 2, "cell 3: R3 0.119616"));
         test17.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
            false, 2, "cell 3: no chain violation"));
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
      //
      // CLOSED and AUTOMATED 2026-11-06: the swap question no longer needs
      // an answer. Test 12 (with the cone fit) showed a sibling swap is
      // allowed geometry, not a library-gap symptom, and library gaps are
      // now checked directly for every type (Tests 29, 30; CHECKNESTING
      // with perturbed cells; CHECKREMEDIATION found 0 missing
      // arrangements). These two cells could not show an M1A/M2A swap in
      // any case: cell 1 is an exact M1A cell and M1B, M2A are far away
      // (about 24 and 27). Kept as a regression guard.
      {
         FlexibleSellaTestCase test18("M1A_M1B_M2A_NoSwapFound_InconclusiveSample",
            "An exact M1A cell and a perturbation of it: M1A near 0, M1B and "
            "M2A far (about 24 and 27). Guards those cone-fit values, "
            "HasFailure false and M1A <= M2B. The former swap question is "
            "closed (see source comment).");
         test18.addInputCell(LatticeCell("S -25.10300 -25.10300 -71.96250 -56.43450 -56.43450 -59.44700"));
         test18.addInputCell(LatticeCell("S -25.05836 -25.02022 -71.96804 -56.52384 -56.43615 -59.41671"));
         test18.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M1A", 0, 0.001, 0, "cell 1 (exact M1A): M1A 0"));
         test18.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M1B", 23.8519, 23.8619, 0, "cell 1: M1B 23.8569"));
         test18.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M2A", 27.3943, 27.4043, 0, "cell 1: M2A 27.3993"));
         test18.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M1A", 0.0626173, 0.0726173, 1, "cell 2: M1A 0.0676173"));
         test18.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M1B", 23.9167, 23.9267, 1, "cell 2: M1B 23.9217"));
         test18.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M2A", 27.2947, 27.3047, 1, "cell 2: M2A 27.2997"));
         test18.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
            false, 0, "cell 1: no chain violation"));
         test18.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
            false, 1, "cell 2: no chain violation"));
         for (size_t cell = 0; cell < 2; ++cell) {
            test18.addNestingExpectation(FlexibleSellaTestCase::NestingInequalityExpectation(
               "M1A", "M2B", 1.0E-6, cell, "M1A <= M2B, cell " + std::to_string(cell + 1)));
         }
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
         // 2026-10-10: the old 0.124645 came from a candidate setting with a
         // scalar of +9.97 (10x the 10%-of-|S6| limit of 1.01 for this cell).
         test20.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M2A", 0.2075, 0.2085, 0,
            "M2A inside-cone fit (observed 0.208044); was 0.124645 from a far-outside setting"));
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
      // AUTOMATED 2026-11-06: was MANUAL. Now a regression guard on the
      // current cone-fit values of C5, R1 and R3 on all 11 cells, plus
      // HasFailure false. (The per-cell comments on the input lines are
      // the older values; several changed with FACECROSSED/CONEFIT.) The
      // R1/R3 <= C5 nesting on these cells is checked in Test 27.
      {
         FlexibleSellaTestCase test21("R1_R3_CubicSeed_NoIssueFound",
            "Perturbations of an exact cubic cell, on the R1/R3 boundary. "
            "Both types fit small throughout. Guards the C5/R1/R3 cone-fit "
            "values and HasFailure false (nesting in Test 27).");
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
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "C5", 0, 0.001, 0, "cell 1: C5 0"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R1", 0, 0.001, 0, "cell 1: R1 0"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R3", 0, 0.001, 0, "cell 1: R3 0"));
         test21.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
            false, 0, "cell 1: no chain violation"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "C5", 0.120444, 0.130444, 1, "cell 2: C5 0.125444"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R1", 0.120444, 0.130444, 1, "cell 2: R1 0.125444"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R3", 0.096275, 0.106275, 1, "cell 2: R3 0.101275"));
         test21.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
            false, 1, "cell 2: no chain violation"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "C5", 0.156248, 0.166248, 2, "cell 3: C5 0.161248"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R1", 0.155223, 0.165223, 2, "cell 3: R1 0.160223"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R3", 0.148664, 0.158664, 2, "cell 3: R3 0.153664"));
         test21.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
            false, 2, "cell 3: no chain violation"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "C5", 0.110886, 0.120886, 3, "cell 4: C5 0.115886"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R1", 0.0835978, 0.0935978, 3, "cell 4: R1 0.0885978"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R3", 0.0747907, 0.0847907, 3, "cell 4: R3 0.0797907"));
         test21.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
            false, 3, "cell 4: no chain violation"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "C5", 0.192751, 0.202751, 4, "cell 5: C5 0.197751"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R1", 0.192751, 0.202751, 4, "cell 5: R1 0.197751"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R3", 0.18617, 0.19617, 4, "cell 5: R3 0.19117"));
         test21.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
            false, 4, "cell 5: no chain violation"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "C5", 0.138543, 0.148543, 5, "cell 6: C5 0.143543"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R1", 0.0409772, 0.0509772, 5, "cell 6: R1 0.0459772"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R3", 0.138543, 0.148543, 5, "cell 6: R3 0.143543"));
         test21.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
            false, 5, "cell 6: no chain violation"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "C5", 0.238315, 0.248315, 6, "cell 7: C5 0.243315"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R1", 0.185012, 0.195012, 6, "cell 7: R1 0.190012"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R3", 0.232345, 0.242345, 6, "cell 7: R3 0.237345"));
         test21.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
            false, 6, "cell 7: no chain violation"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "C5", 0.137105, 0.147105, 7, "cell 8: C5 0.142105"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R1", 0.126211, 0.136211, 7, "cell 8: R1 0.131211"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R3", 0.128442, 0.138442, 7, "cell 8: R3 0.133442"));
         test21.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
            false, 7, "cell 8: no chain violation"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "C5", 0.161881, 0.171881, 8, "cell 9: C5 0.166881"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R1", 0.161881, 0.171881, 8, "cell 9: R1 0.166881"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R3", 0.161881, 0.171881, 8, "cell 9: R3 0.166881"));
         test21.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
            false, 8, "cell 9: no chain violation"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "C5", 0.136999, 0.146999, 9, "cell 10: C5 0.141999"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R1", 0.136999, 0.146999, 9, "cell 10: R1 0.141999"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R3", 0.105443, 0.115443, 9, "cell 10: R3 0.110443"));
         test21.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
            false, 9, "cell 10: no chain violation"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "C5", 0.166022, 0.176022, 10, "cell 11: C5 0.171022"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R1", 0.151655, 0.161655, 10, "cell 11: R1 0.156655"));
         test21.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R3", 0.158915, 0.168915, 10, "cell 11: R3 0.163915"));
         test21.addHasFailureExpectation(FlexibleSellaTestCase::HasFailureExpectation(
            false, 10, "cell 11: no chain violation"));
         addTestCase(test21);
      }

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

      // Test 23 -- exact O3 cells from `cmdgen 10 O3` (originally run
      // 2026-08-16; re-run 2026-10-09 through the main pipeline with
      // CLASSIFYDIRECT FALSE, TYPES O3 O2 O1B). All three types should fit
      // an exact O3 cell at ~0: O3 trivially, O1B and O2 because O3 lies on
      // their boundary. Input is rounded to 3 decimals, which by itself
      // produces fits of ~1e-4 to ~1e-3 (observed 2e-4 to 9e-4 on the good
      // cells), so the [0, 0.01] window is about ten times that noise.
      //
      // Cell 1 is the one sample of the ten that still fails: O2 = 1.24496
      // vs O1B 0.00075 / O3 0.00087. Same value as in the August run --
      // untouched by every remediation change since. Its angles are near
      // 120, and O2's best projection lands on the cF/oF/tI junction cell
      // (9.103 9.103 9.103 120 120 90, same 1.24496 as cF/oF/tI), i.e. O2
      // reaches this O3 point only through the orientation that passes
      // through cF. Expected to FAIL until that is fixed.
      //
      // 2026-10-29: cell 1 now PASSES. Observed in the full suite run:
      // O1B 0.000750, O2 0.000866, O3 0.000866 -- all at rounding-noise
      // level, like cells 2-4. The change that fixed O2 here was not
      // recorded. All four cells are now regression guards.
      //
      // Cells 2-4 are passing samples from the same batch, kept as
      // regression guards. Cell 4's O2 winner is a strongly transformed
      // representation (9.227 13.108 11.335 ...), so it also guards the
      // remediation path, not only the direct projector fit.
      {
         FlexibleSellaTestCase test23("ExactO3Cells_O1B_O2_O3_NearZero",
            "Exact O3 cells from cmdgen 10 O3: O1B, O2 and O3 should all fit "
            "at ~0 (window [0, 0.01], ~10x the 3-decimal rounding noise). "
            "All four cells pass as of 2026-10-29, including cell 1 (angles "
            "near 120, near the cF/tI junction), which used to give "
            "O2 = 1.245; what fixed it was not recorded. Regression guard.");
         test23.addInputCell(LatticeCell("P  9.103  9.103  9.103 119.504 120.498 90.000"));  // cell 1: formerly failed (O2 1.24496); passes 2026-10-29 (O2 0.000866)
         test23.addInputCell(LatticeCell("P  9.030  9.030  9.030 117.884 122.162 90.000"));  // cell 2: passes (~2e-4)
         test23.addInputCell(LatticeCell("P  8.608  8.608  8.608 111.647 129.132 90.000"));  // cell 3: passes (~4e-4)
         test23.addInputCell(LatticeCell("P 11.335 11.335 11.335 109.351 131.963 90.000"));  // cell 4: passes (~9e-4)
         static const char* const o3Family[3] = { "O1B", "O2", "O3" };
         for (size_t cell = 0; cell < 4; ++cell) {
            for (size_t t = 0; t < 3; ++t) {
               test23.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
                  o3Family[t], 0.0, 0.01, cell,
                  std::string(o3Family[t]) + " ~0 on exact O3 cell " + std::to_string(cell + 1)));
            }
         }
         addTestCase(test23);
      }

      // Test 24 -- 20 M4 cells, each a genuine 10% orthogonal S6 perturbation
      // of a cmdgen M4 cell (cmdgen 20 M4 | cmdperturb 1 100, after the
      // 2026-10-10 CmdPerturb fallback fix; every output verified at exactly
      // 0.100 of its parent's S6 length). Expected values are the 2026-10-10
      // results after rejecting candidate settings whose largest scalar exceeds
      // 10% of |S6| (Remediation168ForOneSubtype). Before that filter, all 20
      // came from points 19 to 99 outside the reduced cone and were 15-40%
      // lower. Each value is also <= the distance to its own known M4 parent.
      //
      // 2026-11-06 (CONEFIT): cells 5 and 6 rise (1.62065 -> 2.24436,
      // 5.16597 -> 6.19820) to their direct fits: the 10% filter limited how
      // far outside the cone a candidate could be, but a candidate inside
      // the limit could still project outside the cone. Both new values stay
      // below the distance to the known parent (9.04 and 8.58).
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
            "M4", 2.23936, 2.24936, 4, "M4 cone fit 2.24436 (was 1.62065, outside-cone artifact)"));
         test24.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 6.1932, 6.2032, 5, "M4 cone fit 6.19820 (was 5.16597, outside-cone artifact)"));
         test24.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 1.2299, 1.2399, 6, "inside-cone M4 fit 1.23490"));
         test24.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 8.71929, 8.72929, 7, "inside-cone M4 fit 8.72429"));
         test24.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 4.42245, 4.43245, 8, "inside-cone M4 fit 4.42745"));
         test24.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "M4", 11.5408, 11.5508, 9, "inside-cone M4 fit 11.54577"));
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
      // outside the cone.
      //
      // 2026-11-04: values are now those with FACECROSSED on (the default).
      // Without the face arrangements O2 > O3 on 7 cells (3, 7, 9, 13, 16,
      // 20, 22) -- nesting violations, since every O3 lattice is an O2
      // lattice. O2's three borrowed O3 arrangements bring O2 down to
      // exactly fit(O3) on those cells; nothing else changes. The O2 <= O3
      // and O1B <= O3 nesting checks are now added for every cell.
      // Run with FACECROSSED 0 and this test fails on those 7 cells.
      //
      // 2026-11-06 (CONEFIT): 18 O1B/O2 values rise (descriptions give the
      // old values), most to exactly fit(O3) -- the nesting bound met with
      // equality. The old values were distances to projected points outside
      // the reduced cone, which are not O1B/O2 lattices.
      {
         FlexibleSellaTestCase test25("O1B_O2_O3_Cluster_InsideConeFits",
            "22 cells near the O1B/O2/O3 boundary. O1B, O2, O3 fits must match "
            "the filtered-search values within 0.005 (FACECROSSED on), and "
            "O1B <= O3, O2 <= O3 must hold on every cell.");
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
            "O1B", 0, 0.001, 0, "O1B filtered fit 0.00000"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 0, 0.001, 0, "O2 filtered fit 0.00000"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 0, 0.001, 0, "O3 filtered fit 0.00000"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 4.29313, 4.30313, 1, "O1B cone fit 4.29813 (was 4.27156, outside-cone artifact)"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 4.29589, 4.30589, 1, "O2 filtered fit 4.30089"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 4.90849, 4.91849, 1, "O3 filtered fit 4.91349"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 4.29546, 4.30546, 2, "O1B cone fit 4.30046 (was 3.63967, outside-cone artifact)"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 4.29546, 4.30546, 2, "O2 fit 4.30046 with FACECROSSED (= O3; was 5.06513)"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 4.29546, 4.30546, 2, "O3 filtered fit 4.30046"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 2.63651, 2.64651, 3, "O1B filtered fit 2.64151"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 4.33118, 4.34118, 3, "O2 cone fit 4.33618 (was 3.19159, outside-cone artifact)"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 4.33118, 4.34118, 3, "O3 filtered fit 4.33618"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 5.72876, 5.73876, 4, "O1B filtered fit 5.73376"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 5.68153, 5.69153, 4, "O2 filtered fit 5.68653"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 6.86144, 6.87144, 4, "O3 filtered fit 6.86644"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 2.65786, 2.66786, 5, "O1B cone fit 2.66286 (was 1.19117, outside-cone artifact)"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 1.20705, 1.21705, 5, "O2 filtered fit 1.21205"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 2.86566, 2.87566, 5, "O3 filtered fit 2.87066"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 4.30534, 4.31534, 6, "O1B filtered fit 4.31034"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 4.31466, 4.32466, 6, "O2 fit 4.31966 with FACECROSSED (= O3; was 5.16448)"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 4.31466, 4.32466, 6, "O3 filtered fit 4.31966"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 6.35453, 6.36453, 7, "O1B cone fit 6.35953 (was 4.91703, outside-cone artifact)"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 4.99537, 5.00537, 7, "O2 filtered fit 5.00037"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 6.35453, 6.36453, 7, "O3 filtered fit 6.35953"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 7.04015, 7.05015, 8, "O1B cone fit 7.04515 (was 6.92110, outside-cone artifact)"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 7.04015, 7.05015, 8, "O2 fit 7.04515 with FACECROSSED (= O3; was 7.26041)"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 7.04015, 7.05015, 8, "O3 filtered fit 7.04515"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 4.75338, 4.76338, 9, "O1B cone fit 4.75838 (was 2.02520, outside-cone artifact)"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 2.6946, 2.7046, 9, "O2 filtered fit 2.69960"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 4.75338, 4.76338, 9, "O3 filtered fit 4.75838"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 5.76823, 5.77823, 10, "O1B filtered fit 5.77323"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 6.67895, 6.68895, 10, "O2 cone fit 6.68395 (was 6.67620, outside-cone artifact)"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 6.67895, 6.68895, 10, "O3 filtered fit 6.68395"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 0, 0.001, 11, "O1B filtered fit 0.00000"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 0, 0.001, 11, "O2 filtered fit 0.00000"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 0, 0.001, 11, "O3 filtered fit 0.00000"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 7.613, 7.623, 12, "O1B cone fit 7.61800 (was 7.25003, outside-cone artifact)"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 7.613, 7.623, 12, "O2 fit 7.61800 with FACECROSSED (= O3; was 8.38468)"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 7.613, 7.623, 12, "O3 filtered fit 7.61800"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 7.36603, 7.37603, 13, "O1B cone fit 7.37103 (was 6.68440, outside-cone artifact)"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 7.35821, 7.36821, 13, "O2 filtered fit 7.36321"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 7.36603, 7.37603, 13, "O3 filtered fit 7.37103"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 1.66395, 1.67395, 14, "O1B filtered fit 1.66895"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 4.23799, 4.24799, 14, "O2 cone fit 4.24299 (was 2.05581, outside-cone artifact)"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 4.23799, 4.24799, 14, "O3 filtered fit 4.24299"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 7.60371, 7.61371, 15, "O1B cone fit 7.60871 (was 7.59532, outside-cone artifact)"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 7.60371, 7.61371, 15, "O2 fit 7.60871 with FACECROSSED (= O3; was 9.98521)"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 7.60371, 7.61371, 15, "O3 filtered fit 7.60871"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 6.31524, 6.32524, 16, "O1B cone fit 6.32024 (was 6.27745, outside-cone artifact)"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 6.30649, 6.31649, 16, "O2 filtered fit 6.31149"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 6.31524, 6.32524, 16, "O3 filtered fit 6.32024"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 6.98675, 6.99675, 17, "O1B filtered fit 6.99175"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 7.18157, 7.19157, 17, "O2 cone fit 7.18657 (was 7.07059, outside-cone artifact)"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 7.18157, 7.19157, 17, "O3 filtered fit 7.18657"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 6.55285, 6.56285, 18, "O1B filtered fit 6.55785"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 7.26364, 7.27364, 18, "O2 cone fit 7.26864 (was 7.24781, outside-cone artifact)"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 7.26364, 7.27364, 18, "O3 filtered fit 7.26864"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 7.23094, 7.24094, 19, "O1B cone fit 7.23594 (was 7.13978, outside-cone artifact)"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 7.23094, 7.24094, 19, "O2 fit 7.23594 with FACECROSSED (= O3; was 7.96256)"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 7.23094, 7.24094, 19, "O3 filtered fit 7.23594"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 6.66213, 6.67213, 20, "O1B cone fit 6.66713 (was 5.64635, outside-cone artifact)"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 6.53784, 6.54784, 20, "O2 filtered fit 6.54284"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 6.66213, 6.67213, 20, "O3 filtered fit 6.66713"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O1B", 5.53013, 5.54013, 21, "O1B cone fit 5.53513 (was 5.00997, outside-cone artifact)"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 5.53013, 5.54013, 21, "O2 fit 5.53513 with FACECROSSED (= O3; was 6.32561)"));
         test25.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O3", 5.53013, 5.54013, 21, "O3 filtered fit 5.53513"));
         // Nesting: O3 lies on both O1B and O2, so neither may fit worse.
         for (size_t cell = 0; cell < 22; ++cell) {
            test25.addNestingExpectation(FlexibleSellaTestCase::NestingInequalityExpectation(
               "O1B", "O3", 1.0E-6, cell, "O1B <= O3, cell " + std::to_string(cell + 1)));
            test25.addNestingExpectation(FlexibleSellaTestCase::NestingInequalityExpectation(
               "O2", "O3", 1.0E-6, cell, "O2 <= O3, cell " + std::to_string(cell + 1)));
         }
         addTestCase(test25);
      }

      // Test 26 (2026-11-04) -- the 10 cells with the largest O2 > O3 gap in
      // the 220-cell batch (cmdgen 20 O3 | cmdperturb 10 10, i.e. 1%
      // perturbations of exact O3 cells; COMPARE TYPES O1B O2 O3). Without
      // the face arrangements O2 fit worse than O3 on 93 of the 220 cells;
      // these are the worst ten (gap 0.31 to 0.44). With FACECROSSED on, O2
      // equals O3 on every one. Cells are the reduced S6 forms printed by
      // COMPARE, rounded to 5 decimals. Guards the O2 face arrangements
      // (three borrowed O3 forms, Appendix B).
      {
         FlexibleSellaTestCase test26("O2_O3_PerturbedBatch_WorstGaps",
            "Worst 10 of 93 O2 > O3 nesting violations from a 1% O3 perturbation "
            "batch. With FACECROSSED on: O2 <= O3 and O1B <= O3 on every cell, "
            "and O2 equal to O3 within 0.005. Fails with FACECROSSED 0.");
         test26.addInputCell(LatticeCell("S -84.53054 -41.94090 -1.61148 -42.11853 -85.25417 -0.85722"));
         test26.addInputCell(LatticeCell("S -66.40445 -47.52225 -1.55024 -48.08136 -66.53153 -0.84167"));
         test26.addInputCell(LatticeCell("S -86.70862 -88.20316 -0.72031 -94.08331 -94.79407 -1.90061"));
         test26.addInputCell(LatticeCell("S -53.93641 -54.63274 -0.80099 -76.10422 -75.97609 -1.52340"));
         test26.addInputCell(LatticeCell("S -33.76551 -68.91324 -1.46886 -68.74946 -33.35640 -0.62815"));
         test26.addInputCell(LatticeCell("S -59.08488 -59.86765 -0.83031 -68.98858 -68.95271 -1.51263"));
         test26.addInputCell(LatticeCell("S -94.60383 -88.14536 -1.72712 -86.36404 -93.93528 -0.90751"));
         test26.addInputCell(LatticeCell("S -75.91755 -54.12860 -1.58612 -54.61158 -75.15713 -1.05635"));
         test26.addInputCell(LatticeCell("S -66.69698 -47.76546 -1.38036 -48.56256 -66.37047 -0.64899"));
         test26.addInputCell(LatticeCell("S -42.18369 -85.31241 -1.22931 -84.76345 -42.65861 -0.73687"));
         test26.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 1.25251, 1.26251, 0, "O2 = O3 = 1.25751 (was 1.69543 without face arrangements)"));
         test26.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 1.16754, 1.17754, 1, "O2 = O3 = 1.17254 (was 1.60238 without face arrangements)"));
         test26.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 1.80645, 1.81645, 2, "O2 = O3 = 1.81145 (was 2.23198 without face arrangements)"));
         test26.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 1.18417, 1.19417, 3, "O2 = O3 = 1.18917 (was 1.60356 without face arrangements)"));
         test26.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 1.08974, 1.09974, 4, "O2 = O3 = 1.09474 (was 1.50155 without face arrangements)"));
         test26.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 1.20412, 1.21412, 5, "O2 = O3 = 1.20912 (was 1.61091 without face arrangements)"));
         test26.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 1.81307, 1.82307, 6, "O2 = O3 = 1.81807 (was 2.18929 without face arrangements)"));
         test26.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 1.33749, 1.34749, 7, "O2 = O3 = 1.34249 (was 1.70925 without face arrangements)"));
         test26.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 1.14699, 1.15699, 8, "O2 = O3 = 1.15199 (was 1.50877 without face arrangements)"));
         test26.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "O2", 1.01917, 1.02917, 9, "O2 = O3 = 1.02417 (was 1.33216 without face arrangements)"));
         for (size_t cell = 0; cell < 10; ++cell) {
            test26.addNestingExpectation(FlexibleSellaTestCase::NestingInequalityExpectation(
               "O2", "O3", 1.0E-6, cell, "O2 <= O3, cell " + std::to_string(cell + 1)));
            test26.addNestingExpectation(FlexibleSellaTestCase::NestingInequalityExpectation(
               "O1B", "O3", 1.0E-6, cell, "O1B <= O3, cell " + std::to_string(cell + 1)));
         }
         addTestCase(test26);
      }

      // Test 27 (2026-11-04) -- R1 and R3 against C5 (every cP lattice is an
      // hR lattice). Cells are those of Test 10 (2), Test 17 (3) and Test 21
      // (11). Without the face arrangements several violate nesting: Test 10
      // cell 1 gave R1 = 47.13 against C5 = 38.37, and Tests 17/21 gave R1/R3
      // above C5 on several cells. Guards the R1 (12) and R3 (4) face
      // arrangements, borrowed from C5 (Appendix B).
      {
         FlexibleSellaTestCase test27("R1_R3_vs_C5_Nesting",
            "R1 <= C5 and R3 <= C5 on the cells of Tests 10, 17 and 21. "
            "Fails with FACECROSSED 0 (e.g. Test 10 cell 1: R1 47.13 > C5 38.37).");
         test27.addInputCell(LatticeCell("S -43.34318 -38.15350 -0.00055 -38.13490 -43.34264 -0.02616"));
         test27.addInputCell(LatticeCell("S -38.11642 -43.36206 -0.01851 -38.17840 -43.42403 -0.04436"));
         test27.addInputCell(LatticeCell("S 0.00000 0.00000 -0.17453 -99.82547 -99.82547 -100.00000"));
         test27.addInputCell(LatticeCell("S -0.10976 -0.07316 -0.09934 -99.66756 -99.67093 -99.57723"));
         test27.addInputCell(LatticeCell("S -0.06282 -0.01222 -99.84618 -99.85396 -0.13101 -99.84497"));
         test27.addInputCell(LatticeCell("P 10 10 10 90 90 90"));
         test27.addInputCell(LatticeCell("P 9.994 9.992 9.988 89.958 89.937 89.957"));
         test27.addInputCell(LatticeCell("P 10.002 10.003 9.996 89.957 90.036 90.011"));
         test27.addInputCell(LatticeCell("P 9.996 9.992 9.993 89.994 89.950 89.946"));
         test27.addInputCell(LatticeCell("P 9.990 9.999 10.005 90.058 89.971 89.938"));
         test27.addInputCell(LatticeCell("P 9.997 9.999 9.998 90.051 89.949 89.967"));
         test27.addInputCell(LatticeCell("P 9.996 9.998 9.990 89.966 89.916 90.033"));
         test27.addInputCell(LatticeCell("P 10.002 9.993 9.996 89.980 90.028 89.940"));
         test27.addInputCell(LatticeCell("P 10.002 9.992 9.988 89.931 89.977 89.993"));
         test27.addInputCell(LatticeCell("P 9.991 9.992 9.991 89.934 89.936 89.982"));
         test27.addInputCell(LatticeCell("P 10.002 9.998 10.012 90.030 90.042 89.996"));
         for (size_t cell = 0; cell < 16; ++cell) {
            test27.addNestingExpectation(FlexibleSellaTestCase::NestingInequalityExpectation(
               "R1", "C5", 1.0E-6, cell, "R1 <= C5, cell " + std::to_string(cell + 1)));
            test27.addNestingExpectation(FlexibleSellaTestCase::NestingInequalityExpectation(
               "R3", "C5", 1.0E-6, cell, "R3 <= C5, cell " + std::to_string(cell + 1)));
         }
         addTestCase(test27);
      }

      // Test 28 (2026-11-04) -- O4 against T5 (every tP lattice is an oC
      // lattice, but only 36 of T5's 48 arrangements lie on O4's own list;
      // the other 12 are O4's face arrangements). Cells are those of Tests
      // 2-7, 11 and 19, where Bravais containment used to log "oC lowered
      // ... via tP": the Delone O4 fit was worse than T5 and only the
      // reported Bravais value was patched. With FACECROSSED on, O4 reaches
      // the tP lattices directly and those log lines disappear.
      {
         FlexibleSellaTestCase test28("O4_vs_T5_Nesting",
            "O4 <= T5 on the cells of Tests 2-7, 11 and 19 (formerly patched by "
            "oC-via-tP containment). Fails with FACECROSSED 0.");
         test28.addInputCell(LatticeCell("S -240.86238 -24.92865 -8.64775 -7.17380 -34.21200 -8.19574"));
         test28.addInputCell(LatticeCell("S -230.17565 -17.27155 -54.34361 -23.25853 -11.49257 -55.62911"));
         test28.addInputCell(LatticeCell("S -2.78162 -34.43272 -237.34522 -10.96929 -43.32243 -21.83436"));
         test28.addInputCell(LatticeCell("S -11.10601 -81.30216 -222.54748 -5.31460 -54.25901 -27.69384"));
         test28.addInputCell(LatticeCell("S -9.12428 -0.87504 -63.50943 -233.55358 -4.41117 -36.27397"));
         test28.addInputCell(LatticeCell("S -3.00480 -19.63201 -241.04017 -9.76684 -34.86767 -13.93003"));
         test28.addInputCell(LatticeCell("S -43.40350 -47.63950 0.00000 -28.62950 -43.40350 -0.00000"));
         test28.addInputCell(LatticeCell("S -47.59416 -28.66333 -0.02968 -43.33617 -43.38776 -0.05159"));
         test28.addInputCell(LatticeCell("S -47.62349 -43.35998 -0.01838 -28.66778 -43.42698 -0.04721"));
         test28.addInputCell(LatticeCell("P 21.775 21.785 27.232  90.028  89.974  90.023"));
         test28.addInputCell(LatticeCell("P  1.307  3.134  3.150 144.338  90.898 113.499"));
         test28.addInputCell(LatticeCell("P  1.447  2.154  1.448  93.001  93.077 118.555"));
         test28.addInputCell(LatticeCell("P  1.511  3.987  3.918 149.756 103.646  94.613"));
         test28.addInputCell(LatticeCell("P  1.909  3.372  3.412 140.217  95.846 108.922"));
         test28.addInputCell(LatticeCell("P  1.916  4.033  1.656 105.411 101.400  96.056"));
         test28.addInputCell(LatticeCell("P  1.918  1.118  1.899  94.267 130.788 116.312"));
         test28.addInputCell(LatticeCell("P  2.044  2.594  3.604 121.251  93.338  95.754"));
         test28.addInputCell(LatticeCell("P  2.092  3.512  1.736  92.747  95.706 123.113"));
         test28.addInputCell(LatticeCell("P  2.212  3.228  3.111 124.239  96.056 120.065"));
         test28.addInputCell(LatticeCell("P  2.418  2.421  1.105  91.232 105.455 146.970"));
         test28.addInputCell(LatticeCell("P  2.778  1.881  3.996 114.240  97.653  94.507"));
         test28.addInputCell(LatticeCell("P  2.891  2.299  3.839  90.545 118.914 100.136"));
         test28.addInputCell(LatticeCell("P  2.973  2.869  1.853  93.424 114.956 138.239"));
         test28.addInputCell(LatticeCell("P  3.286  1.671  1.295  98.790  90.427 114.783"));
         test28.addInputCell(LatticeCell("P  3.345  3.397  1.230  91.705 101.386 151.360"));
         for (size_t cell = 0; cell < 25; ++cell) {
            test28.addNestingExpectation(FlexibleSellaTestCase::NestingInequalityExpectation(
               "O4", "T5", 1.0E-6, cell, "O4 <= T5, cell " + std::to_string(cell + 1)));
         }
         addTestCase(test28);
      }

      // Test 29 (2026-11-04) -- Delone parentage regression. Runs
      // TestDeloneParentage over every type pair with fewer child free
      // parameters (the CHECKNESTING pair set) and requires exactly these
      // 120 nested pairs, taken from the CHECKNESTING run of 2026-11-04.
      // The old VCP-based test reported 135, 18 of them false (e.g. O4>O3:
      // vcp_1 of an O3 cell lies on O4's 00abbc but is a mirror image, not
      // the same lattice), and missed 3 genuine pairs (O2>O3, O5>C5,
      // O5>T5). A missing or an extra pair fails. Needs no input cells.
      {
         FlexibleSellaTestCase test29("DeloneParentage_Table",
            "TestDeloneParentage must find exactly the 120 confirmed nested "
            "parent/child pairs: no false pairs (the old VCP test had 18) and "
            "none missing (it missed O2>O3, O5>C5, O5>T5).");
         const std::set<std::string> expectedPairs = {
            "R1>C1", "R1>C5", "R3>C3", "R3>C5", "T1>C1", "T1>C3",
            "T2>C3", "T5>C5", "O1A>C1", "O1A>C3", "O1A>T1", "O1A>T2",
            "O1B>C1", "O1B>C3", "O1B>T1", "O1B>O3", "O2>C3", "O2>T2",
            "O2>O3", "O3>C3", "O4>C5", "O4>T5", "O4>H4", "O5>C5",
            "O5>T5", "M1A>C1", "M1A>C3", "M1A>C5", "M1A>R1", "M1A>T1",
            "M1A>T2", "M1A>T5", "M1A>O1A", "M1A>O2", "M1A>O3", "M1A>O4",
            "M1A>M2B", "M1A>H4", "M1B>C1", "M1B>C3", "M1B>C5", "M1B>R3",
            "M1B>T1", "M1B>T2", "M1B>T5", "M1B>O1A", "M1B>O1B", "M1B>O3",
            "M1B>M2B", "M1B>M3", "M2B>C3", "M2B>C5", "M2B>T2", "M2B>T5",
            "M2B>O3", "M2A>C3", "M2A>C5", "M2A>R3", "M2A>T2", "M2A>T5",
            "M2A>O2", "M2A>O3", "M2A>O4", "M2A>M3", "M2A>H4", "M3>C3",
            "M3>C5", "M3>R3", "M3>T5", "M3>O3", "M4>C5", "M4>T5",
            "M4>O4", "M4>O5", "M4>H4", "A1>C1", "A1>C3", "A1>C5",
            "A1>R1", "A1>R3", "A1>T1", "A1>T2", "A1>T5", "A1>O1A",
            "A1>O1B", "A1>O2", "A1>O3", "A1>O4", "A1>O5", "A1>M1A",
            "A1>M1B", "A1>M2B", "A1>M2A", "A1>M3", "A1>M4", "A1>A2",
            "A1>A3", "A1>H4", "A2>C3", "A2>C5", "A2>R3", "A2>T2",
            "A2>T5", "A2>O2", "A2>O3", "A2>O4", "A2>O5", "A2>M2B",
            "A2>M2A", "A2>M3", "A2>M4", "A2>A3", "A2>H4", "A3>C3",
            "A3>C5", "A3>R3", "A3>T5", "A3>O3", "A3>O5", "A3>M3"
         };
         test29.addParentageTableExpectation(FlexibleSellaTestCase::ParentageTableExpectation(
            expectedPairs, "120 nested pairs, exact match"));
         addTestCase(test29);
      }

      // Test 30 (2026-11-04) -- face-crossed library check. Every one of the
      // 143 face arrangements (SELLA white paper v4, Appendix B) must be a
      // projector already stored for some other type and not for its own
      // type (VerifyFaceCrossedArrangements), and the per-type counts must
      // match Appendix B. Independent of the FACECROSSED switch. Needs no
      // input cells.
      {
         FlexibleSellaTestCase test30("FaceCrossed_Library",
            "143 face arrangements for 11 types (Appendix B): each matches a "
            "stored projector of another type and none of its own type; "
            "per-type counts R1 12, R3 4, O1B 6, O2 3, O4 12, M1A 30, M1B 18, "
            "M2A 18, M2B 24, M3 12, A3 4.");
         const std::map<std::string, size_t> expectedCounts = {
            { "R1", 12 }, { "R3", 4 }, { "O1B", 6 }, { "O2", 3 }, { "O4", 12 },
            { "M1A", 30 }, { "M1B", 18 }, { "M2A", 18 }, { "M2B", 24 }, { "M3", 12 },
            { "A3", 4 }
         };
         test30.addFaceCrossedLibraryExpectation(FlexibleSellaTestCase::FaceCrossedLibraryExpectation(
            expectedCounts, "Appendix B counts, all verified"));
         addTestCase(test30);
      }

      // Test 31 (2026-11-08) -- conventional cells, library check. For every
      // library and face arrangement of every Delone type (249 + 143 = 392),
      // generic members must reach the type's canonical arrangement
      // (ToCanonicalArrangement), and the conventional cell from the
      // centering table must have the type's symmetry and centering: mC
      // C-centered with b unique and beta >= 90, mP b unique, hR in the
      // rhombohedral setting. Needs no input cells.
      {
         FlexibleSellaTestCase test31("ConventionalCell_Library",
            "Every library and face arrangement (392) reaches its type's "
            "canonical arrangement, and the conventional cell has the type's "
            "symmetry and centering.");
         test31.addConventionalCellLibraryExpectation(
            FlexibleSellaTestCase::ConventionalCellLibraryExpectation(
               392, 3, "392 arrangements, 3 samples each, 0 failures"));
         addTestCase(test31);
      }

      // Test 32 (2026-11-08) -- conventional cells for one input, end to
      // end. The input is the primitive reduced cell of
      // I 10 10.01 10.02 90.1 90.2 90.3 (computed to 8 decimals). Its C1
      // (cI) fit is 0.503905. Expected:
      //   ideal: I 10.0100 10.0100 10.0100 90 90 90
      //          (a = sqrt(-4 x mean scalar) = 10.0100);
      //   input: I-centered, edges 10.00, 10.01, 10.02 and angles 90.1,
      //          90.2, 90.3 in some order -- the cell that was typed in.
      {
         FlexibleSellaTestCase test32("ConventionalCell_C1_EndToEnd",
            "Primitive cell of I 10 10.01 10.02 90.1 90.2 90.3: the C1 fit "
            "gives ideal I 10.0100 x3, 90 x3, and the input in the same "
            "setting is the typed cell.");
         test32.addInputCell(LatticeCell("S -25.23765339 -25.22495664 -25.21198499 -25.06259661 -24.87519336 -24.68786501"));
         test32.addConventionalCellExpectation(FlexibleSellaTestCase::ConventionalCellExpectation(
            "C1", 0, 'I',
            { 10.0100, 10.0100, 10.0100, 90.0, 90.0, 90.0 },
            { 10.000, 10.010, 10.020 },
            { 0.1, 0.2, 0.3 },
            2.0E-4, 1.0E-3,
            "C1 conventional cells, ideal and input"));
         addTestCase(test32);
      }

      // ----------------------------------------------------------------
      // Test 33: mC conventional cells are reduced in the a-c plane.
      // Before ReduceMonoclinicPlane (2026-11-08) the centering table gave
      // valid but unreduced C cells: cell 1 M2A had beta 130.70
      // (C 16.6133 9.2475 8.6952) and cell 2 M1B had beta 155.67
      // (C 22.2548 12.8627 16.2639). Expected (C-centered, b unique, beta > 90):
      //   cell 1 M2A ideal C 14.1998 9.2475 8.6952 90 117.5018 90
      //              input C 14.2028 9.2520 8.7001 89.9401 117.5230 89.9076
      //   cell 2 M1B ideal C 15.3020 12.8627 10.0100 90 103.1776 90
      //              input C 15.3020 12.8627 10.0100 89.9313 103.1776 89.9418
      // Cell 1 is a perturbed M3 from nb.txt; cell 2 is near-rhombohedral
      // (also used in Test 34).
      {
         FlexibleSellaTestCase test33("ConventionalCell_mC_Reduced",
            "mC conventional cells are C-centered, b unique, beta > 90, and reduced "
            "in the a-c plane (formerly beta 130.7 and 155.7 for these fits).");
         test33.addInputCell(LatticeCell("P 8.469 9.519 9.499 121.780 122.422 89.689"));
         test33.addInputCell(LatticeCell("P 10.01 10.00 9.99 80.1 79.9 80.0"));
         test33.addConventionalCellExpectation(FlexibleSellaTestCase::ConventionalCellExpectation(
            "M2A", 0, 'C',
            { 14.1998, 9.2475, 8.6952, 90.0, 117.5018, 90.0 },
            { 8.7001, 9.2520, 14.2028 },
            { 0.0599, 0.0924, 27.5230 },
            2.0E-4, 1.0E-3,
            "cell 1 M2A conventional cells"));
         test33.addConventionalCellExpectation(FlexibleSellaTestCase::ConventionalCellExpectation(
            "M1B", 1, 'C',
            { 15.3020, 12.8627, 10.0100, 90.0, 103.1776, 90.0 },
            { 10.0100, 12.8627, 15.3020 },
            { 0.0582, 0.0687, 13.1776 },
            2.0E-4, 1.0E-3,
            "cell 2 M1B conventional cells"));
         addTestCase(test33);
      }

      // ----------------------------------------------------------------
      // Test 34: hR conventional cell, rhombohedral setting (printed as P,
      // since R in LRL input means hexagonal axes). Expected for R3:
      //   ideal P 10.0017 10.0017 10.0017 79.9934 79.9934 79.9934
      //   input P 10.0100 10.0000 9.9900 80.1 79.9 80.0 -- the typed cell.
      {
         FlexibleSellaTestCase test34("ConventionalCell_hR_Rhombohedral",
            "R3 fit of P 10.01 10.00 9.99 80.1 79.9 80.0: ideal rhombohedral "
            "cell 10.0017 x3, 79.9934 x3, and the input in the same setting "
            "is the typed cell.");
         test34.addInputCell(LatticeCell("P 10.01 10.00 9.99 80.1 79.9 80.0"));
         test34.addConventionalCellExpectation(FlexibleSellaTestCase::ConventionalCellExpectation(
            "R3", 0, 'P',
            { 10.0017, 10.0017, 10.0017, 79.9934, 79.9934, 79.9934 },
            { 9.9900, 10.0000, 10.0100 },
            { 9.9, 10.0, 10.1 },
            2.0E-4, 1.0E-3,
            "R3 conventional cells, ideal and input"));
         addTestCase(test34);
      }

      // ----------------------------------------------------------------
      // Test 35: tP, oP, oC and mP conventional cells of the near-tP cell
      // T19c01 (also the input of Test 28, cell 10). Expected:
      //   T5 ideal P 21.7736 21.7736 27.2252 90 90 90
      //   O5 ideal P 21.7688 21.7783 27.2252 90 90 90
      //   O4 ideal C 30.7925 30.7925 27.2252 90 90 90
      //   M4 ideal P 21.7806 21.7688 27.2271 90 90.0096 90 (b unique)
      // Inputs: P edges 21.7750 21.7850 27.2320, angles 90.0280 90.0260
      // 89.9770; C edges 27.2320 30.7954 30.8078 (the tP diagonals).
      // The O4 ideal has a = b, i.e. the nearest O4 point here is on its
      // tP boundary (fit(O4) = fit(T5), as in Test 28); if that changes,
      // this test should fail and be looked at.
      {
         FlexibleSellaTestCase test35("ConventionalCell_tP_oP_oC_mP",
            "Near-tP cell P 21.775 21.785 27.232 90.028 89.974 90.023: tP, oP, "
            "oC and mP conventional cells, ideal and input.");
         test35.addInputCell(LatticeCell("P 21.775 21.785 27.232  90.028  89.974  90.023"));
         test35.addConventionalCellExpectation(FlexibleSellaTestCase::ConventionalCellExpectation(
            "T5", 0, 'P',
            { 21.7736, 21.7736, 27.2252, 90.0, 90.0, 90.0 },
            { 21.7750, 21.7850, 27.2320 },
            { 0.0230, 0.0260, 0.0280 },
            2.0E-4, 1.0E-3,
            "T5 conventional cells"));
         test35.addConventionalCellExpectation(FlexibleSellaTestCase::ConventionalCellExpectation(
            "O5", 0, 'P',
            { 21.7688, 21.7783, 27.2252, 90.0, 90.0, 90.0 },
            { 21.7750, 21.7850, 27.2320 },
            { 0.0230, 0.0260, 0.0280 },
            2.0E-4, 1.0E-3,
            "O5 conventional cells"));
         test35.addConventionalCellExpectation(FlexibleSellaTestCase::ConventionalCellExpectation(
            "O4", 0, 'C',
            { 30.7925, 30.7925, 27.2252, 90.0, 90.0, 90.0 },
            { 27.2320, 30.7954, 30.8078 },
            { 0.0014, 0.0263, 0.0382 },
            2.0E-4, 1.0E-3,
            "O4 conventional cells"));
         test35.addConventionalCellExpectation(FlexibleSellaTestCase::ConventionalCellExpectation(
            "M4", 0, 'P',
            { 21.7806, 21.7688, 27.2271, 90.0, 90.0096, 90.0 },
            { 21.7750, 21.7850, 27.2320 },
            { 0.0230, 0.0260, 0.0280 },
            2.0E-4, 1.0E-3,
            "M4 conventional cells"));
         addTestCase(test35);
      }

      // ----------------------------------------------------------------
      // Test 36: R1 reaches its cubic special case through the face
      // arrangements. FaceCrossedArrangements.h gives R1 12 arrangements
      // borrowed from C5 (cP is hR with r = 0). Cell 1 (a 1% perturbed
      // M3 cell): R1's own 4 arrangements give about 47.7; a borrowed C5
      // arrangement gives 28.7331, so fit(R1) = fit(C5) = 28.7331 and no
      // containment line is printed. Cell 2 (an exact M3 cell from cmdgen,
      // G6 158.574 65.368 65.368 -6.970 -123.765 0.000 as S6): R1 finds a
      // non-cubic point, 23.4319 < fit(C5) = 23.5181; R3 (4 borrowed C5
      // arrangements) equals C5. Expected to fail with FACECROSSED 0.
      {
         FlexibleSellaTestCase test36("R1_R3_ReachCubic_ViaFaceArrangements",
            "fit(R1) = fit(C5) = 28.7331 on a perturbed M3 cell through R1's "
            "borrowed C5 face arrangements (own arrangements give about 47.7); "
            "on an exact M3 cell R1 = 23.4319 < C5 = 23.5181 = R3. Fails with "
            "FACECROSSED 0.");
         test36.addInputCell(LatticeCell("P 8.469 9.519 9.499 121.780 122.422 89.689"));
         test36.addInputCell(LatticeCell("S -3.485 -61.8825 0.000 -96.6915 -61.883 -0.0005"));
         test36.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R1", 28.7330, 28.7332, 0, "cell 1 R1 = 28.7331 (cubic, via face arrangement)"));
         test36.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "C5", 28.7330, 28.7332, 0, "cell 1 C5 = 28.7331"));
         test36.addNestingExpectation(FlexibleSellaTestCase::NestingInequalityExpectation(
            "R1", "C5", 1.0E-6, 0, "R1 <= C5, cell 1"));
         test36.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R1", 23.4318, 23.4320, 1, "cell 2 R1 = 23.4319 (non-cubic)"));
         test36.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "C5", 23.5180, 23.5182, 1, "cell 2 C5 = 23.5181"));
         test36.addFitRangeExpectation(FlexibleSellaTestCase::FitRangeExpectation(
            "R3", 23.5180, 23.5182, 1, "cell 2 R3 = 23.5181 (cubic, via face arrangement)"));
         test36.addNestingExpectation(FlexibleSellaTestCase::NestingInequalityExpectation(
            "R1", "C5", 1.0E-6, 1, "R1 <= C5, cell 2"));
         test36.addNestingExpectation(FlexibleSellaTestCase::NestingInequalityExpectation(
            "R3", "C5", 1.0E-6, 1, "R3 <= C5, cell 2"));
         addTestCase(test36);
      }

      // ----------------------------------------------------------------
      // Test 37: I-centered alternative for a strongly oblique mC cell. A 1%
      // orthogonal perturbation of an exact M1A cell (a library sample from
      // Test 31, S -53.70492 -53.70492 -71.29379 -44.82996 -44.82996
      // -3.31836). The reduced C-centered beta, 131.44, exceeds
      // MONOCLINIC_I_BETA_THRESHOLD (120), so the I-centered cell is also
      // given. Recorded 2026-11-09 (M1A fit 0.573858):
      //   idealized C 13.9965 22.0043 9.5784 90 131.4425 90
      //   input     C 13.9965 22.0043 9.5784 89.9529 131.4425 90.1797
      //   idealized I 9.5784 22.0043 10.4968 90 91.7171 90
      //   input     I 9.5784 22.0043 10.4968 90.1967 91.7171 90.0471
      // I cell a = C cell c; I cell c = |C cell a + c|; same b and volume.
      // The fit changes only alpha and gamma: edges and beta of idealized and
      // input agree.
      {
         FlexibleSellaTestCase test37("ConventionalCell_mC_ICentered_Alternative",
            "Perturbed M1A cell with reduced C-centered beta 131.44: the "
            "I-centered cell I 9.5784 22.0043 10.4968, beta 91.72, is also "
            "given; input cells differ from idealized only in alpha and gamma.");
         test37.addInputCell(LatticeCell("S -53.18832 -53.98118 -72.07240 -44.45272 -44.27952 -3.01266"));
         FlexibleSellaTestCase::ConventionalCellExpectation e37(
            "M1A", 0, 'C',
            { 13.9965, 22.0043, 9.5784, 90.0, 131.4425, 90.0 },
            { 9.5784, 13.9965, 22.0043 },
            { 0.0471, 0.1797, 41.4425 },
            2.0E-4, 1.0E-3,
            "M1A C-centered and I-centered cells");
         e37.idealCellI = { 9.5784, 22.0043, 10.4968, 90.0, 91.7171, 90.0 };
         test37.addConventionalCellExpectation(e37);
         addTestCase(test37);
      }

      // ----------------------------------------------------------------
      // Test 38: basis matrices from the typed cell (2026-11-09). One lattice
      // typed five ways: Test 32's I 10 10.01 10.02 90.1 90.2 90.3, its
      // primitive cell, the M1A C cell, the O1A F cell, and R on hexagonal
      // axes (obverse) from the R1 cell. Rows of each typed cell in terms of
      // the I axes:
      //   P  -1/2 1/2 1/2   1/2 -1/2 1/2   1/2 1/2 -1/2
      //   C  -1 0 0   0 -1 1   1/2 1/2 1/2
      //   F  0 0 1   -1 1 0   -1 -1 0
      //   R  -1 1 0   1 0 -1   -1/2 -1/2 -1/2
      // All five gave the same fits to about 1e-5 (2026-11-09; C1 0.5039,
      // M1A 0.1328, R1 0.2656). The R cell is near-hexagonal, not exact:
      // before LatSymMatG6 used the G6 shape comparison it was read as
      // primitive (H4 0.37, C1 110.5), which the fit check here catches.
      // No basis matrix is frozen (see TypedBasisExpectation).
      {
         FlexibleSellaTestCase test38("TypedBasis_FiveSettings",
            "One lattice typed as I, P, C, F and R (hexagonal axes): basis "
            "matrices from the typed cell are lattice bases with the right "
            "determinant, and all five settings give the same fits and ideal "
            "conventional cells.");
         test38.addInputCell(LatticeCell("I 10 10.01 10.02 90.1 90.2 90.3"));
         test38.addInputCell(LatticeCell("P 8.689047 8.678988 8.668937 109.599257 109.565391 109.531354"));
         test38.addInputCell(LatticeCell("C 10.000000 14.175705 8.638614 89.912353 125.011339 90.070472"));
         test38.addInputCell(LatticeCell("F 10.020000 14.186202 14.112117 90.057268 89.787346 89.929580"));
         test38.addInputCell(LatticeCell("R 14.186202 14.180971 8.638614 89.824736 90.087617 120.036763"));
         test38.addTypedBasisExpectation(FlexibleSellaTestCase::TypedBasisExpectation(
            { "C1", "M1A", "M1B", "O1A", "O1B", "R1", "T1" },
            1.0E-9, 5.0E-5, 2.0E-4, 1.0E-3,
            "basis matrices, five typed settings of one lattice"));
         addTestCase(test38);
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
