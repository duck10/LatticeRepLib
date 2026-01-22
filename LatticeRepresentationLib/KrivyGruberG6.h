#ifndef KRIVYGRUBERG6_H
#define KRIVYGRUBERG6_H

#include <algorithm>
#include <iostream>
#include <limits>
#include <chrono>

#include "G6.h"
#include "LRL_Cell.h"

enum class ReductionStatus {
   Success,
   InvalidInput,
   FailedToConverge,
   Timeout
};

struct ReductionResult {
   G6 reducedCell;
   ReductionStatus status;
   int iterations;
   double elapsedSeconds;

   // Convenience methods
   bool isValid() const { return status == ReductionStatus::Success; }
   bool succeeded() const { return status == ReductionStatus::Success; }
};

template <typename T = double>
class KrivyGruberG6 {
   using DOUBLETYPE = T;
   static constexpr DOUBLETYPE DEFAULT_TOLERANCE = 1.0e-5;  // Changed from 0.0!
   static constexpr int DEFAULT_MAX_ITERATIONS = 10000;
   static constexpr double DEFAULT_MAX_SECONDS = 10.0;  // 10 second timeout

public:
   static inline ReductionResult Reduce(
      const G6& inputG6,
      const DOUBLETYPE tolerance = DEFAULT_TOLERANCE,
      const bool verbose = false,
      const int maxIterations = DEFAULT_MAX_ITERATIONS,
      const double maxSeconds = DEFAULT_MAX_SECONDS)
   {
      auto startTime = std::chrono::high_resolution_clock::now();

      // Validate input
      if (!inputG6.IsValid()) {
         if (verbose) std::cout << "ERROR: Invalid input - negative determinant" << std::endl;
         return { inputG6, ReductionStatus::InvalidInput, 0, 0.0 };
      }

      // Working copy of G6
      G6 resultG6 = inputG6;
      int mainIter = 0;
      int totalSteps = 0;

      // Execute initial normalization sequence
      DoSteps12134(resultG6, tolerance, totalSteps, verbose);


      // Track recent states for cycle detection
      std::vector<G6> recentStates;
      const int CYCLE_CHECK_DEPTH = 10;

      // Main algorithm loop
      while (mainIter < maxIterations) {
         mainIter++;

         // Check timeout
         auto currentTime = std::chrono::high_resolution_clock::now();
         std::chrono::duration<double> elapsed = currentTime - startTime;
         if (elapsed.count() > maxSeconds) {
            if (verbose) {
               std::cout << "TIMEOUT after " << elapsed.count() << " seconds, "
                  << mainIter << " iterations" << std::endl;
            }
            return { resultG6, ReductionStatus::Timeout, mainIter, elapsed.count() };
         }

         // Check for cycles
         for (const auto& prevState : recentStates) {
            if ((resultG6 - prevState).norm() < tolerance) {
               auto endTime = std::chrono::high_resolution_clock::now();
               std::chrono::duration<double> finalElapsed = endTime - startTime;
               if (verbose) {
                  std::cout << "Cycle detected at iteration " << mainIter
                     << ", stopping" << std::endl;
               }
               return { resultG6, ReductionStatus::Success, mainIter, finalElapsed.count() };
            }
         }

         // Store current state
         recentStates.push_back(resultG6);
         if (recentStates.size() > CYCLE_CHECK_DEPTH) {
            recentStates.erase(recentStates.begin());
         }

         if (DoStep5(resultG6, tolerance, totalSteps, verbose)) continue;
         if (DoStep6(resultG6, tolerance, totalSteps, verbose)) continue;
         if (DoStep7(resultG6, tolerance, totalSteps, verbose)) continue;
         if (DoStep8(resultG6, tolerance, totalSteps, verbose)) continue;

         // If we reach here, algorithm has converged
         break;
      }

      auto endTime = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> finalElapsed = endTime - startTime;

      // Report results
      ReportResults(mainIter, maxIterations, totalSteps, resultG6, finalElapsed.count(), verbose);

      if (mainIter < maxIterations) {
         return { resultG6, ReductionStatus::Success, mainIter, finalElapsed.count() };
      } else {
         return { resultG6, ReductionStatus::FailedToConverge, mainIter, finalElapsed.count() };
      }
   }

   // true = cell is in reduced form within tolerance
   static bool IsReduced(const G6& g, DOUBLETYPE tolerance = 1e-10)
   {
      return
         !IfStep1(g, tolerance) &&
         !IfStep2(g, tolerance) &&
         !IfStep5(g, tolerance) &&
         !IfStep6(g, tolerance) &&
         !IfStep7(g, tolerance) &&
         !IfStep8(g, tolerance);
   }

   static int GetMaxIterations() { return DEFAULT_MAX_ITERATIONS; }

private:

