#include "MatrixAnalyzer.h"
#include "P3TestGenerator.h"
#include <cmath>
#include <algorithm>

CombinedMatrixAnalyzer::AnalysisProperties CombinedMatrixAnalyzer::AnalyzeMatrix(const Matrix_3x3& matrix, size_t index, const std::string& source) {
   AnalysisProperties props = {};

   props.determinant = static_cast<int>(std::round(matrix.Det()));
   props.maxElement = GetMaxAbsElement(matrix);
   props.sourceSet = source;

   // Structural analysis
   props.isPermutation = IsPermutationMatrix(matrix);
   props.isProjection = IsProjectionLike(matrix);
   props.isCombination = IsCombinationMatrix(matrix);

   // Geometric effect analysis
   auto effects = AnalyzeGeometricEffects(matrix);
   props.reducesScalarProducts = effects.reducesScalarProducts;
   props.changesAngles = effects.changesAngles;
   props.changesLengths = effects.changesLengths;
   props.effectivenessScore = effects.effectivenessScore;

   props.primaryEffect = DeterminePrimaryEffect(props);

   return props;
}

CombinedMatrixAnalyzer::GeometricEffects CombinedMatrixAnalyzer::AnalyzeGeometricEffects(const Matrix_3x3& matrix) {
   GeometricEffects effects = {};

   // Generate comprehensive test cases using the proper P3TestGenerator
   auto testCases = P3TestGenerator::GenerateTestCases(25);  // Generate 25 diverse test cases

   double totalImprovement = 0;
   int improvementCount = 0;
   double totalAngleChange = 0;
   double totalLengthChange = 0;
   int validTestCount = 0;

   for (const auto& testCase : testCases) {
      const P3& testP3 = testCase.p3;

      // Verify test case is valid
      if (!P3TestGenerator::IsValidP3(testP3)) continue;

      LRL_Cell original(testP3);
      if (!original.IsValid()) continue;

      LRL_Cell transformed = ApplyMatrixToCell(original, matrix);
      if (!transformed.IsValid()) continue;

      P3 resultP3(transformed);
      validTestCount++;

      // Analyze scalar product changes
      double originalCost = testCase.originalCost;
      double transformedCost = P3TestGenerator::CalculateP3Cost(resultP3);

      if (transformedCost < originalCost && originalCost > 1e-10) {
         double improvement = (originalCost - transformedCost) / originalCost;
         totalImprovement += improvement;
         improvementCount++;
      }

      // Analyze angle and length changes
      for (size_t i = 0; i < 3; ++i) {
         double origAngle = std::atan2(testP3[i].second, testP3[i].first);
         double newAngle = std::atan2(resultP3[i].second, resultP3[i].first);
         totalAngleChange += std::abs(origAngle - newAngle);

         double origLen = std::sqrt(testP3[i].first * testP3[i].first + testP3[i].second * testP3[i].second);
         double newLen = std::sqrt(resultP3[i].first * resultP3[i].first + resultP3[i].second * resultP3[i].second);
         if (origLen > 1e-10) {
            totalLengthChange += std::abs(origLen - newLen) / origLen;
         }
      }
   }

   // Calculate effects based on results
   if (validTestCount > 0) {
      effects.reducesScalarProducts = (improvementCount > 0);
      effects.changesAngles = (totalAngleChange / validTestCount > 0.1);
      effects.changesLengths = (totalLengthChange / validTestCount > 0.1);
      effects.effectivenessScore = (improvementCount > 0) ? totalImprovement / improvementCount : 0.0;
   }
   else {
      // Fallback if no valid test cases
      effects.reducesScalarProducts = false;
      effects.changesAngles = false;
      effects.changesLengths = false;
      effects.effectivenessScore = 0.0;
   }

   return effects;
}

bool CombinedMatrixAnalyzer::IsPermutationMatrix(const Matrix_3x3& matrix) {
   // Check if matrix only contains 0, 1, -1 and represents row/column permutation
   for (int i = 0; i < 9; ++i) {
      int val = static_cast<int>(std::round(matrix[i]));
      if (val < -1 || val > 1) return false;
      if (std::abs(matrix[i] - val) > 1e-10) return false;
   }

   // Check if each row and column has exactly one ±1
   for (int row = 0; row < 3; ++row) {
      int nonZeroCount = 0;
      for (int col = 0; col < 3; ++col) {
         if (std::abs(matrix[row * 3 + col]) > 0.5) nonZeroCount++;
      }
      if (nonZeroCount != 1) return false;
   }
   return true;
}

