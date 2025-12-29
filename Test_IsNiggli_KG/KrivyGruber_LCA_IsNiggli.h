// KrivyGruber_LCA.h - Larry Andrews Version
// Uses G6 notation directly, tests without tolerance, convergence based on sum change

#ifndef KRIVY_GRUBER_LCA_H
#define KRIVY_GRUBER_LCA_H

#include "G6.h"
#include "LRL_Cell.h"
#include <cmath>
#include <iostream>
#include <iomanip>

class KrivyGruber_LCA {
public:
   struct ReductionResult {
      G6 reducedG6;
      Matrix_3x3 m33;
      bool success;
      int iterations;
      int totalSteps;

      ReductionResult() : success(false), iterations(0), totalSteps(0) {}
   };

   static ReductionResult ReduceWithTransformation_LCA(const G6& inputG6, double tolerance = 1.0e-6, bool verbose = false);

private:
   // Helper: sign function
   static int sign(double x) {
      return (x >= 0) ? 1 : -1;
   }

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

   if (verbose) {
      std::cout << "\nInitial: g1=" << g1 << " g2=" << g2 << " g3=" << g3
         << " g4=" << g4 << " g5=" << g5 << " g6=" << g6 << std::endl;
   }

   // Track sum for convergence
   double previousSum = g1 + g2 + g3;

   // Main algorithm loop
   bool step1_label = true;

