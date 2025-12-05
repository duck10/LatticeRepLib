// KrivyGruber_LCA.h - Larry Andrews Version
// Uses G6 notation directly, tests without tolerance, convergence based on sum change

#ifndef KRIVY_GRUBER_LCA_H
#define KRIVY_GRUBER_LCA_H

#include "G6.h"
#include "LRL_Cell.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>

class KrivyGruber_LCA {
public:
   struct ReductionResult {
      G6 reducedG6;
      Matrix_3x3 m33;
      bool success;
      int iterations;
      int totalSteps;

      ReductionResult() : success(false), iterations(0), totalSteps(0) {}

      friend std::ostream& operator<<(std::ostream& os, const ReductionResult& r) {
         os << "ReductionResult:\n"
            << "  Success: " << (r.success ? "true" : "false") << "\n"
            << "  Iterations: " << r.iterations << "\n"
            << "  Total steps: " << r.totalSteps << "\n"
            << "  Reduced G6: " << r.reducedG6 << "\n"
            << "  Transformation matrix det: " << r.m33.Det();
         return os;
      }
   };

   static ReductionResult ReduceWithTransformation_LCA(const G6& inputG6, double tolerance = 1.0e-6, bool verbose = false);

private:
   // Helper: sign function
   static int sign(double x) {
      return (x >= 0) ? 1 : -1;
   }

   // Helper: Sign normalization (Steps 3-4)
   // Returns true if signs were changed
   static bool NormalizeSigns(double& g4, double& g5, double& g6, Matrix_3x3& m3d, int& totalSteps, bool verbose);

   // 3D transformation matrices corresponding to each Krivy-Gruber step

   // Identity matrix
   static const Matrix_3x3 spnull_3D;

   // Step 1: Swap (g1,g4) <-> (g2,g5) corresponds to swapping a and b vectors
   static const Matrix_3x3 sp1_3D;

   // Step 2: Swap (g2,g5) <-> (g3,g6) corresponds to swapping b and c vectors
   static const Matrix_3x3 sp2_3D;

   // Sign change matrices for steps 3-4 (MKnorm sign normalization)
   static const Matrix_3x3 sp34a_3D;  // Negate g4 and g5
   static const Matrix_3x3 sp34b_3D;  // Negate g4 and g6
   static const Matrix_3x3 sp34c_3D;  // Negate g5 and g6

   // Step 5: Operations on (g3, g5, g4) when |g4| > g2
   static const Matrix_3x3 R5_Plus_3D;
   static const Matrix_3x3 R5_Minus_3D;

   // Step 6: Operations on (g3, g4, g5) when |g5| > g1
   static const Matrix_3x3 R6_Plus_3D;
   static const Matrix_3x3 R6_Minus_3D;

   // Step 7: Operations on (g2, g4, g6) when |g6| > g1
   static const Matrix_3x3 R7_Plus_3D;
   static const Matrix_3x3 R7_Minus_3D;

   // Step 8: Final adjustment operation
   static const Matrix_3x3 R8_3D;
};

// ===================================================================
// Initialize 3D transformation matrices
// ===================================================================

// Identity
const Matrix_3x3 KrivyGruber_LCA::spnull_3D = Matrix_3x3(
   1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0
);

// CORRECTED sp1_3D (swaps a and b)
const Matrix_3x3 KrivyGruber_LCA::sp1_3D = Matrix_3x3(
   0.0, -1.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, -1.0
);

// CORRECTED sp2_3D (swaps b and c)
const Matrix_3x3 KrivyGruber_LCA::sp2_3D = Matrix_3x3(
   -1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, -1.0, 0.0
);

// CORRECTED sp34a_3D (sign changes for beta and gamma)
const Matrix_3x3 KrivyGruber_LCA::sp34a_3D = Matrix_3x3(
   -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 1.0
);

// sp34b_3D (sign changes)
const Matrix_3x3 KrivyGruber_LCA::sp34b_3D = Matrix_3x3(
   -1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0
);

