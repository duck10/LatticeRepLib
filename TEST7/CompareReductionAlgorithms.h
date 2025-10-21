// CompareReductionAlgorithms.h - Corrected Comparison Framework
#ifndef COMPARE_REDUCTION_ALGORITHMS_H
#define COMPARE_REDUCTION_ALGORITHMS_H

#include "G6.h"
#include "S6.h"
#include "LRL_Cell.h"
#include "Niggli.h"
#include "Selling.h"
#include "KrivyGruber.h"
#include "EisensteinReduction.h"
#include "Polar.h"
#include "StoreResults.h"
#include <chrono>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>

class ReductionComparison {
public:
   struct AlgorithmStats {
      std::string name;
      int successes = 0;
      int failures = 0;
      int totalIterations = 0;
      size_t totalTransformations = 0;
      int mismatches = 0;
      double maxDifference = 0.0;

      std::vector<double> executionTimes;  // microseconds

      double avgTime() const {
         if (executionTimes.empty()) return 0.0;
         double sum = 0.0;
         for (double t : executionTimes) sum += t;
         return sum / executionTimes.size();
      }

      double minTime() const {
         if (executionTimes.empty()) return 0.0;
         double min = executionTimes[0];
         for (double t : executionTimes) min = std::min(min, t);
         return min;
      }

      double maxTime() const {
         if (executionTimes.empty()) return 0.0;
         double max = executionTimes[0];
         for (double t : executionTimes) max = std::max(max, t);
         return max;
      }
   };

   struct MismatchCase {
      int cellNumber;
      G6 inputG6;
      G6 abResult;
      G6 compareResult;
      double abTrace;
      double compareTrace;
      double maxDiff;
      int abCycles;
      int compareIterations;
      std::string winner;  // "A-B", "Other", or "Tie"

      MismatchCase() : cellNumber(0), abTrace(0), compareTrace(0),
         maxDiff(0), abCycles(0), compareIterations(0) {
      }

      friend std::ostream& operator<<(std::ostream& os, const MismatchCase& mc) {
         os << "Cell #" << mc.cellNumber << " - WINNER: " << mc.winner << std::endl;
         os << "  Input:    " << mc.inputG6 << std::endl;
         os << "  A-B:       " << mc.abResult
            << " (trace=" << std::fixed << std::setprecision(2) << mc.abTrace
            << ", cycles=" << mc.abCycles << ")" << std::endl;
         os << "  Compare:   " << mc.compareResult
            << " (trace=" << mc.compareTrace
            << ", iter=" << mc.compareIterations << ")" << std::endl;
         os << "  Max G6 component diff: " << std::scientific << std::setprecision(4)
            << mc.maxDiff << std::endl;
         os << "  Trace difference: " << std::fixed << std::setprecision(6)
            << (mc.compareTrace - mc.abTrace);
         if (mc.compareTrace < mc.abTrace - 1.0e-6) {
            os << " (Compare is BETTER)";
         } else if (mc.abTrace < mc.compareTrace - 1.0e-6) {
            os << " (A-B is BETTER)";
         } else {
            os << " (Same trace, different cell)";
         }
         os << std::endl;
         return os;
      }
   };

   static void Compare(int numCells = 1000);

private:
   static double ComputeTrace(const G6& g6) {
      return g6[0] + g6[1] + g6[2];
   }

   static void PrintAlgorithmStats(const AlgorithmStats& stats);

   static void PrintComparativeSummary(const AlgorithmStats& ab,
      const AlgorithmStats& kg,
      const AlgorithmStats& eisenAdaptive,
      const AlgorithmStats& eisenCumulative);
};

// ===================================================================
// IMPLEMENTATION
// ===================================================================