   // Helper: sign function (returns 1 for positive, -1 for negative)
   static int Sign(const DOUBLETYPE x) {
      return (x >= 0) ? 1 : -1;
   }

   // Step test functions
   static inline bool IfStep1(const G6& g6, const DOUBLETYPE tolerance) {
      return g6[0] > g6[1] + tolerance ||
         (std::abs(g6[0] - g6[1]) <= tolerance && std::abs(g6[3]) > std::abs(g6[4]) + tolerance);
   }

   static inline bool IfStep2(const G6& g6, const DOUBLETYPE tolerance) {
      return g6[1] > g6[2] + tolerance ||
         (std::abs(g6[1] - g6[2]) <= tolerance && std::abs(g6[4]) > std::abs(g6[5]) + tolerance);
   }

   static inline bool IfStep5(const G6& g6, const DOUBLETYPE tolerance) {
      if (std::abs(g6[3]) > g6[1] + tolerance) return true;
      if (std::abs(g6[3] - g6[1]) <= tolerance && 2 * g6[4] < g6[5] - tolerance) return true;
      if (std::abs(g6[3] + g6[1]) <= tolerance && g6[5] < -tolerance) return true;
      return false;
   }

   static inline bool IfStep6(const G6& g6, const DOUBLETYPE tolerance) {
      if (std::abs(g6[4]) > g6[0] + tolerance) return true;
      if (std::abs(g6[4] - g6[0]) <= tolerance && 2 * g6[3] < g6[5] - tolerance) return true;
      if (std::abs(g6[4] + g6[0]) <= tolerance && g6[5] < -tolerance) return true;
      return false;
   }

   static inline bool IfStep7(const G6& g6, const DOUBLETYPE tolerance) {
      if (std::abs(g6[5]) > g6[0] + tolerance) return true;
      if (std::abs(g6[5] - g6[0]) <= tolerance && 2 * g6[3] < g6[4] - tolerance) return true;
      if (std::abs(g6[5] + g6[0]) <= tolerance && g6[4] < -tolerance) return true;
      return false;
   }

   static inline bool IfStep8(const G6& g6, const DOUBLETYPE tolerance) {
      const DOUBLETYPE sum = g6[3] + g6[4] + g6[5] + g6[0] + g6[1];
      if (sum < -tolerance) return true;
      if (std::abs(sum) <= tolerance && 2 * (g6[0] + g6[4]) + g6[5] > tolerance) return true;
      return false;
   }

   // Step action functions
   static inline bool DoStep1(G6& g6, const DOUBLETYPE tolerance, int& totalSteps, const bool verbose) {
      if (!IfStep1(g6, tolerance)) return false;

      std::swap(g6[0], g6[1]);
      std::swap(g6[3], g6[4]);
      totalSteps++;
      if (verbose) std::cout << "Step 1: Swap (g1,g4)<->(g2,g5)" << std::endl;
      return true;
   }

   static inline bool DoStep2(G6& g6, const DOUBLETYPE tolerance, int& totalSteps, const bool verbose) {
      if (!IfStep2(g6, tolerance)) return false;

      std::swap(g6[1], g6[2]);
      std::swap(g6[4], g6[5]);
      totalSteps++;
      if (verbose) std::cout << "Step 2: Swap (g2,g5)<->(g3,g6)" << std::endl;
      return true;
   }

   static inline bool DoSteps3and4(G6& g6, const DOUBLETYPE tolerance, int& totalSteps, const bool verbose) {
      int minusPattern = 0;
      int zeroPattern = 0;

      if (g6[3] < -tolerance) minusPattern |= 4;
      if (g6[4] < -tolerance) minusPattern |= 2;
      if (g6[5] < -tolerance) minusPattern |= 1;

      if (std::abs(g6[3]) <= tolerance) zeroPattern |= 4;
      if (std::abs(g6[4]) <= tolerance) zeroPattern |= 2;
      if (std::abs(g6[5]) <= tolerance) zeroPattern |= 1;

      bool signChanged = false;

      switch (minusPattern) {
      case 0:  break;
      case 1:  g6[3] = -g6[3]; g6[4] = -g6[4]; signChanged = true; break;
      case 2:  g6[3] = -g6[3]; g6[5] = -g6[5]; signChanged = true; break;
      case 3:
         if ((zeroPattern & 2) == 2) { g6[3] = -g6[3]; g6[4] = -g6[4]; } else if ((zeroPattern & 1) == 1) { g6[3] = -g6[3]; g6[5] = -g6[5]; } else { g6[4] = -g6[4]; g6[5] = -g6[5]; }
         signChanged = true;
         break;
      case 4:  g6[4] = -g6[4]; g6[5] = -g6[5]; signChanged = true; break;
      case 5:
         if ((zeroPattern & 4) == 4) { g6[3] = -g6[3]; g6[4] = -g6[4]; } else if ((zeroPattern & 1) == 1) { g6[4] = -g6[4]; g6[5] = -g6[5]; } else { g6[3] = -g6[3]; g6[5] = -g6[5]; }
         signChanged = true;
         break;
      case 6:
         if ((zeroPattern & 4) == 4) { g6[3] = -g6[3]; g6[5] = -g6[5]; } else if ((zeroPattern & 2) == 2) { g6[4] = -g6[4]; g6[5] = -g6[5]; } else { g6[3] = -g6[3]; g6[4] = -g6[4]; }
         signChanged = true;
         break;
      case 7:  break;
      }

      if (signChanged) {
         totalSteps++;
         if (verbose) std::cout << "Steps 3/4: Sign normalization" << std::endl;
      }

      return signChanged;
   }

