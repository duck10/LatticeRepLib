#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <chrono>

#include "TestNiggliControls.h"
#include "ProgramSetup.h"
#include "NCDist_McCoy.h"
#include "NCDist.h"
#include "NCDist_HJB.h"
#include "S6M_SellingReduce.h"
#include "Polar.h"
#include "LRL_Cell_Degrees.h"
//#include "KrivyGruber.h"
//#include "KrivyGruber_LCA.h"
//#include "KrivyGruber_LCA_oldforcycling.h"
//#include "KrivyGruber_lca_showcycles.h"
#include "TestKrivyGruber.h"
#include "KrivyGruberG6.h"
#include "KrivyGruber_LCA_v01_NoCycling_step1label.h"
#include "Niggli.h"
#include "IsNiggli_KG.h"
#include "B4.h"
#include "Cycledetection.h"
#include "EisenstenConditions.h"


#include "ReductionAnalyzer.h"


inline LRL_Cell operator*(const Matrix_3x3& matrix, const LRL_Cell& cell) {
   // Convert cell to B4 representation
   B4 b4Cell(cell);

   // Apply transformation in B4 space
   B4 transformedB4 = matrix * b4Cell;

   // Convert back to LRL_Cell
   return LRL_Cell(transformedB4);
}

// Helper function: Format G6 with high precision for small values
std::string FormatG6WithPrecision(const G6& g6) {
   std::ostringstream oss;
   oss << std::fixed;

   for (size_t i = 0; i < 6; ++i) {
      if (i > 0) oss << "  ";

      // Use higher precision for small values
      if (std::abs(g6[i]) < 0.1) {
         oss << std::setprecision(12) << std::setw(16) << g6[i];
      } else {
         oss << std::setprecision(6) << std::setw(10) << g6[i];
      }
   }

   return oss.str();
}

// ===================================================================
// Tolerance Comparison Testing
// ===================================================================

struct ToleranceComparisonStats {
   int totalTests = 0;

   // Stats for each tolerance value
   struct ToleranceResult {
      double tolerance;
      int failures = 0;
      int maxIterations = 0;
      int totalIterations = 0;
      double totalTime = 0.0;
      G6 maxIterInput;

      // Failure tracking
      std::vector<G6> failureInputs;
      std::vector<int> failureIterations;
   };

   ToleranceResult tol1;  // tolerance = 0.0
   ToleranceResult tol2;  // tolerance = 1.0e-10
   ToleranceResult tol3;  // tolerance = 1.0e-6

   // Comparison results
   int identical_01 = 0;  // tol=0 vs tol=1e-10
   int identical_02 = 0;  // tol=0 vs tol=1e-6
   int identical_12 = 0;  // tol=1e-10 vs tol=1e-6
   double maxDiff_01 = 0.0;
   double maxDiff_02 = 0.0;
   double maxDiff_12 = 0.0;
   G6 maxDiffInput_01;
   G6 maxDiffInput_02;
   G6 maxDiffInput_12;
};

