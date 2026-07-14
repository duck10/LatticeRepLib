#ifndef PRODUCTION_LATTICE_MATCHER_SYSTEM_H
#define PRODUCTION_LATTICE_MATCHER_SYSTEM_H

#include "CompareMatchResults.h"
#include "ProductionCommon.h"
#include "LatticeCell.h"
#include "LatticeMatchResult.h"
#include "Layer2Result.h"
#include "MatrixScore.h"
#include "UnifiedReservoir.h"
#include "MultiTransformFinderControls.h"
#include "TransformationMatrices.h"
#include "TransformerUtilities.h"

#include "Selling.h"
#include "LRL_Cell.h"

#include <vector>
#include <iostream>
#include <iomanip>

#include <chrono>

// ====================================================================
// LAYER 4: Core Unimodular Matrix Matcher
// ====================================================================
class CoreUnimodularMatcher {
public:
   explicit CoreUnimodularMatcher(const MultiTransformFinderControls& controls)
      : m_controls(controls)
      , m_reservoir(300)
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

   void mergeCase(const std::vector<LatticeMatchResult>& caseResults,
      const Matrix_3x3& refInverseMatrix,
      const Matrix_3x3& mobReductionMatrix);

   explicit NiggliReductionCoordinator(const MultiTransformFinderControls& controls)
      : m_controls(controls)
      , m_matcher(controls)
      , m_integrationReservoir(500)
   {
   }

   std::vector<LatticeMatchResult> performFourWayMatching(
      const LRL_Cell& primitiveMobile,
      const LRL_Cell& primitiveReference);

private:
   const MultiTransformFinderControls& m_controls;
   CoreUnimodularMatcher m_matcher;
   LatticeMatchReservoir m_integrationReservoir;

   ReductionData m_referenceReduction;
   ReductionData m_mobileReduction;

   ReductionData performNiggliReduction(const LRL_Cell& cell);
   ReductionData performSellingReduction(const LRL_Cell& cell);
   Matrix_3x3 applyStoredReductionMatrices(const Matrix_3x3& matchMatrix);

   void insertUniqueResult(std::vector<LatticeMatchResult>& results,
      const LatticeMatchResult& newResult) {
      auto lowerBound = std::lower_bound(results.begin(), results.end(), newResult);
      auto upperBound = std::upper_bound(results.begin(), results.end(), newResult);
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

   Layer2Result processLatticePair(
      const LatticeCell& reference,
      const LatticeCell& mobile);

private:
   const MultiTransformFinderControls& m_controls;
   NiggliReductionCoordinator m_niggliCoordinator;
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

   std::vector<LatticeMatchResult> processInputList(const std::vector<LatticeCell>& inputList);

   Layer2Result matchSinglePair(
      const LatticeCell& reference,
      const LatticeCell& mobile);

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
      std::cout << "DEBUG: Reference G6: " << G6(reference) << std::endl;
      std::cout << "DEBUG: Mobile G6: " << G6(mobile) << std::endl;
      std::cout << "DEBUG: Difference: " << (G6(reference) - G6(mobile)) << std::endl;
   }

   const int unimodularOrder = m_controls.getMatrixOrder();

   const auto t_init0 = std::chrono::high_resolution_clock::now();
   static std::vector<Matrix_3x3> matricesEQUIVALENT;
   static std::vector<Matrix_3x3> matricesSUPER;
   static std::vector<Matrix_3x3> matricesALL;
   if (matricesALL.empty()) {
      matricesEQUIVALENT = TransformationMatrices::generateUnimodularMatrices(unimodularOrder);
      matricesSUPER = TransformationMatrices::generateSupercellMatrices({ 2, 3, 4 }, 2);
      const auto hnf34 = TransformationMatrices::generateHNFMatrices({ 3, 4, 5, 6 });
      matricesSUPER.insert(matricesSUPER.end(), hnf34.begin(), hnf34.end());
      matricesALL = matricesEQUIVALENT;
      matricesALL.insert(matricesALL.end(), matricesSUPER.begin(), matricesSUPER.end());
   }
   const auto t_init1 = std::chrono::high_resolution_clock::now();
   static bool s_initTimed = false;
   if (!s_initTimed && m_controls.shouldShowDetails()) {
      s_initTimed = true;
      std::cout << "; Matrix init: "
         << std::chrono::duration_cast<std::chrono::milliseconds>(t_init1 - t_init0).count()
         << " ms  (ALL: " << matricesALL.size()
         << ", SUPER: " << matricesSUPER.size()
         << ", EQUIVALENT: " << matricesEQUIVALENT.size() << ")\n";
   }

