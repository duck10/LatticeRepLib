// UnifiedLatticeMatcher.cpp
// Production lattice matching program with multiple algorithms
// Follows Swedish design methodology with const correctness

#ifndef UNIFIED_LATTICE_MATCHER_H
#define UNIFIED_LATTICE_MATCHER_H

#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "Matrix_3x3.h"
#include "B4.h"
#include "P3.h"
#include "NCDist.h"
#include "Niggli.h"
#include "G6.h"
#include "TransformationMatrices.h"
#include "MultiTransformFinderControls.h"
#include "BasicProgramInput.h"
#include "InputHandler.h"
#include "TransformerUtilities.h"
#include "B4Matcher.h"
#include "ProductionLatticeMatcher.h"
#include "BidirectionalMatrixMatcher.h"
#include "ComparisonStats.h"
#include "UnifiedMatchResult.h"


#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

/**
 * Production lattice matcher implementing all algorithms in systematic order
 */
class UnifiedLatticeMatcher {
public:
   explicit UnifiedLatticeMatcher(const MultiTransformFinderControls& controls)
      : m_controls(controls) {
   }

   /**
    * Main matching function - tries algorithms in systematic order
    */
   UnifiedMatchResult findBestMatch(const LRL_Cell& mobileCell, const LRL_Cell& referenceCell) const {
      if (m_controls.shouldShowDetails()) {
         std::cout << "\n=== UNIFIED LATTICE MATCHING ===" << std::endl;
         std::cout << "Mobile:    " << LRL_Cell_Degrees(mobileCell) << std::endl;
         std::cout << "Reference: " << LRL_Cell_Degrees(referenceCell) << std::endl;
      }

      // Try algorithms in order of efficiency/likelihood of success

      // 1. Direct comparison
      UnifiedMatchResult result = tryDirectComparison(mobileCell, referenceCell);
      if (result.success) return result;

      // 2. Niggli-reduced comparison  
      result = tryNiggliComparison(mobileCell, referenceCell);
      if (result.success) return result;

      // 3. B4Matcher (user controls whether Niggli is used)
      result = tryB4MatcherDirect(mobileCell, referenceCell);
      if (result.success) return result;

      // 4. B4Matcher with full pipeline (centering + Niggli + lattice matching + permutations)
      result = tryB4MatcherWithPipeline(mobileCell, referenceCell);
      if (result.success) return result;

      // 5. Progressive guided cloud (single direction)
      result = tryProgressiveGuidedCloud(mobileCell, referenceCell);
      if (result.success) return result;

      // 6. Progressive guided cloud (bidirectional)
      result = tryBidirectionalGuidedCloud(mobileCell, referenceCell);
      if (result.success) return result;

      // 7. Hybrid approach: guided cloud + B4Matcher
      result = tryHybridApproach(mobileCell, referenceCell);

      return result;
   }

private:
   const MultiTransformFinderControls& m_controls;

   // Helper functions for distance calculations
   double calculateP3Distance(const LRL_Cell& cell1, const LRL_Cell& cell2) const {
      const P3 p3_1(cell1);
      const P3 p3_2(cell2);
      return (p3_1 - p3_2).norm();
   }

   bool isGoodMatch(const double p3Distance, const double ncDistance) const {
      return (p3Distance < m_controls.getSuccessThreshold() &&
         ncDistance < m_controls.getSuccessThreshold());
   }

