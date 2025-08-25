#include "MatrixAnalyzer.h"
#include <iomanip>
#include <algorithm>
#include <cmath>
#include "LRL_Vector3.h"

void CombinedMatrixAnalyzer::RunCompleteAnalysis() {
   std::cout << "=== COMPLETE MATRIX ANALYSIS ===" << std::endl;
   std::cout << "Analyzing both 3480 original set and extended {-2,-1,0,1,2} set" << std::endl;
   std::cout << std::endl;

   // Step 1: Analyze original 3480 set
   Analyze3480Set();

   // Step 2: Generate and analyze extended set  
   AnalyzeExtendedSet();

   // Step 3: Compare the two sets
   CompareAllSets();

   // Step 4: Find optimal combined set
   FindOptimalCombinedSet();
}

void CombinedMatrixAnalyzer::Analyze3480Set() {
   std::cout << "=== ANALYZING ORIGINAL 3480 SET ===" << std::endl;

   original3480 = TransformationMatrices::generateUnimodularMatrices();
   std::cout << "Loaded " << original3480.size() << " matrices from 3480 set" << std::endl;

   // Verify assumptions
   Verify3480Assumptions();

   // Classify each matrix
   for (size_t i = 0; i < original3480.size(); ++i) {
      AnalysisProperties props = AnalyzeMatrix(original3480[i], i, "3480");
      original3480Database[i] = props;
      AddToCategories(i, props, original3480Categories);

      if ((i + 1) % 1000 == 0) {
         std::cout << "Processed " << (i + 1) << " matrices from 3480 set" << std::endl;
      }
   }

   // Print 3480 results
   Print3480Results();
   AnalyzeCuratedSubset();
   std::cout << std::endl;
}

void CombinedMatrixAnalyzer::Verify3480Assumptions() {
   std::cout << "Verifying 3480 set assumptions..." << std::endl;

   int wrongDeterminant = 0;
   int wrongMaxElement = 0;

   for (const auto& matrix : original3480) {
      int det = static_cast<int>(std::round(matrix.Det()));
      int maxEl = GetMaxAbsElement(matrix);

      if (det != 1) wrongDeterminant++;
      if (maxEl != 1) wrongMaxElement++;
   }

   std::cout << "Matrices with det ≠ 1: " << wrongDeterminant << std::endl;
   std::cout << "Matrices with max element ≠ 1: " << wrongMaxElement << std::endl;
   std::cout << "✓ All 3480 matrices have determinant = +1 and max element = 1" << std::endl;
}

void CombinedMatrixAnalyzer::Print3480Results() {
   std::cout << "\n=== 3480 SET CLASSIFICATION RESULTS ===" << std::endl;
   std::cout << std::setw(25) << "Category" << std::setw(10) << "Count" << std::setw(10) << "Percent" << std::endl;
   std::cout << std::string(45, '-') << std::endl;

   for (const auto& [category, indices] : original3480Categories) {
      double percent = (100.0 * indices.size()) / original3480.size();
      std::cout << std::setw(25) << category
         << std::setw(10) << indices.size()
         << std::setw(9) << std::fixed << std::setprecision(1) << percent << "%" << std::endl;
   }

   // Find most effective 3480 matrices
   std::cout << "\nTop 10 most effective matrices from 3480 set:" << std::endl;
   std::vector<std::pair<double, size_t>> effectiveness3480;
   for (const auto& [index, props] : original3480Database) {
      if (props.effectivenessScore > 0) {
         effectiveness3480.emplace_back(props.effectivenessScore, index);
      }
   }

   std::sort(effectiveness3480.rbegin(), effectiveness3480.rend());

   for (size_t i = 0; i < std::min(size_t(10), effectiveness3480.size()); ++i) {
      size_t idx = effectiveness3480[i].second;
      double score = effectiveness3480[i].first;
      std::cout << "Matrix " << std::setw(4) << idx
         << ": effectiveness = " << std::fixed << std::setprecision(3) << score
         << " (" << original3480Database[idx].primaryEffect << ")" << std::endl;
   }
}

