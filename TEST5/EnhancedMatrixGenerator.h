#ifndef ENHANCEDMATRIXGENERATOR_H
#define ENHANCEDMATRIXGENERATOR_H

#include "P3Utilities.h"
#include "TransformationMatrices.h"
#include "P3_Reduce.h"
#include "P3.h"
#include "LRL_Vector3.h"

#include <vector>
#include <set>

class EnhancedMatrixGenerator {
public:
   struct GenerationStrategy {
      std::vector<int> allowedElements;
      int maxAbsElement;
      bool includeHigherOrderCombinations;
      bool focusOnOptimizers;
      std::string description;
   };

   static std::vector<Matrix_3x3> GenerateTargetedMatrices(const GenerationStrategy& strategy);
   static std::vector<Matrix_3x3> GenerateOptimizerFocusedSet();
   static std::vector<Matrix_3x3> GenerateHighEffectivenessSet();

private:
   static std::vector<Matrix_3x3> GenerateReducedFormMatrices();
   static std::vector<Matrix_3x3> GenerateShearMatrices(const std::vector<int>& elements);
   static std::vector<Matrix_3x3> GenerateCompositeMatrices(const std::vector<int>& elements);
   static std::vector<Matrix_3x3> GenerateScalingCombinations(const std::vector<int>& elements);
   static std::vector<Matrix_3x3> GenerateTargetedPermutations(const std::vector<int>& elements);

   static Matrix_3x3 CreateShearMatrix(int axis1, int axis2, int factor);
   static Matrix_3x3 CreateScalingMatrix(int scale1, int scale2, int scale3);
   static Matrix_3x3 CreateCompositeMatrix(const Matrix_3x3& base, const Matrix_3x3& modifier);

   static bool IsValidForStrategy(const Matrix_3x3& matrix, const GenerationStrategy& strategy);
   static bool HasPromiseForOptimization(const Matrix_3x3& matrix);
   static std::vector<Matrix_3x3> RemoveDuplicates(const std::vector<Matrix_3x3>& matrices);
   static bool MatricesEqual(const Matrix_3x3& a, const Matrix_3x3& b);
   static Matrix_3x3 CreateIdentityMatrix();
   static bool IsValidUnimodular(const Matrix_3x3& matrix);
};

#endif // ENHANCEDMATRIXGENERATOR_H