   /**
    * Algorithm 1: Direct comparison
    */
   UnifiedMatchResult tryDirectComparison(const LRL_Cell& mobileCell,
      const LRL_Cell& referenceCell) const {
      UnifiedMatchResult result;
      result.algorithmUsed = "Direct Comparison";
      result.algorithmStep = 1;
      result.iterationsUsed = 0;

      result.p3Distance = calculateP3Distance(mobileCell, referenceCell);
      result.ncDistance = CalculateNCDistWithReduction(mobileCell, referenceCell);

      if (m_controls.shouldShowDetails()) {
         std::cout << "\n--- Algorithm 1: Direct Comparison ---" << std::endl;
         std::cout << "P3 Distance: " << result.p3Distance << std::endl;
         std::cout << "NC Distance: " << result.ncDistance << std::endl;
      }

      if (isGoodMatch(result.p3Distance, result.ncDistance)) {
         result.success = true;
         result.transformationMatrix.UnitMatrix();
         result.transformedMobileCell = mobileCell;
         result.notes = "Cells already match directly";
      }
      else {
         result.notes = "Direct comparison insufficient";
      }

      return result;
   }

   /**
    * Algorithm 2: Niggli-reduced comparison
    */
   UnifiedMatchResult tryNiggliComparison(const LRL_Cell& mobileCell,
      const LRL_Cell& referenceCell) const {
      UnifiedMatchResult result;
      result.algorithmUsed = "Niggli Comparison";
      result.algorithmStep = 2;
      result.iterationsUsed = 0;

      if (m_controls.shouldShowDetails()) {
         std::cout << "\n--- Algorithm 2: Niggli Comparison ---" << std::endl;
      }

      try {
         // Reduce both cells to Niggli form
         G6 mobileG6(mobileCell);
         G6 referenceG6(referenceCell);
         G6 mobileReduced, referenceReduced;

         const bool mobileSuccess = Niggli::Reduce(mobileG6, mobileReduced);
         const bool referenceSuccess = Niggli::Reduce(referenceG6, referenceReduced);

         if (!mobileSuccess || !referenceSuccess) {
            result.notes = "Niggli reduction failed";
            return result;
         }

         const LRL_Cell mobileReducedCell(mobileReduced);
         const LRL_Cell referenceReducedCell(referenceReduced);

         result.p3Distance = calculateP3Distance(mobileReducedCell, referenceReducedCell);
         result.ncDistance = CalculateNCDistWithReduction(mobileReducedCell, referenceReducedCell);

         if (m_controls.shouldShowDetails()) {
            std::cout << "Mobile Niggli:    " << LRL_Cell_Degrees(mobileReducedCell) << std::endl;
            std::cout << "Reference Niggli: " << LRL_Cell_Degrees(referenceReducedCell) << std::endl;
            std::cout << "P3 Distance: " << result.p3Distance << std::endl;
            std::cout << "NC Distance: " << result.ncDistance << std::endl;
         }

         if (isGoodMatch(result.p3Distance, result.ncDistance)) {
            result.success = true;
            result.transformationMatrix.UnitMatrix(); // Simple case - just Niggli reduction
            result.transformedMobileCell = mobileReducedCell;
            result.notes = "Match found with Niggli reduction";
         }
         else {
            result.notes = "Niggli comparison insufficient";
         }

      }
      catch (const std::exception& e) {
         result.notes = std::string("Niggli comparison failed: ") + e.what();
      }

      return result;
   }

   /**
    * Algorithm 3: B4Matcher direct (uses controls as configured by user)
    */
   UnifiedMatchResult tryB4MatcherDirect(const LRL_Cell& mobileCell,
      const LRL_Cell& referenceCell) const {
      UnifiedMatchResult result;
      result.algorithmUsed = "B4Matcher Direct";
      result.algorithmStep = 3;

      if (m_controls.shouldShowDetails()) {
         std::cout << "\n--- Algorithm 3: B4Matcher Direct ---" << std::endl;
      }

      try {
         B4Matcher matcher(mobileCell, referenceCell, m_controls);
         matcher.FindTransformationsWithReservoir();

         if (matcher.size() > 0) {
            const auto bestTransform = matcher.findBestTransformation();

            result.p3Distance = bestTransform.p3DistanceFinal;
            result.ncDistance = bestTransform.cs6DistanceFinal;
            result.iterationsUsed = static_cast<int>(matcher.size());

            if (m_controls.shouldShowDetails()) {
               std::cout << "Best P3 Distance: " << result.p3Distance << std::endl;
               std::cout << "Transformations found: " << matcher.size() << std::endl;
            }

            if (isGoodMatch(result.p3Distance, result.ncDistance)) {
               result.success = true;
               result.transformationMatrix = bestTransform.transformMatrix;
               result.transformedMobileCell = bestTransform.transformedCompleteCell;
               result.notes = "B4Matcher direct success";
            }
            else {
               result.notes = "B4Matcher direct insufficient";
            }
         }
         else {
            result.notes = "B4Matcher direct found no transformations";
         }

      }
      catch (const std::exception& e) {
         result.notes = std::string("B4Matcher direct failed: ") + e.what();
      }

      return result;
   }