   static inline void DoSteps12134(G6& g6, const DOUBLETYPE tolerance, int& totalSteps, const bool verbose) {
      DoStep1(g6, tolerance, totalSteps, verbose);
      DoStep2(g6, tolerance, totalSteps, verbose);
      DoStep1(g6, tolerance, totalSteps, verbose);
      DoSteps3and4(g6, tolerance, totalSteps, verbose);
   }

   static inline bool DoStep5(G6& g6, const DOUBLETYPE tolerance, int& totalSteps, const bool verbose) {
      if (!IfStep5(g6, tolerance)) return false;

      const int s = Sign(g6[3]);
      g6[2] = g6[1] + g6[2] - s * g6[3];
      g6[4] = g6[4] - s * g6[5];
      g6[3] = -2 * g6[1] * s + g6[3];
      totalSteps++;
      if (verbose) std::cout << "Step 5: Adjust g3,g5,g4" << std::endl;

      DoSteps12134(g6, tolerance, totalSteps, verbose);
      return true;
   }

   static inline bool DoStep6(G6& g6, const DOUBLETYPE tolerance, int& totalSteps, const bool verbose) {
      if (!IfStep6(g6, tolerance)) return false;

      const int s = Sign(g6[4]);
      g6[2] = g6[0] + g6[2] - s * g6[4];
      g6[3] = g6[3] - s * g6[5];
      g6[4] = -2 * g6[0] * s + g6[4];
      totalSteps++;
      if (verbose) std::cout << "Step 6: Adjust g3,g4,g5" << std::endl;

      DoSteps12134(g6, tolerance, totalSteps, verbose);
      return true;
   }

   static inline bool DoStep7(G6& g6, const DOUBLETYPE tolerance, int& totalSteps, const bool verbose) {
      if (!IfStep7(g6, tolerance)) return false;

      const int s = Sign(g6[5]);
      g6[1] = g6[0] + g6[1] - s * g6[5];
      g6[3] = g6[3] - s * g6[4];
      g6[5] = -2 * g6[0] * s + g6[5];
      totalSteps++;
      if (verbose) std::cout << "Step 7: Adjust g2,g4,g6" << std::endl;

      DoSteps12134(g6, tolerance, totalSteps, verbose);
      return true;
   }

   static inline bool DoStep8(G6& g6, const DOUBLETYPE tolerance, int& totalSteps, const bool verbose) {
      if (!IfStep8(g6, tolerance)) return false;

      g6[2] = g6[0] + g6[1] + g6[2] + g6[3] + g6[4] + g6[5];
      g6[3] = 2 * g6[1] + g6[3] + g6[5];
      g6[4] = 2 * g6[0] + g6[4] + g6[5];
      totalSteps++;
      if (verbose) std::cout << "Step 8: Final adjustment" << std::endl;

      DoSteps12134(g6, tolerance, totalSteps, verbose);
      return true;
   }

   static inline void ReportResults(const int mainIter, const int maxIterations,
      const int totalSteps, const G6& resultG6, const double elapsedSeconds,
      const bool verbose) {
      if (!verbose) return;

      std::cout << "\n=== KG Results ===" << std::endl;
      if (mainIter < maxIterations) {
         std::cout << "Converged after " << mainIter << " iterations, "
            << totalSteps << " total steps, "
            << elapsedSeconds << " seconds" << std::endl;
      } else {
         std::cout << "WARNING: Failed to converge (max iterations = " << maxIterations << ")" << std::endl;
         std::cout << "Completed " << mainIter << " iterations, "
            << totalSteps << " steps, "
            << elapsedSeconds << " seconds" << std::endl;
      }
      std::cout << "Result G6: " << resultG6 << std::endl;
   }
};

using KG = KrivyGruberG6<>;
using KGfloat = KrivyGruberG6<float>;
using KGdouble = KrivyGruberG6<double>;
using KGlong = KrivyGruberG6<long double>;
#endif // KRIVYGRUBERG6_H

