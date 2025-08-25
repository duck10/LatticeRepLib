//#include "MatrixAnalyzer.h"
//#include "P3TestGenerator.h"
//#include "TransformationMatrices.h"
//#include <iostream>
//#include <iomanip>
//#include <set>
//#include <algorithm>
//
//// Test iterative reduction to convergence using both matrix sets
//class IterativeReductionTester {
//public:
//   struct IterativeResult {
//      P3 originalP3;
//      double originalCost;
//
//      // 3480 iterative reduction
//      P3 final3480;
//      double final3480Cost;
//      int iterations3480;
//      std::vector<size_t> matrices3480Used;
//
//      // Extended iterative reduction
//      P3 finalExtended;
//      double finalExtendedCost;
//      int iterationsExtended;
//
//      // Comparison
//      bool extendedAchievesBetter;
//      double additionalReduction;
//      std::string description;
//   };
//
//   static void TestIterativeReduction() {
//      std::cout << "=== ITERATIVE REDUCTION TO CONVERGENCE TEST ===" << std::endl;
//      std::cout << "Testing full iterative reduction process with both matrix sets" << std::endl;
//
//      // Load matrix sets
//      auto matrices3480 = TransformationMatrices::generateUnimodularMatrices();
//      std::cout << "Loaded " << matrices3480.size() << " matrices from 3480 set" << std::endl;
//
//      // Generate full extended set (this is crucial for proper testing)
//      auto extendedMatrices = GenerateFullExtendedSet({ -2, -1, 0, 1, 2 });
//      std::cout << "Generated " << extendedMatrices.size() << " extended matrices" << std::endl;
//
//      // Remove duplicates between sets
//      auto uniqueExtended = RemoveDuplicatesFromExtended(extendedMatrices, matrices3480);
//      std::cout << "After removing duplicates: " << uniqueExtended.size() << " unique extended matrices" << std::endl;
//
//      // Test cases
//      auto testCases = P3TestGenerator::GenerateTestCases(20);
//      std::cout << "Testing iterative reduction on " << testCases.size() << " P3 configurations" << std::endl;
//      std::cout << std::endl;
//
//      std::vector<IterativeResult> results;
//      for (size_t i = 0; i < testCases.size(); ++i) {
//         std::cout << "Testing case " << (i + 1) << "/" << testCases.size()
//            << ": " << testCases[i].description << std::endl;
//
//         IterativeResult result = PerformIterativeReduction(testCases[i], matrices3480, uniqueExtended);
//         results.push_back(result);
//
//         // Show results
//         std::cout << "  Original cost: " << std::setprecision(4) << std::fixed << result.originalCost << std::endl;
//         std::cout << "  3480 final:    " << result.final3480Cost
//            << " after " << result.iterations3480 << " iterations ("
//            << std::setprecision(1) << ((result.originalCost - result.final3480Cost) / result.originalCost * 100)
//            << "% total reduction)" << std::endl;
//         std::cout << "  Extended final:" << result.finalExtendedCost
//            << " after " << result.iterationsExtended << " iterations ("
//            << std::setprecision(1) << ((result.originalCost - result.finalExtendedCost) / result.originalCost * 100)
//            << "% total reduction)" << std::endl;
//
//         if (result.extendedAchievesBetter) {
//            std::cout << "  ✅ Extended achieves " << std::setprecision(1)
//               << (result.additionalReduction * 100) << "% better final reduction" << std::endl;
//         }
//         else if (std::abs(result.finalExtendedCost - result.final3480Cost) < 1e-10) {
//            std::cout << "  ➖ Both achieve same final result" << std::endl;
//         }
//         else {
//            std::cout << "  ❌ 3480 achieves better final result" << std::endl;
//         }
//         std::cout << std::endl;
//      }
//
//      AnalyzeIterativeResults(results);
//   }
//
//private:
//   static std::vector<Matrix_3x3> GenerateFullExtendedSet(const std::vector<double>& elements) {
//      std::cout << "Generating full extended matrix set..." << std::endl;
//      std::vector<Matrix_3x3> matrices;
//      const size_t n = elements.size();
//
//      size_t count = 0;
//      for (size_t a00 = 0; a00 < n; a00++) {
//         if (a00 % (n / 5 + 1) == 0) {
//            std::cout << "Progress: " << std::setprecision(1) << std::fixed
//               << (100.0 * a00) / n << "%" << std::endl;
//         }
//
//         for (size_t a01 = 0; a01 < n; a01++) {
//            for (size_t a02 = 0; a02 < n; a02++) {
//               for (size_t a10 = 0; a10 < n; a10++) {
//                  for (size_t a11 = 0; a11 < n; a11++) {
//                     for (size_t a12 = 0; a12 < n; a12++) {
//                        for (size_t a20 = 0; a20 < n; a20++) {
//                           for (size_t a21 = 0; a21 < n; a21++) {
//                              for (size_t a22 = 0; a22 < n; a22++) {
//                                 Matrix_3x3 mat = {
//                                     elements[a00], elements[a01], elements[a02],
//                                     elements[a10], elements[a11], elements[a12],
//                                     elements[a20], elements[a21], elements[a22]
//                                 };
//
//                                 double det = mat.Det();
//                                 if (std::abs(det - 1.0) < 1e-10) {
//                                    matrices.push_back(mat);
//                                    count++;
//                                 }
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
//      std::cout << "Generated " << count << " total extended matrices" << std::endl;
//      return matrices;
//   }
//
//   static std::vector<Matrix_3x3> RemoveDuplicatesFromExtended(const std::vector<Matrix_3x3>& extended,
//      const std::vector<Matrix_3x3>& matrices3480) {
//      std::vector<Matrix_3x3> unique;
//      int duplicateCount = 0;
//
//      for (const auto& extMatrix : extended) {
//         bool isDuplicate = false;
//         for (const auto& origMatrix : matrices3480) {
//            if (MatricesEqual(extMatrix, origMatrix)) {
//               isDuplicate = true;
//               duplicateCount++;
//               break;
//            }
//         }
//         if (!isDuplicate) {
//            unique.push_back(extMatrix);
//         }
//      }
//
//      std::cout << "Removed " << duplicateCount << " duplicate matrices" << std::endl;
//      return unique;
//   }
//
//   static bool MatricesEqual(const Matrix_3x3& a, const Matrix_3x3& b) {
//      for (int i = 0; i < 9; ++i) {
//         if (std::abs(a[i] - b[i]) > 1e-10) return false;
//      }
//      return true;
//   }
//
//   static IterativeResult PerformIterativeReduction(const P3TestGenerator::TestCase& testCase,
//      const std::vector<Matrix_3x3>& matrices3480,
//      const std::vector<Matrix_3x3>& extendedMatrices) {
//      IterativeResult result;
//      result.originalP3 = testCase.p3;
//      result.originalCost = testCase.originalCost;
//      result.description = testCase.description;
//
//      // Iterative reduction with 3480 set
//      result.final3480 = result.originalP3;
//      result.final3480Cost = result.originalCost;
//      result.iterations3480 = 0;
//      result.matrices3480Used.clear();
//
//      bool improved3480 = true;
//      const int maxIterations = 100;  // Prevent infinite loops
//
//      while (improved3480 && result.iterations3480 < maxIterations) {
//         improved3480 = false;
//         double bestCost = result.final3480Cost;
//         P3 bestP3 = result.final3480;
//         size_t bestMatrixIndex = 0;
//
//         // Try all 3480 matrices on current state
//         for (size_t i = 0; i < matrices3480.size(); ++i) {
//            LRL_Cell current(result.final3480);
//            if (!current.IsValid()) continue;
//
//            LRL_Cell transformed = ApplyMatrixToCell(current, matrices3480[i]);
//            if (!transformed.IsValid()) continue;
//
//            P3 candidateP3(transformed);
//            double candidateCost = P3TestGenerator::CalculateP3Cost(candidateP3);
//
//            if (candidateCost < bestCost - 1e-10) {  // Significant improvement
//               bestCost = candidateCost;
//               bestP3 = candidateP3;
//               bestMatrixIndex = i;
//               improved3480 = true;
//            }
//         }
//
//         if (improved3480) {
//            result.final3480 = bestP3;
//            result.final3480Cost = bestCost;
//            result.matrices3480Used.push_back(bestMatrixIndex);
//            result.iterations3480++;
//         }
//      }
//
//      // Iterative reduction with extended set (3480 + unique extended)
//      std::vector<Matrix_3x3> combinedSet = matrices3480;
//      combinedSet.insert(combinedSet.end(), extendedMatrices.begin(), extendedMatrices.end());
//
//      result.finalExtended = result.originalP3;
//      result.finalExtendedCost = result.originalCost;
//      result.iterationsExtended = 0;
//
//      bool improvedExtended = true;
//
//      while (improvedExtended && result.iterationsExtended < maxIterations) {
//         improvedExtended = false;
//         double bestCost = result.finalExtendedCost;
//         P3 bestP3 = result.finalExtended;
//
//         // Try all combined matrices on current state
//         for (const auto& matrix : combinedSet) {
//            LRL_Cell current(result.finalExtended);
//            if (!current.IsValid()) continue;
//
//            LRL_Cell transformed = ApplyMatrixToCell(current, matrix);
//            if (!transformed.IsValid()) continue;
//
//            P3 candidateP3(transformed);
//            double candidateCost = P3TestGenerator::CalculateP3Cost(candidateP3);
//
//            if (candidateCost < bestCost - 1e-10) {  // Significant improvement
//               bestCost = candidateCost;
//               bestP3 = candidateP3;
//               improvedExtended = true;
//            }
//         }
//
//         if (improvedExtended) {
//            result.finalExtended = bestP3;
//            result.finalExtendedCost = bestCost;
//            result.iterationsExtended++;
//         }
//      }
//
//      // Compare final results
//      result.extendedAchievesBetter = result.finalExtendedCost < result.final3480Cost - 1e-10;
//      if (result.extendedAchievesBetter) {
//         result.additionalReduction = (result.final3480Cost - result.finalExtendedCost) / result.final3480Cost;
//      }
//      else {
//         result.additionalReduction = 0.0;
//      }
//
//      return result;
//   }
//
//   static void AnalyzeIterativeResults(const std::vector<IterativeResult>& results) {
//      std::cout << std::string(80, '=') << std::endl;
//      std::cout << "ITERATIVE REDUCTION ANALYSIS" << std::endl;
//
//      int extendedWins = 0;
//      int set3480Wins = 0;
//      int ties = 0;
//      double totalAdditionalReduction = 0;
//      double avg3480Iterations = 0;
//      double avgExtendedIterations = 0;
//
//      for (const auto& result : results) {
//         avg3480Iterations += result.iterations3480;
//         avgExtendedIterations += result.iterationsExtended;
//
//         if (result.extendedAchievesBetter) {
//            extendedWins++;
//            totalAdditionalReduction += result.additionalReduction;
//         }
//         else if (std::abs(result.finalExtendedCost - result.final3480Cost) < 1e-10) {
//            ties++;
//         }
//         else {
//            set3480Wins++;
//         }
//      }
//
//      avg3480Iterations /= results.size();
//      avgExtendedIterations /= results.size();
//
//      std::cout << "\n--- CONVERGENCE COMPARISON ---" << std::endl;
//      std::cout << "Extended set convergence wins: " << extendedWins << " ("
//         << std::setprecision(1) << (100.0 * extendedWins) / results.size() << "%)" << std::endl;
//      std::cout << "Ties (same final result): " << ties << " ("
//         << std::setprecision(1) << (100.0 * ties) / results.size() << "%)" << std::endl;
//      std::cout << "3480 set convergence wins: " << set3480Wins << " ("
//         << std::setprecision(1) << (100.0 * set3480Wins) / results.size() << "%)" << std::endl;
//
//      std::cout << "\n--- ITERATION ANALYSIS ---" << std::endl;
//      std::cout << "Average iterations (3480 set): " << std::setprecision(1) << avg3480Iterations << std::endl;
//      std::cout << "Average iterations (extended set): " << std::setprecision(1) << avgExtendedIterations << std::endl;
//
//      if (extendedWins > 0) {
//         std::cout << "Average additional reduction when extended wins: "
//            << std::setprecision(2) << (totalAdditionalReduction / extendedWins * 100) << "%" << std::endl;
//      }
//
//      std::cout << "\n--- FINAL RECOMMENDATION ---" << std::endl;
//      if (extendedWins == 0) {
//         std::cout << "❌ Extended matrices provide no convergence benefits." << std::endl;
//      }
//      else if (extendedWins >= results.size() * 0.2) {
//         std::cout << "✅ Extended matrices significantly improve final convergence." << std::endl;
//         std::cout << "The larger element set enables better iterative reduction." << std::endl;
//      }
//      else {
//         std::cout << "⚠️  Extended matrices provide limited convergence benefits." << std::endl;
//      }
//   }
//};
//
//int main() {
//   IterativeReductionTester::TestIterativeReduction();
//   return 0;
//}