   /**
    * Algorithm 4: B4Matcher with full pipeline
    * Complete matrix: M_1R^-1 * [P] * M_MR * M_2M * M_1M
    */
   UnifiedMatchResult tryB4MatcherWithPipeline(const LRL_Cell& mobileCell,
      const LRL_Cell& referenceCell) const {
      UnifiedMatchResult result;
      result.algorithmUsed = "B4Matcher Pipeline";
      result.algorithmStep = 4;

      if (m_controls.shouldShowDetails()) {
         std::cout << "\n--- Algorithm 4: B4Matcher with Full Pipeline ---" << std::endl;
      }

      try {
         // Step 1: Transform to primitive (M_1R and M_1M)
         const Matrix_3x3 M_1R = ToPrimitive("P", referenceCell); // Assume primitive for now
         const Matrix_3x3 M_1M = ToPrimitive("P", mobileCell);

         const LRL_Cell referencePrimitive = M_1R * referenceCell;
         const LRL_Cell mobilePrimitive = M_1M * mobileCell;

         // Step 2: Niggli reduce both (M_2R and M_2M)
         const Matrix_3x3 M_2R = TransformerUtilities::calculateNiggliMatrix(referencePrimitive, m_controls.shouldShowDetails());
         const Matrix_3x3 M_2M = TransformerUtilities::calculateNiggliMatrix(mobilePrimitive, m_controls.shouldShowDetails());

         const LRL_Cell referenceNiggli = M_2R * referencePrimitive;
         const LRL_Cell mobileNiggli = M_2M * mobilePrimitive;

         // Step 3: Lattice matching (M_MR)
         B4Matcher matcher(mobileNiggli, referenceNiggli, m_controls);
         matcher.FindTransformationsWithReservoir();

         Matrix_3x3 M_MR;
         M_MR.UnitMatrix();

         if (matcher.size() > 0) {
            const auto bestMatch = matcher.findBestTransformation();
            M_MR = bestMatch.transformMatrix;

            if (m_controls.shouldShowDetails()) {
               std::cout << "Lattice matching found " << matcher.size() << " candidates" << std::endl;
               std::cout << "Best lattice match P3: " << bestMatch.p3DistanceFinal << std::endl;
            }
         }

         // Step 4: Try all permutation matrices [P]
         const std::vector<Matrix_3x3> permutations = MatrixSets::getAllPermutationMatrices();

         double bestP3 = 19191.0;
         Matrix_3x3 bestCompleteMatrix;
         bestCompleteMatrix.UnitMatrix();

         for (const auto& P : permutations) {
            // Compose complete matrix: M_1R^-1 * [P] * M_MR * M_2M * M_1M
            const Matrix_3x3 completeMatrix = M_1R.Inverse() * P * M_MR * M_2M * M_1M;

            // Test this transformation
            const LRL_Cell transformedCell = completeMatrix * mobileCell;
            const double p3Dist = calculateP3Distance(transformedCell, referenceCell);

            if (p3Dist < bestP3) {
               bestP3 = p3Dist;
               bestCompleteMatrix = completeMatrix;
            }
         }

         result.p3Distance = bestP3;
         result.ncDistance = CalculateNCDistWithReduction(bestCompleteMatrix * mobileCell, referenceCell);
         result.iterationsUsed = static_cast<int>(permutations.size());

         if (m_controls.shouldShowDetails()) {
            std::cout << "Tested " << permutations.size() << " permutation combinations" << std::endl;
            std::cout << "Best complete P3 distance: " << result.p3Distance << std::endl;
         }

         if (isGoodMatch(result.p3Distance, result.ncDistance)) {
            result.success = true;
            result.transformationMatrix = bestCompleteMatrix;
            result.transformedMobileCell = bestCompleteMatrix * mobileCell;
            result.notes = "B4Matcher pipeline success";
         }
         else {
            result.notes = "B4Matcher pipeline insufficient";
         }

      }
      catch (const std::exception& e) {
         result.notes = std::string("B4Matcher pipeline failed: ") + e.what();
      }

      return result;
   }

