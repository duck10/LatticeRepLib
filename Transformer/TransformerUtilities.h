// TransformerUtilities.h
#ifndef TRANSFORMER_UTILITIES_H
#define TRANSFORMER_UTILITIES_H

#include "LRL_Vector3.h"
#include "LRL_Cell.h"
#include "B4.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_StringTools.h"
#include "LatticeCell.h"
#include "S6.h"
#include "Selling.h"
#include "CS6Dist.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>

static constexpr double M_PI = 3.14159265358979323846;


// Using inline functions prevents multiple definition errors
inline std::vector<Matrix_3x3> generateBasicPermutationMatrices() {
   std::vector<Matrix_3x3> permutationMatrices;

   // Basic permutation indices with only even permutations (det = +1)
   const int evenPerms[3][3] = {
       {0, 1, 2}, // Identity (a,b,c) - even
       {1, 2, 0}, // Cycle right (b,c,a) - even
       {2, 0, 1}  // Cycle left (c,a,b) - even
   };

   // Generate matrices with det = +1
   for (int p = 0; p < 3; p++) {
      for (int signA = -1; signA <= 1; signA += 2) {
         for (int signB = -1; signB <= 1; signB += 2) {
            for (int signC = -1; signC <= 1; signC += 2) {
               // Only accept sign combinations with product = +1
               if (signA * signB * signC != 1) continue;

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
                  matrix[i * 3 + evenPerms[p][i]] = signs[i];
               }

               // Double-check the determinant
               const double det = matrix.Det();
               if (det > 0.0) {
                  permutationMatrices.push_back(matrix);
               }
            }
         }
      }
   }

   return permutationMatrices;
}


inline void testSpecificMatricesWithB4(const LRL_Cell& sourceCell, const LRL_Cell& targetCell) {
   std::cout << "Testing specific transformation matrices using B4..." << std::endl;

   // Define matrices to test
   std::vector<Matrix_3x3> matricesToTest;

   // 1. Identity
   Matrix_3x3 identity(
      1, 0, 0,
      0, 1, 0,
      0, 0, 1
   );
   double detIdentity = identity.Det();
   if (std::abs(detIdentity - 1.0) < 1e-10) {
      matricesToTest.push_back(identity);
   }
   else {
      std::cout << "Skipping identity matrix with determinant " << detIdentity << std::endl;
   }

   // 2. Cyclic permutation
   Matrix_3x3 permCyclic(
      0, 0, 1,
      1, 0, 0,
      0, 1, 0
   );
   double detCyclic = permCyclic.Det();
   if (std::abs(detCyclic - 1.0) < 1e-10) {
      matricesToTest.push_back(permCyclic);
   }
   else {
      std::cout << "Skipping cyclic permutation matrix with determinant " << detCyclic << std::endl;
   }

   // 3. Complex matrix
   Matrix_3x3 complex(
      3, -3, -1,
      1, 2, 2,
      -3, -1, -2
   );
   double detComplex = complex.Det();
   if (std::abs(detComplex - 1.0) < 1e-10) {
      matricesToTest.push_back(complex);
   }
   else {
      std::cout << "Skipping complex matrix with determinant " << detComplex << std::endl;
   }

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

// For backward compatibility
inline void testTransformationMatrices(const LRL_Cell& sourceCell, const LRL_Cell& targetCell) {
   testSpecificMatricesWithB4(sourceCell, targetCell);
}

inline std::vector<Matrix_3x3> generateAdvancedTransformationMatrices(int complexity = 2) {
   std::set<Matrix_3x3> matrixSet; // Use set to automatically eliminate duplicates

   // Start with the identity matrix
   Matrix_3x3 identity;
   for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
         identity[i * 3 + j] = (i == j) ? 1.0 : 0.0;
      }
   }
   matrixSet.insert(identity);

   // Generate elementary unimodular matrices
   // These are the "elementary row operations" that preserve det = +1

   // 1. Row swaps with sign flips (to keep det = +1)
   for (int i = 0; i < 3; i++) {
      for (int j = i + 1; j < 3; j++) {
         Matrix_3x3 swapMatrix = identity;
         swapMatrix[i * 3 + i] = 0.0;
         swapMatrix[i * 3 + j] = 1.0;
         swapMatrix[j * 3 + i] = 1.0;
         swapMatrix[j * 3 + j] = 0.0;

         // Double-check determinant
         double det = swapMatrix.Det();
         if (det < 0.0) {
            // Flip a sign to make det = +1
            swapMatrix[i * 3 + j] = -1.0;
         }
         matrixSet.insert(swapMatrix);
      }
   }

   // 2. Add multiples of one row to another
   // For each coefficient from -complexity to +complexity
   for (int coeff = -complexity; coeff <= complexity; coeff++) {
      // Skip 0
      if (coeff == 0) continue;

      for (int i = 0; i < 3; i++) {
         for (int j = 0; j < 3; j++) {
            if (i == j) continue; // Skip diagonal

            Matrix_3x3 addMatrix = identity;
            addMatrix[i * 3 + j] = coeff;
            matrixSet.insert(addMatrix);
         }
      }
   }

   // 3. Generate combined transformations by matrix multiplication
   // Take products of the elementary matrices (up to a certain number of combinations)
   const int MAX_COMBINATIONS = 3;
   std::vector<Matrix_3x3> baseMatrices(matrixSet.begin(), matrixSet.end()); // Convert set to vector

   for (int combo = 2; combo <= MAX_COMBINATIONS; combo++) {
      std::set<Matrix_3x3> newMatrixSet;

      // Multiply each existing matrix by each elementary matrix
      for (const auto& baseMatrix : matrixSet) {
         for (const auto& elemMatrix : baseMatrices) {
            Matrix_3x3 productMatrix = baseMatrix * elemMatrix;

            // Check if matrix has reasonable entries (not too large)
            bool isReasonable = true;
            for (int i = 0; i < 9; i++) {
               if (std::abs(productMatrix[i]) > complexity * 2) {
                  isReasonable = false;
                  break;
               }
            }

            if (isReasonable) {
               // Ensure all matrices have integer entries (within numerical precision)
               for (int i = 0; i < 9; i++) {
                  productMatrix[i] = std::round(productMatrix[i]);
               }

               // Final check of determinant
               double det = productMatrix.Det();
               if (std::abs(det - 1.0) < 1e-10) {
                  newMatrixSet.insert(productMatrix);
               }
            }
         }
      }

      // Add the new matrices to our set
      matrixSet.insert(newMatrixSet.begin(), newMatrixSet.end());

      // Limit the total number of matrices
      if (matrixSet.size() > 1000) {
         break;
      }
   }

   // Convert set back to vector for return
   return std::vector<Matrix_3x3>(matrixSet.begin(), matrixSet.end());
}