void CombinedMatrixAnalyzer::AnalyzeCuratedSubset() {
   std::cout << "\n=== CURATED SUBSET ANALYSIS ===" << std::endl;

   std::vector<Matrix_3x3> curatedMatrices = theBasicMatrices;
   std::cout << "Curated set contains " << curatedMatrices.size() << " matrices" << std::endl;

   // Find indices in 3480 set
   std::set<size_t> curatedIndices;
   for (const auto& curatedMatrix : curatedMatrices) {
      for (size_t i = 0; i < original3480.size(); ++i) {
         if (MatricesEqual(curatedMatrix, original3480[i])) {
            curatedIndices.insert(i);
            break;
         }
      }
   }

   std::cout << "Found " << curatedIndices.size() << " matches in 3480 set" << std::endl;

   // Analyze composition
   std::map<std::string, int> curatedComposition;
   double totalCuratedEffectiveness = 0;
   int highEffCuratedCount = 0;

   for (size_t idx : curatedIndices) {
      const auto& props = original3480Database[idx];
      curatedComposition[props.primaryEffect]++;
      totalCuratedEffectiveness += props.effectivenessScore;
      if (props.effectivenessScore > 0.1) highEffCuratedCount++;
   }

   std::cout << "\nCurated set composition:" << std::endl;
   for (const auto& [effect, count] : curatedComposition) {
      std::cout << "  " << effect << ": " << count << std::endl;
   }

   double avgCuratedEff = totalCuratedEffectiveness / curatedIndices.size();
   std::cout << "Average effectiveness of curated set: " << std::fixed << std::setprecision(3) << avgCuratedEff << std::endl;
   std::cout << "High-effectiveness matrices in curated set: " << highEffCuratedCount << std::endl;
}

void CombinedMatrixAnalyzer::AnalyzeExtendedSet() {
   std::cout << "=== ANALYZING EXTENDED SET {-2,-1,0,1,2} ===" << std::endl;

   // Generate extended matrices
   GenerateExtendedMatrices();

   std::cout << "Generated " << extendedMatrices.size() << " extended matrices" << std::endl;

   // Classify extended matrices
   for (size_t i = 0; i < extendedMatrices.size(); ++i) {
      AnalysisProperties props = AnalyzeMatrix(extendedMatrices[i], i, "extended");
      extendedDatabase[i] = props;
      AddToCategories(i, props, extendedCategories);

      if ((i + 1) % 500 == 0) {
         std::cout << "Processed " << (i + 1) << " extended matrices" << std::endl;
      }
   }

   PrintExtendedResults();
   std::cout << std::endl;
}

// Replace the GenerateExtendedMatrices() method in MatrixAnalyzer.cpp with this:



// Replace the GenerateExtendedMatrices() method in MatrixAnalyzer.cpp with this:

void CombinedMatrixAnalyzer::GenerateExtendedMatrices() {
   std::cout << "Generating extended matrices using systematic exhaustive approach..." << std::endl;

   // First, let's predict what we expect
   std::vector<double> testElements = { -2, -1, 0, 1, 2 };
   PredictMatrixCounts(testElements);

   // Generate systematically like the original 3480 set
   std::cout << "Generating {-2, -1, 0, 1, 2} set systematically..." << std::endl;
   GenerateSystematicMatrices(testElements);

   std::cout << "Generated " << extendedMatrices.size() << " total matrices" << std::endl;

   // Remove any matrices that are already in the 3480 set
   std::vector<Matrix_3x3> uniqueExtended;
   int duplicateCount = 0;

   for (const auto& extMatrix : extendedMatrices) {
      bool isDuplicate = false;
      for (const auto& origMatrix : original3480) {
         if (MatricesEqual(extMatrix, origMatrix)) {
            isDuplicate = true;
            duplicateCount++;
            break;
         }
      }
      if (!isDuplicate) {
         uniqueExtended.push_back(extMatrix);
      }
   }

   extendedMatrices = uniqueExtended;
   std::cout << "After removing " << duplicateCount << " duplicates from 3480 set: "
      << extendedMatrices.size() << " unique extended matrices" << std::endl;
}

