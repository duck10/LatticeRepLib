#include "TransformerDisplay.h"
#include <iostream>
#include <iomanip>

#include "CS6Dist.h"
#include "CS6Dist.cpp"

TransformerDisplay::TransformerDisplay(const MultiTransformFinderControls& controls)
   : m_controls(controls) {
}

void TransformerDisplay::showInputCells(const LatticeCell& sourceCell, const LatticeCell& targetCell) const {
   LRL_Cell_Degrees sourceCellDeg(sourceCell.getCell());
   LRL_Cell_Degrees targetCellDeg(targetCell.getCell());
   std::cout << "Cell to Transform (Degrees): " << sourceCellDeg << std::endl;
   std::cout << "Reference Cell (Degrees):    " << targetCellDeg << std::endl;
}

void TransformerDisplay::displayTransformResult(
   const B4Matcher::TransformResult& result,
   const LRL_Cell& sourceCell,
   const LRL_Cell& targetCell) const {

   // Basic output that always shows
   std::cout << "Matrix:" << std::endl;
   std::cout << result.transformMatrix << std::endl;

   // Convert to cell parameters for display
   LatticeCell transformedCell(result.transformedB4);
   LRL_Cell transformedRawCell = transformedCell.getCell();
   LRL_Cell_Degrees transformedCellDeg(transformedRawCell);
   LRL_Cell_Degrees targetCellDeg(targetCell);

   std::cout << "Transformed Cell: " << transformedCellDeg << std::endl;
   std::cout << "Reference Cell:   " << targetCellDeg << std::endl;

   // Calculate metrics
   S6 transformedS6 = S6(transformedRawCell);
   S6 targetS6 = S6(targetCell);
   double s6Angle = angleS6(transformedS6, targetS6);

   P3 transformedP3(transformedRawCell);
   P3 targetP3(targetCell);
   double p3Distance = P3::DistanceBetween(transformedP3, targetP3);

   // Calculate parameter differences
   double paramDiffs[6] = {
       std::abs(transformedCellDeg[0] - targetCellDeg[0]),
       std::abs(transformedCellDeg[1] - targetCellDeg[1]),
       std::abs(transformedCellDeg[2] - targetCellDeg[2]),
       std::abs(transformedCellDeg[3] - targetCellDeg[3]),
       std::abs(transformedCellDeg[4] - targetCellDeg[4]),
       std::abs(transformedCellDeg[5] - targetCellDeg[5])
   };

   double totalParamDiff =
      paramDiffs[0] + paramDiffs[1] + paramDiffs[2] +
      paramDiffs[3] / 10.0 + paramDiffs[4] / 10.0 + paramDiffs[5] / 10.0;

   // Basic metrics that always show
   std::cout << "\nMetrics Summary:" << std::endl;
   std::cout << "  B4 Distance:    " << result.b4Distance << std::endl;

   // Changed from Param Diff to P3 Distance and S6 Angle in compact mode
   if (!m_controls.shouldShowDetails()) {
      std::cout << "  P3 Distance:    " << p3Distance << std::endl;
      std::cout << "  S6 Angle:       " << s6Angle << " degrees" << std::endl;
   }
   else {
      // In detailed mode, show all metrics
      std::cout << "  Param Diff:     " << totalParamDiff << std::endl;
      std::cout << "  P3 Distance:    " << p3Distance << std::endl;
      std::cout << "  S6 Angle:       " << s6Angle << " degrees" << std::endl;

      // Detailed parameter comparison
      std::cout << "\nParameter matches:" << std::endl;
      std::cout << "  a: " << transformedCellDeg[0] << " vs " << targetCellDeg[0]
         << " (diff: " << paramDiffs[0] << ")" << std::endl;
      std::cout << "  b: " << transformedCellDeg[1] << " vs " << targetCellDeg[1]
         << " (diff: " << paramDiffs[1] << ")" << std::endl;
      std::cout << "  c: " << transformedCellDeg[2] << " vs " << targetCellDeg[2]
         << " (diff: " << paramDiffs[2] << ")" << std::endl;
      std::cout << "  alpha: " << transformedCellDeg[3] << " vs " << targetCellDeg[3]
         << " (diff: " << paramDiffs[3] << ")" << std::endl;
      std::cout << "  beta: " << transformedCellDeg[4] << " vs " << targetCellDeg[4]
         << " (diff: " << paramDiffs[4] << ")" << std::endl;
      std::cout << "  gamma: " << transformedCellDeg[5] << " vs " << targetCellDeg[5]
         << " (diff: " << paramDiffs[5] << ")" << std::endl;
   }
}

