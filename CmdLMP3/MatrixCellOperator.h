#pragma warning(disable: 4101) // Visual Studio warning suppression
#ifndef MATRIXCELLOPERATOR_H
#define MATRIXCELLOPERATOR_H

#include "LRL_Cell.h"
#include "Matrix_3x3.h"
#include "B4.h"

// CRITICAL: Matrix multiplication operator for LRL_Cell
// This operator is essential for lattice transformations
// Define it once here to avoid conflicts

#ifndef MATRIX_CELL_OPERATOR_DEFINED
#define MATRIX_CELL_OPERATOR_DEFINED

inline LRL_Cell operator*(const Matrix_3x3& matrix, const LRL_Cell& cell) {
   // Convert cell to B4 representation
   B4 b4Cell(cell);

   // Apply transformation in B4 space
   B4 transformedB4 = matrix * b4Cell;

   // Convert back to LRL_Cell
   return LRL_Cell(transformedB4);
}


//inline LRL_Cell operator*(const Matrix_3x3& matrix, const LRL_Cell& cell) {
//   const G6 g = cell;
//   Matrix_3x3 G(
//      g[0], g[5] / 2.0, g[4] / 2.0,
//      g[5] / 2.0, g[1], g[3] / 2.0,
//      g[4] / 2.0, g[3] / 2.0, g[2]
//   );
//   const Matrix_3x3 Gprime = matrix.Transpose() * G * matrix;
//   G6 gout;
//   gout[0] = Gprime[0];
//   gout[1] = Gprime[4];
//   gout[2] = Gprime[8];
//   gout[3] = 2.0 * Gprime[5];
//   gout[4] = 2.0 * Gprime[2];
//   gout[5] = 2.0 * Gprime[1];
//   return LRL_Cell(gout);
//}

//inline LRL_Cell operator*(const Matrix_3x3& matrix, const LRL_Cell& cell) {
//   const G6 g = cell;
//   Matrix_3x3 G(
//      g[0], g[5] / 2.0, g[4] / 2.0,
//      g[5] / 2.0, g[1], g[3] / 2.0,
//      g[4] / 2.0, g[3] / 2.0, g[2]
//   );
//   const Matrix_3x3 Gprime = matrix * G * matrix.Transpose();
//   G6 gout;
//   gout[0] = Gprime[0];
//   gout[1] = Gprime[4];
//   gout[2] = Gprime[8];
//   gout[3] = 2.0 * Gprime[5];
//   gout[4] = 2.0 * Gprime[2];
//   gout[5] = 2.0 * Gprime[1];
//
//   static int callCount = 0;
//   ++callCount;
//   //if (callCount <= 3) {
//   //   std::cout << "; DEBUG operator* call " << callCount << std::endl;
//   //   std::cout << ";   matrix: " << matrix << std::endl;
//   //   std::cout << ";   input G6: " << g[0] << " " << g[1] << " " << g[2]
//   //      << " " << g[3] << " " << g[4] << " " << g[5] << std::endl;
//   //   std::cout << ";   G[0][0]=" << G[0] << " G[0][1]=" << G[1] << " G[0][2]=" << G[2] << std::endl;
//   //   std::cout << ";   G[1][0]=" << G[3] << " G[1][1]=" << G[4] << " G[1][2]=" << G[5] << std::endl;
//   //   std::cout << ";   G[2][0]=" << G[6] << " G[2][1]=" << G[7] << " G[2][2]=" << G[8] << std::endl;
//   //   std::cout << ";   Gprime[0][0]=" << Gprime[0] << " [0][1]=" << Gprime[1] << " [0][2]=" << Gprime[2] << std::endl;
//   //   std::cout << ";   Gprime[1][0]=" << Gprime[3] << " [1][1]=" << Gprime[4] << " [1][2]=" << Gprime[5] << std::endl;
//   //   std::cout << ";   Gprime[2][0]=" << Gprime[6] << " [2][1]=" << Gprime[7] << " [2][2]=" << Gprime[8] << std::endl;
//   //   std::cout << ";   output G6: " << gout[0] << " " << gout[1] << " " << gout[2]
//   //      << " " << gout[3] << " " << gout[4] << " " << gout[5] << std::endl;
//   //   std::cout << ";   output cell: " << LRL_Cell_Degrees(LRL_Cell(gout)) << std::endl;
//   //}
//
//   return LRL_Cell(gout);
//}

#endif // MATRIX_CELL_OPERATOR_DEFINED

#endif // MATRIXCELLOPERATOR_H

