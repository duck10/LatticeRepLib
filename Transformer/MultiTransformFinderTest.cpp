// GuidedMatrixCloud.cpp
// Navigate P3 space using iterative matrix transformations toward target

#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "Matrix_3x3.h"
#include "TransformerUtilities.h"
#include "TransformationMatrices.h"
#include "NiggliMatrices.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <algorithm>

class GuidedMatrixCloud {
private:
   struct CloudPoint {
      LRL_Cell cell;
      Matrix_3x3 totalTransformation;
      double distanceToReference;
      double distanceToLine;
      double distanceAlongLine;
      double distanceFromMobile;
      int iterationFound;
      std::string lastMatrixUsed;

      CloudPoint(const LRL_Cell& c, const Matrix_3x3& trans, double distRef, double distLine, double distAlong, double distMobile, int iter, const std::string& matrix)
         : cell(c), totalTransformation(trans), distanceToReference(distRef), distanceToLine(distLine), distanceAlongLine(distAlong), distanceFromMobile(distMobile), iterationFound(iter), lastMatrixUsed(matrix) {
      }
   };

   static std::vector<Matrix_3x3> getAllTransformationMatrices() {
      // NOW USE THE COMPLETE MATRIX SET including Niggli matrices!
      std::vector<Matrix_3x3> allMatrices;

      // Add all Niggli matrices AND their inverses (as in ProductionLatticeMatcher)
      std::cout << "Loading Niggli matrices and their inverses..." << std::endl;
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
      std::cout << "Niggli matrices (with inverses): " << allMatrices.size() << std::endl;

      // Add all 24 crystallographic transformations 
      const auto permutations = MatrixSets::getAllPermutationMatrices();
      std::cout << "Adding crystallographic matrices: " << permutations.size() << std::endl;
      for (const auto& matrix : permutations) {
         allMatrices.push_back(matrix);
      }

      // Add shear matrices with inverses
      std::cout << "Adding shear matrices with inverses..." << std::endl;
      size_t shearStart = allMatrices.size();
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
      std::cout << "Shear matrices (with inverses): " << (allMatrices.size() - shearStart) << std::endl;

      // Add complex matrices with inverses
      std::cout << "Adding complex matrices with inverses..." << std::endl;
      size_t complexStart = allMatrices.size();
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
      std::cout << "Complex matrices (with inverses): " << (allMatrices.size() - complexStart) << std::endl;

      // Add B4Matcher matrices if available
      // Note: These might be in a different header or namespace
      std::cout << "Total matrices loaded: " << allMatrices.size() << std::endl;

      if (allMatrices.size() >= 70) {
         std::cout << "SUCCESS: Using comprehensive matrix set!" << std::endl;
      }
      else {
         std::cout << "WARNING: Still missing some matrices (expected ~78, got " << allMatrices.size() << ")" << std::endl;
      }

      return allMatrices;
   }

   static double calculateDistanceAlongLine(const LRL_Cell& mobile, const LRL_Cell& reference, const LRL_Cell& point) {
      // Calculate how far along the mobile->reference line the point is
      // Returns 0.0 at mobile, 1.0 at reference, >1.0 past reference

      try {
         double totalDistance = TransformerUtilities::getP3Distance(mobile, reference);
         double pointToReference = TransformerUtilities::getP3Distance(point, reference);
         double mobileToPoint = TransformerUtilities::getP3Distance(mobile, point);

         if (totalDistance < 1e-10) return 0.0;  // mobile == reference

         // Use cosine rule to find projection
         double cosTheta = (totalDistance * totalDistance + mobileToPoint * mobileToPoint - pointToReference * pointToReference)
            / (2.0 * totalDistance * mobileToPoint);

         // Clamp cosine to valid range
         cosTheta = std::max(-1.0, std::min(1.0, cosTheta));

         double projectionDistance = mobileToPoint * cosTheta;
         return projectionDistance / totalDistance;

      }
      catch (...) {
         return -1.0;  // Error case
      }
   }