   while (mainIter < maxIterations) {
      mainIter++;

      // Step 1: If g1 > g2 OR (g1 = g2 AND |g4| > |g5|), swap
      // Following Krivy-Gruber paper: "a < b" test (strict inequality)
      if (step1_label) {
         step1_label = false;
         if (g1 > g2 || (g1 == g2 && std::abs(g4) > std::abs(g5))) {
            std::swap(g1, g2);
            std::swap(g4, g5);
            m3d = KrivyGruber_LCA::sp1_3D * m3d;
            totalSteps++;
            if (verbose) std::cout << "Step 1: Swap (g1,g4)<->(g2,g5)" << std::endl;
         }
      }

      // Step 2: If g2 > g3 OR (g2 = g3 AND |g5| > |g6|), swap and go to 1
      // Following Krivy-Gruber paper: "b < c" test
      if (g2 > g3 || (g2 == g3 && std::abs(g5) > std::abs(g6))) {
         std::swap(g2, g3);
         std::swap(g5, g6);
         m3d = KrivyGruber_LCA::sp2_3D * m3d;
         totalSteps++;
         if (verbose) std::cout << "Step 2: Swap (g2,g5)<->(g3,g6), go to 1" << std::endl;
         step1_label = true;
         continue;
      }

      // Steps 3-4: Sign normalization (all +++ or all ---)
      int minusPattern = 0;
      int zeroPattern = 0;

      if (g4 <= 0.0) minusPattern |= 4;
      if (g5 <= 0.0) minusPattern |= 2;
      if (g6 <= 0.0) minusPattern |= 1;

      if (g4 == 0.0) zeroPattern |= 4;
      if (g5 == 0.0) zeroPattern |= 2;
      if (g6 == 0.0) zeroPattern |= 1;

      bool signChanged = false;
      Matrix_3x3 signMatrix = KrivyGruber_LCA::spnull_3D;

      switch (minusPattern) {
      case 0:  // +++ : keep as is
         break;

      case 1:  // ++- -> ---
         g4 = -g4;
         g5 = -g5;
         signMatrix = KrivyGruber_LCA::sp34a_3D;
         signChanged = true;
         break;

      case 2:  // +-+ -> ---
         g4 = -g4;
         g6 = -g6;
         signMatrix = KrivyGruber_LCA::sp34b_3D;
         signChanged = true;
         break;

      case 3:  // +-- -> depends on zeros
         if ((zeroPattern & 2) == 2) {  // +0-
            g4 = -g4;
            g5 = -g5;
            signMatrix = KrivyGruber_LCA::sp34a_3D;
         } else if ((zeroPattern & 1) == 1) {  // +-0
            g4 = -g4;
            g6 = -g6;
            signMatrix = KrivyGruber_LCA::sp34b_3D;
         } else {  // +-- -> +++
            g5 = -g5;
            g6 = -g6;
            signMatrix = KrivyGruber_LCA::sp34c_3D;
         }
         signChanged = true;
         break;

      case 4:  // -++ -> ---
         g5 = -g5;
         g6 = -g6;
         signMatrix = KrivyGruber_LCA::sp34c_3D;
         signChanged = true;
         break;

      case 5:  // -+- -> depends on zeros
         if ((zeroPattern & 4) == 4) {  // 0+-
            g4 = -g4;
            g5 = -g5;
            signMatrix = KrivyGruber_LCA::sp34a_3D;
         } else if ((zeroPattern & 1) == 1) {  // -+0
            g5 = -g5;
            g6 = -g6;
            signMatrix = KrivyGruber_LCA::sp34c_3D;
         } else {  // -+- -> +++
            g4 = -g4;
            g6 = -g6;
            signMatrix = KrivyGruber_LCA::sp34b_3D;
         }
         signChanged = true;
         break;

      case 6:  // --+ -> depends on zeros
         if ((zeroPattern & 4) == 4) {  // 0-+
            g4 = -g4;
            g6 = -g6;
            signMatrix = KrivyGruber_LCA::sp34b_3D;
         } else if ((zeroPattern & 2) == 2) {  // -0+
            g5 = -g5;
            g6 = -g6;
            signMatrix = KrivyGruber_LCA::sp34c_3D;
         } else {  // --+ -> +++
            g4 = -g4;
            g5 = -g5;
            signMatrix = KrivyGruber_LCA::sp34a_3D;
         }
         signChanged = true;
         break;

      case 7:  // --- : keep as is
         break;
      }

      if (signChanged) {
         m3d = signMatrix * m3d;
         totalSteps++;
         if (verbose) std::cout << "Steps 3/4: Sign normalization" << std::endl;
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
         int s = sign(g4);
         g3 = g2 + g3 - s * g4;
         g5 = g5 - s * g6;
         g4 = -2 * g2 * s + g4;

         m3d = ((s > 0) ? KrivyGruber_LCA::R5_Plus_3D : KrivyGruber_LCA::R5_Minus_3D) * m3d;

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
         int s = sign(g5);
         g3 = g1 + g3 - s * g5;
         g4 = g4 - s * g6;
         g5 = -2 * g1 * s + g5;

         m3d = ((s > 0) ? KrivyGruber_LCA::R6_Plus_3D : KrivyGruber_LCA::R6_Minus_3D) * m3d;

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
         int s = sign(g6);
         g2 = g1 + g2 - s * g6;
         g4 = g4 - s * g5;
         g6 = -2 * g1 * s + g6;

         m3d = ((s > 0) ? KrivyGruber_LCA::R7_Plus_3D : KrivyGruber_LCA::R7_Minus_3D) * m3d;

         totalSteps++;
         if (verbose) std::cout << "Step 7: Adjust g2,g4,g6, go to 1" << std::endl;
         step1_label = true;
         continue;
      }

      // Step 8: If g4+g5+g6+g1+g2 < 0 OR (g4+g5+g6+g1+g2 = 0 AND 2*(g1+g5)+g6 > 0)
      double sum = g4 + g5 + g6 + g1 + g2;
      bool step8 = false;
      if (sum < 0.0) {
         step8 = true;
      }
      if (sum == 0.0 && 2 * (g1 + g5) + g6 > 0.0) {
         step8 = true;
      }

      if (step8) {
         g3 = g1 + g2 + g3 + g4 + g5 + g6;
         g4 = 2 * g2 + g4 + g6;
         g5 = 2 * g1 + g5 + g6;

         m3d = KrivyGruber_LCA::R8_3D * m3d;

         totalSteps++;
         if (verbose) std::cout << "Step 8: Final adjustment, go to 1" << std::endl;
         step1_label = true;
         continue;
      }

      // Check convergence: has g1+g2+g3 changed?
      double currentSum = g1 + g2 + g3;
      double sumChange = std::abs(currentSum - previousSum);

      if (sumChange < tolerance) {
         // Converged!
         if (verbose) {
            std::cout << "\nConverged after " << mainIter << " iterations" << std::endl;
            std::cout << "Total steps: " << totalSteps << std::endl;
            std::cout << "Final: g1=" << g1 << " g2=" << g2 << " g3=" << g3
               << " g4=" << g4 << " g5=" << g5 << " g6=" << g6 << std::endl;
         }

         result.reducedG6 = G6(g1, g2, g3, g4, g5, g6);
         result.m33 = m3d;
         result.success = true;
         result.iterations = mainIter;
         result.totalSteps = totalSteps;

         return result;
      }

      previousSum = currentSum;
   }

   // Failed to converge
   if (verbose) {
      std::cout << "WARNING: Failed to converge after " << maxIterations
         << " iterations" << std::endl;
   }

   result.reducedG6 = G6(g1, g2, g3, g4, g5, g6);
   result.m33 = m3d;
   result.success = false;
   result.iterations = maxIterations;
   result.totalSteps = totalSteps;

   return result;
}

#endif // KRIVY_GRUBER_LCA_H
