#include "TransformerDisplay.h"
#include "TransformerUtilities.h"

#include <iostream>
#include <iomanip>

#include "CS6Dist.h"
#include "CS6Dist.cpp"

TransformerDisplay::TransformerDisplay(const MultiTransformFinderControls& controls)
   : m_controls(controls) {
}

void TransformerDisplay::showInputCells(const LatticeCell& sourceCell, const LatticeCell& referenceCell) const {
   LRL_Cell_Degrees sourceCellDeg(sourceCell.getCell());
   LRL_Cell_Degrees referenceCellDeg(referenceCell.getCell());
   std::cout << "Cell to Transform (Degrees): " << sourceCell.getLatticeType() << " "
      << sourceCellDeg << std::endl;
   std::cout << "Reference Cell (Degrees):    " << referenceCell.getLatticeType() << " "
      << referenceCellDeg << std::endl;
}

// Display a single transformation with complete centering transformation
void TransformerDisplay::displayCompleteTransformResult(
   const B4Matcher::TransformResult& result,
   const LatticeCell& cellToTransform,
   const LatticeCell& referenceCell) const {

   // Show the primitive-to-primitive transformation matrix
   std::cout << "Matrix (primitive-to-primitive):" << std::endl;
   std::cout << result.transformMatrix << std::endl;

   // In detailed mode, also show the complete matrix
      std::cout << "Complete Matrix (with centering):" << std::endl;
      std::cout << result.completeTransformMatrix << std::endl;

   // Display the cells
   std::cout << "Transformed Cell: " << referenceCell.getLatticeType() << " "
      << LRL_Cell_Degrees(result.transformedCompleteCell) << std::endl;
   std::cout << "Reference Cell:   " << referenceCell.getLatticeType() << " "
      << LRL_Cell_Degrees(referenceCell.getCell()) << std::endl;

   // Display metrics
   std::cout << "Metrics Summary:" << std::endl;

   // Handle P3 distance
   if (result.p3DistanceFinal != 19191.0) {
      std::cout << "  P3 Distance:    " << result.p3DistanceFinal << " A" << std::endl;
   }
   else {
      std::cout << "  P3 Distance:    (Not calculated)" << std::endl;
   }

   // Handle S6 angle
   if (result.s6AngleFinal != 19191.0) {
      std::cout << "  S6 Angle:       " << result.s6AngleFinal << " degrees" << std::endl;
   }
   else {
      std::cout << "  S6 Angle:       (Not calculated)" << std::endl;
   }


   // Show parameter differences in detailed mode
   if (m_controls.shouldShowDetails()) {
      const LRL_Cell_Degrees transformedDegrees(result.transformedCompleteCell);
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

// Display transformation results for cells with complete centering
void TransformerDisplay::showCompleteTransformations(
   const CompleteTransformationResult& result,
   const LatticeCell& sourceCell,
   const LatticeCell& referenceCell) const {

   // Check if we have any transformations
   if (!result.primitiveResult.isValid || result.primitiveResult.transformations.empty()) {
      std::cout << "\nNo valid transformations found." << std::endl;
      return;
   }

   // Show transformation count in detailed mode
   if (m_controls.shouldShowDetails()) {
      std::cout << "\nFound " << result.primitiveResult.transformations.size()
         << " valid transformations";

      if (result.primitiveResult.duplicatesRemoved > 0) {
         std::cout << ", removed " << result.primitiveResult.duplicatesRemoved
            << " duplicates";
      }

      std::cout << "." << std::endl;

      // Show centering types
      if (result.hasCentering) {
         std::cout << "Input centering: "
            << (result.inputCenteringType.empty() ? "P" : result.inputCenteringType)
            << ", Reference centering: "
            << (result.referenceCenteringType.empty() ? "P" : result.referenceCenteringType)
            << std::endl;

         // In detailed mode, also show the transformation matrices
         std::cout << "Input-to-primitive matrix:" << std::endl;
         std::cout << result.inputToPrimitiveMatrix << std::endl;
         std::cout << "Reference-to-primitive matrix:" << std::endl;
         std::cout << result.referenceToPrimitiveMatrix << std::endl;
      }
   }

   // Always show this header
   std::cout << "\nBest transformations (sorted by "
      << (m_controls.getSortBy() == "P3" ? "P3 distance" : "S6 angle")
      << "):" << std::endl;

   // Determine number of transformations to show
   const size_t displayCount = result.primitiveResult.transformations.size();

   // Show the best transformations
   for (size_t i = 0; i < displayCount; ++i) {
      std::cout << "=================================" << std::endl;

      if (displayCount > 1) {
         std::cout << "Matrix " << (i + 1);
         if (!m_controls.shouldShowDetails()) {
            std::cout << " of " << displayCount;
         }
         std::cout << ":" << std::endl;
      }

      // Display the transformation
      displayCompleteTransformResult(
         result.primitiveResult.transformations[i],
         sourceCell,
         referenceCell);

      std::cout << "=================================" << std::endl << std::endl;
   }
}

void TransformerDisplay::showB4MatcherResults(const B4Matcher& matcher,
   const LatticeCell& sourceCell,
   const LatticeCell& referenceCell) const {
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
   LRL_Cell_Degrees referenceCellDeg(referenceCell.getCell());
   std::cout << "Cell to Transform:" << std::endl;
   std::cout << "            " << sourceCellDeg << std::endl;
   std::cout << "Reference Cell:   " << std::endl;
   std::cout << "            " << referenceCellDeg << std::endl;

   // Use testSpecificMatricesWithB4 from TransformerUtilities.h
   testSpecificMatricesWithB4(sourceCell.getCell(), referenceCell.getCell());
}



