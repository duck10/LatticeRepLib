// ProductionLatticeMatcher.cpp
#include "ProductionLatticeMatcher.h"
#include "NiggliMatrices.h"
#include "TransformationMatrices.h"
#include "MatrixSets_B4Matcher.h"
#include "P3.h"
#include "TransformerUtilities.h"
#include <algorithm>
#include <stdexcept>

ProductionLatticeMatcher::ProductionLatticeMatcher(const MultiTransformFinderControls& controls)
   : m_controls(controls)
   , m_allMatrices()
{
   buildMatrixCollection();
}

void ProductionLatticeMatcher::buildMatrixCollection() {
   m_allMatrices = getAllTransformationMatrices();

   if (m_controls.shouldShowDetails()) {
      std::cout << "Built matrix collection with " << getMatrixCount()
         << " transformation matrices" << std::endl;
   }
}

std::vector<Matrix_3x3> ProductionLatticeMatcher::getAllTransformationMatrices() const {
   std::vector<Matrix_3x3> allMatrices;

   // Add all Niggli matrices AND their inverses
   // This preserves your exact approach
   for (const auto& matrix : NiggliMatrices::ALL_NIGGLI_MATRICES) {
      allMatrices.push_back(matrix);

      try {
         const Matrix_3x3 inverse = matrix.Inverse();
         if (std::abs(inverse.Det()) > 1e-10) {
            if (!(matrix == inverse)) {  // Avoid duplicates for self-inverse matrices
               allMatrices.push_back(inverse);
            }
         }
      }
      catch (...) {
         continue;  // Skip matrices that can't be inverted
      }
   }

   // Add all 24 crystallographic transformations 
   const auto permutations = MatrixSets::getAllPermutationMatrices();
   for (const auto& matrix : permutations) {
      allMatrices.push_back(matrix);
   }

   // Add shear matrices with inverses
   for (const auto& matrix : MatrixSets::SHEARS) {
      allMatrices.push_back(matrix);

      try {
         const Matrix_3x3 inverse = matrix.Inverse();
         if (std::abs(inverse.Det()) > 1e-10) {
            if (!(matrix == inverse)) {
               allMatrices.push_back(inverse);
            }
         }
      }
      catch (...) {
         continue;
      }
   }

   // Add complex matrices with inverses
   for (const auto& matrix : MatrixSets::COMPLEX) {
      allMatrices.push_back(matrix);

      try {
         const Matrix_3x3 inverse = matrix.Inverse();
         if (std::abs(inverse.Det()) > 1e-10) {
            if (!(matrix == inverse)) {
               allMatrices.push_back(inverse);
            }
         }
      }
      catch (...) {
         continue;
      }
   }

   // Add B4Matcher matrices (these don't need inverses attempted)
   allMatrices.push_back(MatrixSets::B4_CRITICAL);
   allMatrices.push_back(MatrixSets::B4_SHEAR_XY_NEG_XZ);
   allMatrices.push_back(MatrixSets::B4_NEG_XY);
   allMatrices.push_back(MatrixSets::B4_NEG_XZ);
   allMatrices.push_back(MatrixSets::B4_NEG_YZ);

   return allMatrices;
}

double ProductionLatticeMatcher::calculateP3Distance(const LRL_Cell& cell1,
   const LRL_Cell& cell2) const {
   const P3 p3_1(cell1);
   const P3 p3_2(cell2);
   return (p3_1 - p3_2).norm();
}

Matrix_3x3 ProductionLatticeMatcher::findBestMatrixForStep(const LRL_Cell& currentMobile,
   const LRL_Cell& referenceCell,
   double& bestDistance) const {
   Matrix_3x3 bestMatrix;
   bestMatrix.UnitMatrix();  // Identity as fallback
   bestDistance = calculateP3Distance(currentMobile, referenceCell);
   bool foundImprovement = false;

   // Test all matrices - this is your proven exhaustive approach
   for (const auto& matrix : m_allMatrices) {
      try {
         const LRL_Cell testMobile = applyMatrixToCell(currentMobile, matrix);
         const double testDistance = calculateP3Distance(testMobile, referenceCell);

         if (testDistance < bestDistance) {
            bestMatrix = matrix;
            bestDistance = testDistance;
            foundImprovement = true;
         }
      }
      catch (const std::exception& e) {
         continue;  // Skip problematic matrices
      }
   }

   return bestMatrix;
}