void CompareKrivyGruberLCA_Tolerances(const G6& inputG6, ToleranceComparisonStats& stats, bool verbose = false) {
   stats.totalTests++;

   if (verbose) {
      std::cout << "\n" << std::string(70, '=') << std::endl;
      std::cout << "Testing KrivyGruber_LCA with multiple tolerances" << std::endl;
      std::cout << "Input: " << LRL_Cell_Degrees(inputG6) << std::endl;
      std::cout << "Input G6: " << FormatG6WithPrecision(inputG6) << std::endl;
      std::cout << "Input G6 with full precision:" << std::endl;
      std::cout << std::fixed << std::setprecision(20);
      std::cout << "  g1 = " << inputG6[0] << std::endl;
      std::cout << "  g2 = " << inputG6[1] << std::endl;
      std::cout << "  g3 = " << inputG6[2] << std::endl;
      std::cout << "  g4 = " << inputG6[3] << std::endl;
      std::cout << "  g5 = " << inputG6[4] << std::endl;
      std::cout << "  g6 = " << inputG6[5] << std::endl;
   }

   // Test with tolerance = 0.0
   auto start1 = std::chrono::high_resolution_clock::now();
   KrivyGruber_LCA::ReductionResult result1 = KrivyGruber_LCA::ReduceWithTransformation_LCA(inputG6, 0.0, verbose);

   std::vector<NiggliViolation> violations;
   const double tolerance = 0.0;

   ReductionAnalyzer::Analysis analysis1 = ReductionAnalyzer::Analyze(result1.reducedG6, 0.0, 1e-13);
   std::cout << analysis1 << std::endl;
   auto end1 = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double, std::micro> duration1 = end1 - start1;

   stats.tol1.tolerance = 0.0;
   stats.tol1.totalTime += duration1.count();
   stats.tol1.totalIterations += result1.iterations;
   if (!result1.success) {
      stats.tol1.failures++;
      if (stats.tol1.failureInputs.size() < 10) {  // Keep first 10 failures
         stats.tol1.failureInputs.push_back(inputG6);
         stats.tol1.failureIterations.push_back(result1.iterations);
      }
   }
   if (result1.iterations > stats.tol1.maxIterations) {
      stats.tol1.maxIterations = result1.iterations;
      stats.tol1.maxIterInput = inputG6;
   }

   // Test with tolerance = 1.0e-10
   auto start2 = std::chrono::high_resolution_clock::now();
   KrivyGruber_LCA::ReductionResult result2 = KrivyGruber_LCA::ReduceWithTransformation_LCA(inputG6, 1.0e-10, verbose);
   auto end2 = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double, std::micro> duration2 = end2 - start2;

   stats.tol2.tolerance = 1.0e-10;
   stats.tol2.totalTime += duration2.count();
   stats.tol2.totalIterations += result2.iterations;
   if (!result2.success) {
      stats.tol2.failures++;
      if (stats.tol2.failureInputs.size() < 10) {
         stats.tol2.failureInputs.push_back(inputG6);
         stats.tol2.failureIterations.push_back(result2.iterations);
      }
   }
   if (result2.iterations > stats.tol2.maxIterations) {
      stats.tol2.maxIterations = result2.iterations;
      stats.tol2.maxIterInput = inputG6;
   }

   // Test with tolerance = 1.0e-6
   auto start3 = std::chrono::high_resolution_clock::now();
   KrivyGruber_LCA::ReductionResult result3 = KrivyGruber_LCA::ReduceWithTransformation_LCA(inputG6, 1.0e-6, verbose);
   auto end3 = std::chrono::high_resolution_clock::now();
   std::chrono::duration<double, std::micro> duration3 = end3 - start3;

   stats.tol3.tolerance = 1.0e-6;
   stats.tol3.totalTime += duration3.count();
   stats.tol3.totalIterations += result3.iterations;
   if (!result3.success) {
      stats.tol3.failures++;
      if (stats.tol3.failureInputs.size() < 10) {
         stats.tol3.failureInputs.push_back(inputG6);
         stats.tol3.failureIterations.push_back(result3.iterations);
      }
   }
   if (result3.iterations > stats.tol3.maxIterations) {
      stats.tol3.maxIterations = result3.iterations;
      stats.tol3.maxIterInput = inputG6;
   }

   // Compare G6 results (only if all succeeded)
   if (result1.success && result2.success && result3.success) {
      bool identical01 = true;
      bool identical02 = true;
      bool identical12 = true;
      double maxDiff01 = 0.0;
      double maxDiff02 = 0.0;
      double maxDiff12 = 0.0;

      for (size_t i = 0; i < 6; ++i) {
         double diff01 = std::abs(result1.reducedG6[i] - result2.reducedG6[i]);
         double diff02 = std::abs(result1.reducedG6[i] - result3.reducedG6[i]);
         double diff12 = std::abs(result2.reducedG6[i] - result3.reducedG6[i]);

         maxDiff01 = std::max(maxDiff01, diff01);
         maxDiff02 = std::max(maxDiff02, diff02);
         maxDiff12 = std::max(maxDiff12, diff12);

         if (diff01 > 0.0) identical01 = false;
         if (diff02 > 0.0) identical02 = false;
         if (diff12 > 0.0) identical12 = false;
      }

      if (identical01) stats.identical_01++;
      if (identical02) stats.identical_02++;
      if (identical12) stats.identical_12++;

      if (maxDiff01 > stats.maxDiff_01) {
         stats.maxDiff_01 = maxDiff01;
         stats.maxDiffInput_01 = inputG6;
      }
      if (maxDiff02 > stats.maxDiff_02) {
         stats.maxDiff_02 = maxDiff02;
         stats.maxDiffInput_02 = inputG6;
      }
      if (maxDiff12 > stats.maxDiff_12) {
         stats.maxDiff_12 = maxDiff12;
         stats.maxDiffInput_12 = inputG6;
      }
   }

   if (verbose) {
      std::cout << "\nResults:" << std::endl;
      std::cout << "  tol=0.0:     " << result1.iterations << " iter, "
         << duration1.count() << " us, " << (result1.success ? "SUCCESS" : "FAILED") << std::endl;
      std::cout << "    G6: " << FormatG6WithPrecision(result1.reducedG6) << std::endl;
      std::cout << "  tol=1e-10:   " << result2.iterations << " iter, "
         << duration2.count() << " us, " << (result2.success ? "SUCCESS" : "FAILED") << std::endl;
      std::cout << "    G6: " << FormatG6WithPrecision(result2.reducedG6) << std::endl;
      std::cout << "  tol=1e-6:    " << result3.iterations << " iter, "
         << duration3.count() << " us, " << (result3.success ? "SUCCESS" : "FAILED") << std::endl;
      std::cout << "    G6: " << FormatG6WithPrecision(result3.reducedG6) << std::endl;

      if (result1.success && result2.success && result3.success) {
         bool identical01 = true;
         bool identical02 = true;
         bool identical12 = true;
         double maxDiff01 = 0.0;
         double maxDiff02 = 0.0;
         double maxDiff12 = 0.0;

         for (size_t i = 0; i < 6; ++i) {
            double diff01 = std::abs(result1.reducedG6[i] - result2.reducedG6[i]);
            double diff02 = std::abs(result1.reducedG6[i] - result3.reducedG6[i]);
            double diff12 = std::abs(result2.reducedG6[i] - result3.reducedG6[i]);
            maxDiff01 = std::max(maxDiff01, diff01);
            maxDiff02 = std::max(maxDiff02, diff02);
            maxDiff12 = std::max(maxDiff12, diff12);
            if (diff01 > 0.0) identical01 = false;
            if (diff02 > 0.0) identical02 = false;
            if (diff12 > 0.0) identical12 = false;
         }

         std::cout << "\nComparisons:" << std::endl;
         std::cout << "  Max diff (0.0 vs 1e-10): " << maxDiff01 << (identical01 ? " [IDENTICAL]" : "") << std::endl;
         std::cout << "  Max diff (0.0 vs 1e-6):  " << maxDiff02 << (identical02 ? " [IDENTICAL]" : "") << std::endl;
         std::cout << "  Max diff (1e-10 vs 1e-6): " << maxDiff12 << (identical12 ? " [IDENTICAL]" : "") << std::endl;
      }
   }
}

