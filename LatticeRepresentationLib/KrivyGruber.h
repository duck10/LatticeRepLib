// KrivyGruber.h - Corrected Implementation from 1976 Paper with 3D Matrix Tracking
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
      Matrix_3x3 m33;
      bool success;
      int iterations;
      int totalSteps;  // Total algorithm steps (for tracking)

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

   static ReductionResult Reduce(const G6& inputG6, double tolerance = 1.0e-6, bool verbose = false);
   static ReductionResult ReduceWithTransformation(const G6& inputG6, double tolerance = 1.0e-6, bool verbose = false);

private:
   // Helper: sign function (returns 1 for positive, -1 for negative)
   static int sign(double x) {
      return (x >= 0) ? 1 : -1;
   }

   // 3D transformation matrices corresponding to each Krivy-Gruber step
   // These match the operations in G6 space but operate on 3x3 lattice vectors

   // Identity matrix
   static const Matrix_3x3 spnull_3D;

   // Step 1: Swap (A,xi) <-> (B,eta) corresponds to swapping a and b vectors
   static const Matrix_3x3 sp1_3D;  // Swaps a <-> b

   // Step 2: Swap (B,eta) <-> (C,zeta) corresponds to swapping b and c vectors
   static const Matrix_3x3 sp2_3D;  // Swaps b <-> c

   // Sign change matrices for steps 3-4 (MKnorm sign normalization)
   static const Matrix_3x3 sp34a_3D;  // Negate xi and eta
   static const Matrix_3x3 sp34b_3D;  // Negate xi and zeta
   static const Matrix_3x3 sp34c_3D;  // Negate eta and zeta

   // Step 5: Operations on (C, eta, xi) when |xi| > B
   static const Matrix_3x3 R5_Plus_3D;
   static const Matrix_3x3 R5_Minus_3D;

   // Step 6: Operations on (C, xi, eta) when |eta| > A
   static const Matrix_3x3 R6_Plus_3D;
   static const Matrix_3x3 R6_Minus_3D;

   // Step 7: Operations on (B, xi, zeta) when |zeta| > A
   static const Matrix_3x3 R7_Plus_3D;
   static const Matrix_3x3 R7_Minus_3D;

   // Step 8: Final adjustment operation
   static const Matrix_3x3 R8_3D;
};

// Initialize 3D transformation matrices
// Identity
const Matrix_3x3 KrivyGruber::spnull_3D = Matrix_3x3(
   1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0
);

// CORRECTED sp1_3D (swaps a and b)
const Matrix_3x3 KrivyGruber::sp1_3D = Matrix_3x3(
   0.0, -1.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, -1.0
);

// CORRECTED sp2_3D (swaps b and c)
const Matrix_3x3 KrivyGruber::sp2_3D = Matrix_3x3(
   -1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, -1.0, 0.0
);

// CORRECTED sp34a_3D (sign changes for beta and gamma)
const Matrix_3x3 KrivyGruber::sp34a_3D = Matrix_3x3(
   -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 1.0
);

// sp34b_3D (sign changes)
const Matrix_3x3 KrivyGruber::sp34b_3D = Matrix_3x3(
   -1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0
);

// CORRECTED sp34c_3D (sign changes for gamma and xi)
const Matrix_3x3 KrivyGruber::sp34c_3D = Matrix_3x3(
   1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, -1.0
);

// Reduction operation matrices
const Matrix_3x3 KrivyGruber::R5_Plus_3D = Matrix_3x3(
   1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, -1.0, 1.0
);

const Matrix_3x3 KrivyGruber::R5_Minus_3D = Matrix_3x3(
   1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0
);

const Matrix_3x3 KrivyGruber::R6_Plus_3D = Matrix_3x3(
   1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, 0.0, 1.0
);

const Matrix_3x3 KrivyGruber::R6_Minus_3D = Matrix_3x3(
   1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0
);

const Matrix_3x3 KrivyGruber::R7_Plus_3D = Matrix_3x3(
   1.0, 0.0, 0.0, -1.0, 1.0, 0.0, 0.0, 0.0, 1.0
);

const Matrix_3x3 KrivyGruber::R7_Minus_3D = Matrix_3x3(
   1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0
);

