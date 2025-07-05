#ifndef PRODUCTION_LATTICE_MATCHER_SYSTEM_H
#define PRODUCTION_LATTICE_MATCHER_SYSTEM_H

#include "ProductionCommon.h"
#include "LatticeCell.h"
#include "LatticeMatchResult.h"
#include "UnifiedReservoir.h"
#include "MultiTransformFinderControls.h"
#include "TransformationMatrices.h"
#include "TransformerUtilities.h"
#include <vector>
#include <iostream>
#include <iomanip>

#include "MatrixSimplicityRanker.h"

// ====================================================================
// LAYER 4: Core Unimodular Matrix Matcher
// ====================================================================
class CoreUnimodularMatcher {
public:
   explicit CoreUnimodularMatcher(const MultiTransformFinderControls& controls)
      : m_controls(controls)
      , m_reservoir(3000)
   {
   }

   std::vector<LatticeMatchResult> findBestMatches(
      const LRL_Cell& reference,
      const LRL_Cell& mobile,
      const std::string& description = "");

private:
   struct MobileResult {
      int mobileIndex;
      LatticeCell mobile;
      LatticeMatchResult result;
      double distance;
   };
   const MultiTransformFinderControls& m_controls;
   LatticeMatchReservoir m_reservoir;

   double calculateP3Distance(const LRL_Cell& cell1, const LRL_Cell& cell2) const;
};

// ====================================================================
// LAYER 3: Niggli Reduction & 4-Way Matching Coordinator
// ====================================================================
class NiggliReductionCoordinator {
public:
   struct ReductionData {
      LRL_Cell reducedCell;
      Matrix_3x3 reductionMatrix;
      Matrix_3x3 inverseReductionMatrix;
      bool wasAlreadyReduced;

      ReductionData() : wasAlreadyReduced(false) {}
   };

   explicit NiggliReductionCoordinator(const MultiTransformFinderControls& controls)
      : m_controls(controls)
      , m_matcher(controls)
      , m_integrationReservoir(500)  // Reduce from 50 to 10 to force better deduplication
   {
   }

   std::vector<LatticeMatchResult> performFourWayMatching(
      const LRL_Cell& primitiveMobile,
      const LRL_Cell& primitiveReference);

private:
   const MultiTransformFinderControls& m_controls;
   CoreUnimodularMatcher m_matcher;
   LatticeMatchReservoir m_integrationReservoir;

   // Store reduction matrices for later application
   ReductionData m_referenceReduction;
   ReductionData m_mobileReduction;

   ReductionData performNiggliReduction(const LRL_Cell& cell);
   Matrix_3x3 applyStoredReductionMatrices(const Matrix_3x3& matchMatrix);
   void integrateResultsIntoReservoir(const std::vector<LatticeMatchResult>& results,
      const std::string& caseDescription);

   void insertUniqueResult(std::vector<LatticeMatchResult>& results,
      const LatticeMatchResult& newResult) {

      // Find the range of results with the same P3 distance
      auto lowerBound = std::lower_bound(results.begin(), results.end(), newResult);
      auto upperBound = std::upper_bound(results.begin(), results.end(), newResult);

      // Check for matrix duplicates within this P3 distance range
      for (auto it = lowerBound; it != upperBound; ++it) {
         if (it->getTransformationMatrix() == newResult.getTransformationMatrix()) {
            // Duplicate matrix found - don't insert
            return;
         }
      }

      // No duplicate found - insert at the correct sorted position
      results.insert(lowerBound, newResult);
   }
};

// ====================================================================
// LAYER 2: Primitive Conversion & Final Matrix Application
// ====================================================================
class PrimitiveConversionManager {
public:
   struct ConversionData {
      LRL_Cell primitiveCell;
      Matrix_3x3 toPrimitiveMatrix;
      Matrix_3x3 fromPrimitiveMatrix;
      std::string originalLatticeType;

      ConversionData() {}
   };

   explicit PrimitiveConversionManager(const MultiTransformFinderControls& controls)
      : m_controls(controls)
      , m_niggliCoordinator(controls)
   {
   }

   std::vector<LatticeMatchResult> processLatticePair(
      const LatticeCell& reference,
      const LatticeCell& mobile);

private:
   const MultiTransformFinderControls& m_controls;
   NiggliReductionCoordinator m_niggliCoordinator;

