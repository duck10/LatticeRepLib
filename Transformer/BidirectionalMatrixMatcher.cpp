#include "BidirectionalMatrixMatcher.h"
#include "B4.h"
#include "P3.h"
#include "TransformerUtilities.h"

#include <algorithm>
#include <limits>

BidirectionalMatrixMatcher::BidirectionalMatrixMatcher(double threshold, int maxIter, 
   const MultiTransformFinderControls& controls)
   : convergenceThreshold(threshold)
   , maxIterations(maxIter) 
   , m_controls(controls)
{
   generateTransformationMatrices();
}

void BidirectionalMatrixMatcher::generateTransformationMatrices() {
   // Use a set to eliminate duplicates, then convert to vectors
   std::set<Matrix_3x3> uniqueMatrices;
   std::vector<std::pair<Matrix_3x3, std::string>> namedMatrices;

   // Add identity matrix
   uniqueMatrices.insert(MatrixSets::IDENTITY);
   namedMatrices.push_back({ MatrixSets::IDENTITY, "Identity" });

   // Add all 24 crystallographic transformation matrices
   std::vector<Matrix_3x3> crystallographic = MatrixSets::getAllPermutationMatrices();
   for (size_t i = 0; i < crystallographic.size(); ++i) {
      if (uniqueMatrices.insert(crystallographic[i]).second) {
         namedMatrices.push_back({ crystallographic[i], "Cryst_" + std::to_string(i) });
      }
   }

   // Add shear matrices
   std::vector<Matrix_3x3> shears = {
       MatrixSets::SHEAR_XY, MatrixSets::SHEAR_XZ, MatrixSets::SHEAR_YX,
       MatrixSets::SHEAR_YZ, MatrixSets::SHEAR_ZX, MatrixSets::SHEAR_ZY,
       MatrixSets::SHEAR_XY_NEG, MatrixSets::SHEAR_XZ_NEG, MatrixSets::SHEAR_YX_NEG,
       MatrixSets::SHEAR_YZ_NEG, MatrixSets::SHEAR_ZX_NEG, MatrixSets::SHEAR_ZY_NEG
   };

   std::vector<std::string> shearNames = {
       "Shear_XY", "Shear_XZ", "Shear_YX", "Shear_YZ", "Shear_ZX", "Shear_ZY",
       "Shear_XY_Neg", "Shear_XZ_Neg", "Shear_YX_Neg", "Shear_YZ_Neg", "Shear_ZX_Neg", "Shear_ZY_Neg"
   };

   for (size_t i = 0; i < shears.size(); ++i) {
      if (uniqueMatrices.insert(shears[i]).second) {
         namedMatrices.push_back({ shears[i], shearNames[i] });
      }
   }

   // Add complex matrices
   std::vector<Matrix_3x3> complex = {
       MatrixSets::COMPLEX_1, MatrixSets::COMPLEX_2
   };

   for (size_t i = 0; i < complex.size(); ++i) {
      if (uniqueMatrices.insert(complex[i]).second) {
         namedMatrices.push_back({ complex[i], "Complex_" + std::to_string(i) });
      }
   }

   // Add Niggli reduction matrices
   for (size_t i = 0; i < NiggliMatrices::ALL_NIGGLI_MATRICES.size(); ++i) {
      if (uniqueMatrices.insert(NiggliMatrices::ALL_NIGGLI_MATRICES[i]).second) {
         namedMatrices.push_back({ NiggliMatrices::ALL_NIGGLI_MATRICES[i], "Niggli_" + std::to_string(i) });
      }
   }

   // Add B4Matcher matrices
   std::vector<Matrix_3x3> b4Matrices = {
       MatrixSets::B4_CRITICAL, MatrixSets::B4_SHEAR_XY_NEG_XZ,
       MatrixSets::B4_NEG_XY, MatrixSets::B4_NEG_XZ, MatrixSets::B4_NEG_YZ
   };

   std::vector<std::string> b4Names = {
       "B4_Critical", "B4_Shear_XY_Neg_XZ", "B4_Neg_XY", "B4_Neg_XZ", "B4_Neg_YZ"
   };

   for (size_t i = 0; i < b4Matrices.size(); ++i) {
      if (uniqueMatrices.insert(b4Matrices[i]).second) {
         namedMatrices.push_back({ b4Matrices[i], b4Names[i] });
      }
   }

   // Convert to final vectors
   transformationMatrices.clear();
   transformationNames.clear();

   for (const auto& pair : namedMatrices) {
      // Verify determinant is +1 (valid transformation)
      if (std::abs(pair.first.Det() - 1.0) < 1e-10) {
         transformationMatrices.push_back(pair.first);
         transformationNames.push_back(pair.second);
      }
   }

   std::cout << "Generated " << transformationMatrices.size() << " unique transformation matrices" << std::endl;
}

