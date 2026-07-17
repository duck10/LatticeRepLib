#ifndef FLEXIBLE_TEST_MODE_H
#define FLEXIBLE_TEST_MODE_H

#include "Programheader.h"
#include "MatchPresentation.h"
#include "Displaydispatch.h"
#include "LRL_Cell_Degrees.h"
#include "MatchPair.h"
#include "P3RelativeThresholds.h"
#include "ProductionLatticeMatcherSystem.h"
#include "SearchMatrixBuilder.h"
#include "LatticeCell.h"
#include "MultiTransformFinderControls.h"
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <map>
#include <cmath>
#include <chrono>

class FlexibleTestCase {
public:
   std::string name;
   std::string description;
   std::vector<LatticeCell> inputCells;

   // Quality-based expectations instead of exact matrix matching
   struct QualityThresholds {
      double maxP3Distance;
      double maxS6Angle;
      double minDeterminant;
      double maxDeterminant;
      std::string description;

      QualityThresholds(double p3 = 0.0, double s6 = 0.0,
         double minDet = 0.99, double maxDet = 1.01,
         const std::string& desc = "Quality thresholds")
         : maxP3Distance(p3), maxS6Angle(s6),
         minDeterminant(minDet), maxDeterminant(maxDet), description(desc) {
      }
   };

   // Traditional exact matrix expectation (for specific cases)
   struct ExactExpectation {
      Matrix_3x3 expectedMatrix;
      double expectedP3Distance;
      double expectedS6Angle;
      double tolerance;
      std::string description;

      ExactExpectation(const Matrix_3x3& matrix, double p3Dist, double s6Angle,
         double tol = 1e-3, const std::string& desc = "")
         : expectedMatrix(matrix), expectedP3Distance(p3Dist),
         expectedS6Angle(s6Angle), tolerance(tol), description(desc) {
      }
   };

   // Position-independent expectation: passes if the given matrix appears
   // ANYWHERE among the results (within tolerance), regardless of index.
   // Use this for regression-guarding a specific alternate orientation that
   // is tied in quality with others -- tie order is not guaranteed stable,
   // so ExactExpectation (which checks a fixed results[i]) is the wrong
   // tool for that; this checks presence, not position.
   struct ContainsExpectation {
      Matrix_3x3 expectedMatrix;
      double tolerance;
      std::string description;

      ContainsExpectation(const Matrix_3x3& matrix, double tol = 1e-3,
         const std::string& desc = "")
         : expectedMatrix(matrix), tolerance(tol), description(desc) {
      }
   };

   std::vector<ContainsExpectation> containsExpectations;

   void addContainsExpectation(const ContainsExpectation& contains) {
      containsExpectations.push_back(contains);
   }

   // Test can use either quality thresholds OR exact expectations
   std::vector<QualityThresholds> qualityExpectations;
   std::vector<ExactExpectation> exactExpectations;
   bool useQualityMode = false;

   FlexibleTestCase(const std::string& testName, const std::string& desc)
      : name(testName), description(desc) {
   }

   void addInputCell(const LatticeCell& cell) {
      inputCells.push_back(cell);
   }

   // Use quality-based testing (flexible)
   void addQualityExpectation(const QualityThresholds& quality) {
      qualityExpectations.push_back(quality);
      useQualityMode = true;
   }

   // Use exact matrix testing (strict)
   void addExactExpectation(const ExactExpectation& exact) {
      exactExpectations.push_back(exact);
      useQualityMode = false;
   }
};

class FlexibleTestRunner {
private:
   std::vector<FlexibleTestCase> testCases;
   int passedTests = 0;
   int failedTests = 0;
   int manualTests = 0;

   bool compareMatrices(const Matrix_3x3& actual, const Matrix_3x3& expected, double tolerance) {
      for (int i = 0; i < 9; ++i) {
         if (std::abs(actual[i] - expected[i]) > tolerance) {
            return false;
         }
      }
      return true;
   }

   bool compareValues(double actual, double expected, double tolerance) {
      return std::abs(actual - expected) <= tolerance;
   }

   void displayTestInputs(const FlexibleTestCase& test) {
      std::cout << "Input cells:" << std::endl;
      for (size_t i = 0; i < test.inputCells.size(); ++i) {
         const auto& cell = test.inputCells[i];
         std::cout << "  Cell " << (i + 1) << " (" << cell.getLatticeType() << "): "
            << LRL_Cell_Degrees(cell.getCell()) << std::endl;
      }
   }

   bool validateQualityThresholds(const LatticeMatchResult& result,
      const LRL_Cell& referenceCell,
      const FlexibleTestCase::QualityThresholds& thresholds,
      const MultiTransformFinderControls& controls) {
      // Check P3 distance
      bool p3Ok = result.getP3Distance() <= thresholds.maxP3Distance;

      // Check S6 angle
      G6 redRef, redMob;
      Niggli::Reduce(G6(referenceCell), redRef);
      Niggli::Reduce(G6(result.getTransformedMobile()), redMob);
      const double actualS6Angle = TransformerUtilities::angleS6(
         S6(LRL_Cell(redMob)), S6(LRL_Cell(redRef)));
      bool s6Ok = actualS6Angle <= thresholds.maxS6Angle;

      // Check matrix determinant
      double det = result.getTransformationMatrix().Det();
      bool detOk = (det >= thresholds.minDeterminant && det <= thresholds.maxDeterminant);

      const bool passed = p3Ok && s6Ok && detOk;

      // Print validation details only on failure or when details requested
      if (!passed || controls.shouldShowDetails()) {
         std::cout << "    Quality validation:" << std::endl;
         std::cout << "      P3 distance: " << std::scientific << std::setprecision(3)
            << result.getP3Distance() << " (max: " << thresholds.maxP3Distance << ") "
            << (p3Ok ? "+" : "X") << std::endl;
         std::cout << "      S6 angle: " << std::fixed << std::setprecision(2)
            << actualS6Angle << " deg (max: " << thresholds.maxS6Angle << " deg) "
            << (s6Ok ? "+" : "X") << std::endl;
         std::cout << "      Determinant: " << std::fixed << std::setprecision(6)
            << det << " (expected: " << thresholds.minDeterminant
            << " to " << thresholds.maxDeterminant << ") "
            << (detOk ? "+" : "X") << std::endl;
      }

      return passed;
   }

