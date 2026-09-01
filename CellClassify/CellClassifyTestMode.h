#ifndef CELLCLASSIFYTESTMODE_H
#define CELLCLASSIFYTESTMODE_H

// CellClassifyTestMode.h
//
// CellClassify's own regression suite, parallel to CmdSella's
// FlexibleSellaTestMode.h but deliberately much lighter-weight: this
// tool's output is two StoreResults tallies (failure signatures, near-tie
// pairs), not a 24-type fit table, so there's no need for
// FlexibleSellaTestCase's HasFailure/RankStability/ReflectionRelation
// expectation machinery here -- just counts and store contents.
//
// Test 1 (the only test so far) is the 16-cell historical mP/oC/tP set,
// the same 16 cells as CmdSella's FlexibleSellaTestMode.h Test 19, run
// through CellClassify's own HandleOneInputCell instead of directly
// through ComputeDeloneFits. All 16 are known (from CmdSella's Test 19,
// confirmed this session) to resolve with HasFailure()==false, so
// totalProcessed is 16, totalFailed is 0, and the failure-signature store
// is empty. The near-tie table is pinned to exact counts confirmed this
// session (TieCountExpectation) -- safe to pin because this cell set is
// fully deterministic; a CmdGen-generated population is NOT reproducible
// run-to-run unless CmdGen runs off a fixed seed, so its near-tie counts
// should stay unpinned (nearTieTableIsManualReview) unless that changes.

#include "CellClassifyControls.h"
#include "DeloneFitComputation.h"
#include "LatticeCell.h"
#include "Sella.h"
#include "StoreResults.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// Forward declaration -- HandleOneInputCell is defined in CellClassify.cpp
// (not moved to a header) since it's currently only called from two
// places (main()'s batch loop and this test suite) both living in the
// same translation unit as of this writing. If a third caller shows up,
// this should move into its own header the same way DeloneFitFiltering.h
// got extracted from CmdSella.cpp once CellClassify needed it too.
void HandleOneInputCell(
   const LatticeCell& inputCell,
   const CellClassifyControls& controls,
   const CmdSellaControls& cmdSellaControls,
   Sella& sella,
   StoreResults<std::string, std::string>& failureStore,
   StoreResults<std::string, std::string>& tieStore,
   size_t& totalProcessed,
   size_t& totalFailed);

struct TieCountExpectation {
   std::string pairKey;   // e.g. "M1A/M1B" -- MakeTiePairKey's output form
   long expectedCount;
   TieCountExpectation(const std::string& key, const long count)
      : pairKey(key), expectedCount(count) {
   }
};

struct CellClassifyTestCase {
   std::string name;
   std::string description;
   std::vector<std::string> inputCells;

   // -1 means "don't check this" -- matches FlexibleSellaTestMode.h's
   // convention of leaving genuinely unknown things unchecked rather than
   // asserting a guessed value.
   long expectedTotalProcessed = -1;
   long expectedTotalFailed = -1;
   long expectedFailureStoreSize = -1;

   // Only safe to populate for DETERMINISTIC input cell sets (fixed
   // literal cells, no randomness anywhere in ComputeDeloneFits) -- a
   // CmdGen-generated population is NOT reproducible run-to-run unless
   // CmdGen is running off a fixed seed, so pinning ITS near-tie counts
   // would just make the test fail on its own next execution for no real
   // reason. See this session's chat for the distinction.
   std::vector<TieCountExpectation> tieExpectations;

   // If true, the near-tie table is printed but not compared against any
   // expectation -- this run's job is to establish the baseline, not
   // confirm one. Automatically false once tieExpectations is non-empty.
   bool nearTieTableIsManualReview = true;

   CellClassifyTestCase(const std::string& n, const std::string& desc)
      : name(n), description(desc) {
   }

   void addInputCell(const std::string& cellString) {
      inputCells.push_back(cellString);
   }

   void addTieExpectation(const std::string& pairKey, const long count) {
      tieExpectations.emplace_back(pairKey, count);
      nearTieTableIsManualReview = false;
   }
};

class CellClassifyTestRunner {
public:
   CellClassifyTestRunner() {
      setupStandardTests();
   }

   void RunAll() {
      RunSelected(std::vector<int>());
   }