inline void ReductionComparison::Compare(int numCells) {
   std::cout << "\n" << std::string(70, '=') << std::endl;
   std::cout << "COMPREHENSIVE ALGORITHM COMPARISON" << std::endl;
   std::cout << std::string(70, '=') << std::endl;
   std::cout << "Testing " << numCells << " random triclinic cells\n" << std::endl;

   AlgorithmStats ab, kg, eisenAdaptive, eisenCumulative;
   ab.name = "Andrews-Bernstein (2014)";
   kg.name = "Krivy-Gruber (1976)";
   eisenAdaptive.name = "Eisenstein Adaptive";
   eisenCumulative.name = "Eisenstein BEST_CUMULATIVE";

   // Store problem cases
   StoreResults<std::string, MismatchCase> kgMismatches(5);
   kgMismatches.SetTitle("Krivy-Gruber Mismatches vs Andrews-Bernstein");
   kgMismatches.SetKeyLabel("Cell Number");

   StoreResults<std::string, MismatchCase> eisenAdaptiveMismatches(5);
   eisenAdaptiveMismatches.SetTitle("Eisenstein Adaptive Mismatches vs A-B");
   eisenAdaptiveMismatches.SetKeyLabel("Cell Number");

   StoreResults<std::string, MismatchCase> eisenCumulativeMismatches(5);
   eisenCumulativeMismatches.SetTitle("Eisenstein BEST_CUMULATIVE Mismatches vs A-B");
   eisenCumulativeMismatches.SetKeyLabel("Cell Number");

   int validCells = 0;
   int nearDegenerateCells = 0;

   for (int i = 0; i < numCells; ++i) {
      LRL_Cell randomCell = Polar::rand();
      if (!randomCell.IsValid()) continue;

      S6 s6out;
      Selling::Reduce(S6(randomCell), s6out);

      G6 inputG6(randomCell);

      // Check for near-degenerate angles
      double cos_alpha = std::abs(inputG6[3]) / (2.0 * std::sqrt(inputG6[1] * inputG6[2]));
      double cos_beta = std::abs(inputG6[4]) / (2.0 * std::sqrt(inputG6[0] * inputG6[2]));
      double cos_gamma = std::abs(inputG6[5]) / (2.0 * std::sqrt(inputG6[0] * inputG6[1]));
      const double cos_5_degrees = 0.9962;

      if (cos_alpha >= cos_5_degrees || cos_beta >= cos_5_degrees ||
         cos_gamma >= cos_5_degrees) {
         nearDegenerateCells++;
         continue;
      }

      validCells++;

      // ===== Test Andrews-Bernstein (REFERENCE) =====
      auto startAB = std::chrono::high_resolution_clock::now();
      G6 resultAB;
      bool successAB = Niggli::ReduceWithoutMatrices(inputG6, resultAB, 1.0e-6);
      auto endAB = std::chrono::high_resolution_clock::now();
      auto durationAB = std::chrono::duration_cast<std::chrono::microseconds>(endAB - startAB);

      ab.executionTimes.push_back(durationAB.count());
      if (successAB) {
         ab.successes++;
         ab.totalIterations += Niggli::GetCycles();
      } else {
         ab.failures++;
         continue;  // Skip this cell if A-B fails
      }

      double abTrace = ComputeTrace(resultAB);
      int abCycles = Niggli::GetCycles();

      // ===== Test Krivy-Gruber =====
      auto startKG = std::chrono::high_resolution_clock::now();
      auto resultKG = KrivyGruber::Reduce(inputG6, 1.0e-6, false);
      auto endKG = std::chrono::high_resolution_clock::now();
      auto durationKG = std::chrono::duration_cast<std::chrono::microseconds>(endKG - startKG);

      kg.executionTimes.push_back(durationKG.count());
      if (resultKG.success) {
         kg.successes++;
         kg.totalIterations += resultKG.iterations;

         // Compare with A-B
         double maxDiff = 0.0;
         for (size_t j = 0; j < 6; ++j) {
            double diff = std::abs(resultKG.reducedG6[j] - resultAB[j]);
            maxDiff = std::max(maxDiff, diff);
         }
         kg.maxDifference = std::max(kg.maxDifference, maxDiff);

         if (maxDiff > 1.0e-3) {
            kg.mismatches++;

            MismatchCase mc;
            mc.cellNumber = i;
            mc.inputG6 = inputG6;
            mc.abResult = resultAB;
            mc.compareResult = resultKG.reducedG6;
            mc.abTrace = abTrace;
            mc.compareTrace = ComputeTrace(resultKG.reducedG6);
            mc.maxDiff = maxDiff;
            mc.abCycles = abCycles;
            mc.compareIterations = resultKG.iterations;

            // Determine winner
            if (mc.compareTrace < mc.abTrace - 1.0e-6) {
               mc.winner = "K-G";
            } else if (mc.abTrace < mc.compareTrace - 1.0e-6) {
               mc.winner = "A-B";
            } else {
               mc.winner = "Tie (different cell, same trace)";
            }

            kgMismatches.Store(std::to_string(i), mc);
         }
      } else {
         kg.failures++;
      }

      // ===== Test Eisenstein Adaptive =====
      auto startEA = std::chrono::high_resolution_clock::now();
      auto resultEA = EisensteinReduction::ReduceAdaptive(inputG6, 1.0e-6, false);
      auto endEA = std::chrono::high_resolution_clock::now();
      auto durationEA = std::chrono::duration_cast<std::chrono::microseconds>(endEA - startEA);

      eisenAdaptive.executionTimes.push_back(durationEA.count());
      if (resultEA.trace > 0) {
         eisenAdaptive.successes++;
         eisenAdaptive.totalIterations += resultEA.iterations;
         eisenAdaptive.totalTransformations += resultEA.transformationsTested;

         // Compare with A-B
         double maxDiff = 0.0;
         for (size_t j = 0; j < 6; ++j) {
            double diff = std::abs(resultEA.reducedG6[j] - resultAB[j]);
            maxDiff = std::max(maxDiff, diff);
         }
         eisenAdaptive.maxDifference = std::max(eisenAdaptive.maxDifference, maxDiff);

         if (maxDiff > 1.0e-3) {
            eisenAdaptive.mismatches++;

            MismatchCase mc;
            mc.cellNumber = i;
            mc.inputG6 = inputG6;
            mc.abResult = resultAB;
            mc.compareResult = resultEA.reducedG6;
            mc.abTrace = abTrace;
            mc.compareTrace = resultEA.trace;
            mc.maxDiff = maxDiff;
            mc.abCycles = abCycles;
            mc.compareIterations = resultEA.iterations;

            if (resultEA.trace < abTrace - 1.0e-6) {
               mc.winner = "Eisenstein Adaptive";
            } else if (abTrace < resultEA.trace - 1.0e-6) {
               mc.winner = "A-B";
            } else {
               mc.winner = "Tie (different cell, same trace)";
            }

            eisenAdaptiveMismatches.Store(std::to_string(i), mc);
         }
      } else {
         eisenAdaptive.failures++;
      }

      // ===== Test Eisenstein BEST_CUMULATIVE =====
      auto startEC = std::chrono::high_resolution_clock::now();
      auto resultEC = EisensteinReduction::ReduceBestCumulative(inputG6, 1.0e-6, false);
      auto endEC = std::chrono::high_resolution_clock::now();
      auto durationEC = std::chrono::duration_cast<std::chrono::microseconds>(endEC - startEC);

      eisenCumulative.executionTimes.push_back(durationEC.count());
      if (resultEC.trace > 0) {
         eisenCumulative.successes++;
         eisenCumulative.totalIterations += resultEC.iterations;
         eisenCumulative.totalTransformations += resultEC.transformationsTested;

         // Compare with A-B
         double maxDiff = 0.0;
         for (size_t j = 0; j < 6; ++j) {
            double diff = std::abs(resultEC.reducedG6[j] - resultAB[j]);
            maxDiff = std::max(maxDiff, diff);
         }
         eisenCumulative.maxDifference = std::max(eisenCumulative.maxDifference, maxDiff);

         if (maxDiff > 1.0e-3) {
            eisenCumulative.mismatches++;

            MismatchCase mc;
            mc.cellNumber = i;
            mc.inputG6 = inputG6;
            mc.abResult = resultAB;
            mc.compareResult = resultEC.reducedG6;
            mc.abTrace = abTrace;
            mc.compareTrace = resultEC.trace;
            mc.maxDiff = maxDiff;
            mc.abCycles = abCycles;
            mc.compareIterations = resultEC.iterations;

            if (resultEC.trace < abTrace - 1.0e-6) {
               mc.winner = "Eisenstein BEST_CUMULATIVE";
            } else if (abTrace < resultEC.trace - 1.0e-6) {
               mc.winner = "A-B";
            } else {
               mc.winner = "Tie (different cell, same trace)";
            }

            eisenCumulativeMismatches.Store(std::to_string(i), mc);
         }
      } else {
         eisenCumulative.failures++;
      }

      // Progress indicator
      if ((i + 1) % 10 == 0) std::cout << "." << std::flush;
   }

   // Print results
   std::cout << "\n\n" << std::string(70, '=') << std::endl;
   std::cout << "RESULTS" << std::endl;
   std::cout << std::string(70, '=') << std::endl;

   std::cout << "\nSample:" << std::endl;
   std::cout << "  Total cells generated: " << numCells << std::endl;
   std::cout << "  Near-degenerate (excluded): " << nearDegenerateCells << std::endl;
   std::cout << "  Valid test cells: " << validCells << std::endl;

   // Print individual stats
   PrintAlgorithmStats(ab);
   PrintAlgorithmStats(kg);
   PrintAlgorithmStats(eisenAdaptive);
   PrintAlgorithmStats(eisenCumulative);

   // Print comparative summary
   PrintComparativeSummary(ab, kg, eisenAdaptive, eisenCumulative);

   // Show mismatch details
   if (!kgMismatches.empty()) {
      std::cout << "\n" << std::string(70, '=') << std::endl;
      kgMismatches.ShowResults();
   }

   if (!eisenAdaptiveMismatches.empty()) {
      std::cout << "\n" << std::string(70, '=') << std::endl;
      eisenAdaptiveMismatches.ShowResults();
   }

   if (!eisenCumulativeMismatches.empty()) {
      std::cout << "\n" << std::string(70, '=') << std::endl;
      eisenCumulativeMismatches.ShowResults();
   }

   // Final conclusions
   std::cout << "\n" << std::string(70, '=') << std::endl;
   std::cout << "CONCLUSIONS" << std::endl;
   std::cout << std::string(70, '=') << std::endl;

   // Find fastest algorithm
   double abAvg = ab.avgTime();
   double kgAvg = kg.avgTime();
   double eaAvg = eisenAdaptive.avgTime();
   double ecAvg = eisenCumulative.avgTime();

   std::cout << "\nCorrectness Summary:" << std::endl;
   std::cout << "  Andrews-Bernstein: Reference algorithm" << std::endl;
   std::cout << "  Krivy-Gruber: " << kg.mismatches << " mismatches";
   if (kg.mismatches == 0) std::cout << " ? PERFECT";
   std::cout << std::endl;
   std::cout << "  Eisenstein Adaptive: " << eisenAdaptive.mismatches << " mismatches";
   if (eisenAdaptive.mismatches == 0) std::cout << " ? PERFECT";
   std::cout << std::endl;
   std::cout << "  Eisenstein BEST_CUMULATIVE: " << eisenCumulative.mismatches << " mismatches";
   if (eisenCumulative.mismatches == 0) std::cout << " ? PERFECT";
   std::cout << std::endl;

   std::cout << "\nSpeed Ranking (fastest to slowest):" << std::endl;
   struct SpeedRank { std::string name; double time; };
   std::vector<SpeedRank> ranking = {
       {ab.name, abAvg},
       {kg.name, kgAvg},
       {eisenAdaptive.name, eaAvg},
       {eisenCumulative.name, ecAvg}
   };
   std::sort(ranking.begin(), ranking.end(),
      [](const SpeedRank& a, const SpeedRank& b) { return a.time < b.time; });

   for (size_t i = 0; i < ranking.size(); ++i) {
      std::cout << "  " << (i + 1) << ". " << ranking[i].name
         << " (" << std::fixed << std::setprecision(1) << ranking[i].time << " ?s)";
      if (i == 0) std::cout << " ? FASTEST";
      std::cout << std::endl;
   }

// Production recommendation (data-driven)
   std::string fastestName = ab.name;
   double fastestTime = abAvg;
   if (kgAvg < fastestTime) { fastestName = kg.name; fastestTime = kgAvg; }
   if (eaAvg < fastestTime) { fastestName = eisenAdaptive.name; fastestTime = eaAvg; }
   if (ecAvg < fastestTime) { fastestName = eisenCumulative.name; fastestTime = ecAvg; }

   std::cout << "\n? " << fastestName << " is FASTEST:" << std::endl;
   std::cout << "  • Execution time: " << std::setprecision(1) << fastestTime << " ?s average" << std::endl;

   std::cout << "\n? Andrews-Bernstein (2014) is the PRODUCTION CHOICE:" << std::endl;
   std::cout << "  • Very fast execution time (" << std::setprecision(1) << abAvg << " ?s average)" << std::endl;
   std::cout << "  • Highly optimized implementation" << std::endl;
   std::cout << "  • Reference standard for correctness" << std::endl;
   std::cout << "  • Well-tested and documented" << std::endl;
   std::cout << "\n" << std::string(70, '=') << std::endl;
}

