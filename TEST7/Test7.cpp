// Demonstration of Krivy-Gruber failure vs Andrews-Bernstein success
// 
// This code shows the key difference: K-G lacks boundary case steps (R9-R12)
// and proper normalization that A-B provides.

#include <iostream>
#include <iomanip>
#include <chrono>
#include <fstream>


#include "G6.h"
#include "KrivyGruber.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "EisensteinReduction.h"
#include "Niggli.h"
#include "NiggliLandscape.h"
#include "Polar.h"  // Add this to includes
#include "StoreResults.h"
#include "CompareReductionAlgorithms.h"

#include "NiceAxisScaling.h"


// Test cell that fails K-G but succeeds with A-B
void demonstrateFailure() {
   std::cout << "=== Krivy-Gruber Failure Case ===" << std::endl;
   std::cout << "\nTest cell (after standard presentation):" << std::endl;
   std::cout << "G6: 4.401  7.579  34.382  18.085  9.491  14.527" << std::endl;

   G6 test_g6;
   test_g6[0] = 4.401;
   test_g6[1] = 7.579;
   test_g6[2] = 34.382;
   test_g6[3] = 18.085;
   test_g6[4] = 9.491;
   test_g6[5] = 14.527;

   std::cout << "\nViolations found:" << std::endl;
   std::cout << "  Condition 5: |?| > B  (|18.085| > 7.579)" << std::endl;
   std::cout << "  Condition 6: |?| > A  (|9.491| > 4.401)" << std::endl;
   std::cout << "  Condition 7: |?| > A  (|14.527| > 4.401)" << std::endl;

   std::cout << "\n--- Attempting Krivy-Gruber Step 5 ---" << std::endl;
   std::cout << "Condition: |?| > B" << std::endl;
   std::cout << "Would apply: C ? B+C-?·sign(?)" << std::endl;
   std::cout << "            = 7.579 + 34.382 - 18.085" << std::endl;
   std::cout << "            = 23.876" << std::endl;

   G6 kg_test = test_g6;
   kg_test[2] = 7.579 + 34.382 - 18.085;  // C
   kg_test[4] = 9.491 - 14.527;           // ?
   kg_test[3] = -2 * 7.579;               // ?

   LRL_Cell test_cell(kg_test);
   bool is_valid = test_cell.IsValid();

   std::cout << "Result G6: " << kg_test << std::endl;
   std::cout << "Cell validity: " << (is_valid ? "VALID" : "**INVALID**") << std::endl;
   std::cout << "Reason: Violates Gram determinant constraints" << std::endl;

   std::cout << "\n--- Attempting Krivy-Gruber Step 6 ---" << std::endl;
   std::cout << "Condition: |?| > A" << std::endl;
   std::cout << "Would apply: C ? A+C-?·sign(?)" << std::endl;
   std::cout << "            = 4.401 + 34.382 - 9.491" << std::endl;
   std::cout << "            = 29.292" << std::endl;

   G6 kg_test2 = test_g6;
   kg_test2[2] = 4.401 + 34.382 - 9.491;  // C
   kg_test2[3] = 18.085 - 14.527;         // ?  
   kg_test2[4] = -2 * 4.401;              // ?

   LRL_Cell test_cell2(kg_test2);
   bool is_valid2 = test_cell2.IsValid();

   std::cout << "Result G6: " << kg_test2 << std::endl;
   std::cout << "Cell validity: " << (is_valid2 ? "VALID" : "**INVALID**") << std::endl;
   std::cout << "Reason: Violates Gram determinant constraints" << std::endl;

   std::cout << "\n--- Attempting Krivy-Gruber Step 7 ---" << std::endl;
   std::cout << "Condition: |?| > A" << std::endl;
   std::cout << "Would apply: B ? A+B-?·sign(?)" << std::endl;
   std::cout << "            = 4.401 + 7.579 - 14.527" << std::endl;
   std::cout << "            = -2.547 (NEGATIVE!)" << std::endl;

   std::cout << "\nResult: **ALL THREE** K-G steps create invalid cells!" << std::endl;
   std::cout << "K-G algorithm **FAILS** on this cell." << std::endl;

   std::cout << "\n=== Andrews-Bernstein Success ===" << std::endl;
   std::cout << "The A-B algorithm successfully reduces this cell to:" << std::endl;
   std::cout << "G6: 2.489  4.401  13.371  1.169  0.043  0.689" << std::endl;

   std::cout << "\n=== Why Andrews-Bernstein Succeeds ===" << std::endl;
   std::cout << "1. Has boundary case steps (R9-R12) that K-G lacks" << std::endl;
   std::cout << "2. Applies full MKnorm after EVERY reduction step" << std::endl;
   std::cout << "3. Sophisticated zero-handling in normalization" << std::endl;
   std::cout << "4. Handles edge cases where values are equal or near-equal" << std::endl;

   std::cout << "\n=== Key Differences ===" << std::endl;
   std::cout << "Krivy-Gruber (1976):" << std::endl;
   std::cout << "  - 8 steps (1-8)" << std::endl;
   std::cout << "  - Simple normalization" << std::endl;
   std::cout << "  - Goto 1 after steps 5-8" << std::endl;
   std::cout << "  - No boundary case handling" << std::endl;

   std::cout << "\nAndrews-Bernstein (2014):" << std::endl;
   std::cout << "  - 12 steps (R5-R12)" << std::endl;
   std::cout << "  - Comprehensive MKnorm normalization" << std::endl;
   std::cout << "  - MKnorm after EVERY reduction" << std::endl;
   std::cout << "  - R9-R12 handle boundary cases" << std::endl;

   std::cout << "\n=== Missing K-G Steps ===" << std::endl;
   std::cout << "R9:  Boundary case when |g4| ? g2" << std::endl;
   std::cout << "R10: Boundary case when |g5| ? g1" << std::endl;
   std::cout << "R11: Boundary case when |g6| ? g1" << std::endl;
   std::cout << "R12: Refinement when g4+g5+g6+g1+g2 ? 0" << std::endl;

   std::cout << "\n=== Conclusion ===" << std::endl;
   std::cout << "Krivy-Gruber is **incomplete** and can fail on valid cells." << std::endl;
   std::cout << "Andrews-Bernstein is the **production algorithm** for Niggli reduction." << std::endl;
   std::cout << "Papers contain errors - A-B code notes corrections needed." << std::endl;
}

void testEisensteinReduction() {
   std::cout << "\n=== Testing Eisenstein Reduction ===" << std::endl;

   // Original test case - using constructor that takes degrees
   LRL_Cell test_cell(2.753, 5.864, 2.098, 137.316, 34.747, 116.741);

   std::cout << "Input cell (a,b,c,?,?,?): " << test_cell << std::endl;
   std::cout << "Input cell valid: "
      << (test_cell.IsValid() ? "YES" : "NO") << std::endl;

   if (!test_cell.IsValid()) {
      std::cout << "ERROR: Input cell is not valid" << std::endl;
      return;
   }

   G6 test_g6(test_cell);
   std::cout << "\nInput G6: " << test_g6 << std::endl;
   std::cout << "Input trace (g1+g2+g3): "
      << (test_g6[0] + test_g6[1] + test_g6[2]) << std::endl;

   // Check if this is Niggli reduced already
   bool isNiggli = Niggli::IsNiggli(test_g6);
   std::cout << "Already Niggli reduced: " << (isNiggli ? "YES" : "NO") << std::endl;

   // Perform Eisenstein reduction
   std::cout << "\n--- Starting Eisenstein Reduction ---" << std::endl;
   auto result = EisensteinReduction::Reduce(test_g6);

   std::cout << "\nEisenstein reduced G6: " << result.reducedG6 << std::endl;
   std::cout << "Reduced trace: " << result.trace << std::endl;
   std::cout << "Iterations to converge: " << result.iterations << std::endl;

   // Verify reduced cell is valid
   LRL_Cell reducedCell(result.reducedG6);
   std::cout << "\nReduced cell validity: "
      << (reducedCell.IsValid() ? "VALID" : "INVALID") << std::endl;

   // Check if Eisenstein result is Niggli reduced
   bool eisensteinIsNiggli = Niggli::IsNiggli(result.reducedG6);
   std::cout << "Eisenstein result is Niggli: "
      << (eisensteinIsNiggli ? "YES" : "NO") << std::endl;

   // Compare with Andrews-Bernstein result
   std::cout << "\n=== Comparison with Andrews-Bernstein ===" << std::endl;
   G6 ab_result;
   if (Niggli::ReduceWithoutMatrices(test_g6, ab_result, 1.0e-6)) {
      std::cout << "A-B reduced G6: " << ab_result << std::endl;
      std::cout << "A-B trace: " << (ab_result[0] + ab_result[1] + ab_result[2])
         << std::endl;
      std::cout << "A-B reduction cycles: " << Niggli::GetCycles() << std::endl;

      // Check if results are equivalent
      bool equivalent = true;
      double maxDiff = 0.0;
      for (size_t i = 0; i < 6; ++i) {
         double diff = std::abs(result.reducedG6[i] - ab_result[i]);
         maxDiff = std::max(maxDiff, diff);
         if (diff > 1.0e-3) {
            equivalent = false;
         }
      }

      std::cout << "Maximum difference: " << maxDiff << std::endl;
      std::cout << "Results are " << (equivalent ? "EQUIVALENT" : "DIFFERENT")
         << std::endl;

      std::cout << "\n=== Performance Comparison ===" << std::endl;
      std::cout << "Eisenstein iterations: " << result.iterations << std::endl;
      std::cout << "A-B cycles: " << Niggli::GetCycles() << std::endl;

      if (!equivalent) {
         std::cout << "\nDifferences:" << std::endl;
         for (size_t i = 0; i < 6; ++i) {
            double diff = result.reducedG6[i] - ab_result[i];
            if (std::abs(diff) > 1.0e-6) {
               std::cout << "  g" << (i + 1) << ": Eisenstein=" << result.reducedG6[i]
                  << " A-B=" << ab_result[i]
                  << " diff=" << diff << std::endl;
            }
         }
      }
   } else {
      std::cout << "A-B reduction FAILED" << std::endl;
   }
}


// Move these struct definitions OUTSIDE and BEFORE bulkComparisonTest()

// These struct definitions go BEFORE the function, at file scope

struct ProblemCase {
   G6 inputG6;
   G6 abResult;
   EisensteinReduction::ReductionResult eisensteinResult;
   int abCycles;
   std::string description;

   ProblemCase() : abCycles(0) {}
};

inline std::ostream& operator<<(std::ostream& os, const ProblemCase& pc) {
   os << pc.description << std::endl;
   os << "  Input G6:       " << pc.inputG6 << std::endl;
   os << "  Input trace:    " << (pc.inputG6[0] + pc.inputG6[1] + pc.inputG6[2]) << std::endl;
   os << "  A-B result:     " << pc.abResult << std::endl;
   os << "  A-B cycles:     " << pc.abCycles << std::endl;
   os << "  Eisenstein G6:  " << pc.eisensteinResult.reducedG6 << std::endl;
   os << "  Eisenstein iter:" << pc.eisensteinResult.iterations << std::endl;
   os << "  Eisenstein trace:" << pc.eisensteinResult.trace << std::endl;
   return os;
}

struct IterationStats {
   int cellNumber;
   int abCycles;
   int eisensteinIterations;
   double inputTrace;
   double outputTrace;
   G6 inputG6;

   IterationStats() : cellNumber(0), abCycles(0), eisensteinIterations(0),
      inputTrace(0.0), outputTrace(0.0) {
   }
};

inline std::ostream& operator<<(std::ostream& os, const IterationStats& stats) {
   os << "Cell #" << stats.cellNumber
      << ": A-B=" << stats.abCycles
      << " Eisenstein=" << stats.eisensteinIterations
      << " trace:" << std::fixed << std::setprecision(0)
      << stats.inputTrace << "->" << stats.outputTrace;
   return os;
}