   // Store conversion matrices for final application
   ConversionData m_referenceConversion;
   ConversionData m_mobileConversion;

   ConversionData convertToPrimitive(const LatticeCell& lattice);
   Matrix_3x3 applyStoredConversionMatrices(const Matrix_3x3& niggliResult);
};

// ====================================================================
// LAYER 1: Main Interface
// ====================================================================
class ProductionLatticeMatcherSystem {
public:
   explicit ProductionLatticeMatcherSystem(const MultiTransformFinderControls& controls)
      : m_controls(controls)
      , m_primitiveManager(controls)
   {
   }

   // Main entry point - processes vector of LatticeCell inputs
   std::vector<LatticeMatchResult> processInputList(const std::vector<LatticeCell>& inputList);

   // Single pair processing
   std::vector<LatticeMatchResult> matchSinglePair(
      const LatticeCell& reference,
      const LatticeCell& mobile);

   // Get the single best match
   LatticeMatchResult getBestMatch(const LatticeCell& reference, const LatticeCell& mobile);

private:
   const MultiTransformFinderControls& m_controls;
   PrimitiveConversionManager m_primitiveManager;
};

// ====================================================================
// IMPLEMENTATIONS
// ====================================================================

// Layer 4 Implementation
inline std::vector<LatticeMatchResult>
CoreUnimodularMatcher::findBestMatches(const LRL_Cell& reference,
   const LRL_Cell& mobile,
   const std::string& description) {
   m_reservoir.clear();

   if (m_controls.shouldShowDetails()) {
      // In your Layer 4 core matcher, before P3 calculation:
      std::cout << "DEBUG: Reference G6: " << G6(reference) << std::endl;
      std::cout << "DEBUG: Mobile G6: " << G6(mobile) << std::endl;
      std::cout << "DEBUG: Difference: " << (G6(reference) - G6(mobile)) << std::endl;
   }

   static const std::vector<Matrix_3x3> matrices = TransformationMatrices::generateUnimodularMatrices();

   if (m_controls.shouldShowDetails()) {
      std::cout << "; Layer 4 - Testing " << matrices.size()
         << " unimodular matrices for " << description << std::endl;
   }

   for (const auto& matrix : matrices) {
      // Apply transformation to mobile
      const LRL_Cell transformedMobile = matrix * mobile;

      // Calculate P3 distance
      const double p3Distance = calculateP3Distance(reference, transformedMobile);

      // Create result and try to add to reservoir
      LatticeMatchResult result(matrix, p3Distance, 19191.,
         transformedMobile, description);
      m_reservoir.add(result);
   }

   std::vector<LatticeMatchResult> results = m_reservoir.getAllResults();

   if (m_controls.shouldShowDetails()) {
      std::cout << "; Layer 4 - Found " << results.size()
         << " viable matches for " << description << std::endl;
      if (!results.empty()) {
         std::cout << "; Layer 4 - Best P3 distance: " << results[0].getP3Distance() << std::endl;
      }
   }

   return results;
}

inline double CoreUnimodularMatcher::calculateP3Distance(const LRL_Cell& cell1, const LRL_Cell& cell2) const {
   const P3 p1(cell1);
   const P3 p2(cell2);
   return (p1 - p2).norm();
}

// Static helper function (can be in the same file or header)
static std::vector<LatticeMatchResult> addNCDistanceToResults(
   const std::vector<LatticeMatchResult>& results,
   const double ncDistance) {

   std::vector<LatticeMatchResult> output;
   for (auto& result : results) {
      LatticeMatchResult temp = result;
      temp.setNCDistance(ncDistance);
      output.emplace_back(temp);
   }
   return output;
}

// Layer 3 Implementation
// CORRECT SOLUTION: Sequential merge with matrix correction at each step
// Replace the performFourWayMatching method in NiggliReductionCoordinator

