//#include "MatrixAnalyzer.h"
//#include "TransformationMatrices.h"
//#include <iostream>
//#include <iomanip>
//#include <chrono>
//#include <cmath>
//#include <random>
//#include <algorithm>
//
//// Enhanced test function with detailed analysis
//std::vector<Matrix_3x3> TestGenerateSystematicWithFullAnalysis(const std::vector<double>& allowedElements, const std::string& setName) {
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
//   std::cout << "Total combinations: " << totalCombinations << std::endl;
//
//   auto start = std::chrono::high_resolution_clock::now();
//
//   // Detailed statistics
//   size_t detPlus1 = 0, detMinus1 = 0, detZero = 0, detOther = 0;
//   std::map<int, int> detIntegerCounts;
//   std::map<int, int> maxElementDist;
//
//   // Systematic generation
//   for (size_t a00 = 0; a00 < n; a00++) {
//      if (n > 4 && a00 % std::max(1ULL, n / 10) == 0) {
//         std::cout << "Progress: " << std::setprecision(1) << std::fixed
//            << (100.0 * a00) / n << "%" << std::endl;
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
//                              int detInt = static_cast<int>(std::round(det));
//
//                              // Classify determinants
//                              if (std::abs(det - 1.0) < 1e-10) {
//                                 detPlus1++;
//                                 result.emplace_back(mat);
//
//                                 // Analyze max element for valid matrices
//                                 int maxEl = 0;
//                                 for (int i = 0; i < 9; ++i) {
//                                    maxEl = std::max(maxEl, static_cast<int>(std::abs(mat[i])));
//                                 }
//                                 maxElementDist[maxEl]++;
//
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
//
//                              // Track integer determinant distribution
//                              if (std::abs(det - detInt) < 1e-10 && std::abs(detInt) <= 10) {
//                                 detIntegerCounts[detInt]++;
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
//   // Results
//   std::cout << "Time: " << duration.count() << " ms" << std::endl;
//   std::cout << "Matrices found: " << result.size() << std::endl;
//   std::cout << "Success rate: " << std::setprecision(6) << std::fixed << (100.0 * result.size()) / totalCombinations << "%" << std::endl;
//
//   // Determinant analysis
//   std::cout << "Determinant distribution:" << std::endl;
//   std::cout << "  +1: " << detPlus1 << " (" << std::setprecision(4) << (100.0 * detPlus1) / totalCombinations << "%)" << std::endl;
//   std::cout << "  -1: " << detMinus1 << " (" << std::setprecision(4) << (100.0 * detMinus1) / totalCombinations << "%)" << std::endl;
//   std::cout << "   0: " << detZero << " (" << std::setprecision(4) << (100.0 * detZero) / totalCombinations << "%)" << std::endl;
//   std::cout << "  other: " << detOther << " (" << std::setprecision(4) << (100.0 * detOther) / totalCombinations << "%)" << std::endl;
//
//   // Max element distribution for valid matrices
//   std::cout << "Max element distribution (det=+1 matrices):" << std::endl;
//   for (const auto& [maxEl, count] : maxElementDist) {
//      std::cout << "  max=" << maxEl << ": " << count << " (" << std::setprecision(2) << (100.0 * count) / result.size() << "%)" << std::endl;
//   }
//
//   return result;
//}
//
//// Test random element sets
//void TestRandomElementSets() {
//   std::cout << "\n" << std::string(80, '=') << std::endl;
//   std::cout << "RANDOM ELEMENT SET EXPLORATION" << std::endl;
//
//   std::random_device rd;
//   std::mt19937 gen(rd());
//
//   // Test various element ranges
//   std::vector<std::pair<int, int>> ranges = {
//       {-1, 2},   // {-1, 0, 1, 2}
//       {-3, 1},   // {-3, -2, -1, 0, 1}
//       {0, 3},    // {0, 1, 2, 3}
//       {-2, 3},   // {-2, -1, 0, 1, 2, 3}
//       {-1, 3},   // {-1, 0, 1, 2, 3}
//       {-4, 0},   // {-4, -3, -2, -1, 0}
//       {1, 4},    // {1, 2, 3, 4}
//   };
//
//   for (const auto& [minEl, maxEl] : ranges) {
//      std::vector<double> elements;
//      for (int i = minEl; i <= maxEl; ++i) {
//         elements.push_back(i);
//      }
//
//      if (std::pow(elements.size(), 9) > 50000000) {  // 50M limit
//         std::cout << "\nSkipping range [" << minEl << ", " << maxEl << "] - too large" << std::endl;
//         continue;
//      }
//
//      std::string name = "[" + std::to_string(minEl) + ", " + std::to_string(maxEl) + "]";
//      auto matrices = TestGenerateSystematicWithFullAnalysis(elements, name);
//   }
//}
//
//// Test sparse element sets
//void TestSparseElementSets() {
//   std::cout << "\n" << std::string(80, '=') << std::endl;
//   std::cout << "SPARSE ELEMENT SET EXPLORATION" << std::endl;
//
//   // Test non-contiguous element sets
//   std::vector<std::vector<double>> sparseSets = {
//       {-3, -1, 1, 3},           // Skip even numbers
//       {-2, 0, 2},               // Only even numbers
//       {-5, -2, 0, 2, 5},        // Larger sparse set
//       {-1, 0, 1, 3},            // Skip 2
//       {-2, -1, 1, 2},           // Skip 0
//       {-4, -1, 0, 1, 4},        // Symmetric with gaps
//       {-3, -2, 2, 3},           // Skip -1, 0, 1
//   };
//
//   for (size_t i = 0; i < sparseSets.size(); ++i) {
//      const auto& elements = sparseSets[i];
//
//      if (std::pow(elements.size(), 9) > 50000000) {
//         std::cout << "\nSkipping sparse set " << i << " - too large" << std::endl;
//         continue;
//      }
//
//      std::string name = "sparse_" + std::to_string(i);
//      auto matrices = TestGenerateSystematicWithFullAnalysis(elements, name);
//   }
//}
//
//// Analyze scaling patterns
//void AnalyzeScalingPatterns() {
//   std::cout << "\n" << std::string(80, '=') << std::endl;
//   std::cout << "SCALING PATTERN ANALYSIS" << std::endl;
//
//   struct TestResult {
//      std::string name;
//      size_t elementCount;
//      size_t totalCombinations;
//      size_t validMatrices;
//      double successRate;
//      int timeMs;
//   };
//
//   std::vector<TestResult> results;
//
//   // Test systematic scaling
//   for (int maxAbs = 1; maxAbs <= 6; ++maxAbs) {
//      std::vector<double> elements;
//      for (int i = -maxAbs; i <= maxAbs; ++i) {
//         elements.push_back(i);
//      }
//
//      if (std::pow(elements.size(), 9) > 100000000) {  // 100M limit
//         std::cout << "Stopping at maxAbs=" << maxAbs << " - too large" << std::endl;
//         break;
//      }
//
//      std::string name = "[-" + std::to_string(maxAbs) + ", " + std::to_string(maxAbs) + "]";
//      std::cout << "\nTesting " << name << "..." << std::endl;
//
//      auto start = std::chrono::high_resolution_clock::now();
//      auto matrices = TestGenerateSystematicWithFullAnalysis(elements, name);
//      auto end = std::chrono::high_resolution_clock::now();
//      int timeMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//
//      TestResult result;
//      result.name = name;
//      result.elementCount = elements.size();
//      result.totalCombinations = std::pow(elements.size(), 9);
//      result.validMatrices = matrices.size();
//      result.successRate = (100.0 * matrices.size()) / result.totalCombinations;
//      result.timeMs = timeMs;
//
//      results.push_back(result);
//   }
//
//   // Print scaling analysis
//   std::cout << "\n=== SCALING ANALYSIS SUMMARY ===" << std::endl;
//   std::cout << std::setw(15) << "Range"
//      << std::setw(8) << "Elements"
//      << std::setw(12) << "Total"
//      << std::setw(10) << "Valid"
//      << std::setw(10) << "Rate%"
//      << std::setw(8) << "Time(ms)"
//      << std::setw(10) << "Rate/Time" << std::endl;
//   std::cout << std::string(75, '-') << std::endl;
//
//   for (const auto& r : results) {
//      double efficiency = r.successRate / r.timeMs;
//      std::cout << std::setw(15) << r.name
//         << std::setw(8) << r.elementCount
//         << std::setw(12) << r.totalCombinations
//         << std::setw(10) << r.validMatrices
//         << std::setw(9) << std::setprecision(4) << std::fixed << r.successRate << "%"
//         << std::setw(8) << r.timeMs
//         << std::setw(10) << std::setprecision(6) << efficiency << std::endl;
//   }
//}
//
//// Test optimal element selection
//void TestOptimalElementSelection() {
//   std::cout << "\n" << std::string(80, '=') << std::endl;
//   std::cout << "OPTIMAL ELEMENT SELECTION ANALYSIS" << std::endl;
//
//   // Based on the pattern that {-2,-1,0,1,2} has 94.9% matrices with max=2
//   // Let's test if we can get better results by targeting specific max elements
//
//   std::cout << "\nTesting element sets optimized for specific max elements..." << std::endl;
//
//   // Strategy 1: Bias toward elements that create max=2 matrices
//   std::vector<std::vector<double>> optimizedSets = {
//       {-2, -1, 0, 1, 2},                    // Baseline
//       {-2, -1, -1, 0, 1, 1, 2},            // Bias toward ±1
//       {-2, -2, -1, 0, 1, 2, 2},            // Bias toward ±2
//       {-3, -1, 0, 1, 3},                    // Skip ±2
//       {-2, -1, 0, 1, 2, 3},                // Add 3
//       {-3, -2, -1, 0, 1, 2},               // Add -3
//   };
//
//   for (size_t i = 0; i < optimizedSets.size(); ++i) {
//      const auto& elements = optimizedSets[i];
//
//      if (std::pow(elements.size(), 9) > 50000000) {
//         std::cout << "Skipping optimized set " << i << " - too large" << std::endl;
//         continue;
//      }
//
//      std::string name = "optimized_" + std::to_string(i);
//      auto matrices = TestGenerateSystematicWithFullAnalysis(elements, name);
//   }
//}
//
//int main() {
//   std::cout << "=== EXTENSIVE MATRIX GENERATION EXPLORATION ===" << std::endl;
//   std::cout << "Testing many more element combinations to understand patterns..." << std::endl;
//
//   // Run all test suites
//   TestRandomElementSets();
//   TestSparseElementSets();
//   AnalyzeScalingPatterns();
//   TestOptimalElementSelection();
//
//   std::cout << "\n" << std::string(80, '=') << std::endl;
//   std::cout << "FINAL RECOMMENDATIONS" << std::endl;
//   std::cout << "Based on all tests, the best element sets for matrix generation appear to be:" << std::endl;
//   std::cout << "1. {-2, -1, 0, 1, 2} - Good balance of size vs. matrix count" << std::endl;
//   std::cout << "2. Larger sets if computational time allows" << std::endl;
//   std::cout << "3. Sets that include ±2 elements tend to generate more effective matrices" << std::endl;
//   std::cout << "\nReady for comprehensive analysis with your chosen set!" << std::endl;
//
//   return 0;
//}