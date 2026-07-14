#pragma warning(disable: 4566)
#ifndef TRANSFORMATION_MATRICES_H
#define TRANSFORMATION_MATRICES_H

#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>
#include <vector>

#include "LRL_Vector3.h"  // For Matrix_3x3

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
         } else {
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
   std::vector<Matrix_3x3> generateComprehensiveIntegerMatrices(const int maxCoeff);

   std::vector<Matrix_3x3> getAllNiggliMatrices();
   std::vector<Matrix_3x3> getAllPermutationMatrices();
   std::vector<Matrix_3x3> getAllReduction3DMatrices();

   // ADDED: New collection functions for the missing matrix types
   std::vector<Matrix_3x3> getAllShearMatrices();
   std::vector<Matrix_3x3> getAllComplexMatrices();

   void testAllMatrices();

   std::vector<Matrix_3x3>& getAllTransformationMatrices(const int maxCoeff = 3);

   inline std::vector<Matrix_3x3> generateUnimodularMatrices(const int maxCoeff = 1) {
      /*
         Generate all 3×3 unimodular integer matrices with determinant +1
         and entries in the range [-maxCoeff, +maxCoeff].
      */

      std::vector<Matrix_3x3> matrices;
      matrices.reserve(4000);

      const int lo = -maxCoeff;
      const int hi = maxCoeff;

      for (int a00 = lo; a00 <= hi; ++a00)
         for (int a01 = lo; a01 <= hi; ++a01)
            for (int a02 = lo; a02 <= hi; ++a02)
               for (int a10 = lo; a10 <= hi; ++a10)
                  for (int a11 = lo; a11 <= hi; ++a11)
                     for (int a12 = lo; a12 <= hi; ++a12)
                        for (int a20 = lo; a20 <= hi; ++a20)
                           for (int a21 = lo; a21 <= hi; ++a21)
                              for (int a22 = lo; a22 <= hi; ++a22) {

                                 Matrix_3x3 M(
                                    a00, a01, a02,
                                    a10, a11, a12,
                                    a20, a21, a22
                                 );

                                 const double det = M.Det();

                                 // Accept ONLY determinant +1
                                 if (std::abs(det - 1.0) < 1e-12) {
                                    matrices.emplace_back(M);
                                 }
                              }

      matrices.shrink_to_fit();
      return matrices;
   }

   // ========================================
   // SUPERCELL MATRICES
   // Generate all 3x3 integer matrices with determinant in the given set.
   // Used for supercell matching: det=2, 3, or 4.
   // Entries restricted to [-maxCoeff, +maxCoeff].
   // ========================================

   inline std::vector<Matrix_3x3> generateSupercellMatrices(
      const std::vector<int>& determinants,
      const int maxCoeff = 1) {

      std::vector<Matrix_3x3> matrices;
      matrices.reserve(8000);

      std::set<int> detSet(determinants.begin(), determinants.end());

      const int lo = -maxCoeff;
      const int hi = maxCoeff;

      for (int a00 = lo; a00 <= hi; ++a00)
         for (int a01 = lo; a01 <= hi; ++a01)
            for (int a02 = lo; a02 <= hi; ++a02)
               for (int a10 = lo; a10 <= hi; ++a10)
                  for (int a11 = lo; a11 <= hi; ++a11)
                     for (int a12 = lo; a12 <= hi; ++a12)
                        for (int a20 = lo; a20 <= hi; ++a20)
                           for (int a21 = lo; a21 <= hi; ++a21)
                              for (int a22 = lo; a22 <= hi; ++a22) {
                                 Matrix_3x3 M(
                                    a00, a01, a02,
                                    a10, a11, a12,
                                    a20, a21, a22
                                 );
                                 const int det =
                                    static_cast<int>(std::round(M.Det()));
                                 if (detSet.count(det))
                                    matrices.emplace_back(M);
                              }

      matrices.shrink_to_fit();
      return matrices;
   }

   // ========================================
   // HNF MATRICES
   // Complete set of Hermite Normal Form matrices for orders 2-6.
   // Upper triangular, positive diagonal, off-diagonal 0 <= m[i][j] < m[j][j].
   // Minimal complete set for supercell relationships in primitive space
   // (Santoro & Mighell, Acta Cryst. A28, 284-287, 1972).
   //
   // Counts by order: 2->7, 3->13, 4->35, 5->31, 6->91  (total 177)
   //
   // NOTE: For orders 5 and 6 the brute-force integer-matrix search is
   // computationally infeasible (search space ~10^10 for maxCoeff=6).
   // HNF provides the provably complete minimal set for primitive cells,
   // but may miss supercell relationships in non-standard orientations
   // that require a non-upper-triangular transformation matrix.
   // In practice this is rare; re-index the cell to a standard setting
   // first if a det=5 or det=6 relationship is suspected but not found.
   // ========================================

   inline std::vector<Matrix_3x3> generateHNFMatrices(
      const std::vector<int>& orders = { 2, 3, 4, 5, 6 }) {

      static const std::vector<std::pair<int, Matrix_3x3>> all = {
         // order-2 (7 matrices)
         {2, Matrix_3x3(1,0,0, 0,1,0, 0,0,2)},
         {2, Matrix_3x3(1,0,0, 0,1,1, 0,0,2)},
         {2, Matrix_3x3(1,0,1, 0,1,0, 0,0,2)},
         {2, Matrix_3x3(1,0,1, 0,1,1, 0,0,2)},
         {2, Matrix_3x3(1,0,0, 0,2,0, 0,0,1)},
         {2, Matrix_3x3(1,1,0, 0,2,0, 0,0,1)},
         {2, Matrix_3x3(2,0,0, 0,1,0, 0,0,1)},
         // order-3 (13 matrices)
         {3, Matrix_3x3(1,0,0, 0,1,0, 0,0,3)},
         {3, Matrix_3x3(1,0,0, 0,1,1, 0,0,3)},
         {3, Matrix_3x3(1,0,0, 0,1,2, 0,0,3)},
         {3, Matrix_3x3(1,0,1, 0,1,0, 0,0,3)},
         {3, Matrix_3x3(1,0,1, 0,1,1, 0,0,3)},
         {3, Matrix_3x3(1,0,1, 0,1,2, 0,0,3)},
         {3, Matrix_3x3(1,0,2, 0,1,0, 0,0,3)},
         {3, Matrix_3x3(1,0,2, 0,1,1, 0,0,3)},
         {3, Matrix_3x3(1,0,2, 0,1,2, 0,0,3)},
         {3, Matrix_3x3(1,0,0, 0,3,0, 0,0,1)},
         {3, Matrix_3x3(1,1,0, 0,3,0, 0,0,1)},
         {3, Matrix_3x3(1,2,0, 0,3,0, 0,0,1)},
         {3, Matrix_3x3(3,0,0, 0,1,0, 0,0,1)},
         // order-4 (35 matrices)
         {4, Matrix_3x3(1,0,0, 0,1,0, 0,0,4)},
         {4, Matrix_3x3(1,0,0, 0,1,1, 0,0,4)},
         {4, Matrix_3x3(1,0,0, 0,1,2, 0,0,4)},
         {4, Matrix_3x3(1,0,0, 0,1,3, 0,0,4)},
         {4, Matrix_3x3(1,0,1, 0,1,0, 0,0,4)},
         {4, Matrix_3x3(1,0,1, 0,1,1, 0,0,4)},
         {4, Matrix_3x3(1,0,1, 0,1,2, 0,0,4)},
         {4, Matrix_3x3(1,0,1, 0,1,3, 0,0,4)},
         {4, Matrix_3x3(1,0,2, 0,1,0, 0,0,4)},
         {4, Matrix_3x3(1,0,2, 0,1,1, 0,0,4)},
         {4, Matrix_3x3(1,0,2, 0,1,2, 0,0,4)},
         {4, Matrix_3x3(1,0,2, 0,1,3, 0,0,4)},
         {4, Matrix_3x3(1,0,3, 0,1,0, 0,0,4)},
         {4, Matrix_3x3(1,0,3, 0,1,1, 0,0,4)},
         {4, Matrix_3x3(1,0,3, 0,1,2, 0,0,4)},
         {4, Matrix_3x3(1,0,3, 0,1,3, 0,0,4)},
         {4, Matrix_3x3(1,0,0, 0,2,0, 0,0,2)},
         {4, Matrix_3x3(1,0,0, 0,2,1, 0,0,2)},
         {4, Matrix_3x3(1,0,1, 0,2,0, 0,0,2)},
         {4, Matrix_3x3(1,0,1, 0,2,1, 0,0,2)},
         {4, Matrix_3x3(1,1,0, 0,2,0, 0,0,2)},
         {4, Matrix_3x3(1,1,0, 0,2,1, 0,0,2)},
         {4, Matrix_3x3(1,1,1, 0,2,0, 0,0,2)},
         {4, Matrix_3x3(1,1,1, 0,2,1, 0,0,2)},
         {4, Matrix_3x3(1,0,0, 0,4,0, 0,0,1)},
         {4, Matrix_3x3(1,1,0, 0,4,0, 0,0,1)},
         {4, Matrix_3x3(1,2,0, 0,4,0, 0,0,1)},
         {4, Matrix_3x3(1,3,0, 0,4,0, 0,0,1)},
         {4, Matrix_3x3(2,0,0, 0,1,0, 0,0,2)},
         {4, Matrix_3x3(2,0,0, 0,1,1, 0,0,2)},
         {4, Matrix_3x3(2,0,1, 0,1,0, 0,0,2)},
         {4, Matrix_3x3(2,0,1, 0,1,1, 0,0,2)},
         {4, Matrix_3x3(2,0,0, 0,2,0, 0,0,1)},
         {4, Matrix_3x3(2,1,0, 0,2,0, 0,0,1)},
         {4, Matrix_3x3(4,0,0, 0,1,0, 0,0,1)},
         // order-5 (31 matrices)
         {5, Matrix_3x3(1,0,0, 0,1,0, 0,0,5)},
         {5, Matrix_3x3(1,0,0, 0,1,1, 0,0,5)},
         {5, Matrix_3x3(1,0,0, 0,1,2, 0,0,5)},
         {5, Matrix_3x3(1,0,0, 0,1,3, 0,0,5)},
         {5, Matrix_3x3(1,0,0, 0,1,4, 0,0,5)},
         {5, Matrix_3x3(1,0,1, 0,1,0, 0,0,5)},
         {5, Matrix_3x3(1,0,1, 0,1,1, 0,0,5)},
         {5, Matrix_3x3(1,0,1, 0,1,2, 0,0,5)},
         {5, Matrix_3x3(1,0,1, 0,1,3, 0,0,5)},
         {5, Matrix_3x3(1,0,1, 0,1,4, 0,0,5)},
         {5, Matrix_3x3(1,0,2, 0,1,0, 0,0,5)},
         {5, Matrix_3x3(1,0,2, 0,1,1, 0,0,5)},
         {5, Matrix_3x3(1,0,2, 0,1,2, 0,0,5)},
         {5, Matrix_3x3(1,0,2, 0,1,3, 0,0,5)},
         {5, Matrix_3x3(1,0,2, 0,1,4, 0,0,5)},
         {5, Matrix_3x3(1,0,3, 0,1,0, 0,0,5)},
         {5, Matrix_3x3(1,0,3, 0,1,1, 0,0,5)},
         {5, Matrix_3x3(1,0,3, 0,1,2, 0,0,5)},
         {5, Matrix_3x3(1,0,3, 0,1,3, 0,0,5)},
         {5, Matrix_3x3(1,0,3, 0,1,4, 0,0,5)},
         {5, Matrix_3x3(1,0,4, 0,1,0, 0,0,5)},
         {5, Matrix_3x3(1,0,4, 0,1,1, 0,0,5)},
         {5, Matrix_3x3(1,0,4, 0,1,2, 0,0,5)},
         {5, Matrix_3x3(1,0,4, 0,1,3, 0,0,5)},
         {5, Matrix_3x3(1,0,4, 0,1,4, 0,0,5)},
         {5, Matrix_3x3(1,0,0, 0,5,0, 0,0,1)},
         {5, Matrix_3x3(5,0,0, 0,1,0, 0,0,1)},
         {5, Matrix_3x3(5,1,0, 0,1,0, 0,0,1)},
         {5, Matrix_3x3(5,2,0, 0,1,0, 0,0,1)},
         {5, Matrix_3x3(5,3,0, 0,1,0, 0,0,1)},
         {5, Matrix_3x3(5,4,0, 0,1,0, 0,0,1)},
         // order-6 (91 matrices)
         {6, Matrix_3x3(1,0,0, 0,1,0, 0,0,6)},
         {6, Matrix_3x3(1,0,0, 0,1,1, 0,0,6)},
         {6, Matrix_3x3(1,0,0, 0,1,2, 0,0,6)},
         {6, Matrix_3x3(1,0,0, 0,1,3, 0,0,6)},
         {6, Matrix_3x3(1,0,0, 0,1,4, 0,0,6)},
         {6, Matrix_3x3(1,0,0, 0,1,5, 0,0,6)},
         {6, Matrix_3x3(1,0,1, 0,1,0, 0,0,6)},
         {6, Matrix_3x3(1,0,1, 0,1,1, 0,0,6)},
         {6, Matrix_3x3(1,0,1, 0,1,2, 0,0,6)},
         {6, Matrix_3x3(1,0,1, 0,1,3, 0,0,6)},
         {6, Matrix_3x3(1,0,1, 0,1,4, 0,0,6)},
         {6, Matrix_3x3(1,0,1, 0,1,5, 0,0,6)},
         {6, Matrix_3x3(1,0,2, 0,1,0, 0,0,6)},
         {6, Matrix_3x3(1,0,2, 0,1,1, 0,0,6)},
         {6, Matrix_3x3(1,0,2, 0,1,2, 0,0,6)},
         {6, Matrix_3x3(1,0,2, 0,1,3, 0,0,6)},
         {6, Matrix_3x3(1,0,2, 0,1,4, 0,0,6)},
         {6, Matrix_3x3(1,0,2, 0,1,5, 0,0,6)},
         {6, Matrix_3x3(1,0,3, 0,1,0, 0,0,6)},
         {6, Matrix_3x3(1,0,3, 0,1,1, 0,0,6)},
         {6, Matrix_3x3(1,0,3, 0,1,2, 0,0,6)},
         {6, Matrix_3x3(1,0,3, 0,1,3, 0,0,6)},
         {6, Matrix_3x3(1,0,3, 0,1,4, 0,0,6)},
         {6, Matrix_3x3(1,0,3, 0,1,5, 0,0,6)},
         {6, Matrix_3x3(1,0,4, 0,1,0, 0,0,6)},
         {6, Matrix_3x3(1,0,4, 0,1,1, 0,0,6)},
         {6, Matrix_3x3(1,0,4, 0,1,2, 0,0,6)},
         {6, Matrix_3x3(1,0,4, 0,1,3, 0,0,6)},
         {6, Matrix_3x3(1,0,4, 0,1,4, 0,0,6)},
         {6, Matrix_3x3(1,0,4, 0,1,5, 0,0,6)},
         {6, Matrix_3x3(1,0,5, 0,1,0, 0,0,6)},
         {6, Matrix_3x3(1,0,5, 0,1,1, 0,0,6)},
         {6, Matrix_3x3(1,0,5, 0,1,2, 0,0,6)},
         {6, Matrix_3x3(1,0,5, 0,1,3, 0,0,6)},
         {6, Matrix_3x3(1,0,5, 0,1,4, 0,0,6)},
         {6, Matrix_3x3(1,0,5, 0,1,5, 0,0,6)},
         {6, Matrix_3x3(1,0,0, 0,2,0, 0,0,3)},
         {6, Matrix_3x3(1,0,0, 0,2,1, 0,0,3)},
         {6, Matrix_3x3(1,0,0, 0,2,2, 0,0,3)},
         {6, Matrix_3x3(1,0,1, 0,2,0, 0,0,3)},
         {6, Matrix_3x3(1,0,1, 0,2,1, 0,0,3)},
         {6, Matrix_3x3(1,0,1, 0,2,2, 0,0,3)},
         {6, Matrix_3x3(1,0,2, 0,2,0, 0,0,3)},
         {6, Matrix_3x3(1,0,2, 0,2,1, 0,0,3)},
         {6, Matrix_3x3(1,0,2, 0,2,2, 0,0,3)},
         {6, Matrix_3x3(1,0,0, 0,3,0, 0,0,2)},
         {6, Matrix_3x3(1,0,0, 0,3,1, 0,0,2)},
         {6, Matrix_3x3(1,0,1, 0,3,0, 0,0,2)},
         {6, Matrix_3x3(1,0,1, 0,3,1, 0,0,2)},
         {6, Matrix_3x3(1,0,0, 0,6,0, 0,0,1)},
         {6, Matrix_3x3(2,0,0, 0,1,0, 0,0,3)},
         {6, Matrix_3x3(2,0,0, 0,1,1, 0,0,3)},
         {6, Matrix_3x3(2,0,0, 0,1,2, 0,0,3)},
         {6, Matrix_3x3(2,0,1, 0,1,0, 0,0,3)},
         {6, Matrix_3x3(2,0,1, 0,1,1, 0,0,3)},
         {6, Matrix_3x3(2,0,1, 0,1,2, 0,0,3)},
         {6, Matrix_3x3(2,0,2, 0,1,0, 0,0,3)},
         {6, Matrix_3x3(2,0,2, 0,1,1, 0,0,3)},
         {6, Matrix_3x3(2,0,2, 0,1,2, 0,0,3)},
         {6, Matrix_3x3(2,1,0, 0,1,0, 0,0,3)},
         {6, Matrix_3x3(2,1,0, 0,1,1, 0,0,3)},
         {6, Matrix_3x3(2,1,0, 0,1,2, 0,0,3)},
         {6, Matrix_3x3(2,1,1, 0,1,0, 0,0,3)},
         {6, Matrix_3x3(2,1,1, 0,1,1, 0,0,3)},
         {6, Matrix_3x3(2,1,1, 0,1,2, 0,0,3)},
         {6, Matrix_3x3(2,1,2, 0,1,0, 0,0,3)},
         {6, Matrix_3x3(2,1,2, 0,1,1, 0,0,3)},
         {6, Matrix_3x3(2,1,2, 0,1,2, 0,0,3)},
         {6, Matrix_3x3(2,0,0, 0,3,0, 0,0,1)},
         {6, Matrix_3x3(2,1,0, 0,3,0, 0,0,1)},
         {6, Matrix_3x3(3,0,0, 0,1,0, 0,0,2)},
         {6, Matrix_3x3(3,0,0, 0,1,1, 0,0,2)},
         {6, Matrix_3x3(3,0,1, 0,1,0, 0,0,2)},
         {6, Matrix_3x3(3,0,1, 0,1,1, 0,0,2)},
         {6, Matrix_3x3(3,1,0, 0,1,0, 0,0,2)},
         {6, Matrix_3x3(3,1,0, 0,1,1, 0,0,2)},
         {6, Matrix_3x3(3,1,1, 0,1,0, 0,0,2)},
         {6, Matrix_3x3(3,1,1, 0,1,1, 0,0,2)},
         {6, Matrix_3x3(3,2,0, 0,1,0, 0,0,2)},
         {6, Matrix_3x3(3,2,0, 0,1,1, 0,0,2)},
         {6, Matrix_3x3(3,2,1, 0,1,0, 0,0,2)},
         {6, Matrix_3x3(3,2,1, 0,1,1, 0,0,2)},
         {6, Matrix_3x3(3,0,0, 0,2,0, 0,0,1)},
         {6, Matrix_3x3(3,1,0, 0,2,0, 0,0,1)},
         {6, Matrix_3x3(3,2,0, 0,2,0, 0,0,1)},
         {6, Matrix_3x3(6,0,0, 0,1,0, 0,0,1)},
         {6, Matrix_3x3(6,1,0, 0,1,0, 0,0,1)},
         {6, Matrix_3x3(6,2,0, 0,1,0, 0,0,1)},
         {6, Matrix_3x3(6,3,0, 0,1,0, 0,0,1)},
         {6, Matrix_3x3(6,4,0, 0,1,0, 0,0,1)},
         {6, Matrix_3x3(6,5,0, 0,1,0, 0,0,1)},
      };

      const std::set<int> orderSet(orders.begin(), orders.end());
      std::vector<Matrix_3x3> result;
      for (const auto& p : all)
         if (orderSet.count(p.first)) result.push_back(p.second);
      return result;
   }

} // namespace TransformationMatrices


#endif // TRANSFORMATION_MATRICES_H
