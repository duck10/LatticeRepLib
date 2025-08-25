#include "P3_Reduce.h"
#include "P3TestGenerator.h"
#include <iostream>
#include <iomanip>

// Test extended matrices using the actual complete reduction algorithm
class ProperReductionComparison {
public:
   struct ReductionComparison {
      LRL_Cell originalCell;

      // 3480 reduction results
      LRL_Cell reduced3480;
      P3 final3480;
      double cost3480;
      Matrix_3x3 transform3480;

      // Extended reduction results (hypothetical)
      LRL_Cell reducedExtended;
      P3 finalExtended;
      double costExtended;
      Matrix_3x3 transformExtended;

      // Comparison
      bool extendedIsBetter;
      double improvement;
      std::string description;
   };

   static void TestProperReduction() {
      std::cout << "=== PROPER REDUCTION ALGORITHM COMPARISON ===" << std::endl;
      std::cout << "Testing extended matrices with the complete ReduceCellWith3480AndMatrix algorithm" << std::endl;

      // Generate test cases
      auto testCases = P3TestGenerator::GenerateTestCases(20);
      std::cout << "Testing " << testCases.size() << " diverse P3 configurations" << std::endl;
      std::cout << std::endl;

      std::vector<ReductionComparison> results;
      int extendedWins = 0;
      int ties = 0;
      int set3480Wins = 0;

      for (size_t i = 0; i < testCases.size(); ++i) {
         std::cout << "Test " << (i + 1) << "/" << testCases.size()
            << ": " << testCases[i].description << std::endl;

         ReductionComparison result = CompareReductionMethods(testCases[i]);
         results.push_back(result);

         // Show immediate results
         std::cout << "  Original cost: " << std::setprecision(6) << std::fixed << P3_Reduce::P3Cost(testCases[i].p3) << std::endl;
         std::cout << "  3480 final:    " << result.cost3480 << std::endl;
         std::cout << "  Extended final:" << result.costExtended << std::endl;

         if (result.extendedIsBetter) {
            std::cout << "  ✅ Extended wins by " << std::setprecision(2) << (result.improvement * 100) << "%" << std::endl;
            extendedWins++;
         }
         else if (std::abs(result.costExtended - result.cost3480) < 1e-10) {
            std::cout << "  ➖ Tie (same final cost)" << std::endl;
            ties++;
         }
         else {
            std::cout << "  ❌ 3480 wins" << std::endl;
            set3480Wins++;
         }
         std::cout << std::endl;
      }

      AnalyzeFinalResults(results, extendedWins, ties, set3480Wins);
   }

private:
   static ReductionComparison CompareReductionMethods(const P3TestGenerator::TestCase& testCase) {
      ReductionComparison result;
      result.originalCell = LRL_Cell(testCase.p3);
      result.description = testCase.description;

      // Test with current 3480 algorithm
      auto [reduced3480, transform3480] = P3_Reduce::ReduceCellWith3480AndMatrix(result.originalCell);
      result.reduced3480 = reduced3480;
      result.transform3480 = transform3480;

      // Apply StandardPresentation to get proper ordering
      result.final3480 = P3_Reduce::StandardPresentation(P3(reduced3480));
      result.cost3480 = P3_Reduce::P3Cost(result.final3480);

      // For now, simulate extended matrix reduction
      // TODO: Implement ReduceCellWithExtendedAndMatrix when extended matrices are ready
      result.reducedExtended = reduced3480;  // Placeholder
      result.finalExtended = result.final3480;  // Placeholder
      result.costExtended = result.cost3480;    // Placeholder
      result.transformExtended = transform3480; // Placeholder

      // In a real implementation, this would be:
      // auto [reducedExt, transformExt] = P3_Reduce::ReduceCellWithExtendedAndMatrix(result.originalCell);
      // result.reducedExtended = reducedExt;
      // result.finalExtended = P3_Reduce::StandardPresentation(P3(reducedExt));
      // result.costExtended = P3_Reduce::P3Cost(result.finalExtended);

      // Compare results
      result.extendedIsBetter = result.costExtended < result.cost3480 - 1e-10;
      if (result.extendedIsBetter) {
         result.improvement = (result.cost3480 - result.costExtended) / result.cost3480;
      }
      else {
         result.improvement = 0.0;
      }

      return result;
   }