void TransformerDisplay::showB4MatcherResults(const B4Matcher& matcher,
   const LatticeCell& sourceCell,
   const LatticeCell& targetCell) const {
   if (!m_controls.shouldShowDetails()) return;

   // Display B4 best transformation
   std::cout << "Finding best transformation using B4 distance..." << std::endl;
   B4Matcher::TransformResult bestTransformResult = matcher.findBestTransformation();
   std::cout << "Best transformation matrix:" << std::endl;
   std::cout << bestTransformResult.transformMatrix << std::endl;

   // Test specific matrices using TransformerUtilities functions
   std::cout << "Testing specific transformation matrices using B4..." << std::endl;
   std::cout << "Testing 3 specific transformation matrices..." << std::endl;
   LRL_Cell_Degrees sourceCellDeg(sourceCell.getCell());
   LRL_Cell_Degrees targetCellDeg(targetCell.getCell());
   std::cout << "Cell to Transform:" << std::endl;
   std::cout << "            " << sourceCellDeg << std::endl;
   std::cout << "Reference Cell:   " << std::endl;
   std::cout << "            " << targetCellDeg << std::endl;

   // Use testSpecificMatricesWithB4 from TransformerUtilities.h
   testSpecificMatricesWithB4(sourceCell.getCell(), targetCell.getCell());
}

void TransformerDisplay::showB4Transformations(const B4Matcher& matcher,
   const LatticeCell& sourceCell,
   const LatticeCell& targetCell) const {
   if (!m_controls.shouldShowDetails()) return;

   // Find and display B4-optimized transformations
   LRL_Cell_Degrees sourceCellDeg(sourceCell.getCell());
   LRL_Cell_Degrees targetCellDeg(targetCell.getCell());

   std::cout << "Finding transformations using direct B4 comparison..." << std::endl;
   std::cout << "Cell to Transform: " << sourceCellDeg << std::endl;
   std::cout << "Reference Cell:    " << targetCellDeg << std::endl;
   std::cout << "Cell to Transform B4: " << matcher.getSourceB4() << std::endl;
   std::cout << "Reference Cell B4:    " << matcher.getTargetB4() << std::endl;

   auto b4Transformations = matcher.findMultipleTransformations(5);
   std::cout << "Best transformations (sorted by B4 distance):" << std::endl;
   for (size_t i = 0; i < b4Transformations.size(); ++i) {
      std::cout << "Matrix " << (i + 1) << " (B4 Distance = " << b4Transformations[i].b4Distance << "):" << std::endl;
      displayTransformResult(b4Transformations[i], sourceCell.getCell(), targetCell.getCell());
      std::cout << "--------------------------" << std::endl;
   }
}

void TransformerDisplay::showParameterTransformations(
   const std::vector<B4Matcher::TransformResult>& paramTransformations,
   const LatticeCell& sourceCell,
   const LatticeCell& targetCell,
   int duplicatesRemoved) const {

   // Check if we have any transformations
   if (paramTransformations.empty()) {
      std::cout << "\nNo valid transformations found." << std::endl;
      return;
   }

   // Only show header in detailed mode
   if (m_controls.shouldShowDetails()) {
      std::cout << "\nFinding transformations optimized for parameter matching..." << std::endl;

      // Only show duplicate count in detailed mode
      if (duplicatesRemoved > 0) {
         std::cout << "Found " << (paramTransformations.size() + duplicatesRemoved)
            << " transformations, removed "
            << duplicatesRemoved
            << " duplicates." << std::endl;
      }

      std::cout << "Best transformations (sorted by parameter match):" << std::endl;
   }

   // The key fix: calculate how many transformations to show based on mode
   size_t numToShow;
   if (m_controls.shouldShowDetails()) {
      // In detailed mode, show all transformations
      numToShow = paramTransformations.size();
   }
   else {
      // In compact mode, show up to 3 transformations
      numToShow = std::min(size_t(3), paramTransformations.size());
   }

   // Debug output - TEMPORARY
   if (m_controls.shouldShowDetails()) {
      std::cout << "Debug: Showing " << numToShow << " of " << paramTransformations.size() << " transformations in detailed mode" << std::endl;
   }

   for (size_t i = 0; i < numToShow; ++i) {
      // In compact mode, show a matrix number for each result
      if (!m_controls.shouldShowDetails()) {
         std::cout << "\n=================================\n";
         if (numToShow > 1) {
            std::cout << "Matrix " << (i + 1) << " of " << numToShow << ":" << std::endl;
         }
      }
      else {
         std::cout << "\n=================================\n";
         std::cout << "Matrix " << (i + 1) << ":" << std::endl;
      }

      displayTransformResult(paramTransformations[i], sourceCell.getCell(), targetCell.getCell());
      std::cout << "=================================\n";
   }
}

// Add to your existing TransformerDisplay.cpp file









// New methods to add to your TransformerDisplay.cpp file

// Add these methods to your TransformerDisplay.cpp file