const Matrix_3x3 KrivyGruber::R8_3D = Matrix_3x3(
   1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0
);

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

   // Step 0: Convert from G6 to (A, B, C, xi, eta, zeta) notation
   // G6: (g1, g2, g3, g4, g5, g6) where g1=a^2, g2=b^2, g3=c^2
   //     g4=2bc*cos(alpha), g5=2ac*cos(beta), g6=2ab*cos(gamma)
   // Krivy-Gruber: A=a^2, B=b^2, C=c^2, xi=2bc*cos(alpha), eta=2ac*cos(beta), zeta=2ab*cos(gamma)
   double A = inputG6[0];
   double B = inputG6[1];
   double C = inputG6[2];
   double xi = inputG6[3];    // xi
   double eta = inputG6[4];   // eta
   double zeta = inputG6[5];  // zeta

   const int maxIterations = 1000;
   int mainIter = 0;
   int totalSteps = 0;

   if (verbose) {
      std::cout << "\nInitial: A=" << A << " B=" << B << " C=" << C
         << " xi=" << xi << " eta=" << eta << " zeta=" << zeta << std::endl;
   }

   // Main algorithm loop
   bool step1_label = true;  // Start at step 1

   while (mainIter < maxIterations) {
      mainIter++;

      // Step 1: CORRECTED - swap (A,xi) <-> (B,eta)
      if (step1_label) {
         step1_label = false;
         if (A > B + tolerance || (std::abs(A - B) <= tolerance && std::abs(xi) > std::abs(eta) + tolerance)) {
            std::swap(A, B);
            std::swap(xi, eta);  // CORRECTED: was std::swap(zeta, eta)
            totalSteps++;
            if (verbose) std::cout << "Step 1: Swap (A,xi)<->(B,eta)" << std::endl;
         }
      }

      // Step 2: If B > C or (B = C and |eta| > |zeta|), swap (B,eta) <-> (C,zeta) and go to 1
      if (B > C + tolerance || (std::abs(B - C) <= tolerance && std::abs(eta) > std::abs(zeta) + tolerance)) {
         std::swap(B, C);
         std::swap(eta, zeta);
         totalSteps++;
         if (verbose) std::cout << "Step 2: Swap (B,eta)<->(C,zeta), go to 1" << std::endl;
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

      case 1:  // ++- → ---
         xi = -xi;
         eta = -eta;
         signChanged = true;
         break;

      case 2:  // +-+ → ---
         xi = -xi;
         zeta = -zeta;
         signChanged = true;
         break;

      case 3:  // +-- → depends on zeros
         if ((zeroPattern & 2) == 2) {  // +0-
            xi = -xi;
            eta = -eta;
         } else if ((zeroPattern & 1) == 1) {  // +-0
            xi = -xi;
            zeta = -zeta;
         } else {  // +-- → +++
            eta = -eta;
            zeta = -zeta;
         }
         signChanged = true;
         break;

      case 4:  // -++ → ---
         eta = -eta;
         zeta = -zeta;
         signChanged = true;
         break;

      case 5:  // -+- → depends on zeros
         if ((zeroPattern & 4) == 4) {  // 0+-
            xi = -xi;
            eta = -eta;
         } else if ((zeroPattern & 1) == 1) {  // -+0
            eta = -eta;
            zeta = -zeta;
         } else {  // -+- → +++
            xi = -xi;
            zeta = -zeta;
         }
         signChanged = true;
         break;

      case 6:  // --+ → depends on zeros
         if ((zeroPattern & 4) == 4) {  // 0-+
            xi = -xi;
            zeta = -zeta;
         } else if ((zeroPattern & 2) == 2) {  // -0+
            eta = -eta;
            zeta = -zeta;
         } else {  // --+ → +++
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
      if (std::abs(xi - B) <= tolerance && 2 * eta < zeta - tolerance) {  // xi = +B
         step5 = true;
      }
      if (std::abs(xi + B) <= tolerance && zeta < -tolerance) {         // xi = -B
         step5 = true;
      }

      if (step5) {
         int s = sign(xi);
         C = B + C - s * xi;
         eta = eta - s * zeta;
         xi = -2 * B * s + xi;
         totalSteps++;
         if (verbose) std::cout << "Step 5: Adjust C,eta,xi, go to 1" << std::endl;
         step1_label = true;
         continue;
      }

      // Step 6: If |eta| > A or (eta = A and 2xi < zeta) or (eta = -A and zeta < 0)
      // CORRECTED: Check eta = +A and eta = -A separately
      bool step6 = false;
      if (std::abs(eta) > A + tolerance) {
         step6 = true;
      }
      if (std::abs(eta - A) <= tolerance && 2 * xi < zeta - tolerance) {  // eta = +A
         step6 = true;
      }
      if (std::abs(eta + A) <= tolerance && zeta < -tolerance) {        // eta = -A
         step6 = true;
      }

      if (step6) {
         int s = sign(eta);
         C = A + C - s * eta;
         xi = xi - s * zeta;
         eta = -2 * A * s + eta;
         totalSteps++;
         if (verbose) std::cout << "Step 6: Adjust C,xi,eta, go to 1" << std::endl;
         step1_label = true;
         continue;
      }

      // Step 7: If |zeta| > A or (zeta = A and 2xi < eta) or (zeta = -A and eta < 0)
      // CORRECTED: Check zeta = +A and zeta = -A separately
      bool step7 = false;
      if (std::abs(zeta) > A + tolerance) {
         step7 = true;
      }
      if (std::abs(zeta - A) <= tolerance && 2 * xi < eta - tolerance) {  // zeta = +A
         step7 = true;
      }
      if (std::abs(zeta + A) <= tolerance && eta < -tolerance) {        // zeta = -A
         step7 = true;
      }

      if (step7) {
         int s = sign(zeta);
         B = A + B - s * zeta;
         xi = xi - s * eta;
         zeta = -2 * A * s + zeta;
         totalSteps++;
         if (verbose) std::cout << "Step 7: Adjust B,xi,zeta, go to 1" << std::endl;
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
            << " xi=" << xi << " eta=" << eta << " zeta=" << zeta << std::endl;
      }

      // Convert back to G6
      result.reducedG6[0] = A;
      result.reducedG6[1] = B;
      result.reducedG6[2] = C;
      result.reducedG6[3] = xi;
      result.reducedG6[4] = eta;
      result.reducedG6[5] = zeta;

      result.m33 = spnull_3D;  // Basic Reduce doesn't track transformations, return identity
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

   result.m33 = spnull_3D;  // Return identity on failure
   result.success = false;
   result.iterations = maxIterations;
   result.totalSteps = totalSteps;

   return result;
}

// ===================================================================
// ReduceWithTransformation - Tracks 3D matrix transformations
// ===================================================================

inline KrivyGruber::ReductionResult
KrivyGruber::ReduceWithTransformation(const G6& inputG6, double tolerance, bool verbose) {
   ReductionResult result;

   // Validate input
   LRL_Cell cell(inputG6);
   if (!cell.IsValid()) {
      if (verbose) std::cout << "ERROR: Invalid input cell" << std::endl;
      return result;
   }

   if (verbose) {
      std::cout << "Krivy & Gruber (1976) reduction with transformation tracking" << std::endl;
      std::cout << "Input G6: " << inputG6 << std::endl;
   }

   // Initialize transformation matrix to identity
   Matrix_3x3 m3d = spnull_3D;

   // Step 0: Convert from G6 to (A, B, C, xi, eta, zeta) notation
   double A = inputG6[0];
   double B = inputG6[1];
   double C = inputG6[2];
   double xi = inputG6[3];    // xi
   double eta = inputG6[4];   // eta
   double zeta = inputG6[5];  // zeta

   const int maxIterations = 1000;
   int mainIter = 0;
   int totalSteps = 0;

   if (verbose) {
      std::cout << "\nInitial: A=" << A << " B=" << B << " C=" << C
         << " xi=" << xi << " eta=" << eta << " zeta=" << zeta << std::endl;
   }

   // Main algorithm loop
   bool step1_label = true;  // Start at step 1

   while (mainIter < maxIterations) {
      mainIter++;

      // Step 1: CORRECTED - swap (A,xi) <-> (B,eta)
      if (step1_label) {
         step1_label = false;
         if (A > B + tolerance || (std::abs(A - B) <= tolerance && std::abs(xi) > std::abs(eta) + tolerance)) {
            std::swap(A, B);
            std::swap(xi, eta);
            m3d = sp1_3D * m3d;  // Accumulate transformation
            totalSteps++;
            if (verbose) std::cout << "Step 1: Swap (A,xi)<->(B,eta)" << std::endl;
         }
      }

      // Step 2: If B > C or (B = C and |eta| > |zeta|), swap (B,eta) <-> (C,zeta) and go to 1
      if (B > C + tolerance || (std::abs(B - C) <= tolerance && std::abs(eta) > std::abs(zeta) + tolerance)) {
         std::swap(B, C);
         std::swap(eta, zeta);
         m3d = sp2_3D * m3d;  // Accumulate transformation
         totalSteps++;
         if (verbose) std::cout << "Step 2: Swap (B,eta)<->(C,zeta), go to 1" << std::endl;
         step1_label = true;
         continue;  // Go to step 1
      }

      // Steps 3-4: Sign normalization (MKnorm)
      int minusPattern = 0;
      int zeroPattern = 0;

      if (xi <= tolerance) minusPattern |= 4;
      if (eta <= tolerance) minusPattern |= 2;
      if (zeta <= tolerance) minusPattern |= 1;

      if (std::abs(xi) <= tolerance) zeroPattern |= 4;
      if (std::abs(eta) <= tolerance) zeroPattern |= 2;
      if (std::abs(zeta) <= tolerance) zeroPattern |= 1;

      bool signChanged = false;
      Matrix_3x3 signMatrix = spnull_3D;

      switch (minusPattern) {
      case 0:  // +++ : keep as is
         break;

      case 1:  // ++- → ---
         xi = -xi;
         eta = -eta;
         signMatrix = sp34a_3D;
         signChanged = true;
         break;

      case 2:  // +-+ → ---
         xi = -xi;
         zeta = -zeta;
         signMatrix = sp34b_3D;
         signChanged = true;
         break;

      case 3:  // +-- → depends on zeros
         if ((zeroPattern & 2) == 2) {  // +0-
            xi = -xi;
            eta = -eta;
            signMatrix = sp34a_3D;
         } else if ((zeroPattern & 1) == 1) {  // +-0
            xi = -xi;
            zeta = -zeta;
            signMatrix = sp34b_3D;
         } else {  // +-- → +++
            eta = -eta;
            zeta = -zeta;
            signMatrix = sp34c_3D;
         }
         signChanged = true;
         break;

      case 4:  // -++ → ---
         eta = -eta;
         zeta = -zeta;
         signMatrix = sp34c_3D;
         signChanged = true;
         break;

      case 5:  // -+- → depends on zeros
         if ((zeroPattern & 4) == 4) {  // 0+-
            xi = -xi;
            eta = -eta;
            signMatrix = sp34a_3D;
         } else if ((zeroPattern & 1) == 1) {  // -+0
            eta = -eta;
            zeta = -zeta;
            signMatrix = sp34c_3D;
         } else {  // -+- → +++
            xi = -xi;
            zeta = -zeta;
            signMatrix = sp34b_3D;
         }
         signChanged = true;
         break;

      case 6:  // --+ → depends on zeros
         if ((zeroPattern & 4) == 4) {  // 0-+
            xi = -xi;
            zeta = -zeta;
            signMatrix = sp34b_3D;
         } else if ((zeroPattern & 2) == 2) {  // -0+
            eta = -eta;
            zeta = -zeta;
            signMatrix = sp34c_3D;
         } else {  // --+ → +++
            xi = -xi;
            eta = -eta;
            signMatrix = sp34a_3D;
         }
         signChanged = true;
         break;

      case 7:  // --- : keep as is
         break;
      }

      if (signChanged) {
         m3d = signMatrix * m3d;  // Accumulate transformation
         totalSteps++;
         if (verbose) std::cout << "Steps 3/4: Sign normalization (MKnorm)" << std::endl;
      }

      // Step 5: If |xi| > B or (xi = B and 2eta < zeta) or (xi = -B and zeta < 0)
      bool step5 = false;
      if (std::abs(xi) > B + tolerance) {
         step5 = true;
      }
      if (std::abs(xi - B) <= tolerance && 2 * eta < zeta - tolerance) {
         step5 = true;
      }
      if (std::abs(xi + B) <= tolerance && zeta < -tolerance) {
         step5 = true;
      }

      if (step5) {
         int s = sign(xi);
         C = B + C - s * xi;
         eta = eta - s * zeta;
         xi = -2 * B * s + xi;

         // Accumulate 3D transformation
         m3d = ((s > 0) ? R5_Plus_3D : R5_Minus_3D) * m3d;

         totalSteps++;
         if (verbose) std::cout << "Step 5: Adjust C,eta,xi, go to 1" << std::endl;
         step1_label = true;
         continue;
      }

      // Step 6: If |eta| > A or (eta = A and 2xi < zeta) or (eta = -A and zeta < 0)
      bool step6 = false;
      if (std::abs(eta) > A + tolerance) {
         step6 = true;
      }
      if (std::abs(eta - A) <= tolerance && 2 * xi < zeta - tolerance) {
         step6 = true;
      }
      if (std::abs(eta + A) <= tolerance && zeta < -tolerance) {
         step6 = true;
      }

      if (step6) {
         int s = sign(eta);
         C = A + C - s * eta;
         xi = xi - s * zeta;
         eta = -2 * A * s + eta;

         // Accumulate 3D transformation
         m3d = ((s > 0) ? R6_Plus_3D : R6_Minus_3D) * m3d;

         totalSteps++;
         if (verbose) std::cout << "Step 6: Adjust C,xi,eta, go to 1" << std::endl;
         step1_label = true;
         continue;
      }

      // Step 7: If |zeta| > A or (zeta = A and 2xi < eta) or (zeta = -A and eta < 0)
      bool step7 = false;
      if (std::abs(zeta) > A + tolerance) {
         step7 = true;
      }
      if (std::abs(zeta - A) <= tolerance && 2 * xi < eta - tolerance) {
         step7 = true;
      }
      if (std::abs(zeta + A) <= tolerance && eta < -tolerance) {
         step7 = true;
      }

      if (step7) {
         int s = sign(zeta);
         B = A + B - s * zeta;
         xi = xi - s * eta;
         zeta = -2 * A * s + zeta;

         // Accumulate 3D transformation
         m3d = ((s > 0) ? R7_Plus_3D : R7_Minus_3D) * m3d;

         totalSteps++;
         if (verbose) std::cout << "Step 7: Adjust B,xi,zeta, go to 1" << std::endl;
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

         // Accumulate 3D transformation
         m3d = R8_3D * m3d;

         totalSteps++;
         if (verbose) std::cout << "Step 8: Final adjustment, go to 1" << std::endl;
         step1_label = true;
         continue;
      }

      // Verify determinant
      const double det = m3d.Det();
      if (std::abs(det - 1.0) > 0.001) {
         if (verbose) {
            std::cout << "WARNING: 3D transformation determinant = " << det
               << " (should be 1.0)" << std::endl;
         }
      }
      if (det < 0.0) {
         if (verbose) {
            std::cout << "ERROR: Negative determinant in 3D transformation" << std::endl;
         }
      }

      // If we reach here, algorithm has converged
      if (verbose) {
         std::cout << "\nConverged after " << mainIter << " main iterations, "
            << totalSteps << " total steps" << std::endl;
         std::cout << "Final: A=" << A << " B=" << B << " C=" << C
            << " xi=" << xi << " eta=" << eta << " zeta=" << zeta << std::endl;
         std::cout << "Final determinant: " << m3d.Det() << std::endl;
      }

      // Convert back to G6
      result.reducedG6[0] = A;
      result.reducedG6[1] = B;
      result.reducedG6[2] = C;
      result.reducedG6[3] = xi;
      result.reducedG6[4] = eta;
      result.reducedG6[5] = zeta;

      result.m33 = m3d;
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

   result.m33 = m3d;
   result.success = false;
   result.iterations = maxIterations;
   result.totalSteps = totalSteps;

   return result;
}

// IsNiggli_KG.h
// Niggli condition checker based on Krivy-Gruber (1976) reduction conditions
// Reports which specific conditions are violated

// IsNiggli_KG.h
// Niggli condition checker based on Krivy-Gruber (1976) reduction conditions
// Reports which specific conditions are violated

// IsNiggli_KG.h
// Niggli condition checker based on Krivy-Gruber (1976) reduction conditions
// Reports which specific conditions are violated
/*
#ifndef ISNIGGLI_KG_H
#define ISNIGGLI_KG_H

#include "G6.h"
#include <vector>
#include <string>
#include <cmath>

struct NiggliViolation {
   std::string condition;
   std::string description;

   NiggliViolation(const std::string& cond, const std::string& desc)
      : condition(cond), description(desc) {
   }
};

class IsNiggli_KG {
public:
   // Main checking function - returns true if cell is Niggli reduced
   // Populates violations vector with any failed conditions
   static bool Check(const G6& v, std::vector<NiggliViolation>& violations, double tolerance = 1.0e-6) {
      violations.clear();

      // G6 notation: g1=a^2, g2=b^2, g3=c^2, g4=2bc*cos(alpha), g5=2ac*cos(beta), g6=2ab*cos(gamma)
      const double g1 = v[0];
      const double g2 = v[1];
      const double g3 = v[2];
      const double g4 = v[3];
      const double g5 = v[4];
      const double g6 = v[5];

      // Basic validity checks
      if (g1 <= 0.0) {
         violations.push_back(NiggliViolation("g1 > 0", "g1 must be positive"));
      }
      if (g2 <= 0.0) {
         violations.push_back(NiggliViolation("g2 > 0", "g2 must be positive"));
      }
      if (g3 <= 0.0) {
         violations.push_back(NiggliViolation("g3 > 0", "g3 must be positive"));
      }

      // Step 1 condition: g1 <= g2 (with tie-breaking when g1 ~= g2)
      if (g1 > g2 + tolerance) {
         violations.push_back(NiggliViolation("g1 <= g2",
            "g1 > g2 (should swap (g1,g4) <-> (g2,g5))"));
      } else if (std::abs(g1 - g2) <= tolerance && std::abs(g4) > std::abs(g5) + tolerance) {
         violations.push_back(NiggliViolation("g1 = g2 => |g4| <= |g5|",
            "When g1 ~= g2, require |g4| <= |g5| (tie-breaking)"));
      }

      // Step 2 condition: g2 <= g3 (with tie-breaking when g2 ~= g3)
      if (g2 > g3 + tolerance) {
         violations.push_back(NiggliViolation("g2 <= g3",
            "g2 > g3 (should swap (g2,g5) <-> (g3,g6))"));
      } else if (std::abs(g2 - g3) <= tolerance && std::abs(g5) > std::abs(g6) + tolerance) {
         violations.push_back(NiggliViolation("g2 = g3 => |g5| <= |g6|",
            "When g2 ~= g3, require |g5| <= |g6| (tie-breaking)"));
      }

      // Steps 3-4: Sign normalization (all +++ or all ---)
      int nneg = 0;
      if (g4 < -tolerance) nneg++;
      if (g5 < -tolerance) nneg++;
      if (g6 < -tolerance) nneg++;

      int npos = 0;
      if (g4 > tolerance) npos++;
      if (g5 > tolerance) npos++;
      if (g6 > tolerance) npos++;

      if (nneg != 0 && nneg != 3) {
         violations.push_back(NiggliViolation("Signs: all + or all -",
            "g4,g5,g6 must be all positive, all negative, or involve zeros"));
      }

      // Step 5 conditions: |g4| <= g2
      if (std::abs(g4) > g2 + tolerance) {
         violations.push_back(NiggliViolation("|g4| <= g2",
            "|g4| > g2 (violates Step 5)"));
      }
      // Special case: g4 = g2 and 2*g5 < g6
      if (std::abs(std::abs(g4) - g2) <= tolerance && 2 * g5 < g6 - tolerance) {
         violations.push_back(NiggliViolation("g4 = g2 => 2*g5 >= g6",
            "When |g4| = g2, require 2*g5 >= g6 (Step 5 tie-breaking)"));
      }
      // Special case: g4 = -g2 and g6 < 0
      if (std::abs(g4 + g2) <= tolerance && g6 < -tolerance) {
         violations.push_back(NiggliViolation("g4 = -g2 => g6 >= 0",
            "When g4 = -g2, require g6 >= 0 (Step 5 special case)"));
      }

      // Step 6 conditions: |g5| <= g1
      if (std::abs(g5) > g1 + tolerance) {
         violations.push_back(NiggliViolation("|g5| <= g1",
            "|g5| > g1 (violates Step 6)"));
      }
      // Special case: g5 = g1 and 2*g4 < g6
      if (std::abs(std::abs(g5) - g1) <= tolerance && 2 * g4 < g6 - tolerance) {
         violations.push_back(NiggliViolation("g5 = g1 => 2*g4 >= g6",
            "When |g5| = g1, require 2*g4 >= g6 (Step 6 tie-breaking)"));
      }
      // Special case: g5 = -g1 and g6 < 0
      if (std::abs(g5 + g1) <= tolerance && g6 < -tolerance) {
         violations.push_back(NiggliViolation("g5 = -g1 => g6 >= 0",
            "When g5 = -g1, require g6 >= 0 (Step 6 special case)"));
      }

      // Step 7 conditions: |g6| <= g1
      if (std::abs(g6) > g1 + tolerance) {
         violations.push_back(NiggliViolation("|g6| <= g1",
            "|g6| > g1 (violates Step 7)"));
      }
      // Special case: g6 = g1 and 2*g4 < g5
      if (std::abs(std::abs(g6) - g1) <= tolerance && 2 * g4 < g5 - tolerance) {
         violations.push_back(NiggliViolation("g6 = g1 => 2*g4 >= g5",
            "When |g6| = g1, require 2*g4 >= g5 (Step 7 tie-breaking)"));
      }
      // Special case: g6 = -g1 and g5 < 0
      if (std::abs(g6 + g1) <= tolerance && g5 < -tolerance) {
         violations.push_back(NiggliViolation("g6 = -g1 => g5 >= 0",
            "When g6 = -g1, require g5 >= 0 (Step 7 special case)"));
      }

      // Step 8 conditions: g4 + g5 + g6 + g1 + g2 >= 0
      double sum = g4 + g5 + g6 + g1 + g2;
      if (sum < -tolerance) {
         violations.push_back(NiggliViolation("g4 + g5 + g6 + g1 + g2 >= 0",
            "Sum of scalar products and squared lengths is negative (Step 8)"));
      }
      // Special case: sum = 0 and 2*(g1+g5)+g6 > 0
      if (std::abs(sum) <= tolerance && 2 * (g1 + g5) + g6 > tolerance) {
         violations.push_back(NiggliViolation("sum = 0 => 2*(g1+g5)+g6 <= 0",
            "When g4+g5+g6+g1+g2 = 0, require 2*(g1+g5)+g6 <= 0 (Step 8 tie-breaking)"));
      }

      // Additional condition: when all negative, sum should still be checked
      if (nneg == 3 && sum > tolerance) {
         violations.push_back(NiggliViolation("All negative => g4+g5+g6+g1+g2 <= 0",
            "When all g4,g5,g6 negative, sum g4+g5+g6+g1+g2 must be <= 0"));
      }

      return violations.empty();
   }

   //// Simple boolean check without violation details
   //static bool IsNiggli(const G6& v, double tolerance = 1.0e-6) {
   //   std::vector<NiggliViolation> violations;
   //   if (!violations.empty()) {
   //      std::cout << v << std::endl;
   //      PrintViolations(violations);
   //      const int i19191 = 19191l;
   //   }
   //   return Check(v, violations, tolerance);
   //}

   // Print violations to output stream
   static void PrintViolations(const std::vector<NiggliViolation>& violations, std::ostream& os = std::cout) {
      if (violations.empty()) {
         os << "Cell satisfies all Niggli conditions" << std::endl;
         return;
      }

      os << "Niggli condition violations (" << violations.size() << "):" << std::endl;
      for (size_t i = 0; i < violations.size(); ++i) {
         os << "  " << (i + 1) << ". " << violations[i].condition << std::endl;
         os << "     " << violations[i].description << std::endl;
      }
   }
};

#endif // ISNIGGLI_KG_H
*/
#endif // KRIVY_GRUBER_H
