#ifndef TRACEKRIVYGRUBERG6_H
#define TRACEKRIVYGRUBERG6_H

#include <algorithm>
#include <iostream>
#include <limits>

#include "G6.h"
#include "LRL_Cell.h"
#include "Niggli.h"
#include "NCDist.h"

static constexpr int MAX_REDUCTION_ITERATIONS = 10000;


enum class TraceReductionStatus {
   Success,
   InvalidInput,
   FailedToConverge
};

struct TraceReductionResult {
   G6 reducedCell;
   TraceReductionStatus status;
   int iterations;           // ← ADD THIS
   double elapsedSeconds;    // ← ADD THIS

   // Convenience methods
   bool isValid() const { return status == TraceReductionStatus::Success; }
   bool succeeded() const { return status == TraceReductionStatus::Success; }
};

bool CheckVectors(const G6& a, const G6& b) {
   G6 g1;
   G6 g2;
   Niggli::Reduce(a, g1);
   Niggli::Reduce(b, g2);
   const double diff = NCDist(g1.data(), g2.data());
   if (diff > 1.0E-4) {
      return false;
   }
   return true;
}


template <typename T = double>
class TraceReduction {
   using DOUBLETYPE = T;
   static constexpr DOUBLETYPE DEFAULT_TOLERANCE = 1.0e-5;

public:

   static inline TraceReductionResult Reduce(
      const G6& inputG6,
      const DOUBLETYPE tolerance = DEFAULT_TOLERANCE,
      const bool verbose = false)
   {
      auto startTime = std::chrono::high_resolution_clock::now();

      // Validate input
      if (!inputG6.IsValid()) {
         if (verbose) std::cout << "ERROR: Invalid input - negative determinant" << std::endl;
         return { inputG6, TraceReductionStatus::InvalidInput, 0, 0.0 };
      }

      // Working copy of G6
      G6 resultG6 = inputG6;
      static constexpr int MAX_REDUCTION_ITERATIONS = 10000;
      int mainIter = 0;
      int totalSteps = 0;

      // Execute initial normalization
      DoSteps12134(resultG6, 0.0, totalSteps, verbose);

#ifdef _DEBUG
      CheckVectors(inputG6, resultG6);
#endif

      if (verbose) {
         std::cout << "Starting trace reduction" << std::endl;
         std::cout << "Tolerance: " << tolerance << std::endl;
      }

      // Main trace minimization loop
      while (mainIter++ < MAX_REDUCTION_ITERATIONS) {
         if (DoStep5Exact(resultG6, totalSteps, verbose)) continue;
         if (DoStep6Exact(resultG6, totalSteps, verbose)) continue;
         if (DoStep7Exact(resultG6, totalSteps, verbose)) continue;
         if (DoStep8Exact(resultG6, totalSteps, verbose)) continue;
         break;  // No steps fired - converged
      }

      auto endTime = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> elapsed = endTime - startTime;

      if (verbose) {
         std::cout << "\nTrace minimization converged" << std::endl;
         std::cout << "Total iterations: " << mainIter << std::endl;
         std::cout << "Total transformation steps: " << totalSteps << std::endl;
         std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;
      }

      // Apply final normalization including secondary conditions
      if (verbose) std::cout << "\nApplying final normalization..." << std::endl;
      NormalizeFinal(resultG6, 0.0, totalSteps, verbose);

#ifdef _DEBUG
      CheckVectors(inputG6, resultG6);
#endif

      if (mainIter < MAX_REDUCTION_ITERATIONS) {
         return { resultG6, TraceReductionStatus::Success, mainIter, elapsed.count() };
      } else {
         return { resultG6, TraceReductionStatus::FailedToConverge, mainIter, elapsed.count() };
      }
   }


   // Test if cell is in reduced form within tolerance
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

   static int GetMaxIterations() { return MAX_REDUCTION_ITERATIONS; }

private:

   // Helper: sign function (returns 1 for positive, -1 for negative)
   static int Sign(const DOUBLETYPE x) {
      return (x >= 0) ? 1 : -1;
   }