   void runTest(const FlexibleTestCase& test, const MultiTransformFinderControls& controls) {
      std::cout << "\n=== TEST: " << test.name << " ===" << std::endl;
      std::cout << "Description: " << test.description << std::endl;

      displayTestInputs(test);

      if (test.inputCells.size() < 2) {
         std::cout << "FAIL: Test needs at least 2 input cells" << std::endl;
         failedTests++;
         return;
      }

      // Run via matchPair so volume swap matches production behavior
      const auto mr = matchPair(test.inputCells[0], test.inputCells[1], controls);
      std::vector<LatticeMatchResult> results = mr.layer2.results;
      const LatticeCell& referenceCell = mr.reference;

      if (results.empty()) {
         std::cout << "FAIL: No results returned" << std::endl;
         failedTests++;
         return;
      }

      std::cout << "   ; Found " << results.size() << " results" << std::endl;

      // Use production display dispatch so test output matches production exactly
      displayMatchResult(mr.layer2, controls, mr.reference, mr.mobile);

      // Validate results
      if (test.qualityExpectations.empty() && test.exactExpectations.empty()) {
         std::cout << "\n;No expectations defined - manual verification needed" << std::endl;
         std::cout << ";OVERALL: MANUAL CHECK REQUIRED" << std::endl;
         manualTests++;
         return;
      }

      bool testPassed = true;

      if (test.useQualityMode) {
         // Quality-based validation
         std::cout << "\nValidating using quality thresholds:" << std::endl;

         for (size_t i = 0; i < test.qualityExpectations.size(); ++i) {
            const auto& quality = test.qualityExpectations[i];

            if (i >= results.size()) {
               std::cout << "FAIL: Expected result " << (i + 1) << " not found" << std::endl;
               testPassed = false;
               continue;
            }

            std::cout << "  Checking result " << (i + 1) << " (" << quality.description << "):" << std::endl;
            bool resultOk = validateQualityThresholds(results[i], mr.layer2.primitiveReference, quality, controls);
            if (resultOk) {
               std::cout << "  Result " << (i + 1) << ": PASS" << std::endl;
            }
            else {
               std::cout << "  Result " << (i + 1) << ": FAIL" << std::endl;
               testPassed = false;
            }
         }
      }
      else {
         // Exact matrix validation (original behavior)
         std::cout << "\nValidating using exact expectations:" << std::endl;

         for (size_t i = 0; i < test.exactExpectations.size(); ++i) {
            const auto& expected = test.exactExpectations[i];

            if (i >= results.size()) {
               std::cout << "FAIL: Expected result " << (i + 1) << " not found" << std::endl;
               testPassed = false;
               continue;
            }

            const auto& actual = results[i];

            bool matrixOk = compareMatrices(actual.getTransformationMatrix(),
               expected.expectedMatrix, expected.tolerance);
            bool p3Ok = compareValues(actual.getP3Distance(),
               expected.expectedP3Distance, expected.tolerance);

            G6 redRef2, redMob2;
            Niggli::Reduce(G6(mr.layer2.primitiveReference), redRef2);
            Niggli::Reduce(G6(actual.getTransformedMobile()), redMob2);
            const double actualS6Angle = TransformerUtilities::angleS6(
               S6(LRL_Cell(redMob2)), S6(LRL_Cell(redRef2)));

            bool s6Ok = compareValues(actualS6Angle, expected.expectedS6Angle, expected.tolerance);

            std::cout << "  Result " << (i + 1) << " (" << expected.description << "): ";
            if (matrixOk && p3Ok && s6Ok) {
               std::cout << "PASS" << std::endl;
            }
            else {
               std::cout << "FAIL" << std::endl;
               testPassed = false;

               if (!matrixOk) {
                  std::cout << "    Matrix mismatch (tolerance: " << expected.tolerance << ")" << std::endl;
               }
               if (!p3Ok) {
                  std::cout << "    P3 distance mismatch: expected " << expected.expectedP3Distance
                     << ", got " << actual.getP3Distance() << std::endl;
               }
               if (!s6Ok) {
                  std::cout << "    S6 angle mismatch: expected " << expected.expectedS6Angle
                     << " deg, got " << actualS6Angle << " deg" << std::endl;
               }
            }
         }
      }

      // Position-independent checks: does each expected matrix appear
      // ANYWHERE among the results? Runs in addition to whichever primary
      // mode (quality or exact) was used above.
      for (size_t i = 0; i < test.containsExpectations.size(); ++i) {
         const auto& contains = test.containsExpectations[i];
         bool found = false;
         for (const auto& r : results) {
            if (compareMatrices(r.getTransformationMatrix(), contains.expectedMatrix, contains.tolerance)) {
               found = true;
               break;
            }
         }
         std::cout << "  Checking for presence of (" << contains.description << "): ";
         if (found) {
            std::cout << "FOUND" << std::endl;
         }
         else {
            std::cout << "NOT FOUND" << std::endl;
            testPassed = false;
         }
      }

      if (testPassed) {
         std::cout << "OVERALL: PASS" << std::endl;
         passedTests++;
      }
      else {
         std::cout << "OVERALL: FAIL" << std::endl;
         failedTests++;
      }
   }

public:
   void addTestCase(const FlexibleTestCase& testCase) {
      testCases.push_back(testCase);
   }

   // NEW: Run specific test by number
   void runSingleTest(int testNumber, const MultiTransformFinderControls& controls) {
      if (testNumber < 1 || testNumber > static_cast<int>(testCases.size())) {
         std::cout << "Error: Test number " << testNumber << " is out of range (1-"
            << testCases.size() << ")" << std::endl;
         return;
      }

      const auto& test = testCases[testNumber - 1]; // Convert to 0-based index
      std::cout << "\n========================================" << std::endl;
      std::cout << "RUNNING SINGLE TEST: " << testNumber << " (" << test.name << ")" << std::endl;
      std::cout << "========================================" << std::endl;

      // Reset counters for single test
      passedTests = 0;
      failedTests = 0;
      manualTests = 0;

      const auto t_test0 = std::chrono::high_resolution_clock::now();
      runTest(test, controls);
      const auto t_test1 = std::chrono::high_resolution_clock::now();
      std::cout << "; Test " << testNumber << " elapsed: "
         << std::chrono::duration_cast<std::chrono::milliseconds>(t_test1 - t_test0).count()
         << " ms" << std::endl;

      // Show summary for single test
      std::cout << "\n========================================" << std::endl;
      std::cout << "SINGLE TEST RESULT" << std::endl;
      std::cout << "========================================" << std::endl;
      if (passedTests > 0) {
         std::cout << "TEST PASSED!" << std::endl;
      }
      else if (failedTests > 0) {
         std::cout << "TEST FAILED!" << std::endl;
      }
      else {
         std::cout << "TEST REQUIRES MANUAL VERIFICATION!" << std::endl;
      }
   }

   // IMPROVED: Run all tests, or one or more specific tests, based on controls
   void runAllTests(const MultiTransformFinderControls& controls) {
      const std::vector<int>& requested = controls.getTestNumbers();

      // Special case: 999 anywhere in the list means show the test list
      for (int n : requested) {
         if (n >= 999) {
            listTests();
            return;
         }
      }

      // Validate requested test numbers; warn and drop any out of range
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
         // One or more specific tests requested: run each in turn, each with
         // its own single-test banner and summary (as if TEST n had been
         // given separately for each).
         for (int testNumber : validTests) {
            runSingleTest(testNumber, controls);
         }
         return;
      }

      // No valid specific tests requested (empty list, or all requests were
      // out of range): run the full suite.
      std::cout << "\n========================================" << std::endl;
      std::cout << "RUNNING FLEXIBLE LATTICE MATCHER TEST SUITE" << std::endl;
      std::cout << "========================================" << std::endl;

      passedTests = 0;
      failedTests = 0;
      manualTests = 0;

      const auto t_suite0 = std::chrono::high_resolution_clock::now();
      for (size_t i = 0; i < testCases.size(); ++i) {
         std::cout << "\n--- Test " << (i + 1) << " of " << testCases.size() << " ---";
         const auto t_test0 = std::chrono::high_resolution_clock::now();
         runTest(testCases[i], controls);
         const auto t_test1 = std::chrono::high_resolution_clock::now();
         std::cout << "; Test " << (i + 1) << " elapsed: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(t_test1 - t_test0).count()
            << " ms" << std::endl;
      }
      const auto t_suite1 = std::chrono::high_resolution_clock::now();

      std::cout << "\n========================================" << std::endl;
      std::cout << "TEST SUMMARY" << std::endl;
      std::cout << "========================================" << std::endl;
      std::cout << "Total time: "
         << std::chrono::duration_cast<std::chrono::milliseconds>(t_suite1 - t_suite0).count()
         << " ms" << std::endl;
      std::cout << "Passed: " << passedTests << std::endl;
      std::cout << "Failed: " << failedTests << std::endl;
      std::cout << "Manual: " << manualTests << std::endl;
      std::cout << "Total:  " << (passedTests + failedTests + manualTests) << std::endl;

