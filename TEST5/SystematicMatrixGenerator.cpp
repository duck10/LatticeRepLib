// Save this as SystematicMatrixGenerator.cpp in your project folder
#include "SystematicMatrixGenerator.h"
#include <iostream>
#include <iomanip>
#include <cmath>

std::vector<Matrix_3x3> SystematicMatrixGenerator::GenerateAllUnimodularMatrices(
   const std::vector<double>& allowedElements,
   bool allowNegativeDeterminant) {

   std::vector<Matrix_3x3> result;
   const size_t n = allowedElements.size();

   std::cout << "Generating all possible 3x3 matrices with " << n << " allowed elements..." << std::endl;
   std::cout << "Total combinations to check: " << std::pow(n, 9) << std::endl;

   size_t totalChecked = 0;
   size_t validCount = 0;

   // Systematic exhaustive generation - exactly like the 3480 function
   for (size_t a00 = 0; a00 < n; a00++) {
      if (a00 % std::max(1ULL, n / 5) == 0) {
         std::cout << "Progress: " << std::setprecision(1) << std::fixed
            << (100.0 * a00) / n << "% complete..." << std::endl;
      }

      for (size_t a01 = 0; a01 < n; a01++) {
         for (size_t a02 = 0; a02 < n; a02++) {
            for (size_t a10 = 0; a10 < n; a10++) {
               for (size_t a11 = 0; a11 < n; a11++) {
                  for (size_t a12 = 0; a12 < n; a12++) {
                     for (size_t a20 = 0; a20 < n; a20++) {
                        for (size_t a21 = 0; a21 < n; a21++) {
                           for (size_t a22 = 0; a22 < n; a22++) {
                              Matrix_3x3 mat = {
                                  allowedElements[a00], allowedElements[a01], allowedElements[a02],
                                  allowedElements[a10], allowedElements[a11], allowedElements[a12],
                                  allowedElements[a20], allowedElements[a21], allowedElements[a22]
                              };

                              totalChecked++;
                              double det = mat.Det();

                              if (IsValidDeterminant(det, allowNegativeDeterminant)) {
                                 result.emplace_back(mat);
                                 validCount++;
                              }
                           }
                        }
                     }
                  }
               }
            }
         }
      }
   }

   std::cout << "Generation complete!" << std::endl;
   std::cout << "Total combinations checked: " << totalChecked << std::endl;
   std::cout << "Valid unimodular matrices found: " << validCount << std::endl;
   std::cout << "Success rate: " << std::setprecision(4) << std::fixed
      << (100.0 * validCount) / totalChecked << "%" << std::endl;

   return result;
}

std::vector<Matrix_3x3> SystematicMatrixGenerator::GenerateSet_Neg1To1() {
   std::vector<double> elements = { -1, 0, 1 };
   return GenerateAllUnimodularMatrices(elements, false);  // Only det = +1
}

std::vector<Matrix_3x3> SystematicMatrixGenerator::GenerateSet_Neg2To2() {
   std::vector<double> elements = { -2, -1, 0, 1, 2 };
   return GenerateAllUnimodularMatrices(elements, false);  // Only det = +1
}

std::vector<Matrix_3x3> SystematicMatrixGenerator::GenerateSet_Neg3To3() {
   std::vector<double> elements = { -3, -2, -1, 0, 1, 2, 3 };
   return GenerateAllUnimodularMatrices(elements, false);  // Only det = +1
}

void SystematicMatrixGenerator::PredictMatrixCounts(const std::vector<double>& elements) {
   size_t n = elements.size();
   size_t totalCombinations = std::pow(n, 9);

   std::cout << "\n=== MATRIX COUNT PREDICTION ===" << std::endl;
   std::cout << "Element set size: " << n << std::endl;
   std::cout << "Elements: {";
   for (size_t i = 0; i < elements.size(); ++i) {
      std::cout << elements[i];
      if (i < elements.size() - 1) std::cout << ", ";
   }
   std::cout << "}" << std::endl;
   std::cout << "Total 3x3 combinations: " << totalCombinations << std::endl;

   // Rough estimates based on determinant constraint
   // For random integer matrices, roughly 1/n of combinations have det = +1
   double estimatedValidCount = totalCombinations / (2.0 * n);  // Rough heuristic

   std::cout << "Estimated matrices with det = +1: ~"
      << static_cast<size_t>(estimatedValidCount) << std::endl;

   // Known exact values for reference
   if (n == 3) {
      std::cout << "Known exact count for {-1,0,1}: 3480" << std::endl;
   }

   std::cout << "Generation time estimate: ";
   if (totalCombinations < 1000000) {
      std::cout << "< 1 second" << std::endl;
   }
   else if (totalCombinations < 100000000) {
      std::cout << "few seconds" << std::endl;
   }
   else if (totalCombinations < 10000000000ULL) {
      std::cout << "minutes" << std::endl;
   }
   else {
      std::cout << "hours+ (consider reducing element set)" << std::endl;
   }
   std::cout << std::endl;
}

bool SystematicMatrixGenerator::IsValidDeterminant(double det, bool allowNegative) {
   const double tolerance = 1e-10;

   if (std::abs(det - 1.0) < tolerance) {
      return true;  // det = +1
   }

   if (allowNegative && std::abs(det + 1.0) < tolerance) {
      return true;  // det = -1
   }

   return false;
}