   static std::vector<Matrix_3x3> matricesHNF;
   static std::vector<Matrix_3x3> matricesHNF_SUPER;
   if (matricesHNF.empty()) {
      // det=5,6 must be included here: they are HNF-only regardless of
      // USEHNF (the brute-force search is infeasible at those orders), so
      // this set has to carry them even when det=2,3,4 also fall back to
      // brute-force in the non-HNF path. Previously this list stopped at
      // 4, silently dropping det=5,6 from the USEHNF search entirely.
      matricesHNF_SUPER = TransformationMatrices::generateHNFMatrices({ 2, 3, 4, 5, 6 });
      matricesHNF = TransformationMatrices::generateUnimodularMatrices(1);
      matricesHNF.insert(matricesHNF.end(),
         matricesHNF_SUPER.begin(), matricesHNF_SUPER.end());
   }

   const auto runMode = m_controls.getRunMode();
   const bool useHNF = m_controls.useHNF();
   const std::vector<Matrix_3x3>& matrices =
      useHNF ?
      ((runMode == MultiTransformFinderControls::RunMode::SUPER) ? matricesHNF_SUPER :
         (runMode == MultiTransformFinderControls::RunMode::EQUIVALENT) ? matricesEQUIVALENT :
         matricesHNF)
      :
      ((runMode == MultiTransformFinderControls::RunMode::SUPER) ? matricesSUPER :
         (runMode == MultiTransformFinderControls::RunMode::EQUIVALENT) ? matricesEQUIVALENT :
         matricesALL);

   if (m_controls.shouldShowDetails()) {
      std::cout << "; Layer 4 - Testing " << matrices.size()
         << " matrices (mode: " << m_controls.getModeString()
         << ") for " << description << std::endl;
   }

   const auto t_search0 = std::chrono::high_resolution_clock::now();
   for (const auto& matrix : matrices) {
      const LRL_Cell transformedMobile = matrix * mobile;
      const double p3Distance = calculateP3Distance(reference, transformedMobile);
      LatticeMatchResult result(matrix, p3Distance, 19191.,
         transformedMobile, description);
      m_reservoir.add(result);
   }
   const auto t_search1 = std::chrono::high_resolution_clock::now();
   if (m_controls.shouldShowDetails()) {
      std::cout << "; Search (" << matrices.size() << " matrices, "
         << description << "): "
         << std::chrono::duration_cast<std::chrono::milliseconds>(t_search1 - t_search0).count()
         << " ms\n";
   }

