#ifndef TRANSFORMER_UTILITIES_H
#define TRANSFORMER_UTILITIES_H

#include "LRL_Cell.h"
#include "Matrix_3x3.h"
#include "B4.h"
#include "P3.h"
#include "S6.h"
#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include "LatticeCell.h"
#include "Selling.h"
#include "MultiTransformFinderControls.h"
#include "LRL_StringTools.h"
#include <vector>
#include <cmath>
#include <iostream>

// Define M_PI if it's not already defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


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

// Function to convert from centered to primitive lattice
inline Matrix_3x3 ToPrimitive(const std::string& s, const LRL_Cell& cell) {
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



// Function to display detailed transformation information
inline void showTransformationInfo(const LatticeCell& mobileCell, const LatticeCell& referenceCell) {
   std::cout << "\n--- TRANSFORMATION DETAILS ---" << std::endl;

   // Show centering type information
   std::cout << "Mobile cell centering: " <<
      (mobileCell.getLatticeType().empty() ? "P" : mobileCell.getLatticeType()) << std::endl;
   std::cout << "Reference cell centering: " <<
      (referenceCell.getLatticeType().empty() ? "P" : referenceCell.getLatticeType()) << std::endl;

   // Show cells in G6 representation
   G6 mobileG6 = mobileCell.toPrimitive();
   G6 refG6 = referenceCell.toPrimitive();
   std::cout << "Mobile cell G6: " << mobileG6 << std::endl;
   std::cout << "Reference cell G6: " << refG6 << std::endl;

   // Show cells in S6 representation
   S6 mobileS6 = S6(mobileCell.getCell());
   S6 refS6 = S6(referenceCell.getCell());
   std::cout << "Mobile cell S6: " << mobileS6 << std::endl;
   std::cout << "Reference cell S6: " << refS6 << std::endl;

   // Calculate the centering matrices
   Matrix_3x3 mobileToPrimitive = ToPrimitive(mobileCell.getLatticeType(), mobileCell.getCell());
   Matrix_3x3 refToPrimitive = ToPrimitive(referenceCell.getLatticeType(), referenceCell.getCell());

   std::cout << "Mobile-to-primitive matrix:" << std::endl;
   std::cout << mobileToPrimitive << std::endl;

   std::cout << "Reference-to-primitive matrix:" << std::endl;
   std::cout << refToPrimitive << std::endl;

   // Show Niggli-reduced cell information
   G6 mobileNiggli = mobileCell.getNiggliReducedCell();
   G6 refNiggli = referenceCell.getNiggliReducedCell();
   std::cout << "Mobile cell (Niggli reduced): " << mobileNiggli << std::endl;
   std::cout << "Reference cell (Niggli reduced): " << refNiggli << std::endl;

   std::cout << "--- END TRANSFORMATION DETAILS ---\n" << std::endl;
}

// Function to show usage information
inline void showUsageInformation(const MultiTransformFinderControls& controls) {
   std::cout << "Error: Need at least two cells for transformation." << std::endl;
   std::cout << "Usage: Transformer [options] [reference_cell] [mobile_cell1] [mobile_cell2] ..." << std::endl;
   std::cout << "Options:" << std::endl;
   std::cout << "  SHOWDETAILS or DETAILS [true/false]  - Show detailed output" << std::endl;
   std::cout << "  SHOWTRANSFORMINFO [true/false]       - Show extra transformation details" << std::endl;
   std::cout << "  MATRIXSEARCHDEPTH [number]           - Set maximum search depth for matrices" << std::endl;
   std::cout << "  MAXTRANSFORMS [number]               - Set maximum transformations to display" << std::endl;
   std::cout << "  MAXDISTANCE [number]                 - Set maximum allowed transformation distance" << std::endl;
   std::cout << "  DISTANCETHRESHOLD [number]           - Set distance threshold for transformations" << std::endl;
   std::cout << "  INCLUDEIDENTITY [true/false]         - Include identity matrix in results" << std::endl;
   std::cout << "  SHOWCONTROLS or SHOW [true/false]    - Show control settings" << std::endl;
}

// Generate basic permutation matrices
inline std::vector<Matrix_3x3> generateBasicPermutationMatrices() {
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

inline void testSpecificMatricesWithB4(const LRL_Cell& sourceCell, const LRL_Cell& targetCell) {
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

// Calculate S6 angle between two S6 vectors
inline double angleS6(const S6& s1, const S6& s2) {
   // Make reduced copies to ensure we're comparing properly reduced cells
   S6 reduced1, reduced2;
   Selling::Reduce(s1, reduced1);
   Selling::Reduce(s2, reduced2);

   // Normalize the vectors for angle calculation
   const double norm1 = reduced1.norm();
   const double norm2 = reduced2.norm();

   if (norm1 < 1e-10 || norm2 < 1e-10) {
      return 0.0; // Handle zero or near-zero vectors
   }

   S6 normalized1 = reduced1 * (1.0 / norm1);
   S6 normalized2 = reduced2 * (1.0 / norm2);

   // Calculate dot product
   double dotProduct = 0.0;
   for (int i = 0; i < 6; i++) {
      dotProduct += normalized1[i] * normalized2[i];
   }

   // Clamp to valid range for acos
   if (dotProduct > 1.0) dotProduct = 1.0;
   if (dotProduct < -1.0) dotProduct = -1.0;

   // Convert to degrees
   return acos(dotProduct) * 180.0 / M_PI;
}

// Calculate P3 distance between two cells
// This is a simplified version that assumes P3 has appropriate distance calculation
inline double getP3Distance(const LRL_Cell& cell1, const LRL_Cell& cell2) {
   P3 p1(cell1);
   P3 p2(cell2);
   return P3::DistanceBetween(p1, p2);
}

//// Check if a matrix is a valid transformation matrix (det = +1)
//inline bool isValidTransformationMatrix(const Matrix_3x3& matrix) {
//   const double det = matrix.Det();
//   const double epsilon = 1e-10;
//   return std::abs(det - 1.0) < epsilon;
//}

inline LRL_Cell operator*(const Matrix_3x3& matrix, const LRL_Cell& cell) {
   // Convert cell to B4 representation
   B4 b4Cell(cell);

   // Apply transformation in B4 space
   B4 transformedB4 = matrix * b4Cell;

   // Convert back to LRL_Cell
   return LRL_Cell(transformedB4);
}

// Function to get the complete transformation matrix from mobile cell to reference cell
inline Matrix_3x3 getCompleteTransformationMatrix(
   const LatticeCell& mobileCell,
   const LatticeCell& referenceCell,
   const Matrix_3x3& primitiveToPrivitiveMatrix) {

   // Get the centering matrices
   Matrix_3x3 mobileToPrimitive = ToPrimitive(mobileCell.getLatticeType(), mobileCell.getCell());
   Matrix_3x3 refToPrimitive = ToPrimitive(referenceCell.getLatticeType(), referenceCell.getCell());

   // Calculate primitive-to-reference centering matrix (inverse of refToPrimitive)
   Matrix_3x3 primitiveToRef = refToPrimitive.Inver();

   // Compose the transformations:
   // primitiveToRef * primitiveToPrivitiveMatrix * mobileToPrimitive
   Matrix_3x3 completeTransformation = primitiveToRef * primitiveToPrivitiveMatrix * mobileToPrimitive;

   return completeTransformation;
}

// Function to verify the transformation produces expected results
inline void verifyTransformation(
   const LatticeCell& mobileCell,
   const LatticeCell& referenceCell,
   const Matrix_3x3& transformationMatrix) {

   // Convert mobile cell to B4
   B4 mobileB4(mobileCell.getCell());

   // Apply transformation
   B4 transformedB4 = transformationMatrix * mobileB4;

   // Convert back to cell
   LRL_Cell transformedCell(transformedB4);

   // Display results
   std::cout << "\n--- TRANSFORMATION VERIFICATION ---" << std::endl;
   std::cout << "Original mobile cell: " << LRL_Cell_Degrees(mobileCell.getCell()) << std::endl;
   std::cout << "Transformed cell:     " << LRL_Cell_Degrees(transformedCell) << std::endl;
   std::cout << "Reference cell:       " << LRL_Cell_Degrees(referenceCell.getCell()) << std::endl;

   // Calculate and display differences
   std::cout << "\nDifferences between transformed and reference cell:" << std::endl;
   for (int i = 0; i < 6; ++i) {
      double diff = transformedCell[i] - referenceCell.getCell()[i];
      std::cout << "Param " << i << ": " << diff;
      if (i >= 3) std::cout << " degrees";
      std::cout << std::endl;
   }

   // Calculate S6 distance
   S6 s1 = S6(transformedCell);
   S6 s2 = S6(referenceCell.getCell());
   S6 reduced1, reduced2;
   Selling::Reduce(s1, reduced1);
   Selling::Reduce(s2, reduced2);
   const double cs6Dist = CS6Dist(reduced1.data(), reduced2.data());

   // Calculate S6 angle
   const double s6Angle = angleS6(s1, s2);

   std::cout << "CS6 Distance: " << cs6Dist << std::endl;
   std::cout << "S6 Angle: " << s6Angle << " degrees" << std::endl;
   std::cout << "--- END VERIFICATION ---\n" << std::endl;
}


#endif // TRANSFORMER_UTILITIES_H

