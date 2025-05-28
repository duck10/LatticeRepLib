#ifndef MATRIXSETS_B4MATCHER_H
#define MATRIXSETS_B4MATCHER_H

#include "Matrix_3x3.h"

namespace MatrixSets {
   // Matrices from B4Matcher.cpp
   static const Matrix_3x3 B4_CRITICAL(-1.0, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0);
   static const Matrix_3x3 B4_SHEAR_XY_NEG_XZ(-1, 1, 0, 0, 1, 0, 0, 0, -1);

   // Add these simple sign flip matrices:
   static const Matrix_3x3 B4_NEG_XY(-1, 0, 0, 0, -1, 0, 0, 0, 1);   // -x,-y
   static const Matrix_3x3 B4_NEG_XZ(-1, 0, 0, 0, 1, 0, 0, 0, -1);   // -x,-z  
   static const Matrix_3x3 B4_NEG_YZ(1, 0, 0, 0, -1, 0, 0, 0, -1);   // -y,-z
}





#endif // !MATRIXSETS_B4MATCHER_H