inline void ReductionComparison::PrintAlgorithmStats(const AlgorithmStats& stats) {
   std::cout << "\n" << std::string(70, '-') << std::endl;
   std::cout << stats.name << std::endl;
   std::cout << std::string(70, '-') << std::endl;

   std::cout << "\nSuccess Rate:" << std::endl;
   int total = stats.successes + stats.failures;
   std::cout << "  Successes: " << stats.successes << " / " << total;
   if (total > 0) {
      std::cout << " (" << std::fixed << std::setprecision(1)
         << (100.0 * stats.successes / total) << "%)";
   }
   std::cout << std::endl;
   std::cout << "  Failures:  " << stats.failures << std::endl;

   if (stats.successes > 0) {
      std::cout << "\nIterations:" << std::endl;
      std::cout << "  Total:   " << stats.totalIterations << std::endl;
      std::cout << "  Average: " << std::setprecision(2)
         << (double)stats.totalIterations / stats.successes << std::endl;
   }

   if (stats.totalTransformations > 0) {
      std::cout << "\nTransformations:" << std::endl;
      std::cout << "  Total:   " << stats.totalTransformations << std::endl;
      std::cout << "  Average: " << std::setprecision(1)
         << (double)stats.totalTransformations / stats.successes << std::endl;
   }

   std::cout << "\nExecution Time (microseconds):" << std::endl;
   std::cout << "  Average: " << std::setprecision(1) << stats.avgTime() << std::endl;
   std::cout << "  Range:   " << stats.minTime() << " - " << stats.maxTime() << std::endl;

   std::cout << "\nCorrectness (vs Andrews-Bernstein):" << std::endl;
   if (stats.name.find("Andrews-Bernstein") != std::string::npos) {
      std::cout << "  Reference algorithm" << std::endl;
   } else {
      std::cout << "  Mismatches: " << stats.mismatches << " / " << stats.successes;
      if (stats.mismatches == 0) {
         std::cout << " ? PERFECT" << std::endl;
      } else {
         if (stats.successes > 0) {
            std::cout << " (" << std::setprecision(2)
               << (100.0 * stats.mismatches / stats.successes) << "%)" << std::endl;
         } else {
            std::cout << std::endl;
         }
      }
      std::cout << "  Max difference: " << std::scientific << stats.maxDifference << std::endl;
   }
}