// CORRECTED sp34c_3D (sign changes for gamma and xi)
const Matrix_3x3 KrivyGruber_LCA::sp34c_3D = Matrix_3x3(
   1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, -1.0
);

// Reduction operation matrices
const Matrix_3x3 KrivyGruber_LCA::R5_Plus_3D = Matrix_3x3(
   1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, -1.0, 1.0
);

const Matrix_3x3 KrivyGruber_LCA::R5_Minus_3D = Matrix_3x3(
   1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0
);

const Matrix_3x3 KrivyGruber_LCA::R6_Plus_3D = Matrix_3x3(
   1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, 0.0, 1.0
);

const Matrix_3x3 KrivyGruber_LCA::R6_Minus_3D = Matrix_3x3(
   1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0
);

const Matrix_3x3 KrivyGruber_LCA::R7_Plus_3D = Matrix_3x3(
   1.0, 0.0, 0.0, -1.0, 1.0, 0.0, 0.0, 0.0, 1.0
);

const Matrix_3x3 KrivyGruber_LCA::R7_Minus_3D = Matrix_3x3(
   1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0
);

const Matrix_3x3 KrivyGruber_LCA::R8_3D = Matrix_3x3(
   1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0
);

// ===================================================================
// ReduceWithTransformation_LCA - Larry Andrews Version
// - Uses G6 notation (g1, g2, g3, g4, g5, g6) directly
// - Tests without tolerance (strict inequalities)
// - Convergence: change in g1+g2+g3 < tolerance
// ===================================================================

// ===================================================================
// Helper function: Sign normalization (Steps 3-4)
// Returns true if signs were changed
// ===================================================================
inline bool KrivyGruber_LCA::NormalizeSigns(
   double& g4, double& g5, double& g6,
   Matrix_3x3& m3d,
   int& totalSteps,
   bool verbose)
{
   int minusPattern = 0, zeroPattern = 0;

   if (g4 <= 0.0) minusPattern |= 4;
   if (g5 <= 0.0) minusPattern |= 2;
   if (g6 <= 0.0) minusPattern |= 1;

   if (g4 == 0.0) zeroPattern |= 4;
   if (g5 == 0.0) zeroPattern |= 2;
   if (g6 == 0.0) zeroPattern |= 1;

   Matrix_3x3 signMatrix = spnull_3D;
   bool signChanged = false;

   switch (minusPattern) {
   case 0:  // +++ : keep as is
   case 7:  // --- : keep as is
      break;

   case 1:  // ++- -> ---
      g4 = -g4; g5 = -g5;
      signMatrix = sp34a_3D;
      signChanged = true;
      break;

   case 2:  // +-+ -> ---
      g4 = -g4; g6 = -g6;
      signMatrix = sp34b_3D;
      signChanged = true;
      break;

   case 3:  // +-- -> depends on zeros
      if ((zeroPattern & 2) == 2) {  // +0-
         g4 = -g4; g5 = -g5;
         signMatrix = sp34a_3D;
      } else if ((zeroPattern & 1) == 1) {  // +-0
         g4 = -g4; g6 = -g6;
         signMatrix = sp34b_3D;
      } else {  // +-- -> +++
         g5 = -g5; g6 = -g6;
         signMatrix = sp34c_3D;
      }
      signChanged = true;
      break;

   case 4:  // -++ -> ---
      g5 = -g5; g6 = -g6;
      signMatrix = sp34c_3D;
      signChanged = true;
      break;

   case 5:  // -+- -> depends on zeros
      if ((zeroPattern & 4) == 4) {  // 0+-
         g4 = -g4; g5 = -g5;
         signMatrix = sp34a_3D;
      } else if ((zeroPattern & 1) == 1) {  // -+0
         g5 = -g5; g6 = -g6;
         signMatrix = sp34c_3D;
      } else {  // -+- -> +++
         g4 = -g4; g6 = -g6;
         signMatrix = sp34b_3D;
      }
      signChanged = true;
      break;

   case 6:  // --+ -> depends on zeros
      if ((zeroPattern & 4) == 4) {  // 0-+
         g4 = -g4; g6 = -g6;
         signMatrix = sp34b_3D;
      } else if ((zeroPattern & 2) == 2) {  // -0+
         g5 = -g5; g6 = -g6;
         signMatrix = sp34c_3D;
      } else {  // --+ -> +++
         g4 = -g4; g5 = -g5;
         signMatrix = sp34a_3D;
      }
      signChanged = true;
      break;
   }

   if (signChanged) {
      m3d = signMatrix * m3d;
      totalSteps++;
      if (verbose) std::cout << "Steps 3/4: Sign normalization" << std::endl;
   }

   return signChanged;
}