   static double calculateDistanceToLine(const LRL_Cell& mobile, const LRL_Cell& reference, const LRL_Cell& point) {
      // Calculate perpendicular distance from point to the mobile->reference line

      try {
         double totalDistance = TransformerUtilities::getP3Distance(mobile, reference);
         double pointToReference = TransformerUtilities::getP3Distance(point, reference);
         double mobileToPoint = TransformerUtilities::getP3Distance(mobile, point);

         if (totalDistance < 1e-10) return mobileToPoint;  // mobile == reference, return distance to mobile

         // Special case: if point is exactly mobile or reference, distance to line is 0
         if (mobileToPoint < 1e-10) return 0.0;  // point == mobile
         if (pointToReference < 1e-10) return 0.0;  // point == reference

         // Use triangle area method: Area = 0.5 * base * height
         // So height (perpendicular distance) = 2 * Area / base

         // Calculate area using Heron's formula
         double s = (totalDistance + pointToReference + mobileToPoint) / 2.0;  // semi-perimeter
         double area_squared = s * (s - totalDistance) * (s - pointToReference) * (s - mobileToPoint);

         // Check for degenerate triangle (points are collinear)
         if (area_squared <= 1e-20) {
            // Points are collinear, so perpendicular distance is 0
            return 0.0;
         }

         double area = std::sqrt(area_squared);
         double perpendicularDistance = (2.0 * area) / totalDistance;

         return perpendicularDistance;

      }
      catch (...) {
         return 1e10;  // Error case, return large distance
      }
   }

public:

   struct GuidedCloudResult {
      bool success;
      LRL_Cell finalCell;
      Matrix_3x3 totalTransformation;
      double finalDistance;
      int iterations;
      std::vector<CloudPoint> path;
      std::string summary;
   };