// COMPLETE: Layer 3 performFourWayMatching with proper duplicate detection
std::vector<LatticeMatchResult>
NiggliReductionCoordinator::performFourWayMatching(const LRL_Cell& primitiveMobile,
   const LRL_Cell& primitiveReference) {

   if (m_controls.shouldShowDetails()) {
      std::cout << "; Layer 3 - Starting 4-way matching process" << std::endl;
   }

   // Perform Niggli reductions and store matrices
   m_referenceReduction = performNiggliReduction(primitiveReference);
   m_mobileReduction = performNiggliReduction(primitiveMobile);

   // Calculate NC distance (already existing in original code)
   G6 redRef;
   G6 redMob;
   Niggli::ReduceWithoutMatrices(m_referenceReduction.reducedCell, redRef, m_controls.getNiggliDelta());
   Niggli::ReduceWithoutMatrices(m_mobileReduction.reducedCell, redMob, m_controls.getNiggliDelta());
   const double ncdistance = NCDist(redRef.data(), redMob.data());

   // Get sorted results from all 4 cases
   auto resultsA = m_matcher.findBestMatches(primitiveReference, primitiveMobile,
      "Case A: Primitive vs Primitive");
   auto resultsB = m_matcher.findBestMatches(m_referenceReduction.reducedCell, primitiveMobile,
      "Case B: Reduced Reference vs Primitive Mobile");
   auto resultsC = m_matcher.findBestMatches(primitiveReference, m_mobileReduction.reducedCell,
      "Case C: Primitive Reference vs Reduced Mobile");
   auto resultsD = m_matcher.findBestMatches(m_referenceReduction.reducedCell, m_mobileReduction.reducedCell,
      "Case D: Reduced vs Reduced");

   // STEP 1: Start with Case A (P-to-P) as provisional reservoir
   std::vector<LatticeMatchResult> provisionalResults;
   for (const auto& result : resultsA) {
      Matrix_3x3 finalMatrix = applyStoredReductionMatrices(result.getTransformationMatrix());
      provisionalResults.emplace_back(finalMatrix, result.getP3Distance(), 19191.,
         result.getTransformedMobile(), "Case A: " + result.getDescription());
   }

   if (m_controls.shouldShowDetails()) {
      std::cout << "; Layer 3 - Step 1: Started with " << provisionalResults.size()
         << " Case A results" << std::endl;
   }

   LatticeMatchResult correctedResult;
   insertUniqueResult(provisionalResults, correctedResult);

   if (m_controls.shouldShowDetails()) {
      std::cout << "; Layer 3 - Step 4: Final merge complete, total " << provisionalResults.size()
         << " results" << std::endl;
      if (!provisionalResults.empty()) {
         std::cout << "; Layer 3 - Best P3 distance: " << provisionalResults[0].getP3Distance() << std::endl;
      }
   }

   // Limit to reservoir capacity
   size_t maxResults = std::min(provisionalResults.size(), size_t(500));
   std::vector<LatticeMatchResult> finalResults(provisionalResults.begin(),
      provisionalResults.begin() + maxResults);

   // Add NC distance and return
   return addNCDistanceToResults(finalResults, ncdistance);
}

inline NiggliReductionCoordinator::ReductionData
NiggliReductionCoordinator::performNiggliReduction(const LRL_Cell& cell) {
   ReductionData data;

   // Check if already reduced
   if (Niggli::IsNiggli(G6(cell), m_controls.getNiggliDelta())) {
      data.reducedCell = cell;
      data.reductionMatrix = Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1); // Identity
      data.inverseReductionMatrix = Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
      data.wasAlreadyReduced = true;
      return data;
   }

   // Perform Niggli reduction with transform tracking
   G6 reducedG6;
   MatG6 matG6;
   Matrix_3x3 matrix3d;

   bool success = Niggli::ReduceWithTransforms(G6(cell), matG6, matrix3d, reducedG6, m_controls.getNiggliDelta());

   if (success) {
      data.reducedCell = LRL_Cell(reducedG6);
      data.reductionMatrix = matrix3d;
      data.inverseReductionMatrix = matrix3d.Inverse();
      data.wasAlreadyReduced = false;
   }
   else {
      // Fallback to original if reduction failed
      data.reducedCell = cell;
      data.reductionMatrix = Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
      data.inverseReductionMatrix = Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
      data.wasAlreadyReduced = true;
   }

   return data;
}

inline Matrix_3x3 NiggliReductionCoordinator::applyStoredReductionMatrices(const Matrix_3x3& matchMatrix) {
   // Transform from primitive mobile to primitive reference:
   // primitiveRef = invReductionRef * matchMatrix * reductionMobile * primitiveMobile
   return m_referenceReduction.inverseReductionMatrix * matchMatrix * m_mobileReduction.reductionMatrix;
}

