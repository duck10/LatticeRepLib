// EXACT MODIFICATIONS for your current TransformationMatrices.cpp
// Replace your current .cpp file with this complete version:

#include "TransformationMatrices.h"
#include <map>
#include <set>
#include <iostream>
#include <iomanip>

std::vector<Matrix_3x3>& TransformationMatrices::getAllTransformationMatrices(const int maxCoeff) {
   static std::map<int, std::vector<Matrix_3x3>> cache;

   // Check if we already have this maxCoeff cached
   if (cache.find(maxCoeff) == cache.end()) {
      cache[maxCoeff] = buildAllTransformationMatrices(true, maxCoeff);
      std::cout << "Built " << cache[maxCoeff].size() << " matrices for maxCoeff=" << maxCoeff << std::endl;
   }

   return cache[maxCoeff];
}

std::vector<Matrix_3x3> TransformationMatrices::getAllNiggliMatrices() {
   return {
       spnull_3D,
       sp1_3D, sp2_3D,
       sp34a_3D, sp34b_3D, sp34c_3D,
       R5_Plus_3D, R5_Minus_3D,
       R6_Plus_3D, R6_Minus_3D,
       R7_Plus_3D, R7_Minus_3D,
       R8_3D, R12_3D
       // Note: R9-R11 are aliases, so not duplicated
   };
}

// ========================================
// ADD: NEW FUNCTIONS FOR MISSING MATRIX TYPES
// ========================================

std::vector<Matrix_3x3> TransformationMatrices::getAllShearMatrices() {
   return {
       SHEAR_XY, SHEAR_XZ, SHEAR_YX, SHEAR_YZ, SHEAR_ZX, SHEAR_ZY,
       SHEAR_XY_NEG, SHEAR_XZ_NEG, SHEAR_YX_NEG, SHEAR_YZ_NEG, SHEAR_ZX_NEG, SHEAR_ZY_NEG
   };
}

std::vector<Matrix_3x3> TransformationMatrices::getAllComplexMatrices() {
   return { COMPLEX_1, COMPLEX_2 };
}

std::vector<Matrix_3x3> TransformationMatrices::getAllReduction3DMatrices() {
   // Return additional reduction matrices (if you have them)
   // For now, return empty - you can add more later
   return {};
}

// ========================================
// MODIFIED: Updated vector definitions to use proper matrices
// ========================================

const std::vector<Matrix_3x3> TransformationMatrices::PERMUTATIONS =
TransformationMatrices::generateComplete24CrystallographicMatrices();

const std::vector<Matrix_3x3> TransformationMatrices::SHEARS = {
    SHEAR_XY, SHEAR_XZ, SHEAR_YX, SHEAR_YZ, SHEAR_ZX, SHEAR_ZY,
    SHEAR_XY_NEG, SHEAR_XZ_NEG, SHEAR_YX_NEG, SHEAR_YZ_NEG, SHEAR_ZX_NEG, SHEAR_ZY_NEG
};

const std::vector<Matrix_3x3> TransformationMatrices::COMPLEX = {
    COMPLEX_1, COMPLEX_2
};

// ========================================
// KEEP: Your existing getAllPermutationMatrices (it's already correct!)
// ========================================

std::vector<Matrix_3x3> TransformationMatrices::getAllPermutationMatrices() {
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
      Matrix_3x3 matrix(
         data[0], data[1], data[2],
         data[3], data[4], data[5],
         data[6], data[7], data[8]
      );

      // Verify det = +1 (should be true for all these matrices)
      double det = matrix.Det();
      if (std::abs(det - 1.0) < 1e-10) {
         transformations.push_back(matrix);
      }
      else {
         std::cerr << "ERROR: Matrix with det=" << det << " found in crystallographic set!" << std::endl;
      }
   }

   return transformations;
}

// ========================================
// KEEP: Your helper function
// ========================================

double maxCoef(const Matrix_3x3& m) {
   double maxvalue = -DBL_MAX;
   for (int i = 0; i < 9; ++i) {
      maxvalue = std::max(maxvalue, std::abs(m[i]));
   }
   return maxvalue;
}

// ========================================
// MODIFIED: Enhanced buildAllTransformationMatrices with missing matrices
// ========================================

// DEBUG: Add this to buildAllTransformationMatrices to see what's happening

