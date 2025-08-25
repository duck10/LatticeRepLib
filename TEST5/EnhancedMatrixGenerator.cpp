#include "EnhancedMatrixGenerator.h"
#include <algorithm>
#include <cmath>

std::vector<Matrix_3x3> EnhancedMatrixGenerator::GenerateTargetedMatrices(const GenerationStrategy& strategy) {
   std::vector<Matrix_3x3> matrices;

   // Generate different types based on strategy
   auto shearMatrices = GenerateShearMatrices(strategy.allowedElements);
   auto scalingMatrices = GenerateScalingCombinations(strategy.allowedElements);
   auto permutationMatrices = GenerateTargetedPermutations(strategy.allowedElements);

   // Combine all matrices
   matrices.insert(matrices.end(), shearMatrices.begin(), shearMatrices.end());
   matrices.insert(matrices.end(), scalingMatrices.begin(), scalingMatrices.end());
   matrices.insert(matrices.end(), permutationMatrices.begin(), permutationMatrices.end());

   if (strategy.includeHigherOrderCombinations) {
      auto compositeMatrices = GenerateCompositeMatrices(strategy.allowedElements);
      matrices.insert(matrices.end(), compositeMatrices.begin(), compositeMatrices.end());
   }

   // Filter based on strategy requirements
   std::vector<Matrix_3x3> filtered;
   for (const auto& matrix : matrices) {
      if (IsValidForStrategy(matrix, strategy)) {
         if (!strategy.focusOnOptimizers || HasPromiseForOptimization(matrix)) {
            filtered.push_back(matrix);
         }
      }
   }

   return RemoveDuplicates(filtered);
}

std::vector<Matrix_3x3> EnhancedMatrixGenerator::GenerateOptimizerFocusedSet() {
   GenerationStrategy strategy;
   strategy.allowedElements = { -3, -2, -1, 0, 1, 2, 3 };
   strategy.maxAbsElement = 3;
   strategy.includeHigherOrderCombinations = true;
   strategy.focusOnOptimizers = true;
   strategy.description = "Optimizer-focused with elements up to ±3";

   return GenerateTargetedMatrices(strategy);
}

std::vector<Matrix_3x3> EnhancedMatrixGenerator::GenerateHighEffectivenessSet() {
   std::vector<Matrix_3x3> matrices;

   // Based on analysis, focus on matrices similar to top performers
   // Generate matrices that create strong shear effects
   for (int shear : {-3, -2, 2, 3}) {
      for (int i = 0; i < 3; ++i) {
         for (int j = 0; j < 3; ++j) {
            if (i != j) {
               // Create shear matrix v_i = v_i + shear*v_j
               Matrix_3x3 matrix = CreateIdentityMatrix();
               matrix[i * 3 + j] = shear;

               if (IsValidUnimodular(matrix)) {
                  matrices.push_back(matrix);
               }
            }
         }
      }
   }

   // Generate matrices with multiple shear operations
   for (int shear1 : {-2, 2}) {
      for (int shear2 : {-2, 2}) {
         if (shear1 != shear2) {
            Matrix_3x3 matrix = CreateIdentityMatrix();
            matrix[0 * 3 + 1] = shear1;  // v0 = v0 + shear1*v1
            matrix[1 * 3 + 2] = shear2;  // v1 = v1 + shear2*v2

            if (IsValidUnimodular(matrix)) {
               matrices.push_back(matrix);
            }

            // Try different combinations
            matrix = CreateIdentityMatrix();
            matrix[0 * 3 + 2] = shear1;  // v0 = v0 + shear1*v2
            matrix[1 * 3 + 0] = shear2;  // v1 = v1 + shear2*v0

            if (IsValidUnimodular(matrix)) {
               matrices.push_back(matrix);
            }
         }
      }
   }

   return RemoveDuplicates(matrices);
}

std::vector<Matrix_3x3> EnhancedMatrixGenerator::GenerateShearMatrices(const std::vector<int>& elements) {
   std::vector<Matrix_3x3> matrices;

   for (int shear : elements) {
      if (shear == 0) continue;

      for (int i = 0; i < 3; ++i) {
         for (int j = 0; j < 3; ++j) {
            if (i != j) {
               Matrix_3x3 matrix = CreateShearMatrix(i, j, shear);
               if (IsValidUnimodular(matrix)) {
                  matrices.push_back(matrix);
               }
            }
         }
      }
   }

   return matrices;
}

std::vector<Matrix_3x3> EnhancedMatrixGenerator::GenerateScalingCombinations(const std::vector<int>& elements) {
   std::vector<Matrix_3x3> matrices;

   // Generate scaling matrices with determinant ±1
   for (int s1 : elements) {
      for (int s2 : elements) {
         if (s1 == 0 || s2 == 0) continue;

         int s3 = 1 / (s1 * s2);  // Ensure det = 1
         if (std::find(elements.begin(), elements.end(), s3) != elements.end()) {
            Matrix_3x3 matrix = CreateScalingMatrix(s1, s2, s3);
            if (IsValidUnimodular(matrix)) {
               matrices.push_back(matrix);
            }
         }

         s3 = -1 / (s1 * s2);  // Ensure det = -1
         if (std::find(elements.begin(), elements.end(), s3) != elements.end()) {
            Matrix_3x3 matrix = CreateScalingMatrix(s1, s2, s3);
            if (IsValidUnimodular(matrix)) {
               matrices.push_back(matrix);
            }
         }
      }
   }

   return matrices;
}