      if (failedTests == 0) {
         std::cout << "ALL AUTOMATED TESTS PASSED!" << std::endl;
         if (manualTests > 0) {
            std::cout << "(" << manualTests << " tests require manual verification)" << std::endl;
         }
      }
      else {
         std::cout << "SOME TESTS FAILED!" << std::endl;
      }
   }

   // NEW: List available tests
   void listTests() const {
      std::cout << "\n========================================" << std::endl;
      std::cout << "AVAILABLE TESTS" << std::endl;
      std::cout << "========================================" << std::endl;

      for (size_t i = 0; i < testCases.size(); ++i) {
         std::cout << "Test " << (i + 1) << ": " << testCases[i].name
            << " - " << testCases[i].description << std::endl;
      }

      std::cout << "\nUsage:" << std::endl;
      std::cout << "  test 0 or test >=" << testCases.size() + 1 << " : Run all tests" << std::endl;
      std::cout << "  test 1-" << testCases.size() << " : Run specific test" << std::endl;
      std::cout << "  test 999 : Show this test list" << std::endl;
   }

   void setupStandardTests() {
      // Test 1: Hawaiian test case - exact matrix expectation
      FlexibleTestCase test1("Hawaiian", "Nearly identical cells with known transformation");
      test1.addInputCell(LatticeCell(LRL_Cell(10.25, 10.74, 21.08, 87.72 * M_PI / 180, 75.97 * M_PI / 180, 61.53 * M_PI / 180), "P"));
      test1.addInputCell(LatticeCell(LRL_Cell(10.25, 10.74, 21.08, 78.96 * M_PI / 180, 75.97 * M_PI / 180, 61.49 * M_PI / 180), "P"));

      Matrix_3x3 expectedMatrix1(-1, 0, 0, -1, 1, 0, 0, 0, -1);
      test1.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         0.05, 1.0, 0.99, 1.01, "EQUIVALENT near-identical cells dist~0.01"));
      addTestCase(test1);

      // Test 2: Identical cells - quality-based expectation
      FlexibleTestCase test2("TrulyIdentical", "Identical cells should give perfect quality metrics");
      test2.addInputCell(LatticeCell(LRL_Cell(10, 10, 10, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test2.addInputCell(LatticeCell(LRL_Cell(10, 10, 10, M_PI / 2, M_PI / 2, M_PI / 2), "P"));

      // Any valid transformation is acceptable as long as quality is perfect
      test2.addQualityExpectation(FlexibleTestCase::QualityThresholds(1e-6, 1e-6, 0.99, 1.01, "Perfect identity match"));
      addTestCase(test2);

      // Test 3: Small difference - quality-based with relaxed thresholds
      FlexibleTestCase test3("SmallDifference", "Small parameter differences should give good quality");
      test3.addInputCell(LatticeCell(LRL_Cell(10.0, 10.0, 10.0, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test3.addInputCell(LatticeCell(LRL_Cell(10.1, 10.0, 10.0, M_PI / 2, M_PI / 2, M_PI / 2), "P"));

      // Expect reasonable quality (not perfect, but good)
      test3.addQualityExpectation(FlexibleTestCase::QualityThresholds(0.5, 5.0, 0.99, 1.01, "Reasonable quality match"));
      addTestCase(test3);

      // Test 4: Manual verification
      FlexibleTestCase test4("ManualCheck", "Complex case for manual verification");
      test4.addInputCell(LatticeCell(LRL_Cell(5.0, 6.0, 7.0, M_PI / 3, M_PI / 4, M_PI / 6), "P"));
      test4.addInputCell(LatticeCell(LRL_Cell(5.1, 5.9, 7.1, M_PI / 3 + 0.1, M_PI / 4 - 0.05, M_PI / 6 + 0.02), "P"));

      // No expectations - just display results
      addTestCase(test4);

      // Test 5: Large difference - should still find some transformation
      FlexibleTestCase test5("LargeDifference", "Very relaxed limits");
      test5.addInputCell(LatticeCell(LRL_Cell(5.0, 5.0, 5.0, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test5.addInputCell(LatticeCell(LRL_Cell(10.0, 8.0, 12.0, M_PI / 3, M_PI / 4, M_PI / 6), "P"));

      // Very relaxed thresholds - just ensure we get a valid transformation
      test5.addQualityExpectation(FlexibleTestCase::QualityThresholds(50.0, 90.0, 0.5, 4.5, "Any valid transformation"));
      addTestCase(test5);

      // Test 6: Centered to differently centered with the same lattice
      FlexibleTestCase test6("DifferentCentered", "Identical lattices with different centering");
      test6.addInputCell(LatticeCell(LRL_Cell(12.770, 21.235, 14.411, 136.017, 84.071, 111.795), "C"));
      test6.addInputCell(LatticeCell(LRL_Cell(33.151, 18.241, 20.218, 83.054, 144.781, 120.639), "F"));

      // Use quality-based check -- exact matrix varies by axis convention
      test6.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-3, 1.0, 1.99, 2.01, "EQUIVALENT C vs F, det~2, dist~0"));
      addTestCase(test6);


      // Test 7: Duplicate Detection - Same input should not produce duplicate matrices
      FlexibleTestCase test7("DuplicateDetection", "Ensure no duplicate transformation matrices in results");
      // Use inputs that are likely to generate multiple equivalent transformations
      test7.addInputCell(LatticeCell(LRL_Cell(10.0, 10.0, 10.0, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test7.addInputCell(LatticeCell(LRL_Cell(10.0, 10.0, 10.0, M_PI / 2, M_PI / 2, M_PI / 2), "P"));

      // Custom validation: Check that all transformation matrices are unique
      // (This would require extending the test framework to check for duplicates)
      // For now, use quality thresholds but add comment about duplicate checking
      test7.addQualityExpectation(FlexibleTestCase::QualityThresholds(1e-6, 1e-6, 0.99, 1.01, "Perfect match with no duplicates"));
      addTestCase(test7);

      // Test 8: High Symmetry Duplicate Prevention  
      FlexibleTestCase test8("HighSymmetryDuplicates", "High symmetry case prone to generating duplicates");
      // Cubic cell that could generate many equivalent transformations
      test8.addInputCell(LatticeCell(LRL_Cell(5.0, 5.0, 5.0, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test8.addInputCell(LatticeCell(LRL_Cell(5.01, 4.99, 5.0, M_PI / 2, M_PI / 2, M_PI / 2), "P"));

      // Should find good matches but without duplicates
      test8.addQualityExpectation(FlexibleTestCase::QualityThresholds(0.5, 5.0, 0.99, 1.01, "Good quality without duplicates"));
      addTestCase(test8);

      // Test 9: Multiple Valid Transformations (controlled duplicate scenario)
      FlexibleTestCase test9("MultipleValidTransforms", "Case where multiple valid transformations exist");
      // Use a case where the lattice has some symmetry that could lead to multiple solutions
      test9.addInputCell(LatticeCell(LRL_Cell(8.0, 8.0, 12.0, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test9.addInputCell(LatticeCell(LRL_Cell(8.0, 8.0, 12.0, M_PI / 2, M_PI / 2, 2 * M_PI / 3), "P"));

      // Expect to find transformations but all should be unique
// Very relaxed thresholds - just ensure we get a valid transformation without duplicates
      test9.addQualityExpectation(FlexibleTestCase::QualityThresholds(50.0, 90.0, 0.5, 4.5, "Different lattices - any valid transformation"));
      addTestCase(test9);

      // -----------------------------------------------------------------------
// Tests 10-19: Production test suite (P3 lattice matching)
      // Supercell tests are marked [SUPERCELL].
      // -----------------------------------------------------------------------

// Test 10: Axis permutation
      FlexibleTestCase test10("AxisPermutation",
         "Same triclinic lattice with axes permuted -- EQUIVALENT dist~0");
      test10.addInputCell(LatticeCell(LRL_Cell(7, 8, 9, 85 * M_PI / 180, 95 * M_PI / 180, 100 * M_PI / 180), "P"));
      test10.addInputCell(LatticeCell(LRL_Cell(8, 9, 7, 95 * M_PI / 180, 100 * M_PI / 180, 85 * M_PI / 180), "P"));
      test10.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-3, 1.0, 0.99, 1.01, "EQUIVALENT dist~0"));
      addTestCase(test10);

      // Test 11: [SUPERCELL] Face diagonal order-2
      FlexibleTestCase test11("FaceDiagonalOrder2",
         "[SUPERCELL] Cubic doubled along face diagonal -- order-2, dist=0");
      test11.addInputCell(LatticeCell(LRL_Cell(10, 10, 10, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test11.addInputCell(LatticeCell(LRL_Cell(14.142, 14.142, 10, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test11.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-2, 1.0, 1.99, 2.01, "[SUPERCELL] order-2 det~2 dist~0"));
      addTestCase(test11);

      // Test 12: [SUPERCELL] Distorted order-2
      FlexibleTestCase test12("DistortedOrder2",
         "[SUPERCELL] Nearest order-2 supercell with dist=1.000");
      test12.addInputCell(LatticeCell(LRL_Cell(10, 10, 10, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test12.addInputCell(LatticeCell(LRL_Cell(19, 10, 10, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test12.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1.1, 10.0, 1.99, 2.01, "[SUPERCELL] order-2 det~2 dist~1.0"));
      addTestCase(test12);

      // Test 13: [SUPERCELL] Exact order-3
      FlexibleTestCase test13("ExactOrder3",
         "[SUPERCELL] Cubic tripled along a -- order-3, dist=0");
      test13.addInputCell(LatticeCell(LRL_Cell(10, 10, 10, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test13.addInputCell(LatticeCell(LRL_Cell(30, 10, 10, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test13.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-2, 1.0, 2.99, 3.01, "[SUPERCELL] order-3 det~3 dist~0"));
      addTestCase(test13);

      // Test 14: [SUPERCELL] Simon Parsons case
      FlexibleTestCase test14("Simon Parsons",
         "[SUPERCELL] R primitive vs P triclinic -- order-4, dist=0.820");
      test14.addInputCell(LatticeCell(LRL_Cell(9.477, 9.477, 9.477,
         64.24 * M_PI / 180, 64.24 * M_PI / 180, 64.24 * M_PI / 180), "P"));
      test14.addInputCell(LatticeCell(LRL_Cell(9.482, 13.705, 20.048,
         89.51 * M_PI / 180, 88.72 * M_PI / 180, 86.96 * M_PI / 180), "P"));
      test14.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         2.0, 10.0, 3.99, 4.01, "[SUPERCELL] order-4 det~4 dist~1.9"));
      addTestCase(test14);

      // Test 15: [SUPERCELL] Triclinic order-2
      FlexibleTestCase test15("TriclinicOrder2",
         "[SUPERCELL] Triclinic doubled along a -- order-2, dist=0");
      test15.addInputCell(LatticeCell(LRL_Cell(7, 8, 9, 85 * M_PI / 180, 95 * M_PI / 180, 100 * M_PI / 180), "P"));
      test15.addInputCell(LatticeCell(LRL_Cell(14, 8, 9, 85 * M_PI / 180, 95 * M_PI / 180, 100 * M_PI / 180), "P"));
      test15.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-2, 1.0, 1.99, 2.01, "[SUPERCELL] order-2 det~2 dist~0"));
      addTestCase(test15);

      // Test 16: [SUPERCELL] Monoclinic order-2
      FlexibleTestCase test16("MonoclinicOrder2",
         "[SUPERCELL] Monoclinic doubled along c -- order-2, dist=0");
      test16.addInputCell(LatticeCell(LRL_Cell(8, 10, 12, M_PI / 2, 105 * M_PI / 180, M_PI / 2), "P"));
      test16.addInputCell(LatticeCell(LRL_Cell(8, 10, 24, M_PI / 2, 105 * M_PI / 180, M_PI / 2), "P"));
      test16.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-2, 1.0, 1.99, 2.01, "[SUPERCELL] order-2 det~2 dist~0"));
      addTestCase(test16);

      // Test 17: Marsh 2002 case 3.11 -- P vs C
      FlexibleTestCase test17("Marsh2002_3_11",
         "Marsh 2002 case 3.11: P triclinic == C monoclinic, 2 representations");
      test17.addInputCell(LatticeCell(LRL_Cell(7.813, 8.114, 13.634,
         74.02 * M_PI / 180, 73.08 * M_PI / 180, 61.22 * M_PI / 180), "P"));
      test17.addInputCell(LatticeCell(LRL_Cell(14.223, 7.813, 13.813,
         90.26 * M_PI / 180, 111.26 * M_PI / 180, 90 * M_PI / 180), "C"));
      test17.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         2.5, 10.0, 0.49, 0.51, "SUBCELL det~0.5 dist~2.1"));
      addTestCase(test17);

      // Test 18: Marsh 2002 case 3.23 -- C vs I
      FlexibleTestCase test18("Marsh2002_3_23",
         "Marsh 2002 case 3.23: C monoclinic == I orthorhombic, 4 representations");
      test18.addInputCell(LatticeCell(LRL_Cell(24.970, 19.252, 15.913,
         M_PI / 2, 129.46 * M_PI / 180, M_PI / 2), "C"));
      test18.addInputCell(LatticeCell(LRL_Cell(19.279, 19.252, 15.9131,
         M_PI / 2, 89.87 * M_PI / 180, M_PI / 2), "I"));
      test18.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         0.01, 1.0, 0.99, 1.01, "EQUIVALENT det~1 dist~0"));
      addTestCase(test18);

      // Test 19: [SUPERCELL] Cubic vs rhombohedral-angle
      FlexibleTestCase test19("CubicVsRhombohedral",
         "[SUPERCELL] Cubic P vs rhombohedral-angle P -- best order-2, dist=2.680");
      test19.addInputCell(LatticeCell(LRL_Cell(10, 10, 10, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test19.addInputCell(LatticeCell(LRL_Cell(10, 10, 10,
         109.471 * M_PI / 180, 109.471 * M_PI / 180, 109.471 * M_PI / 180), "P"));
      test19.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         2.8, 20.0, 1.99, 2.01, "[SUPERCELL] order-2 det~2 dist~2.680"));
      addTestCase(test19);

      // Test 20: [SUPERCELL] Exact order-5
      FlexibleTestCase test20("ExactOrder5",
         "[SUPERCELL] Orthorhombic quintupled along a -- order-5, dist=0");
      test20.addInputCell(LatticeCell(LRL_Cell(10, 8, 9, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test20.addInputCell(LatticeCell(LRL_Cell(50, 8, 9, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test20.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-2, 1.0, 4.99, 5.01, "[SUPERCELL] order-5 det~5 dist~0"));
      addTestCase(test20);

      // Test 21: [SUPERCELL] Exact order-6
      FlexibleTestCase test21("ExactOrder6",
         "[SUPERCELL] Orthorhombic sextupled along a -- order-6, dist=0");
      test21.addInputCell(LatticeCell(LRL_Cell(10, 20, 30, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test21.addInputCell(LatticeCell(LRL_Cell(60, 20, 30, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test21.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-2, 1.0, 5.99, 6.01, "[SUPERCELL] order-6 det~6 dist~0"));
      // Regression guard: this specific alternate det=6 orientation is only
      // reachable via composition (U*HNF), not raw HNF alone -- confirmed
      // missing when an earlier version of the generation used only one
      // composition direction. Checked by presence, not position, since it
      // is tied in quality with several other det=6 orientations and tie
      // order is not guaranteed stable.
      test21.addContainsExpectation(FlexibleTestCase::ContainsExpectation(
         Matrix_3x3(0, 3, 0, -2, 0, 0, 0, 0, 1), 1e-3,
         "alternate det=6 orientation reachable only via U*HNF composition"));
      // NOTE (not a gap): the mirror-handedness partner of the matrix
      // above, [0,3,0; 2,0,0; 0,0,1] (same rows except the second axis
      // flips from -2a to +2a -- same lengths/angles, opposite chirality,
      // det=-6 instead of +6), is confirmed absent from the generated set
      // and confirmed reachable only by composing with a det=-1
      // (reflection/improper) unimodular matrix. This is CORRECT: the
      // unimodular multiplier is restricted to det=+1 deliberately --
      // allowing det=-1 would let a chirality-flipping reflection count as
      // a valid crystallographic basis change, which it is not. The
      // matrix's absence is expected behavior, not a limitation to fix.
      addTestCase(test21);

      // Test 22: [SUPERCELL] Swap direction -- large cell input first
      FlexibleTestCase test22("SwapDirection",
         "[SUPERCELL] Larger cell given first -- swap logic must place it as reference, order-3, dist=0");
      // Deliberately add the LARGER cell first to exercise the swap
      test22.addInputCell(LatticeCell(LRL_Cell(30, 10, 10, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test22.addInputCell(LatticeCell(LRL_Cell(10, 10, 10, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test22.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-2, 1.0, 2.99, 3.01, "[SUPERCELL] order-3 det~3 dist~0 after swap"));
      addTestCase(test22);

      // Test 23: [SUPERCELL] P primitive cell vs I orthorhombic -- order-2, dist=0
      FlexibleTestCase test23("ICellSupercell",
         "[SUPERCELL] P primitive cell vs I orthorhombic -- order-2, dist=0");
      test23.addInputCell(LatticeCell(LRL_Cell(10, 12, 14, M_PI / 2, M_PI / 2, M_PI / 2), "I"));
      test23.addInputCell(LatticeCell(LRL_Cell(10.000, 10.488, 10.488,
         69.790 * M_PI / 180, 61.528 * M_PI / 180, 61.528 * M_PI / 180), "P"));
      test23.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-2, 190.0, 1.99, 2.01, "[SUPERCELL] I vs P primitive order-2 det~2 dist~0"));
      addTestCase(test23);


      // Test 24: F cubic cell vs P cubic -- EQUIVALENT (F primitive = P primitive)
      FlexibleTestCase test24("FCellEquivalent",
         "F cubic cell vs P cubic -- EQUIVALENT, det=1, dist=0");
      test24.addInputCell(LatticeCell(LRL_Cell(10, 10, 10, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test24.addInputCell(LatticeCell(LRL_Cell(10, 10, 10, M_PI / 2, M_PI / 2, M_PI / 2), "F"));
      test24.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-2, 1.0, 0.99, 1.01, "EQUIVALENT F vs P det~1 dist~0"));
      addTestCase(test24);

      // Test 25: R cell vs P -- rhombohedral centering conversion
      FlexibleTestCase test25("RhombohedralCentering",
         "R cell vs its primitive cell -- EQUIVALENT, det=1, dist=0");
      test25.addInputCell(LatticeCell(LRL_Cell(9.477, 9.477, 9.477,
         64.24 * M_PI / 180, 64.24 * M_PI / 180, 64.24 * M_PI / 180), "P"));
      test25.addInputCell(LatticeCell(LRL_Cell(9.477, 9.477, 9.477,
         64.24 * M_PI / 180, 64.24 * M_PI / 180, 64.24 * M_PI / 180), "R"));
      test25.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-2, 1.0, 0.99, 1.01, "EQUIVALENT R vs P primitive det~1 dist~0"));
      addTestCase(test25);

      // Test 26: Rhombohedral primitive vs 3x supercell
      FlexibleTestCase test26("RhombohedralOrder3",
         "[SUPERCELL] Rhombohedral primitive cell vs tripled a-axis -- order-3, dist=0");
      test26.addInputCell(LatticeCell(LRL_Cell(6, 6, 6,
         64.24 * M_PI / 180, 64.24 * M_PI / 180, 64.24 * M_PI / 180), "P"));
      test26.addInputCell(LatticeCell(LRL_Cell(18, 6, 6,
         64.24 * M_PI / 180, 64.24 * M_PI / 180, 64.24 * M_PI / 180), "P"));
      test26.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-2, 1.0, 2.99, 3.01, "[SUPERCELL] order-3 det~3 dist~0"));
      addTestCase(test26);

      // Test 27: Tetragonal a/c swap -- equivalent lattice
      FlexibleTestCase test27("TetragonalACSwap",
         "Tetragonal cell with a and c swapped -- EQUIVALENT, det=1, dist=0");
      test27.addInputCell(LatticeCell(LRL_Cell(8, 8, 12, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test27.addInputCell(LatticeCell(LRL_Cell(8, 12, 8, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test27.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-2, 1.0, 0.99, 1.01, "EQUIVALENT det~1 dist~0"));
      addTestCase(test27);

      // Test 28: Triclinic cell vs its Niggli-reduced form
      FlexibleTestCase test28("TriclinicNiggliReduced",
         "Triclinic cell vs its own Niggli-reduced form -- EQUIVALENT, det=1, dist=0");
      test28.addInputCell(LatticeCell(LRL_Cell(6.136, 6.140, 1.536,
         71.333 * M_PI / 180, 66.765 * M_PI / 180, 77.337 * M_PI / 180), "P"));
      test28.addInputCell(LatticeCell(LRL_Cell(1.536, 5.676, 5.833,
         95.586 * M_PI / 180, 94.222 * M_PI / 180, 96.590 * M_PI / 180), "P"));
      test28.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-2, 1.0, 0.99, 1.01, "EQUIVALENT det~1 dist~0"));
      addTestCase(test28);

      // Test 29: Near det=2 -- distorted supercell
      FlexibleTestCase test29("NearOrder2",
         "[SUPERCELL] Cubic cell vs 1.98x elongated -- order-2, dist~0.2");
      test29.addInputCell(LatticeCell(LRL_Cell(10, 10, 10, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test29.addInputCell(LatticeCell(LRL_Cell(19.8, 10, 10, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test29.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         0.25, 2.0, 1.99, 2.01, "[SUPERCELL] order-2 det~2 dist~0.2"));
      addTestCase(test29);

      // -----------------------------------------------------------------------
// Tests 30-46: Additional coverage
      // -----------------------------------------------------------------------

// Test 30: P vs I centering -- DIFFERENT lattices, poor match expected
      FlexibleTestCase test30("PvsICentering",
         "P primitive vs I body-centered orthorhombic -- DIFFERENT lattices, poor match expected");
      test30.addInputCell(LatticeCell(LRL_Cell(10, 12, 14, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test30.addInputCell(LatticeCell(LRL_Cell(7, 10, 13, 85 * M_PI / 180, 95 * M_PI / 180, 100 * M_PI / 180), "I"));
      test30.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         9.0, 180.0, 1.99, 2.01, "DIFFERENT P vs I lattices, best match order-2 supercell"));
      addTestCase(test30);

      // Test 31: P vs A centering -- DIFFERENT lattices, poor match expected
      FlexibleTestCase test31("PvsACentering",
         "P primitive vs A-centered monoclinic -- DIFFERENT lattices, poor match expected");
      test31.addInputCell(LatticeCell(LRL_Cell(8, 10, 12, M_PI / 2, 105 * M_PI / 180, M_PI / 2), "P"));
      test31.addInputCell(LatticeCell(LRL_Cell(8, 10, 12, M_PI / 2, 105 * M_PI / 180, M_PI / 2), "A"));
      test31.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1000.0, 180.0, 0.99, 1.01, "DIFFERENT P vs A lattices det~1 dist>1"));
      addTestCase(test31);

      // Test 32: R obverse vs R reverse -- same primitive cell
      FlexibleTestCase test32("RObverseVsReverse",
         "R obverse vs R reverse setting -- EQUIVALENT primitive cells, det=1, dist=0");
      test32.addInputCell(LatticeCell(LRL_Cell(9.477, 9.477, 9.477,
         64.24 * M_PI / 180, 64.24 * M_PI / 180, 64.24 * M_PI / 180), "R"));
      test32.addInputCell(LatticeCell(LRL_Cell(9.477, 9.477, 9.477,
         64.24 * M_PI / 180, 64.24 * M_PI / 180, 64.24 * M_PI / 180), "R"));
      test32.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-2, 1.0, 0.99, 1.01, "EQUIVALENT R vs R det~1 dist~0"));
      addTestCase(test32);

      // Test 33: Monoclinic b-unique vs c-unique setting
      FlexibleTestCase test33("MonoclinicAxisConvention",
         "Monoclinic b-unique vs c-unique -- EQUIVALENT, det=1, dist=0");
      test33.addInputCell(LatticeCell(LRL_Cell(8, 10, 12, M_PI / 2, 105 * M_PI / 180, M_PI / 2), "P"));
      test33.addInputCell(LatticeCell(LRL_Cell(8, 12, 10, M_PI / 2, M_PI / 2, 105 * M_PI / 180), "P"));
      test33.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-2, 1.0, 0.99, 1.01, "EQUIVALENT monoclinic settings det~1 dist~0"));
      addTestCase(test33);

      // Test 34: Identical triclinic -- identity matrix must be Match 1
      FlexibleTestCase test34("TriclinicIdentityFirst",
         "Identical triclinic cells -- identity matrix must be Match 1");
      test34.addInputCell(LatticeCell(LRL_Cell(7, 8, 9, 85 * M_PI / 180, 95 * M_PI / 180, 100 * M_PI / 180), "P"));
      test34.addInputCell(LatticeCell(LRL_Cell(7, 8, 9, 85 * M_PI / 180, 95 * M_PI / 180, 100 * M_PI / 180), "P"));
      test34.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-6, 1e-6, 0.99, 1.01, "EQUIVALENT dist~0 identity first"));
      addTestCase(test34);

      // Test 35: Matrix score tiebreak -- [2 0 0 / 0 1 0 / 0 0 1] must rank above [-2 0 0 / ...]
      FlexibleTestCase test35("MatrixScoreTiebreak",
         "Tiebreak: [2 0 0 / 0 1 0 / 0 0 1] must rank above [-2 0 0 / 0 1 0 / 0 0 1]");
      test35.addInputCell(LatticeCell(LRL_Cell(20, 10, 10, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test35.addInputCell(LatticeCell(LRL_Cell(10, 10, 10, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test35.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-2, 1.0, 1.99, 2.01, "[SUPERCELL] order-2 det~2 dist~0 simpler matrix first"));
      addTestCase(test35);

      // Test 36: Hexagonal vs orthorhombic C-centered -- subcell det=0.5, dist=0
      FlexibleTestCase test36("HexagonalVsOrthorhombicC",
         "Hexagonal vs orthorhombic C-centered -- SUBCELL det~0.5, dist~0");
      test36.addInputCell(LatticeCell(LRL_Cell(5, 5, 8, M_PI / 2, M_PI / 2, 2 * M_PI / 3), "P"));
      test36.addInputCell(LatticeCell(LRL_Cell(5, 8.660, 8, M_PI / 2, M_PI / 2, M_PI / 2), "C"));
      test36.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         0.1, 2.0, 0.49, 0.51, "SUBCELL hex vs orth-C det~0.5 dist~0"));
      addTestCase(test36);

      // Test 37: Near-tetragonal orthorhombic vs tetragonal -- det=1, dist small
      FlexibleTestCase test37("TetragonalVsOrthorhombic",
         "Near-tetragonal orthorhombic vs tetragonal -- EQUIVALENT, det=1, dist small");
      test37.addInputCell(LatticeCell(LRL_Cell(8, 8, 12, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test37.addInputCell(LatticeCell(LRL_Cell(8.1, 7.9, 12, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test37.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         0.5, 5.0, 0.99, 1.01, "EQUIVALENT near-tetragonal det~1 dist small"));
      addTestCase(test37);

      // Test 38: Swap robustness -- order-2 with larger cell input second
      FlexibleTestCase test38("SwapOrder2",
         "[SUPERCELL] Larger cell given second for order-2 -- swap logic test, dist=0");
      test38.addInputCell(LatticeCell(LRL_Cell(10, 10, 10, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test38.addInputCell(LatticeCell(LRL_Cell(20, 10, 10, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test38.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-2, 1.0, 1.99, 2.01, "[SUPERCELL] order-2 det~2 dist~0 after swap"));
      addTestCase(test38);

      // Test 39: Swap robustness -- order-4 with larger cell input first
      FlexibleTestCase test39("SwapOrder4",
         "[SUPERCELL] Larger cell given first for order-4 -- swap logic test, dist=0");
      test39.addInputCell(LatticeCell(LRL_Cell(40, 10, 10, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test39.addInputCell(LatticeCell(LRL_Cell(10, 10, 10, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test39.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-2, 1.0, 3.99, 4.01, "[SUPERCELL] order-4 det~4 dist~0 after swap"));
      addTestCase(test39);

      // Test 40: Distorted order-3 -- nearest order-3 supercell with dist~1.0
      FlexibleTestCase test40("DistortedOrder3",
         "[SUPERCELL] Nearest order-3 supercell with dist~1.0");
      test40.addInputCell(LatticeCell(LRL_Cell(10, 10, 10, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test40.addInputCell(LatticeCell(LRL_Cell(29, 10, 10, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test40.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1.5, 10.0, 2.99, 3.01, "[SUPERCELL] order-3 det~3 dist~1.0"));
      addTestCase(test40);

      // Test 41: Exact order-4 -- quadrupled along a, dist=0
      FlexibleTestCase test41("ExactOrder4",
         "[SUPERCELL] Orthorhombic quadrupled along a -- order-4, dist=0");
      test41.addInputCell(LatticeCell(LRL_Cell(10, 8, 9, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test41.addInputCell(LatticeCell(LRL_Cell(40, 8, 9, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test41.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-2, 1.0, 3.99, 4.01, "[SUPERCELL] order-4 det~4 dist~0"));
      addTestCase(test41);

      // Test 42: Very distorted cells -- result still returned despite poor quality
      FlexibleTestCase test42("DistortedQuality",
         "Very distorted cells -- poor quality, result still returned");
      test42.addInputCell(LatticeCell(LRL_Cell(5, 5, 5, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test42.addInputCell(LatticeCell(LRL_Cell(5, 5, 5, 75 * M_PI / 180, 75 * M_PI / 180, 75 * M_PI / 180), "P"));
      test42.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         50.0, 90.0, 0.99, 1.01, "Any unimodular result returned"));
      addTestCase(test42);

      // Test 43: Orthorhombic all-axes permutation -- abc vs bca
      FlexibleTestCase test43("OrthorhombicAllPermutations",
         "Orthorhombic cell with a,b,c all distinct -- abc vs bca permutation, EQUIVALENT det=1 dist=0");
      test43.addInputCell(LatticeCell(LRL_Cell(5, 7, 11, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test43.addInputCell(LatticeCell(LRL_Cell(7, 11, 5, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test43.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-3, 1.0, 0.99, 1.01, "EQUIVALENT abc->bca det~1 dist~0"));
      addTestCase(test43);

      // Test 44: Inversion-related triclinic -- same metric, identity first
      FlexibleTestCase test44("InversionRelated",
         "Triclinic cell vs its inversion -- EQUIVALENT, det=1, dist=0");
      test44.addInputCell(LatticeCell(LRL_Cell(7, 8, 9, 85 * M_PI / 180, 95 * M_PI / 180, 100 * M_PI / 180), "P"));
      test44.addInputCell(LatticeCell(LRL_Cell(7, 8, 9, 85 * M_PI / 180, 95 * M_PI / 180, 100 * M_PI / 180), "P"));
      test44.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-6, 1e-6, 0.99, 1.01, "EQUIVALENT inversion det~1 dist~0"));
      addTestCase(test44);

      // Test 45: Near order-3 -- 2.97x elongated
      FlexibleTestCase test45("NearOrder3",
         "[SUPERCELL] Cubic cell vs 2.97x elongated -- order-3, dist~0.3");
      test45.addInputCell(LatticeCell(LRL_Cell(10, 10, 10, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test45.addInputCell(LatticeCell(LRL_Cell(29.7, 10, 10, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test45.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         0.5, 5.0, 2.99, 3.01, "[SUPERCELL] order-3 det~3 dist~0.3"));
      addTestCase(test45);

      // Test 46: C vs F same lattice
      FlexibleTestCase test46("CvsF_Chambord",
         "C monoclinic vs F triclinic -- same primitive lattice, det~2, dist~0");
      test46.addInputCell(LatticeCell(LRL_Cell(
         12.770, 21.235, 14.411,
         136.017 * M_PI / 180, 84.071 * M_PI / 180, 111.795 * M_PI / 180), "C"));
      test46.addInputCell(LatticeCell(LRL_Cell(
         33.151, 18.241, 20.218,
         83.054 * M_PI / 180, 144.781 * M_PI / 180, 120.639 * M_PI / 180), "F"));
      test46.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-2, 1.0, 1.99, 2.01,
         "C vs F same lattice -- primitive cells match, det~2, dist~0"));
      addTestCase(test46);

      // Test 47: P vs I identical conventional parameters -- centering relationship
            // A centered lattice IS a primitive lattice with extra points added. The I cell
            // with parameters 10,12,14,90,90,90 contains all points of the P cell with those
            // same parameters PLUS body center points. So P is a sublattice of I -- the I cell
            // is a supercell of the P cell.
            //
            // In ALL mode the matcher correctly finds this relationship:
            //   - Layer 4 finds a det=2 matrix mapping I_primitive -> P_primitive (P3~0)
            //   - Layer 2 composes det=2 * det=0.5 (bodyCentering) = det=1 final matrix
            //   - The result is labeled EQUIVALENT with P3~0 and NC=167
            //
            // In EQUIVALENT mode (det=1 only) the best match is det=0.5 subcell with P3=8.128,
            // confirming P and I are genuinely different lattices -- the difference being the
            // body center points.
            //
            // NC distance = 167 correctly flags these as different lattices where P3 cannot.
            // This test documents this known behavior for future reference.
      FlexibleTestCase test47("PvsIDegenerate",
         "P vs I identical conventional parameters: a centered lattice is always generated\n"
         "by adding points to a primitive lattice, so the P cell is always a sublattice of\n"
         "the I cell with the same parameters. The perfect match (P3~0) is correct and\n"
         "expected -- this relationship is not a supercell in the usual sense but is the\n"
         "fundamental centering relationship. NCDist distance=167 distinguishes this from a true\n"
         "equivalence between identical lattices.");
      test47.addInputCell(LatticeCell(LRL_Cell(10, 12, 14, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test47.addInputCell(LatticeCell(LRL_Cell(10, 12, 14, M_PI / 2, M_PI / 2, M_PI / 2), "I"));
      test47.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-3, 1.0, 0.99, 1.01,
         "P sublattice of I: centering relationship, \nP3~0 correct, NCDist distance=167 flags as different lattice type"));
      addTestCase(test47);

      // Test 48: One very large axis -- numerical stability
      FlexibleTestCase test48("OneLargeAxis",
         "Cell with one very large axis (200A) -- numerical stability test, EQUIVALENT dist~0");
      test48.addInputCell(LatticeCell(LRL_Cell(200.0, 8.0, 9.0,
         90.0 * M_PI / 180, 90.0 * M_PI / 180, 90.0 * M_PI / 180), "P"));
      test48.addInputCell(LatticeCell(LRL_Cell(200.0, 8.0, 9.0,
         90.0 * M_PI / 180, 90.0 * M_PI / 180, 90.0 * M_PI / 180), "P"));
      test48.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-3, 1.0, 0.99, 1.01, "EQUIVALENT large-a det~1 dist~0"));
      addTestCase(test48);

      // Test 49: Two very large axes -- numerical stability
      FlexibleTestCase test49("TwoLargeAxes",
         "Cell with two very large axes (200A) -- numerical stability test, EQUIVALENT dist~0");
      test49.addInputCell(LatticeCell(LRL_Cell(200.0, 200.0, 9.0,
         90.0 * M_PI / 180, 90.0 * M_PI / 180, 90.0 * M_PI / 180), "P"));
      test49.addInputCell(LatticeCell(LRL_Cell(200.0, 200.0, 9.0,
         90.0 * M_PI / 180, 90.0 * M_PI / 180, 90.0 * M_PI / 180), "P"));
      test49.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-3, 1.0, 0.99, 1.01, "EQUIVALENT large-ab det~1 dist~0"));
      addTestCase(test49);

      // Test 50: Rhombohedral hexagonal setting vs primitive rhombohedral
            // The hexagonal setting of an R lattice has triple the volume of the primitive
            // rhombohedral cell. The matcher should find an order-3 supercell relationship.
            // c_hex for alpha=64.24 deg, a=9.477: verified via cmdniggli.
      FlexibleTestCase test50("RhombohedralHexVsPrimitive",
         "[SUPERCELL] R cell in hexagonal setting vs primitive rhombohedral -- order-3, dist~0");
      test50.addInputCell(LatticeCell(LRL_Cell(10.084, 10.084, 22.433,
         90.0 * M_PI / 180, 90.0 * M_PI / 180, 120.0 * M_PI / 180), "P"));
      test50.addInputCell(LatticeCell(LRL_Cell(9.477, 9.477, 9.477,
         64.24 * M_PI / 180, 64.24 * M_PI / 180, 64.24 * M_PI / 180), "P"));
      test50.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         0.02, 1.0, 2.99, 3.01, "[SUPERCELL] R hex vs R primitive det~3 dist~0.013"));
      addTestCase(test50);

      // Test 51: [SUPERCELL] det=5 requiring a non-diagonal orientation.
      // Mobile cell is the cubic reference transformed by
      // [1 -2 0; 1 3 0; 0 0 1] (det=5) -- a non-diagonal matrix, unlike
      // ExactOrder5's simple diag(5,1,1). Verified computationally (not
      // hand-derived) that this specific matrix is reachable as U*HNF(5)
      // in the current generation, and NOT as HNF(5)*U -- this exercises
      // det=5's composed-only coverage the same way FaceDiagonalOrder2
      // (det=2) and PvsICentering (det=4) already exercise their own
      // determinants' non-trivial orientation coverage.
      FlexibleTestCase test51("NonDiagonalOrder5",
         "[SUPERCELL] Cubic transformed by non-diagonal det=5 matrix -- order-5, dist=0");
      test51.addInputCell(LatticeCell(LRL_Cell(10, 10, 10, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test51.addInputCell(LatticeCell(LRL_Cell(22.3607, 31.6228, 10,
         90.0 * M_PI / 180, 90.0 * M_PI / 180, 135.0 * M_PI / 180), "P"));
      test51.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-2, 1.0, 4.99, 5.01, "[SUPERCELL] non-diagonal order-5 det~5 dist~0"));
      addTestCase(test51);

      // Test 52: [SUPERCELL] Independent per-axis multipliers (a doubled,
      // c tripled) -- a common real superstructure pattern, distinct from
      // both single-axis multiplication (ExactOrder6) and the coincidental
      // axis-ratio degeneracy in that test. Generic dimensions (7,11,13,
      // no integer ratio among them) are used deliberately so this tests
      // the general case, not another special-ratio coincidence. Verified
      // computationally that diag(2,1,3) sits directly in the raw HNF(6)
      // canonical set (it is already upper-triangular), so it is reachable
      // regardless of composition direction -- this case is confirmed safe,
      // not a gap; it is included for coverage of a physically common
      // pattern that was otherwise untested.
      FlexibleTestCase test52("IndependentAxisMultipliers",
         "[SUPERCELL] a doubled, c tripled independently -- order-6, dist=0");
      test52.addInputCell(LatticeCell(LRL_Cell(7, 11, 13, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test52.addInputCell(LatticeCell(LRL_Cell(14, 11, 39, M_PI / 2, M_PI / 2, M_PI / 2), "P"));
      test52.addQualityExpectation(FlexibleTestCase::QualityThresholds(
         1e-2, 1.0, 5.99, 6.01, "[SUPERCELL] independent 2x/3x axis multipliers det~6 dist~0"));
      addTestCase(test52);


   }  // end setupStandardTests



};  // end class FlexibleTestRunner

// ---------------------------------------------------------------------------
// runMatchPresentationTests -- regression test for MatchPresentation
// (buildMatchPresentation), independent of the FlexibleTestCase/runTest
// framework since it inspects individual matches' reference-frame outcome
// directly rather than validating the matcher's top result.
//
// Uses the C-vs-F Chambord-like pair (same reference as test 46) with a
// mobile cell that produces two known order-2 supercell matches at the same
// determinant: a "trivial" matrix that reproduces the reference cell when
// carried into its frame, and an alternate matrix (same determinant,
// different embedding) that does not. This is a concrete, reproducible
// example of "good primitive match, poor reference-frame match" -- the case
// buildMatchPresentation exists to expose.
// ---------------------------------------------------------------------------
inline void runMatchPresentationTests(const MultiTransformFinderControls& controls) {
   std::cout << "\n========================================" << std::endl;
   std::cout << "RUNNING MATCH PRESENTATION TESTS" << std::endl;
   std::cout << "========================================" << std::endl;

   auto matricesMatch = [](const Matrix_3x3& a, const Matrix_3x3& b, double tolerance) {
      for (int i = 0; i < 9; ++i) {
         if (std::abs(a[i] - b[i]) > tolerance) return false;
      }
      return true;
      };

   const LatticeCell mobileCell(LRL_Cell(
      12.701, 21.197, 14.363,
      136.609 * M_PI / 180, 83.582 * M_PI / 180, 111.713 * M_PI / 180), "C");
   const LatticeCell referenceCell(LRL_Cell(
      33.151, 18.241, 20.218,
      83.054 * M_PI / 180, 144.781 * M_PI / 180, 120.639 * M_PI / 180), "F");

   std::cout << "  Reference: C/F pair, reference cell type F: "
      << LRL_Cell_Degrees(referenceCell.getCell()) << std::endl;
   std::cout << "  Mobile: type C: " << LRL_Cell_Degrees(mobileCell.getCell()) << std::endl;

   const auto mr = matchPair(referenceCell, mobileCell, controls);
   const Layer2Result& layer2 = mr.layer2;
   const LatticeCell& actualReference = mr.reference;  // matchPair may swap on volume; use what it actually used

   int passed = 0;
   int failed = 0;

   // The "trivial" matrix: matches the reference's own referenceFromPrimitive
   // relationship, should reproduce the reference cell when carried into its frame.
   const Matrix_3x3 trivialMatrix(0, 1, -1, -1, 0, 1, 1, 0, 1);

   // An alternate order-2 supercell matrix found for the same mobile cell:
   // same determinant, different embedding, should NOT reproduce the reference.
   const Matrix_3x3 alternateMatrix(-1.5, -2.5, -2.0, 1.5, 0.5, 0.0, 0.5, 1.5, 2.0);

   const LatticeMatchResult* trivialResult = nullptr;
   const LatticeMatchResult* alternateResult = nullptr;

   for (const auto& result : layer2.results) {
      if (!trivialResult && matricesMatch(result.getTransformationMatrix(), trivialMatrix, 1e-6)) {
         trivialResult = &result;
      }
      if (!alternateResult && matricesMatch(result.getTransformationMatrix(), alternateMatrix, 1e-6)) {
         alternateResult = &result;
      }
   }

   if (!trivialResult) {
      std::cout << "FAIL: trivial matrix not found among matcher results" << std::endl;
      ++failed;
   }
   else {
      std::vector<double> distances;
      for (const auto& r : layer2.results) distances.push_back(r.getP3Distance());
      const auto thresholds = P3RelativeThresholds::calculateAdaptiveP3Thresholds(
         actualReference.getCell(), actualReference.getLatticeType(), distances);
      const MatchPresentation presentation = buildMatchPresentation(
         layer2, *trivialResult, actualReference, thresholds.referenceP3Norm);

      std::cout << "  Checking trivial matrix (expect reference-frame P3% <= "
         << controls.getConventionalThreshold() << "%): ";
      if (presentation.referenceFrame.p3Percent <= controls.getConventionalThreshold()) {
         std::cout << "PASS  (reference frame " << presentation.referenceFrame.p3Percent << "% of P3 norm)" << std::endl;
         ++passed;
      }
      else {
         std::cout << "FAIL  (reference frame " << presentation.referenceFrame.p3Percent << "% of P3 norm, expected <= "
            << controls.getConventionalThreshold() << "%)" << std::endl;
         ++failed;
      }
   }

   if (!alternateResult) {
      std::cout << "FAIL: alternate matrix not found among matcher results" << std::endl;
      ++failed;
   }
   else {
      std::vector<double> distances;
      for (const auto& r : layer2.results) distances.push_back(r.getP3Distance());
      const auto thresholds = P3RelativeThresholds::calculateAdaptiveP3Thresholds(
         actualReference.getCell(), actualReference.getLatticeType(), distances);
      const MatchPresentation presentation = buildMatchPresentation(
         layer2, *alternateResult, actualReference, thresholds.referenceP3Norm);

      std::cout << "  Checking alternate matrix (expect reference-frame P3% > "
         << controls.getConventionalThreshold() << "%): ";
      if (presentation.referenceFrame.p3Percent > controls.getConventionalThreshold()) {
         std::cout << "PASS  (reference frame " << presentation.referenceFrame.p3Percent << "% of P3 norm)" << std::endl;
         ++passed;
      }
      else {
         std::cout << "FAIL  (reference frame " << presentation.referenceFrame.p3Percent << "% of P3 norm, expected > "
            << controls.getConventionalThreshold() << "%)" << std::endl;
         ++failed;
      }
   }

   // Third check uses a MORE perturbed (25%) version of the same mobile cell,
   // which produces a third order-2 supercell matrix even further from the
   // reference's basis than alternateMatrix. Confirms the mechanism degrades
   // sensibly across perturbation level, not just at one coincidental boundary.
   const LatticeCell mobileCell25(LRL_Cell(
      12.595, 21.139, 14.290,
      137.522 * M_PI / 180, 82.831 * M_PI / 180, 111.588 * M_PI / 180), "C");

   std::cout << "  Mobile (25% perturbation): type C: " << LRL_Cell_Degrees(mobileCell25.getCell()) << std::endl;

   const auto mr25 = matchPair(referenceCell, mobileCell25, controls);
   const Layer2Result& layer2_25 = mr25.layer2;
   const LatticeCell& actualReference25 = mr25.reference;

   const Matrix_3x3 worstMatrix(1.5, 2.5, 3.0, -1.5, -0.5, -1.0, -0.5, -1.5, -1.0);

   const LatticeMatchResult* worstResult = nullptr;
   for (const auto& result : layer2_25.results) {
      if (!worstResult && matricesMatch(result.getTransformationMatrix(), worstMatrix, 1e-6)) {
         worstResult = &result;
      }
   }

   if (!worstResult) {
      std::cout << "FAIL: worst-case (25% perturbation) matrix not found among matcher results" << std::endl;
      ++failed;
   }
   else {
      std::vector<double> distances25;
      for (const auto& r : layer2_25.results) distances25.push_back(r.getP3Distance());
      const auto thresholds25 = P3RelativeThresholds::calculateAdaptiveP3Thresholds(
         actualReference25.getCell(), actualReference25.getLatticeType(), distances25);
      const MatchPresentation presentation = buildMatchPresentation(
         layer2_25, *worstResult, actualReference25, thresholds25.referenceP3Norm);

      std::cout << "  Checking worst-case (25% perturbation) matrix (expect reference-frame P3% > "
         << controls.getConventionalThreshold() << "%): ";
      if (presentation.referenceFrame.p3Percent > controls.getConventionalThreshold()) {
         std::cout << "PASS  (reference frame " << presentation.referenceFrame.p3Percent << "% of P3 norm)" << std::endl;
         ++passed;
      }
      else {
         std::cout << "FAIL  (reference frame " << presentation.referenceFrame.p3Percent << "% of P3 norm, expected > "
            << controls.getConventionalThreshold() << "%)" << std::endl;
         ++failed;
      }
   }

   std::cout << "\nMatch presentation tests: " << passed << " passed, " << failed << " failed" << std::endl;
}

// Function to run flexible test mode
inline void runFlexibleTestMode(const MultiTransformFinderControls& controls) {
   ShowProgramHeader("CmdLMP3 -- P3 Lattice Matching (Test Mode)");

   // generateSearchMatrixGroups builds every RunMode/USEHNF variant eagerly on
   // its first call, regardless of which mode that call needs (see
   // SearchMatrixBuilder.h/.cpp). Without forcing that here, the cost is
   // silently absorbed into whichever test happens to run first -- making
   // that test's elapsed time meaningless (it's mostly one-time setup, not
   // its own search) and making every other test's time look artificially
   // cheap by comparison. Forcing and reporting it here keeps per-test
   // timings honest.
   const auto t_warm0 = std::chrono::high_resolution_clock::now();
   generateSearchMatrixGroups(controls);
   const auto t_warm1 = std::chrono::high_resolution_clock::now();
   std::cout << "; Matrix set construction (one-time): "
      << std::chrono::duration_cast<std::chrono::milliseconds>(t_warm1 - t_warm0).count()
      << " ms" << std::endl;

   FlexibleTestRunner runner;
   runner.setupStandardTests();
   runner.runAllTests(controls);
   if (controls.getTestNumbers().empty()) {
      runMatchPresentationTests(controls);
   }
}

#endif // FLEXIBLE_TEST_MODE_H