   /**
    * Algorithm 5: Progressive guided cloud (single direction)
    */
   UnifiedMatchResult tryProgressiveGuidedCloud(const LRL_Cell& mobileCell,
      const LRL_Cell& referenceCell) const {
      UnifiedMatchResult result;
      result.algorithmUsed = "Progressive Guided Cloud";
      result.algorithmStep = 5;

      if (m_controls.shouldShowDetails()) {
         std::cout << "\n--- Algorithm 5: Progressive Guided Cloud ---" << std::endl;
      }

      try {
         // Create matcher with controls (new unified approach)
         ProductionLatticeMatcher matcher(m_controls);

         // Execute the matching algorithm
         const auto matchResult = matcher.findMatchingPath(mobileCell, referenceCell);

         // Extract results
         result.p3Distance = matchResult.finalP3Distance;
         result.ncDistance = CalculateNCDistWithReduction(matchResult.cumulativeTransformation * mobileCell, referenceCell);
         result.iterationsUsed = matchResult.stepsUsed;

         // Show progress if requested
         if (m_controls.shouldShowDetails()) {
            std::cout << "Progressive cloud used " << result.iterationsUsed << " steps" << std::endl;
            std::cout << "Final P3 distance: " << result.p3Distance << std::endl;
         }

         // Determine success using the new success threshold logic
         if (matchResult.success || isGoodMatch(result.p3Distance, result.ncDistance)) {
            result.success = true;
            result.transformationMatrix = matchResult.cumulativeTransformation;
            result.transformedMobileCell = result.transformationMatrix * mobileCell;
            result.notes = "Progressive cloud converged";
         }
         else {
            result.success = false;
            result.transformedMobileCell = mobileCell;  // Use original on failure
            result.notes = "Progressive cloud did not converge";
         }
      }
      catch (const std::exception& e) {
         // Handle any exceptions gracefully
         result.success = false;
         result.transformedMobileCell = mobileCell;  // Use original on error
         result.notes = std::string("Progressive cloud failed: ") + e.what();
      }

      return result;
   }