   /**
    * Apply guided matrix cloud algorithm with line-projection focus
    */
   static GuidedCloudResult guidedMatrixCloud(const LRL_Cell& mobile, const LRL_Cell& reference,
      double targetThreshold = 0.1, int maxIterations = 30) {

      std::cout << "=== GUIDED MATRIX CLOUD ALGORITHM (Line Projection Focus) ===" << std::endl;
      std::cout << "Mobile: " << LRL_Cell_Degrees(mobile) << std::endl;
      std::cout << "Reference: " << LRL_Cell_Degrees(reference) << std::endl;
      std::cout << "Initial P3 distance: " << std::fixed << std::setprecision(6)
         << TransformerUtilities::getP3Distance(mobile, reference) << std::endl;
      std::cout << "Strategy: Target line position 1.0 with bounded progress" << std::endl;
      std::cout << std::string(80, '-') << std::endl;

      GuidedCloudResult result;
      result.success = false;
      result.iterations = 0;

      auto allMatrices = getAllTransformationMatrices();
      std::cout << "Using " << allMatrices.size() << " transformation matrices" << std::endl;

      // Start with mobile cell
      LRL_Cell currentCell = mobile;
      Matrix_3x3 identityMatrix;
      identityMatrix.UnitMatrix();
      Matrix_3x3 totalTransformation = identityMatrix;

      double currentDistance = TransformerUtilities::getP3Distance(currentCell, reference);
      double currentLineDistance = calculateDistanceToLine(mobile, reference, currentCell);
      double currentLinePosition = calculateDistanceAlongLine(mobile, reference, currentCell);
      double currentMobileDistance = 0.0;  // Start at mobile

      // Track visited states to prevent oscillation
      std::vector<CloudPoint> visitedPoints;

      // Track the path
      result.path.push_back(CloudPoint(currentCell, totalTransformation, currentDistance, currentLineDistance, currentLinePosition, currentMobileDistance, 0, "START"));
      visitedPoints.push_back(result.path.back());

      for (int iteration = 1; iteration <= maxIterations; iteration++) {
         result.iterations = iteration;

         std::cout << "\n--- Iteration " << iteration << " ---" << std::endl;
         std::cout << "Current distance to reference: " << std::fixed << std::setprecision(6) << currentDistance << std::endl;
         std::cout << "Current line position: " << std::fixed << std::setprecision(3) << currentLinePosition << std::endl;
         std::cout << "Current distance from mobile: " << std::fixed << std::setprecision(6) << currentMobileDistance << std::endl;

         // Check if we've reached target - either close to reference OR close to line position 1.0
         if (currentDistance < targetThreshold ||
            (std::abs(currentLinePosition - 1.0) < 0.05 && currentDistance < 1.0)) {
            std::cout << "*** TARGET REACHED! ***" << std::endl;
            if (currentDistance < targetThreshold) {
               std::cout << "Success: Reference distance < " << targetThreshold << std::endl;
            }
            else {
               std::cout << "Success: Line position ≈ 1.0 and reasonable reference distance" << std::endl;
            }
            result.success = true;
            break;
         }

         // Collect all candidate points and sort them
         std::vector<CloudPoint> candidates;

         for (size_t i = 0; i < allMatrices.size(); i++) {
            try {
               LRL_Cell candidateCell = allMatrices[i] * currentCell;
               double candidateDistanceToRef = TransformerUtilities::getP3Distance(candidateCell, reference);
               double candidateDistanceToLine = calculateDistanceToLine(mobile, reference, candidateCell);
               double candidateLinePosition = calculateDistanceAlongLine(mobile, reference, candidateCell);
               double candidateMobileDistance = TransformerUtilities::getP3Distance(mobile, candidateCell);

               Matrix_3x3 newTotalTransformation = allMatrices[i] * totalTransformation;
               std::string matrixName = MatrixSets::getPermutationName(allMatrices[i]);

               candidates.push_back(CloudPoint(candidateCell, newTotalTransformation, candidateDistanceToRef,
                  candidateDistanceToLine, candidateLinePosition, candidateMobileDistance, iteration, matrixName));

            }
            catch (...) {
               // Skip invalid transformations
            }
         }

         // Sort candidates by proximity to target line position (1.0) with bounded progress
         std::sort(candidates.begin(), candidates.end(), [&currentLinePosition](const CloudPoint& a, const CloudPoint& b) {
            // Only consider candidates that make reasonable progress (bounded)
            const double maxAllowedPosition = std::max(currentLinePosition + 0.3, 1.2);  // Allow modest progress or slight overshoot
            const double minAllowedPosition = currentLinePosition - 0.1;  // Allow slight retreat

            // Filter out candidates that are too far
            bool aValid = (a.distanceAlongLine >= minAllowedPosition && a.distanceAlongLine <= maxAllowedPosition);
            bool bValid = (b.distanceAlongLine >= minAllowedPosition && b.distanceAlongLine <= maxAllowedPosition);

            if (aValid && !bValid) return true;   // a is valid, b is not
            if (!aValid && bValid) return false;  // b is valid, a is not
            if (!aValid && !bValid) {
               // Both invalid, pick the one closer to current position
               return std::abs(a.distanceAlongLine - currentLinePosition) < std::abs(b.distanceAlongLine - currentLinePosition);
            }

            // Both valid - prefer the one closer to target line position 1.0
            double aDistanceFromTarget = std::abs(a.distanceAlongLine - 1.0);
            double bDistanceFromTarget = std::abs(b.distanceAlongLine - 1.0);

            return aDistanceFromTarget < bDistanceFromTarget;
            });

         // Find the best candidate that we haven't visited recently
         CloudPoint bestPoint = candidates[0];  // This is now the one with maximum line position
         bool foundNewPoint = false;

         // Print top 5 candidates to see what we're choosing from
         const double maxAllowed = std::max(currentLinePosition + 0.3, 1.2);
         const double minAllowed = currentLinePosition - 0.1;

         std::cout << "Allowed range: [" << std::fixed << std::setprecision(3) << minAllowed
            << ", " << maxAllowed << "]" << std::endl;
         std::cout << "Top 5 candidates by proximity to target (line pos 1.0):" << std::endl;

         for (size_t i = 0; i < std::min(size_t(5), candidates.size()); i++) {
            bool inRange = (candidates[i].distanceAlongLine >= minAllowed &&
               candidates[i].distanceAlongLine <= maxAllowed);
            std::cout << "  " << i + 1 << ". pos=" << std::fixed << std::setprecision(3) << candidates[i].distanceAlongLine
               << " P3=" << std::setprecision(6) << candidates[i].distanceToReference
               << " target_dist=" << std::setprecision(3) << std::abs(candidates[i].distanceAlongLine - 1.0)
               << " " << (inRange ? "✓" : "✗")
               << " matrix=" << candidates[i].lastMatrixUsed << std::endl;
         }

         for (const auto& candidate : candidates) {
            // Check if this candidate is too similar to recent visited points
            bool tooSimilar = false;

            for (int checkBack = std::max(0, (int)visitedPoints.size() - 3); checkBack < (int)visitedPoints.size(); checkBack++) {
               double distanceToVisited = TransformerUtilities::getP3Distance(candidate.cell, visitedPoints[checkBack].cell);
               if (distanceToVisited < 0.001) {  // Very close to a recent point
                  tooSimilar = true;
                  break;
               }
            }

            if (!tooSimilar) {
               bestPoint = candidate;
               foundNewPoint = true;
               std::cout << "Selected: pos=" << std::fixed << std::setprecision(3) << bestPoint.distanceAlongLine
                  << " (target_dist=" << std::setprecision(3) << std::abs(bestPoint.distanceAlongLine - 1.0) << ")"
                  << " P3=" << std::setprecision(6) << bestPoint.distanceToReference
                  << " matrix=" << bestPoint.lastMatrixUsed << std::endl;
               break;
            }
         }

         if (!foundNewPoint) {
            std::cout << "All good candidates are too similar to recent points - taking best available" << std::endl;
            bestPoint = candidates[0];
         }

         // Update current state
         currentCell = bestPoint.cell;
         totalTransformation = bestPoint.totalTransformation;
         currentDistance = bestPoint.distanceToReference;
         currentLineDistance = bestPoint.distanceToLine;
         currentLinePosition = bestPoint.distanceAlongLine;
         currentMobileDistance = bestPoint.distanceFromMobile;

         result.path.push_back(bestPoint);
         visitedPoints.push_back(bestPoint);

         std::cout << "Best matrix: " << bestPoint.lastMatrixUsed << std::endl;
         std::cout << "New distance to reference: " << std::fixed << std::setprecision(6) << currentDistance << std::endl;
         std::cout << "New line position: " << std::fixed << std::setprecision(3) << currentLinePosition << std::endl;
         std::cout << "New distance from mobile: " << std::fixed << std::setprecision(6) << currentMobileDistance << std::endl;

         // Check for progress - if we're not making any progress, try more aggressive exploration
         if (iteration > 5 && currentDistance > result.path[iteration - 3].distanceToReference) {
            std::cout << "No progress in recent iterations - may need different approach" << std::endl;
            if (iteration > 10) break;  // Stop after reasonable attempt
         }
      }

      result.finalCell = currentCell;
      result.totalTransformation = totalTransformation;
      result.finalDistance = currentDistance;

      // Generate summary
      std::ostringstream summary;
      summary << "Line projection guided cloud completed in " << result.iterations << " iterations. ";
      if (result.success) {
         summary << "SUCCESS: Reached target";
      }
      else {
         summary << "Best result: ref distance " << std::fixed << std::setprecision(6) << currentDistance
            << ", line position " << currentLinePosition;
      }
      result.summary = summary.str();

      return result;
   }