inline void NiggliReductionCoordinator::integrateResultsIntoReservoir(
   const std::vector<LatticeMatchResult>& results,
   const std::string& caseDescription) {

   for (const auto& result : results) {
      // Apply the stored reduction matrices to get the final transformation
      Matrix_3x3 finalMatrix = applyStoredReductionMatrices(result.getTransformationMatrix());

      // Don't transform the cell here - preserve Layer 4's work for now
      // The final cell transformation will happen in Layer 2
      LRL_Cell placeholderTransformedCell = result.getTransformedMobile();

      // Create new result with final matrix and properly transformed cell
      // IMPORTANT: Use the original P3 distance, not recalculated, to preserve Layer 4's work
      LatticeMatchResult finalResult(finalMatrix,
         result.getP3Distance(),
         19191.,
         placeholderTransformedCell,  // Will be corrected in Layer 2
         caseDescription + ": " + result.getDescription());

      // The reservoir will automatically deduplicate and keep only the best results
      m_integrationReservoir.add(finalResult);
   }
}

// Layer 2 Implementation
inline std::vector<LatticeMatchResult>
PrimitiveConversionManager::processLatticePair(const LatticeCell& reference,
   const LatticeCell& mobile) {
   if (m_controls.shouldShowDetails()) {
      std::cout << "; Layer 2 - Converting lattices to primitive" << std::endl;
   }

   // Convert to primitive and store conversion matrices
   m_referenceConversion = convertToPrimitive(reference);
   m_mobileConversion = convertToPrimitive(mobile);

   if (m_controls.shouldShowDetails()) {
      std::cout << "; Layer 2 - Reference: " << reference.getLatticeType()
         << " -> Primitive: " << LRL_Cell_Degrees(m_referenceConversion.primitiveCell) << std::endl;
      std::cout << "; Layer 2 - Mobile: " << mobile.getLatticeType()
         << " -> Primitive: " << LRL_Cell_Degrees(m_mobileConversion.primitiveCell) << std::endl;
   }

   // Call Layer 3 with primitive cells
   std::vector<LatticeMatchResult> niggliResults =
      m_niggliCoordinator.performFourWayMatching(m_mobileConversion.primitiveCell,
         m_referenceConversion.primitiveCell);

   // Apply stored conversion matrices to all results
   std::vector<LatticeMatchResult> finalResults;
   finalResults.reserve(niggliResults.size());

   for (const auto& result : niggliResults) {
      Matrix_3x3 finalMatrix = applyStoredConversionMatrices(result.getTransformationMatrix());

      // CRITICAL FIX: Apply final matrix to original mobile for consistency
      LRL_Cell finalTransformedMobile = finalMatrix * mobile.getCell();

      const double finalP3Distance = (P3(reference.getCell()) - P3(finalTransformedMobile)).norm();

      const LatticeMatchResult finalResult(finalMatrix,
         finalP3Distance,  // Use recalculated distance, not Layer 4's distance
         result.getNcDistance(),
         finalTransformedMobile,
         "Final: " + result.getDescription());

      finalResults.emplace_back(finalResult);
   }

   std::sort(finalResults.begin(), finalResults.end(),
      [](const LatticeMatchResult& a, const LatticeMatchResult& b) {
         return a.getP3Distance() < b.getP3Distance();
      });


   if (m_controls.shouldShowDetails()) {
      std::cout << "; Layer 2 - Applied conversion matrices to " << finalResults.size()
         << " results" << std::endl;
   }

   return finalResults;
}

inline PrimitiveConversionManager::ConversionData
PrimitiveConversionManager::convertToPrimitive(const LatticeCell& lattice) {
   ConversionData data;
   data.originalLatticeType = lattice.getLatticeType();
   data.toPrimitiveMatrix = ToPrimitive(lattice.getLatticeType(), lattice.getCell());

   // DEBUG: Add these lines
   if (m_controls.shouldShowDetails()) {
      std::cout << "; DEBUG: Original cell: " << LRL_Cell_Degrees(lattice.getCell()) << std::endl;
      std::cout << "; DEBUG: Matrix determinant: " << data.toPrimitiveMatrix.Det() << std::endl;
      std::cout << "; DEBUG: Matrix: " << data.toPrimitiveMatrix << std::endl;
   }

   data.primitiveCell = data.toPrimitiveMatrix * lattice.getCell();

   // DEBUG: Add this line
   if (m_controls.shouldShowDetails()) {
      std::cout << "; DEBUG: Transformed cell: " << LRL_Cell_Degrees(data.primitiveCell) << std::endl;
   }

   data.fromPrimitiveMatrix = data.toPrimitiveMatrix.Inverse();
   return data;
}

