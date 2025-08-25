#ifndef MATRIXANALYZER_H
#define MATRIXANALYZER_H

#include "P3Utilities.h"
#include "TransformationMatrices.h"
#include "P3_Reduce.h"
#include "P3.h"
#include "P3TestGenerator.h"
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>

class CombinedMatrixAnalyzer {
public:
   struct AnalysisProperties {
      bool isPermutation;
      bool isProjection;
      bool isCombination;
      bool reducesScalarProducts;
      bool changesAngles;
      bool changesLengths;
      int determinant;
      int maxElement;
      std::string primaryEffect;
      double effectivenessScore;
      std::string sourceSet; // "3480" or "extended"
   };

   struct GeometricEffects {
      bool reducesScalarProducts;
      bool changesAngles;
      bool changesLengths;
      double effectivenessScore;
   };

   void RunCompleteAnalysis();

private:
   // Data members
   std::vector<Matrix_3x3> original3480;
   std::map<size_t, AnalysisProperties> original3480Database;
   std::map<std::string, std::set<size_t>> original3480Categories;

   std::vector<Matrix_3x3> extendedMatrices;
   std::map<size_t, AnalysisProperties> extendedDatabase;
   std::map<std::string, std::set<size_t>> extendedCategories;

   // Main analysis phases
   void Analyze3480Set();
   void AnalyzeExtendedSet();
   void CompareAllSets();
   void FindOptimalCombinedSet();

   // 3480 set analysis
   void Verify3480Assumptions();
   void Print3480Results();
   void AnalyzeCuratedSubset();

   // Extended set generation and analysis
   void GenerateExtendedMatrices();
   void GenerateSystematicMatrices(const std::vector<double>& elements);
   void PrintExtendedResults();
   void PredictMatrixCounts(const std::vector<double>& elements);

   // Matrix analysis methods
   AnalysisProperties AnalyzeMatrix(const Matrix_3x3& matrix, size_t index, const std::string& source);
   GeometricEffects AnalyzeGeometricEffects(const Matrix_3x3& matrix);

   // Classification methods
   bool IsPermutationMatrix(const Matrix_3x3& matrix);
   bool IsProjectionLike(const Matrix_3x3& matrix);
   bool IsCombinationMatrix(const Matrix_3x3& matrix);
   std::string DeterminePrimaryEffect(const AnalysisProperties& props);

   // Utility methods
   Matrix_3x3 CreateIdentityMatrix();
   bool IsValidUnimodular(const Matrix_3x3& matrix);
   bool MatricesEqual(const Matrix_3x3& a, const Matrix_3x3& b);
   int GetMaxAbsElement(const Matrix_3x3& matrix);

   // Statistics and analysis
   double CalculateAverageEffectiveness(const std::map<size_t, AnalysisProperties>& database);
   int CountHighEffectiveness(const std::map<size_t, AnalysisProperties>& database);
   double FindBestEffectiveness(const std::map<size_t, AnalysisProperties>& database);
   void AddToCategories(size_t index, const AnalysisProperties& props,
      std::map<std::string, std::set<size_t>>& categories);
};

// Global function
void RunCompleteMatrixAnalysis();

#endif // MATRIXANALYZER_H