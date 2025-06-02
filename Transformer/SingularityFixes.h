// SingularityFixes.h
// Critical fixes for the 90° singularity problem

#ifndef SINGULARITY_FIXES_H
#define SINGULARITY_FIXES_H

#include "ProductionLatticeMatcher.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "Matrix_3x3.h"
#include "TransformerUtilities.h"
#include <vector>
#include <algorithm>

/**
 * Enhanced matcher with 90° singularity fixes
 */
class SingularityFixedMatcher {
public:
   struct FixedMatchingOptions {
      int maxIterations;
      double convergenceThreshold;
      bool showProgress;

      // NEW: Singularity-specific options
      bool allowTemporaryWorsening;      // Key fix!
      int worseningTolerance;            // Steps allowed to get worse
      double worseningThreshold;         // How much worse is acceptable
      bool useAngleSpecificMatrices;     // Target 90° crossings specifically
      bool useMultiPathSearch;           // Try multiple transformation paths

      FixedMatchingOptions()
         : maxIterations(200)
         , convergenceThreshold(1e-8)
         , showProgress(false)
         , allowTemporaryWorsening(true)   // CRITICAL
         , worseningTolerance(3)           // Allow 3 steps of worsening
         , worseningThreshold(2.0)         // Max 2x worse P3 distance
         , useAngleSpecificMatrices(true)
         , useMultiPathSearch(true) {
      }

      friend std::ostream& operator<<(std::ostream& os, const FixedMatchingOptions& opts) {
         os << "FixedMatchingOptions:\n"
            << "  Max Iterations: " << opts.maxIterations << "\n"
            << "  Allow Temporary Worsening: " << (opts.allowTemporaryWorsening ? "YES" : "NO") << "\n"
            << "  Worsening Tolerance: " << opts.worseningTolerance << " steps\n"
            << "  Worsening Threshold: " << opts.worseningThreshold << "x\n"
            << "  Angle-Specific Matrices: " << (opts.useAngleSpecificMatrices ? "YES" : "NO") << "\n"
            << "  Multi-Path Search: " << (opts.useMultiPathSearch ? "YES" : "NO");
         return os;
      }
   };

private:
   const FixedMatchingOptions m_options;
   std::vector<Matrix_3x3> m_standardMatrices;
   std::vector<Matrix_3x3> m_angleSpecificMatrices;

   // Generate matrices specifically for 90° crossings
   std::vector<Matrix_3x3> generateAngleSpecificMatrices() const {
      std::vector<Matrix_3x3> matrices;

      // Direct angle transformations that cross 90°
      // α flip: moves acute angles to obtuse and vice versa
      matrices.emplace_back(-1, 0, 0, 0, 1, 0, 0, 0, 1);   // α → 180° - α
      matrices.emplace_back(1, 0, 0, 0, -1, 0, 0, 0, 1);   // β → 180° - β  
      matrices.emplace_back(1, 0, 0, 0, 1, 0, 0, 0, -1);   // γ → 180° - γ

      // Compound transformations for stubborn cases
      matrices.emplace_back(-1, 0, 0, 0, -1, 0, 0, 0, 1);  // α,β flip
      matrices.emplace_back(-1, 0, 0, 0, 1, 0, 0, 0, -1);  // α,γ flip
      matrices.emplace_back(1, 0, 0, 0, -1, 0, 0, 0, -1);  // β,γ flip
      matrices.emplace_back(-1, 0, 0, 0, -1, 0, 0, 0, -1); // All angles flip

      // Shear transformations that can move angles across 90°
      matrices.emplace_back(1, 1, 0, 0, -1, 0, 0, 0, 1);   // Special shear 1
      matrices.emplace_back(-1, 1, 0, 0, 1, 0, 0, 0, 1);   // Special shear 2
      matrices.emplace_back(1, 0, 0, 1, -1, 0, 0, 0, 1);   // Special shear 3

      return matrices;
   }

   bool needsCrossing90(const LRL_Cell& mobileCell, const LRL_Cell& referenceCell) const {
      const LRL_Cell_Degrees mobileDeg(mobileCell);
      const LRL_Cell_Degrees refDeg(referenceCell);

      for (int i = 3; i <= 5; ++i) {  // α, β, γ
         const bool crosses = (mobileDeg[i] < 90.0 && refDeg[i] > 90.0) ||
            (mobileDeg[i] > 90.0 && refDeg[i] < 90.0);
         if (crosses) return true;
      }
      return false;
   }

public:
   explicit SingularityFixedMatcher(const FixedMatchingOptions& options = FixedMatchingOptions())
      : m_options(options)
      , m_standardMatrices()
      , m_angleSpecificMatrices() {

      // Build standard matrix collection (your proven 78 matrices)
      ProductionLatticeMatcher standardMatcher;  // Use your existing collection
      // m_standardMatrices = standardMatcher.getAllMatrices(); // Need to expose this

      // Add angle-specific matrices for 90° crossings
      if (m_options.useAngleSpecificMatrices) {
         m_angleSpecificMatrices = generateAngleSpecificMatrices();
      }
   }