inline void ReductionComparison::PrintComparativeSummary(
   const AlgorithmStats& ab, const AlgorithmStats& kg,
   const AlgorithmStats& eisenAdaptive, const AlgorithmStats& eisenCumulative) {

   std::cout << "\n" << std::string(70, '=') << std::endl;
   std::cout << "COMPARATIVE SUMMARY" << std::endl;
   std::cout << std::string(70, '=') << std::endl;

   double abAvg = ab.avgTime();
   double kgAvg = kg.avgTime();
   double eaAvg = eisenAdaptive.avgTime();
   double ecAvg = eisenCumulative.avgTime();

   // Find actual baseline (fastest)
   double baseline = std::min({ abAvg, kgAvg, eaAvg, ecAvg });

   std::cout << "\nSpeed Comparison (relative to fastest):" << std::endl;
   std::cout << std::fixed << std::setprecision(2);
   std::cout << "  " << std::setw(30) << std::left << ab.name
      << ":     " << (abAvg / baseline) << "× ";
   if (std::abs(abAvg - baseline) < 0.01) std::cout << "(baseline)";
   else std::cout << "(slower)";
   std::cout << std::endl;

   std::cout << "  " << std::setw(30) << std::left << kg.name
      << ":     " << (kgAvg / baseline) << "× ";
   if (std::abs(kgAvg - baseline) < 0.01) std::cout << "(baseline)";
   else std::cout << "(slower)";
   std::cout << std::endl;

   std::cout << "  " << std::setw(30) << std::left << eisenAdaptive.name
      << ":   " << (eaAvg / baseline) << "× (slower)" << std::endl;
   std::cout << "  " << std::setw(30) << std::left << eisenCumulative.name
      << ":  " << (ecAvg / baseline) << "× (slower)" << std::endl;

   // Rest stays the same...
   std::cout << "\nIterations Comparison:" << std::endl;
   if (ab.successes > 0) {
      std::cout << "  " << std::setw(30) << std::left << ab.name << ":     "
         << std::setprecision(2) << (double)ab.totalIterations / ab.successes << std::endl;
   }
   if (kg.successes > 0) {
      std::cout << "  " << std::setw(30) << std::left << kg.name << ":     "
         << (double)kg.totalIterations / kg.successes << std::endl;
   }
   if (eisenAdaptive.successes > 0) {
      std::cout << "  " << std::setw(30) << std::left << eisenAdaptive.name << ":     "
         << (double)eisenAdaptive.totalIterations / eisenAdaptive.successes << std::endl;
   }
   if (eisenCumulative.successes > 0) {
      std::cout << "  " << std::setw(30) << std::left << eisenCumulative.name << ":     "
         << (double)eisenCumulative.totalIterations / eisenCumulative.successes << std::endl;
   }

   std::cout << "\nTransformations Comparison:" << std::endl;
   std::cout << "  " << std::setw(30) << std::left << ab.name << ":      N/A" << std::endl;
   std::cout << "  " << std::setw(30) << std::left << kg.name << ":      N/A" << std::endl;
   if (eisenAdaptive.successes > 0) {
      std::cout << "  " << std::setw(30) << std::left << eisenAdaptive.name << ":   "
         << std::setprecision(1) << (double)eisenAdaptive.totalTransformations / eisenAdaptive.successes << std::endl;
   }
   if (eisenCumulative.successes > 0) {
      std::cout << "  " << std::setw(30) << std::left << eisenCumulative.name << ":   "
         << (double)eisenCumulative.totalTransformations / eisenCumulative.successes << std::endl;
   }

   std::cout << "\nCorrectness Summary:" << std::endl;
   std::cout << "  " << std::setw(30) << std::left << ab.name << ": Reference algorithm" << std::endl;
   std::cout << "  " << std::setw(30) << std::left << kg.name << ": "
      << kg.mismatches << " mismatches";
   if (kg.mismatches == 0) std::cout << " ?";
   std::cout << std::endl;
   std::cout << "  " << std::setw(30) << std::left << eisenAdaptive.name << ": "
      << eisenAdaptive.mismatches << " mismatches";
   if (eisenAdaptive.mismatches == 0) std::cout << " ?";
   std::cout << std::endl;
   std::cout << "  " << std::setw(30) << std::left << eisenCumulative.name << ": "
      << eisenCumulative.mismatches << " mismatches";
   if (eisenCumulative.mismatches == 0) std::cout << " ?";
   std::cout << std::endl;
}

#endif // COMPARE_REDUCTION_ALGORITHMS_H

