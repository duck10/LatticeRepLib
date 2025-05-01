#ifndef MATRIX_VERIFIER_H
#define MATRIX_VERIFIER_H

#include "LRL_Vector3.h"
#include <iostream>
#include <set>

// Custom comparator for Matrix_3x3
struct Matrix3x3Comparator {
   bool operator()(const Matrix_3x3& lhs, const Matrix_3x3& rhs) const {
      constexpr double tolerance = 1e-10;
      for (int i = 0; i < 9; ++i) {
         if (std::abs(lhs[i] - rhs[i]) > tolerance) {
            return lhs[i] < rhs[i];
         }
      }
      return false;  // Matrices are equal
   }
};

// Standalone function to verify permutation matrices
void verifyPermutationMatrices() {
   std::cout << "Verifying permutation matrix generation..." << std::endl;

   // Basic permutation indices (6 permutations)
   const int perms[6][3] = {
       {0, 1, 2}, // Identity
       {0, 2, 1}, // Swap b,c
       {1, 0, 2}, // Swap a,b
       {1, 2, 0}, // Cycle right
       {2, 0, 1}, // Cycle left
       {2, 1, 0}  // Swap a,c
   };

   // Store unique matrices in a set
   std::set<Matrix_3x3, Matrix3x3Comparator> uniqueMatrices;

   // Count matrices by permutation type
   int countsByPermType[6] = { 0, 0, 0, 0, 0, 0 };

   // Generate all basic transformation matrices with determinant +1
   for (int p = 0; p < 6; p++) {
      for (int signA = -1; signA <= 1; signA += 2) {
         for (int signB = -1; signB <= 1; signB += 2) {
            for (int signC = -1; signC <= 1; signC += 2) {
               // Calculate permutation parity
               int inversions = 0;
               for (int i = 0; i < 3; i++) {
                  for (int j = i + 1; j < 3; j++) {
                     if (perms[p][i] > perms[p][j]) {
                        inversions++;
                     }
                  }
               }
               bool isOdd = (inversions % 2 == 1);

               // Calculate determinant sign
               int det = signA * signB * signC;
               if (isOdd) det = -det;

               // Only accept matrices with det = +1
               if (det != 1) continue;

               // Create matrix
               Matrix_3x3 matrix;
               // Zero out the matrix
               for (int i = 0; i < 3; i++) {
                  for (int j = 0; j < 3; j++) {
                     matrix[i * 3 + j] = 0.0;
                  }
               }

               // Set the permuted and sign-flipped elements
               int sign[3] = { signA, signB, signC };
               for (int i = 0; i < 3; i++) {
                  matrix[i * 3 + perms[p][i]] = sign[i];
               }

               // Verify determinant using the existing function
               if (!isValidTransformationMatrix(matrix)) {
                  std::cout << "ERROR: Matrix generated with incorrect determinant" << std::endl;
                  continue;
               }

               // Insert into set of unique matrices
               uniqueMatrices.insert(matrix);

               // Count by permutation type
               countsByPermType[p]++;
            }
         }
      }
   }

   // Display statistics
   std::cout << "Generated " << uniqueMatrices.size() << " unique permutation matrices with det=+1" << std::endl;
   std::cout << "Expected count: 24" << std::endl;

   // Display counts by permutation type
   std::cout << "Counts by permutation type:" << std::endl;
   std::cout << "  Identity:  " << countsByPermType[0] << std::endl;
   std::cout << "  Swap b,c:  " << countsByPermType[1] << std::endl;
   std::cout << "  Swap a,b:  " << countsByPermType[2] << std::endl;
   std::cout << "  Cycle right: " << countsByPermType[3] << std::endl;
   std::cout << "  Cycle left:  " << countsByPermType[4] << std::endl;
   std::cout << "  Swap a,c:  " << countsByPermType[5] << std::endl;

   // Display all matrices
   std::cout << "\nAll unique matrices with determinant +1:" << std::endl;
   int count = 0;
   for (const auto& matrix : uniqueMatrices) {
      count++;
      std::cout << "Matrix " << count << ":" << std::endl;
      std::cout << matrix << std::endl;

      // Calculate determinant
      double det = matrix.Det();
      std::cout << "  Determinant: " << det << std::endl;

      // Count non-zero elements
      int nonZeroCount = 0;
      for (int i = 0; i < 9; i++) {
         if (std::abs(matrix[i]) > 1e-10) nonZeroCount++;
      }

      // Verify this is a valid permutation matrix (exactly 3 non-zero elements)
      if (nonZeroCount != 3) {
         std::cout << "  ERROR: Not a valid permutation matrix! Has " << nonZeroCount << " non-zero elements." << std::endl;
      }

      std::cout << std::endl;
   }
}
#endif // MATRIX_VERIFIER_H

