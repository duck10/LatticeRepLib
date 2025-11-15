#ifndef GRUBER_1997_H
#define GRUBER_1997_H

#include <iostream>
#include <cmath>
#include "G6.h"

// Helper function to check if two values are approximately equal
inline bool approximately_equal(double a, double b, double eps = 1e-10) {
   return std::abs(a - b) < eps;
}

// Helper function for sign
inline int sign(double x) {
   return (x >= 0) ? 1 : -1;
}

// Helper function to calculate K = a² + b² + 2a·b + 2a·c + 2b·c
inline double calculate_K(const G6& g6) {
   return g6[0] + g6[1] + g6[5] + g6[4] + g6[3];
}

std::pair<G6, Matrix_3x3> NiggliReduce_Gruber1997(const G6& g6_input, const int maxCycles = 10000) {
   // Convert from G6 to working variables
   double A = g6_input[0];
   double B = g6_input[1];
   double C = g6_input[2];
   double xi = g6_input[3];
   double eta = g6_input[4];
   double zeta = g6_input[5];

   // Use the working 3D matrices from your Niggli code
   const Matrix_3x3 sp1_3D(0, -1, 0, -1, 0, 0, 0, 0, -1);
   const Matrix_3x3 sp2_3D(-1, 0, 0, 0, 0, -1, 0, -1, 0);
   const Matrix_3x3 sp34a_3D(-1, 0, 0, 0, -1, 0, 0, 0, 1);
   const Matrix_3x3 sp34b_3D(-1, 0, 0, 0, 1, 0, 0, 0, -1);
   const Matrix_3x3 sp34c_3D(1, 0, 0, 0, -1, 0, 0, 0, -1);
   const Matrix_3x3 R5_Plus_3D(1, 0, 0, 0, 1, 0, 0, -1, 1);
   const Matrix_3x3 R5_Minus_3D(1, 0, 0, 0, 1, 0, 0, 1, 1);
   const Matrix_3x3 R6_Plus_3D(1, 0, 0, 0, 1, 0, -1, 0, 1);
   const Matrix_3x3 R6_Minus_3D(1, 0, 0, 0, 1, 0, 1, 0, 1);
   const Matrix_3x3 R7_Plus_3D(1, 0, 0, -1, 1, 0, 0, 0, 1);
   const Matrix_3x3 R7_Minus_3D(1, 0, 0, 1, 1, 0, 0, 0, 1);
   const Matrix_3x3 R8_3D(1, 0, 0, 0, 1, 0, 1, 1, 1);

   Matrix_3x3 M_total(1, 0, 0, 0, 1, 0, 0, 0, 1);

   const double tolerance = 1e-6;
   int mainIter = 0;
   bool step1_label = true;

   while (mainIter < maxCycles) {
      mainIter++;

      // Step 1: Swap (A,?) ? (B,?)
      if (step1_label) {
         step1_label = false;
         if (A > B + tolerance || (std::abs(A - B) <= tolerance && std::abs(xi) > std::abs(eta) + tolerance)) {
            std::swap(A, B);
            std::swap(xi, eta);
            M_total = sp1_3D * M_total;
         }
      }

      // Step 2: Swap (B,?) ? (C,?) and go to 1
      if (B > C + tolerance || (std::abs(B - C) <= tolerance && std::abs(eta) > std::abs(zeta) + tolerance)) {
         std::swap(B, C);
         std::swap(eta, zeta);
         M_total = sp2_3D * M_total;
         step1_label = true;
         continue;
      }

      // Steps 3-4: Sign normalization
      int minusPattern = 0;
      int zeroPattern = 0;

      if (xi < -tolerance) minusPattern |= 4;
      if (eta < -tolerance) minusPattern |= 2;
      if (zeta < -tolerance) minusPattern |= 1;

      if (std::abs(xi) < tolerance) zeroPattern |= 4;
      if (std::abs(eta) < tolerance) zeroPattern |= 2;
      if (std::abs(zeta) < tolerance) zeroPattern |= 1;

      bool signChanged = false;
      Matrix_3x3 M_sign(1, 0, 0, 0, 1, 0, 0, 0, 1);

      switch (minusPattern) {
      case 0:  // +++
         break;

      case 1:  // ++- ? ---
         xi = -xi;
         eta = -eta;
         M_sign = sp34a_3D;
         signChanged = true;
         break;

      case 2:  // +-+ ? ---
         xi = -xi;
         zeta = -zeta;
         M_sign = sp34b_3D;
         signChanged = true;
         break;

      case 3:  // +--
         if ((zeroPattern & 2) == 2) {
            xi = -xi;
            eta = -eta;
            M_sign = sp34a_3D;
         } else if ((zeroPattern & 1) == 1) {
            xi = -xi;
            zeta = -zeta;
            M_sign = sp34b_3D;
         } else {
            eta = -eta;
            zeta = -zeta;
            M_sign = sp34c_3D;
         }
         signChanged = true;
         break;

      case 4:  // -++ ? ---
         eta = -eta;
         zeta = -zeta;
         M_sign = sp34c_3D;
         signChanged = true;
         break;

      case 5:  // -+-
         if ((zeroPattern & 4) == 4) {
            xi = -xi;
            eta = -eta;
            M_sign = sp34a_3D;
         } else if ((zeroPattern & 1) == 1) {
            eta = -eta;
            zeta = -zeta;
            M_sign = sp34c_3D;
         } else {
            xi = -xi;
            zeta = -zeta;
            M_sign = sp34b_3D;
         }
         signChanged = true;
         break;

      case 6:  // --+
         if ((zeroPattern & 4) == 4) {
            xi = -xi;
            zeta = -zeta;
            M_sign = sp34b_3D;
         } else if ((zeroPattern & 2) == 2) {
            eta = -eta;
            zeta = -zeta;
            M_sign = sp34c_3D;
         } else {
            xi = -xi;
            eta = -eta;
            M_sign = sp34a_3D;
         }
         signChanged = true;
         break;

      case 7:  // ---
         break;
      }

      if (signChanged) {
         M_total = M_sign * M_total;
      }

      // Step 5
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
         M_total = (s < 0 ? R5_Minus_3D : R5_Plus_3D) * M_total;
         step1_label = true;
         continue;
      }

      // Step 6
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
         M_total = (s < 0 ? R6_Minus_3D : R6_Plus_3D) * M_total;
         step1_label = true;
         continue;
      }

      // Step 7
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
         M_total = (s < 0 ? R7_Minus_3D : R7_Plus_3D) * M_total;
         step1_label = true;
         continue;
      }

      // Step 8
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
         M_total = R8_3D * M_total;
         step1_label = true;
         continue;
      }

      // Converged
      break;
   }

   // Convert back to G6
   G6 result;
   result[0] = A;
   result[1] = B;
   result[2] = C;
   result[3] = xi;
   result[4] = eta;
   result[5] = zeta;

   // Final sign normalization : enforce Niggli convention
   // If we can't have all positive (because of zeros or special geometry),
   // ensure all are non-positive
   const bool hasZero = (std::abs(xi) < tolerance) || (std::abs(eta) < tolerance) || (std::abs(zeta) < tolerance);
   bool allPositive = (xi > tolerance) && (eta > tolerance) && (zeta > tolerance);
   bool allNonPositive = (xi < -tolerance || std::abs(xi) < tolerance) &&
      (eta < -tolerance || std::abs(eta) < tolerance) &&
      (zeta < -tolerance || std::abs(zeta) < tolerance);

   if (!allPositive && !allNonPositive) {
      // Need to flip signs to make consistent
      // If there's a zero, make all non-positive
      if (hasZero) {
         if (xi > tolerance) {
            xi = -xi;
            eta = -eta;
            M_total = sp34a_3D * M_total;
         } else if (eta > tolerance) {
            xi = -xi;
            zeta = -zeta;
            M_total = sp34b_3D * M_total;
         } else if (zeta > tolerance) {
            eta = -eta;
            zeta = -zeta;
            M_total = sp34c_3D * M_total;
         }
      }
   }

   // NOW convert to G6
   result[0] = A;
   result[1] = B;
   result[2] = C;
   result[3] = xi;   // These should now be negative
   result[4] = eta;  // These should now be negative
   result[5] = zeta; // This should be zero

   return std::make_pair(result, M_total);
}


#endif // GRUBER_1997_H

