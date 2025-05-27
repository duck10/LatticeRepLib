
#pragma warning(disable: 4566) // Visual Studio 

#ifndef TRANSFORMATIONMATRICES_H
#define TRANSFORMATIONMATRICES_H

#include "LRL_Vector3.h"
#include "Matrix_3x3.h"
#include <vector>
#include <string>
#include <set>
#include <sstream>

namespace MatrixSets {
   // Identity matrix
   static const Matrix_3x3 IDENTITY(1, 0, 0, 0, 1, 0, 0, 0, 1);

   // Individual named matrices (keep for backward compatibility)
   static const Matrix_3x3 PERM_CYCLIC_1(0, 0, 1, 1, 0, 0, 0, 1, 0);  // xyz->zxy
   static const Matrix_3x3 PERM_CYCLIC_2(0, 1, 0, 0, 0, 1, 1, 0, 0);  // xyz->yzx

   // CORRECTED: These were previously wrong (det = -1), now fixed (det = +1)
   static const Matrix_3x3 PERM_SWAP_XY(0, 1, 0, 1, 0, 0, 0, 0, -1);  // xyz->yx(-z), det = +1
   static const Matrix_3x3 PERM_SWAP_YZ(1, 0, 0, 0, 0, 1, 0, -1, 0);  // xyz->x(-z)y, det = +1  
   static const Matrix_3x3 PERM_SWAP_XZ(0, 0, 1, 0, -1, 0, 1, 0, 0);  // xyz->zx(-y), det = +1

   // Sign flips (determinant = +1)
   static const Matrix_3x3 FLIP_XY(-1, 0, 0, 0, -1, 0, 0, 0, 1);      // Invert x,y
   static const Matrix_3x3 FLIP_XZ(-1, 0, 0, 0, 1, 0, 0, 0, -1);      // Invert x,z
   static const Matrix_3x3 FLIP_YZ(1, 0, 0, 0, -1, 0, 0, 0, -1);      // Invert y,z

   // Shear matrices (all have determinant = +1)
   static const Matrix_3x3 SHEAR_XY(1, 1, 0, 0, 1, 0, 0, 0, 1);       // Shear in xy plane
   static const Matrix_3x3 SHEAR_XZ(1, 0, 1, 0, 1, 0, 0, 0, 1);       // Shear in xz plane
   static const Matrix_3x3 SHEAR_YX(1, 0, 0, 1, 1, 0, 0, 0, 1);       // Shear in yx plane
   static const Matrix_3x3 SHEAR_YZ(1, 0, 0, 0, 1, 1, 0, 0, 1);       // Shear in yz plane
   static const Matrix_3x3 SHEAR_ZX(1, 0, 0, 0, 1, 0, 1, 0, 1);       // Shear in zx plane
   static const Matrix_3x3 SHEAR_ZY(1, 0, 0, 0, 1, 0, 0, 1, 1);       // Shear in zy plane

   // Negative shears
   static const Matrix_3x3 SHEAR_XY_NEG(1, -1, 0, 0, 1, 0, 0, 0, 1);  // Negative shear in xy
   static const Matrix_3x3 SHEAR_XZ_NEG(1, 0, -1, 0, 1, 0, 0, 0, 1);  // Negative shear in xz
   static const Matrix_3x3 SHEAR_YX_NEG(1, 0, 0, -1, 1, 0, 0, 0, 1);  // Negative shear in yx
   static const Matrix_3x3 SHEAR_YZ_NEG(1, 0, 0, 0, 1, -1, 0, 0, 1);  // Negative shear in yz
   static const Matrix_3x3 SHEAR_ZX_NEG(1, 0, 0, 0, 1, 0, -1, 0, 1);  // Negative shear in zx
   static const Matrix_3x3 SHEAR_ZY_NEG(1, 0, 0, 0, 1, 0, 0, -1, 1);  // Negative shear in zy

   // Complex matrices known to be useful for challenging cases
   static const Matrix_3x3 COMPLEX_1(-1, 1, 0, -1, 0, 0, 0, 0, 1);    // Complex case 1
   static const Matrix_3x3 COMPLEX_2(0, 1, 0, 0, 0, 1, 1, 0, 1);      // Complex case 2