   // Empty testNumbers means "run everything" (matches CmdSellaControls's
   // TEST convention: TEST alone runs the whole suite).
   void RunSelected(const std::vector<int>& testNumbers) {
      if (!testNumbers.empty() &&
         testNumbers.size() == 1 && testNumbers[0] == 999) {
         ListTests();
         return;
      }

      std::cout << "\n========================================" << std::endl;
      std::cout << "RUNNING CELLCLASSIFY TEST SUITE" << std::endl;
      std::cout << "========================================" << std::endl;

      size_t passed = 0, failed = 0;
      for (size_t i = 0; i < m_tests.size(); ++i) {
         const int testNum = static_cast<int>(i) + 1;
         if (!testNumbers.empty() &&
            std::find(testNumbers.begin(), testNumbers.end(), testNum) == testNumbers.end()) {
            continue;
         }
         const bool ok = RunOne(testNum, m_tests[i]);
         if (ok) ++passed; else ++failed;
      }

      std::cout << "\n========================================" << std::endl;
      std::cout << "TEST SUMMARY" << std::endl;
      std::cout << "========================================" << std::endl;
      std::cout << "Passed: " << passed << std::endl;
      std::cout << "Failed: " << failed << std::endl;
      std::cout << "Total:  " << (passed + failed) << std::endl;
      if (failed > 0) std::cout << "SOME TESTS FAILED!" << std::endl;
   }

private:
   std::vector<CellClassifyTestCase> m_tests;

   void addTestCase(const CellClassifyTestCase& t) { m_tests.push_back(t); }

   void ListTests() const {
      std::cout << "; Available CellClassify tests:" << std::endl;
      for (size_t i = 0; i < m_tests.size(); ++i) {
         std::cout << ";  " << (i + 1) << ". " << m_tests[i].name << std::endl;
      }
   }

   bool RunOne(const int testNum, const CellClassifyTestCase& t) {
      std::cout << "\n--- Test " << testNum << " of " << m_tests.size() << " ---" << std::endl;
      std::cout << "=== TEST: " << t.name << " ===" << std::endl;
      std::cout << "Description: " << t.description << std::endl;

      CmdSellaControls cmdSellaControls;
      Sella sella;
      StoreResults<std::string, std::string> failureStore(3);
      StoreResults<std::string, std::string> tieStore(3);
      size_t totalProcessed = 0;
      size_t totalFailed = 0;

      for (const auto& cellStr : t.inputCells) {
         const LatticeCell cell(cellStr);
         HandleOneInputCell(cell, CellClassifyControls(), cmdSellaControls, sella,
            failureStore, tieStore, totalProcessed, totalFailed);
      }

      bool allPass = true;

      if (t.expectedTotalProcessed >= 0) {
         const bool ok = (static_cast<long>(totalProcessed) == t.expectedTotalProcessed);
         allPass = allPass && ok;
         std::cout << "  totalProcessed: " << (ok ? "PASS" : "FAIL")
            << " -- got " << totalProcessed
            << " (expected " << t.expectedTotalProcessed << ")" << std::endl;
      }

      if (t.expectedTotalFailed >= 0) {
         const bool ok = (static_cast<long>(totalFailed) == t.expectedTotalFailed);
         allPass = allPass && ok;
         std::cout << "  totalFailed: " << (ok ? "PASS" : "FAIL")
            << " -- got " << totalFailed
            << " (expected " << t.expectedTotalFailed << ")" << std::endl;
      }

      if (t.expectedFailureStoreSize >= 0) {
         const bool ok = (static_cast<long>(failureStore.size()) == t.expectedFailureStoreSize);
         allPass = allPass && ok;
         std::cout << "  failureStore distinct keys: " << (ok ? "PASS" : "FAIL")
            << " -- got " << failureStore.size()
            << " (expected " << t.expectedFailureStoreSize << ")" << std::endl;
         if (failureStore.size() > 0) {
            failureStore.ShowTableOfKeysVersusCount();
         }
      }

      if (!t.tieExpectations.empty()) {
         std::cout << "  Checking tie-count expectations:" << std::endl;
         for (const auto& te : t.tieExpectations) {
            const long got = static_cast<long>(tieStore.GetItemCount(te.pairKey));
            const bool ok = (got == te.expectedCount);
            allPass = allPass && ok;
            std::cout << "    " << te.pairKey << ": " << (ok ? "PASS" : "FAIL")
               << " -- got " << got << " (expected " << te.expectedCount << ")" << std::endl;
         }
      }
      else if (t.nearTieTableIsManualReview) {
         std::cout << "; Near-tie table (MANUAL REVIEW -- not yet asserted on):" << std::endl;
         tieStore.ShowTableOfKeysVersusCount();
      }

      std::cout << "OVERALL: " << (allPass ? "PASS" : "FAIL") << std::endl;
      return allPass;
   }

