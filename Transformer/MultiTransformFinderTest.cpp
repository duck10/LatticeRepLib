#include <iostream>
#include <vector>
#include <algorithm>

#include "B4.h"
#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include "LatticeCell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_Cell.h"
#include "LRL_Vector3.h"
#include "B4.h"
#include "S6.h"
#include "Selling.h"
#include "ProgramSetup.h"
#include "MultiTransformFinderControls.h"
#include "B4Matcher.h"
#include "TransformerUtilities.h"
#include "TransformerDisplay.h"
#include "TransformationModule.h"
#include "LatticeTransformer.h"

int main() {
   // Setup program and load controls
   MultiTransformFinderControls controls;
   const BasicProgramInput<MultiTransformFinderControls> program_setup("MultiTransformFinder", controls);
   const std::vector<LatticeCell>& inputList = program_setup.getInputList();

   // Check if we have enough input cells
   if (inputList.size() < 2) {
      throw std::runtime_error("; Need at least two lattices (reference and cells to transform) for transformation");
   }

   // Show controls if enabled
   if (controls.shouldShowControls()) {
      std::cout << controls << std::endl;
   }

   // Create display helper
   TransformerDisplay display(controls);

   // Create lattice transformer
   LatticeTransformer transformer(controls);

   // First cell is the reference
   const LatticeCell& referenceCell = inputList[0];

   // Process each cell to transform against the reference
   for (size_t i = 1; i < inputList.size(); ++i) {
      const LatticeCell& cellToTransform = inputList[i];

      // Display the cell pair
      display.showInputCells(cellToTransform, referenceCell);

      // ------------------------ Layer 1 & 2: Calculate transformations ----------------------------
      // This handles both centering conversions and primitive transformations
      CompleteTransformationResult result = transformer.transformLattice(
         cellToTransform, referenceCell);

      // Show centering information in detailed mode
      if (controls.shouldShowDetails() && result.hasCentering) {
         std::cout << "Centering conversion performed:" << std::endl;
         std::cout << "  Input cell centering: "
            << (result.inputCenteringType.empty() ? "P" : result.inputCenteringType)
            << std::endl;
         std::cout << "  Reference cell centering: "
            << (result.referenceCenteringType.empty() ? "P" : result.referenceCenteringType)
            << std::endl;

         // Display centering-to-primitive matrices
         std::cout << "Input cell to primitive matrix:" << std::endl;
         std::cout << result.inputToPrimitiveMatrix << std::endl;

         std::cout << "Reference cell to primitive matrix:" << std::endl;
         std::cout << result.referenceToPrimitiveMatrix << std::endl;
      }

      // Show additional information for primitive cells in detailed mode
      if (controls.shouldShowDetails() && !result.hasCentering) {
         // Only show detailed B4Matcher results for primitive cells
         B4Matcher matcher(cellToTransform.getCell(), referenceCell.getCell());

         // Show B4 matcher results
         display.showB4MatcherResults(matcher, cellToTransform, referenceCell);

         // Show B4 transformations
         display.showB4Transformations(matcher, cellToTransform, referenceCell);
      }

      // ------------------------ Layer 3: Display transformation results --------------------------
      if (result.primitiveResult.isValid) {
         // Choose the appropriate display method based on centering
         if (result.hasCentering) {
            // Use the centered display method
            display.showParameterTransformationsWithCentering(
               result.primitiveResult.transformations,
               cellToTransform,
               referenceCell,
               result.primitiveResult.duplicatesRemoved,
               result.inputToPrimitiveMatrix,
               result.referenceToPrimitiveMatrix);
         }
         else {
            // Use the original display method for primitive cells
            display.showParameterTransformations(
               result.primitiveResult.transformations,
               cellToTransform,
               referenceCell,
               result.primitiveResult.duplicatesRemoved);
         }

         // No need for the additional verification section anymore
         // since the correct metrics are shown directly in the display
      }
      else {
         std::cout << "\nCould not find valid transformations for this cell pair." << std::endl;
      }

      // Add a separator between cell pairs if processing multiple inputs
      if (i < inputList.size() - 1) {
         std::cout << "\n===============================================\n" << std::endl;
      }
   }

   return 0;
}