LRL_Cell BidirectionalMatrixMatcher::applyTransformation(const LRL_Cell& cell, const Matrix_3x3& transform) const {
   // Convert to B4, apply transformation, convert back
   B4 b4Cell(cell);
   B4 transformedB4 = transform * b4Cell;
   return LRL_Cell(transformedB4);
}

std::pair<LRL_Cell, std::string> BidirectionalMatrixMatcher::findBestTransformation(
   const LRL_Cell& currentCell, const LRL_Cell& targetCell, double& bestDistance) const {

   LRL_Cell bestCell = currentCell;
   std::string bestName = "No_Improvement";
   bestDistance = std::numeric_limits<double>::max();

   for (size_t i = 0; i < transformationMatrices.size(); ++i) {
      try {
         LRL_Cell candidateCell = applyTransformation(currentCell, transformationMatrices[i]);
         double candidateDistance = calculateP3Distance(candidateCell, targetCell);

         if (candidateDistance < bestDistance) {
            bestDistance = candidateDistance;
            bestCell = candidateCell;
            bestName = transformationNames[i];
         }
      }
      catch (...) {
         // Skip problematic transformations
         continue;
      }
   }

   return std::make_pair(bestCell, bestName);
}

void BidirectionalMatrixMatcher::printStepInfo(const std::string& stepType, int iteration,
   const LRL_Cell& currentCell, double distance) const {
   LRL_Cell_Degrees cellDegrees(currentCell);
   std::cout << stepType << " STEP:" << std::endl;
   std::cout << "Current " << stepType << " cell: "
      << std::fixed << std::setprecision(3)
      << cellDegrees[0] << " " << cellDegrees[1] << " " << cellDegrees[2] << " "
      << cellDegrees[3] << " " << cellDegrees[4] << " " << cellDegrees[5] << std::endl;
   std::cout << "Distance: " << std::fixed << std::setprecision(6) << distance << std::endl;
}