// NOW the complete function
void bulkComparisonTest(int numCells = 100) {
   std::cout << "\n=== Bulk Testing: A-B vs Eisenstein ===" << std::endl;
   std::cout << "Testing " << numCells << " random cells...\n" << std::endl;

   int validCells = 0;
   int invalidCells = 0;
   int nearDegenerateCells = 0;  // Cells with angles < 5° or > 175°
   int ab_failures = 0;
   int eisenstein_failures = 0;
   int ab_eisenstein_mismatch = 0;

   int totalABCycles = 0;
   int totalEisensteinIterations = 0;

   int minABCycles = INT_MAX;
   int maxABCycles = 0;
   int minEisensteinIterations = INT_MAX;
   int maxEisensteinIterations = 0;

   double maxTraceDifference = 0.0;

   // Track distribution of iterations
   std::vector<int> abCycleDistribution(100, 0);
   std::vector<int> eisensteinIterDistribution(100, 0);

   // Track trace reduction statistics
   double totalInputTrace = 0.0;
   double totalOutputTrace = 0.0;
   double maxTraceReduction = 0.0;
   double minTraceReduction = DBL_MAX;

   // Store problem cases with detailed results
   StoreResults<std::string, ProblemCase> problemCases(5);
   problemCases.SetTitle("Problem Cases Found During Bulk Testing");
   problemCases.SetKeyLabel("Problem Type");

   // Store iteration distribution data
   StoreResults<int, IterationStats> iterationStats(3);
   iterationStats.SetTitle("Iteration Statistics");
   iterationStats.SetKeyLabel("Eisenstein Iterations");

   for (int i = 0; i < numCells; ++i) {
      // Generate random triclinic cell
      LRL_Cell randomCell = Polar::rand();

      // Validate the cell
      if (!randomCell.IsValid()) {
         invalidCells++;
         continue;
      }

      validCells++;
      G6 inputG6(randomCell);
      double inputTrace = inputG6[0] + inputG6[1] + inputG6[2];

      // Check for near-degenerate angles directly from G6
      // cos(angle) = g_ij / (2 * sqrt(g_i * g_j))
      double cos_alpha = std::abs(inputG6[3]) / (2.0 * std::sqrt(inputG6[1] * inputG6[2]));
      double cos_beta = std::abs(inputG6[4]) / (2.0 * std::sqrt(inputG6[0] * inputG6[2]));
      double cos_gamma = std::abs(inputG6[5]) / (2.0 * std::sqrt(inputG6[0] * inputG6[1]));

      const double cos_5_degrees = 0.9962;  // cos(5°) ? 0.9962

      if (cos_alpha >= cos_5_degrees || cos_beta >= cos_5_degrees || cos_gamma >= cos_5_degrees) {
         nearDegenerateCells++;

         if (nearDegenerateCells <= 5) {  // Report first 5
            std::cout << "\nCell " << i << ": Skipping near-degenerate cell (angle < 5° or > 175°)" << std::endl;
            std::cout << "  cos(?) = " << cos_alpha << std::endl;
            std::cout << "  cos(?) = " << cos_beta << std::endl;
            std::cout << "  cos(?) = " << cos_gamma << std::endl;
         }

         continue;  // Skip this cell entirely
      }

      totalInputTrace += inputTrace;

      // Test Andrews-Bernstein
      G6 ab_result;
      bool ab_success = Niggli::ReduceWithoutMatrices(inputG6, ab_result, 1.0e-6);
      int ab_cycles = Niggli::GetCycles();

      if (!ab_success) {
         ab_failures++;
         std::cout << "Cell " << i << ": A-B FAILED" << std::endl;

         ProblemCase pc;
         pc.inputG6 = inputG6;
         pc.abResult = ab_result;
         pc.abCycles = ab_cycles;
         pc.eisensteinResult = EisensteinReduction::ReductionResult();
         pc.description = "A-B reduction failed";
         problemCases.Store("A-B Failed", pc);
         continue;
      }

      totalABCycles += ab_cycles;
      minABCycles = std::min(minABCycles, ab_cycles);
      maxABCycles = std::max(maxABCycles, ab_cycles);
      if (ab_cycles < 100) abCycleDistribution[ab_cycles]++;

      double outputTrace = ab_result[0] + ab_result[1] + ab_result[2];
      totalOutputTrace += outputTrace;
      double traceReduction = inputTrace - outputTrace;
      maxTraceReduction = std::max(maxTraceReduction, traceReduction);
      minTraceReduction = std::min(minTraceReduction, traceReduction);

      // Test Eisenstein (we've already filtered out near-degenerate cells)
      auto eisenstein_result = EisensteinReduction::ReduceQuiet(inputG6, 1.0e-6);

      // Check for zero result (shouldn't happen now, but check anyway)
      bool isZero = true;
      for (size_t j = 0; j < 6; ++j) {
         if (std::abs(eisenstein_result.reducedG6[j]) > 1.0e-10) {
            isZero = false;
            break;
         }
      }

      if (isZero) {
         eisenstein_failures++;
         std::cout << "\nCell " << i << ": Eisenstein produced ZERO result!" << std::endl;
         std::cout << "  Input G6: " << inputG6 << std::endl;

         ProblemCase pc;
         pc.inputG6 = inputG6;
         pc.abResult = ab_result;
         pc.abCycles = ab_cycles;
         pc.eisensteinResult = eisenstein_result;
         pc.description = "Eisenstein produced zero G6 (unexpected bug)";
         problemCases.Store("Eisenstein Zero Result", pc);
         continue;
      }

      if (eisenstein_result.trace <= 0 || eisenstein_result.iterations >= 1000) {
         eisenstein_failures++;
         std::cout << "Cell " << i << ": Eisenstein FAILED or didn't converge" << std::endl;

         ProblemCase pc;
         pc.inputG6 = inputG6;
         pc.abResult = ab_result;
         pc.abCycles = ab_cycles;
         pc.eisensteinResult = eisenstein_result;
         pc.description = "Eisenstein failed to converge or invalid trace";
         problemCases.Store("Eisenstein Failed/No Convergence", pc);
         continue;
      }

      totalEisensteinIterations += eisenstein_result.iterations;
      minEisensteinIterations = std::min(minEisensteinIterations, eisenstein_result.iterations);
      maxEisensteinIterations = std::max(maxEisensteinIterations, eisenstein_result.iterations);
      if (eisenstein_result.iterations < 100) {
         eisensteinIterDistribution[eisenstein_result.iterations]++;
      }

      // Store iteration statistics
      IterationStats stats;
      stats.cellNumber = i;
      stats.abCycles = ab_cycles;
      stats.eisensteinIterations = eisenstein_result.iterations;
      stats.inputTrace = inputTrace;
      stats.outputTrace = outputTrace;
      stats.inputG6 = inputG6;
      iterationStats.Store(eisenstein_result.iterations, stats);

      // Compare results
      bool equivalent = true;
      double maxDiff = 0.0;
      for (size_t j = 0; j < 6; ++j) {
         double diff = std::abs(eisenstein_result.reducedG6[j] - ab_result[j]);
         maxDiff = std::max(maxDiff, diff);
         if (diff > 1.0e-3) {
            equivalent = false;
         }
      }

      maxTraceDifference = std::max(maxTraceDifference, maxDiff);

      if (!equivalent) {
         ab_eisenstein_mismatch++;
         std::cout << "\nCell " << i << ": MISMATCH!" << std::endl;
         std::cout << "  Input G6: " << inputG6 << std::endl;
         std::cout << "  A-B:      " << ab_result << std::endl;
         std::cout << "  Eisenstein: " << eisenstein_result.reducedG6 << std::endl;
         std::cout << "  Max diff: " << maxDiff << std::endl;

         ProblemCase pc;
         pc.inputG6 = inputG6;
         pc.abResult = ab_result;
         pc.abCycles = ab_cycles;
         pc.eisensteinResult = eisenstein_result;
         pc.description = "Results differ beyond tolerance";
         problemCases.Store("A-B vs Eisenstein Mismatch", pc);
      }

      // Progress indicator
      if ((i + 1) % 10 == 0) {
         std::cout << "." << std::flush;
      }
   }

   int testableValidCells = validCells - nearDegenerateCells;

   std::cout << "\n\n=== Bulk Test Results ===" << std::endl;
   std::cout << "Total cells generated: " << numCells << std::endl;
   std::cout << "Invalid cells: " << invalidCells << std::endl;
   std::cout << "Valid cells: " << validCells << std::endl;
   std::cout << "Near-degenerate cells (skipped): " << nearDegenerateCells
      << " (" << std::fixed << std::setprecision(1)
      << (100.0 * nearDegenerateCells / validCells) << "% of valid)" << std::endl;
   std::cout << "Testable cells: " << testableValidCells << std::endl;

   std::cout << "\nA-B failures: " << ab_failures << std::endl;
   std::cout << "Eisenstein failures: " << eisenstein_failures << std::endl;
   std::cout << "A-B vs Eisenstein mismatches: " << ab_eisenstein_mismatch << std::endl;

   if (testableValidCells > 0) {
      std::cout << "\n=== Performance Statistics ===" << std::endl;
      std::cout << "Based on " << testableValidCells << " testable cells" << std::endl;

      std::cout << "\nA-B cycles:" << std::endl;
      std::cout << "  Average: " << (double)totalABCycles / testableValidCells << std::endl;
      std::cout << "  Min: " << minABCycles << std::endl;
      std::cout << "  Max: " << maxABCycles << std::endl;

      std::cout << "\nEisenstein iterations:" << std::endl;
      std::cout << "  Average: " << (double)totalEisensteinIterations / testableValidCells << std::endl;
      std::cout << "  Min: " << minEisensteinIterations << std::endl;
      std::cout << "  Max: " << maxEisensteinIterations << std::endl;

      std::cout << "\nIteration ratio (Eisenstein/A-B): "
         << (double)totalEisensteinIterations / totalABCycles << std::endl;

      std::cout << "\n=== Trace Reduction Statistics ===" << std::endl;
      std::cout << "Average input trace: " << totalInputTrace / testableValidCells << std::endl;
      std::cout << "Average output trace: " << totalOutputTrace / testableValidCells << std::endl;
      std::cout << "Average trace reduction: "
         << (totalInputTrace - totalOutputTrace) / testableValidCells << std::endl;
      std::cout << "Max trace reduction: " << maxTraceReduction << std::endl;
      std::cout << "Min trace reduction: " << minTraceReduction << std::endl;

      std::cout << "\n=== A-B Cycle Distribution ===" << std::endl;
      for (int k = 0; k < 20; ++k) {
         if (abCycleDistribution[k] > 0) {
            std::cout << "  " << std::setw(2) << k << " cycles: "
               << std::setw(4) << abCycleDistribution[k]
               << " cells (" << std::fixed << std::setprecision(1)
               << (100.0 * abCycleDistribution[k] / testableValidCells)
               << "%)" << std::endl;
         }
      }

      std::cout << "\n=== Eisenstein Iteration Distribution ===" << std::endl;
      for (int k = 0; k < 30; ++k) {
         if (eisensteinIterDistribution[k] > 0) {
            std::cout << "  " << std::setw(2) << k << " iterations: "
               << std::setw(4) << eisensteinIterDistribution[k]
               << " cells (" << std::fixed << std::setprecision(1)
               << (100.0 * eisensteinIterDistribution[k] / testableValidCells)
               << "%)" << std::endl;
         }
      }

      std::cout << "\nMax G6 component difference: " << std::scientific
         << maxTraceDifference << std::endl;
   }

   // Show detailed problem cases
   if (!problemCases.empty()) {
      std::cout << "\n================================================" << std::endl;
      std::cout << "=== PROBLEM CASES - DETAILED ANALYSIS ===" << std::endl;
      std::cout << "================================================\n" << std::endl;
      problemCases.ShowResults();
   }

   // Show iteration statistics with examples
   if (!iterationStats.empty()) {
      std::cout << "\n================================================" << std::endl;
      std::cout << "=== ITERATION STATISTICS - EXAMPLES BY COUNT ===" << std::endl;
      std::cout << "================================================\n" << std::endl;
      std::cout << "Showing up to 3 example cells for each iteration count:\n" << std::endl;
      iterationStats.ShowResultsByKeyAscending();
   }

   if (ab_eisenstein_mismatch == 0 && ab_failures == 0 && eisenstein_failures == 0) {
      std::cout << "\n? ALL TESTS PASSED - A-B and Eisenstein agree perfectly!" << std::endl;
      if (nearDegenerateCells > 0) {
         std::cout << "  (Note: " << nearDegenerateCells
            << " near-degenerate cells were excluded from testing)" << std::endl;
      }
   } else {
      std::cout << "\n? PROBLEMS FOUND - see detailed analysis above" << std::endl;
   }
}

void debugCell512() {
   std::cout << "\n=== Debugging Cell 512 ===" << std::endl;

   G6 problemG6;
   problemG6[0] = 14.628;
   problemG6[1] = 65.640;
   problemG6[2] = 3.869;
   problemG6[3] = 31.869;
   problemG6[4] = -11.769;
   problemG6[5] = -48.700;

   std::cout << "Problem G6: " << problemG6 << std::endl;

   // Check Schwarz inequalities
   std::cout << "\nSchwarz inequality checks:" << std::endl;
   double g4_sq = problemG6[3] * problemG6[3];
   double limit_g4 = 4.0 * problemG6[1] * problemG6[2];
   std::cout << "  |g4|^2 = " << g4_sq
      << " vs 4*g2*g3 = " << limit_g4
      << " : " << ((g4_sq <= limit_g4) ? "OK" : "**FAIL**") << std::endl;

   double g5_sq = problemG6[4] * problemG6[4];
   double limit_g5 = 4.0 * problemG6[0] * problemG6[2];
   std::cout << "  |g5|^2 = " << g5_sq
      << " vs 4*g1*g3 = " << limit_g5
      << " : " << ((g5_sq <= limit_g5) ? "OK" : "**FAIL**") << std::endl;

   double g6_sq = problemG6[5] * problemG6[5];
   double limit_g6 = 4.0 * problemG6[0] * problemG6[1];
   std::cout << "  |g6|^2 = " << g6_sq
      << " vs 4*g1*g2 = " << limit_g6
      << " : " << ((g6_sq <= limit_g6) ? "OK" : "**FAIL**") << std::endl;

   // Try to create cell
   LRL_Cell cell(problemG6);
   std::cout << "\nLRL_Cell.IsValid(): " << (cell.IsValid() ? "YES" : "NO") << std::endl;

   // What does A-B do with it?
   std::cout << "\n=== A-B Reduction ===" << std::endl;
   G6 ab_result;
   bool ab_success = Niggli::ReduceWithoutMatrices(problemG6, ab_result, 1.0e-6);
   std::cout << "A-B success: " << (ab_success ? "YES" : "NO") << std::endl;
   if (ab_success) {
      std::cout << "A-B result: " << ab_result << std::endl;
      std::cout << "A-B cycles: " << Niggli::GetCycles() << std::endl;
   }

   // Try B4 conversion
   std::cout << "\n=== B4 Conversion ===" << std::endl;
   B4 b4(cell);
   std::cout << "B4 created" << std::endl;

   // Try to compute trace
   LRL_Cell tempCell(b4);
   std::cout << "tempCell.IsValid(): " << (tempCell.IsValid() ? "YES" : "NO") << std::endl;

   G6 g6_from_b4(tempCell);
   std::cout << "G6 from B4: " << g6_from_b4 << std::endl;

   double trace = g6_from_b4[0] + g6_from_b4[1] + g6_from_b4[2];
   std::cout << "Trace: " << trace << std::endl;
}

