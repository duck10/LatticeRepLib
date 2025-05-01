// TransformerUtilities.cpp
#include "TransformerUtilities.h"
#include "LRL_Cell_Degrees.h"
#include "LatticeCell.h"
#include "S6.h"
#include "Selling.h"
#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include <iostream>
#include <iomanip>

std::vector<Matrix_3x3> generateBasicPermutationMatrices() {
   std::vector<Matrix_3x3> permutationMatrices;

   // Basic permutation indices
   const int perms[6][3] = {
       {0, 1, 2}, // Identity
       {0, 2, 1}, // Swap b,c
       {1, 0, 2}, // Swap a,b
       {1, 2, 0}, // Cycle right
       {2, 0, 1}, // Cycle left
       {2, 1, 0}  // Swap a,c
   };

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
               int signs[3] = { signA, signB, signC };
               for (int i = 0; i < 3; i++) {
                  matrix[i * 3 + perms[p][i]] = signs[i];
               }

               permutationMatrices.push_back(matrix);
            }
         }
      }
   }

   return permutationMatrices;
}

void testSpecificMatricesWithB4(const LRL_Cell& sourceCell, const LRL_Cell& targetCell) {
   std::cout << "Testing specific transformation matrices using B4..." << std::endl;

   // Define matrices to test
   std::vector<Matrix_3x3> matricesToTest;

   // 1. Identity
   matricesToTest.push_back(Matrix_3x3(
      1, 0, 0,
      0, 1, 0,
      0, 0, 1
   ));

   // 2. Permutation (swap a and c)
   matricesToTest.push_back(Matrix_3x3(
      0, 0, 1,
      1, 0, 0,
      0, 1, 0
   ));

   // 3. Complex matrix from previous tests
   matricesToTest.push_back(Matrix_3x3(
      3, -3, -1,
      1, 2, 2,
      -3, -1, -2
   ));

   std::cout << "Testing " << matricesToTest.size() << " specific transformation matrices..." << std::endl;

   std::cout << "Source Cell:\n        " << LRL_Cell_Degrees(sourceCell) << std::endl;
   std::cout << "Target Cell:\n        " << LRL_Cell_Degrees(targetCell) << std::endl;

   std::cout << "\nTesting each transformation matrix:\n" << std::endl;

   // Create B4 representations
   const B4 sourceB4(sourceCell);
   const B4 targetB4(targetCell);

   // Test each matrix
   int count = 0;
   for (const auto& matrix : matricesToTest) {
      count++;
      std::cout << "Matrix " << count << ":" << std::endl;
      std::cout << matrix << std::endl;

      // Calculate determinant
      double det = matrix.Det();
      std::cout << "Determinant: " << det << std::endl;

      // Apply transformation directly to B4
      std::cout << sourceB4 << std::endl;
      const B4 transformedB4 = matrix * sourceB4;
      std::cout << "transformedB4 " << transformedB4 << std::endl;

      // Construct the transformed cell (for display only)
      const LatticeCell transformedCell = LatticeCell(transformedB4);

      std::cout << "Transformed Cell (B):\n      " << (transformedCell.getCell()) << std::endl;
      std::cout << "Transformed Cell (B):\n        " << LRL_Cell_Degrees(transformedCell.getCell()) << std::endl;

      // Calculate distances
      S6 s1 = S6(transformedCell.getCell());
      S6 s2 = S6(targetCell);
      S6 reduced1, reduced2;
      Selling::Reduce(s1, reduced1);
      Selling::Reduce(s2, reduced2);
      const double cs6Dist = CS6Dist(reduced1.data(), reduced2.data());

      // Calculate B4 distance
      const double b4Distance = B4::DistanceBetween(transformedB4, targetB4);

      // Calculate Euclidean distance (for reference)
      double euclideanDist = 0.0;
      for (int i = 0; i < 6; i++) {
         double diff = s1[i] - s2[i];
         euclideanDist += diff * diff;
      }
      euclideanDist = std::sqrt(euclideanDist);

      std::cout << "B4 Distance = " << b4Distance << std::endl;
      std::cout << "CS6Dist = " << cs6Dist << ", Euclidean Dist = " << euclideanDist << std::endl;

      std::cout << "--------------------------" << std::endl;
   }
}