   /**
    * FIXED: Enhanced search with temporary worsening allowed
    */
   ProductionLatticeMatcher::MatchResult findMatchingPathFixed(const LRL_Cell& mobileCell,
      const LRL_Cell& referenceCell) const {
      ProductionLatticeMatcher::MatchResult result;

      LRL_Cell currentMobile = mobileCell;
      result.initialP3Distance = TransformerUtilities::getP3Distance(currentMobile, referenceCell);
      double currentDistance = result.initialP3Distance;
      double bestEverDistance = currentDistance;

      Matrix_3x3 cumulativeTransform;
      cumulativeTransform.UnitMatrix();

      int stepsWithoutImprovement = 0;
      int consecutiveWorseningSteps = 0;

      if (m_options.showProgress) {
         std::cout << "=== SINGULARITY-FIXED LATTICE MATCHING ===" << std::endl;
         std::cout << "Initial P3: " << currentDistance << std::endl;
         std::cout << "Needs 90° crossing: " << (needsCrossing90(mobileCell, referenceCell) ? "YES" : "NO") << std::endl;
      }

      for (int iteration = 1; iteration <= m_options.maxIterations; ++iteration) {
         Matrix_3x3 bestMatrix;
         bestMatrix.UnitMatrix();
         double bestDistance = currentDistance;
         bool foundAnyImprovement = false;
         bool foundMatrixToTest = false;

         // Combine standard and angle-specific matrices
         std::vector<Matrix_3x3> allMatrices = m_standardMatrices;
         if (needsCrossing90(currentMobile, referenceCell)) {
            allMatrices.insert(allMatrices.end(),
               m_angleSpecificMatrices.begin(),
               m_angleSpecificMatrices.end());
         }

         // Test all matrices
         for (const auto& matrix : allMatrices) {
            try {
               const LRL_Cell testMobile = matrix * currentMobile;
               const double testDistance = TransformerUtilities::getP3Distance(testMobile, referenceCell);
               foundMatrixToTest = true;

               // CRITICAL FIX: Allow temporary worsening for 90° crossings
               bool acceptMatrix = false;

               if (testDistance < bestDistance) {
                  // Standard improvement - always accept
                  acceptMatrix = true;
                  foundAnyImprovement = true;
               }
               else if (m_options.allowTemporaryWorsening &&
                  consecutiveWorseningSteps < m_options.worseningTolerance &&
                  testDistance < currentDistance * m_options.worseningThreshold) {
                  // Accept temporary worsening to escape 90° traps
                  acceptMatrix = true;
                  if (m_options.showProgress) {
                     std::cout << "  Accepting temporary worsening: " << testDistance
                        << " (step " << (consecutiveWorseningSteps + 1) << ")" << std::endl;
                  }
               }

               if (acceptMatrix && testDistance < bestDistance) {
                  bestMatrix = matrix;
                  bestDistance = testDistance;
               }
            }
            catch (...) {
               continue;
            }
         }

         if (!foundMatrixToTest) {
            if (m_options.showProgress) {
               std::cout << "No valid matrices to test at iteration " << iteration << std::endl;
            }
            break;
         }

         // Check if we should apply the best matrix found
         bool shouldApply = false;
         if (foundAnyImprovement) {
            shouldApply = true;
            consecutiveWorseningSteps = 0;
         }
         else if (m_options.allowTemporaryWorsening &&
            consecutiveWorseningSteps < m_options.worseningTolerance &&
            bestDistance < currentDistance * m_options.worseningThreshold) {
            shouldApply = true;
            consecutiveWorseningSteps++;
         }

         if (!shouldApply) {
            if (m_options.showProgress) {
               std::cout << "No acceptable move found at iteration " << iteration << std::endl;
            }
            stepsWithoutImprovement++;
            if (stepsWithoutImprovement >= 3) {  // Give up after 3 failed attempts
               break;
            }
            continue;
         }

         // Apply the transformation
         currentMobile = bestMatrix * currentMobile;
         currentDistance = bestDistance;
         cumulativeTransform = bestMatrix * cumulativeTransform;

         // Track best ever achieved
         if (currentDistance < bestEverDistance) {
            bestEverDistance = currentDistance;
            stepsWithoutImprovement = 0;
         }
         else {
            stepsWithoutImprovement++;
         }

         if (m_options.showProgress) {
            std::cout << "Step " << iteration << ": P3 = " << currentDistance;
            if (!foundAnyImprovement) {
               std::cout << " [TEMPORARY WORSENING]";
            }
            std::cout << std::endl;
         }

         // Check convergence
         if (currentDistance < m_options.convergenceThreshold) {
            result.success = true;
            result.stepsUsed = iteration;
            break;
         }

         result.stepsUsed = iteration;
      }

      result.finalP3Distance = currentDistance;
      result.cumulativeTransformation = cumulativeTransform;
      result.success = (result.finalP3Distance < controls.getSuccessThreshold());
      return result;
   }

   friend std::ostream& operator<<(std::ostream& os, const SingularityFixedMatcher& matcher) {
      os << "SingularityFixedMatcher:\n" << matcher.m_options;
      return os;
   }
};

#endif // SINGULARITY_FIXES_H

