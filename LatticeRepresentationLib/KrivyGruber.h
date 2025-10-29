// KrivyGruber.h - Corrected Implementation from 1976 Paper
#ifndef KRIVY_GRUBER_H
#define KRIVY_GRUBER_H

#include "G6.h"
#include "LRL_Cell.h"
#include <cmath>
#include <iostream>
#include <iomanip>

class KrivyGruber {
public:
   struct ReductionResult {
      G6 reducedG6;
      bool success;
      int iterations;
      int totalSteps;  // Total algorithm steps (for tracking)

      ReductionResult() : success(false), iterations(0), totalSteps(0) {}
   };

   static ReductionResult Reduce(const G6& inputG6, double tolerance = 1.0e-6, bool verbose = false);

private:
   // Helper: sign function (returns 1 for positive, -1 for negative)
   static int sign(double x) {
      return (x >= 0) ? 1 : -1;
   }
};

// ===================================================================
// IMPLEMENTATION
// ===================================================================

inline KrivyGruber::ReductionResult
KrivyGruber::Reduce(const G6& inputG6, double tolerance, bool verbose) {
   ReductionResult result;

   // Validate input
   LRL_Cell cell(inputG6);
   if (!cell.IsValid()) {
      if (verbose) std::cout << "ERROR: Invalid input cell" << std::endl;
      return result;
   }

   if (verbose) {
      std::cout << "Krivy & Gruber (1976) reduction" << std::endl;
      std::cout << "Input G6: " << inputG6 << std::endl;
   }

   // Step 0: Convert from G6 to (A, B, C, ?, ?, ?) notation
   // G6: (g1, g2, g3, g4, g5, g6) where g1=a², g2=b², g3=c²
   //     g4=2bc·cos(?), g5=2ac·cos(?), g6=2ab·cos(?)
   // Krivy-Gruber: A=a², B=b², C=c², ?=2bc·cos(?), ?=2ac·cos(?), ?=2ab·cos(?)
   double A = inputG6[0];
   double B = inputG6[1];
   double C = inputG6[2];
   double xi = inputG6[3];    // ?
   double eta = inputG6[4];   // ?
   double zeta = inputG6[5];  // ?

   const int maxIterations = 10000;
   int mainIter = 0;
   int totalSteps = 0;

   if (verbose) {
      std::cout << "\nInitial: A=" << A << " B=" << B << " C=" << C
         << " ?=" << xi << " ?=" << eta << " ?=" << zeta << std::endl;
   }

   // Main algorithm loop
   bool step1_label = true;  // Start at step 1

   while (mainIter < maxIterations) {
      mainIter++;

      // Step 1: CORRECTED - swap (A,?) ? (B,?)
      if (step1_label) {
         step1_label = false;
         if (A > B + tolerance || (std::abs(A - B) <= tolerance && std::abs(xi) > std::abs(eta) + tolerance)) {
            std::swap(A, B);
            std::swap(xi, eta);  // CORRECTED: was std::swap(zeta, eta)
            totalSteps++;
            if (verbose) std::cout << "Step 1: Swap (A,?)?(B,?)" << std::endl;
         }
      }

      // Step 2: If B > C or (B = C and |?| > |?|), swap (B,?) ? (C,?) and go to 1
      if (B > C + tolerance || (std::abs(B - C) <= tolerance && std::abs(eta) > std::abs(zeta) + tolerance)) {
         std::swap(B, C);
         std::swap(eta, zeta);
         totalSteps++;
         if (verbose) std::cout << "Step 2: Swap (B,?)?(C,?), go to 1" << std::endl;
         step1_label = true;
         continue;  // Go to step 1
      }

      // Replace Steps 3-4 with proper sign normalization matching MKnorm

      // Determine pattern
      int minusPattern = 0;
      int zeroPattern = 0;

      if (xi < tolerance) minusPattern |= 4;
      if (eta < tolerance) minusPattern |= 2;
      if (zeta < tolerance) minusPattern |= 1;

      if (std::abs(xi) < tolerance) zeroPattern |= 4;
      if (std::abs(eta) < tolerance) zeroPattern |= 2;
      if (std::abs(zeta) < tolerance) zeroPattern |= 1;

      bool signChanged = false;

      switch (minusPattern) {
      case 0:  // +++ : keep as is
         break;

      case 1:  // ++- ? ---
         xi = -xi;
         eta = -eta;
         signChanged = true;
         break;

      case 2:  // +-+ ? ---
         xi = -xi;
         zeta = -zeta;
         signChanged = true;
         break;

      case 3:  // +-- ? depends on zeros
         if ((zeroPattern & 2) == 2) {  // +0-
            xi = -xi;
            eta = -eta;
         } else if ((zeroPattern & 1) == 1) {  // +-0
            xi = -xi;
            zeta = -zeta;
         } else {  // +-- ? +++
            eta = -eta;
            zeta = -zeta;
         }
         signChanged = true;
         break;

      case 4:  // -++ ? ---
         eta = -eta;
         zeta = -zeta;
         signChanged = true;
         break;

      case 5:  // -+- ? depends on zeros
         if ((zeroPattern & 4) == 4) {  // 0+-
            xi = -xi;
            eta = -eta;
         } else if ((zeroPattern & 1) == 1) {  // -+0
            eta = -eta;
            zeta = -zeta;
         } else {  // -+- ? +++
            xi = -xi;
            zeta = -zeta;
         }
         signChanged = true;
         break;

      case 6:  // --+ ? depends on zeros
         if ((zeroPattern & 4) == 4) {  // 0-+
            xi = -xi;
            zeta = -zeta;
         } else if ((zeroPattern & 2) == 2) {  // -0+
            eta = -eta;
            zeta = -zeta;
         } else {  // --+ ? +++
            xi = -xi;
            eta = -eta;
         }
         signChanged = true;
         break;

      case 7:  // --- : keep as is
         break;
      }

      if (signChanged) {
         totalSteps++;
         if (verbose) std::cout << "Steps 3/4: Sign normalization (MKnorm)" << std::endl;
      }


      // Step 5: If |xi| > B or (xi = B and 2eta < zeta) or (xi = -B and zeta < 0)
      // CORRECTED: Check xi = +B and xi = -B separately
      bool step5 = false;
      if (std::abs(xi) > B + tolerance) {
         step5 = true;
      }
      if (std::abs(xi - B) <= tolerance && 2 * eta < zeta - tolerance) {  // ? = +B
         step5 = true;
      }
      if (std::abs(xi + B) <= tolerance && zeta < -tolerance) {         // ? = -B
         step5 = true;
      }

      if (step5) {
         int s = sign(xi);
         C = B + C - s * xi;
         eta = eta - s * zeta;
         xi = -2 * B * s + xi;
         totalSteps++;
         if (verbose) std::cout << "Step 5: Adjust C,?,?, go to 1" << std::endl;
         step1_label = true;
         continue;
      }

      // Step 6: If |eta| > A or (eta = A and 2x < zeta) or (eta = -A and zeta < 0)
      // CORRECTED: Check ? = +A and ? = -A separately
      bool step6 = false;
      if (std::abs(eta) > A + tolerance) {
         step6 = true;
      }
      if (std::abs(eta - A) <= tolerance && 2 * xi < zeta - tolerance) {  // ? = +A
         step6 = true;
      }
      if (std::abs(eta + A) <= tolerance && zeta < -tolerance) {        // ? = -A
         step6 = true;
      }

      if (step6) {
         int s = sign(eta);
         C = A + C - s * eta;
         xi = xi - s * zeta;
         eta = -2 * A * s + eta;
         totalSteps++;
         if (verbose) std::cout << "Step 6: Adjust C,?,?, go to 1" << std::endl;
         step1_label = true;
         continue;
      }

      // Step 7: If |zeta| > A or (zeta = A and xi? < eta) or (zeta = -A and eta < 0)
      // CORRECTED: Check ? = +A and ? = -A separately
      bool step7 = false;
      if (std::abs(zeta) > A + tolerance) {
         step7 = true;
      }
      if (std::abs(zeta - A) <= tolerance && 2 * xi < eta - tolerance) {  // ? = +A
         step7 = true;
      }
      if (std::abs(zeta + A) <= tolerance && eta < -tolerance) {        // ? = -A
         step7 = true;
      }

      if (step7) {
         int s = sign(zeta);
         B = A + B - s * zeta;
         xi = xi - s * eta;
         zeta = -2 * A * s + zeta;
         totalSteps++;
         if (verbose) std::cout << "Step 7: Adjust B,?,?, go to 1" << std::endl;
         step1_label = true;
         continue;
      }

      // Step 8: If xi+eta+zeta+A+B < 0 or [xi+eta+zeta+A+B = 0 and 2(A+eta)+zeta > 0]
      double sum = xi + eta + zeta + A + B;
      bool step8 = false;
      if (sum < -tolerance) {
         step8 = true;
      }
      if (std::abs(sum) <= tolerance && 2 * (A + eta) + zeta > tolerance) {
         step8 = true;
      }

      if (step8) {
         C = A + B + C + xi + eta + zeta;
         xi = 2 * B + xi + zeta;
         eta = 2 * A + eta + zeta;
         totalSteps++;
         if (verbose) std::cout << "Step 8: Final adjustment, go to 1" << std::endl;
         step1_label = true;
         continue;
      }

      // If we reach here, algorithm has converged
      if (verbose) {
         std::cout << "\nConverged after " << mainIter << " main iterations, "
            << totalSteps << " total steps" << std::endl;
         std::cout << "Final: A=" << A << " B=" << B << " C=" << C
            << " ?=" << xi << " ?=" << eta << " ?=" << zeta << std::endl;
      }

      // Convert back to G6
      result.reducedG6[0] = A;
      result.reducedG6[1] = B;
      result.reducedG6[2] = C;
      result.reducedG6[3] = xi;
      result.reducedG6[4] = eta;
      result.reducedG6[5] = zeta;

      result.success = true;
      result.iterations = mainIter;
      result.totalSteps = totalSteps;

      if (verbose) {
         std::cout << "Result G6: " << result.reducedG6 << std::endl;
      }

      return result;
   }

   // Failed to converge
   if (verbose) {
      std::cout << "WARNING: Failed to converge after " << maxIterations
         << " iterations" << std::endl;
   }

   result.success = false;
   result.iterations = maxIterations;
   result.totalSteps = totalSteps;

   return result;
}

#endif // KRIVY_GRUBER_H

