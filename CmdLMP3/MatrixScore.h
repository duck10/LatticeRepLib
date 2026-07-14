#ifndef MATRIXSCORE_H
#define MATRIXSCORE_H

#include "Matrix_3x3.h"

inline double matrixScore(const Matrix_3x3& m) {
   double norm = 0.0;
   int negCount = 0;
   int offDiagCount = 0;
   for (int i = 0; i < 9; ++i) {
      norm += m[i] * m[i];
      if (m[i] < 0) ++negCount;
      if (i != 0 && i != 4 && i != 8 && m[i] != 0) ++offDiagCount;
   }
   return norm + 0.1 * negCount + 0.05 * offDiagCount;
}

#endif // !MATRIXSCORE_H
