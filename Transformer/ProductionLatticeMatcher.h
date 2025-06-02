// ProductionLatticeMatcher.h
#ifndef PRODUCTION_LATTICE_MATCHER_H
#define PRODUCTION_LATTICE_MATCHER_H

#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "Matrix_3x3.h"
#include "P3.h"
#include "TransformerUtilities.h"
#include <vector>
#include <iostream>

/**
 * Production lattice matcher based on proven gradient descent algorithm
 * Uses exhaustive matrix testing at each step for optimal results
 */
class ProductionLatticeMatcher {
public:
   struct MatchingStep {
      LRL_Cell mobileCell;
      LRL_Cell referenceCell;
      Matrix_3x3 appliedMatrix;
      double p3Distance;
      int stepNumber;

      MatchingStep()
         : mobileCell()
         , referenceCell()
         , appliedMatrix()
         , p3Distance(19191.0)
         , stepNumber(19191) {
      }

      friend std::ostream& operator<<(std::ostream& os, const MatchingStep& step) {
         os << "Step " << step.stepNumber << ": P3 = " << step.p3Distance << "\n"
            << "  Mobile: " << LRL_Cell_Degrees(step.mobileCell) << "\n"
            << "  Matrix:\n" << step.appliedMatrix;
         return os;
      }
   };

   struct MatchResult {
      bool success;
      std::vector<MatchingStep> matchingPath;
      Matrix_3x3 cumulativeTransformation;
      double initialP3Distance;
      double finalP3Distance;
      int stepsUsed;

      MatchResult()
         : success(false)
         , matchingPath()
         , cumulativeTransformation()
         , initialP3Distance(19191.0)
         , finalP3Distance(19191.0)
         , stepsUsed(19191) {
      }

      double getImprovementPercent() const {
         return (initialP3Distance > 0.0) ?
            100.0 * (1.0 - finalP3Distance / initialP3Distance) : 0.0;
      }

      double getTotalImprovement() const {
         return initialP3Distance - finalP3Distance;
      }

      friend std::ostream& operator<<(std::ostream& os, const MatchResult& result) {
         os << "MatchResult:\n"
            << "  Success: " << (result.success ? "true" : "false") << "\n"
            << "  Steps: " << result.stepsUsed << "\n"
            << "  Initial P3: " << result.initialP3Distance << "\n"
            << "  Final P3: " << result.finalP3Distance << "\n"
            << "  Improvement: " << result.getTotalImprovement()
            << " (" << result.getImprovementPercent() << "%)\n"
            << "  Cumulative Matrix:\n" << result.cumulativeTransformation;
         return os;
      }
   };

   struct MatchingOptions {
      int maxIterations;
      double convergenceThreshold;
      bool showProgress;
      bool showDetailedSteps;
      bool includeStepHistory;

      MatchingOptions()
         : maxIterations(200)
         , convergenceThreshold(1e-8)
         , showProgress(false)
         , showDetailedSteps(false)
         , includeStepHistory(true) {
      }

      friend std::ostream& operator<<(std::ostream& os, const MatchingOptions& opts) {
         os << "MatchingOptions:\n"
            << "  Max Iterations: " << opts.maxIterations << "\n"
            << "  Convergence Threshold: " << opts.convergenceThreshold << "\n"
            << "  Show Progress: " << (opts.showProgress ? "true" : "false") << "\n"
            << "  Show Detailed Steps: " << (opts.showDetailedSteps ? "true" : "false") << "\n"
            << "  Include Step History: " << (opts.includeStepHistory ? "true" : "false");
         return os;
      }
   };

private:
   const MultiTransformFinderControls& m_controls;
   std::vector<Matrix_3x3> m_allMatrices;

   void buildMatrixCollection();
   std::vector<Matrix_3x3> getAllTransformationMatrices() const;

   double calculateP3Distance(const LRL_Cell& cell1, const LRL_Cell& cell2) const;

   Matrix_3x3 findBestMatrixForStep(const LRL_Cell& currentMobile,
      const LRL_Cell& referenceCell,
      double& bestDistance) const;

public:
   ProductionLatticeMatcher(const MultiTransformFinderControls& controls);

   /**
    * Main matching function - finds transformation path from mobile to reference
    * Based on your proven gradient descent algorithm
    */
   MatchResult findMatchingPath(const LRL_Cell& mobileCell,
      const LRL_Cell& referenceCell) const;

   /**
    * Simple interface - returns just the final transformation matrix
    * For customers who only need the end result
    */
   Matrix_3x3 findTransformationMatrix(const LRL_Cell& mobileCell,
      const LRL_Cell& referenceCell) const;

   // Accessors
   int getMatrixCount() const { return static_cast<int>(m_allMatrices.size()); }
   const MultiTransformFinderControls& getOptions() const { return m_controls; }

   // Analysis functions
   void analyzeMatchingPath(const MatchResult& result) const;
   bool validateMatrixCollection() const;

   // Convenience test functions (matching your interface)
   void testMatchingExperiment(const LRL_Cell& mobileCell,
      const LRL_Cell& referenceCell) const;

   friend std::ostream& operator<<(std::ostream& os, const ProductionLatticeMatcher& matcher) {
      os << "ProductionLatticeMatcher:\n"
         << "  Matrix Count: " << matcher.getMatrixCount() << "\n"
         << matcher.getOptions();
      return os;
   }
};

#endif // PRODUCTION_LATTICE_MATCHER_H