inline std::vector<Matrix_3x3> generateBodyDiagonalTransformations(int complexity = 2) {
   std::set<Matrix_3x3> matrixSet; // Use set to eliminate duplicates

   // Basic transformations that involve the body diagonal

   // Transformations of type: c' = -a-b+c (and permutations)
   Matrix_3x3 m1(
      1, 0, 0,
      0, 1, 0,
      -1, -1, 1
   );
   matrixSet.insert(m1);

   // Additional key transformations for handling obtuse/acute angles
   Matrix_3x3 m2(
      -1, 0, 0,
      0, -1, 0,
      1, 1, 1
   );
   matrixSet.insert(m2);

   Matrix_3x3 m3(
      -1, 0, 0,
      0, 1, 0,
      1, -1, -1
   );
   matrixSet.insert(m3);

   Matrix_3x3 m4(
      1, 0, 0,
      0, -1, 0,
      -1, 1, -1
   );
   matrixSet.insert(m4);

   // Generate cyclic permutations of these basic transformations
   std::vector<Matrix_3x3> baseMatrices(matrixSet.begin(), matrixSet.end());
   for (const Matrix_3x3& base : baseMatrices) {
      // Cyclic permutation patterns
      Matrix_3x3 cycle1 = Matrix_3x3(
         0, 0, 1,
         1, 0, 0,
         0, 1, 0
      ) * base;

      Matrix_3x3 cycle2 = Matrix_3x3(
         0, 1, 0,
         0, 0, 1,
         1, 0, 0
      ) * base;

      if (std::abs(cycle1.Det() - 1.0) < 1e-10) matrixSet.insert(cycle1);
      if (std::abs(cycle2.Det() - 1.0) < 1e-10) matrixSet.insert(cycle2);
   }

   // Add compound transformations if complexity > 1
   if (complexity > 1) {
      std::vector<Matrix_3x3> compoundMatrices;
      baseMatrices = std::vector<Matrix_3x3>(matrixSet.begin(), matrixSet.end());

      for (size_t i = 0; i < baseMatrices.size(); ++i) {
         for (size_t j = i; j < baseMatrices.size(); ++j) {
            Matrix_3x3 compound = baseMatrices[i] * baseMatrices[j];
            if (std::abs(compound.Det() - 1.0) < 1e-10) {
               compoundMatrices.push_back(compound);
            }

            // Try reverse order too
            compound = baseMatrices[j] * baseMatrices[i];
            if (std::abs(compound.Det() - 1.0) < 1e-10) {
               compoundMatrices.push_back(compound);
            }
         }
      }

      // Add valid compound matrices
      for (const Matrix_3x3& m : compoundMatrices) {
         matrixSet.insert(m);
      }
   }

   // Check validity and convert set to vector
   std::vector<Matrix_3x3> validMatrices;
   for (const auto& matrix : matrixSet) {
      if (std::abs(matrix.Det() - 1.0) < 1e-10) {
         validMatrices.push_back(matrix);
      }
   }

   return validMatrices;
}