   /**
    * Print detailed results
    */
   static void printResults(const GuidedCloudResult& result) {
      std::cout << "\n" << std::string(80, '=') << std::endl;
      std::cout << "GUIDED MATRIX CLOUD RESULTS" << std::endl;
      std::cout << std::string(80, '=') << std::endl;

      std::cout << "\nSummary: " << result.summary << std::endl;

      if (result.success) {
         std::cout << "\n*** SOLUTION FOUND! ***" << std::endl;
         std::cout << "Final cell: " << LRL_Cell_Degrees(result.finalCell) << std::endl;
         std::cout << "Total transformation matrix:" << std::endl;
         std::cout << result.totalTransformation << std::endl;
      }
      else {
         std::cout << "\nBest result achieved:" << std::endl;
         std::cout << "Final cell: " << LRL_Cell_Degrees(result.finalCell) << std::endl;
         std::cout << "Final distance: " << std::fixed << std::setprecision(6) << result.finalDistance << std::endl;
         std::cout << "Improvement: " << std::fixed << std::setprecision(1)
            << (100.0 * (1.0 - result.finalDistance / result.path[0].distanceToReference)) << "%" << std::endl;
      }

      std::cout << "\nPath summary:" << std::endl;
      for (size_t i = 0; i < result.path.size(); i++) {
         const auto& point = result.path[i];
         std::cout << "Step " << std::setw(2) << i << ": "
            << std::setw(25) << point.lastMatrixUsed
            << " → P3=" << std::fixed << std::setprecision(6) << point.distanceToReference
            << " line=" << std::setprecision(6) << point.distanceToLine
            << " mobile=" << std::setprecision(6) << point.distanceFromMobile
            << " pos=" << std::setprecision(3) << point.distanceAlongLine << std::endl;
      }
   }
};