std::vector<Matrix_3x3> TransformationMatrices::buildAllTransformationMatrices(bool includeInverses, int maxCoeff) {
   std::cout << "=== DEBUG: buildAllTransformationMatrices called ===" << std::endl;
   std::cout << "  includeInverses: " << (includeInverses ? "true" : "false") << std::endl;
   std::cout << "  maxCoeff: " << maxCoeff << std::endl;

   std::cout << "Building comprehensive transformation matrix set..." << std::endl;

   std::set<Matrix_3x3> uniqueMatrices;

   // Add base specialized matrices
   auto niggliMatrices = getAllNiggliMatrices();
   std::cout << "  Niggli: " << niggliMatrices.size() << std::endl;
   uniqueMatrices.insert(niggliMatrices.begin(), niggliMatrices.end());

   auto permMatrices = getAllPermutationMatrices();
   std::cout << "  Crystallographic: " << permMatrices.size() << " (should be 24)" << std::endl;
   uniqueMatrices.insert(permMatrices.begin(), permMatrices.end());

   auto shearMatrices = getAllShearMatrices();
   std::cout << "  Shear: " << shearMatrices.size() << " (should be 12) - RESTORED" << std::endl;
   uniqueMatrices.insert(shearMatrices.begin(), shearMatrices.end());

   auto complexMatrices = getAllComplexMatrices();
   std::cout << "  Complex: " << complexMatrices.size() << " (should be 2) - RESTORED" << std::endl;
   uniqueMatrices.insert(complexMatrices.begin(), complexMatrices.end());

   // FIX: Generate systematic matrices for maxCoeff >= 1
   if (maxCoeff >= 1) {
      std::cout << "  Generating SYSTEMATIC matrices for maxCoeff=" << maxCoeff << "..." << std::endl;

      auto systematicMatrices = generateComprehensiveIntegerMatrices(maxCoeff);
      std::cout << "  Generated " << systematicMatrices.size() << " systematic matrices" << std::endl;
      uniqueMatrices.insert(systematicMatrices.begin(), systematicMatrices.end());

      // Verification of expected counts
      if (maxCoeff == 1 && systematicMatrices.size() < 30) {
         std::cout << "  ❌ WARNING: Expected 40+ matrices for maxCoeff=1, got " << systematicMatrices.size() << std::endl;
      }
      else if (maxCoeff == 2 && systematicMatrices.size() < 200) {
         std::cout << "  ❌ WARNING: Expected 300+ matrices for maxCoeff=2, got " << systematicMatrices.size() << std::endl;
      }
      else {
         std::cout << "  ✅ Good systematic generation: " << systematicMatrices.size() << " matrices" << std::endl;
      }
   }
   else {
      std::cout << "  Skipping systematic generation (maxCoeff=" << maxCoeff << " < 1)" << std::endl;
   }

   std::cout << "  Total before inverses: " << uniqueMatrices.size() << std::endl;

   // Add inverses
   if (includeInverses) {
      std::vector<Matrix_3x3> inverses;
      for (const auto& matrix : uniqueMatrices) {
         try {
            Matrix_3x3 inverse = matrix.Inverse();
            inverses.push_back(inverse);
         }
         catch (...) {
            // Skip non-invertible matrices
         }
      }
      uniqueMatrices.insert(inverses.begin(), inverses.end());
      std::cout << "  Added " << inverses.size() << " inverses" << std::endl;
   }

   std::cout << "  Final total: " << uniqueMatrices.size() << std::endl;

   // Final verification
   if (maxCoeff == 1 && uniqueMatrices.size() < 80) {
      std::cout << "❌ CRITICAL: Only " << uniqueMatrices.size() << " matrices for maxCoeff=1" << std::endl;
      std::cout << "   Expected 80-120 for complete ±1 coverage" << std::endl;
   }
   else if (maxCoeff == 2 && uniqueMatrices.size() < 300) {
      std::cout << "❌ CRITICAL: Only " << uniqueMatrices.size() << " matrices for maxCoeff=2" << std::endl;
      std::cout << "   Expected 400-800 for complete ±2 coverage" << std::endl;
   }
   else {
      std::cout << "✅ Good matrix coverage: " << uniqueMatrices.size() << " matrices" << std::endl;
   }

   return { uniqueMatrices.begin(), uniqueMatrices.end() };
}

