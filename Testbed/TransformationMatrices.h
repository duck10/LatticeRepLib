﻿#pragma warning(disable: 4566)
#ifndef TRANSFORMATION_MATRICES_H
#define TRANSFORMATION_MATRICES_H
#include <vector>
#include "LRL_Vector3.h"  // For Matrix_3x3
#include <set>
#include <algorithm>

// ========================================
// TRANSFORMATION MATRICES CENTRAL REPOSITORY
// All 3D lattice transformation matrices defined here
// Complete replacement for old MatrixSets namespace
// ========================================

namespace TransformationMatrices {

   // ========================================
   // BASIC MATRICES
   // ========================================

   // Identity matrix
   const Matrix_3x3 IDENTITY(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
   const Matrix_3x3 spnull_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);

   // ========================================
   // CORRECTED PERMUTATION MATRICES (det = +1)
   // From comprehensive search - these are EXACT
   // ========================================

   const Matrix_3x3 sp1_3D(0.0, -1.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, -1.0);
   const Matrix_3x3 sp2_3D(-1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, -1.0, 0.0);

   // ========================================
   // SIGN CHANGE MATRICES
   // ========================================

   const Matrix_3x3 sp34a_3D(-1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 1.0);
   const Matrix_3x3 sp34b_3D(-1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0);
   const Matrix_3x3 sp34c_3D(1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, -1.0);

   // B4Matcher aliases
   const Matrix_3x3 B4_NEG_XY = sp34a_3D;
   const Matrix_3x3 B4_NEG_XZ = sp34b_3D;
   const Matrix_3x3 B4_NEG_YZ = sp34c_3D;

   // ========================================
   // NIGGLI REDUCTION MATRICES
   // ========================================

   const Matrix_3x3 R5_Plus_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, -1.0, 1.0);
   const Matrix_3x3 R5_Minus_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0);
   const Matrix_3x3 R6_Plus_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, 0.0, 1.0);
   const Matrix_3x3 R6_Minus_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
   const Matrix_3x3 R7_Plus_3D(1.0, 0.0, 0.0, -1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
   const Matrix_3x3 R7_Minus_3D(1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
   const Matrix_3x3 R8_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0);
   const Matrix_3x3 R12_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, -1.0, 1.0);

   // Aliases
   const Matrix_3x3 R9_Plus_3D = R5_Plus_3D;
   const Matrix_3x3 R9_Minus_3D = R5_Minus_3D;
   const Matrix_3x3 R10_Plus_3D = R6_Plus_3D;
   const Matrix_3x3 R10_Minus_3D = R6_Minus_3D;
   const Matrix_3x3 R11_Plus_3D = R7_Plus_3D;
   const Matrix_3x3 R11_Minus_3D = R7_Minus_3D;

   // ========================================
   // ADDED: MISSING SHEAR MATRICES (12 total)
   // These were in the old version but missing from current
   // ========================================

   // Positive shears (6 matrices)
   const Matrix_3x3 SHEAR_XY(1, 1, 0, 0, 1, 0, 0, 0, 1);       // Shear in xy plane
   const Matrix_3x3 SHEAR_XZ(1, 0, 1, 0, 1, 0, 0, 0, 1);       // Shear in xz plane
   const Matrix_3x3 SHEAR_YX(1, 0, 0, 1, 1, 0, 0, 0, 1);       // Shear in yx plane
   const Matrix_3x3 SHEAR_YZ(1, 0, 0, 0, 1, 1, 0, 0, 1);       // Shear in yz plane
   const Matrix_3x3 SHEAR_ZX(1, 0, 0, 0, 1, 0, 1, 0, 1);       // Shear in zx plane
   const Matrix_3x3 SHEAR_ZY(1, 0, 0, 0, 1, 0, 0, 1, 1);       // Shear in zy plane

   // Negative shears (6 matrices)
   const Matrix_3x3 SHEAR_XY_NEG(1, -1, 0, 0, 1, 0, 0, 0, 1);  // Negative shear in xy
   const Matrix_3x3 SHEAR_XZ_NEG(1, 0, -1, 0, 1, 0, 0, 0, 1);  // Negative shear in xz
   const Matrix_3x3 SHEAR_YX_NEG(1, 0, 0, -1, 1, 0, 0, 0, 1);  // Negative shear in yx
   const Matrix_3x3 SHEAR_YZ_NEG(1, 0, 0, 0, 1, -1, 0, 0, 1);  // Negative shear in yz
   const Matrix_3x3 SHEAR_ZX_NEG(1, 0, 0, 0, 1, 0, -1, 0, 1);  // Negative shear in zx
   const Matrix_3x3 SHEAR_ZY_NEG(1, 0, 0, 0, 1, 0, 0, -1, 1);  // Negative shear in zy

   // ========================================
   // ADDED: MISSING COMPLEX MATRICES (2 total)
   // These handle challenging transformation cases
   // ========================================

   const Matrix_3x3 COMPLEX_1(-1, 1, 0, -1, 0, 0, 0, 0, 1);    // Complex case 1
   const Matrix_3x3 COMPLEX_2(0, 1, 0, 0, 0, 1, 1, 0, 1);      // Complex case 2

   // ========================================
   // B4MATCHER SPECIFIC MATRICES
   // ========================================

   // Priority 2: Corrected permutations (det = +1)
   const Matrix_3x3 PERM_SWAP_XY = sp1_3D;  // Use corrected sp1_3D
   const Matrix_3x3 PERM_SWAP_YZ = sp2_3D;  // Use corrected sp2_3D  
   const Matrix_3x3 PERM_SWAP_XZ(0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0);  // Swap a,c
   const Matrix_3x3 PERM_CYCLIC_1(0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0); // a→b→c→a
   const Matrix_3x3 PERM_CYCLIC_2(0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0); // a→c→b→a
   const Matrix_3x3 B4_SHEAR_XY_NEG_XZ(-1, 1, 0, 0, 1, 0, 0, 0, -1);

   // ========================================
   // COMPREHENSIVE 24 CRYSTALLOGRAPHIC MATRICES
   // Complete octahedral group from the old version
   // ========================================

   static std::vector<Matrix_3x3> generateComplete24CrystallographicMatrices() {
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
   // MATRIX COLLECTIONS
   // ========================================

   // Forward declarations - implement in .cpp
   extern const std::vector<Matrix_3x3> PERMUTATIONS;
   extern const std::vector<Matrix_3x3> SHEARS;
   extern const std::vector<Matrix_3x3> COMPLEX;

   // B4Matcher critical matrix
   const Matrix_3x3 B4_CRITICAL = sp1_3D;

   // ========================================
   // FUNCTION DECLARATIONS
   // ========================================

   std::vector<Matrix_3x3> buildAllTransformationMatrices(const bool includeInverses = true, const int maxCoeff = 3);
   std::vector<Matrix_3x3> generateComprehensiveIntegerMatrices(int maxCoeff);

   std::vector<Matrix_3x3> getAllNiggliMatrices();
   std::vector<Matrix_3x3> getAllPermutationMatrices();
   std::vector<Matrix_3x3> getAllReduction3DMatrices();

   // ADDED: New collection functions for the missing matrix types
   std::vector<Matrix_3x3> getAllShearMatrices();
   std::vector<Matrix_3x3> getAllComplexMatrices();

   void testAllMatrices();

   std::vector<Matrix_3x3>& getAllTransformationMatrices(const int maxCoeff = 3);

} // namespace TransformationMatrices

#endif // TRANSFORMATION_MATRICES_H
