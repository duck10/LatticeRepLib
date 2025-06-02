#include "LatticeMatcher.h"
#include "InputHandler.h"
#include "MultiTransformFinderControls.h"
#include "ProductionLatticeMatcher.h"
#include "ProgramSetup.h"
#include "B4Matcher.h"
#include "NCDist.h"
#include "Niggli.h"
#include "G6.h"
#include <iostream>
#include <iomanip>

/**
 * Corrected test that works with your existing infrastructure
 * WITHOUT assuming specific B4Matcher interface
 */

 // Helper function to calculate NC distance
double calculateNCDistance(const LRL_Cell& cell1, const LRL_Cell& cell2) {
   return CalculateNCDistWithReduction(cell1, cell2);
}

// Helper function to try Niggli reduction comparison
bool tryNiggliMatch(const LRL_Cell& mobile, const LRL_Cell& reference, double& p3Distance, double& ncDistance) {
   G6 mobileG6(mobile);
   G6 referenceG6(reference);

   G6 mobileReduced, referenceReduced;
   const bool mobileSuccess = Niggli::Reduce(mobileG6, mobileReduced);
   const bool referenceSuccess = Niggli::Reduce(referenceG6, referenceReduced);

   if (!mobileSuccess || !referenceSuccess) {
      return false;
   }

   const LRL_Cell mobileReducedCell(mobileReduced);
   const LRL_Cell referenceReducedCell(referenceReduced);

   p3Distance = calculateP3Distance(mobileReducedCell, referenceReducedCell);
   ncDistance = calculateNCDistance(mobileReducedCell, referenceReducedCell);

   return true;
}

// Extended ProductionLatticeMatcher testing class
class ExtendedProductionTesting {
private:
   const double m_p3Threshold = 0.1;
   const double m_ncThreshold = 0.01;

public:
   struct ExtendedResult {
      std::string methodName;
      bool success;
      double finalP3Distance;
      double finalNCDistance;
      int stepsUsed;
      Matrix_3x3 transformationMatrix;
      LRL_Cell transformedMobile;
      std::string details;

      ExtendedResult()
         : methodName("UNINITIALIZED")
         , success(false)
         , finalP3Distance(19191.0)
         , finalNCDistance(19191.0)
         , stepsUsed(19191)
         , transformationMatrix()
         , transformedMobile()
         , details("") {
      }

      friend std::ostream& operator<<(std::ostream& os, const ExtendedResult& result) {
         os << result.methodName << ": "
            << (result.success ? "SUCCESS" : "FAILED")
            << " | P3=" << std::fixed << std::setprecision(6) << result.finalP3Distance
            << " | NC=" << std::fixed << std::setprecision(6) << result.finalNCDistance
            << " | Steps=" << result.stepsUsed;
         if (!result.details.empty()) {
            os << " | " << result.details;
         }
         return os;
      }
   };

   // Method 1: Standard ProductionLatticeMatcher
   ExtendedResult tryStandardProduction(const LRL_Cell& mobile, const LRL_Cell& reference) const {
      ExtendedResult result;
      result.methodName = "Standard_Production";

      ProductionLatticeMatcher::MatchingOptions options;
      options.maxIterations = 50;
      options.convergenceThreshold = 1e-10;
      options.showProgress = false;

      ProductionLatticeMatcher matcher(options);
      const ProductionLatticeMatcher::MatchResult matchResult = matcher.findMatchingPath(mobile, reference);

      result.finalP3Distance = matchResult.finalP3Distance;
      result.stepsUsed = matchResult.stepsUsed;
      result.transformationMatrix = matchResult.cumulativeTransformation;

      if (matchResult.stepsUsed > 0) {
         result.transformedMobile = applyMatrixToCell(mobile, matchResult.cumulativeTransformation);
         result.finalNCDistance = calculateNCDistance(result.transformedMobile, reference);
         result.success = (result.finalP3Distance < m_p3Threshold) && (result.finalNCDistance < m_ncThreshold);
      }

      result.details = "Improvement: " + std::to_string(matchResult.getImprovementPercent()) + "%";
      return result;
   }

   // Method 2: Aggressive ProductionLatticeMatcher
   ExtendedResult tryAggressiveProduction(const LRL_Cell& mobile, const LRL_Cell& reference) const {
      ExtendedResult result;
      result.methodName = "Aggressive_Production";

      ProductionLatticeMatcher::MatchingOptions options;
      options.maxIterations = 200;
      options.convergenceThreshold = 1e-12;
      options.showProgress = false;

      ProductionLatticeMatcher matcher(options);
      const ProductionLatticeMatcher::MatchResult matchResult = matcher.findMatchingPath(mobile, reference);

      result.finalP3Distance = matchResult.finalP3Distance;
      result.stepsUsed = matchResult.stepsUsed;
      result.transformationMatrix = matchResult.cumulativeTransformation;

      if (matchResult.stepsUsed > 0) {
         result.transformedMobile = applyMatrixToCell(mobile, matchResult.cumulativeTransformation);
         result.finalNCDistance = calculateNCDistance(result.transformedMobile, reference);
         result.success = (result.finalP3Distance < m_p3Threshold) && (result.finalNCDistance < m_ncThreshold);
      }

      result.details = "Aggressive: " + std::to_string(matchResult.getImprovementPercent()) + "%";
      return result;
   }

