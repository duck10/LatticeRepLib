#ifndef  EDGETRANSFORMMATRICES3X3_H
#define EDGETRANSFORMMATRICES3X3_H

#include <vector>

#include "LRL_Vector3.h"

// Each matrix is a 3x3 integer array
using Matrix3x3 = int[3][3];

// -------------------- Simple Edge Transforms --------------------
const Matrix3x3 M_a_plus_b = { {1, 1, 0}, {0, 1, 0}, {0, 0, 1} };
const Matrix3x3 M_a_minus_b = { {1,-1, 0}, {0, 1, 0}, {0, 0, 1} };
const Matrix3x3 M_a_plus_c = { {1, 0, 1}, {0, 1, 0}, {0, 0, 1} };
const Matrix3x3 M_a_minus_c = { {1, 0,-1}, {0, 1, 0}, {0, 0, 1} };

const Matrix3x3 M_b_plus_a = { {1, 0, 0}, {1, 1, 0}, {0, 0, 1} };
const Matrix3x3 M_b_minus_a = { {1, 0, 0}, {-1,1, 0}, {0, 0, 1} };
const Matrix3x3 M_b_plus_c = { {1, 0, 0}, {0, 1, 1}, {0, 0, 1} };
const Matrix3x3 M_b_minus_c = { {1, 0, 0}, {0, 1,-1}, {0, 0, 1} };

const Matrix3x3 M_c_plus_a = { {1, 0, 0}, {0, 1, 0}, {1, 0, 1} };
const Matrix3x3 M_c_minus_a = { {1, 0, 0}, {0, 1, 0}, {-1,0, 1} };
const Matrix3x3 M_c_plus_b = { {1, 0, 0}, {0, 1, 0}, {0, 1, 1} };
const Matrix3x3 M_c_minus_b = { {1, 0, 0}, {0, 1, 0}, {0,-1, 1} };

const std::vector<const Matrix3x3*> SimpleEdgeTransforms = {
   &M_a_plus_b, &M_a_minus_b, &M_a_plus_c, &M_a_minus_c,
   &M_b_plus_a, &M_b_minus_a, &M_b_plus_c, &M_b_minus_c,
   &M_c_plus_a, &M_c_minus_a, &M_c_plus_b, &M_c_minus_b
};

// -------------------- Double Edge Transforms --------------------
const Matrix3x3 M_a_plus_b_plus_c = { {1, 1, 1}, {0, 1, 0}, {0, 0, 1} };
const Matrix3x3 M_a_plus_b_minus_c = { {1, 1,-1}, {0, 1, 0}, {0, 0, 1} };
const Matrix3x3 M_a_minus_b_plus_c = { {1,-1, 1}, {0, 1, 0}, {0, 0, 1} };
const Matrix3x3 M_a_minus_b_minus_c = { {1,-1,-1}, {0, 1, 0}, {0, 0, 1} };

const Matrix3x3 M_b_plus_a_plus_c = { {1, 0, 0}, {1, 1, 1}, {0, 0, 1} };
const Matrix3x3 M_b_plus_a_minus_c = { {1, 0, 0}, {1, 1,-1}, {0, 0, 1} };
const Matrix3x3 M_b_minus_a_plus_c = { {1, 0, 0}, {-1,1, 1}, {0, 0, 1} };
const Matrix3x3 M_b_minus_a_minus_c = { {1, 0, 0}, {-1,1,-1}, {0, 0, 1} };

const Matrix3x3 M_c_plus_a_plus_b = { {1, 0, 0}, {0, 1, 0}, {1, 1, 1} };
const Matrix3x3 M_c_plus_a_minus_b = { {1, 0, 0}, {0, 1, 0}, {1,-1, 1} };
const Matrix3x3 M_c_minus_a_plus_b = { {1, 0, 0}, {0, 1, 0}, {-1,1, 1} };
const Matrix3x3 M_c_minus_a_minus_b = { {1, 0, 0}, {0, 1, 0}, {-1,-1,1} };

const std::vector<const Matrix3x3*> DoubleEdgeTransforms = {
   &M_a_plus_b_plus_c, &M_a_plus_b_minus_c, &M_a_minus_b_plus_c, &M_a_minus_b_minus_c,
   &M_b_plus_a_plus_c, &M_b_plus_a_minus_c, &M_b_minus_a_plus_c, &M_b_minus_a_minus_c,
   &M_c_plus_a_plus_b, &M_c_plus_a_minus_b, &M_c_minus_a_plus_b, &M_c_minus_a_minus_b
};

// -------------------- Full Set --------------------
const std::vector<const Matrix3x3*> AllEdgeTransforms = {
   SimpleEdgeTransforms.begin(), SimpleEdgeTransforms.end(),
   DoubleEdgeTransforms.begin(), DoubleEdgeTransforms.end()
};

#endif // EDGETRANSFORMMATRICES3X3_H