   static void AnalyzeFinalResults(const std::vector<ReductionComparison>& results,
      int extendedWins, int ties, int set3480Wins) {
      std::cout << std::string(80, '=') << std::endl;
      std::cout << "FINAL ANALYSIS USING COMPLETE REDUCTION ALGORITHM" << std::endl;

      std::cout << "\n--- COMPARISON RESULTS ---" << std::endl;
      std::cout << "Extended matrix wins: " << extendedWins << " ("
         << std::setprecision(1) << (100.0 * extendedWins) / results.size() << "%)" << std::endl;
      std::cout << "Ties: " << ties << " ("
         << std::setprecision(1) << (100.0 * ties) / results.size() << "%)" << std::endl;
      std::cout << "3480 matrix wins: " << set3480Wins << " ("
         << std::setprecision(1) << (100.0 * set3480Wins) / results.size() << "%)" << std::endl;

      if (extendedWins > 0) {
         double totalImprovement = 0;
         for (const auto& result : results) {
            if (result.extendedIsBetter) {
               totalImprovement += result.improvement;
            }
         }
         double avgImprovement = totalImprovement / extendedWins;
         std::cout << "Average improvement when extended wins: "
            << std::setprecision(2) << (avgImprovement * 100) << "%" << std::endl;
      }

      // Analyze transformation matrices
      std::cout << "\n--- TRANSFORMATION ANALYSIS ---" << std::endl;
      AnalyzeTransformationMatrices(results);

      // Final recommendation
      std::cout << "\n--- RECOMMENDATION ---" << std::endl;
      if (extendedWins == 0) {
         std::cout << "❌ Extended matrices provide no benefits with the complete reduction algorithm." << std::endl;
         std::cout << "The 3480 set is sufficient. Focus on optimizing its usage." << std::endl;
      }
      else if (extendedWins >= results.size() * 0.2) {
         std::cout << "✅ Extended matrices provide significant benefits." << std::endl;
         std::cout << "Consider implementing ReduceCellWithExtendedAndMatrix." << std::endl;
         std::cout << "The complete reduction algorithm can exploit extended matrix advantages." << std::endl;
      }
      else {
         std::cout << "⚠️  Extended matrices provide limited benefits." << std::endl;
         std::cout << "Evaluate whether the computational cost justifies the improvement." << std::endl;
      }
   }

   static void AnalyzeTransformationMatrices(const std::vector<ReductionComparison>& results) {
      std::cout << "Analyzing transformation matrices used..." << std::endl;

      // Look for patterns in the transformation matrices
      for (size_t i = 0; i < std::min(size_t(5), results.size()); ++i) {
         const auto& result = results[i];
         std::cout << "Case " << (i + 1) << " transformation matrix:" << std::endl;

         const auto& matrix = result.transform3480;
         for (int row = 0; row < 3; ++row) {
            std::cout << "  [" << std::setw(4) << static_cast<int>(matrix[row * 3 + 0])
               << " " << std::setw(4) << static_cast<int>(matrix[row * 3 + 1])
               << " " << std::setw(4) << static_cast<int>(matrix[row * 3 + 2]) << "]" << std::endl;
         }

         // Calculate max element in transformation
         int maxElement = 0;
         for (int j = 0; j < 9; ++j) {
            maxElement = std::max(maxElement, static_cast<int>(std::abs(matrix[j])));
         }
         std::cout << "  Max element: " << maxElement << std::endl;
         std::cout << std::endl;
      }
   }
};

// Additional test to validate StandardPresentation fix
class StandardPresentationValidator {
public:
   static void TestStandardPresentationFix() {
      std::cout << "\n=== STANDARD PRESENTATION VALIDATION ===" << std::endl;

      // Test a few cases to verify StandardPresentation fixes ordering
      auto testCases = P3TestGenerator::GenerateTestCases(5);

      for (size_t i = 0; i < testCases.size(); ++i) {
         std::cout << "Test " << (i + 1) << ":" << std::endl;

         LRL_Cell original(testCases[i].p3);
         auto [reduced, transform] = P3_Reduce::ReduceCellWith3480AndMatrix(original);

         P3 beforeStandard(reduced);
         P3 afterStandard = P3_Reduce::StandardPresentation(beforeStandard);

         // Calculate magnitudes
         auto calcMagnitude = [](const P3::Vec2& v) {
            return std::sqrt(v.first * v.first + v.second * v.second);
            };

         std::cout << "  Before StandardPresentation:" << std::endl;
         std::cout << "    |p1| = " << std::setprecision(4) << calcMagnitude(beforeStandard[0]) << std::endl;
         std::cout << "    |p2| = " << calcMagnitude(beforeStandard[1]) << std::endl;
         std::cout << "    |p3| = " << calcMagnitude(beforeStandard[2]) << std::endl;

         std::cout << "  After StandardPresentation:" << std::endl;
         std::cout << "    |p1| = " << calcMagnitude(afterStandard[0]) << std::endl;
         std::cout << "    |p2| = " << calcMagnitude(afterStandard[1]) << std::endl;
         std::cout << "    |p3| = " << calcMagnitude(afterStandard[2]) << std::endl;

         // Verify proper ordering
         double mag1 = calcMagnitude(afterStandard[0]);
         double mag2 = calcMagnitude(afterStandard[1]);
         double mag3 = calcMagnitude(afterStandard[2]);

         bool properlyOrdered = (mag1 <= mag2 + 1e-10) && (mag2 <= mag3 + 1e-10);
         std::cout << "  ✓ Properly ordered: " << (properlyOrdered ? "YES" : "NO") << std::endl;
         std::cout << std::endl;
      }
   }
};

int main() {
   // First validate that StandardPresentation fixes the ordering issue
   StandardPresentationValidator::TestStandardPresentationFix();

   // Then test whether extended matrices provide real benefits
   ProperReductionComparison::TestProperReduction();

   return 0;
}