   // Method 3: Niggli-first then ProductionLatticeMatcher
   ExtendedResult tryNiggliThenProduction(const LRL_Cell& mobile, const LRL_Cell& reference) const {
      ExtendedResult result;
      result.methodName = "Niggli_Then_Production";

      // First, reduce both cells
      G6 mobileG6(mobile);
      G6 referenceG6(reference);

      G6 mobileReduced, referenceReduced;
      const bool mobileSuccess = Niggli::Reduce(mobileG6, mobileReduced);
      const bool referenceSuccess = Niggli::Reduce(referenceG6, referenceReduced);

      if (!mobileSuccess || !referenceSuccess) {
         result.details = "Niggli reduction failed";
         return result;
      }

      const LRL_Cell mobileReducedCell(mobileReduced);
      const LRL_Cell referenceReducedCell(referenceReduced);

      // Then apply ProductionLatticeMatcher to reduced cells
      ProductionLatticeMatcher::MatchingOptions options;
      options.maxIterations = 100;
      options.convergenceThreshold = 1e-10;
      options.showProgress = false;

      ProductionLatticeMatcher matcher(options);
      const ProductionLatticeMatcher::MatchResult matchResult = matcher.findMatchingPath(mobileReducedCell, referenceReducedCell);

      result.finalP3Distance = matchResult.finalP3Distance;
      result.stepsUsed = matchResult.stepsUsed;
      result.transformationMatrix = matchResult.cumulativeTransformation;

      if (matchResult.stepsUsed > 0) {
         result.transformedMobile = applyMatrixToCell(mobileReducedCell, matchResult.cumulativeTransformation);
         result.finalNCDistance = calculateNCDistance(result.transformedMobile, referenceReducedCell);
         result.success = (result.finalP3Distance < m_p3Threshold) && (result.finalNCDistance < m_ncThreshold);
      }

      result.details = "Niggli+Prod: " + std::to_string(matchResult.getImprovementPercent()) + "%";
      return result;
   }

   // Method 4: Relaxed threshold ProductionLatticeMatcher
   ExtendedResult tryRelaxedProduction(const LRL_Cell& mobile, const LRL_Cell& reference) const {
      ExtendedResult result;
      result.methodName = "Relaxed_Production";

      ProductionLatticeMatcher::MatchingOptions options;
      options.maxIterations = 300;
      options.convergenceThreshold = 1e-6;  // Much looser convergence
      options.showProgress = false;

      ProductionLatticeMatcher matcher(options);
      const ProductionLatticeMatcher::MatchResult matchResult = matcher.findMatchingPath(mobile, reference);

      result.finalP3Distance = matchResult.finalP3Distance;
      result.stepsUsed = matchResult.stepsUsed;
      result.transformationMatrix = matchResult.cumulativeTransformation;

      if (matchResult.stepsUsed > 0) {
         result.transformedMobile = applyMatrixToCell(mobile, matchResult.cumulativeTransformation);
         result.finalNCDistance = calculateNCDistance(result.transformedMobile, reference);
         result.success = (result.finalP3Distance < m_p3Threshold) && (result.finalNCDistance < m_ncThreshold);
      }

      result.details = "Relaxed: " + std::to_string(matchResult.getImprovementPercent()) + "%";
      return result;
   }

   // Method 5: Direct comparison baseline
   ExtendedResult tryDirectComparison(const LRL_Cell& mobile, const LRL_Cell& reference) const {
      ExtendedResult result;
      result.methodName = "Direct_Comparison";

      result.finalP3Distance = calculateP3Distance(mobile, reference);
      result.finalNCDistance = calculateNCDistance(mobile, reference);
      result.stepsUsed = 0;
      result.transformationMatrix.UnitMatrix();
      result.transformedMobile = mobile;

      result.success = (result.finalP3Distance < m_p3Threshold) && (result.finalNCDistance < m_ncThreshold);
      result.details = "No transformation";

      return result;
   }

   // Main interface
   std::vector<ExtendedResult> tryAllExtendedMethods(const LRL_Cell& mobile, const LRL_Cell& reference) const {
      std::cout << "Testing extended ProductionLatticeMatcher approaches..." << std::endl;

      std::vector<ExtendedResult> results;

      results.push_back(tryDirectComparison(mobile, reference));
      results.push_back(tryStandardProduction(mobile, reference));
      results.push_back(tryAggressiveProduction(mobile, reference));
      results.push_back(tryNiggliThenProduction(mobile, reference));
      results.push_back(tryRelaxedProduction(mobile, reference));

      return results;
   }