BidirectionalResult BidirectionalMatrixMatcher::search(const LRL_Cell& mobileCell, const LRL_Cell& referenceCell) const {
   BidirectionalResult result;

   std::cout << "=== BIDIRECTIONAL GUIDED MATRIX CLOUD ALGORITHM ===" << std::endl;

   LRL_Cell_Degrees mobileDegrees(mobileCell);
   LRL_Cell_Degrees referenceDegrees(referenceCell);

   std::cout << "Mobile:     " << std::fixed << std::setprecision(3)
      << mobileDegrees[0] << " " << mobileDegrees[1] << " " << mobileDegrees[2] << " "
      << mobileDegrees[3] << " " << mobileDegrees[4] << " " << mobileDegrees[5] << std::endl;
   std::cout << "Reference:  " << std::fixed << std::setprecision(3)
      << referenceDegrees[0] << " " << referenceDegrees[1] << " " << referenceDegrees[2] << " "
      << referenceDegrees[3] << " " << referenceDegrees[4] << " " << referenceDegrees[5] << std::endl;

   double initialDistance = calculateP3Distance(mobileCell, referenceCell);
   std::cout << "Initial P3 distance: " << std::fixed << std::setprecision(6) << initialDistance << std::endl;
   std::cout << "Convergence threshold: " << convergenceThreshold << std::endl;
   std::cout << std::string(80, '-') << std::endl;
   std::cout << "Using " << transformationMatrices.size() << " transformation matrices" << std::endl;

   // Initialize search states
   LRL_Cell forwardCell = mobileCell;
   LRL_Cell backwardCell = referenceCell;
   double forwardDistanceToRef = initialDistance;
   double backwardDistanceToMobile = initialDistance;

   // Track paths
   result.forwardPath.push_back(SearchStep(0, "START", forwardCell, forwardDistanceToRef));
   result.backwardPath.push_back(SearchStep(0, "START", backwardCell, backwardDistanceToMobile));

   for (int iteration = 1; iteration <= maxIterations; ++iteration) {
      std::cout << "\n--- Iteration " << iteration << " ---" << std::endl;

      // Check convergence first
      double convergenceDistance = calculateP3Distance(forwardCell, backwardCell);
      std::cout << "Convergence distance: " << std::fixed << std::setprecision(6) << convergenceDistance << std::endl;

      if (convergenceDistance < convergenceThreshold) {
         std::cout << "*** CONVERGENCE ACHIEVED! ***" << std::endl;
         std::cout << "Forward and backward searches met within threshold" << std::endl;
         result.converged = true;
         result.finalForwardCell = forwardCell;
         result.finalBackwardCell = backwardCell;
         result.finalConvergenceDistance = convergenceDistance;
         result.iterations = iteration;
         return result;
      }

      // FORWARD STEP (Mobile side)
      std::cout << std::endl;
      printStepInfo("FORWARD", iteration, forwardCell, forwardDistanceToRef);

      double bestForwardDistance;
      auto forwardResult = findBestTransformation(forwardCell, referenceCell, bestForwardDistance);

      bool forwardImproved = (bestForwardDistance < forwardDistanceToRef);
      if (forwardImproved) {
         forwardCell = forwardResult.first;
         forwardDistanceToRef = bestForwardDistance;
         std::cout << "Forward improved: " << forwardResult.second
            << " -> P3=" << std::fixed << std::setprecision(6) << bestForwardDistance << std::endl;
         result.forwardPath.push_back(SearchStep(iteration, "F_" + forwardResult.second, forwardCell, forwardDistanceToRef));
      }
      else {
         std::cout << "Forward: No improvement found" << std::endl;
         result.forwardPath.push_back(SearchStep(iteration, "F_NO_IMPROVE", forwardCell, forwardDistanceToRef));
      }

      // BACKWARD STEP (Reference side)
      std::cout << std::endl;
      printStepInfo("BACKWARD", iteration, backwardCell, backwardDistanceToMobile);

      double bestBackwardDistance;
      auto backwardResult = findBestTransformation(backwardCell, mobileCell, bestBackwardDistance);

      bool backwardImproved = (bestBackwardDistance < backwardDistanceToMobile);
      if (backwardImproved) {
         backwardCell = backwardResult.first;
         backwardDistanceToMobile = bestBackwardDistance;
         std::cout << "Backward improved: " << backwardResult.second
            << " -> P3=" << std::fixed << std::setprecision(6) << bestBackwardDistance << std::endl;
         result.backwardPath.push_back(SearchStep(iteration, "B_" + backwardResult.second, backwardCell, backwardDistanceToMobile));
      }
      else {
         std::cout << "Backward: No improvement found" << std::endl;
         result.backwardPath.push_back(SearchStep(iteration, "B_NO_IMPROVE", backwardCell, backwardDistanceToMobile));
      }

      // *** STAGNATION CHECK ***
      if (!forwardImproved && !backwardImproved) {
         std::cout << "*** NO IMPROVEMENT IN EITHER DIRECTION - STOPPING ***" << std::endl;
         result.iterations = iteration;  // Set the actual iteration count
         break;
      }
   }

   std::cout << "\n*** MAXIMUM ITERATIONS REACHED ***" << std::endl;
   double finalConvergence = calculateP3Distance(forwardCell, backwardCell);
   std::cout << "Final convergence distance: " << std::fixed << std::setprecision(6) << finalConvergence << std::endl;
   std::cout << "Bidirectional search used " << result.iterations << " iterations" << std::endl;  // Use result.iterations
   std::cout << "Convergence distance: " << std::fixed << std::setprecision(6) << finalConvergence << std::endl;



   std::cout << "\n*** MAXIMUM ITERATIONS REACHED ***" << std::endl;
   std::cout << "Final convergence distance: " << std::fixed << std::setprecision(6) << finalConvergence << std::endl;

   result.converged = false;
   result.finalForwardCell = forwardCell;
   result.finalBackwardCell = backwardCell;
   result.finalConvergenceDistance = finalConvergence;
   result.iterations = maxIterations;

   return result;
}