   // CORRECTED: Use the complete 24 crystallographic transformations
   // This function generates the correct 24 matrices (all det = +1)
   static std::vector<Matrix_3x3> generateCorrect24Transformations() {
      std::vector<Matrix_3x3> transformations;

      // The complete set of 24 octahedral rotation matrices (det = +1)
      std::vector<std::vector<int>> matrixData = {
         // Identity
         { 1,  0,  0,   0,  1,  0,   0,  0,  1},

         // 8 rotations by ±120° around the 4 body diagonals [±1,±1,±1]
         { 0,  0,  1,   1,  0,  0,   0,  1,  0},  // 120° around [1,1,1]
         { 0,  1,  0,   0,  0,  1,   1,  0,  0},  // 240° around [1,1,1]
         { 0,  0, -1,  -1,  0,  0,   0,  1,  0},  // 120° around [-1,1,1]
         { 0, -1,  0,   0,  0,  1,  -1,  0,  0},  // 240° around [-1,1,1]
         { 0,  0,  1,  -1,  0,  0,   0, -1,  0},  // 120° around [1,-1,1]
         { 0,  1,  0,   0,  0, -1,  -1,  0,  0},  // 240° around [1,-1,1]
         { 0,  0, -1,   1,  0,  0,   0, -1,  0},  // 120° around [1,1,-1]
         { 0, -1,  0,   0,  0, -1,   1,  0,  0},  // 240° around [1,1,-1]

         // 6 rotations by 180° around face centers
         {-1,  0,  0,   0, -1,  0,   0,  0,  1},  // 180° around [0,0,1]
         {-1,  0,  0,   0,  1,  0,   0,  0, -1},  // 180° around [0,1,0]
         { 1,  0,  0,   0, -1,  0,   0,  0, -1},  // 180° around [1,0,0]
         { 0,  0,  1,   0, -1,  0,   1,  0,  0},  // 180° around [1,0,1]/√2
         { 0,  0, -1,   0, -1,  0,  -1,  0,  0},  // 180° around [-1,0,1]/√2
         {-1,  0,  0,   0,  0,  1,   0,  1,  0},  // 180° around [0,1,1]/√2
         {-1,  0,  0,   0,  0, -1,   0, -1,  0},  // 180° around [0,-1,1]/√2
         { 0, -1,  0,  -1,  0,  0,   0,  0, -1},  // 180° around [1,1,0]/√2
         { 0,  1,  0,   1,  0,  0,   0,  0, -1},  // 180° around [-1,1,0]/√2

         // 9 rotations by ±90° around coordinate axes
         { 0, -1,  0,   1,  0,  0,   0,  0,  1},  // 90° around [0,0,1]
         { 0,  1,  0,  -1,  0,  0,   0,  0,  1},  // -90° around [0,0,1]
         { 1,  0,  0,   0,  0, -1,   0,  1,  0},  // 90° around [1,0,0]
         { 1,  0,  0,   0,  0,  1,   0, -1,  0},  // -90° around [1,0,0]
         { 0,  0,  1,   0,  1,  0,  -1,  0,  0},  // 90° around [0,1,0]
         { 0,  0, -1,   0,  1,  0,   1,  0,  0},  // -90° around [0,1,0]
      };

      // Convert to Matrix_3x3 objects
      for (const auto& data : matrixData) {
         Matrix_3x3 mat;
         for (int i = 0; i < 9; ++i) {
            mat[i] = static_cast<double>(data[i]);
         }

         // Verify determinant = +1
         double det = mat.Det();
         if (std::abs(det - 1.0) < 1e-10) {
            transformations.push_back(mat);
         }
      }

      return transformations;
   }

   // CORRECTED VECTORS: Now using the proper 24 matrices
   static inline const std::vector<Matrix_3x3> PERMUTATIONS = generateCorrect24Transformations();