   /**
    * Algorithm 6: Bidirectional guided cloud
    */
   UnifiedMatchResult tryBidirectionalGuidedCloud(const LRL_Cell& mobileCell,
      const LRL_Cell& referenceCell) const {
      UnifiedMatchResult result;
      result.algorithmUsed = "Bidirectional Guided Cloud";
      result.algorithmStep = 6;

      if (m_controls.shouldShowDetails()) {
         std::cout << "\n--- Algorithm 6: Bidirectional Guided Cloud ---" << std::endl;
      }

      try {
         BidirectionalMatrixMatcher matcher(m_controls.getSuccessThreshold(),
            m_controls.getMaxIterations(), m_controls);
         const auto bidirResult = matcher.search(mobileCell, referenceCell);

         result.p3Distance = bidirResult.finalConvergenceDistance;
         result.iterationsUsed = bidirResult.iterations;

         if (bidirResult.converged) {
            result.success = true;
            result.transformedMobileCell = bidirResult.finalForwardCell;
            result.ncDistance = CalculateNCDistWithReduction(result.transformedMobileCell, referenceCell);
            result.notes = "Bidirectional cloud converged";
            const double ncDistance = result.ncDistance;

            // Calculate transformation matrix (would need implementation in BidirectionalMatrixMatcher)
            result.transformationMatrix.UnitMatrix(); // Placeholder
         }
         else {
            result.success = false;  // ✅ Add this
            result.ncDistance = CalculateNCDistWithReduction(bidirResult.finalForwardCell, referenceCell);  // ✅ Add this
            result.transformedMobileCell = bidirResult.finalForwardCell;  // ✅ Add this
            result.notes = "Bidirectional cloud did not converge";
         }

         if (m_controls.shouldShowDetails()) {
            std::cout << "Convergence distance: " << result.p3Distance << std::endl;
         }

      }
      catch (const std::exception& e) {
         result.notes = std::string("Bidirectional cloud failed: ") + e.what();
      }

      return result;
   }

   /**
    * Algorithm 7: Hybrid approach - guided cloud followed by B4Matcher
    */
   UnifiedMatchResult tryHybridApproach(const LRL_Cell& mobileCell,
      const LRL_Cell& referenceCell) const {
      UnifiedMatchResult result;
      result.algorithmUsed = "Hybrid Cloud+B4";
      result.algorithmStep = 7;

      if (m_controls.shouldShowDetails()) {
         std::cout << "\n--- Algorithm 7: Hybrid Approach ---" << std::endl;
      }

      try {
         // First, try progressive guided cloud to get closer
         const UnifiedMatchResult cloudResult = tryProgressiveGuidedCloud(mobileCell, referenceCell);

         if (cloudResult.success) {
            result = cloudResult;
            result.algorithmUsed = "Hybrid Cloud+B4";
            result.notes = "Hybrid: cloud phase succeeded";
            return result;
         }

         // If cloud got us closer but not close enough, use that result for B4Matcher
         LRL_Cell intermediateCell = mobileCell;
         if (!intermediateCell.IsValid()) {
            const int i19191 = 19191;
         }
         if (cloudResult.p3Distance < calculateP3Distance(mobileCell, referenceCell)) {
            intermediateCell = cloudResult.transformedMobileCell;
            if (!intermediateCell.IsValid()) {
               const int i19191 = 19191;
            }
            if (m_controls.shouldShowDetails()) {
               std::cout << "Using cloud result as B4Matcher input" << std::endl;
               std::cout << "Intermediate P3 distance: " << cloudResult.p3Distance << std::endl;
            }
         }

         // Now try B4Matcher on the intermediate result
         const UnifiedMatchResult b4Result = tryB4MatcherWithPipeline(intermediateCell, referenceCell);

         if (b4Result.success) {
            result.success = true;
            result.p3Distance = b4Result.p3Distance;
            result.ncDistance = b4Result.ncDistance;
            std::cout << "B4 result NC distance: " << b4Result.ncDistance << std::endl;
            result.iterationsUsed = cloudResult.iterationsUsed + b4Result.iterationsUsed;

            // Compose transformation matrices
            result.transformationMatrix = b4Result.transformationMatrix * cloudResult.transformationMatrix;
            result.transformedMobileCell = result.transformationMatrix * mobileCell;
            result.notes = "Hybrid: cloud + B4Matcher succeeded";
         }
         else {
            result.notes = "Hybrid: both phases failed";
            result.p3Distance = std::min(cloudResult.p3Distance, b4Result.p3Distance);

            // ✅ Add these missing lines:
            result.ncDistance = (cloudResult.p3Distance < b4Result.p3Distance) ?
               cloudResult.ncDistance : b4Result.ncDistance;
            result.iterationsUsed = cloudResult.iterationsUsed + b4Result.iterationsUsed;
            result.success = false;  // Make it explicit
         }

      }
      catch (const std::exception& e) {
         result.notes = std::string("Hybrid approach failed: ") + e.what();
      }

      return result;
   }
};

