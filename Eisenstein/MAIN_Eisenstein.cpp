#include <iostream>
#include <cfloat>
#include <limits>
#include <iomanip>
#include <chrono>

#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "G6.h"
#include "Niggli.h"
#include "B4.h"
#include "EisensteinControls.h"
#include "EdgeTransformationMatrices.h"
#include "ProgramSetup.h"
#include "Eisenstein.h"
#include "ScoredReservoir.h"

static double GetTrace(const G6& g6) {
   return g6[0] + g6[1] + g6[2];
}

static std::tuple<double, G6, int> TestToConvergence(const LRL_Cell& cell, const int nmax,
   const std::vector<Matrix_3x3>& matrices, const double tolerance) {

   G6 best = cell;
   double minTrace = GetTrace(best);
   int iterations = 0;

   for (int loop = 0; loop < nmax; ++loop) {
      ++iterations;
      G6 candidateBest = best;
      double candidateMinTrace = minTrace;

      for (const auto& m : matrices) {
         const G6 transformed = m * best;
         if (!transformed.IsValid()) continue;
         const double trace = GetTrace(transformed);

         if (trace < candidateMinTrace) {
            candidateMinTrace = trace;
            candidateBest = transformed;
         }
      }

      if (std::abs(candidateMinTrace - minTrace) < tolerance) break;

      minTrace = candidateMinTrace;
      best = candidateBest;
   }

   return { minTrace, best, iterations };
}

struct ReductionResult {
   std::string method;
   double trace;
   G6 reduced;
   int iterations;
   double time_ms;
};

std::vector<ReductionResult> CompareReductionMethods(const LRL_Cell& cell, const int nmax, const double tolerance) {
   std::vector<ReductionResult> results;

   // Test Eisenstein (edge transforms only)
   {
      auto start = std::chrono::high_resolution_clock::now();
      const auto [edgeTrace, edgeG6, edgeCycles] =
         TestToConvergence(cell, nmax, GetSimpleEdgeTransforms(), tolerance);
      auto end = std::chrono::high_resolution_clock::now();
      double time_ms = std::chrono::duration<double, std::milli>(end - start).count();

      results.push_back({ "Eisenstein12", edgeTrace, edgeG6, edgeCycles, time_ms });
   }

   // Test Eisenstein24 (all 24 transforms)
   {
      auto start = std::chrono::high_resolution_clock::now();
      const auto [allTrace, allG6, allCycles] =
         TestToConvergence(cell, nmax, GetAllTransforms(), tolerance);
      auto end = std::chrono::high_resolution_clock::now();
      double time_ms = std::chrono::duration<double, std::milli>(end - start).count();

      results.push_back({ "Eisenstein24", allTrace, allG6, allCycles, time_ms });
   }

   // Test Niggli reduction directly on input
   {
      G6 niggliInput = cell;
      G6 niggliOut;

      auto start = std::chrono::high_resolution_clock::now();
      bool success = Niggli::ReduceWithoutMatrices(niggliInput, niggliOut, tolerance);
      auto end = std::chrono::high_resolution_clock::now();
      double time_ms = std::chrono::duration<double, std::milli>(end - start).count();

      double trace = GetTrace(niggliOut);
      results.push_back({ "Niggli", trace, niggliOut, success ? 1 : 0, time_ms });
   }

   return results;
}

struct DifferenceCase {
   LRL_Cell input;
   ReductionResult eisenstein12;
   ReductionResult eisenstein24;
   ReductionResult niggli;
   double trace_diff;
};

struct ComparisonSummary {
   int total_tests = 0;
   int eisenstein12_best = 0;
   int eisenstein24_best = 0;
   int niggli_best = 0;
   int ties = 0;
   double eisenstein12_total_time = 0.0;
   double eisenstein24_total_time = 0.0;
   double niggli_total_time = 0.0;
   double eisenstein12_total_iterations = 0;
   double eisenstein24_total_iterations = 0;
   double max_trace_difference = 0.0;
   std::vector<DifferenceCase> notable_differences;

