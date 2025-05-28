// FILE: NiggliMatrices.h
// ======================
#ifndef NIGGLI_MATRICES_H
#define NIGGLI_MATRICES_H

#include "Matrix_3x3.h"
#include <vector>

namespace NiggliMatrices {
   // Presentation matrices (corrected versions)
   static const Matrix_3x3 SP1_3D(0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, -1.0);  // [b,a,-c]
   static const Matrix_3x3 SP2_3D(-1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);  // [-a,c,b]

   // Sign change matrices
   static const Matrix_3x3 SP34A_3D(1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, -1.0);
   static const Matrix_3x3 SP34B_3D(-1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0);
   static const Matrix_3x3 SP34C_3D(-1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 1.0);

   // Reduction operation matrices (the key ones for edge cases!)
   static const Matrix_3x3 R5_PLUS_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, -1.0, 1.0);   // [a, b, c-b]
   static const Matrix_3x3 R5_MINUS_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0);   // [a, b, c+b]

   static const Matrix_3x3 R6_PLUS_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, 0.0, 1.0);   // [a, b, c-a]
   static const Matrix_3x3 R6_MINUS_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);   // [a, b, c+a]

   static const Matrix_3x3 R7_PLUS_3D(1.0, 0.0, 0.0, -1.0, 1.0, 0.0, 0.0, 0.0, 1.0);   // [a, b-a, c]
   static const Matrix_3x3 R7_MINUS_3D(1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0);   // [a, b+a, c]

   static const Matrix_3x3 R8_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0);         // [a, b, c+a+b]

   static const Matrix_3x3 R12_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, -1.0, 1.0);      // [a, b, c-a-b]

   // Array of all Niggli matrices for easy iteration
   static inline const std::vector<Matrix_3x3> ALL_NIGGLI_MATRICES{
       SP1_3D, SP2_3D, SP34A_3D, SP34B_3D, SP34C_3D,
       R5_PLUS_3D, R5_MINUS_3D, R6_PLUS_3D, R6_MINUS_3D,
       R7_PLUS_3D, R7_MINUS_3D, R8_3D, R12_3D
   };
}

#endif // NIGGLI_MATRICES_H