ProductionLatticeMatcher::MatchResult
ProductionLatticeMatcher::findMatchingPath(const LRL_Cell& mobileCell,
   const LRL_Cell& referenceCell) const {
   MatchResult result;

   // Initialize current state
   LRL_Cell currentMobile = mobileCell;
   result.initialP3Distance = calculateP3Distance(currentMobile, referenceCell);
   double currentDistance = result.initialP3Distance;

   // Initialize cumulative transformation to identity
   result.cumulativeTransformation.UnitMatrix();

   if (m_controls.showProgress()) {
      std::cout << "=== LATTICE MATCHING EXPERIMENT ===" << std::endl;
      std::cout << "Mobile cell: " << LRL_Cell_Degrees(mobileCell) << std::endl;
      std::cout << "Reference cell: " << LRL_Cell_Degrees(referenceCell) << std::endl;
      std::cout << "Initial P3 distance: " << currentDistance << std::endl;
      std::cout << "Testing " << getMatrixCount() << " transformation matrices per iteration" << std::endl;
      std::cout << std::endl;
   }

   // Record initial state if requested
   if (m_controls.includeStepHistory()) {
      MatchingStep initialStep;
      initialStep.mobileCell = currentMobile;
      initialStep.referenceCell = referenceCell;
      initialStep.appliedMatrix.UnitMatrix();  // Identity matrix
      initialStep.p3Distance = currentDistance;
      initialStep.stepNumber = 0;
      result.matchingPath.push_back(initialStep);
   }

   // Iterative improvement - exactly your algorithm
   for (int iteration = 1; iteration <= m_controls.getMaxIterations(); ++iteration) {
      double bestDistance = currentDistance;
      const Matrix_3x3 bestMatrix = findBestMatrixForStep(currentMobile, referenceCell, bestDistance);

      // Check if we found improvement
      if (bestDistance >= currentDistance) {
         if (m_controls.showProgress()) {
            std::cout << "No improvement found at iteration " << iteration << ". Stopping." << std::endl;
         }
         result.stepsUsed = iteration - 1;
         break;
      }

      // Apply the best transformation
      currentMobile = applyMatrixToCell(currentMobile, bestMatrix);
      currentDistance = bestDistance;

      // Update cumulative transformation
      result.cumulativeTransformation = bestMatrix * result.cumulativeTransformation;

      // Record this step if requested
      if (m_controls.includeStepHistory()) {
         MatchingStep step;
         step.mobileCell = currentMobile;
         step.referenceCell = referenceCell;
         step.appliedMatrix = bestMatrix;
         step.p3Distance = currentDistance;
         step.stepNumber = iteration;
         result.matchingPath.push_back(step);
      }

      // Show progress - matching your output style
      if (m_controls.showProgress() &&
         (iteration % 10 == 0 || iteration <= 5 || currentDistance < 0.1)) {
         std::cout << "Step " << iteration << ": P3 distance = " << currentDistance << std::endl;
         if (m_controls.showDetailedSteps()) {
            std::cout << "  Transformed cell: " << LRL_Cell_Degrees(currentMobile) << std::endl;
         }
      }

      // Check convergence
      if (currentDistance < m_controls.getConvergenceThreshold()) {
         if (m_controls.showProgress()) {
            std::cout << "Cells matched! Final distance: " << currentDistance << std::endl;
         }
         result.success = true;
         result.stepsUsed = iteration;
         break;
      }

      // Update steps used
      result.stepsUsed = iteration;
   }

   // Set final results
   result.finalP3Distance = currentDistance;
   result.success = (result.finalP3Distance < m_controls.getSuccessThreshold());
   return result;
}

Matrix_3x3 ProductionLatticeMatcher::findTransformationMatrix(const LRL_Cell& mobileCell,
   const LRL_Cell& referenceCell) const {
   const MatchResult result = findMatchingPath(mobileCell, referenceCell);
   return result.cumulativeTransformation;
}

void ProductionLatticeMatcher::analyzeMatchingPath(const MatchResult& result) const {
   std::cout << "\n=== MATCHING PATH ANALYSIS ===" << std::endl;
   std::cout << "Total steps: " << result.stepsUsed << std::endl;

   if (result.stepsUsed == 0) {
      std::cout << "No matching steps taken." << std::endl;
      return;
   }

   const double improvement = result.getTotalImprovement();
   const double improvementPercent = result.getImprovementPercent();

   std::cout << "Initial P3 distance: " << result.initialP3Distance << std::endl;
   std::cout << "Final P3 distance: " << result.finalP3Distance << std::endl;
   std::cout << "Total improvement: " << improvement << " (" << improvementPercent << "%)" << std::endl;

   // Show key milestones with cell transformations
   if (!result.matchingPath.empty()) {
      std::cout << "\nKey milestones:" << std::endl;
      for (size_t i = 0; i < result.matchingPath.size(); ++i) {
         const auto& step = result.matchingPath[i];
         if (i < 5 || i % 10 == 0 || i >= result.matchingPath.size() - 3) {
            std::cout << "Step " << step.stepNumber << ": P3 = " << step.p3Distance;
            if (i > 0) {
               const double stepImprovement = result.matchingPath[i - 1].p3Distance - step.p3Distance;
               std::cout << " (improved by " << stepImprovement << ")";
            }
            std::cout << std::endl;
            std::cout << "  Mobile cell: " << LRL_Cell_Degrees(step.mobileCell) << std::endl;
         }
      }
   }
}

bool ProductionLatticeMatcher::validateMatrixCollection() const {
   const double epsilon = 1e-10;
   for (const auto& matrix : m_allMatrices) {
      const double det = matrix.Det();
      // Accept det = ±1 (proper and improper rotations)
      if (std::abs(std::abs(det) - 1.0) > epsilon) {
         return false;
      }
   }
   return true;
}

void ProductionLatticeMatcher::testMatchingExperiment(const LRL_Cell& mobileCell,
   const LRL_Cell& referenceCell) const {
   const MatchResult result = findMatchingPath(mobileCell, referenceCell);
   analyzeMatchingPath(result);
}