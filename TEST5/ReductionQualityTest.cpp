//#include "MatrixAnalyzer.h"
//#include "P3TestGenerator.h"
//#include "TransformationMatrices.h"
//#include <iostream>
//#include <iomanip>
//#include <set>
//#include <algorithm>
//
//// Comprehensive test showing detailed results for both matrix sets
//class ComprehensiveReductionTester {
//public:
//   struct DetailedResult {
//      P3 originalP3;
//      double originalCost;
//
//      // 3480 set results
//      P3 bestReduced3480;
//      double best3480Cost;
//      double improvement3480;
//      size_t best3480MatrixIndex;
//
//      // Extended set results
//      P3 bestReducedExtended;
//      double bestExtendedCost;
//      double improvementExtended;
//
//      // Comparison
//      bool extendedIsBetter;
//      double additionalImprovement;  // How much better extended is vs 3480
//      std::string description;
//      std::string testCaseType;
//   };
//
//   static void TestBothSets() {
//      std::cout << "=== COMPREHENSIVE REDUCTION COMPARISON ===" << std::endl;
//      std::cout << "Detailed analysis of both {-1,0,1} and {-2,-1,0,1,2} matrix sets" << std::endl;
//
//      // Load both matrix sets
//      auto matrices3480 = TransformationMatrices::generateUnimodularMatrices();
//      std::cout << "Loaded " << matrices3480.size() << " matrices from 3480 set" << std::endl;
//
//      auto extendedSample = GenerateExtendedSample({ -2, -1, 0, 1, 2 }, 2000);
//      std::cout << "Generated " << extendedSample.size() << " sample extended matrices" << std::endl;
//
//      // Generate test cases
//      auto testCases = P3TestGenerator::GenerateTestCases(30);
//      std::cout << "Testing " << testCases.size() << " P3 configurations" << std::endl;
//      std::cout << std::endl;
//
//      // Test each configuration
//      std::vector<DetailedResult> results;
//      for (size_t i = 0; i < testCases.size(); ++i) {
//         std::cout << "Testing case " << (i + 1) << "/" << testCases.size()
//            << ": " << testCases[i].description << std::endl;
//
//         DetailedResult result = TestBothSetsOnP3(testCases[i], matrices3480, extendedSample);
//         results.push_back(result);
//
//         // Show immediate results
//         std::cout << "  Original cost: " << std::setprecision(4) << std::fixed << result.originalCost << std::endl;
//         std::cout << "  3480 best:     " << result.best3480Cost
//            << " (" << std::setprecision(1) << (result.improvement3480 * 100) << "% improvement)" << std::endl;
//         std::cout << "  Extended best: " << result.bestExtendedCost
//            << " (" << std::setprecision(1) << (result.improvementExtended * 100) << "% improvement)" << std::endl;
//
//         if (result.extendedIsBetter) {
//            std::cout << "  ✅ Extended wins by " << std::setprecision(1)
//               << (result.additionalImprovement * 100) << "% additional reduction" << std::endl;
//         }
//         else if (std::abs(result.bestExtendedCost - result.best3480Cost) < 1e-10) {
//            std::cout << "  ➖ Tie (same result)" << std::endl;
//         }
//         else {
//            std::cout << "  ❌ 3480 set wins" << std::endl;
//         }
//         std::cout << std::endl;
//      }
//
//      // Comprehensive analysis
//      AnalyzeDetailedResults(results);
//   }
//
//private:
//   static std::vector<Matrix_3x3> GenerateExtendedSample(const std::vector<double>& elements, size_t maxSample) {
//      std::vector<Matrix_3x3> sample;
//      const size_t n = elements.size();
//      size_t totalCombinations = std::pow(n, 9);
//
//      std::cout << "Sampling " << maxSample << " matrices from " << totalCombinations << " combinations..." << std::endl;
//
//      // Sample more systematically
//      std::vector<size_t> indices;
//      for (size_t i = 0; i < totalCombinations; i += totalCombinations / maxSample) {
//         indices.push_back(i);
//      }
//
//      size_t count = 0;
//      size_t currentIndex = 0;
//
//      for (size_t a00 = 0; a00 < n && count < maxSample; a00++) {
//         for (size_t a01 = 0; a01 < n && count < maxSample; a01++) {
//            for (size_t a02 = 0; a02 < n && count < maxSample; a02++) {
//               for (size_t a10 = 0; a10 < n && count < maxSample; a10++) {
//                  for (size_t a11 = 0; a11 < n && count < maxSample; a11++) {
//                     for (size_t a12 = 0; a12 < n && count < maxSample; a12++) {
//                        for (size_t a20 = 0; a20 < n && count < maxSample; a20++) {
//                           for (size_t a21 = 0; a21 < n && count < maxSample; a21++) {
//                              for (size_t a22 = 0; a22 < n && count < maxSample; a22++) {
//                                 if (std::find(indices.begin(), indices.end(), currentIndex) != indices.end()) {
//                                    Matrix_3x3 mat = {
//                                        elements[a00], elements[a01], elements[a02],
//                                        elements[a10], elements[a11], elements[a12],
//                                        elements[a20], elements[a21], elements[a22]
//                                    };
//
//                                    double det = mat.Det();
//                                    if (std::abs(det - 1.0) < 1e-10) {
//                                       sample.push_back(mat);
//                                       count++;
//                                    }
//                                 }
//                                 currentIndex++;
//                              }
//                           }
//                        }
//                     }
//                  }
//               }
//            }
//         }
//      }
//
//      return sample;
//   }
//
//   static DetailedResult TestBothSetsOnP3(const P3TestGenerator::TestCase& testCase,
//      const std::vector<Matrix_3x3>& matrices3480,
//      const std::vector<Matrix_3x3>& extendedSample) {
//      DetailedResult result;
//      result.originalP3 = testCase.p3;
//      result.originalCost = testCase.originalCost;
//      result.description = testCase.description;
//      result.testCaseType = GetTestCaseTypeName(testCase.type);
//
//      // Initialize with original values
//      result.bestReduced3480 = result.originalP3;
//      result.best3480Cost = result.originalCost;
//      result.best3480MatrixIndex = 0;
//      result.bestReducedExtended = result.originalP3;
//      result.bestExtendedCost = result.originalCost;
//
//      // Test 3480 matrices
//      for (size_t i = 0; i < matrices3480.size(); ++i) {
//         const auto& matrix = matrices3480[i];
//         LRL_Cell original(testCase.p3);
//         if (!original.IsValid()) continue;
//
//         LRL_Cell transformed = ApplyMatrixToCell(original, matrix);
//         if (!transformed.IsValid()) continue;
//
//         P3 reducedP3(transformed);
//         double cost = P3TestGenerator::CalculateP3Cost(reducedP3);
//
//         if (cost < result.best3480Cost) {
//            result.best3480Cost = cost;
//            result.bestReduced3480 = reducedP3;
//            result.best3480MatrixIndex = i;
//         }
//      }
//
//      // Test extended matrices
//      for (const auto& matrix : extendedSample) {
//         LRL_Cell original(testCase.p3);
//         if (!original.IsValid()) continue;
//
//         LRL_Cell transformed = ApplyMatrixToCell(original, matrix);
//         if (!transformed.IsValid()) continue;
//
//         P3 reducedP3(transformed);
//         double cost = P3TestGenerator::CalculateP3Cost(reducedP3);
//
//         if (cost < result.bestExtendedCost) {
//            result.bestExtendedCost = cost;
//            result.bestReducedExtended = reducedP3;
//         }
//      }
//
//      // Calculate improvements and comparison
//      result.improvement3480 = (result.originalCost - result.best3480Cost) / result.originalCost;
//      result.improvementExtended = (result.originalCost - result.bestExtendedCost) / result.originalCost;
//
//      result.extendedIsBetter = result.bestExtendedCost < result.best3480Cost - 1e-10;
//      if (result.extendedIsBetter) {
//         result.additionalImprovement = (result.best3480Cost - result.bestExtendedCost) / result.best3480Cost;
//      }
//      else {
//         result.additionalImprovement = 0.0;
//      }
//
//      return result;
//   }
//
//   static std::string GetTestCaseTypeName(P3TestGenerator::TestCaseType type) {
//      switch (type) {
//      case P3TestGenerator::TestCaseType::RANDOM: return "Random";
//      case P3TestGenerator::TestCaseType::NEARLY_ORTHOGONAL: return "Nearly Orthogonal";
//      case P3TestGenerator::TestCaseType::NEARLY_PARALLEL: return "Nearly Parallel";
//      case P3TestGenerator::TestCaseType::GENERAL_CASE: return "General";
//      case P3TestGenerator::TestCaseType::HIGH_COST: return "High Cost";
//      case P3TestGenerator::TestCaseType::LOW_COST: return "Low Cost";
//      case P3TestGenerator::TestCaseType::EXTREME_RATIOS: return "Extreme Ratios";
//      default: return "Unknown";
//      }
//   }
//
//   static void AnalyzeDetailedResults(const std::vector<DetailedResult>& results) {
//      std::cout << "\n" << std::string(80, '=') << std::endl;
//      std::cout << "COMPREHENSIVE ANALYSIS" << std::endl;
//
//      // Overall statistics
//      int extendedWins = 0, ties = 0, set3480Wins = 0;
//      double total3480Improvement = 0, totalExtendedImprovement = 0;
//      double totalAdditionalImprovement = 0;
//
//      for (const auto& result : results) {
//         total3480Improvement += result.improvement3480;
//         totalExtendedImprovement += result.improvementExtended;
//
//         if (result.extendedIsBetter) {
//            extendedWins++;
//            totalAdditionalImprovement += result.additionalImprovement;
//         }
//         else if (std::abs(result.bestExtendedCost - result.best3480Cost) < 1e-10) {
//            ties++;
//         }
//         else {
//            set3480Wins++;
//         }
//      }
//
//      std::cout << "\n--- OVERALL PERFORMANCE ---" << std::endl;
//      std::cout << "3480 set average improvement: " << std::setprecision(2) << std::fixed
//         << (total3480Improvement / results.size()) * 100 << "%" << std::endl;
//      std::cout << "Extended set average improvement: " << std::setprecision(2) << std::fixed
//         << (totalExtendedImprovement / results.size()) * 100 << "%" << std::endl;
//
//      std::cout << "\n--- HEAD-TO-HEAD COMPARISON ---" << std::endl;
//      std::cout << "Extended set wins: " << extendedWins << " (" << std::setprecision(1)
//         << (100.0 * extendedWins) / results.size() << "%)" << std::endl;
//      std::cout << "Ties: " << ties << " (" << std::setprecision(1)
//         << (100.0 * ties) / results.size() << "%)" << std::endl;
//      std::cout << "3480 set wins: " << set3480Wins << " (" << std::setprecision(1)
//         << (100.0 * set3480Wins) / results.size() << "%)" << std::endl;
//
//      if (extendedWins > 0) {
//         std::cout << "Average additional improvement when extended wins: "
//            << std::setprecision(2) << (totalAdditionalImprovement / extendedWins) * 100 << "%" << std::endl;
//      }
//
//      // Analysis by test case type
//      std::cout << "\n--- PERFORMANCE BY TEST CASE TYPE ---" << std::endl;
//      std::map<std::string, std::vector<DetailedResult>> resultsByType;
//      for (const auto& result : results) {
//         resultsByType[result.testCaseType].push_back(result);
//      }
//
//      for (const auto& [type, typeResults] : resultsByType) {
//         int typeExtendedWins = 0;
//         double avgAdditionalImprovement = 0;
//
//         for (const auto& result : typeResults) {
//            if (result.extendedIsBetter) {
//               typeExtendedWins++;
//               avgAdditionalImprovement += result.additionalImprovement;
//            }
//         }
//
//         if (typeExtendedWins > 0) {
//            avgAdditionalImprovement /= typeExtendedWins;
//         }
//
//         std::cout << type << ": " << typeExtendedWins << "/" << typeResults.size()
//            << " extended wins";
//         if (typeExtendedWins > 0) {
//            std::cout << " (avg " << std::setprecision(1) << avgAdditionalImprovement * 100 << "% additional)";
//         }
//         std::cout << std::endl;
//      }
//
//      // Final recommendation
//      std::cout << "\n--- RECOMMENDATION ---" << std::endl;
//      if (extendedWins == 0) {
//         std::cout << "❌ The extended set provides no benefits over the 3480 set." << std::endl;
//         std::cout << "Stick with the proven 3480 matrices." << std::endl;
//      }
//      else if (extendedWins < results.size() * 0.15) {
//         std::cout << "⚠️  The extended set provides limited benefits (" << extendedWins << " wins)." << std::endl;
//         std::cout << "Consider if the computational cost is justified for your applications." << std::endl;
//      }
//      else {
//         std::cout << "✅ The extended set provides significant benefits." << std::endl;
//         std::cout << "Proceed with extended matrix analysis for improved reductions." << std::endl;
//      }
//   }
//};
//
//int main() {
//   ComprehensiveReductionTester::TestBothSets();
//   return 0;
//}