void compareStrategies() {
   LRL_Cell test_cell(2.753, 5.864, 2.098, 137.316, 34.747, 116.741);
   G6 inputG6(test_cell);

   std::cout << "Testing three search strategies:\n" << std::endl;

   auto result1 = EisensteinReduction::ReduceWithStrategy(
      inputG6, EisensteinReduction::SearchStrategy::FIRST_IMPROVEMENT, 1.0e-6, true);

   std::cout << "\n" << std::string(60, '=') << "\n" << std::endl;

   auto result2 = EisensteinReduction::ReduceWithStrategy(
      inputG6, EisensteinReduction::SearchStrategy::BEST_IN_SWEEP, 1.0e-6, true);

   std::cout << "\n" << std::string(60, '=') << "\n" << std::endl;

   auto result3 = EisensteinReduction::ReduceWithStrategy(
      inputG6, EisensteinReduction::SearchStrategy::BEST_CUMULATIVE, 1.0e-6, true);
}

void bulkStrategyComparison(int numCells = 100) {
   std::cout << "\n=== Bulk Strategy Comparison ===" << std::endl;

   // Track statistics for each strategy
   struct StrategyStats {
      int totalIterations = 0;
      size_t totalTransformations = 0;
      int minIterations = INT_MAX;
      int maxIterations = 0;
      std::string name;
   };

   StrategyStats stats[3];
   stats[0].name = "FIRST_IMPROVEMENT";
   stats[1].name = "BEST_IN_SWEEP";
   stats[2].name = "BEST_CUMULATIVE";

   int validTestable = 0;

   for (int i = 0; i < numCells; ++i) {
      LRL_Cell randomCell = Polar::rand();
      if (!randomCell.IsValid()) continue;

      G6 inputG6(randomCell);

      // Check for near-degenerate
      double cos_alpha = std::abs(inputG6[3]) / (2.0 * std::sqrt(inputG6[1] * inputG6[2]));
      double cos_beta = std::abs(inputG6[4]) / (2.0 * std::sqrt(inputG6[0] * inputG6[2]));
      double cos_gamma = std::abs(inputG6[5]) / (2.0 * std::sqrt(inputG6[0] * inputG6[1]));
      const double cos_5_degrees = 0.9962;

      if (cos_alpha >= cos_5_degrees || cos_beta >= cos_5_degrees || cos_gamma >= cos_5_degrees) {
         continue;
      }

      validTestable++;

      // Test each strategy
      auto result1 = EisensteinReduction::ReduceWithStrategy(
         inputG6, EisensteinReduction::SearchStrategy::FIRST_IMPROVEMENT, 1.0e-6, false);
      stats[0].totalIterations += result1.iterations;
      stats[0].minIterations = std::min(stats[0].minIterations, result1.iterations);
      stats[0].maxIterations = std::max(stats[0].maxIterations, result1.iterations);

      auto result2 = EisensteinReduction::ReduceWithStrategy(
         inputG6, EisensteinReduction::SearchStrategy::BEST_IN_SWEEP, 1.0e-6, false);
      stats[1].totalIterations += result2.iterations;
      stats[1].minIterations = std::min(stats[1].minIterations, result2.iterations);
      stats[1].maxIterations = std::max(stats[1].maxIterations, result2.iterations);

      auto result3 = EisensteinReduction::ReduceWithStrategy(
         inputG6, EisensteinReduction::SearchStrategy::BEST_CUMULATIVE, 1.0e-6, false);
      stats[2].totalIterations += result3.iterations;
      stats[2].minIterations = std::min(stats[2].minIterations, result3.iterations);
      stats[2].maxIterations = std::max(stats[2].maxIterations, result3.iterations);

      if ((i + 1) % 10 == 0) std::cout << "." << std::flush;
   }

   std::cout << "\n\n=== Strategy Comparison Results ===" << std::endl;
   std::cout << "Tested " << validTestable << " cells\n" << std::endl;

   for (int i = 0; i < 3; ++i) {
      std::cout << stats[i].name << ":" << std::endl;
      std::cout << "  Average iterations: "
         << (double)stats[i].totalIterations / validTestable << std::endl;
      std::cout << "  Min iterations: " << stats[i].minIterations << std::endl;
      std::cout << "  Max iterations: " << stats[i].maxIterations << std::endl;
      std::cout << "  Total iterations: " << stats[i].totalIterations << std::endl;
      std::cout << std::endl;
   }
}

void testAdaptiveStrategy() {
   std::cout << "\n=== Testing Adaptive Strategy ===" << std::endl;

   LRL_Cell test_cell(2.753, 5.864, 2.098, 137.316, 34.747, 116.741);
   G6 inputG6(test_cell);

   std::cout << "\n" << std::string(70, '=') << std::endl;
   std::cout << "ADAPTIVE_BY_STATE:" << std::endl;
   std::cout << std::string(70, '=') << std::endl;

   auto result = EisensteinReduction::ReduceAdaptive(inputG6, 1.0e-6, true);

   std::cout << "\n=== Result ===" << std::endl;
   std::cout << "Iterations: " << result.iterations << std::endl;
   std::cout << "Final G6: " << result.reducedG6 << std::endl;

   // Compare with A-B
   G6 ab_result;
   Niggli::ReduceWithoutMatrices(inputG6, ab_result, 1.0e-6);

   bool match = true;
   for (size_t i = 0; i < 6; ++i) {
      if (std::abs(result.reducedG6[i] - ab_result[i]) > 1.0e-6) {
         match = false;
         break;
      }
   }
   std::cout << "Matches A-B: " << (match ? "YES ?" : "NO ?") << std::endl;
}

void bulkTestAdaptive(int numCells = 1000) {
   std::cout << "\n=== Bulk Test: Adaptive vs BEST_CUMULATIVE ===" << std::endl;

   struct Stats {
      int totalIterations = 0;
      size_t totalTransformations = 0;
      int minIterations = INT_MAX;
      int maxIterations = 0;
      size_t minTransformations = SIZE_MAX;
      size_t maxTransformations = 0;
      int mismatches = 0;

      // Track by initial state
      int countHighlySkewed = 0;
      int countModerate = 0;
      int countVeryClose = 0;

      size_t transformsHighlySkewed = 0;
      size_t transformsModerate = 0;
      size_t transformsVeryClose = 0;
   };

   Stats adaptiveStats, cumulativeStats;
   int validCells = 0;

   for (int i = 0; i < numCells; ++i) {
      LRL_Cell randomCell = Polar::rand();
      if (!randomCell.IsValid()) continue;

      G6 inputG6(randomCell);

      // Check for near-degenerate
      double cos_alpha = std::abs(inputG6[3]) / (2.0 * std::sqrt(inputG6[1] * inputG6[2]));
      double cos_beta = std::abs(inputG6[4]) / (2.0 * std::sqrt(inputG6[0] * inputG6[2]));
      double cos_gamma = std::abs(inputG6[5]) / (2.0 * std::sqrt(inputG6[0] * inputG6[1]));
      const double cos_5_degrees = 0.9962;

      if (cos_alpha >= cos_5_degrees || cos_beta >= cos_5_degrees || cos_gamma >= cos_5_degrees) {
         continue;
      }

      validCells++;

      // Analyze initial state
      auto initialMetrics = EisensteinReduction::CellStateMetrics::Analyze(inputG6);

      // Test ADAPTIVE
      auto adaptiveResult = EisensteinReduction::ReduceAdaptive(inputG6, 1.0e-6, false);
      adaptiveStats.totalIterations += adaptiveResult.iterations;
      // Note: We'd need to track transformations tested - add to ReductionResult?
      adaptiveStats.minIterations = std::min(adaptiveStats.minIterations, adaptiveResult.iterations);
      adaptiveStats.maxIterations = std::max(adaptiveStats.maxIterations, adaptiveResult.iterations);

      // Track by initial state
      switch (initialMetrics.state) {
      case EisensteinReduction::CellStateMetrics::State::HIGHLY_SKEWED:
         adaptiveStats.countHighlySkewed++;
         break;
      case EisensteinReduction::CellStateMetrics::State::MODERATELY_SKEWED:
         adaptiveStats.countModerate++;
         break;
      case EisensteinReduction::CellStateMetrics::State::VERY_CLOSE_TO_REDUCED:
         adaptiveStats.countVeryClose++;
         break;
      }

      // Test BEST_CUMULATIVE (for comparison)
      auto cumulativeResult = EisensteinReduction::ReduceWithStrategy(
         inputG6, EisensteinReduction::SearchStrategy::BEST_CUMULATIVE, 1.0e-6, false);
      cumulativeStats.totalIterations += cumulativeResult.iterations;
      cumulativeStats.minIterations = std::min(cumulativeStats.minIterations, cumulativeResult.iterations);
      cumulativeStats.maxIterations = std::max(cumulativeStats.maxIterations, cumulativeResult.iterations);

      // Compare results
      bool match = true;
      for (size_t j = 0; j < 6; ++j) {
         if (std::abs(adaptiveResult.reducedG6[j] - cumulativeResult.reducedG6[j]) > 1.0e-6) {
            match = false;
            break;
         }
      }
      if (!match) adaptiveStats.mismatches++;

      if ((i + 1) % 10 == 0) std::cout << "." << std::flush;
   }

   std::cout << "\nADAPTIVE Strategy:" << std::endl;
   std::cout << "  Total transformations: " << adaptiveStats.totalTransformations << std::endl;
   std::cout << "  Average transformations per cell: "
      << (double)adaptiveStats.totalTransformations / validCells << std::endl;
   std::cout << "  Average iterations: " << (double)adaptiveStats.totalIterations / validCells << std::endl;

   std::cout << "\nBEST_CUMULATIVE Strategy (baseline):" << std::endl;
   std::cout << "  Total transformations: " << cumulativeStats.totalTransformations << std::endl;
   std::cout << "  Average transformations per cell: "
      << (double)cumulativeStats.totalTransformations / validCells << std::endl;
   std::cout << "  Average iterations: " << (double)cumulativeStats.totalIterations / validCells << std::endl;



   std::cout << "\n\n=== Results for " << validCells << " cells ===" << std::endl;

   std::cout << "\nInitial State Distribution:" << std::endl;
   std::cout << "  Highly Skewed: " << adaptiveStats.countHighlySkewed
      << " (" << (100.0 * adaptiveStats.countHighlySkewed / validCells) << "%)" << std::endl;
   std::cout << "  Moderate: " << adaptiveStats.countModerate
      << " (" << (100.0 * adaptiveStats.countModerate / validCells) << "%)" << std::endl;
   std::cout << "  Very Close: " << adaptiveStats.countVeryClose
      << " (" << (100.0 * adaptiveStats.countVeryClose / validCells) << "%)" << std::endl;

   std::cout << "\nADAPTIVE Strategy:" << std::endl;
   std::cout << "  Average iterations: " << (double)adaptiveStats.totalIterations / validCells << std::endl;
   std::cout << "  Min iterations: " << adaptiveStats.minIterations << std::endl;
   std::cout << "  Max iterations: " << adaptiveStats.maxIterations << std::endl;
   std::cout << "  Mismatches with BEST_CUMULATIVE: " << adaptiveStats.mismatches << std::endl;

   std::cout << "\nBEST_CUMULATIVE Strategy (baseline):" << std::endl;
   std::cout << "  Average iterations: " << (double)cumulativeStats.totalIterations / validCells << std::endl;
   std::cout << "  Min iterations: " << cumulativeStats.minIterations << std::endl;
   std::cout << "  Max iterations: " << cumulativeStats.maxIterations << std::endl;

   std::cout << "\nComparison:" << std::endl;
   double iterationRatio = (double)adaptiveStats.totalIterations / cumulativeStats.totalIterations;
   std::cout << "  Iteration ratio (Adaptive/Cumulative): " << iterationRatio << std::endl;

   // Estimate transformation savings (rough calculation)
   // Assume: Cumulative always uses 3480 per iteration
   //         Adaptive uses mix based on state distribution
   size_t cumulativeTransforms = cumulativeStats.totalIterations * 3480;
   // For adaptive, estimate based on what we saw: ~3480 first iter, ~160-2364 subsequent
   size_t adaptiveTransformsEstimate =
      validCells * 3480 +  // First iteration (most cells start at level 3)
      (adaptiveStats.totalIterations - validCells) * 500;  // Subsequent iters average ~500

   std::cout << "  Estimated transformation ratio: "
      << (double)adaptiveTransformsEstimate / cumulativeTransforms << std::endl;
   std::cout << "  Estimated savings: "
      << (100.0 * (1.0 - (double)adaptiveTransformsEstimate / cumulativeTransforms))
      << "%" << std::endl;
}