   static inline const std::vector<Matrix_3x3> SHEARS{
       SHEAR_XY, SHEAR_XZ, SHEAR_YX, SHEAR_YZ, SHEAR_ZX, SHEAR_ZY,
       SHEAR_XY_NEG, SHEAR_XZ_NEG, SHEAR_YX_NEG, SHEAR_YZ_NEG, SHEAR_ZX_NEG, SHEAR_ZY_NEG
   };

   static inline const std::vector<Matrix_3x3> COMPLEX{
       COMPLEX_1, COMPLEX_2
   };

   // Utility functions (updated to work with corrected matrices)

   // Get all 24 crystallographic transformation matrices
   static std::vector<Matrix_3x3> getAllPermutationMatrices() {
      return PERMUTATIONS;  // Now returns the correct 24 matrices
   }

   // Get the name of a transformation matrix for debugging
   static std::string getPermutationName(const Matrix_3x3& matrix) {
      // Check against known individual matrices
      if (matrix == IDENTITY) return "[a,b,c] (Identity)";
      if (matrix == PERM_CYCLIC_1) return "[c,a,b] (Cyclic 1)";
      if (matrix == PERM_CYCLIC_2) return "[b,c,a] (Cyclic 2)";
      if (matrix == PERM_SWAP_XY) return "[b,a,c] (Swap XY)";
      if (matrix == PERM_SWAP_YZ) return "[a,c,b] (Swap YZ)";
      if (matrix == PERM_SWAP_XZ) return "[c,b,a] (Swap XZ)";
      if (matrix == FLIP_XY) return "[a,b,c] (Flip XY)";
      if (matrix == FLIP_XZ) return "[a,b,c] (Flip XZ)";
      if (matrix == FLIP_YZ) return "[a,b,c] (Flip YZ)";

      // For others, find index in PERMUTATIONS
      for (size_t i = 0; i < PERMUTATIONS.size(); ++i) {
         if (matrix == PERMUTATIONS[i]) {
            return "Crystallographic #" + std::to_string(i + 1);
         }
      }

      return "Unknown transformation";
   }

   // Check if a matrix is a valid transformation matrix (det = +1)
   static bool isValidTransformationMatrix(const Matrix_3x3& matrix) {
      const double det = matrix.Det();
      const double epsilon = 1e-10;
      return std::abs(det - 1.0) < epsilon;
   }
}