bool CombinedMatrixAnalyzer::IsProjectionLike(const Matrix_3x3& matrix) {
   // Pattern: mostly identity with 1-2 off-diagonal ±1 elements
   int diagonalOnes = 0;
   int offDiagonalNonZero = 0;

   for (int i = 0; i < 3; ++i) {
      if (std::abs(matrix[i * 3 + i] - 1.0) < 1e-10) diagonalOnes++;
      for (int j = 0; j < 3; ++j) {
         if (i != j && std::abs(matrix[i * 3 + j]) > 1e-10) offDiagonalNonZero++;
      }
   }
   return (diagonalOnes >= 2 && offDiagonalNonZero <= 2 && !IsPermutationMatrix(matrix));
}

bool CombinedMatrixAnalyzer::IsCombinationMatrix(const Matrix_3x3& matrix) {
   // Pattern: identity matrix plus off-diagonal ±1 elements
   if (IsPermutationMatrix(matrix) || IsProjectionLike(matrix)) return false;

   int diagonalOnes = 0;
   for (int i = 0; i < 3; ++i) {
      if (std::abs(matrix[i * 3 + i] - 1.0) < 1e-10) diagonalOnes++;
   }
   return (diagonalOnes == 3); // All diagonal elements are 1
}

std::string CombinedMatrixAnalyzer::DeterminePrimaryEffect(const AnalysisProperties& props) {
   if (props.isPermutation) return "permutation";
   if (props.isProjection) return "projection";
   if (props.isCombination) return "combination";
   if (props.reducesScalarProducts) return "optimizer";
   return "other";
}

//P3 CombinedMatrixAnalyzer::CreateTestP3(const P3& p3) {
//   return p3;
//}
//
//P3 CombinedMatrixAnalyzer::CreateTestP3(double x1, double y1, double x2, double y2, double x3, double y3) {
//   P3 result;
//   result[0] = { x1, y1 };
//   result[1] = { x2, y2 };
//   result[2] = { x3, y3 };
//   return result;
//}

Matrix_3x3 CombinedMatrixAnalyzer::CreateIdentityMatrix() {
   Matrix_3x3 matrix;
   for (int i = 0; i < 9; ++i) matrix[i] = 0;
   matrix[0] = matrix[4] = matrix[8] = 1;
   return matrix;
}

bool CombinedMatrixAnalyzer::IsValidUnimodular(const Matrix_3x3& matrix) {
   double det = matrix.Det();
   return (std::abs(det - 1.0) < 1e-10 || std::abs(det + 1.0) < 1e-10);
}

bool CombinedMatrixAnalyzer::MatricesEqual(const Matrix_3x3& a, const Matrix_3x3& b) {
   for (int i = 0; i < 9; ++i) {
      if (std::abs(a[i] - b[i]) > 1e-10) return false;
   }
   return true;
}

int CombinedMatrixAnalyzer::GetMaxAbsElement(const Matrix_3x3& matrix) {
   int maxVal = 0;
   for (int i = 0; i < 9; ++i) {
      maxVal = std::max(maxVal, static_cast<int>(std::abs(matrix[i])));
   }
   return maxVal;
}

double CombinedMatrixAnalyzer::CalculateAverageEffectiveness(const std::map<size_t, AnalysisProperties>& database) {
   double total = 0;
   for (const auto& [index, props] : database) {
      total += props.effectivenessScore;
   }
   return total / database.size();
}

int CombinedMatrixAnalyzer::CountHighEffectiveness(const std::map<size_t, AnalysisProperties>& database) {
   int count = 0;
   for (const auto& [index, props] : database) {
      if (props.effectivenessScore > 0.1) count++;
   }
   return count;
}

double CombinedMatrixAnalyzer::FindBestEffectiveness(const std::map<size_t, AnalysisProperties>& database) {
   double best = 0;
   for (const auto& [index, props] : database) {
      best = std::max(best, props.effectivenessScore);
   }
   return best;
}

void CombinedMatrixAnalyzer::AddToCategories(size_t index, const AnalysisProperties& props,
   std::map<std::string, std::set<size_t>>& categories) {
   categories[props.primaryEffect].insert(index);
   if (props.reducesScalarProducts) categories["reduces_scalar_products"].insert(index);
   if (props.changesAngles) categories["changes_angles"].insert(index);
   if (props.changesLengths) categories["changes_lengths"].insert(index);
   if (props.effectivenessScore > 0.1) categories["high_effectiveness"].insert(index);
   if (props.effectivenessScore > 0.05) categories["moderate_effectiveness"].insert(index);
}

// Global function
void RunCompleteMatrixAnalysis() {
   CombinedMatrixAnalyzer analyzer;
   analyzer.RunCompleteAnalysis();
}