std::vector<Matrix_3x3> EnhancedMatrixGenerator::GenerateCompositeMatrices(const std::vector<int>& elements) {
   std::vector<Matrix_3x3> matrices;

   // Generate combinations of basic transformations
   auto shearMatrices = GenerateShearMatrices({ -2, -1, 1, 2 });

   for (size_t i = 0; i < shearMatrices.size() && i < 10; ++i) {
      for (size_t j = i + 1; j < shearMatrices.size() && j < 10; ++j) {
         Matrix_3x3 composite = CreateCompositeMatrix(shearMatrices[i], shearMatrices[j]);
         if (IsValidUnimodular(composite)) {
            bool hasValidElements = true;
            for (int k = 0; k < 9; ++k) {
               int elem = static_cast<int>(std::round(composite[k]));
               if (std::find(elements.begin(), elements.end(), elem) == elements.end()) {
                  hasValidElements = false;
                  break;
               }
            }
            if (hasValidElements) {
               matrices.push_back(composite);
            }
         }
      }
   }

   return matrices;
}

std::vector<Matrix_3x3> EnhancedMatrixGenerator::GenerateTargetedPermutations(const std::vector<int>& elements) {
   std::vector<Matrix_3x3> matrices;

   std::vector<std::vector<int>> permutations = { {0,1,2}, {0,2,1}, {1,0,2}, {1,2,0}, {2,0,1}, {2,1,0} };

   for (const auto& perm : permutations) {
      for (int sign0 : elements) {
         for (int sign1 : elements) {
            for (int sign2 : elements) {
               if (sign0 == 0 || sign1 == 0 || sign2 == 0) continue;

               int det = sign0 * sign1 * sign2;
               if (det != 1 && det != -1) continue;

               Matrix_3x3 matrix;
               for (int i = 0; i < 9; ++i) matrix[i] = 0;

               matrix[0 * 3 + perm[0]] = sign0;
               matrix[1 * 3 + perm[1]] = sign1;
               matrix[2 * 3 + perm[2]] = sign2;

               matrices.push_back(matrix);
            }
         }
      }
   }

   return matrices;
}

Matrix_3x3 EnhancedMatrixGenerator::CreateShearMatrix(int axis1, int axis2, int factor) {
   Matrix_3x3 matrix = CreateIdentityMatrix();
   matrix[axis1 * 3 + axis2] = factor;
   return matrix;
}

Matrix_3x3 EnhancedMatrixGenerator::CreateScalingMatrix(int scale1, int scale2, int scale3) {
   Matrix_3x3 matrix;
   for (int i = 0; i < 9; ++i) matrix[i] = 0;
   matrix[0] = scale1;
   matrix[4] = scale2;
   matrix[8] = scale3;
   return matrix;
}

Matrix_3x3 EnhancedMatrixGenerator::CreateCompositeMatrix(const Matrix_3x3& base, const Matrix_3x3& modifier) {
   // Matrix multiplication: result = base * modifier
   Matrix_3x3 result;
   for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
         result[i * 3 + j] = 0;
         for (int k = 0; k < 3; ++k) {
            result[i * 3 + j] += base[i * 3 + k] * modifier[k * 3 + j];
         }
      }
   }
   return result;
}

bool EnhancedMatrixGenerator::IsValidForStrategy(const Matrix_3x3& matrix, const GenerationStrategy& strategy) {
   // Check if all elements are within allowed set
   for (int i = 0; i < 9; ++i) {
      int elem = static_cast<int>(std::round(matrix[i]));
      if (std::abs(elem) > strategy.maxAbsElement) return false;
      if (std::find(strategy.allowedElements.begin(), strategy.allowedElements.end(), elem) == strategy.allowedElements.end()) {
         return false;
      }
   }

   return IsValidUnimodular(matrix);
}

bool EnhancedMatrixGenerator::HasPromiseForOptimization(const Matrix_3x3& matrix) {
   // Heuristic: matrices with moderate off-diagonal elements tend to be good optimizers
   int offDiagonalSum = 0;
   int nonZeroOffDiagonal = 0;

   for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
         if (i != j) {
            int elem = static_cast<int>(std::abs(matrix[i * 3 + j]));
            if (elem > 0) {
               offDiagonalSum += elem;
               nonZeroOffDiagonal++;
            }
         }
      }
   }

   // Prefer matrices with 1-3 non-zero off-diagonal elements of moderate size
   return (nonZeroOffDiagonal >= 1 && nonZeroOffDiagonal <= 3 && offDiagonalSum >= 2 && offDiagonalSum <= 6);
}

std::vector<Matrix_3x3> EnhancedMatrixGenerator::RemoveDuplicates(const std::vector<Matrix_3x3>& matrices) {
   std::vector<Matrix_3x3> unique;

   for (const auto& matrix : matrices) {
      bool isUnique = true;
      for (const auto& existing : unique) {
         if (MatricesEqual(matrix, existing)) {
            isUnique = false;
            break;
         }
      }
      if (isUnique) {
         unique.push_back(matrix);
      }
   }

   return unique;
}

bool EnhancedMatrixGenerator::MatricesEqual(const Matrix_3x3& a, const Matrix_3x3& b) {
   for (int i = 0; i < 9; ++i) {
      if (std::abs(a[i] - b[i]) > 1e-10) return false;
   }
   return true;
}

Matrix_3x3 EnhancedMatrixGenerator::CreateIdentityMatrix() {
   Matrix_3x3 matrix;
   for (int i = 0; i < 9; ++i) matrix[i] = 0;
   matrix[0] = matrix[4] = matrix[8] = 1;
   return matrix;
}

bool EnhancedMatrixGenerator::IsValidUnimodular(const Matrix_3x3& matrix) {
   double det = matrix.Det();
   return (std::abs(det - 1.0) < 1e-10 || std::abs(det + 1.0) < 1e-10);
}
