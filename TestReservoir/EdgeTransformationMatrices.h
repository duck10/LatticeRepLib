#ifndef  EDGETRANSFORMMATRICES_H
#define EDGETRANSFORMMATRICES_H

#include <vector>

#include "LRL_Vector3.h"

// single edge transforms
const Matrix_3x3 M_a_plus_b(1, 1, 0, 0, 1, 0, 0, 0, 1);
const Matrix_3x3 M_a_minus_b(1, -1, 0, 0, 1, 0, 0, 0, 1);
const Matrix_3x3 M_a_plus_c(1, 0, 1, 0, 1, 0, 0, 0, 1);
const Matrix_3x3 M_a_minus_c(1, 0, -1, 0, 1, 0, 0, 0, 1);

const Matrix_3x3 M_b_plus_a(1, 0, 0, 1, 1, 0, 0, 0, 1);
const Matrix_3x3 M_b_minus_a(1, 0, 0, -1, 1, 0, 0, 0, 1);
const Matrix_3x3 M_b_plus_c(1, 0, 0, 0, 1, 1, 0, 0, 1);
const Matrix_3x3 M_b_minus_c(1, 0, 0, 0, 1, -1, 0, 0, 1);

const Matrix_3x3 M_c_plus_a(1, 0, 0, 0, 1, 0, 1, 0, 1);
const Matrix_3x3 M_c_minus_a(1, 0, 0, 0, 1, 0, -1, 0, 1);
const Matrix_3x3 M_c_plus_b(1, 0, 0, 0, 1, 0, 0, 1, 1);
const Matrix_3x3 M_c_minus_b(1, 0, 0, 0, 1, 0, 0, -1, 1);

inline const std::vector<Matrix_3x3> SimpleEdgeTransforms = {
   M_a_plus_b, M_a_minus_b, M_a_plus_c, M_a_minus_c,
   M_b_plus_a, M_b_minus_a, M_b_plus_c, M_b_minus_c,
   M_c_plus_a, M_c_minus_a, M_c_plus_b, M_c_minus_b
};

inline std::vector<Matrix_3x3> GetSimpleEdgeTransforms() {
   return SimpleEdgeTransforms;
}

//double edge transforms
const Matrix_3x3 M_a_plus_b_plus_c(1, 1, 1, 0, 1, 0, 0, 0, 1);
const Matrix_3x3 M_a_plus_b_minus_c(1, 1, -1, 0, 1, 0, 0, 0, 1);
const Matrix_3x3 M_a_minus_b_plus_c(1, -1, 1, 0, 1, 0, 0, 0, 1);
const Matrix_3x3 M_a_minus_b_minus_c(1, -1, -1, 0, 1, 0, 0, 0, 1);

const Matrix_3x3 M_b_plus_a_plus_c(1, 0, 0, 1, 1, 1, 0, 0, 1);
const Matrix_3x3 M_b_plus_a_minus_c(1, 0, 0, 1, 1, -1, 0, 0, 1);
const Matrix_3x3 M_b_minus_a_plus_c(1, 0, 0, -1, 1, 1, 0, 0, 1);
const Matrix_3x3 M_b_minus_a_minus_c(1, 0, 0, -1, 1, -1, 0, 0, 1);

const Matrix_3x3 M_c_plus_a_plus_b(1, 0, 0, 0, 1, 0, 1, 1, 1);
const Matrix_3x3 M_c_plus_a_minus_b(1, 0, 0, 0, 1, 0, 1, -1, 1);
const Matrix_3x3 M_c_minus_a_plus_b(1, 0, 0, 0, 1, 0, -1, 1, 1);
const Matrix_3x3 M_c_minus_a_minus_b(1, 0, 0, 0, 1, 0, -1, -1, 1);

inline const std::vector<Matrix_3x3> DoubleEdgeTransforms = {
   M_a_plus_b_plus_c, M_a_plus_b_minus_c, M_a_minus_b_plus_c, M_a_minus_b_minus_c,
   M_b_plus_a_plus_c, M_b_plus_a_minus_c, M_b_minus_a_plus_c, M_b_minus_a_minus_c,
   M_c_plus_a_plus_b, M_c_plus_a_minus_b, M_c_minus_a_plus_b, M_c_minus_a_minus_b
};

inline std::vector<Matrix_3x3> GetDoubleEdgeTransforms() {
   return DoubleEdgeTransforms;
}

inline std::vector<Matrix_3x3> GetAllTransforms() {
   std::vector<Matrix_3x3> transforms = SimpleEdgeTransforms;
   transforms.insert(transforms.end(), 
      DoubleEdgeTransforms.begin(), DoubleEdgeTransforms.end());
   return transforms;
}

#endif // ! EDGETRANSFORMMATRICES_H