   ExtendedResult findBestExtendedMethod(const LRL_Cell& mobile, const LRL_Cell& reference) const {
      const std::vector<ExtendedResult> results = tryAllExtendedMethods(mobile, reference);

      // Find the best successful result
      ExtendedResult bestResult;
      bestResult.finalP3Distance = 19191.0;

      for (const auto& result : results) {
         if (result.success && result.finalP3Distance < bestResult.finalP3Distance) {
            bestResult = result;
         }
      }

      // If no success, return the best attempt
      if (!bestResult.success) {
         for (const auto& result : results) {
            if (result.finalP3Distance < bestResult.finalP3Distance) {
               bestResult = result;
            }
         }
      }

      return bestResult;
   }
};

void testExtendedApproaches(const LRL_Cell& mobile, const LRL_Cell& reference) {
   std::cout << "\n" << std::string(70, '=') << std::endl;
   std::cout << "EXTENDED PRODUCTION LATTICE MATCHER TESTING" << std::endl;
   std::cout << std::string(70, '=') << std::endl;

   std::cout << "Mobile:    " << LRL_Cell_Degrees(mobile) << std::endl;
   std::cout << "Reference: " << LRL_Cell_Degrees(reference) << std::endl;

   const double initialP3 = calculateP3Distance(mobile, reference);
   const double initialNC = calculateNCDistance(mobile, reference);

   std::cout << "\nInitial distances:" << std::endl;
   std::cout << "  P3: " << std::fixed << std::setprecision(6) << initialP3 << std::endl;
   std::cout << "  NC: " << std::fixed << std::setprecision(6) << initialNC << std::endl;

   ExtendedProductionTesting extendedTester;
   const std::vector<ExtendedProductionTesting::ExtendedResult> results = extendedTester.tryAllExtendedMethods(mobile, reference);

   std::cout << "\nExtended method results:" << std::endl;
   for (const auto& result : results) {
      std::cout << "  " << result << std::endl;
   }

   // Find and display best result
   const ExtendedProductionTesting::ExtendedResult bestResult = extendedTester.findBestExtendedMethod(mobile, reference);

   std::cout << "\n" << std::string(50, '-') << std::endl;
   std::cout << "BEST RESULT: " << bestResult << std::endl;

   if (bestResult.success) {
      std::cout << "SUCCESS! Found transformation." << std::endl;
      std::cout << "Transformed mobile: " << LRL_Cell_Degrees(bestResult.transformedMobile) << std::endl;
      std::cout << "Transformation matrix:" << std::endl;
      std::cout << bestResult.transformationMatrix << std::endl;
   }
   else {
      std::cout << "All extended methods failed to find satisfactory match." << std::endl;
      std::cout << "Best P3 distance achieved: " << bestResult.finalP3Distance << std::endl;
      std::cout << "Gap to threshold: " << (bestResult.finalP3Distance - 0.1) << std::endl;

      // Analysis
      if (bestResult.finalP3Distance < 1.0) {
         std::cout << "ANALYSIS: Very close to solution. Consider:" << std::endl;
         std::cout << "  - Further relaxing convergence criteria" << std::endl;
         std::cout << "  - Manual matrix refinement from best position" << std::endl;
         std::cout << "  - B4Matcher hand-off (if available)" << std::endl;
      }
      else if (bestResult.finalP3Distance < 5.0) {
         std::cout << "ANALYSIS: Significant improvement achieved. Consider:" << std::endl;
         std::cout << "  - Different starting transformations" << std::endl;
         std::cout << "  - Hybrid approaches" << std::endl;
      }
      else {
         std::cout << "ANALYSIS: Limited improvement. Consider:" << std::endl;
         std::cout << "  - Alternative algorithms" << std::endl;
         std::cout << "  - Different lattice representations" << std::endl;
      }
   }
}

int main() {
   std::cout << "=== CORRECTED LATTICE MATCHING TEST ===" << std::endl;
   std::cout << "Extended ProductionLatticeMatcher testing (no B4Matcher dependency)" << std::endl;

   MultiTransformFinderControls controls;
   ProgramSetup<MultiTransformFinderControls> program_setup("CorrectedTest", controls);
   const std::vector<LatticeCell>& inputList = program_setup.getInputList();

   if (inputList.size() < 2) {
      std::cout << "Testing with the problematic case that gets stuck at P3=4.67..." << std::endl;

      const LRL_Cell mobile(9.023, 9.028, 9.029,
         60.018 * M_PI / 180, 60.022 * M_PI / 180, 60.018 * M_PI / 180);
      const LRL_Cell reference(9.030, 9.030, 9.030,
         120.000 * M_PI / 180, 120.000 * M_PI / 180, 90.000 * M_PI / 180);

      testExtendedApproaches(mobile, reference);
   }
   else {
      testExtendedApproaches(inputList[0].getCell(), inputList[1].getCell());

      // Test additional pairs if available
      if (inputList.size() > 2) {
         for (size_t i = 2; i < inputList.size(); ++i) {
            testExtendedApproaches(inputList[0].getCell(), inputList[i].getCell());
         }
      }
   }

   std::cout << "\n=== TEST COMPLETE ===" << std::endl;
   return 0;
}