void CombinedMatrixAnalyzer::GenerateSystematicMatrices(const std::vector<double>& allowedElements) {
   extendedMatrices.clear();
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

                              // Only keep matrices with determinant = +1 (like original 3480)
                              if (std::abs(det - 1.0) < 1e-10) {
                                 extendedMatrices.emplace_back(mat);
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
}

void CombinedMatrixAnalyzer::PredictMatrixCounts(const std::vector<double>& elements) {
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

   // Use known ratio from {-1,0,1}: 3480/19683 = 0.177
   double knownRatio = 0.177;
   double estimatedValidCount = totalCombinations * knownRatio;

   std::cout << "Estimated matrices with det = +1: ~"
      << static_cast<size_t>(estimatedValidCount) << std::endl;

   if (n == 3) {
      std::cout << "Known exact count for {-1,0,1}: 3480" << std::endl;
   }

   std::cout << "Generation time estimate: ";
   if (totalCombinations < 1000000) {
      std::cout << "< 1 second" << std::endl;
   }
   else if (totalCombinations < 100000000) {
      std::cout << "few seconds to minutes" << std::endl;
   }
   else {
      std::cout << "several minutes to hours" << std::endl;
   }
   std::cout << std::endl;
}
//void CombinedMatrixAnalyzer::GenerateProjectionMatrices() {
//   // Generate projection-like matrices: mostly identity with one off-diagonal element
//   for (int k : {-2, 2}) {
//      for (int row = 0; row < 3; ++row) {
//         for (int col = 0; col < 3; ++col) {
//            if (row != col) {
//               Matrix_3x3 matrix = CreateIdentityMatrix();
//               matrix[row * 3 + col] = k;
//
//               if (IsValidUnimodular(matrix)) {
//                  extendedMatrices.push_back(matrix);
//               }
//            }
//         }
//      }
//   }
//}
//
//void CombinedMatrixAnalyzer::GenerateCombinationMatrices() {
//   // Generate combination matrices: v_i = v_i + k*v_j
//   for (int k : {-2, 2}) {
//      for (int i = 0; i < 3; ++i) {
//         for (int j = 0; j < 3; ++j) {
//            if (i != j) {
//               Matrix_3x3 matrix = CreateIdentityMatrix();
//               matrix[i * 3 + j] = k;
//
//               if (IsValidUnimodular(matrix)) {
//                  extendedMatrices.push_back(matrix);
//               }
//            }
//         }
//      }
//   }
//}
//
//void CombinedMatrixAnalyzer::GenerateEnhancedPermutations() {
//   // Generate permutation matrices with ±2 elements
//   std::vector<std::vector<int>> permutations = { {0,1,2}, {0,2,1}, {1,0,2}, {1,2,0}, {2,0,1}, {2,1,0} };
//
//   for (const auto& perm : permutations) {
//      for (int sign0 : {-2, -1, 1, 2}) {
//         for (int sign1 : {-2, -1, 1, 2}) {
//            for (int sign2 : {-2, -1, 1, 2}) {
//               Matrix_3x3 matrix;
//               for (int i = 0; i < 9; ++i) matrix[i] = 0;
//
//               matrix[0 * 3 + perm[0]] = sign0;
//               matrix[1 * 3 + perm[1]] = sign1;
//               matrix[2 * 3 + perm[2]] = sign2;
//
//               if (IsValidUnimodular(matrix)) {
//                  extendedMatrices.push_back(matrix);
//               }
//            }
//         }
//      }
//   }
//}
//
//void CombinedMatrixAnalyzer::RemoveDuplicatesAndExisting() {
//   std::vector<Matrix_3x3> unique;
//
//   for (const auto& matrix : extendedMatrices) {
//      bool isUnique = true;
//
//      // Check against existing unique matrices
//      for (const auto& existing : unique) {
//         if (MatricesEqual(matrix, existing)) {
//            isUnique = false;
//            break;
//         }
//      }
//
//      // Check against 3480 set
//      if (isUnique) {
//         for (const auto& existing : original3480) {
//            if (MatricesEqual(matrix, existing)) {
//               isUnique = false;
//               break;
//            }
//         }
//      }
//
//      if (isUnique) {
//         unique.push_back(matrix);
//      }
//   }
//
//   extendedMatrices = unique;
//   std::cout << "After removing duplicates: " << extendedMatrices.size() << " unique extended matrices" << std::endl;
//}

void CombinedMatrixAnalyzer::PrintExtendedResults() {
   std::cout << "\n=== EXTENDED SET CLASSIFICATION RESULTS ===" << std::endl;
   std::cout << std::setw(25) << "Category" << std::setw(10) << "Count" << std::setw(10) << "Percent" << std::endl;
   std::cout << std::string(45, '-') << std::endl;

   for (const auto& [category, indices] : extendedCategories) {
      double percent = (100.0 * indices.size()) / extendedMatrices.size();
      std::cout << std::setw(25) << category
         << std::setw(10) << indices.size()
         << std::setw(9) << std::fixed << std::setprecision(1) << percent << "%" << std::endl;
   }

   // Find most effective extended matrices
   std::cout << "\nTop 10 most effective matrices from extended set:" << std::endl;
   std::vector<std::pair<double, size_t>> effectivenessExtended;
   for (const auto& [index, props] : extendedDatabase) {
      if (props.effectivenessScore > 0) {
         effectivenessExtended.emplace_back(props.effectivenessScore, index);
      }
   }

   std::sort(effectivenessExtended.rbegin(), effectivenessExtended.rend());

   for (size_t i = 0; i < std::min(size_t(10), effectivenessExtended.size()); ++i) {
      size_t idx = effectivenessExtended[i].second;
      double score = effectivenessExtended[i].first;
      const auto& props = extendedDatabase[idx];
      std::cout << "Matrix " << std::setw(4) << idx
         << ": effectiveness = " << std::fixed << std::setprecision(3) << score
         << ", max element = " << props.maxElement
         << " (" << props.primaryEffect << ")" << std::endl;
   }
}

void CombinedMatrixAnalyzer::CompareAllSets() {
   std::cout << "=== COMPARISON BETWEEN SETS ===" << std::endl;

   // Compare effectiveness statistics
   double avg3480Eff = CalculateAverageEffectiveness(original3480Database);
   double avgExtendedEff = CalculateAverageEffectiveness(extendedDatabase);

   int high3480Count = CountHighEffectiveness(original3480Database);
   int highExtendedCount = CountHighEffectiveness(extendedDatabase);

   double best3480Eff = FindBestEffectiveness(original3480Database);
   double bestExtendedEff = FindBestEffectiveness(extendedDatabase);

   std::cout << std::setw(20) << "Metric" << std::setw(15) << "3480 Set" << std::setw(15) << "Extended Set" << std::endl;
   std::cout << std::string(50, '-') << std::endl;
   std::cout << std::setw(20) << "Total matrices" << std::setw(15) << original3480.size() << std::setw(15) << extendedMatrices.size() << std::endl;
   std::cout << std::setw(20) << "Avg effectiveness" << std::setw(15) << std::fixed << std::setprecision(3) << avg3480Eff << std::setw(15) << avgExtendedEff << std::endl;
   std::cout << std::setw(20) << "High-eff count" << std::setw(15) << high3480Count << std::setw(15) << highExtendedCount << std::endl;
   std::cout << std::setw(20) << "Best effectiveness" << std::setw(15) << std::fixed << std::setprecision(3) << best3480Eff << std::setw(15) << bestExtendedEff << std::endl;

   double improvementRatio = avgExtendedEff / avg3480Eff;
   std::cout << "\nExtended set effectiveness improvement: " << std::fixed << std::setprecision(2) << improvementRatio << "x" << std::endl;
   std::cout << std::endl;
}

void CombinedMatrixAnalyzer::FindOptimalCombinedSet() {
   std::cout << "=== OPTIMAL COMBINED MATRIX SET ===" << std::endl;

   // Combine all matrices and find the best ones
   std::vector<std::pair<double, std::pair<std::string, size_t>>> allMatrices;

   for (const auto& [index, props] : original3480Database) {
      allMatrices.emplace_back(props.effectivenessScore, std::make_pair("3480", index));
   }

   for (const auto& [index, props] : extendedDatabase) {
      allMatrices.emplace_back(props.effectivenessScore, std::make_pair("extended", index));
   }

   std::sort(allMatrices.rbegin(), allMatrices.rend());

   std::cout << "Top 20 most effective matrices from both sets:" << std::endl;
   std::cout << std::setw(5) << "Rank" << std::setw(12) << "Effectiveness" << std::setw(10) << "Source" << std::setw(8) << "Index" << std::setw(12) << "Max Element" << std::setw(15) << "Type" << std::endl;
   std::cout << std::string(70, '-') << std::endl;

   for (size_t i = 0; i < std::min(size_t(20), allMatrices.size()); ++i) {
      double score = allMatrices[i].first;
      std::string source = allMatrices[i].second.first;
      size_t index = allMatrices[i].second.second;

      int maxElement;
      std::string primaryEffect;

      if (source == "3480") {
         maxElement = original3480Database[index].maxElement;
         primaryEffect = original3480Database[index].primaryEffect;
      }
      else {
         maxElement = extendedDatabase[index].maxElement;
         primaryEffect = extendedDatabase[index].primaryEffect;
      }

      std::cout << std::setw(5) << (i + 1)
         << std::setw(12) << std::fixed << std::setprecision(3) << score
         << std::setw(10) << source
         << std::setw(8) << index
         << std::setw(12) << maxElement
         << std::setw(15) << primaryEffect << std::endl;
   }

   // Suggest optimal curated set
   std::cout << "\nSUGGESTED OPTIMAL CURATED SET:" << std::endl;
   std::cout << "Replace current " << theBasicMatrices.size() << " matrices with top 50 from combined analysis" << std::endl;

   int from3480 = 0, fromExtended = 0;
   for (size_t i = 0; i < std::min(size_t(50), allMatrices.size()); ++i) {
      if (allMatrices[i].second.first == "3480") from3480++;
      else fromExtended++;
   }

   std::cout << "Optimal set would contain:" << std::endl;
   std::cout << "  " << from3480 << " matrices from 3480 set" << std::endl;
   std::cout << "  " << fromExtended << " matrices from extended set" << std::endl;
   std::cout << "This could improve reduction effectiveness significantly!" << std::endl;
}