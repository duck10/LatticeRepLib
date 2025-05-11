#include "MultiTransformFinderControls.h"
#include "LatticeCell.h"
#include "B4Matcher.h"
#include "TransformerDisplay.h"
#include "LatticeTransformer.h"
#include "TransformerUtilities.h"
#include "InputHandler.h"
#include "ProgramSetup.h"
#include "LRL_Cell.h"
#include "Niggli.h"
#include "G6.h"

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

int main(int argc, char* argv[]) {

   //const Matrix_3x3 m33 = { 1,-1,0, 0,1,0, 0,-1,1 };
   //const LRL_Cell c3 = { 1,2,1,90,90, 90 };
   //const LRL_Cell out = m33 * c3;
   //std::cout << m33 << std::endl << std::endl;
   //std::cout << "P " << LRL_Cell_Degrees(c3) << std::endl;
   //std::cout << "P " << LRL_Cell_Degrees(out) << std::endl;
   //exit(0);

   //const LRL_Cell cell(LRL_Cell::rand());
   //const Matrix_3x3 toC = cCentered;
   //const Matrix_3x3 toF = faceCentered;
   //const LRL_Cell_Degrees outC = toC.Inver() * cell;
   //const LRL_Cell_Degrees outF = toF.Inver() * cell;
   //std::cout << "C " << outC << std::endl;
   //std::cout << "F " << outF << std::endl;
   //exit(0);

   //G6 junk;
   //MatG6 mjunk;
   //Niggli::Reduce(junk, mjunk, junk, 19191.);

   try {
      // initialize controls
      MultiTransformFinderControls controls;

      // Initialize program using  BasicProgramInput system
      const BasicProgramInput<MultiTransformFinderControls> program_setup("Transformer", controls);
      const std::vector<LatticeCell>& inputList = program_setup.getInputList();

      // Show control settings if requested
      if (controls.getShowControls()) {
         std::cout << controls << std::endl;
      }

      // Need at least two cells (reference and at least one mobile)
      if (inputList.size() < 2) {
         showUsageInformation(controls);
         return 1;
      }

      // Get the reference cell (first one in the list)
      const LatticeCell& referenceCell = inputList[0];

      // Create lattice transformer
      LatticeTransformer transformer(controls);

      // Process each mobile cell (all cells after the first one)
      for (size_t i = 1; i < inputList.size(); ++i) {
         const LatticeCell& mobileCell = inputList[i];

         // Add a separator between multiple transformations
         if (i >= 1) {
            std::cout << "\n\n=#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==\n";
            std::cout << "Transformation (Cell to Match to reference) " << i << " of " << (inputList.size() - 1) << "\n";
            std::cout << "=#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==\n\n";
         }

         // Create display handler
         TransformerDisplay display(controls);
         // Show input cells for this comparison
         display.showInputCells(mobileCell, referenceCell);

         // If show extra transform info is enabled, show additional information
         if (controls.shouldShowExtraTransformInfo()) {
            // Use the utility function to show transformation info
            showTransformationInfo(mobileCell, referenceCell);
         }

         // Transform lattice with complete centering handling
         CompleteTransformationResult result = transformer.transformLattice(
            mobileCell, referenceCell);

         // Display the results
         display.showCompleteTransformations(result, mobileCell, referenceCell);
      }

      return 0;
   }
   catch (const std::exception& ex) {
      std::cerr << "Error: " << ex.what() << std::endl;
      return 1;
   }
   catch (...) {
      std::cerr << "Unknown error occurred" << std::endl;
      return 1;
   }
}