// Display transformation results with centering
void TransformerDisplay::showParameterTransformationsWithCentering(
   const std::vector<B4Matcher::TransformResult>& paramTransformations,
   const LatticeCell& sourceCell,
   const LatticeCell& targetCell,
   int duplicatesRemoved,
   const Matrix_3x3& inputToPrimitiveMatrix,
   const Matrix_3x3& referenceToPrimitiveMatrix) const {

   std::cout << "\nFinding transformations optimized for parameter matching..." << std::endl;
   std::cout << "Best transformations (sorted by parameter match):" << std::endl;

   const size_t count = paramTransformations.size();
   const size_t displayCount = m_controls.shouldShowDetails() ?
      count : std::min(size_t(3), count);

   if (m_controls.shouldShowDetails()) {
      std::cout << "Debug: Showing " << displayCount << " of " << (count + duplicatesRemoved)
         << " transformations in detailed mode" << std::endl << std::endl;
   }

   for (size_t i = 0; i < displayCount; ++i) {
      std::cout << "=================================" << std::endl;

      std::cout << "Matrix " << (i + 1);
      if (!m_controls.shouldShowDetails()) {
         std::cout << " of " << displayCount;
      }
      std::cout << ":" << std::endl;

      // Call the display method with the individual matrices
      displayCenteredTransformResult(
         paramTransformations[i],
         sourceCell,
         targetCell,
         inputToPrimitiveMatrix,
         paramTransformations[i].transformMatrix,
         referenceToPrimitiveMatrix);

      std::cout << "=================================" << std::endl << std::endl;
   }
}

// Display a single transformation with centering
void TransformerDisplay::displayCenteredTransformResult(
   const B4Matcher::TransformResult& result,
   const LatticeCell& cellToTransform,
   const LatticeCell& referenceCell,
   const Matrix_3x3& inputToPrimitiveMatrix,
   const Matrix_3x3& primitiveTransformMatrix,
   const Matrix_3x3& referenceToPrimitiveMatrix) const {

   // Show the primitive-to-primitive transformation matrix
   std::cout << "Matrix (primitive-to-primitive):" << std::endl;
   std::cout << primitiveTransformMatrix << std::endl;

   // Calculate the complete transformation matrix:
   // Complete = ReferenceToStandard^-1 * PrimitiveTransformation * InputToStandard
   Matrix_3x3 completeMatrix =
      referenceToPrimitiveMatrix.Inver() *
      primitiveTransformMatrix *
      inputToPrimitiveMatrix;

   // In detailed mode, also show the complete matrix
   if (m_controls.shouldShowDetails()) {
      std::cout << "Complete Matrix (with centering):" << std::endl;
      std::cout << completeMatrix << std::endl;
   }

   // Apply the complete transformation to the original cell
   LRL_Cell transformedCell = completeMatrix * cellToTransform.getCell();

   // Display the cells
   std::cout << "Transformed Cell: " << LRL_Cell_Degrees(transformedCell) << std::endl;
   std::cout << "Reference Cell:   " << LRL_Cell_Degrees(referenceCell.getCell()) << std::endl;

   // Calculate metrics between the transformed cell and reference cell

   // Calculate S6 metrics
   S6 transformedS6(transformedCell);
   S6 referenceS6(referenceCell.getCell());
   double s6Angle = angleS6(transformedS6, referenceS6);

   // Calculate P3 metrics
   P3 transformedP3(transformedCell);
   P3 referenceP3(referenceCell.getCell());
   double p3Distance = P3::DistanceBetween(transformedP3, referenceP3);

   // Calculate CS6 distance
   double cs6Distance = CS6Dist(S6(transformedCell).data(), S6(referenceCell.getCell()).data());

   // Display metrics
   std::cout << "Metrics Summary:" << std::endl;
   std::cout << "  B4 Distance:    " << result.b4Distance << std::endl;
   std::cout << "  P3 Distance:    " << p3Distance << std::endl;
   std::cout << "  S6 Angle:       " << s6Angle << " degrees" << std::endl;
   std::cout << "  CS6 Distance:   " << cs6Distance << std::endl;

   // Show parameter differences in detailed mode
   if (m_controls.shouldShowDetails()) {
      const LRL_Cell_Degrees transformedDegrees(transformedCell);
      const LRL_Cell_Degrees referenceDegrees(referenceCell.getCell());

      std::cout << std::endl << "Parameter matches:" << std::endl;

      for (size_t i = 0; i < 6; ++i) {
         std::string label;
         switch (i) {
         case 0: label = "a"; break;
         case 1: label = "b"; break;
         case 2: label = "c"; break;
         case 3: label = "alpha"; break;
         case 4: label = "beta"; break;
         case 5: label = "gamma"; break;
         }

         const double diff = fabs(transformedDegrees[i] - referenceDegrees[i]);
         std::cout << "  " << label << ": " << transformedDegrees[i]
            << " vs " << referenceDegrees[i]
               << " (diff: " << diff << ")" << std::endl;
      }
   }
}