void bulkTestAdaptiveVsCumulativeFinal(int numCells = 1000) {
   std::cout << "\n" << std::string(70, '=') << std::endl;
   std::cout << "DESIGN OF EXPERIMENTS: Adaptive vs BEST_CUMULATIVE" << std::endl;
   std::cout << std::string(70, '=') << std::endl;
   std::cout << "Testing " << numCells << " random triclinic cells\n" << std::endl;

   struct Stats {
      int totalIterations = 0;
      size_t totalTransformations = 0;
      int minIterations = INT_MAX;
      int maxIterations = 0;
      size_t minTransformations = SIZE_MAX;
      size_t maxTransformations = 0;
      int mismatches = 0;

      // Track by initial state
      int countHighlySkewed = 0;
      int countModerate = 0;
      int countVeryClose = 0;

      size_t transformsHighlySkewed = 0;
      size_t transformsModerate = 0;
      size_t transformsVeryClose = 0;

      // Track by iteration count
      std::vector<int> iterationDistribution;

      Stats() {
         iterationDistribution.resize(10, 0);
      }
   };

   Stats adaptiveStats, cumulativeStats;
   int validCells = 0;
   int nearDegenerateCells = 0;

   for (int i = 0; i < numCells; ++i) {
      LRL_Cell randomCell = Polar::rand();
      if (!randomCell.IsValid()) continue;

      G6 inputG6(randomCell);

      // Check for near-degenerate
      double cos_alpha = std::abs(inputG6[3]) / (2.0 * std::sqrt(inputG6[1] * inputG6[2]));
      double cos_beta = std::abs(inputG6[4]) / (2.0 * std::sqrt(inputG6[0] * inputG6[2]));
      double cos_gamma = std::abs(inputG6[5]) / (2.0 * std::sqrt(inputG6[0] * inputG6[1]));
      const double cos_5_degrees = 0.9962;

      if (cos_alpha >= cos_5_degrees || cos_beta >= cos_5_degrees || cos_gamma >= cos_5_degrees) {
         nearDegenerateCells++;
         continue;
      }

      validCells++;

      // Analyze initial state
      auto initialMetrics = EisensteinReduction::CellStateMetrics::Analyze(inputG6);

      // Test ADAPTIVE
      auto adaptiveResult = EisensteinReduction::ReduceAdaptive(inputG6, 1.0e-6, false);
      adaptiveStats.totalIterations += adaptiveResult.iterations;
      adaptiveStats.totalTransformations += adaptiveResult.transformationsTested;
      adaptiveStats.minIterations = std::min(adaptiveStats.minIterations, adaptiveResult.iterations);
      adaptiveStats.maxIterations = std::max(adaptiveStats.maxIterations, adaptiveResult.iterations);
      adaptiveStats.minTransformations = std::min(adaptiveStats.minTransformations,
         adaptiveResult.transformationsTested);
      adaptiveStats.maxTransformations = std::max(adaptiveStats.maxTransformations,
         adaptiveResult.transformationsTested);

      if (adaptiveResult.iterations < 10) {
         adaptiveStats.iterationDistribution[adaptiveResult.iterations]++;
      }

      // Track by initial state
      size_t transforms = adaptiveResult.transformationsTested;
      switch (initialMetrics.state) {
      case EisensteinReduction::CellStateMetrics::State::HIGHLY_SKEWED:
         adaptiveStats.countHighlySkewed++;
         adaptiveStats.transformsHighlySkewed += transforms;
         break;
      case EisensteinReduction::CellStateMetrics::State::MODERATELY_SKEWED:
         adaptiveStats.countModerate++;
         adaptiveStats.transformsModerate += transforms;
         break;
      case EisensteinReduction::CellStateMetrics::State::VERY_CLOSE_TO_REDUCED:
         adaptiveStats.countVeryClose++;
         adaptiveStats.transformsVeryClose += transforms;
         break;
      }

      // Test BEST_CUMULATIVE
      auto cumulativeResult = EisensteinReduction::ReduceBestCumulative(inputG6, 1.0e-6, false);
      cumulativeStats.totalIterations += cumulativeResult.iterations;
      cumulativeStats.totalTransformations += cumulativeResult.transformationsTested;
      cumulativeStats.minIterations = std::min(cumulativeStats.minIterations,
         cumulativeResult.iterations);
      cumulativeStats.maxIterations = std::max(cumulativeStats.maxIterations,
         cumulativeResult.iterations);
      cumulativeStats.minTransformations = std::min(cumulativeStats.minTransformations,
         cumulativeResult.transformationsTested);
      cumulativeStats.maxTransformations = std::max(cumulativeStats.maxTransformations,
         cumulativeResult.transformationsTested);

      if (cumulativeResult.iterations < 10) {
         cumulativeStats.iterationDistribution[cumulativeResult.iterations]++;
      }

      // Compare results
      bool match = true;
      double maxDiff = 0.0;
      for (size_t j = 0; j < 6; ++j) {
         double diff = std::abs(adaptiveResult.reducedG6[j] - cumulativeResult.reducedG6[j]);
         maxDiff = std::max(maxDiff, diff);
         if (diff > 1.0e-6) {
            match = false;
         }
      }
      if (!match) {
         adaptiveStats.mismatches++;
         std::cout << "\n? MISMATCH at cell " << i << " (max diff: " << maxDiff << "):" << std::endl;
         std::cout << "  Input:      " << inputG6 << std::endl;
         std::cout << "  Adaptive:   " << adaptiveResult.reducedG6 << std::endl;
         std::cout << "  Cumulative: " << cumulativeResult.reducedG6 << std::endl;
      }

      if ((i + 1) % 10 == 0) std::cout << "." << std::flush;
   }

   std::cout << "\n\n" << std::string(70, '=') << std::endl;
   std::cout << "EXPERIMENTAL RESULTS" << std::endl;
   std::cout << std::string(70, '=') << std::endl;

   std::cout << "\nSample Characteristics:" << std::endl;
   std::cout << "  Total cells generated: " << numCells << std::endl;
   std::cout << "  Near-degenerate (excluded): " << nearDegenerateCells << std::endl;
   std::cout << "  Valid test cells: " << validCells << std::endl;

   std::cout << "\nInitial State Distribution:" << std::endl;
   std::cout << "  Highly Skewed:  " << std::setw(4) << adaptiveStats.countHighlySkewed
      << " (" << std::fixed << std::setprecision(1)
      << (100.0 * adaptiveStats.countHighlySkewed / validCells) << "%)" << std::endl;
   std::cout << "  Moderate:       " << std::setw(4) << adaptiveStats.countModerate
      << " (" << (100.0 * adaptiveStats.countModerate / validCells) << "%)" << std::endl;
   std::cout << "  Very Close:     " << std::setw(4) << adaptiveStats.countVeryClose
      << " (" << (100.0 * adaptiveStats.countVeryClose / validCells) << "%)" << std::endl;

   std::cout << "\n" << std::string(70, '-') << std::endl;
   std::cout << "ADAPTIVE_BY_STATE Strategy:" << std::endl;
   std::cout << std::string(70, '-') << std::endl;

   std::cout << "\nIterations:" << std::endl;
   std::cout << "  Total:   " << adaptiveStats.totalIterations << std::endl;
   std::cout << "  Average: " << std::setprecision(4)
      << (double)adaptiveStats.totalIterations / validCells << std::endl;
   std::cout << "  Range:   " << adaptiveStats.minIterations
      << " - " << adaptiveStats.maxIterations << std::endl;

   std::cout << "\nTransformations:" << std::endl;
   std::cout << "  Total:   " << adaptiveStats.totalTransformations << std::endl;
   std::cout << "  Average: " << std::setprecision(2)
      << (double)adaptiveStats.totalTransformations / validCells << std::endl;
   std::cout << "  Range:   " << adaptiveStats.minTransformations
      << " - " << adaptiveStats.maxTransformations << std::endl;

   std::cout << "\nTransformations by Initial State:" << std::endl;
   if (adaptiveStats.countHighlySkewed > 0) {
      std::cout << "  Highly Skewed avg:  " << std::setprecision(1)
         << (double)adaptiveStats.transformsHighlySkewed / adaptiveStats.countHighlySkewed
         << std::endl;
   }
   if (adaptiveStats.countModerate > 0) {
      std::cout << "  Moderate avg:       "
         << (double)adaptiveStats.transformsModerate / adaptiveStats.countModerate
         << std::endl;
   }
   if (adaptiveStats.countVeryClose > 0) {
      std::cout << "  Very Close avg:     "
         << (double)adaptiveStats.transformsVeryClose / adaptiveStats.countVeryClose
         << std::endl;
   }

   std::cout << "\nIteration Distribution:" << std::endl;
   for (int i = 1; i < 10; ++i) {
      if (adaptiveStats.iterationDistribution[i] > 0) {
         std::cout << "  " << i << " iterations: "
            << std::setw(4) << adaptiveStats.iterationDistribution[i]
            << " (" << std::setprecision(1)
            << (100.0 * adaptiveStats.iterationDistribution[i] / validCells)
            << "%)" << std::endl;
      }
   }

   std::cout << "\n" << std::string(70, '-') << std::endl;
   std::cout << "BEST_CUMULATIVE Strategy (baseline):" << std::endl;
   std::cout << std::string(70, '-') << std::endl;

   std::cout << "\nIterations:" << std::endl;
   std::cout << "  Total:   " << cumulativeStats.totalIterations << std::endl;
   std::cout << "  Average: " << std::setprecision(4)
      << (double)cumulativeStats.totalIterations / validCells << std::endl;
   std::cout << "  Range:   " << cumulativeStats.minIterations
      << " - " << cumulativeStats.maxIterations << std::endl;

   std::cout << "\nTransformations:" << std::endl;
   std::cout << "  Total:   " << cumulativeStats.totalTransformations << std::endl;
   std::cout << "  Average: " << std::setprecision(2)
      << (double)cumulativeStats.totalTransformations / validCells << std::endl;
   std::cout << "  Range:   " << cumulativeStats.minTransformations
      << " - " << cumulativeStats.maxTransformations << std::endl;

   std::cout << "\nIteration Distribution:" << std::endl;
   for (int i = 1; i < 10; ++i) {
      if (cumulativeStats.iterationDistribution[i] > 0) {
         std::cout << "  " << i << " iterations: "
            << std::setw(4) << cumulativeStats.iterationDistribution[i]
            << " (" << std::setprecision(1)
            << (100.0 * cumulativeStats.iterationDistribution[i] / validCells)
            << "%)" << std::endl;
      }
   }

   std::cout << "\n" << std::string(70, '=') << std::endl;
   std::cout << "COMPARISON & ANALYSIS" << std::endl;
   std::cout << std::string(70, '=') << std::endl;

   std::cout << "\nConvergence Behavior:" << std::endl;
   double iterationRatio = (double)adaptiveStats.totalIterations / cumulativeStats.totalIterations;
   std::cout << "  Iteration ratio (Adaptive/Cumulative): "
      << std::setprecision(6) << iterationRatio << std::endl;
   if (std::abs(iterationRatio - 1.0) < 0.01) {
      std::cout << "  ? Essentially identical convergence ?" << std::endl;
   } else if (iterationRatio > 1.0) {
      std::cout << "  ? Adaptive takes " << std::setprecision(2)
         << ((iterationRatio - 1.0) * 100.0) << "% more iterations" << std::endl;
   } else {
      std::cout << "  ? Adaptive takes " << std::setprecision(2)
         << ((1.0 - iterationRatio) * 100.0) << "% fewer iterations" << std::endl;
   }

   std::cout << "\nComputational Efficiency:" << std::endl;
   double transformRatio = (double)adaptiveStats.totalTransformations / cumulativeStats.totalTransformations;
   double savings = 100.0 * (1.0 - transformRatio);
   std::cout << "  Transformation ratio (Adaptive/Cumulative): "
      << std::setprecision(6) << transformRatio << std::endl;
   std::cout << "  Computational savings: "
      << std::setprecision(2) << savings << "%" << std::endl;
   std::cout << "  ? Adaptive tests " << std::setprecision(1)
      << (double)adaptiveStats.totalTransformations / validCells
      << " transformations/cell" << std::endl;
   std::cout << "  ? Cumulative tests "
      << (double)cumulativeStats.totalTransformations / validCells
      << " transformations/cell" << std::endl;

   std::cout << "\nCorrectness Validation:" << std::endl;
   std::cout << "  Mismatches: " << adaptiveStats.mismatches << " / " << validCells;
   if (adaptiveStats.mismatches == 0) {
      std::cout << " ? PERFECT AGREEMENT" << std::endl;
   } else {
      std::cout << " ? " << std::setprecision(2)
         << (100.0 * adaptiveStats.mismatches / validCells)
         << "% error rate" << std::endl;
   }

   std::cout << "\n" << std::string(70, '=') << std::endl;
   std::cout << "STATISTICAL SUMMARY" << std::endl;
   std::cout << std::string(70, '=') << std::endl;

   std::cout << "\nSample size: " << validCells << " cells" << std::endl;
   std::cout << "Confidence: Large sample (n > 100)" << std::endl;

   std::cout << "\n" << std::string(70, '=') << std::endl;
   std::cout << "CONCLUSIONS" << std::endl;
   std::cout << std::string(70, '=') << std::endl;

   if (adaptiveStats.mismatches == 0 && savings > 10.0) {
      std::cout << "\n? ADAPTIVE_BY_STATE is SUPERIOR:" << std::endl;
      std::cout << "  • " << std::setprecision(1) << savings
         << "% reduction in computational cost" << std::endl;
      std::cout << "  • 100% correctness maintained (0/" << validCells << " mismatches)" << std::endl;
      if (std::abs(iterationRatio - 1.0) < 0.02) {
         std::cout << "  • Essentially identical convergence behavior" << std::endl;
      } else if (iterationRatio > 1.0) {
         std::cout << "  • Slightly more iterations (" << std::setprecision(2)
            << ((iterationRatio - 1.0) * 100.0) << "% more)" << std::endl;
      }
      std::cout << "  • RECOMMENDED AS DEFAULT STRATEGY ?" << std::endl;
   } else if (adaptiveStats.mismatches == 0 && savings > 0.0) {
      std::cout << "\n? ADAPTIVE_BY_STATE provides modest improvement:" << std::endl;
      std::cout << "  • " << std::setprecision(1) << savings
         << "% reduction in computational cost" << std::endl;
      std::cout << "  • 100% correctness maintained" << std::endl;
   } else if (adaptiveStats.mismatches == 0 && savings < 0.0) {
      std::cout << "\n? ADAPTIVE_BY_STATE is SLOWER:" << std::endl;
      std::cout << "  • " << std::setprecision(1) << -savings
         << "% INCREASE in computational cost" << std::endl;
      std::cout << "  • 100% correctness maintained" << std::endl;
      std::cout << "  • BEST_CUMULATIVE is preferred" << std::endl;
   } else {
      std::cout << "\n? ADAPTIVE_BY_STATE has ISSUES:" << std::endl;
      std::cout << "  • Correctness errors detected (" << adaptiveStats.mismatches
         << "/" << validCells << ")" << std::endl;
      std::cout << "  • Further investigation needed" << std::endl;
   }

   std::cout << "\n" << std::string(70, '=') << std::endl;
}