   void setupStandardTests() {
      // Test 1 -- the 16-cell historical mP/oC/tP set, same cells as
      // CmdSella's FlexibleSellaTestMode.h Test 19. See file header
      // comment for what is and isn't asserted on here.
      {
         CellClassifyTestCase test1("Historical16Cells_AllResolveNoFailures",
            "The same 16 historical mP/oC/tP cells as CmdSella's "
            "FlexibleSellaTestMode.h Test 19, confirmed there (this "
            "session) to all resolve with HasFailure()==false. Checks "
            "that CellClassify's own HandleOneInputCell agrees: "
            "totalProcessed==16, totalFailed==0, failure-signature store "
            "empty. Near-tie table pinned to the exact counts confirmed "
            "this session -- safe to pin because this cell set is fully "
            "deterministic (fixed literal cells, no randomness anywhere "
            "in ComputeDeloneFits), unlike a CmdGen-generated population.");
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
            test1.addInputCell(historicalCells[i]);
         }
         test1.expectedTotalProcessed = 16;
         test1.expectedTotalFailed = 0;
         test1.expectedFailureStoreSize = 0;
         // Confirmed this session (real run, deterministic cell set):
         //   item   count   Key
         //   0      2       M1B/M4
         //   1      1       M4/O5
         //   2      1       M3/M4
         //   3      1       M2A/M4
         //   4      1       M1A/M4
         //   5      1       M1A/M2A
         //   6      1       M1A/M1B
         test1.addTieExpectation("M1B/M4", 2);
         test1.addTieExpectation("M4/O5", 1);
         test1.addTieExpectation("M3/M4", 1);
         test1.addTieExpectation("M2A/M4", 1);
         test1.addTieExpectation("M1A/M4", 1);
         test1.addTieExpectation("M1A/M2A", 1);
         test1.addTieExpectation("M1A/M1B", 1);
         addTestCase(test1);
      }

      // Test 2 -- 4 real, genuinely unresolved Grimmer chain violations,
      // surfaced this session by running `cmdgen 20 | cellclassify` (a
      // random but reproducible population, since CmdGen runs off a
      // fixed seed -- see this session's chat) and investigating the
      // aP/mC/oC and aP/mP/oC failure signatures that turned up.
      //
      // Every value CellClassify reported for these 4 cells (via
      // FilterForBestExample) was cross-checked directly against
      // CmdSella's own `details` mode -- which reads gcs.GetFirstFailure()
      // at the actual decision point, no separate recomputation -- and
      // matched exactly (aP=0, mC or mP value, oC value, all four cells).
      // So unlike a moment where a mismatch was suspected, this is
      // confirmed: CellClassify is reporting the true internal state.
      //
      // The violations themselves are genuine and NOT expected to
      // resolve -- Remediation168 tries up to 4 passes on each and
      // cannot find a better fit (cell 1's log lands on the same
      // 1.21019 twice before stopping; cells 3/4 fail to improve even
      // on the first attempt). Same category as CmdSella's own
      // KnownUnresolvedViolation_1..6 tripwire tests -- this asserts
      // the FAILING state on purpose, the same way those do, so a
      // future change that starts silently resolving OR silently
      // missing this class of violation gets caught either way.
      {
         CellClassifyTestCase test2("RealUnresolvedFailures_ApMCoC_ApMPoC",
            "4 cells found via cmdgen 20 | cellclassify (fixed-seed "
            "reproducible population) that trip genuinely unresolved "
            "Grimmer chain violations (mC or mP fitting worse than "
            "their oC descendant, which Remediation168 cannot fix). "
            "Every reported fit value was cross-checked against "
            "CmdSella's own `details` mode and matched exactly. "
            "Tripwire, same pattern as CmdSella's "
            "KnownUnresolvedViolation_1..6: asserts the FAILING state "
            "on purpose, so silent resolution or silent miss of this "
            "class of violation both get caught.");
         test2.addInputCell("G6 55.087 33.662 54.141 -48.314 -58.049 0.000 Delone A2 aP");
         test2.addInputCell("G6 74.240 165.052 84.224 -134.088 -34.359 0.000 Delone A3 aP");
         test2.addInputCell("G6 36.972 106.683 106.683 18.486 36.972 36.972 IT 18 tI");
         test2.addInputCell("G6 139.722 187.104 187.104 69.861 139.722 139.722 IT 18 tI");
         test2.expectedTotalProcessed = 4;
         test2.expectedTotalFailed = 4;      // all 4 fail -- this IS the tripwire
         test2.expectedFailureStoreSize = 2; // two distinct signatures: aP/mC/oC, aP/mP/oC
         // Near-tie table, confirmed this session's DETAILS run:
         //   item   count   type pair
         //   0      2       M1A/M1B   (both tI cells)
         //   1      1       M4/O4     (the A2 aP cell only -- the A3 aP
         //                             cell's best/second gap (3.76e-6
         //                             vs 0.0293841) exceeds TIETOL)
         test2.addTieExpectation("M1A/M1B", 2);
         test2.addTieExpectation("M4/O4", 1);
         addTestCase(test2);
      }
   }
};

#endif // CELLCLASSIFYTESTMODE_H