/**
 * Main program function
 */
int main() {
   std::cout << "; UNIFIED LATTICE MATCHING PROGRAM" << std::endl;

   try {
      MultiTransformFinderControls controls;
      const BasicProgramInput<MultiTransformFinderControls> program_setup("UnifiedLatticeMatcher", controls);

      if (program_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      const std::vector<LatticeCell>& inputList = program_setup.getInputList();

      if (inputList.size() < 2) {
         throw std::runtime_error("; Need at least 2 cells for comparison");
      }

      // Use first cell as reference, compare all others to it
      const LRL_Cell referenceCell = inputList[0].getCell();

      UnifiedLatticeMatcher matcher(controls);

      std::cout << "\n; Reference cell: " << LRL_Cell_Degrees(referenceCell) << std::endl;

      // Read flags from controls instead of hardcoding
      bool comparisonMode = controls.shouldRunComparisonMode();  // or whatever the method is
      bool csvOutput = controls.shouldOutputCsv();           // or whatever the method is

      // Alternative: if controls doesn't have these methods, you might need to add them
      // or read from command line arguments directly

      if (comparisonMode) {
         // ===================================
         // COMPARISON MODE: Collect Statistics
         // ===================================

         ComparisonStats stats;

         // Set reference cell for the summary
         std::ostringstream refStream;
         refStream << LRL_Cell_Degrees(referenceCell);
         stats.setReferenceCell(refStream.str());

         std::cout << "; COMPARISON MODE ACTIVE" << std::endl;
         std::cout << "; Processing " << (inputList.size() - 1) << " mobile cells..." << std::endl;
         std::cout << "; Reference: " << LRL_Cell_Degrees(referenceCell) << std::endl;

         // Process all mobile cells (suppress individual output)
         for (size_t i = 1; i < inputList.size(); ++i) {
            const LRL_Cell mobileCell = inputList[i].getCell();

            // Show progress for large datasets
            if ((inputList.size() - 1) >= 10 && (i % ((inputList.size() - 1) / 10) == 0)) {
               std::cout << "; Progress: " << (i * 100 / (inputList.size() - 1)) << "%" << std::endl;
            }

            const UnifiedMatchResult result = matcher.findBestMatch(mobileCell, referenceCell);
            stats.addResult(result);
         }

         // Output in clean order
         if (csvOutput) {
            stats.outputCompactCSVToFile("comparison_results.csv");  // Write CSV file
         }

         stats.outputSummary();           // Algorithm performance summary
      }
      else {
         // ===================================
         // INDIVIDUAL MODE: Detailed Output
         // ===================================

         const LRL_Cell referenceCell = inputList[0].getCell();
         std::cout << "; Reference cell: " << LRL_Cell_Degrees(referenceCell) << std::endl;
         std::cout << "; Comparing " << (inputList.size() - 1) << " mobile cells..." << std::endl;

         for (size_t i = 1; i < inputList.size(); ++i) {
            const LRL_Cell mobileCell = inputList[i].getCell();
            std::cout << "\n; === COMPARISON " << i << " ===" << std::endl;
            std::cout << "; Mobile cell: " << LRL_Cell_Degrees(mobileCell) << std::endl;

            const UnifiedMatchResult result = matcher.findBestMatch(mobileCell, referenceCell);
            std::cout << result << std::endl;

            if (result.success) {
               std::cout << "; SUCCESS with " << result.algorithmUsed << std::endl;
            }
            else {
               std::cout << "; FAILED - no algorithm succeeded" << std::endl;
            }
         }
      }
   }
   catch (const std::exception& e) {
      std::cerr << "; Error: " << e.what() << std::endl;
      return 1;
   }
}

#endif // UNIFIED_LATTICE_MATCHER_H