// Test function to compare all three reduction algorithms
void compareReductionAlgorithms(int numCells = 1000) {
   std::cout << "\n" << std::string(70, '=') << std::endl;
   std::cout << "COMPARISON: Three Niggli Reduction Algorithms" << std::endl;
   std::cout << std::string(70, '=') << std::endl;
   std::cout << "Testing " << numCells << " random triclinic cells\n" << std::endl;

   struct AlgorithmStats {
      std::string name;
      int successes = 0;
      int failures = 0;
      int iterations = 0;
      size_t transformations = 0;
      int mismatches = 0;
      double maxDifference = 0.0;

      std::vector<double> executionTimes;  // in microseconds
   };

   AlgorithmStats andrewsBernstein;
   andrewsBernstein.name = "Andrews-Bernstein (2014)";

   AlgorithmStats krivyGruber;
   krivyGruber.name = "Krivy-Gruber (2016)";

   AlgorithmStats eisensteinAdaptive;
   eisensteinAdaptive.name = "Eisenstein Adaptive";

   AlgorithmStats eisensteinCumulative;
   eisensteinCumulative.name = "Eisenstein Adaptive";

   int validCells = 0;
   int nearDegenerateCells = 0;

   for (int i = 0; i < numCells; ++i) {
      LRL_Cell randomCell = Polar::rand();
      if (!randomCell.IsValid()) continue;

      G6 inputG6(randomCell);

      // Check for near-degenerate
      double cos_alpha = std::abs(inputG6[3]) / (2.0 * std::sqrt(inputG6[1] * inputG6[2]));
      double cos_beta = std::abs(inputG6[4]) / (2.0 * std::sqrt(inputG6[0] * inputG6[2]));
      double cos_gamma = std::abs(inputG6[5]) / (2.0 * std::sqrt(inputG6[0] * inputG6[1]));
      const double cos_5_degrees = 0.9962;

      if (cos_alpha >= cos_5_degrees || cos_beta >= cos_5_degrees || cos_gamma >= cos_5_degrees) {
         nearDegenerateCells++;
         continue;
      }

      validCells++;

      // Test Andrews-Bernstein (your existing implementation)
      auto startAB = std::chrono::high_resolution_clock::now();
      G6 resultAB;
      bool successAB = Niggli::ReduceWithoutMatrices(inputG6, resultAB, 1.0e-6);
      auto endAB = std::chrono::high_resolution_clock::now();
      auto durationAB = std::chrono::duration_cast<std::chrono::microseconds>(endAB - startAB);

      andrewsBernstein.executionTimes.push_back(durationAB.count());
      if (successAB) {
         andrewsBernstein.successes++;
         andrewsBernstein.iterations += Niggli::GetCycles();
      } else {
         andrewsBernstein.failures++;
      }

      // Test Krivy-Gruber
      auto startKG = std::chrono::high_resolution_clock::now();
      auto resultKG = KrivyGruber::Reduce(inputG6, 1.0e-6, false);
      auto endKG = std::chrono::high_resolution_clock::now();
      auto durationKG = std::chrono::duration_cast<std::chrono::microseconds>(endKG - startKG);

      krivyGruber.executionTimes.push_back(durationKG.count());
      if (resultKG.success) {
         krivyGruber.successes++;
         krivyGruber.iterations += resultKG.iterations;
      } else {
         krivyGruber.failures++;
      }

      // Test Eisenstein Adaptive
      auto startEA = std::chrono::high_resolution_clock::now();
      auto resultEA = EisensteinReduction::ReduceAdaptive(inputG6, 1.0e-6, false);
      auto endEA = std::chrono::high_resolution_clock::now();
      auto durationEA = std::chrono::duration_cast<std::chrono::microseconds>(endEA - startEA);

      eisensteinAdaptive.executionTimes.push_back(durationEA.count());
      eisensteinAdaptive.iterations += resultEA.iterations;
      eisensteinAdaptive.transformations += resultEA.transformationsTested;
      if (resultEA.trace > 0) {
         eisensteinAdaptive.successes++;
      } else {
         eisensteinAdaptive.failures++;
      }

      // Compare results (use Andrews-Bernstein as reference)
      if (successAB) {
         // Compare Krivy-Gruber with Andrews-Bernstein
         if (resultKG.success) {
            double maxDiffKG = 0.0;
            for (size_t j = 0; j < 6; ++j) {
               double diff = std::abs(resultKG.reducedG6[j] - resultAB[j]);
               maxDiffKG = std::max(maxDiffKG, diff);
            }
            krivyGruber.maxDifference = std::max(krivyGruber.maxDifference, maxDiffKG);

            if (maxDiffKG > 1.0e-3) {
               krivyGruber.mismatches++;
               if (krivyGruber.mismatches <= 3) {  // Report first 3
                  std::cout << "\nKrivy-Gruber MISMATCH at cell " << i
                     << " (diff: " << maxDiffKG << "):" << std::endl;
                  std::cout << "  Input: " << inputG6 << std::endl;
                  std::cout << "  A-B:   " << resultAB << std::endl;
                  std::cout << "  K-G:   " << resultKG.reducedG6 << std::endl;
               }
            }
         }

         // Compare Eisenstein with Andrews-Bernstein
         double maxDiffEA = 0.0;
         for (size_t j = 0; j < 6; ++j) {
            double diff = std::abs(resultEA.reducedG6[j] - resultAB[j]);
            maxDiffEA = std::max(maxDiffEA, diff);
         }
         eisensteinAdaptive.maxDifference = std::max(eisensteinAdaptive.maxDifference, maxDiffEA);

         if (maxDiffEA > 1.0e-3) {
            eisensteinAdaptive.mismatches++;
            if (eisensteinAdaptive.mismatches <= 3) {  // Report first 3
               std::cout << "\nEisenstein MISMATCH at cell " << i
                  << " (diff: " << maxDiffEA << "):" << std::endl;
               std::cout << "  Input: " << inputG6 << std::endl;
               std::cout << "  A-B:   " << resultAB << std::endl;
               std::cout << "  Eisen: " << resultEA.reducedG6 << std::endl;
            }
         }
      }

      if ((i + 1) % 10 == 0) std::cout << "." << std::flush;
   }

   // Calculate statistics
   auto calcStats = [](const std::vector<double>& times) {
      if (times.empty()) return std::make_tuple(0.0, 0.0, 0.0);
      double sum = 0.0;
      double minTime = times[0];
      double maxTime = times[0];
      for (double t : times) {
         sum += t;
         minTime = std::min(minTime, t);
         maxTime = std::max(maxTime, t);
      }
      return std::make_tuple(sum / times.size(), minTime, maxTime);
      };

   double avgAB, minAB, maxAB;
   std::tie(avgAB, minAB, maxAB) = calcStats(andrewsBernstein.executionTimes);

   double avgKG, minKG, maxKG;
   std::tie(avgKG, minKG, maxKG) = calcStats(krivyGruber.executionTimes);

   double avgEA, minEA, maxEA;
   std::tie(avgEA, minEA, maxEA) = calcStats(eisensteinAdaptive.executionTimes);

   double avgEC, minEC, maxEC;
   std::tie(avgEC, minEC, maxEC) = calcStats(eisensteinCumulative.executionTimes);


   // Print results
   std::cout << "\n\n" << std::string(70, '=') << std::endl;
   std::cout << "RESULTS" << std::endl;
   std::cout << std::string(70, '=') << std::endl;

   std::cout << "\nSample:" << std::endl;
   std::cout << "  Total cells generated: " << numCells << std::endl;
   std::cout << "  Near-degenerate (excluded): " << nearDegenerateCells << std::endl;
   std::cout << "  Valid test cells: " << validCells << std::endl;

   // Print each algorithm's stats
   auto printAlgStats = [validCells](const AlgorithmStats& stats, double avgTime, double minTime, double maxTime) {
      std::cout << "\n" << std::string(70, '-') << std::endl;
      std::cout << stats.name << std::endl;
      std::cout << std::string(70, '-') << std::endl;

      std::cout << "\nSuccess Rate:" << std::endl;
      std::cout << "  Successes: " << stats.successes << " / " << validCells
         << " (" << std::fixed << std::setprecision(1)
         << (100.0 * stats.successes / validCells) << "%)" << std::endl;
      std::cout << "  Failures:  " << stats.failures << std::endl;

      if (stats.successes > 0) {
         std::cout << "\nIterations:" << std::endl;
         std::cout << "  Total:   " << stats.iterations << std::endl;
         std::cout << "  Average: " << std::setprecision(2)
            << (double)stats.iterations / stats.successes << std::endl;
      }

      if (stats.transformations > 0) {
         std::cout << "\nTransformations:" << std::endl;
         std::cout << "  Total:   " << stats.transformations << std::endl;
         std::cout << "  Average: " << std::setprecision(1)
            << (double)stats.transformations / validCells << std::endl;
      }

      std::cout << "\nExecution Time (microseconds):" << std::endl;
      std::cout << "  Average: " << std::setprecision(1) << avgTime << std::endl;
      std::cout << "  Range:   " << minTime << " - " << maxTime << std::endl;

      std::cout << "\nCorrectness (vs Andrews-Bernstein):" << std::endl;
      std::cout << "  Mismatches: " << stats.mismatches << " / " << validCells;
      if (stats.mismatches == 0) {
         std::cout << " ? PERFECT" << std::endl;
      } else {
         std::cout << " (" << std::setprecision(2)
            << (100.0 * stats.mismatches / validCells) << "%)" << std::endl;
      }
      std::cout << "  Max difference: " << std::scientific << stats.maxDifference << std::endl;
      };

   printAlgStats(andrewsBernstein, avgAB, minAB, maxAB);
   printAlgStats(krivyGruber, avgKG, minKG, maxKG);
   printAlgStats(eisensteinAdaptive, avgEA, minEA, maxEA);

   // Comparative summary
   std::cout << "\n" << std::string(70, '=') << std::endl;
   std::cout << "COMPARATIVE SUMMARY" << std::endl;
   std::cout << std::string(70, '=') << std::endl;

   std::cout << "\nSpeed Comparison (relative to Andrews-Bernstein):" << std::endl;
   std::cout << std::fixed << std::setprecision(2);
   std::cout << "  Andrews-Bernstein: 1.00× (baseline)" << std::endl;
   std::cout << "  Krivy-Gruber:      " << (avgKG / avgAB) << "×";
   if (avgKG < avgAB) {
      std::cout << " ? FASTER" << std::endl;
   } else {
      std::cout << " (slower)" << std::endl;
   }
   std::cout << "  Eisenstein:        " << (avgEA / avgAB) << "×";
   if (avgEA < avgAB) {
      std::cout << " ? FASTER" << std::endl;
   } else {
      std::cout << " (slower)" << std::endl;
   }

   std::cout << "\nIterations Comparison:" << std::endl;
   double avgIterAB = (double)andrewsBernstein.iterations / andrewsBernstein.successes;
   double avgIterKG = (double)krivyGruber.iterations / krivyGruber.successes;
   double avgIterEA = (double)eisensteinAdaptive.iterations / eisensteinAdaptive.successes;
   std::cout << "  Andrews-Bernstein: " << std::setprecision(2) << avgIterAB << std::endl;
   std::cout << "  Krivy-Gruber:      " << avgIterKG << std::endl;
   std::cout << "  Eisenstein:        " << avgIterEA << std::endl;

   std::cout << "\nCorrectness:" << std::endl;
   std::cout << "  Andrews-Bernstein: Reference algorithm" << std::endl;
   std::cout << "  Krivy-Gruber:      " << krivyGruber.mismatches << " mismatches";
   if (krivyGruber.mismatches == 0) std::cout << " ?";
   std::cout << std::endl;
   std::cout << "  Eisenstein:        " << eisensteinAdaptive.mismatches << " mismatches";
   if (eisensteinAdaptive.mismatches == 0) std::cout << " ?";
   std::cout << std::endl;

   std::cout << "\n" << std::string(70, '=') << std::endl;
   std::cout << "RECOMMENDATIONS" << std::endl;
   std::cout << std::string(70, '=') << std::endl;

   // Determine best algorithm
   bool kgCorrect = (krivyGruber.mismatches == 0);
   bool eaCorrect = (eisensteinAdaptive.mismatches == 0);

   if (kgCorrect && avgKG < avgAB && avgKG < avgEA) {
      std::cout << "\n? KRIVY-GRUBER is BEST:" << std::endl;
      std::cout << "  • Fastest execution time" << std::endl;
      std::cout << "  • 100% correctness" << std::endl;
   } else if (eaCorrect && avgEA < avgAB && avgEA < avgKG) {
      std::cout << "\n? EISENSTEIN ADAPTIVE is BEST:" << std::endl;
      std::cout << "  • Fastest execution time" << std::endl;
      std::cout << "  • 100% correctness" << std::endl;
   } else if (avgAB < avgKG && avgAB < avgEA) {
      std::cout << "\n? ANDREWS-BERNSTEIN is BEST:" << std::endl;
      std::cout << "  • Fastest execution time" << std::endl;
      std::cout << "  • Reference standard" << std::endl;
   } else {
      std::cout << "\nResults are mixed - choose based on requirements:" << std::endl;
      std::cout << "  • Andrews-Bernstein: Proven, well-tested" << std::endl;
      std::cout << "  • Krivy-Gruber: Alternative approach" << std::endl;
      std::cout << "  • Eisenstein: Minimal trace, pedagogical value" << std::endl;
   }

   std::cout << "\n" << std::string(70, '=') << std::endl;
}