// ===================================================================
// Main algorithm
// ===================================================================

inline KrivyGruber_LCA::ReductionResult
KrivyGruber_LCA::ReduceWithTransformation_LCA(const G6& inputG6, double tolerance, bool verbose) {
   ReductionResult result;

   // Validate input
   LRL_Cell cell(inputG6);
   if (!cell.IsValid()) {
      if (verbose) std::cout << "ERROR: Invalid input cell" << std::endl;
      return result;
   }

   if (verbose) {
      std::cout << "Krivy & Gruber (1976) - LCA Version with G6 notation" << std::endl;
      std::cout << "Input G6: " << inputG6 << std::endl;
   }

   // Initialize transformation matrix to identity
   Matrix_3x3 m3d = KrivyGruber_LCA::spnull_3D;

   // Work directly with G6 components
   double g1 = inputG6[0];
   double g2 = inputG6[1];
   double g3 = inputG6[2];
   double g4 = inputG6[3];
   double g5 = inputG6[4];
   double g6 = inputG6[5];

   const int maxIterations = 1000;
   int mainIter = 0;
   int totalSteps = 0;
   // Cycle detection: track which steps fire in last iterations
   std::vector<int> stepSequence;  // Records which step fired each iteration
   const int CYCLE_DETECT_START = maxIterations - 20;  // Start detailed logging at iteration 980


   // Sum tracking for diagnostics
   const double initialSum = g1 + g2 + g3;
   double previousSum = initialSum;

   // Circular buffer for last 20 sum values (for debugging stuck cases)
   const int HISTORY_SIZE = 20;
   double sumHistory[HISTORY_SIZE];
   int historyIndex = 0;
   int historyCount = 0;

   if (verbose) {
      std::cout << "\nInitial: g1=" << g1 << " g2=" << g2 << " g3=" << g3
         << " g4=" << g4 << " g5=" << g5 << " g6=" << g6 << std::endl;
   }

   // Main algorithm loop
   bool step1_label = true;
   bool workDone = false;  // Tracks if reduction work (Steps 5-8) happens in current iteration
   bool previousWorkDone = false;  // Track if reduction work was done in previous iteration
   int stableIterations = 0;  // Count consecutive iterations with stable trace

   while (mainIter < maxIterations) {
      mainIter++;

      // Record current sum in circular buffer
      const double currentSum = g1 + g2 + g3;
      sumHistory[historyIndex] = currentSum;
      historyIndex = (historyIndex + 1) % HISTORY_SIZE;
      historyCount++;

      // Sum-based convergence check (BEFORE trying any steps)
      // Converge when: trace is stable AND (reduction was attempted OR stable for many iterations)
      if (mainIter > 1) {
         const double sumChange = std::abs(currentSum - previousSum);
         const double fractionalChange = (previousSum != 0.0) ? (sumChange / std::abs(previousSum)) : sumChange;

         const double convergenceTolerance = (tolerance == 0.0) ? 1e-12 : tolerance;

         if (fractionalChange < convergenceTolerance) {
            stableIterations++;

            // Primary convergence: trace stable AND we attempted reduction last iteration
            if (previousWorkDone) {
               if (verbose) {
                  std::cout << "\nConverged after " << mainIter << " iterations (sum stabilized)" << std::endl;
                  std::cout << "  Sum change: " << sumChange << std::endl;
                  std::cout << "  Fractional change: " << fractionalChange << std::endl;
               }
               workDone = false;
               break;
            }

            // Safety valve: if trace stable for 10+ iterations, force convergence
            if (stableIterations >= 10) {
               if (verbose) {
                  std::cout << "\nConverged after " << mainIter << " iterations (trace stable for "
                     << stableIterations << " iterations)" << std::endl;
                  std::cout << "  Sum change: " << sumChange << std::endl;
                  std::cout << "  Fractional change: " << fractionalChange << std::endl;
               }
               workDone = false;
               break;
            }
         } else {
            stableIterations = 0;  // Reset if trace changes
         }
      }

      // CRITICAL: Update previousSum for next iteration (must be here, not at end of loop!)
      previousSum = currentSum;

      // Cycle detection logging
      if (mainIter >= CYCLE_DETECT_START) {
         std::cout << "\n=== ITERATION " << mainIter << " ===" << std::endl;
         std::cout << std::fixed << std::setprecision(20);
         std::cout << "g1 = " << g1 << std::endl;
         std::cout << "g2 = " << g2 << std::endl;
         std::cout << "g3 = " << g3 << std::endl;
         std::cout << "g4 = " << g4 << std::endl;
         std::cout << "g5 = " << g5 << std::endl;
         std::cout << "g6 = " << g6 << std::endl;
         std::cout << "trace = " << currentSum << std::endl;
      }

      workDone = false;  // Assume no work until proven otherwise
      int stepFired = 0;  // Track which step fires this iteration

      // Step 1: If g1 > g2 OR (g1 = g2 AND |g4| > |g5|), swap
      // Following Krivy-Gruber paper: "a < b" test (strict inequality)
      if (step1_label) {
         step1_label = false;
         if (g1 > g2 || (g1 == g2 && std::abs(g4) > std::abs(g5))) {
            if (mainIter >= CYCLE_DETECT_START) {
               std::cout << "STEP 1 TRIGGERS: ";
               if (g1 > g2) std::cout << "g1 > g2";
               else std::cout << "g1 == g2 and |g4| > |g5|";
               std::cout << std::endl;
            }
            std::swap(g1, g2);
            std::swap(g4, g5);
            stepFired = 1;
            // Note: Step 1 does NOT set workDone - it's standardization, not reduction
            m3d = KrivyGruber_LCA::sp1_3D * m3d;
            totalSteps++;
            if (verbose) std::cout << "Step 1: Swap (g1,g4)<->(g2,g5)" << std::endl;
         }
      }

      // Step 2: If g2 > g3 OR (g2 = g3 AND |g5| > |g6|), swap and go to 1
      // Following Krivy-Gruber paper: "b < c" test
      if (g2 > g3 || (g2 == g3 && std::abs(g5) > std::abs(g6))) {
         if (mainIter >= CYCLE_DETECT_START) {
            std::cout << "STEP 2 TRIGGERS: ";
            if (g2 > g3) std::cout << "g2 > g3";
            else std::cout << "g2 == g3 and |g5| > |g6|";
            std::cout << std::endl;
         }
         std::swap(g2, g3);
         std::swap(g5, g6);
         stepFired = 2;
         // Note: Step 2 does NOT set workDone - it's standardization, not reduction
         m3d = KrivyGruber_LCA::sp2_3D * m3d;
         totalSteps++;
         if (verbose) std::cout << "Step 2: Swap (g2,g5)<->(g3,g6), go to 1" << std::endl;
         step1_label = true;
         continue;
      }

      // Steps 3-4: Sign normalization (all +++ or all ---)
      bool signChanged = NormalizeSigns(g4, g5, g6, m3d, totalSteps, verbose);

      if (signChanged) {
         if (mainIter >= CYCLE_DETECT_START) {
            std::cout << "STEP 3/4 TRIGGERS: Sign normalization" << std::endl;
         }
         // Note: Steps 3/4 do NOT set workDone - they're standardization, not reduction
         stepFired = 34;
         step1_label = true;
         continue;
      }

      // Step 5: If |g4| > g2 OR (g4 = g2 AND 2*g5 < g6) OR (g4 = -g2 AND g6 < 0)
      bool step5 = false;
      if (std::abs(g4) > g2) {
         step5 = true;
      }
      if (g4 == g2 && 2 * g5 < g6) {
         step5 = true;
      }
      if (g4 == -g2 && g6 < 0.0) {
         step5 = true;
      }

      if (step5) {
         if (mainIter >= CYCLE_DETECT_START) {
            std::cout << "STEP 5 TRIGGERS" << std::endl;
         }
         const int s = sign(g4);
         g3 = g2 + g3 - s * g4;
         g5 = g5 - s * g6;
         g4 = -2 * g2 * s + g4;

         m3d = ((s > 0) ? KrivyGruber_LCA::R5_Plus_3D : KrivyGruber_LCA::R5_Minus_3D) * m3d;

         workDone = true;
         stepFired = 5;
         totalSteps++;
         if (verbose) std::cout << "Step 5: Adjust g3,g5,g4, go to 1" << std::endl;
         step1_label = true;
         continue;
      }

      // Step 6: If |g5| > g1 OR (g5 = g1 AND 2*g4 < g6) OR (g5 = -g1 AND g6 < 0)
      bool step6 = false;
      if (std::abs(g5) > g1) {
         step6 = true;
      }
      if (g5 == g1 && 2 * g4 < g6) {
         step6 = true;
      }
      if (g5 == -g1 && g6 < 0.0) {
         step6 = true;
      }

      if (step6) {
         if (mainIter >= CYCLE_DETECT_START) {
            std::cout << "STEP 6 TRIGGERS" << std::endl;
         }
         const int s = sign(g5);
         g3 = g1 + g3 - s * g5;
         g4 = g4 - s * g6;
         g5 = -2 * g1 * s + g5;

         m3d = ((s > 0) ? KrivyGruber_LCA::R6_Plus_3D : KrivyGruber_LCA::R6_Minus_3D) * m3d;

         workDone = true;
         stepFired = 6;
         totalSteps++;
         if (verbose) std::cout << "Step 6: Adjust g3,g4,g5, go to 1" << std::endl;
         step1_label = true;
         continue;
      }

      // Step 7: If |g6| > g1 OR (g6 = g1 AND 2*g4 < g5) OR (g6 = -g1 AND g5 < 0)
      bool step7 = false;
      if (std::abs(g6) > g1) {
         step7 = true;
      }
      if (g6 == g1 && 2 * g4 < g5) {
         step7 = true;
      }
      if (g6 == -g1 && g5 < 0.0) {
         step7 = true;
      }

      if (step7) {
         if (mainIter >= CYCLE_DETECT_START) {
            std::cout << "STEP 7 TRIGGERS" << std::endl;
         }
         int s = sign(g6);
         g2 = g1 + g2 - s * g6;
         g4 = g4 - s * g5;
         g6 = -2 * g1 * s + g6;

         m3d = ((s > 0) ? KrivyGruber_LCA::R7_Plus_3D : KrivyGruber_LCA::R7_Minus_3D) * m3d;

         workDone = true;
         stepFired = 7;
         totalSteps++;
         if (verbose) std::cout << "Step 7: Adjust g2,g4,g6, go to 1" << std::endl;
         step1_label = true;
         continue;
      }

      // Step 8: If g4+g5+g6+g1+g2 < 0 OR (g4+g5+g6+g1+g2 = 0 AND 2*(g1+g5)+g6 > 0)
      const double sum = g4 + g5 + g6 + g1 + g2;
      bool step8 = false;
      if (sum < 0.0) {
         step8 = true;
      }
      if (sum == 0.0 && 2 * (g1 + g5) + g6 > 0.0) {
         step8 = true;
      }

      if (step8) {
         if (mainIter >= CYCLE_DETECT_START) {
            std::cout << "STEP 8 TRIGGERS" << std::endl;
         }
         g3 = g1 + g2 + g3 + g4 + g5 + g6;
         g4 = 2 * g2 + g4 + g6;
         g5 = 2 * g1 + g5 + g6;

         m3d = KrivyGruber_LCA::R8_3D * m3d;

         workDone = true;
         stepFired = 8;
         totalSteps++;
         if (verbose) std::cout << "Step 8: Final adjustment, go to 1" << std::endl;
         step1_label = true;
         continue;
      }

      // Record which step fired this iteration
      if (mainIter >= CYCLE_DETECT_START) {
         stepSequence.push_back(stepFired);
      }

      // Save workDone status for convergence check in next iteration
      previousWorkDone = workDone;
   }

   // ===================================================================
   // CYCLE DETECTION ANALYSIS
   // ===================================================================
   if (mainIter >= maxIterations && stepSequence.size() > 0) {
      std::cout << "\n=== HIT MAX ITERATIONS - CYCLE ANALYSIS ===" << std::endl;
      std::cout << "Step sequence for last " << stepSequence.size() << " iterations:" << std::endl;

      // Print step sequence
      std::cout << "Steps: ";
      for (size_t i = 0; i < std::min(size_t(40), stepSequence.size()); ++i) {
         if (stepSequence[i] == 34) std::cout << "3/4";
         else std::cout << stepSequence[i];
         if (i < std::min(size_t(40), stepSequence.size()) - 1) std::cout << " -> ";
      }
      if (stepSequence.size() > 40) {
         std::cout << " ... (and " << (stepSequence.size() - 40) << " more)";
      }
      std::cout << std::endl;

      // Look for repeating patterns
      std::cout << "\nLooking for repeating cycle pattern..." << std::endl;
      for (int cycleLen = 2; cycleLen <= 10; ++cycleLen) {
         if (stepSequence.size() < size_t(3 * cycleLen)) continue;

         bool foundCycle = true;
         for (int rep = 0; rep < 3; ++rep) {
            for (int i = 0; i < cycleLen; ++i) {
               int idx1 = stepSequence.size() - cycleLen - rep * cycleLen + i;
               int idx2 = stepSequence.size() - cycleLen - (rep + 1) * cycleLen + i;
               if (idx2 < 0 || stepSequence[idx1] != stepSequence[idx2]) {
                  foundCycle = false;
                  break;
               }
            }
            if (!foundCycle) break;
         }

         if (foundCycle) {
            std::cout << "FOUND CYCLE of length " << cycleLen << ": ";
            for (int i = 0; i < cycleLen; ++i) {
               int step = stepSequence[stepSequence.size() - cycleLen + i];
               if (step == 34) std::cout << "3/4";
               else std::cout << step;
               if (i < cycleLen - 1) std::cout << " -> ";
            }
            std::cout << " (repeats)" << std::endl;
            break;
         }
      }
   }

   // ===================================================================
   // FINAL STANDARDIZATION - Ensure canonical form (Steps 1-4)
   // ===================================================================

   if (verbose) {
      std::cout << "\n=== Final Standardization ===" << std::endl;
   }

   // Steps 1-2: Sort to g1 <= g2 <= g3
   {
      bool sorting = true;
      int sortIter = 0;
      while (sorting && sortIter < 10) {
         sorting = false;
         sortIter++;

         if (g1 > g2 || (g1 == g2 && std::abs(g4) > std::abs(g5))) {
            if (verbose) std::cout << "  Before swap: g1=" << g1 << " g2=" << g2 << " g4=" << g4 << " g5=" << g5 << std::endl;
            std::swap(g1, g2);
            std::swap(g4, g5);
            m3d = KrivyGruber_LCA::sp1_3D * m3d;
            totalSteps++;
            sorting = true;
            if (verbose) {
               std::cout << "  Final Step 1: Swap (g1,g4)<->(g2,g5)" << std::endl;
               std::cout << "  After swap:  g1=" << g1 << " g2=" << g2 << " g4=" << g4 << " g5=" << g5 << std::endl;
            }
         }

         if (g2 > g3 || (g2 == g3 && std::abs(g5) > std::abs(g6))) {
            if (verbose) std::cout << "  Before swap: g2=" << g2 << " g3=" << g3 << " g5=" << g5 << " g6=" << g6 << std::endl;
            std::swap(g2, g3);
            std::swap(g5, g6);
            m3d = KrivyGruber_LCA::sp2_3D * m3d;
            totalSteps++;
            sorting = true;
            if (verbose) {
               std::cout << "  Final Step 2: Swap (g2,g5)<->(g3,g6)" << std::endl;
               std::cout << "  After swap:  g2=" << g2 << " g3=" << g3 << " g5=" << g5 << " g6=" << g6 << std::endl;
            }
         }
      }
   }

   // Steps 3-4: Normalize signs
   NormalizeSigns(g4, g5, g6, m3d, totalSteps, verbose);

   result.reducedG6 = G6(g1, g2, g3, g4, g5, g6);
   result.m33 = m3d;
   result.success = !workDone;  // Success if exited due to no work
   result.iterations = mainIter;
   result.totalSteps = totalSteps;


   // Exited while loop - either converged (workDone=false) or hit max iterations

   // Always report maxIterations failures with diagnostics
   if (mainIter >= maxIterations) {
      const double finalSum = g1 + g2 + g3;
      std::cout << "\n" << std::string(70, '=') << std::endl;
      std::cout << "=== HIT maxIterations ===" << std::endl;
      std::cout << std::string(70, '=') << std::endl;
      std::cout << "Input G6: " << inputG6 << std::endl;
      std::cout << "Final G6: g1=" << g1 << " g2=" << g2 << " g3=" << g3
         << " g4=" << g4 << " g5=" << g5 << " g6=" << g6 << std::endl;
      std::cout << "Initial sum: " << std::setprecision(15) << initialSum << std::endl;
      std::cout << "Final sum:   " << std::setprecision(15) << finalSum << std::endl;
      std::cout << "Sum change:  " << (initialSum - finalSum) << std::endl;
      std::cout << "Total steps: " << totalSteps << std::endl;
      std::cout << "workDone: " << (workDone ? "true (still working)" : "false") << std::endl;
      std::cout << "step1_label: " << (step1_label ? "true" : "false") << std::endl;

      // Print last 20 sum values from circular buffer
      const int itemsInBuffer = std::min(historyCount, HISTORY_SIZE);
      if (itemsInBuffer > 0) {
         std::cout << "\nLast " << itemsInBuffer << " sum values:" << std::endl;
         int startIdx = (historyCount < HISTORY_SIZE) ? 0 : historyIndex;

         for (int i = 0; i < itemsInBuffer; ++i) {
            int bufIdx = (startIdx + i) % HISTORY_SIZE;
            double change = (i > 0) ? (sumHistory[bufIdx] - sumHistory[(startIdx + i - 1) % HISTORY_SIZE]) : 0.0;
            std::cout << "  Iter " << (mainIter - itemsInBuffer + i + 1)
               << ": " << std::setprecision(15) << sumHistory[bufIdx]
               << " (change: " << change << ")" << std::endl;
         }
      }
      std::cout << std::string(70, '=') << std::endl;
   }

   if (verbose) {
      if (!workDone) {
         std::cout << "\nConverged after " << mainIter << " iterations (no work done)" << std::endl;
      } else {
         std::cout << "WARNING: Failed to converge after " << maxIterations << " iterations" << std::endl;
         std::cerr << result << std::endl;
      }
      std::cout << "Total steps: " << totalSteps << std::endl;
      std::cout << "Final: g1=" << g1 << " g2=" << g2 << " g3=" << g3
         << " g4=" << g4 << " g5=" << g5 << " g6=" << g6 << std::endl;
   }
   return result;
}

#endif // KRIVY_GRUBER_LCA_H