// CRITICAL: Make sure generateComprehensiveIntegerMatrices exists and works
std::vector<Matrix_3x3> TransformationMatrices::generateComprehensiveIntegerMatrices(int maxCoeff) {
   std::vector<Matrix_3x3> matrices;
   std::set<std::vector<int>> uniqueSignatures;

   std::cout << "    SYSTEMATIC enumeration for maxCoeff=" << maxCoeff << std::endl;

   int totalCombinations;
   if (maxCoeff == 1) {
      totalCombinations = 3 * 3 * 3 * 3 * 3 * 3 * 3 * 3 * 3; // 3^9 = 19,683
      std::cout << "    Complete enumeration: " << totalCombinations << " combinations" << std::endl;
   }
   else if (maxCoeff == 2) {
      totalCombinations = 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5; // 5^9 = 1,953,125
      std::cout << "    Complete enumeration: " << totalCombinations << " combinations (may take 30+ seconds)" << std::endl;
   }
   else {
      std::cout << "    WARNING: maxCoeff=" << maxCoeff << " is computationally expensive!" << std::endl;
      totalCombinations = std::pow(2 * maxCoeff + 1, 9);
   }

   int validFound = 0;
   int checked = 0;

   // Complete enumeration
   for (int a00 = -maxCoeff; a00 <= maxCoeff; ++a00) {
      for (int a01 = -maxCoeff; a01 <= maxCoeff; ++a01) {
         for (int a02 = -maxCoeff; a02 <= maxCoeff; ++a02) {
            for (int a10 = -maxCoeff; a10 <= maxCoeff; ++a10) {
               for (int a11 = -maxCoeff; a11 <= maxCoeff; ++a11) {
                  for (int a12 = -maxCoeff; a12 <= maxCoeff; ++a12) {
                     for (int a20 = -maxCoeff; a20 <= maxCoeff; ++a20) {
                        for (int a21 = -maxCoeff; a21 <= maxCoeff; ++a21) {
                           for (int a22 = -maxCoeff; a22 <= maxCoeff; ++a22) {

                              checked++;

                              // Progress for large enumerations
                              if (maxCoeff >= 2 && checked % 200000 == 0) {
                                 double progress = 100.0 * checked / totalCombinations;
                                 std::cout << "      " << progress << "% complete (" << validFound << " found)" << std::endl;
                              }

                              Matrix_3x3 matrix(a00, a01, a02, a10, a11, a12, a20, a21, a22);

                              // Check determinant = ±1
                              double det = matrix.Det();
                              if (std::abs(std::abs(det) - 1.0) < 1e-10) {

                                 std::vector<int> signature = { a00, a01, a02, a10, a11, a12, a20, a21, a22 };

                                 if (uniqueSignatures.find(signature) == uniqueSignatures.end()) {
                                    uniqueSignatures.insert(signature);
                                    matrices.push_back(matrix);
                                    validFound++;
                                 }
                              }
                           }
                        }
                     }
                  }
               }
            }
         }
      }
   }

   std::cout << "    Systematic enumeration complete: " << validFound << " valid matrices found" << std::endl;

   return matrices;
}

// ========================================
// ADD: Diagnostic function
// ========================================

void TransformationMatrices::testAllMatrices() {
   std::cout << "\n=== TRANSFORMATION MATRIX TEST ===" << std::endl;

   auto permMatrices = getAllPermutationMatrices();
   auto niggliMatrices = getAllNiggliMatrices();
   auto shearMatrices = getAllShearMatrices();
   auto complexMatrices = getAllComplexMatrices();
   auto comprehensive = buildAllTransformationMatrices(true, 3);

   std::cout << "Matrix breakdown:" << std::endl;
   std::cout << "  Crystallographic: " << permMatrices.size() << " (expected: 24)" << std::endl;
   std::cout << "  Niggli: " << niggliMatrices.size() << std::endl;
   std::cout << "  Shear: " << shearMatrices.size() << " (expected: 12)" << std::endl;
   std::cout << "  Complex: " << complexMatrices.size() << " (expected: 2)" << std::endl;
   std::cout << "  TOTAL: " << comprehensive.size() << " (expected: 40+)" << std::endl;

   if (permMatrices.size() != 24) {
      std::cout << "❌ ERROR: Expected 24 crystallographic matrices!" << std::endl;
   }
   if (shearMatrices.size() != 12) {
      std::cout << "❌ ERROR: Expected 12 shear matrices!" << std::endl;
   }
   if (complexMatrices.size() != 2) {
      std::cout << "❌ ERROR: Expected 2 complex matrices!" << std::endl;
   }
}