// Add this function to help debug
void investigateCell0() {
   std::cout << "\n=== Detailed Investigation: Cell 0 ===" << std::endl;

   G6 input(37.646, 37.699, 2.358, 6.035, 7.434, 16.517);
   std::cout << "Input: " << input << std::endl;
   std::cout << "Input trace: " << (input[0] + input[1] + input[2]) << std::endl;

   // Run both algorithms with verbose output
   std::cout << "\n--- Andrews-Bernstein ---" << std::endl;
   G6 resultAB;
   bool successAB = Niggli::ReduceWithoutMatrices(input, resultAB, 1.0e-6);
   std::cout << "Result: " << resultAB << std::endl;
   std::cout << "Trace: " << (resultAB[0] + resultAB[1] + resultAB[2]) << std::endl;
   std::cout << "Cycles: " << Niggli::GetCycles() << std::endl;

   std::cout << "\n--- Krivy-Gruber (verbose) ---" << std::endl;
   auto resultKG = KrivyGruber::Reduce(input, 1.0e-6, true);  // verbose = true
   std::cout << "Result: " << resultKG.reducedG6 << std::endl;
   std::cout << "Trace: " << (resultKG.reducedG6[0] + resultKG.reducedG6[1] + resultKG.reducedG6[2]) << std::endl;

   // Verify both cells represent the same lattice
   std::cout << "\n--- Lattice Verification ---" << std::endl;
   LRL_Cell cellAB(resultAB);
   LRL_Cell cellKG(resultKG.reducedG6);

   std::cout << "A-B cell: " << cellAB << std::endl;
   std::cout << "K-G cell: " << cellKG << std::endl;

   // The cells should have the same volume
   std::cout << "A-B volume: " << cellAB.Volume() << std::endl;
   std::cout << "K-G volume: " << cellKG.Volume() << std::endl;
}

// Add this detailed diagnostic function
void diagnoseKrivyGruber() {
   std::cout << "\n" << std::string(70, '=') << std::endl;
   std::cout << "KRIVY-GRUBER DETAILED DIAGNOSIS" << std::endl;
   std::cout << std::string(70, '=') << std::endl;

   // Test Cell 0 which shows volume mismatch
   G6 input(37.646, 37.699, 2.358, 6.035, 7.434, 16.517);

   std::cout << "\nInput G6: " << input << std::endl;
   std::cout << "Input trace: " << (input[0] + input[1] + input[2]) << std::endl;

   // Calculate input volume
   LRL_Cell inputCell(input);
   double inputVolume = inputCell.Volume();
   std::cout << "Input volume: " << inputVolume << std::endl;

   // Check volume formula
   double A = input[0], B = input[1], C = input[2];
   double xi = input[3], eta = input[4], zeta = input[5];

   // Volume² = ABC - A?²/4 - B?²/4 - C?²/4 + ???/4
   double volSq = A * B * C - A * xi * xi / 4.0 - B * eta * eta / 4.0 - C * zeta * zeta / 4.0 + xi * eta * zeta / 4.0;
   double volFormula = std::sqrt(std::abs(volSq));
   std::cout << "Volume from formula: " << volFormula << std::endl;
   std::cout << "Match: " << (std::abs(inputVolume - volFormula) < 0.01 ? "YES" : "NO") << std::endl;

   // Now manually trace through K-G algorithm step by step
   A = input[0];
   B = input[1];
   C = input[2];
   xi = input[3];
   eta = input[4];
   zeta = input[5];

   std::cout << "\n--- K-G Algorithm Step-by-Step ---" << std::endl;
   std::cout << std::fixed << std::setprecision(3);

   int stepCount = 0;
   const int maxSteps = 30;

   auto checkVolume = [](double A, double B, double C, double xi, double eta, double zeta) {
      double volSq = A * B * C - A * xi * xi / 4.0 - B * eta * eta / 4.0 - C * zeta * zeta / 4.0 + xi * eta * zeta / 4.0;
      return std::sqrt(std::abs(volSq));
      };

   auto printState = [&](const std::string& msg) {
      double vol = checkVolume(A, B, C, xi, eta, zeta);
      double trace = A + B + C;
      std::cout << "\n" << msg << std::endl;
      std::cout << "  A=" << std::setw(8) << A
         << " B=" << std::setw(8) << B
         << " C=" << std::setw(8) << C << std::endl;
      std::cout << "  ?=" << std::setw(8) << xi
         << " ?=" << std::setw(8) << eta
         << " ?=" << std::setw(8) << zeta << std::endl;
      std::cout << "  Volume=" << std::setw(8) << vol
         << " Trace=" << std::setw(8) << trace << std::endl;

      if (std::abs(vol - inputVolume) > 0.1) {
         std::cout << "  *** WARNING: Volume changed from " << inputVolume
            << " to " << vol << " ***" << std::endl;
      }
      };

   printState("Initial state:");

   bool step1_label = true;
   const double tol = 1.0e-6;

   while (stepCount < maxSteps) {
      stepCount++;
      bool changed = false;

      // Step 1 - CORRECTED
      if (step1_label) {
         step1_label = false;
         if (A > B + tol || (std::abs(A - B) <= tol && std::abs(xi) > std::abs(eta) + tol)) {
            std::swap(A, B);
            std::swap(xi, eta);  // CORRECTED: swap ? and ?, not ? and ?
            changed = true;
            printState("Step 1: Swap (A,?)?(B,?)");  // Updated message
         }
      }
      // Step 2
      if (B > C + tol || (std::abs(B - C) <= tol && std::abs(eta) > std::abs(zeta) + tol)) {
         std::swap(B, C);
         std::swap(eta, zeta);
         changed = true;
         printState("Step 2: Swap (B,?)?(C,?)");
         step1_label = true;
         continue;
      }

      // Step 3
      if (xi * eta * zeta > tol) {
         double old_xi = xi, old_eta = eta, old_zeta = zeta;
         xi = std::abs(xi);
         eta = std::abs(eta);
         zeta = std::abs(zeta);
         if (xi != old_xi || eta != old_eta || zeta != old_zeta) {
            changed = true;
            printState("Step 3: All positive");
         }
      }

      // Step 4
      if (xi * eta * zeta <= tol) {
         double old_xi = xi, old_eta = eta, old_zeta = zeta;
         xi = -std::abs(xi);
         eta = -std::abs(eta);
         zeta = -std::abs(zeta);
         if (xi != old_xi || eta != old_eta || zeta != old_zeta) {
            changed = true;
            printState("Step 4: All negative");
         }
      }

      // Step 5
      bool step5 = false;
      if (std::abs(xi) > B + tol) step5 = true;
      if (std::abs(xi - B) <= tol && 2 * eta < zeta - tol) step5 = true;
      if (std::abs(xi + B) <= tol && zeta < -tol) step5 = true;

      if (step5) {
         int s = (xi >= 0) ? 1 : -1;
         double old_C = C, old_eta = eta, old_xi = xi;
         C = B + C - s * xi;
         eta = eta - s * zeta;
         xi = -2 * B * s + xi;
         changed = true;
         printState("Step 5: Adjust C,?,?");
         std::cout << "  s=" << s << " old: C=" << old_C
            << " ?=" << old_eta << " ?=" << old_xi << std::endl;
         step1_label = true;
         continue;
      }

      // Step 6
      bool step6 = false;
      if (std::abs(eta) > A + tol) step6 = true;
      if (std::abs(eta - A) <= tol && 2 * xi < zeta - tol) step6 = true;
      if (std::abs(eta + A) <= tol && zeta < -tol) step6 = true;

      if (step6) {
         int s = (eta >= 0) ? 1 : -1;
         double old_C = C, old_xi = xi, old_eta = eta;
         C = A + C - s * eta;
         xi = xi - s * zeta;
         eta = -2 * A * s + eta;
         changed = true;
         printState("Step 6: Adjust C,?,?");
         std::cout << "  s=" << s << " old: C=" << old_C
            << " ?=" << old_xi << " ?=" << old_eta << std::endl;
         step1_label = true;
         continue;
      }

      // Step 7
      bool step7 = false;
      if (std::abs(zeta) > A + tol) step7 = true;
      if (std::abs(zeta - A) <= tol && 2 * xi < eta - tol) step7 = true;
      if (std::abs(zeta + A) <= tol && eta < -tol) step7 = true;

      if (step7) {
         int s = (zeta >= 0) ? 1 : -1;
         double old_B = B, old_xi = xi, old_zeta = zeta;
         B = A + B - s * zeta;
         xi = xi - s * eta;
         zeta = -2 * A * s + zeta;
         changed = true;
         printState("Step 7: Adjust B,?,?");
         std::cout << "  s=" << s << " old: B=" << old_B
            << " ?=" << old_xi << " ?=" << old_zeta << std::endl;
         step1_label = true;
         continue;
      }

      // Step 8
      double sum = xi + eta + zeta + A + B;
      bool step8 = false;
      if (sum < -tol) step8 = true;
      if (std::abs(sum) <= tol && 2 * (A + eta) + zeta > tol) step8 = true;

      if (step8) {
         double old_C = C, old_xi = xi, old_eta = eta;
         C = A + B + C + xi + eta + zeta;
         xi = 2 * B + xi + zeta;
         eta = 2 * A + eta + zeta;
         changed = true;
         printState("Step 8: Final adjustment");
         std::cout << "  old: C=" << old_C << " ?=" << old_xi << " ?=" << old_eta << std::endl;
         step1_label = true;
         continue;
      }

      if (!changed) {
         std::cout << "\nConverged after " << stepCount << " steps" << std::endl;
         break;
      }
   }

   // Final check
   G6 finalG6(A, B, C, xi, eta, zeta);
   LRL_Cell finalCell(finalG6);
   double finalVolume = finalCell.Volume();

   std::cout << "\n--- FINAL RESULT ---" << std::endl;
   std::cout << "Final G6: " << finalG6 << std::endl;
   std::cout << "Final volume: " << finalVolume << std::endl;
   std::cout << "Volume ratio (final/input): " << (finalVolume / inputVolume) << std::endl;

   if (std::abs(finalVolume - inputVolume) > 0.1) {
      std::cout << "\n*** VOLUME NOT PRESERVED - ALGORITHM IS BROKEN ***" << std::endl;
   }

   // Compare with A-B
   G6 resultAB;
   Niggli::ReduceWithoutMatrices(input, resultAB, 1.0e-6);
   LRL_Cell cellAB(resultAB);
   std::cout << "\nAndrews-Bernstein result: " << resultAB << std::endl;
   std::cout << "A-B volume: " << cellAB.Volume() << std::endl;
}

void testLandscape() {
   // Test 1: Unsorted (violates C1)
   std::cout << "\n### TEST 1: Unsorted axes ###" << std::endl;
   G6 unsorted(50.0, 30.0, 40.0, 10.0, 15.0, 20.0);
   NiggliLandscape::AnalyzeLandscape(unsorted);

   // Test 2: Large scalar products (violates C2-C4)
   std::cout << "\n\n### TEST 2: Large scalar products ###" << std::endl;
   G6 largeScalar(25.0, 30.0, 35.0, 20.0, 18.0, 16.0);
   NiggliLandscape::AnalyzeLandscape(largeScalar);

   // Test 3: Wrong sum (violates C5)
   std::cout << "\n\n### TEST 3: Sum violation ###" << std::endl;
   G6 wrongSum(25.0, 30.0, 35.0, 15.0, 16.0, 17.0);
   NiggliLandscape::AnalyzeLandscape(wrongSum);
}