bool BidirectionalMatrixMatcher::runTestCase(const std::string& caseName,
   const LRL_Cell& mobileCell,
   const LRL_Cell& referenceCell) const {
   std::cout << "\n" << std::string(80, '=') << std::endl;
   std::cout << "TESTING " << caseName << std::endl;
   std::cout << std::string(80, '=') << std::endl;

   BidirectionalResult result = search(mobileCell, referenceCell);
   printResult(caseName, result);

   return result.converged;
}

void BidirectionalMatrixMatcher::printResult(const std::string& caseName, const BidirectionalResult& result) const {
   std::cout << "\n" << std::string(80, '=') << std::endl;
   std::cout << "BIDIRECTIONAL SEARCH RESULTS - " << caseName << std::endl;
   std::cout << std::string(80, '=') << std::endl;

   if (result.converged) {
      std::cout << "*** SOLUTION FOUND! ***" << std::endl;
      std::cout << "Convergence distance: " << std::fixed << std::setprecision(6) << result.finalConvergenceDistance << std::endl;

      LRL_Cell_Degrees finalForwardDegrees(result.finalForwardCell);
      LRL_Cell_Degrees finalBackwardDegrees(result.finalBackwardCell);

      std::cout << "Final forward cell:  " << std::fixed << std::setprecision(3)
         << finalForwardDegrees[0] << " " << finalForwardDegrees[1] << " " << finalForwardDegrees[2] << " "
         << finalForwardDegrees[3] << " " << finalForwardDegrees[4] << " " << finalForwardDegrees[5] << std::endl;
      std::cout << "Final backward cell: " << std::fixed << std::setprecision(3)
         << finalBackwardDegrees[0] << " " << finalBackwardDegrees[1] << " " << finalBackwardDegrees[2] << " "
         << finalBackwardDegrees[3] << " " << finalBackwardDegrees[4] << " " << finalBackwardDegrees[5] << std::endl;
   }
   else {
      std::cout << "*** FAILED TO CONVERGE ***" << std::endl;
      std::cout << "Best convergence distance achieved: " << std::fixed << std::setprecision(6) << result.finalConvergenceDistance << std::endl;
   }

   std::cout << "\nForward path summary:" << std::endl;
   for (const auto& step : result.forwardPath) {
      std::cout << "  Step " << std::setw(2) << step.iteration << ": "
         << std::setw(20) << std::left << step.transformName
         << " P3=" << std::fixed << std::setprecision(6) << step.distance << std::endl;
   }

   std::cout << "\nBackward path summary:" << std::endl;
   for (const auto& step : result.backwardPath) {
      std::cout << "  Step " << std::setw(2) << step.iteration << ": "
         << std::setw(20) << std::left << step.transformName
         << " P3=" << std::fixed << std::setprecision(6) << step.distance << std::endl;
   }
}