void PrintToleranceComparisonSummary(const ToleranceComparisonStats& stats) {
   std::cout << "\n" << std::string(70, '=') << std::endl;
   std::cout << "TOLERANCE COMPARISON SUMMARY - KrivyGruber_LCA" << std::endl;
   std::cout << std::string(70, '=') << std::endl;
   std::cout << "Total tests: " << stats.totalTests << std::endl;

   auto printResult = [&](const ToleranceComparisonStats::ToleranceResult& r, const std::string& label) {
      std::cout << "\n" << label << " (tolerance = " << r.tolerance << "):" << std::endl;
      std::cout << "  Failures:          " << r.failures << " ("
         << (stats.totalTests > 0 ? 100.0 * r.failures / stats.totalTests : 0) << "%)" << std::endl;
      std::cout << "  Max iterations:    " << r.maxIterations << std::endl;
      if (r.maxIterations > 0) {
         std::cout << "    Input: " << FormatG6WithPrecision(r.maxIterInput) << std::endl;
      }

      // Print failure details
      if (r.failures > 0 && !r.failureInputs.empty()) {
         std::cout << "  Failure details (showing up to " << r.failureInputs.size() << "):" << std::endl;
         for (size_t i = 0; i < r.failureInputs.size(); ++i) {
            std::cout << "    #" << (i + 1) << ": " << r.failureIterations[i] << " iterations"
               << (r.failureIterations[i] >= 1000 ? " [HIT MAX]" : "") << std::endl;
            std::cout << "        Input:  " << FormatG6WithPrecision(r.failureInputs[i]) << std::endl;
         }
      }

      std::cout << "  Avg iterations:    " << (stats.totalTests > 0 ? (double)r.totalIterations / stats.totalTests : 0) << std::endl;
      std::cout << "  Total time:        " << r.totalTime / 1000.0 << " ms" << std::endl;
      std::cout << "  Avg time:          " << (stats.totalTests > 0 ? r.totalTime / stats.totalTests : 0) << " us/test" << std::endl;
      };

   printResult(stats.tol1, "Tolerance = 0.0");
   printResult(stats.tol2, "Tolerance = 1.0e-10");
   printResult(stats.tol3, "Tolerance = 1.0e-6");

   std::cout << "\n" << std::string(70, '-') << std::endl;
   std::cout << "G6 COMPARISON RESULTS:" << std::endl;
   std::cout << std::string(70, '-') << std::endl;

   int successfulTests = stats.totalTests - std::max({ stats.tol1.failures, stats.tol2.failures, stats.tol3.failures });

   std::cout << "\ntol=0.0 vs tol=1e-10:" << std::endl;
   std::cout << "  Identical G6:      " << stats.identical_01 << " ("
      << (successfulTests > 0 ? 100.0 * stats.identical_01 / successfulTests : 0) << "%)" << std::endl;
   std::cout << "  Max G6 difference: " << stats.maxDiff_01 << std::endl;
   if (stats.maxDiff_01 > 0) {
      std::cout << "    Input: " << FormatG6WithPrecision(stats.maxDiffInput_01) << std::endl;
   }

   std::cout << "\ntol=0.0 vs tol=1e-6:" << std::endl;
   std::cout << "  Identical G6:      " << stats.identical_02 << " ("
      << (successfulTests > 0 ? 100.0 * stats.identical_02 / successfulTests : 0) << "%)" << std::endl;
   std::cout << "  Max G6 difference: " << stats.maxDiff_02 << std::endl;
   if (stats.maxDiff_02 > 0) {
      std::cout << "    Input: " << FormatG6WithPrecision(stats.maxDiffInput_02) << std::endl;
   }

   std::cout << "\ntol=1e-10 vs tol=1e-6:" << std::endl;
   std::cout << "  Identical G6:      " << stats.identical_12 << " ("
      << (successfulTests > 0 ? 100.0 * stats.identical_12 / successfulTests : 0) << "%)" << std::endl;
   std::cout << "  Max G6 difference: " << stats.maxDiff_12 << std::endl;
   if (stats.maxDiff_12 > 0) {
      std::cout << "    Input: " << FormatG6WithPrecision(stats.maxDiffInput_12) << std::endl;
   }

   std::cout << std::string(70, '=') << std::endl;
}