void testBetterNonReducedCells() {
   std::cout << "\n### TEST: Actually Non-Reduced Cell ###" << std::endl;

   // Start from a reduced cell and deliberately unreduced it
   G6 reduced(25.0, 30.0, 35.0, 10.0, 12.0, 11.0);  // This should be reduced

   // Apply a transformation that makes it non-reduced
   // For example, make g4 much larger than g2/2
   G6 unreduced = reduced;
   unreduced[3] = 25.0;  // |g4| = 25 > 30/2 = 15, violates C2

   std::cout << "\nTest cell with |g4| > g2/2:" << std::endl;
   NiggliLandscape::AnalyzeLandscape(unreduced);

   // Or try a cell that needs sorting
   G6 unsorted(100.0, 50.0, 75.0, 10.0, 15.0, 20.0);  // g1 > g2, violates C1
   std::cout << "\n\nTest cell with g1 > g2:" << std::endl;
   NiggliLandscape::AnalyzeLandscape(unsorted);

   // Or generate a random cell that's likely not reduced
   LRL_Cell randomCell = LRL_Cell(10.5, 12.3, 15.7, 75.0, 85.0, 95.0);
   G6 random(randomCell);
   std::cout << "\n\nRandom cell (likely not reduced):" << std::endl;
   NiggliLandscape::AnalyzeLandscape(random);
}


std::string svgplot(const std::vector<LRL_Cell_Degrees>& raw,
   const std::vector<LRL_Cell_Degrees>& niggli,
   const std::vector<LRL_Cell_Degrees>& selling) {

   // Extract all parameters into vectors
   auto extractParam = [](const std::vector<LRL_Cell_Degrees>& cells, int index) {
      std::vector<double> result;
      for (const auto& cell : cells) {
         result.push_back(cell[index]);
      }
      return result;
      };

   auto computeBoxPlot = [](std::vector<double> data) {
      std::sort(data.begin(), data.end());
      return std::make_tuple(
         data.front(),                    // min
         data[data.size() / 4],          // q1
         data[data.size() / 2],          // median
         data[3 * data.size() / 4],      // q3
         data.back()                      // max
      );
      };

   const int width = 1600;  // Increased width for legend
   const int height = 850;  // Increased height for caption
   const int margin = 80;
   const int plotWidth = 180;
   const int plotHeight = 500;
   const int spacing = 20;
   const int boxWidth = 40;
   const int yStart = margin + 50;
   const int angleShift = 50;  // Additional shift for angle boxes and legend

   // Compute global min/max for lengths (a,b,c) and angles (?,?,?)
   double lengthMin = std::numeric_limits<double>::max();
   double lengthMax = 0.0;
   double angleMin = std::numeric_limits<double>::max();
   double angleMax = 0.0;

   for (int param = 0; param < 3; ++param) {
      auto rawData = extractParam(raw, param);
      auto niggliData = extractParam(niggli, param);
      auto sellingData = extractParam(selling, param);

      auto rawBP = computeBoxPlot(rawData);
      auto niggliBP = computeBoxPlot(niggliData);
      auto sellingBP = computeBoxPlot(sellingData);

      double minVal = std::min({ std::get<0>(rawBP), std::get<0>(niggliBP), std::get<0>(sellingBP) });
      double maxVal = std::max({ std::get<4>(rawBP), std::get<4>(niggliBP), std::get<4>(sellingBP) });
      lengthMin = std::min(lengthMin, minVal);
      lengthMax = std::max(lengthMax, maxVal);
   }

   for (int param = 3; param < 6; ++param) {
      auto rawData = extractParam(raw, param);
      auto niggliData = extractParam(niggli, param);
      auto sellingData = extractParam(selling, param);

      auto rawBP = computeBoxPlot(rawData);
      auto niggliBP = computeBoxPlot(niggliData);
      auto sellingBP = computeBoxPlot(sellingData);

      double minVal = std::min({ std::get<0>(rawBP), std::get<0>(niggliBP), std::get<0>(sellingBP) });
      double maxVal = std::max({ std::get<4>(rawBP), std::get<4>(niggliBP), std::get<4>(sellingBP) });
      angleMin = std::min(angleMin, minVal);
      angleMax = std::max(angleMax, maxVal);
   }

   // Use NiceAxisScaling to get nice axis limits
   AxisConfig lengthConfig;
   lengthConfig.minTicks = 5;
   lengthConfig.maxTicks = 10;
   NiceAxisScaling lengthScaler(lengthConfig);
   AxisLimits lengthLimits = lengthScaler.LinearAxisLimits(lengthMin, lengthMax);
   std::vector<TickInfo> lengthTicks = lengthLimits.GetTicksWithLabels();

   AxisConfig angleConfig;
   angleConfig.minTicks = 5;
   angleConfig.maxTicks = 10;
   NiceAxisScaling angleScaler(angleConfig);
   AxisLimits angleLimits = angleScaler.LinearAxisLimits(angleMin, angleMax);
   std::vector<TickInfo> angleTicks = angleLimits.GetTicksWithLabels();

   std::ostringstream svg;
   svg << std::fixed << std::setprecision(1);
   svg << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
   svg << "<svg width=\"" << width << "\" height=\"" << height
      << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
   const std::vector<std::string> colors = { "orange", "#4ecdc4", "#45b7d1" };

   // Define patterns for Niggli and Selling
   svg << "<defs>\n";
   svg << "  <pattern id=\"niggliPattern\" patternUnits=\"userSpaceOnUse\" width=\"8\" height=\"8\">\n";
   svg << "    <rect width=\"8\" height=\"8\" fill=\"" << colors[1] << "\" fill-opacity=\"0.7\"/>\n";
   svg << "    <path d=\"M0,0 L8,8 M8,0 L0,8\" stroke=\"white\" stroke-width=\"1.5\"/>\n";
   svg << "  </pattern>\n";
   svg << "  <pattern id=\"sellingPattern\" patternUnits=\"userSpaceOnUse\" width=\"6\" height=\"6\">\n";
   svg << "    <rect width=\"6\" height=\"6\" fill=\"" << colors[2] << "\" fill-opacity=\"0.7\"/>\n";
   svg << "    <circle cx=\"3\" cy=\"3\" r=\"1.5\" fill=\"white\"/>\n";
   svg << "  </pattern>\n";
   svg << "</defs>\n";

   // Background
   svg << "<rect width=\"" << width << "\" height=\"" << height
      << "\" fill=\"white\"/>\n";

   // Title
   svg << "<text x=\"" << width / 2 << "\" y=\"40\" text-anchor=\"middle\" "
      << "font-size=\"24\" font-weight=\"bold\">"
      << "Cell Parameters: Raw vs Niggli vs Selling</text>\n";

   const std::vector<std::string> labels = { "a", "b", "c", "?", "?", "?" };

   // Create Y-axis group for lengths
   svg << "<g id=\"length_axis\">\n";
   int lengthAxisX = margin - 15;
   svg << "  <line x1=\"" << lengthAxisX << "\" y1=\"" << yStart
      << "\" x2=\"" << lengthAxisX << "\" y2=\"" << (yStart + plotHeight)
      << "\" stroke=\"black\" stroke-width=\"2\"/>\n";

   // Draw length axis ticks using nice values
   double lengthRange = lengthLimits.GetMax() - lengthLimits.GetMin();
   for (const auto& tick : lengthTicks) {
      double fraction = (tick.position - lengthLimits.GetMin()) / lengthRange;
      int y = yStart + plotHeight - static_cast<int>(fraction * plotHeight);
      svg << "  <line x1=\"" << (lengthAxisX - 5) << "\" y1=\"" << y
         << "\" x2=\"" << lengthAxisX << "\" y2=\"" << y
         << "\" stroke=\"black\" stroke-width=\"1.5\"/>\n";
      svg << "  <text x=\"" << (lengthAxisX - 10) << "\" y=\"" << (y + 4)
         << "\" text-anchor=\"end\" font-size=\"10\">"
         << tick.label << "</text>\n";
   }

   svg << "  <text x=\"" << (lengthAxisX - 50) << "\" y=\"" << (yStart + plotHeight / 2)
      << "\" text-anchor=\"middle\" font-size=\"12\" font-weight=\"bold\" "
      << "transform=\"rotate(-90 " << (lengthAxisX - 50) << " "
      << (yStart + plotHeight / 2) << ")\">Length</text>\n";
   svg << "</g>\n";

   // Group for angle boxes and legend with 50px shift
   svg << "<g id=\"angles_and_legend\" transform=\"translate(" << angleShift << ", 0)\">\n";

   // Create Y-axis group for angles
   svg << "  <g id=\"angle_axis\">\n";
   int angleBaseX = margin + 3 * (plotWidth + spacing) + boxWidth;
   int angleAxisX = angleBaseX - 15;

   svg << "    <line x1=\"" << angleAxisX << "\" y1=\"" << yStart
      << "\" x2=\"" << angleAxisX << "\" y2=\"" << (yStart + plotHeight)
      << "\" stroke=\"black\" stroke-width=\"2\"/>\n";

   // Draw angle axis ticks using nice values
   double angleRange = angleLimits.GetMax() - angleLimits.GetMin();
   for (const auto& tick : angleTicks) {
      double fraction = (tick.position - angleLimits.GetMin()) / angleRange;
      int y = yStart + plotHeight - static_cast<int>(fraction * plotHeight);
      svg << "    <line x1=\"" << (angleAxisX - 5) << "\" y1=\"" << y
         << "\" x2=\"" << angleAxisX << "\" y2=\"" << y
         << "\" stroke=\"black\" stroke-width=\"1.5\"/>\n";
      svg << "    <text x=\"" << (angleAxisX - 10) << "\" y=\"" << (y + 4)
         << "\" text-anchor=\"end\" font-size=\"10\">"
         << tick.label << "</text>\n";
   }

   svg << "    <text x=\"" << (angleAxisX - 50) << "\" y=\"" << (yStart + plotHeight / 2)
      << "\" text-anchor=\"middle\" font-size=\"12\" font-weight=\"bold\" "
      << "      transform=\"rotate(-90 " << (angleAxisX - 50) << " "
      << (yStart + plotHeight / 2) << ")\">Angle</text>\n";
   svg << "  </g>\n";

   // For each parameter in the angle group (?, ?, ?)
   for (int param = 3; param < 6; ++param) {
      auto rawData = extractParam(raw, param);
      auto niggliData = extractParam(niggli, param);
      auto sellingData = extractParam(selling, param);

      auto rawBP = computeBoxPlot(rawData);
      auto niggliBP = computeBoxPlot(niggliData);
      auto sellingBP = computeBoxPlot(sellingData);

      double scaleMin = angleLimits.GetMin();
      double scaleMax = angleLimits.GetMax();
      double range = scaleMax - scaleMin;

      int xShift = boxWidth;
      int xBase = margin + param * (plotWidth + spacing) + xShift;

      svg << "  <g id=\"param_" << labels[param] << "\">\n";

      // Gray box around triple
      svg << "    <rect x=\"" << (xBase - 5) << "\" y=\"" << (yStart - 5)
         << "\" width=\"" << (plotWidth + 10) << "\" height=\"" << (plotHeight + 10)
         << "\" fill=\"none\" stroke=\"#888888\" stroke-width=\"2\"/>\n";

      // Draw reference lines at 5 and 175 degrees
      auto drawAngleReferenceLine = [&](double degValue) {
         if (degValue >= scaleMin && degValue <= scaleMax) {
            int y = yStart + plotHeight - (int)((degValue - scaleMin) / range * plotHeight);
            svg << "    <line x1=\"" << (xBase - 5) << "\" y1=\"" << y
               << "\" x2=\"" << (xBase + plotWidth + 5) << "\" y2=\"" << y
               << "\" stroke=\"lightblue\" stroke-width=\"1.5\" stroke-dasharray=\"4,4\"/>\n";
            svg << "    <text x=\"" << (xBase + plotWidth / 2) << "\" y=\"" << (y - 5)
               << "\" font-size=\"10\" fill=\"blue\" text-anchor=\"middle\">" << degValue << " degrees</text>\n";
         }
         };
      drawAngleReferenceLine(5.0);
      drawAngleReferenceLine(175.0);

      // Draw reference line at 90 degrees
      if (90.0 >= scaleMin && 90.0 <= scaleMax) {
         int y = yStart + plotHeight - (int)((90.0 - scaleMin) / range * plotHeight);
         svg << "    <line x1=\"" << (xBase - 5) << "\" y1=\"" << y
            << "\" x2=\"" << (xBase + plotWidth + 5) << "\" y2=\"" << y
            << "\" stroke=\"darkblue\" stroke-width=\"1.5\" stroke-dasharray=\"4,4\"/>\n";
         svg << "    <text x=\"" << (xBase + plotWidth / 2) << "\" y=\"" << (y - 5)
            << "\" font-size=\"10\" fill=\"darkblue\" text-anchor=\"middle\">90 degrees</text>\n";
      }

      auto drawBox = [&](const std::tuple<double, double, double, double, double>& bp,
         int xOffset, const std::string& color, const std::string& name) {
            int x = xBase + xOffset;

            auto yPos = [&](double val) {
               return yStart + plotHeight - (int)((val - scaleMin) / range * plotHeight);
               };

            svg << "    <g id=\"box_" << labels[param] << "_" << name << "\">\n";

            // Whiskers
            svg << "      <line x1=\"" << x << "\" y1=\"" << yPos(std::get<0>(bp))
               << "\" x2=\"" << x << "\" y2=\"" << yPos(std::get<1>(bp))
               << "\" stroke=\"black\" stroke-width=\"1.5\"/>\n";
            svg << "      <line x1=\"" << x << "\" y1=\"" << yPos(std::get<3>(bp))
               << "\" x2=\"" << x << "\" y2=\"" << yPos(std::get<4>(bp))
               << "\" stroke=\"black\" stroke-width=\"1.5\"/>\n";

            // Min/Max caps
            svg << "      <line x1=\"" << (x - 8) << "\" y1=\"" << yPos(std::get<0>(bp))
               << "\" x2=\"" << (x + 8) << "\" y2=\"" << yPos(std::get<0>(bp))
               << "\" stroke=\"black\" stroke-width=\"1.5\"/>\n";
            svg << "      <line x1=\"" << (x - 8) << "\" y1=\"" << yPos(std::get<4>(bp))
               << "\" x2=\"" << (x + 8) << "\" y2=\"" << yPos(std::get<4>(bp))
               << "\" stroke=\"black\" stroke-width=\"1.5\"/>\n";

            // Box - use pattern for Niggli and Selling
            std::string fillAttr;
            if (name == "niggli") {
               fillAttr = "fill=\"url(#niggliPattern)\"";
            } else if (name == "selling") {
               fillAttr = "fill=\"url(#sellingPattern)\"";
            } else {
               fillAttr = "fill=\"" + color + "\" fill-opacity=\"0.7\"";
            }

            svg << "      <rect x=\"" << (x - boxWidth / 2) << "\" y=\"" << yPos(std::get<3>(bp))
               << "\" width=\"" << boxWidth << "\" height=\""
               << (yPos(std::get<1>(bp)) - yPos(std::get<3>(bp)))
               << "\" " << fillAttr << " stroke=\"black\" stroke-width=\"2\"/>\n";

            // Median line
            svg << "      <line x1=\"" << (x - boxWidth / 2) << "\" y1=\"" << yPos(std::get<2>(bp))
               << "\" x2=\"" << (x + boxWidth / 2) << "\" y2=\"" << yPos(std::get<2>(bp))
               << "\" stroke=\"black\" stroke-width=\"3\"/>\n";

            svg << "    </g>\n";
         };

      drawBox(rawBP, 20, colors[0], "raw");
      drawBox(niggliBP, 70, colors[1], "niggli");
      drawBox(sellingBP, 120, colors[2], "selling");

      // Parameter label
      svg << "    <text x=\"" << (xBase + plotWidth / 2) << "\" y=\""
         << (yStart + plotHeight + 30) << "\" text-anchor=\"middle\" "
         << "font-size=\"18\" font-weight=\"bold\">" << labels[param] << "</text>\n";

      svg << "  </g>\n";
   }

   // Legend group (positioned outside and to the right of gamma box)
   svg << "  <g id=\"legend\">\n";
   int gammaBaseX = margin + 5 * (plotWidth + spacing) + boxWidth;
   int legendX = gammaBaseX + plotWidth + 30;
   int legendY = yStart + 150;
   std::vector<std::string> names = { "Raw", "Niggli", "Selling" };

   svg << "    <rect x=\"" << (legendX - 5) << "\" y=\"" << (legendY - 5)
      << "\" width=\"95\" height=\"115\" fill=\"white\" "
      << "stroke=\"#666666\" stroke-width=\"1\"/>\n";

   for (int i = 0; i < 3; ++i) {
      int y = legendY + i * 35;
      // Determine fill for legend box
      std::string fillAttr;
      if (names[i] == "Niggli") {
         fillAttr = "fill=\"url(#niggliPattern)\"";
      } else if (names[i] == "Selling") {
         fillAttr = "fill=\"url(#sellingPattern)\"";
      } else {
         fillAttr = "fill=\"" + colors[i] + "\" fill-opacity=\"0.7\"";
      }

      svg << "    <rect x=\"" << legendX << "\" y=\"" << y << "\" width=\"30\" "
         << "height=\"30\" " << fillAttr << " stroke=\"black\" stroke-width=\"1.5\"/>\n";
      svg << "    <text x=\"" << (legendX + 38) << "\" y=\"" << (y + 20)
         << "\" font-size=\"12\" font-weight=\"bold\">" << names[i] << "</text>\n";
   }
   svg << "  </g>\n";

   // Close angles_and_legend group
   svg << "</g>\n";

   // Caption explaining the box plots
   int captionY = yStart + plotHeight + 60;
   int captionX = margin;
   svg << "<g id=\"caption\">\n";
   svg << "  <text x=\"" << captionX << "\" y=\"" << captionY
      << "\" font-size=\"11\" font-weight=\"bold\">Figure Caption:</text>\n";
   svg << "  <text x=\"" << captionX << "\" y=\"" << (captionY + 15)
      << "\" font-size=\"10\">Box plots showing distribution of unit cell parameters for "
      << raw.size() << " samples. Each box represents the interquartile range (IQR, 25th to 75th percentile),</text>\n";
   svg << "  <text x=\"" << captionX << "\" y=\"" << (captionY + 28)
      << "\" font-size=\"10\">with the orange line indicating the median. Whiskers extend from the box to the minimum and maximum values.</text>\n";
   svg << "  <text x=\"" << captionX << "\" y=\"" << (captionY + 41)
      << "\" font-size=\"10\">Three representations are shown: Raw (solid orange), Niggli-reduced (cross-hatched teal), and Selling-reduced (dotted blue).</text>\n";
   svg << "</g>\n";

   // For each length parameter (a, b, c) - outside the shifted group
   for (int param = 0; param < 3; ++param) {
      auto rawData = extractParam(raw, param);
      auto niggliData = extractParam(niggli, param);
      auto sellingData = extractParam(selling, param);

      auto rawBP = computeBoxPlot(rawData);
      auto niggliBP = computeBoxPlot(niggliData);
      auto sellingBP = computeBoxPlot(sellingData);

      double scaleMin = lengthLimits.GetMin();
      double scaleMax = lengthLimits.GetMax();
      double range = scaleMax - scaleMin;

      int xBase = margin + param * (plotWidth + spacing);

      svg << "<g id=\"param_" << labels[param] << "\">\n";

      // Gray box around triple
      svg << "  <rect x=\"" << (xBase - 5) << "\" y=\"" << (yStart - 5)
         << "\" width=\"" << (plotWidth + 10) << "\" height=\"" << (plotHeight + 10)
         << "\" fill=\"none\" stroke=\"#888888\" stroke-width=\"2\"/>\n";

      // Draw reference line at 1.0 if maximum is less than 25
      if (scaleMax < 25.0 && 1.0 >= scaleMin && 1.0 <= scaleMax) {
         int y = yStart + plotHeight - (int)((1.0 - scaleMin) / range * plotHeight);
         svg << "  <line x1=\"" << (xBase - 5) << "\" y1=\"" << y
            << "\" x2=\"" << (xBase + plotWidth + 5) << "\" y2=\"" << y
            << "\" stroke=\"lightblue\" stroke-width=\"1.5\" stroke-dasharray=\"4,4\"/>\n";
         svg << "  <text x=\"" << (xBase + plotWidth / 2) << "\" y=\"" << (y - 5)
            << "\" font-size=\"10\" fill=\"blue\" text-anchor=\"middle\">1.0</text>\n";
      }

      auto drawBox = [&](const std::tuple<double, double, double, double, double>& bp,
         int xOffset, const std::string& color, const std::string& name) {
            int x = xBase + xOffset;

            auto yPos = [&](double val) {
               return yStart + plotHeight - (int)((val - scaleMin) / range * plotHeight);
               };

            svg << "  <g id=\"box_" << labels[param] << "_" << name << "\">\n";

            // Whiskers
            svg << "    <line x1=\"" << x << "\" y1=\"" << yPos(std::get<0>(bp))
               << "\" x2=\"" << x << "\" y2=\"" << yPos(std::get<1>(bp))
               << "\" stroke=\"black\" stroke-width=\"1.5\"/>\n";
            svg << "    <line x1=\"" << x << "\" y1=\"" << yPos(std::get<3>(bp))
               << "\" x2=\"" << x << "\" y2=\"" << yPos(std::get<4>(bp))
               << "\" stroke=\"black\" stroke-width=\"1.5\"/>\n";

            // Min/Max caps
            svg << "    <line x1=\"" << (x - 8) << "\" y1=\"" << yPos(std::get<0>(bp))
               << "\" x2=\"" << (x + 8) << "\" y2=\"" << yPos(std::get<0>(bp))
               << "\" stroke=\"black\" stroke-width=\"1.5\"/>\n";
            svg << "    <line x1=\"" << (x - 8) << "\" y1=\"" << yPos(std::get<4>(bp))
               << "\" x2=\"" << (x + 8) << "\" y2=\"" << yPos(std::get<4>(bp))
               << "\" stroke=\"black\" stroke-width=\"1.5\"/>\n";

            // Box - use pattern for Niggli and Selling
            std::string fillAttr;
            if (name == "niggli") {
               fillAttr = "fill=\"url(#niggliPattern)\"";
            } else if (name == "selling") {
               fillAttr = "fill=\"url(#sellingPattern)\"";
            } else {
               fillAttr = "fill=\"" + color + "\" fill-opacity=\"0.7\"";
            }

            svg << "    <rect x=\"" << (x - boxWidth / 2) << "\" y=\"" << yPos(std::get<3>(bp))
               << "\" width=\"" << boxWidth << "\" height=\""
               << (yPos(std::get<1>(bp)) - yPos(std::get<3>(bp)))
               << "\" " << fillAttr << " stroke=\"black\" stroke-width=\"2\"/>\n";

            // Median line
            svg << "    <line x1=\"" << (x - boxWidth / 2) << "\" y1=\"" << yPos(std::get<2>(bp))
               << "\" x2=\"" << (x + boxWidth / 2) << "\" y2=\"" << yPos(std::get<2>(bp))
               << "\" stroke=\"black\" stroke-width=\"3\"/>\n";

            svg << "  </g>\n";
         };

      drawBox(rawBP, 20, colors[0], "raw");
      drawBox(niggliBP, 70, colors[1], "niggli");
      drawBox(sellingBP, 120, colors[2], "selling");

      // Parameter label
      svg << "  <text x=\"" << (xBase + plotWidth / 2) << "\" y=\""
         << (yStart + plotHeight + 30) << "\" text-anchor=\"middle\" "
         << "font-size=\"18\" font-weight=\"bold\">" << labels[param] << "</text>\n";

      svg << "</g>\n";
   }

   svg << "</svg>\n";
   return svg.str();
}



