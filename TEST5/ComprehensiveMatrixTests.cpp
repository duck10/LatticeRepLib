//#include "MatrixAnalyzer.h"
//#include "TransformationMatrices.h"
//#include <iostream>
//#include <iomanip>
//#include <chrono>
//#include <cmath>
//
//// Enhanced test function with timing and detailed analysis
//std::vector<Matrix_3x3> TestGenerateSystematicWithTiming(const std::vector<double>& allowedElements, const std::string& setName) {
//   std::vector<Matrix_3x3> result;
//   const size_t n = allowedElements.size();
//
//   std::cout << "\n=== TESTING " << setName << " ===" << std::endl;
//   std::cout << "Elements: {";
//   for (size_t i = 0; i < allowedElements.size(); ++i) {
//      std::cout << allowedElements[i];
//      if (i < allowedElements.size() - 1) std::cout << ", ";
//   }
//   std::cout << "}" << std::endl;
//
//   size_t totalCombinations = std::pow(n, 9);
//   std::cout << "Total combinations to check: " << totalCombinations << std::endl;
//
//   auto start = std::chrono::high_resolution_clock::now();
//
//   size_t validCount = 0;
//   size_t detPlus1 = 0, detMinus1 = 0, detZero = 0, detOther = 0;
//
//   // Systematic generation with detailed statistics
//   for (size_t a00 = 0; a00 < n; a00++) {
//      if (n > 3 && a00 % std::max(1ULL, n / 5) == 0) {
//         std::cout << "Progress: " << std::setprecision(1) << std::fixed
//            << (100.0 * a00) / n << "% complete..." << std::endl;
//      }
//
//      for (size_t a01 = 0; a01 < n; a01++) {
//         for (size_t a02 = 0; a02 < n; a02++) {
//            for (size_t a10 = 0; a10 < n; a10++) {
//               for (size_t a11 = 0; a11 < n; a11++) {
//                  for (size_t a12 = 0; a12 < n; a12++) {
//                     for (size_t a20 = 0; a20 < n; a20++) {
//                        for (size_t a21 = 0; a21 < n; a21++) {
//                           for (size_t a22 = 0; a22 < n; a22++) {
//                              Matrix_3x3 mat = {
//                                  allowedElements[a00], allowedElements[a01], allowedElements[a02],
//                                  allowedElements[a10], allowedElements[a11], allowedElements[a12],
//                                  allowedElements[a20], allowedElements[a21], allowedElements[a22]
//                              };
//
//                              double det = mat.Det();
//
//                              // Classify determinants
//                              if (std::abs(det - 1.0) < 1e-10) {
//                                 detPlus1++;
//                                 result.emplace_back(mat);
//                                 validCount++;
//                              }
//                              else if (std::abs(det + 1.0) < 1e-10) {
//                                 detMinus1++;
//                              }
//                              else if (std::abs(det) < 1e-10) {
//                                 detZero++;
//                              }
//                              else {
//                                 detOther++;
//                              }
//                           }
//                        }
//                     }
//                  }
//               }
//            }
//         }
//      }
//   }
//
//   auto end = std::chrono::high_resolution_clock::now();
//   auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
//
//   // Detailed results
//   std::cout << "Generation complete in " << duration.count() << " ms" << std::endl;
//   std::cout << "Determinant analysis:" << std::endl;
//   std::cout << "  det = +1: " << detPlus1 << " (" << std::setprecision(4) << std::fixed << (100.0 * detPlus1) / totalCombinations << "%)" << std::endl;
//   std::cout << "  det = -1: " << detMinus1 << " (" << std::setprecision(4) << std::fixed << (100.0 * detMinus1) / totalCombinations << "%)" << std::endl;
//   std::cout << "  det =  0: " << detZero << " (" << std::setprecision(4) << std::fixed << (100.0 * detZero) / totalCombinations << "%)" << std::endl;
//   std::cout << "  det = other: " << detOther << " (" << std::setprecision(4) << std::fixed << (100.0 * detOther) / totalCombinations << "%)" << std::endl;
//   std::cout << "Unimodular matrices (det = ±1): " << (detPlus1 + detMinus1) << " (" << std::setprecision(4) << std::fixed << (100.0 * (detPlus1 + detMinus1)) / totalCombinations << "%)" << std::endl;
//   std::cout << "Valid for our purposes (det = +1): " << validCount << std::endl;
//
//   return result;
//}
//
//// Test efficiency by analyzing a sample of matrices
//void TestMatrixEffectiveness(const std::vector<Matrix_3x3>& matrices, const std::string& setName, size_t sampleSize = 100) {
//   std::cout << "\n=== EFFECTIVENESS SAMPLING: " << setName << " ===" << std::endl;
//
//   if (matrices.empty()) {
//      std::cout << "No matrices to test!" << std::endl;
//      return;
//   }
//
//   // Test a sample for performance
//   size_t testCount = std::min(sampleSize, matrices.size());
//   std::cout << "Testing effectiveness on " << testCount << " random matrices..." << std::endl;
//
//   int highEffCount = 0;
//   int moderateEffCount = 0;
//   double totalEff = 0;
//   double maxEff = 0;
//
//   // Sample matrices at regular intervals
//   for (size_t i = 0; i < testCount; ++i) {
//      size_t index = (i * matrices.size()) / testCount;
//      const Matrix_3x3& matrix = matrices[index];
//
//      // Simplified effectiveness test (you'd use your real AnalyzeGeometricEffects here)
//      // For now, just analyze matrix properties
//      int maxElement = 0;
//      int nonZeroCount = 0;
//      for (int j = 0; j < 9; ++j) {
//         maxElement = std::max(maxElement, static_cast<int>(std::abs(matrix[j])));
//         if (std::abs(matrix[j]) > 1e-10) nonZeroCount++;
//      }
//
//      // Rough effectiveness heuristic based on matrix properties
//      double effectiveness = 0.0;
//      if (maxElement >= 2) effectiveness += 0.1;  // Larger elements tend to be more effective
//      if (nonZeroCount >= 6) effectiveness += 0.05; // More complex transformations
//      if (maxElement == 2 && nonZeroCount <= 6) effectiveness += 0.15; // Sweet spot
//
//      totalEff += effectiveness;
//      maxEff = std::max(maxEff, effectiveness);
//
//      if (effectiveness > 0.1) highEffCount++;
//      if (effectiveness > 0.05) moderateEffCount++;
//   }
//
//   double avgEff = totalEff / testCount;
//
//   std::cout << "Sample results:" << std::endl;
//   std::cout << "  Average effectiveness: " << std::setprecision(4) << std::fixed << avgEff << std::endl;
//   std::cout << "  Max effectiveness: " << maxEff << std::endl;
//   std::cout << "  High effectiveness (>0.1): " << highEffCount << " (" << std::setprecision(1) << (100.0 * highEffCount) / testCount << "%)" << std::endl;
//   std::cout << "  Moderate effectiveness (>0.05): " << moderateEffCount << " (" << std::setprecision(1) << (100.0 * moderateEffCount) / testCount << "%)" << std::endl;
//}
//
//// Analyze element distribution patterns
//void AnalyzeElementDistribution(const std::vector<Matrix_3x3>& matrices, const std::string& setName) {
//   std::cout << "\n=== ELEMENT DISTRIBUTION: " << setName << " ===" << std::endl;
//
//   if (matrices.empty()) return;
//
//   std::map<int, int> elementCount;
//   std::map<int, int> maxElementCount;
//
//   for (const auto& matrix : matrices) {
//      int maxEl = 0;
//      for (int i = 0; i < 9; ++i) {
//         int el = static_cast<int>(std::round(matrix[i]));
//         elementCount[el]++;
//         maxEl = std::max(maxEl, std::abs(el));
//      }
//      maxElementCount[maxEl]++;
//   }
//
//   std::cout << "Element frequency:" << std::endl;
//   for (const auto& [element, count] : elementCount) {
//      std::cout << "  " << std::setw(3) << element << ": " << count << std::endl;
//   }
//
//   std::cout << "Max element distribution:" << std::endl;
//   for (const auto& [maxEl, count] : maxElementCount) {
//      double percent = (100.0 * count) / matrices.size();
//      std::cout << "  max=" << maxEl << ": " << count << " matrices (" << std::setprecision(1) << std::fixed << percent << "%)" << std::endl;
//   }
//}
//
//int main() {
//   std::cout << "=== COMPREHENSIVE MATRIX GENERATION TESTS ===" << std::endl;
//
//   // Test 1: Verify known sets
//   std::cout << "\n" << std::string(60, '=') << std::endl;
//   std::cout << "VERIFICATION TESTS" << std::endl;
//
//   auto set1 = TestGenerateSystematicWithTiming({ -1, 0, 1 }, "{-1, 0, 1}");
//   auto original3480 = TransformationMatrices::generateUnimodularMatrices();
//   std::cout << "Match with original: " << (set1.size() == original3480.size() ? "YES" : "NO") << std::endl;
//
//   // Test 2: Extended sets
//   std::cout << "\n" << std::string(60, '=') << std::endl;
//   std::cout << "EXTENDED SET TESTS" << std::endl;
//
//   auto set2 = TestGenerateSystematicWithTiming({ -2, -1, 0, 1, 2 }, "{-2, -1, 0, 1, 2}");
//   auto set3 = TestGenerateSystematicWithTiming({ -1, 0, 1, 2 }, "{-1, 0, 1, 2}");
//   auto set4 = TestGenerateSystematicWithTiming({ -2, -1, 0, 1 }, "{-2, -1, 0, 1}");
//
//   // Test 3: Larger sets (if feasible)
//   std::cout << "\n" << std::string(60, '=') << std::endl;
//   std::cout << "LARGER SET TESTS" << std::endl;
//
//   // Only test if reasonable size
//   if (std::pow(6, 9) < 20000000) {  // ~10M limit
//      auto set5 = TestGenerateSystematicWithTiming({ -2, -1, 0, 1, 2, 3 }, "{-2, -1, 0, 1, 2, 3}");
//      AnalyzeElementDistribution(set5, "{-2, -1, 0, 1, 2, 3}");
//      TestMatrixEffectiveness(set5, "{-2, -1, 0, 1, 2, 3}", 200);
//   }
//   else {
//      std::cout << "Skipping {-2, -1, 0, 1, 2, 3} - too large for quick test" << std::endl;
//   }
//
//   // Test 4: Analysis of patterns
//   std::cout << "\n" << std::string(60, '=') << std::endl;
//   std::cout << "PATTERN ANALYSIS" << std::endl;
//
//   AnalyzeElementDistribution(set1, "{-1, 0, 1}");
//   AnalyzeElementDistribution(set2, "{-2, -1, 0, 1, 2}");
//
//   // Test 5: Effectiveness sampling
//   std::cout << "\n" << std::string(60, '=') << std::endl;
//   std::cout << "EFFECTIVENESS SAMPLING" << std::endl;
//
//   TestMatrixEffectiveness(set1, "{-1, 0, 1}", 100);
//   TestMatrixEffectiveness(set2, "{-2, -1, 0, 1, 2}", 200);
//
//   // Summary
//   std::cout << "\n" << std::string(60, '=') << std::endl;
//   std::cout << "SUMMARY" << std::endl;
//   std::cout << "Set sizes:" << std::endl;
//   std::cout << "  {-1, 0, 1}: " << set1.size() << " matrices" << std::endl;
//   std::cout << "  {-2, -1, 0, 1}: " << set4.size() << " matrices" << std::endl;
//   std::cout << "  {-1, 0, 1, 2}: " << set3.size() << " matrices" << std::endl;
//   std::cout << "  {-2, -1, 0, 1, 2}: " << set2.size() << " matrices" << std::endl;
//
//   double ratio2 = (double)set2.size() / set1.size();
//   std::cout << "Expansion ratio {-2,-1,0,1,2} vs {-1,0,1}: " << std::setprecision(2) << std::fixed << ratio2 << "x" << std::endl;
//
//   std::cout << "\nReady for full analysis with " << set2.size() << " extended matrices!" << std::endl;
//
//   return 0;
//}