// ===================================================================
// Main Program
// ===================================================================

int main(int argc, char* argv[]) {
   //for (size_t i = 0; i < 100000; ++i) {
   //   const G6 g6(Polar::rand());
   //   if (!g6.IsValid() || !LRL_Cell(g6).IsValid()) continue;
   //   const double initialVolume = LRL_Cell(g6).Volume();
   //   const ReductionResult result = KrivyGruberG6::ReduceF(g6);

   //   // Skip if reduction failed
   //   if (result.status != ReductionStatus::Success) continue;

   //   const double finalVolume = LRL_Cell(result.reducedCell).Volume();
   //   const double volumeDifference = initialVolume - finalVolume;
   //   if (volumeDifference / initialVolume > 1.0E-10)
   //   {
   //      G6 red;
   //      Niggli::Reduce(g6, red);
   //      std::cout << "cycle " << i << std::endl;
   //      std::cout << "initial G6   " << g6 << std::endl;
   //      std::cout << "reduced G6   " << result.reducedCell << std::endl;
   //      std::cout << "initial cell " << LRL_Cell_Degrees(g6) << std::endl;
   //      std::cout << "reduced cell " << LRL_Cell_Degrees(result.reducedCell) << std::endl;
   //      std::cout << "reduced differences ";
   //      std::cout << FormatG6WithPrecision(result.reducedCell - red) << std::endl;
   //      std::cout << "NiggliReduced " << red << std::endl;
   //      std::cout << "initial and final volumes " << initialVolume << "  " << finalVolume << std::endl;
   //      std::cout << "Fractional volume difference " << volumeDifference << std::endl;
   //      std::cout << std::endl;
   //   }
   //}

   //return KrivyGruberTest::run();
   const G6 testg6(G6::rand());
   const ReductionResult answer = KG::Reduce(testg6, 0, true);
   std::cout << answer.reducedCell << std::endl;

   TemplateControls controls;
   const BasicProgramInput<TemplateControls> dc_setup("TEMPLATE", controls);
   if (dc_setup.getInputList().empty()) {
      throw std::runtime_error("; No input vectors provided");
   }

   //ToleranceComparisonStats tolStats;
   //// *** ADD THESE THREE LINES ***
   //tolStats.tol1.tolerance = 0.0;
   //tolStats.tol2.tolerance = 1.0e-10;
   //tolStats.tol3.tolerance = 1.0e-6;

   //std::cout << "\n========================================" << std::endl;
   //std::cout << "KrivyGruber_LCA Tolerance Comparison Test" << std::endl;
   //std::cout << "Testing with tolerances: 0.0, 1.0e-10, 1.0e-6" << std::endl;
   //std::cout << "========================================\n" << std::endl;

   //for (const auto& input : dc_setup.getInputList()) {
   //   std::cout << CheckEisensteinConditions(input.getCell());
   //   const G6 g6in = input.getCell();
   //   CompareKrivyGruberLCA_Tolerances(g6in, tolStats, true);  // verbose=false
   //}

   for (const auto& input : dc_setup.getInputList()) {
      const auto out =KG::Reduce(input.getCell(),0,false);
      std::cout << out.reducedCell << std::endl;
   }

   //PrintToleranceComparisonSummary(tolStats);
   return 0;
}