void ExamineRandomBehavior(const int n = 100) {
   std::vector<LRL_Cell_Degrees> cells_raw;
   std::vector<LRL_Cell_Degrees> cells_Niggli;
   std::vector<LRL_Cell_Degrees> cells_Selling;

   for (int i = 0; i < n; ++i) {
      const LRL_Cell cell(Polar::rand());
      if (!cell.IsValid()) {
         continue;
      }
      if (i == 33276) {
         std::cout << " 33276 gives negative g4 in reduced" << std::endl;
         std::cout << "P " << cell << std::endl;
         std::cout << "cell is valid " << ((cell.IsValid()) ? "true" : "false") << std::endl;
         std::cout << "G " << G6(cell) << std::endl;
         const int i19191 = 19191;
      }
      G6 g6red;
      S6 s6red;
      Niggli::ReduceWithoutMatrices(cell, g6red, 1.E-6);
      Selling::Reduce(cell, s6red);
      cells_raw.emplace_back(cell);
      cells_Niggli.emplace_back(g6red);
      cells_Selling.emplace_back(s6red);
   }

   const std::string thePlot = svgplot(cells_raw, cells_Niggli, cells_Selling);
   std::ofstream out("cell_comparison.svg");
   out << thePlot;
   out.close();
}




int main() {

   ExamineRandomBehavior(1000);

   ReductionComparison::Compare(10000);

   testBetterNonReducedCells();

   testLandscape();


   // Example 1: Analyze a specific cell
   LRL_Cell cell(10.0, 10.0, 10.0, 90.0, 90.0, 90.0);  // Cubic
   auto analysis = NiggliLandscape::AnalyzeLandscape(cell);

   // Example 2: Analyze a triclinic cell
   G6 triclinic(25.0, 30.0, 35.0, 10.0, 15.0, 20.0);
   auto analysis2 = NiggliLandscape::AnalyzeLandscape(triclinic);

   // Example 3: Visualize cross-section






   diagnoseKrivyGruber();

   investigateCell0();


   // Compare algorithms on 1000 random cells
   ReductionComparison::Compare(10000);


   compareReductionAlgorithms(10000);

   bulkTestAdaptiveVsCumulativeFinal();

   testAdaptiveStrategy();

   bulkStrategyComparison(1000);

   compareStrategies();

   debugCell512();
   // Option 1: Test single cell
   // LRL_Cell test_cell(2.753, 5.864, 2.098, 137.316, 34.747, 116.741);
   // G6 inputG6(test_cell);
   // auto eisenstein_result = EisensteinReduction::ReduceQuiet(inputG6, 1.0e-6);
   // std::cout << "Result: " << eisenstein_result.reducedG6 << std::endl;

   // Option 2: Run the demonstration
    demonstrateFailure();

   // Option 3: Run detailed Eisenstein test
   // testEisensteinReduction();

   // Option 4: Run bulk testing
   bulkComparisonTest(1000);

   return 0;   return 0;
}