   void addNotableDifference(const LRL_Cell& cell, const std::vector<ReductionResult>& results, double diff) {
      if (notable_differences.size() < 10) {  // Keep top 10
         notable_differences.push_back({ cell, results[0], results[1], results[2], diff });
         // Sort by trace difference, largest first
         std::sort(notable_differences.begin(), notable_differences.end(),
            [](const DifferenceCase& a, const DifferenceCase& b) {
               return a.trace_diff > b.trace_diff;
            });
      } else if (diff > notable_differences.back().trace_diff) {
         notable_differences.back() = { cell, results[0], results[1], results[2], diff };
         std::sort(notable_differences.begin(), notable_differences.end(),
            [](const DifferenceCase& a, const DifferenceCase& b) {
               return a.trace_diff > b.trace_diff;
            });
      }
   }
};

void UpdateSummary(ComparisonSummary& summary, const LRL_Cell& cell,
   const std::vector<ReductionResult>& results, const double tolerance) {
   summary.total_tests++;

   summary.eisenstein12_total_time += results[0].time_ms;
   summary.eisenstein24_total_time += results[1].time_ms;
   summary.niggli_total_time += results[2].time_ms;

   summary.eisenstein12_total_iterations += results[0].iterations;
   summary.eisenstein24_total_iterations += results[1].iterations;

   // Find minimum trace
   double min_trace = std::min({ results[0].trace, results[1].trace, results[2].trace });

   // Count how many achieved minimum
   int at_minimum = 0;
   if (std::abs(results[0].trace - min_trace) < tolerance) { summary.eisenstein12_best++; at_minimum++; }
   if (std::abs(results[1].trace - min_trace) < tolerance) { summary.eisenstein24_best++; at_minimum++; }
   if (std::abs(results[2].trace - min_trace) < tolerance) { summary.niggli_best++; at_minimum++; }

   if (at_minimum > 1) summary.ties++;

   // Track maximum difference
   double max_trace = std::max({ results[0].trace, results[1].trace, results[2].trace });
   double diff = max_trace - min_trace;
   summary.max_trace_difference = std::max(summary.max_trace_difference, diff);

   // Store notable differences (where methods disagree significantly)
   if (diff > tolerance * 10) {  // Significant difference threshold
      summary.addNotableDifference(cell, results, diff);
   }
}

void PrintNotableDifferences(const ComparisonSummary& summary) {
   if (summary.notable_differences.empty()) {
      std::cout << "\nNo notable differences found between methods.\n";
      return;
   }

   std::cout << "\n" << std::string(100, '=') << "\n";
   std::cout << "NOTABLE DIFFERENCES (Top " << summary.notable_differences.size() << " cases)\n";
   std::cout << std::string(100, '=') << "\n";

   int case_num = 1;
   for (const auto& diff_case : summary.notable_differences) {
      std::cout << "\nCase #" << case_num++ << " - Trace difference: "
         << std::fixed << std::setprecision(6) << diff_case.trace_diff << "\n";
      std::cout << std::string(100, '-') << "\n";
      std::cout << "Input: " << diff_case.input << "\n\n";

      std::cout << std::left << std::setw(20) << "Method"
         << std::right << std::setw(15) << "Trace"
         << std::setw(12) << "Iterations"
         << std::setw(12) << "Time (ms)" << "\n";
      std::cout << std::string(60, '-') << "\n";

      for (const auto* r : { &diff_case.eisenstein12, &diff_case.eisenstein24, &diff_case.niggli }) {
         std::cout << std::left << std::setw(20) << r->method
            << std::right << std::fixed << std::setprecision(6)
            << std::setw(15) << r->trace
            << std::setw(12) << r->iterations
            << std::setw(12) << std::setprecision(3) << r->time_ms << "\n";
      }

      std::cout << "\nResults:\n";
      std::cout << "  Eisenstein12:   " << diff_case.eisenstein12.reduced << "\n";
      std::cout << "  Eisenstein24: " << diff_case.eisenstein24.reduced << "\n";
      std::cout << "  Niggli:       " << diff_case.niggli.reduced << "\n";

      // Determine which is best
      double min_trace = std::min({ diff_case.eisenstein12.trace,
                                   diff_case.eisenstein24.trace,
                                   diff_case.niggli.trace });
      std::cout << "\nBest result: ";
      if (std::abs(diff_case.eisenstein12.trace - min_trace) < 1e-6) std::cout << "Eisenstein12 ";
      if (std::abs(diff_case.eisenstein24.trace - min_trace) < 1e-6) std::cout << "Eisenstein24 ";
      if (std::abs(diff_case.niggli.trace - min_trace) < 1e-6) std::cout << "Niggli";
      std::cout << "\n";
   }
   std::cout << std::string(100, '=') << "\n";
}