   // Step test functions (for final checking)
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
      if (verbose) std::cout << "  Step 1: Swap (g1,g4)<->(g2,g5)" << std::endl;
      return true;
   }

   static inline bool DoStep2(G6& g6, const DOUBLETYPE tolerance, int& totalSteps, const bool verbose) {
      if (!IfStep2(g6, tolerance)) return false;

      std::swap(g6[1], g6[2]);
      std::swap(g6[4], g6[5]);
      totalSteps++;
      if (verbose) std::cout << "  Step 2: Swap (g2,g5)<->(g3,g6)" << std::endl;
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
      case 0:  // +++ : keep as is
         break;

      case 1:  // ++- -> ---
         g6[3] = -g6[3];
         g6[4] = -g6[4];
         signChanged = true;
         break;

      case 2:  // +-+ -> ---
         g6[3] = -g6[3];
         g6[5] = -g6[5];
         signChanged = true;
         break;

      case 3:  // +-- -> depends on zeros
         if ((zeroPattern & 2) == 2) {  // +0-
            g6[3] = -g6[3];
            g6[4] = -g6[4];
         } else if ((zeroPattern & 1) == 1) {  // +-0
            g6[3] = -g6[3];
            g6[5] = -g6[5];
         } else {  // +-- -> +++
            g6[4] = -g6[4];
            g6[5] = -g6[5];
         }
         signChanged = true;
         break;

      case 4:  // -++ -> ---
         g6[4] = -g6[4];
         g6[5] = -g6[5];
         signChanged = true;
         break;

      case 5:  // -+- -> depends on zeros
         if ((zeroPattern & 4) == 4) {  // 0+-
            g6[3] = -g6[3];
            g6[4] = -g6[4];
         } else if ((zeroPattern & 1) == 1) {  // -+0
            g6[4] = -g6[4];
            g6[5] = -g6[5];
         } else {  // -+- -> +++
            g6[3] = -g6[3];
            g6[5] = -g6[5];
         }
         signChanged = true;
         break;

      case 6:  // --+ -> depends on zeros
         if ((zeroPattern & 4) == 4) {  // 0-+
            g6[3] = -g6[3];
            g6[5] = -g6[5];
         } else if ((zeroPattern & 2) == 2) {  // -0+
            g6[4] = -g6[4];
            g6[5] = -g6[5];
         } else {  // --+ -> +++
            g6[3] = -g6[3];
            g6[4] = -g6[4];
         }
         signChanged = true;
         break;

      case 7:  // --- : keep as is
         break;
      }

      if (signChanged) {
         totalSteps++;
         if (verbose) std::cout << "  Steps 3/4: Sign normalization" << std::endl;
      }

      return signChanged;
   }

   // Combined normalization sequence: Step1, Step2, Step1, Steps3-4
   static inline void DoSteps12134(G6& g6, const DOUBLETYPE tolerance, int& totalSteps, const bool verbose) {
      DoStep1(g6, tolerance, totalSteps, verbose);
      DoStep2(g6, tolerance, totalSteps, verbose);
      DoStep1(g6, tolerance, totalSteps, verbose);
      DoSteps3and4(g6, tolerance, totalSteps, verbose);
   }

   static inline bool DoStep5Exact(G6& g6, int& totalSteps, const bool verbose) {
      if (std::abs(g6[3]) <= g6[1]) return false;

      const int s = Sign(g6[3]);
      g6[2] = g6[1] + g6[2] - s * g6[3];
      g6[4] = g6[4] - s * g6[5];
      g6[3] = -2 * g6[1] * s + g6[3];
      totalSteps++;
      if (verbose) std::cout << "  Step 5: Adjust g3,g5,g4" << std::endl;

      DoSteps12134(g6, 0.0, totalSteps, verbose);
      return true;
   }

   static inline bool DoStep6Exact(G6& g6, int& totalSteps, const bool verbose) {
      if (std::abs(g6[4]) <= g6[0]) return false;

      const int s = Sign(g6[4]);
      g6[2] = g6[0] + g6[2] - s * g6[4];
      g6[3] = g6[3] - s * g6[5];
      g6[4] = -2 * g6[0] * s + g6[4];
      totalSteps++;
      if (verbose) std::cout << "  Step 6: Adjust g3,g4,g5" << std::endl;

      DoSteps12134(g6, 0.0, totalSteps, verbose);
      return true;
   }

   static inline bool DoStep7Exact(G6& g6, int& totalSteps, const bool verbose) {
      if (std::abs(g6[5]) <= g6[0]) return false;

      const int s = Sign(g6[5]);
      g6[1] = g6[0] + g6[1] - s * g6[5];
      g6[3] = g6[3] - s * g6[4];
      g6[5] = -2 * g6[0] * s + g6[5];
      totalSteps++;
      if (verbose) std::cout << "  Step 7: Adjust g2,g4,g6" << std::endl;

      DoSteps12134(g6, 0.0, totalSteps, verbose);
      return true;
   }

   static inline bool DoStep8Exact(G6& g6, int& totalSteps, const bool verbose) {
      const DOUBLETYPE sum = g6[3] + g6[4] + g6[5] + g6[0] + g6[1];
      if (sum >= 0.0) return false;

      g6[2] = g6[0] + g6[1] + g6[2] + g6[3] + g6[4] + g6[5];
      g6[3] = 2 * g6[1] + g6[3] + g6[5];
      g6[4] = 2 * g6[0] + g6[4] + g6[5];
      totalSteps++;
      if (verbose) std::cout << "  Step 8: Final adjustment" << std::endl;

      DoSteps12134(g6, 0.0, totalSteps, verbose);
      return true;
   }


