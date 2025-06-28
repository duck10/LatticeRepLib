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

#endif // MATRIX_CELL_OPERATOR_DEFINED

#endif // MATRIXCELLOPERATOR_H