void PrintSummary(const ComparisonSummary& summary) {
   std::cout << "\n" << std::string(80, '=') << "\n";
   std::cout << "REDUCTION METHOD COMPARISON SUMMARY\n";
   std::cout << std::string(80, '=') << "\n";
   std::cout << "Total tests: " << summary.total_tests << "\n\n";

   std::cout << "Best trace achieved:\n";
   std::cout << "  Eisenstein12:   " << summary.eisenstein12_best
      << " (" << std::fixed << std::setprecision(1)
      << (100.0 * summary.eisenstein12_best / summary.total_tests) << "%)\n";
   std::cout << "  Eisenstein24: " << summary.eisenstein24_best
      << " (" << (100.0 * summary.eisenstein24_best / summary.total_tests) << "%)\n";
   std::cout << "  Niggli:       " << summary.niggli_best
      << " (" << (100.0 * summary.niggli_best / summary.total_tests) << "%)\n";
   std::cout << "  Ties:         " << summary.ties << "\n\n";

   std::cout << "Average iterations:\n";
   std::cout << "  Eisenstein12:   " << std::setprecision(1)
      << (summary.eisenstein12_total_iterations / summary.total_tests) << "\n";
   std::cout << "  Eisenstein24: "
      << (summary.eisenstein24_total_iterations / summary.total_tests) << "\n\n";

   std::cout << "Average time (ms):\n";
   std::cout << "  Eisenstein12:   " << std::setprecision(3)
      << (summary.eisenstein12_total_time / summary.total_tests) << "\n";
   std::cout << "  Eisenstein24: "
      << (summary.eisenstein24_total_time / summary.total_tests) << "\n";
   std::cout << "  Niggli:       "
      << (summary.niggli_total_time / summary.total_tests) << "\n\n";

   std::cout << "Total time (s):\n";
   std::cout << "  Eisenstein12:   " << std::setprecision(2)
      << (summary.eisenstein12_total_time / 1000.0) << "\n";
   std::cout << "  Eisenstein24: "
      << (summary.eisenstein24_total_time / 1000.0) << "\n";
   std::cout << "  Niggli:       "
      << (summary.niggli_total_time / 1000.0) << "\n\n";

   std::cout << "Maximum trace difference: " << std::setprecision(6)
      << summary.max_trace_difference << "\n";
   std::cout << std::string(80, '=') << "\n";

   // Print notable differences
   PrintNotableDifferences(summary);
}

int main() {
   std::cout << "; Eisenstein Reduction" << std::endl;

   const int nmax = 500;
   const double tolerance = 1.0E-8;

   try {
      EisenControls controls;
      const BasicProgramInput<EisenControls> dc_setup("Eisenstein", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      ComparisonSummary summary;

      int count = 0;
      for (const auto& input : dc_setup.getInputList()) {
         ++count;
         const LRL_Cell cell = input.getCell();

         std::cout << "P ";
         for (size_t i = 0; i < 6; ++i) {
            std::cout << std::setprecision(16) << std::setw(15) << LRL_Cell_Degrees(cell)[i] << " ";
         }
         std::cout << std::endl;

         const G6 g6cell = cell;
         std::cout << "G6 ";
         for (size_t i = 0; i < 6; ++i) {
            std::cout << std::setprecision(16) << std::setw(15) << g6cell[i] << " ";
         }
         std::cout << std::endl;;

         const double trace = g6cell[0] + g6cell[1] + g6cell[2];
         std::cout << "trace = "  << std::setprecision(16) << std::setw(15) << trace << std::endl << std::endl;

         std::cout << std::endl;;
         auto results = CompareReductionMethods(cell, nmax, tolerance);
         UpdateSummary(summary, cell, results, tolerance);
      }


      PrintSummary(summary);
      return 0;
   }
   catch (const std::exception& e) {
      std::cout << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}