// Add to LatticeMatcher.h or B4Matcher.h
static double angleS6(const S6& s1, const S6& s2) {
   const S6 s1n = s1 / s1.Norm();
   const S6 s2n = s2 / s2.Norm();

   double cosFit =
      s1n[0] * s2n[0] + s1n[1] * s2n[1] +
      s1n[2] * s2n[2] + s1n[3] * s2n[3] +
      s1n[4] * s2n[4] + s1n[5] * s2n[5];

   cosFit = std::min(std::max(cosFit, -1.0), 1.0);  // Ensure in valid range
   const double angle = acos(cosFit) * 180.0 / M_PI;
   return angle;
}

// None - Primitive(P)
static const Matrix_3x3 primitive{
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0
};

// Body - Centered(I)
static const Matrix_3x3 bodyCentered{
    0.5, 0.5, 0.5,
    -0.5, 0.5, 0.5,
    -0.5, -0.5, 0.5
};

// Face - Centered(F)
static const Matrix_3x3 faceCentered{
    0.5, 0.0, 0.5,
    0.5, 0.5, 0.0,
    0.0, 0.5, 0.5
};

// Base - Centered(A) - correctly places lattice points at the centers of the bc faces
static const Matrix_3x3 aCentered{
    1.0, 0.0, 0.0,
    0.0, 0.5, 0.5,
    0.0, -0.5, 0.5
};

// Base - Centered(B) - correctly places lattice points at the centers of the ac faces
static const Matrix_3x3 bCentered{
    0.5, 0.0, 0.5,
    0.0, 1.0, 0.0,
    -0.5, 0.0, 0.5
};

// Base - Centered(C) - correctly places lattice points at the centers of the ab faces
static const Matrix_3x3 cCentered{
    0.5, 0.5, 0.0,
    -0.5, 0.5, 0.0,
    0.0, 0.0, 1.0
};

// Rhombohedral(R)
static const Matrix_3x3 rhombohedral{
    2.0 / 3.0, -1.0 / 3.0, -1.0 / 3.0,
    1.0 / 3.0,  1.0 / 3.0, -2.0 / 3.0,
    1.0 / 3.0,  1.0 / 3.0,  1.0 / 3.0
};

static Matrix_3x3 ToPrimitive(const std::string& s, const LRL_Cell& cell) {
   const std::string upper = LRL_StringTools::strToupper(s);
   if (s.empty() || s == "P") return primitive;
   if (s == "I") return bodyCentered;
   if (s == "F") return faceCentered;
   if (s == "A") return aCentered;
   if (s == "B") return bCentered;
   if (s == "C") return cCentered;
   if (s == "H" || s == "R") {
      const double& alpha = cell[3];
      const double& beta = cell[4];
      const double& gamma = cell[5];
      if ((gamma - 120.0 / 180.0 * M_PI) > 1.0E-2) {
         return primitive;
      }
      else {
         return rhombohedral;
      }
   }
   // unknown input or empty lattice
   return primitive;
}

static Matrix_3x3 ToPrimitive(const LatticeCell& cell) {
   return ToPrimitive(cell.getLatticeType(), cell.getCell());
}

// In LRL_Cell.h or a new utility header
inline LRL_Cell operator*(const Matrix_3x3& matrix, const LRL_Cell& cell) {
   // Convert LRL_Cell to B4
   B4 b4Cell(cell);

   // Apply transformation
   B4 transformedB4 = matrix * b4Cell;

   // Convert back to LRL_Cell
   return LRL_Cell(transformedB4);
}


#endif // TRANSFORMER_UTILITIES_H

