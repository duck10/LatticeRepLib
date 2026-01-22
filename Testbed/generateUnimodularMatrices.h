#ifndef GENERATE_UNIMODULAR_MATRICES_H
#define GENERATE_UNIMODULAR_MATRICES_H

#include "LRL_Vector3.h"

#include <cmath>
#include <iostream>
#include <vector>

class TransformationMatrices
{
public:
   inline std::vector<Matrix_3x3> generateUnimodularMatrices(int maxCoeff = 1) {
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
};

#endif // GENERATE_UNIMODULAR_MATRICES_H