   if (m_controls.shouldShowDetails()) {
      for (const auto& r : m_reservoir.getAllResults()) {
         if (r.getP3Distance() < 1e-10) {
            std::cout << "; NEAR-PERFECT MATCH in " << description << std::endl;
            std::cout << ";   Matrix: " << r.getTransformationMatrix() << std::endl;
            std::cout << ";   Mobile G6:      " << G6(mobile) << std::endl;
            std::cout << ";   Transformed G6: " << G6(r.getTransformedMobile()) << std::endl;
            std::cout << ";   Reference G6:   " << G6(reference) << std::endl;
            std::cout << ";   P3 distance:    " << r.getP3Distance() << std::endl;
         }
      }
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
inline void NiggliReductionCoordinator::mergeCase(
   const std::vector<LatticeMatchResult>& caseResults,
   const Matrix_3x3& refInverseMatrix,
   const Matrix_3x3& mobReductionMatrix)
{
   for (const auto& result : caseResults) {
      const Matrix_3x3 finalMatrix =
         refInverseMatrix * result.getTransformationMatrix() * mobReductionMatrix;
      // result.getDescription() already carries its case label (e.g. "Case A:
      // Primitive vs Primitive"), set by findBestMatches when caseResults was
      // produced -- do not prepend another label here, that duplicates it.
      const LatticeMatchResult corrected(finalMatrix, result.getP3Distance(), 19191.,
         result.getTransformedMobile(), result.getDescription());
      m_integrationReservoir.add(corrected);
   }
}

std::vector<LatticeMatchResult>
NiggliReductionCoordinator::performFourWayMatching(const LRL_Cell& primitiveMobile,
   const LRL_Cell& primitiveReference) {

   if (m_controls.shouldShowDetails()) {
      std::cout << "; Layer 3 - Starting 4-way matching process" << std::endl;
   }

   if (m_controls.getUseSellingReduction()) {
      m_referenceReduction = performSellingReduction(primitiveReference);
      m_mobileReduction = performSellingReduction(primitiveMobile);
   } else {
      m_referenceReduction = performNiggliReduction(primitiveReference);
      m_mobileReduction = performNiggliReduction(primitiveMobile);
   }

   G6 redRef, redMob;
   Niggli::ReduceWithoutMatrices(m_referenceReduction.reducedCell, redRef, m_controls.getNiggliDelta());
   Niggli::ReduceWithoutMatrices(m_mobileReduction.reducedCell, redMob, m_controls.getNiggliDelta());
   const double ncdistance = NCDist(redRef.data(), redMob.data());

   auto resultsA = m_matcher.findBestMatches(primitiveReference, primitiveMobile,
      "Case A: Primitive vs Primitive");
   auto resultsB = m_matcher.findBestMatches(m_referenceReduction.reducedCell, primitiveMobile,
      "Case B: Reduced Reference vs Primitive Mobile");
   auto resultsC = m_matcher.findBestMatches(primitiveReference, m_mobileReduction.reducedCell,
      "Case C: Primitive Reference vs Reduced Mobile");
   auto resultsD = m_matcher.findBestMatches(m_referenceReduction.reducedCell, m_mobileReduction.reducedCell,
      "Case D: Reduced vs Reduced");

   const Matrix_3x3 identity(1, 0, 0, 0, 1, 0, 0, 0, 1);
   const Matrix_3x3& refInv = m_referenceReduction.inverseReductionMatrix;
   const Matrix_3x3& mobRed = m_mobileReduction.reductionMatrix;

   m_integrationReservoir.clear();

   mergeCase(resultsA, identity, identity);
   if (m_controls.shouldShowDetails()) {
      std::cout << "; Layer 3 - After Case A: reservoir size = "
         << m_integrationReservoir.size()
         << "  best P3 = "
         << (m_integrationReservoir.empty() ? -1.0 : m_integrationReservoir.getBestDistance())
         << std::endl;
   }

   mergeCase(resultsB, refInv, identity);
   if (m_controls.shouldShowDetails()) {
      std::cout << "; Layer 3 - After Case B: reservoir size = "
         << m_integrationReservoir.size()
         << "  best P3 = "
         << (m_integrationReservoir.empty() ? -1.0 : m_integrationReservoir.getBestDistance())
         << std::endl;
   }

   mergeCase(resultsC, identity, mobRed);
   if (m_controls.shouldShowDetails()) {
      std::cout << "; Layer 3 - After Case C: reservoir size = "
         << m_integrationReservoir.size()
         << "  best P3 = "
         << (m_integrationReservoir.empty() ? -1.0 : m_integrationReservoir.getBestDistance())
         << std::endl;
   }

   mergeCase(resultsD, refInv, mobRed);
   if (m_controls.shouldShowDetails()) {
      std::cout << "; Layer 3 - After Case D: reservoir size = "
         << m_integrationReservoir.size()
         << "  best P3 = "
         << (m_integrationReservoir.empty() ? -1.0 : m_integrationReservoir.getBestDistance())
         << std::endl;
   }

   if (m_controls.shouldShowDetails()) {
      std::cout << "; Layer 3 - Merge complete: "
         << m_integrationReservoir.size() << " unique results" << std::endl;
      if (!m_integrationReservoir.empty()) {
         std::cout << "; Layer 3 - Best P3 distance: "
            << m_integrationReservoir.getBestDistance() << std::endl;
      }
   }

   return addNCDistanceToResults(m_integrationReservoir.getAllResults(), ncdistance);
}

inline NiggliReductionCoordinator::ReductionData
NiggliReductionCoordinator::performSellingReduction(const LRL_Cell& cell) {
   ReductionData data;

   MatS6 m66;
   Matrix_3x3 m33;
   S6 out;
   const bool success = Selling::ReduceWithtransforms(cell, m66, out, m33, 1.0E-6);

   if (success) {
      data.reducedCell = LRL_Cell(out);
      data.reductionMatrix = m33;
      data.inverseReductionMatrix = m33.Inverse();
      data.wasAlreadyReduced = false;
   } else {
      data.reducedCell = cell;
      data.reductionMatrix = Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
      data.inverseReductionMatrix = Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
      data.wasAlreadyReduced = true;
   }

   return data;
}

inline NiggliReductionCoordinator::ReductionData
NiggliReductionCoordinator::performNiggliReduction(const LRL_Cell& cell) {
   ReductionData data;

   if (Niggli::IsNiggli(G6(cell), m_controls.getNiggliDelta())) {
      data.reducedCell = cell;
      data.reductionMatrix = Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
      data.inverseReductionMatrix = Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
      data.wasAlreadyReduced = true;
      return data;
   }

   G6 reducedG6;
   MatG6 matG6;
   Matrix_3x3 matrix3d;

   bool success = Niggli::ReduceWithTransforms(G6(cell), matG6, matrix3d, reducedG6, m_controls.getNiggliDelta());

   if (success) {
      data.reducedCell = LRL_Cell(reducedG6);
      data.reductionMatrix = matrix3d;
      data.inverseReductionMatrix = matrix3d.Inverse();
      data.wasAlreadyReduced = false;
   } else {
      data.reducedCell = cell;
      data.reductionMatrix = Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
      data.inverseReductionMatrix = Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
      data.wasAlreadyReduced = true;
   }

   return data;
}

inline Matrix_3x3 NiggliReductionCoordinator::applyStoredReductionMatrices(const Matrix_3x3& matchMatrix) {
   return m_referenceReduction.inverseReductionMatrix * matchMatrix * m_mobileReduction.reductionMatrix;
}

// Layer 2 Implementation
inline Layer2Result
PrimitiveConversionManager::processLatticePair(const LatticeCell& reference,
   const LatticeCell& mobile) {
   if (m_controls.shouldShowDetails()) {
      std::cout << "; Layer 2 - Converting lattices to primitive" << std::endl;
      std::cout << "; volume of reference " << reference.getCell().Volume() << std::endl;
      std::cout << "; volume of mobile    " << mobile.getCell().Volume() << std::endl;
   }
   m_referenceConversion = convertToPrimitive(reference);
   m_mobileConversion = convertToPrimitive(mobile);
   if (m_controls.shouldShowDetails()) {
      std::cout << "; Layer 2 - Reference: " << reference.getLatticeType()
         << " -> Primitive: " << LRL_Cell_Degrees(m_referenceConversion.primitiveCell) << std::endl;
      std::cout << "; Layer 2 - Mobile: " << mobile.getLatticeType()
         << " -> Primitive: " << LRL_Cell_Degrees(m_mobileConversion.primitiveCell) << std::endl;
   }
   std::vector<LatticeMatchResult> niggliResults =
      m_niggliCoordinator.performFourWayMatching(
         m_mobileConversion.primitiveCell,
         m_referenceConversion.primitiveCell
      );
   std::vector<LatticeMatchResult> finalResults;
   finalResults.reserve(niggliResults.size());
   for (const auto& result : niggliResults) {
      Matrix_3x3 finalMatrix = applyStoredConversionMatrices(result.getTransformationMatrix());

      // Work entirely in primitive space
      LRL_Cell primitiveTransformed =
         result.getTransformationMatrix() * m_mobileConversion.primitiveCell;

      const double finalP3Distance =
         (P3(m_referenceConversion.primitiveCell) - P3(primitiveTransformed)).norm();

      if (m_controls.shouldShowDetails() && finalResults.size() < 5) {
         std::cout << "; Layer 2 DEBUG result " << finalResults.size() + 1
            << ": niggliP3=" << result.getP3Distance()
            << "  finalP3=" << finalP3Distance
            << "  finalMatrix det=" << finalMatrix.Det()
            << "  finalMatrix=" << finalMatrix
            << "  transformedMobile=" << LRL_Cell_Degrees(primitiveTransformed) << std::endl;
      }

      const LatticeMatchResult finalResult(finalMatrix,
         finalP3Distance,
         result.getNcDistance(),
         primitiveTransformed,
         "Final: " + result.getDescription());
      finalResults.emplace_back(finalResult);
   }
   std::sort(finalResults.begin(), finalResults.end(), compareMatchResults);
   if (m_controls.shouldShowDetails()) {
      std::cout << "; Layer 2 - Applied conversion matrices to " << finalResults.size()
         << " results" << std::endl;
   }

   Layer2Result layer2;
   layer2.primitiveReference = m_referenceConversion.primitiveCell;
   layer2.primitiveMobile = m_mobileConversion.primitiveCell;
   layer2.referenceFromPrimitive = m_referenceConversion.fromPrimitiveMatrix;
   layer2.referenceLatticeType = reference.getLatticeType();
   layer2.mobileLatticeType = mobile.getLatticeType();
   layer2.results = finalResults;
   if (m_controls.shouldShowDetails()) {
      std::cout << layer2;
   }
   return layer2;
}

inline PrimitiveConversionManager::ConversionData
PrimitiveConversionManager::convertToPrimitive(const LatticeCell& lattice) {
   ConversionData data;
   data.originalLatticeType = lattice.getLatticeType();
   data.toPrimitiveMatrix = ToPrimitive(lattice.getLatticeType(), lattice.getCell());
   data.primitiveCell = data.toPrimitiveMatrix * lattice.getCell();  // MOVE THIS UP
   data.fromPrimitiveMatrix = data.toPrimitiveMatrix.Inverse();

   if (m_controls.shouldShowDetails()) {
      std::cout << "; DEBUG convertToPrimitive: latticeType=" << lattice.getLatticeType() << std::endl;
      std::cout << "; DEBUG: Original cell: " << LRL_Cell_Degrees(lattice.getCell()) << std::endl;
      std::cout << "; DEBUG: Matrix determinant: " << data.toPrimitiveMatrix.Det() << std::endl;
      std::cout << "; DEBUG: Matrix: " << data.toPrimitiveMatrix << std::endl;
      std::cout << "; DEBUG: Transformed cell: " << LRL_Cell_Degrees(data.primitiveCell) << std::endl;
      std::cout << "; DEBUG: Primitive cell G6: " << G6(data.primitiveCell) << std::endl;
   }

   return data;
}

inline Matrix_3x3 PrimitiveConversionManager::applyStoredConversionMatrices(const Matrix_3x3& niggliResult) {
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

   for (size_t i = 1; i < inputList.size(); ++i) {
      const LatticeCell& mobile = inputList[i];

      if (m_controls.shouldShowDetails()) {
         std::cout << "\n; --- Processing Mobile " << i << " ---" << std::endl;
         std::cout << "; Mobile: " << LRL_Cell_Degrees(mobile.getCell())
            << " (" << mobile.getLatticeType() << ")" << std::endl;
      }

      const Layer2Result layer2 = matchSinglePair(reference, mobile);
      std::vector<LatticeMatchResult> results = layer2.results;

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

   if (m_controls.shouldShowDetails()) {
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

inline Layer2Result
ProductionLatticeMatcherSystem::matchSinglePair(const LatticeCell& reference,
   const LatticeCell& mobile) {
   return m_primitiveManager.processLatticePair(reference, mobile);
}

#endif // PRODUCTION_LATTICE_MATCHER_SYSTEM_H