int main() {
   try {
      // Test multiple difficult cases
      std::vector<std::pair<std::string, std::string>> testCases = {
         // Case 1: All-60° (already solved)
         {"9.023 9.028 9.029 60.018 60.022 60.018", "9.030 9.030 9.030 120.000 120.000 90.000"},

         // Case 2: Near-cubic with small distortions
         {"8.999 9.001 8.998 89.95 90.05 89.98", "9.000 9.000 9.000 90.000 90.000 90.000"},

         // Case 3: 60°/120° boundary case
         {"10.1 10.2 10.3 60.1 119.9 60.2", "10.0 10.0 10.0 120.0 120.0 90.0"},

         // Case 4: Rhombohedral edge case
         {"7.071 7.071 7.071 60.05 60.05 60.05", "10.0 10.0 5.0 90.0 90.0 120.0"}
      };

      for (size_t i = 0; i < testCases.size(); i++) {
         std::cout << "\n" << std::string(100, '=') << std::endl;
         std::cout << "TESTING CASE " << (i + 1) << std::endl;
         std::cout << std::string(100, '=') << std::endl;

         const LRL_Cell mobileCell(testCases[i].first);
         const LRL_Cell referenceCell(testCases[i].second);

         auto result = GuidedMatrixCloud::guidedMatrixCloud(mobileCell, referenceCell, 0.1, 15);
         GuidedMatrixCloud::printResults(result);

         std::cout << "\n" << std::string(50, '-') << " CASE " << (i + 1) << " SUMMARY " << std::string(50, '-') << std::endl;
         if (result.success) {
            std::cout << "✓ SOLVED: P3 = " << std::fixed << std::setprecision(6) << result.finalDistance << std::endl;
            std::cout << "Final transformation matrix:" << std::endl << result.totalTransformation << std::endl;
         }
         else {
            std::cout << "✗ FAILED: Best P3 = " << std::fixed << std::setprecision(6) << result.finalDistance << std::endl;
            std::cout << "Improvement: " << std::fixed << std::setprecision(1)
               << (100.0 * (1.0 - result.finalDistance / result.path[0].distanceToReference)) << "%" << std::endl;
         }
      }

      std::cout << "\n" << std::string(100, '=') << std::endl;
      std::cout << "OVERALL ALGORITHM ASSESSMENT" << std::endl;
      std::cout << std::string(100, '=') << std::endl;

      int solved = 0;
      for (size_t i = 0; i < testCases.size(); i++) {
         const LRL_Cell mobileCell(testCases[i].first);
         const LRL_Cell referenceCell(testCases[i].second);

         auto result = GuidedMatrixCloud::guidedMatrixCloud(mobileCell, referenceCell, 0.1, 15);
         if (result.success) solved++;
      }

      std::cout << "Cases solved: " << solved << "/" << testCases.size() << std::endl;
      std::cout << "Success rate: " << std::fixed << std::setprecision(1)
         << (100.0 * solved / testCases.size()) << "%" << std::endl;

      if (solved == testCases.size()) {
         std::cout << "🎉 ALGORITHM APPEARS ROBUST across multiple boundary cases!" << std::endl;
      }
      else if (solved > 0) {
         std::cout << "⚠️  ALGORITHM shows promise but needs refinement for some cases" << std::endl;
      }
      else {
         std::cout << "❌ ALGORITHM needs fundamental improvements" << std::endl;
      }

   }
   catch (const std::exception& ex) {
      std::cerr << "Error: " << ex.what() << std::endl;
      return 1;
   }
   return 0;
}