inline Matrix_3x3 PrimitiveConversionManager::applyStoredConversionMatrices(const Matrix_3x3& niggliResult) {
   // Transform from centered mobile to centered reference:
   // centeredRef = fromPrimRef * niggliResult * toPrimMobile * centeredMobile
   return m_referenceConversion.fromPrimitiveMatrix * niggliResult * m_mobileConversion.toPrimitiveMatrix;
}

// Layer 1 Implementation
inline std::vector<LatticeMatchResult>
ProductionLatticeMatcherSystem::processInputList(const std::vector<LatticeCell>& inputList) {
   if (inputList.size() < 2) {
      std::cout << "Error: At least 2 input cells are required" << std::endl;
      return {};
   }

   std::vector<LatticeMatchResult> allResults;
   const LatticeCell& reference = inputList[0];

   if (m_controls.shouldShowDetails()) {
      std::cout << "; === PRODUCTION LATTICE MATCHING SYSTEM ===" << std::endl;
      std::cout << "; Reference: " << LRL_Cell_Degrees(reference.getCell())
         << " (" << reference.getLatticeType() << ")" << std::endl;
      std::cout << "; Processing " << (inputList.size() - 1) << " mobile lattice(s)" << std::endl;
   }

   // Process each mobile lattice against the reference
   for (size_t i = 1; i < inputList.size(); ++i) {
      const LatticeCell& mobile = inputList[i];

      if (m_controls.shouldShowDetails()) {
         std::cout << "\n; --- Processing Mobile " << i << " ---" << std::endl;
         std::cout << "; Mobile: " << LRL_Cell_Degrees(mobile.getCell())
            << " (" << mobile.getLatticeType() << ")" << std::endl;
      }

      std::vector<LatticeMatchResult> results = matchSinglePair(reference, mobile);

      // Add mobile index to descriptions
      for (auto& result : results) {
         std::string newDesc = "Mobile_" + std::to_string(i) + ": " + result.getDescription();
         result = LatticeMatchResult(result.getTransformationMatrix(),
            result.getP3Distance(),
            result.getNcDistance(),
            result.getTransformedMobile(),
            newDesc);
      }

      allResults.insert(allResults.end(), results.begin(), results.end());
   }

   if ( m_controls.shouldShowDetails())
   {
      std::cout << "DEBUG: After simplicity ranking, first matrix: [";
      for (int i = 0; i < 9; ++i) {
         std::cout << allResults[0].getTransformationMatrix()[i];
         if (i < 8) std::cout << " ";
      }
      std::cout << "]" << std::endl;
   }



   if (m_controls.shouldShowDetails()) {
      std::cout << "\n; === FINAL RESULTS ===" << std::endl;
      std::cout << "; Total matches found: " << allResults.size() << std::endl;
      if (!allResults.empty()) {
         std::cout << "; Best overall P3 distance: " << allResults[0].getP3Distance() << std::endl;
      }
   }

   return allResults;
}

inline std::vector<LatticeMatchResult>
ProductionLatticeMatcherSystem::matchSinglePair(const LatticeCell& reference,
   const LatticeCell& mobile) {
   return m_primitiveManager.processLatticePair(reference, mobile);
}

inline LatticeMatchResult ProductionLatticeMatcherSystem::getBestMatch(const LatticeCell& reference,
   const LatticeCell& mobile) {
   std::vector<LatticeMatchResult> results = matchSinglePair(reference, mobile);

   if (results.empty()) {
      return LatticeMatchResult(Matrix_3x3(), 19191.0, 19191.0, mobile.getCell(),
         "No match found");
   }

   return results[0]; // Best match (results are sorted)
}

#endif // PRODUCTION_LATTICE_MATCHER_SYSTEM_H

