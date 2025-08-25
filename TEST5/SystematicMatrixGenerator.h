#ifndef SYSTEMATICMATRIXGENERATOR_H
#define SYSTEMATICMATRIXGENERATOR_H

#include "P3Utilities.h"
#include "LRL_Vector3.h"

#include <vector>

class SystematicMatrixGenerator {
public:
   // Generate ALL possible matrices with given elements and determinant constraint
   static std::vector<Matrix_3x3> GenerateAllUnimodularMatrices(
      const std::vector<double>& allowedElements,
      bool allowNegativeDeterminant = false
   );

   // Generate specific element sets
   static std::vector<Matrix_3x3> GenerateSet_Neg2To2();  // {-2,-1,0,1,2}
   static std::vector<Matrix_3x3> GenerateSet_Neg3To3();  // {-3,-2,-1,0,1,2,3}
   static std::vector<Matrix_3x3> GenerateSet_Neg1To1();  // {-1,0,1} - should match 3480

   // Analysis function to predict counts
   static void PredictMatrixCounts(const std::vector<double>& elements);

private:
   static bool IsValidDeterminant(double det, bool allowNegative);
};

#endif // SYSTEMATICMATRIXGENERATOR_H