// Final normalization: ordering, signs, and Niggli secondary conditions
   static inline void NormalizeFinal(G6& g6, const DOUBLETYPE tolerance,
      int& totalSteps, const bool verbose) {
      // Step 1: Apply primary normalization (ordering and signs)
      DoSteps12134(g6, tolerance, totalSteps, verbose);

      // Step 2: Enforce "zero counts as negative" rule
      // If any value is zero (within tolerance), all should be non-positive
      bool hasZero = (std::abs(g6[3]) <= tolerance ||
         std::abs(g6[4]) <= tolerance ||
         std::abs(g6[5]) <= tolerance);

      if (hasZero) {
         // If there's a zero, ensure all are non-positive (flip positive ones)
         bool hasPositive = (g6[3] > tolerance || g6[4] > tolerance || g6[5] > tolerance);

         if (hasPositive) {
            g6[3] = -g6[3];
            g6[4] = -g6[4];
            g6[5] = -g6[5];
            if (verbose) std::cout << "  Final: Flip signs (zero counts as negative)" << std::endl;
         }
      }

      // Step 3: Apply Niggli secondary conditions
      ApplyNiggliSecondaryConditions(g6, tolerance, verbose);
   }

   // Niggli secondary conditions for uniqueness
   static inline void ApplyNiggliSecondaryConditions(G6& g6, const DOUBLETYPE tolerance, const bool verbose) {
      if (verbose) {
         std::cout << "  === ENTERING ApplyNiggliSecondaryConditions ===" << std::endl;
         std::cout << "  g6: " << g6[0] << " " << g6[1] << " " << g6[2] << " "
            << g6[3] << " " << g6[4] << " " << g6[5] << std::endl;
         std::cout << "  tolerance: " << tolerance << std::endl;
      }
      
      // First pass: g1 = g2 check
      if (std::abs(g6[0] - g6[1]) <= tolerance) {
         if (verbose) {
            std::cout << "  Secondary check: g1=" << g6[0] << ", g2=" << g6[1]
               << ", g4=" << g6[3] << ", g5=" << g6[4] << std::endl;
            std::cout << "    |g4|-|g5| = " << (std::abs(g6[3]) - std::abs(g6[4]))
               << ", tolerance=" << tolerance << std::endl;
         }

         if (std::abs(g6[3]) - std::abs(g6[4]) > tolerance) {
            std::swap(g6[0], g6[1]);
            std::swap(g6[3], g6[4]);
            if (verbose) std::cout << "  Secondary: g1=g2, swap (g1,g4)<->(g2,g5)" << std::endl;
         } else {
            if (verbose) std::cout << "  Secondary: g1=g2, NO SWAP needed" << std::endl;
         }
      }

      // Second pass: g2 = g3 check
      if (std::abs(g6[1] - g6[2]) <= tolerance) {
         if (std::abs(g6[4]) - std::abs(g6[5]) > tolerance) {
            std::swap(g6[1], g6[2]);
            std::swap(g6[4], g6[5]);
            if (verbose) std::cout << "  Secondary: g2=g3, swap (g2,g5)<->(g3,g6)" << std::endl;
         }
      }

      // Third pass: g1 = g2 re-check
      if (std::abs(g6[0] - g6[1]) <= tolerance) {
         if (std::abs(g6[3]) - std::abs(g6[4]) > tolerance) {
            std::swap(g6[0], g6[1]);
            std::swap(g6[3], g6[4]);
            if (verbose) std::cout << "  Secondary: g1=g2, swap (g1,g4)<->(g2,g5)" << std::endl;
         }
      }
   }
};

#endif // TRACEKRIVYGRUBERG6_H