inline void testPermutationGroupClosure() {
   std::cout << "\n===== TESTING CLOSURE OF PERMUTATION GROUP =====\n" << std::endl;

   // Get all permutation matrices - NOW USES VECTOR VERSION!
   const std::vector<Matrix_3x3> allPermutations = MatrixSets::getAllPermutationMatrices();
   const size_t permCount = allPermutations.size();

   std::cout << "Testing " << permCount << " permutation matrices for group closure" << std::endl;

   // ... rest of function remains exactly the same ...
   // Track any failures
   bool allProductsValid = true;
   int totalProducts = 0;
   int productsNotFound = 0;

   // Create a vector to store any products that aren't in the permutation set
   std::vector<std::pair<Matrix_3x3, std::pair<int, int>>> invalidProducts;

   // Test all pairs of matrices
   for (size_t i = 0; i < permCount; ++i) {
      for (size_t j = 0; j < permCount; ++j) {
         totalProducts++;

         // Compute the product matrix
         Matrix_3x3 product = allPermutations[i] * allPermutations[j];

         // Check if the product is in the permutation set
         bool found = false;
         for (size_t k = 0; k < permCount; ++k) {
            if (product == allPermutations[k]) {
               found = true;
               break;
            }
         }

         if (!found) {
            allProductsValid = false;
            productsNotFound++;
            invalidProducts.push_back(std::make_pair(product, std::make_pair(i, j)));
         }
      }
   }

   // Report results
   std::cout << "Tested " << totalProducts << " matrix products in total" << std::endl;

   if (allProductsValid) {
      std::cout << "SUCCESS: The permutation matrices form a closed group ✓" << std::endl;
      std::cout << "All products of permutation matrices are also permutation matrices." << std::endl;
   }
   else {
      std::cout << "FAILURE: The permutation matrices do NOT form a closed group (A) ✗" << std::endl;
      std::cout << "Found " << productsNotFound << " products that are not in the permutation set." << std::endl;

      // Print the first few invalid products for diagnosis
      const size_t maxToShow = std::min(size_t(5), invalidProducts.size());
      std::cout << "\nShowing first " << maxToShow << " invalid products:" << std::endl;

      for (size_t i = 0; i < maxToShow; ++i) {
         const Matrix_3x3& product = invalidProducts[i].first;
         const int matrix1Index = invalidProducts[i].second.first;
         const int matrix2Index = invalidProducts[i].second.second;

         std::cout << "\nProduct of matrix #" << matrix1Index << " and matrix #" << matrix2Index << ":" << std::endl;
         std::cout << "Matrix #" << matrix1Index << ":" << std::endl << allPermutations[matrix1Index] << std::endl;
         std::cout << "Matrix #" << matrix2Index << ":" << std::endl << allPermutations[matrix2Index] << std::endl;
         std::cout << "Product:" << std::endl << product << std::endl;
         std::cout << "Det: " << product.Det() << std::endl;
      }
   }

   // Additional test for the identity matrix
   std::cout << "\nChecking if identity matrix is in the permutation set:" << std::endl;
   Matrix_3x3 identity;
   identity.UnitMatrix();  // Using UnitMatrix() instead of SetAsIdentity()

   bool identityFound = false;
   for (size_t i = 0; i < permCount; ++i) {
      if (identity == allPermutations[i]) {
         identityFound = true;
         std::cout << "Identity matrix is found in the permutation set at index " << i << " ✓" << std::endl;
         break;
      }
   }

   if (!identityFound) {
      std::cout << "FAILURE: Identity matrix is NOT in the permutation set ✗" << std::endl;
      allProductsValid = false;
   }

   // Additional test for inverses
   std::cout << "\nChecking if each permutation has its inverse in the set:" << std::endl;

   bool allInversesFound = true;
   int inversesNotFound = 0;

   for (size_t i = 0; i < permCount; ++i) {
      // Compute the inverse
      Matrix_3x3 inverse = allPermutations[i].Inverse();

      // Check if the inverse is in the permutation set
      bool found = false;
      for (size_t j = 0; j < permCount; ++j) {
         if (inverse == allPermutations[j]) {
            found = true;
            break;
         }
      }

      if (!found) {
         allInversesFound = false;
         inversesNotFound++;

         // Print info about matrices without inverses
         if (inversesNotFound <= 5) { // Limit output to first 5
            std::cout << "Matrix #" << i << " does not have its inverse in the set:" << std::endl;
            std::cout << "Matrix:" << std::endl << allPermutations[i] << std::endl;
            std::cout << "Inverse:" << std::endl << inverse << std::endl;
            std::cout << "Det of original: " << allPermutations[i].Det() << std::endl;
            std::cout << "Det of inverse: " << inverse.Det() << std::endl;
         }
      }
   }

   if (allInversesFound) {
      std::cout << "All permutation matrices have their inverses in the set ✓" << std::endl;
   }
   else {
      std::cout << "FAILURE: " << inversesNotFound << " permutation matrices don't have their inverses in the set ✗" << std::endl;
      allProductsValid = false;
   }

   // Final summary
   std::cout << "\n===== GROUP CLOSURE TEST SUMMARY =====\n" << std::endl;
   if (allProductsValid && identityFound && allInversesFound) {
      std::cout << "The permutation matrices form a valid mathematical group ✓" << std::endl;
   }
   else {
      std::cout << "The permutation matrices do NOT form a valid mathematical group (B) ✗" << std::endl;
      if (!allProductsValid) {
         std::cout << "- Group is not closed under multiplication" << std::endl;
      }
      if (!identityFound) {
         std::cout << "- Identity matrix is missing from the set" << std::endl;
      }
      if (!allInversesFound) {
         std::cout << "- Some matrices don't have their inverses in the set" << std::endl;
      }
   }
}


#endif // TRANSFORMATIONMATRICES_H
