// TransformerUtilities.cpp
#include "TransformerUtilities.h"
#include "ProductionCommon.h"
#include "LatticeCell.h"
#include "LRL_Cell_Degrees.h"
#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include "S6.h"
#include "Selling.h"

#include <iomanip>
#include <iostream>
#include <sstream>


inline Matrix_3x3 calculateNiggliMatrix(const LRL_Cell& cell, bool showDetails = false) {
   // Get the G6 representation of the cell
   G6 g6Cell = cell; // Direct conversion using assignment operator

   // Output variables for the reduction
   G6 g6Reduced;
   MatG6 g6Matrix;
   Matrix_3x3 m3d;

   // Perform Niggli reduction with transform tracking
   bool success = Niggli::ReduceWithTransforms(g6Cell, g6Matrix, m3d, g6Reduced);
   std::cout << "reduced cell in calculateNiggliMatrix " << LRL_Cell_Degrees(g6Reduced);

   if (!success) {
      std::cerr << "Warning: Niggli reduction failed for cell " << LRL_Cell_Degrees(cell) << std::endl;
      // Return identity matrix as fallback
      return Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
   }

   // The m3d matrix is the transformation matrix we need
   if (showDetails) {
      std::cout << "Niggli reduction matrix determinant: " << m3d.Det() << std::endl;
      std::cout << "Original cell: " << LRL_Cell_Degrees(cell) << std::endl;
      std::cout << "Reduced cell: " << LRL_Cell_Degrees(g6Reduced) << std::endl;

      // Verify the transformation
      LRL_Cell reducedCell = m3d * cell;
      std::cout << "Transformed cell: " << LRL_Cell_Degrees(reducedCell) << std::endl;
      std::cout << "Distance between reduced and transformed: "
         << LRL_Cell::DistanceBetween(reducedCell, LRL_Cell(g6Reduced)) << std::endl;
   }

   return m3d;
}


void showUsageInformation(const MultiTransformFinderControls& controls) {
   std::cout << "Transformer: Tool for finding transformations between lattice cells" << std::endl;
   std::cout << "Usage: transformer [options] [INPUT_FILE]" << std::endl;
   std::cout << "Options:" << std::endl;
   std::cout << "  useniggli [true/false]  Enable/disable Niggli reduction" << std::endl;
   // Add more options as needed
}


void showTransformationInfo(const LatticeCell& mobileCell, const LatticeCell& referenceCell) {
   std::cout << "\n--- TRANSFORMATION DETAILS ---" << std::endl;

   // Show centering type information
   std::cout << "Mobile cell centering: " <<
      (mobileCell.getLatticeType().empty() ? "P" : mobileCell.getLatticeType()) << std::endl;
   std::cout << "Reference cell centering: " <<
      (referenceCell.getLatticeType().empty() ? "P" : referenceCell.getLatticeType()) << std::endl;

   // Show cells in G6 representation
   G6 mobileG6 = mobileCell.toPrimitive();
   G6 refG6 = referenceCell.toPrimitive();
   std::cout << "Mobile cell G6: " << mobileG6 << std::endl;
   std::cout << "Reference cell G6: " << refG6 << std::endl;

   // Show cells in S6 representation
   S6 mobileS6 = S6(mobileCell.getCell());
   S6 refS6 = S6(referenceCell.getCell());
   std::cout << "Mobile cell S6: " << mobileS6 << std::endl;
   std::cout << "Reference cell S6: " << refS6 << std::endl;

   // Calculate the centering matrices
   Matrix_3x3 mobileToPrimitive = ToPrimitive(mobileCell.getLatticeType(), mobileCell.getCell());
   Matrix_3x3 refToPrimitive =    ToPrimitive(referenceCell.getLatticeType(), referenceCell.getCell());

   std::cout << "Mobile-to-primitive matrix:" << std::endl;
   std::cout << mobileToPrimitive << std::endl;

   std::cout << "Reference-to-primitive matrix:" << std::endl;
   std::cout << refToPrimitive << std::endl;

   // Show Niggli-reduced cell information
   G6 mobileNiggli = mobileCell.getNiggliReducedCell();
   G6 refNiggli = referenceCell.getNiggliReducedCell();
   std::cout << "Mobile cell (Niggli reduced): " << mobileNiggli << std::endl;
   std::cout << "Reference cell (Niggli reduced): " << refNiggli << std::endl;

   std::cout << "--- END TRANSFORMATION DETAILS ---\n" << std::endl;
}

std::vector<LatticeCell> RunTests( MultiTransformFinderControls& controls, const int n) {
   std::stringstream ss;
   std::vector<LatticeCell> cells;
   if (n == 1) {
      ss << "show \n random 3 \n  end\n";
      InputHandler::readMixedInput( controls, cells, ss);
      std::vector<LatticeCell> cells = InputHandler::handleInput(controls);
   }
   return cells;
}

double CalculateNCDistWithReduction(const LRL_Cell& c1, const LRL_Cell& c2) {
   G6 out1;
   G6 out2;
   Niggli::Reduce(c1, out1);
   Niggli::Reduce(c2, out2);
   return NCDist(out1.data(), out2.data());
}


//LRL_Cell TransformerUtilities::convertToPrimitive(
//   const LatticeCell& lattice,
//   Matrix_3x3& centeringTransform,
//   const MultiTransformFinderControls& controls) {
//
//   // Extract the current implementation from LatticeMatchingPipeline.h
//   // and move it here
//
//   const std::string& latticeType = lattice.getLatticeType();
//   const LRL_Cell& originalCell = lattice.getCell();
//
//   if (controls.shouldShowDetails()) {
//      std::cout << "; Converting " << latticeType << " to primitive" << std::endl;
//   }
//
//   // Use existing ToPrimitive for the matrix
//   centeringTransform = TransformerUtilities::ToPrimitive(latticeType, originalCell);
//
//   // Apply transformation to get primitive cell
//   LRL_Cell primitiveCell = centeringTransform * originalCell;
//
//   if (controls.shouldShowDetails()) {
//      std::cout << "; Primitive cell: " << LRL_Cell_Degrees(primitiveCell) << std::endl;
//   }
//
//   return primitiveCell;
//}
//
//LRL_Cell TransformerUtilities::convertToPrimitive(
//   const LatticeCell& lattice,
//   Matrix_3x3& centeringTransform) {
//
//   // Extract the current implementation from LatticeMatchingPipeline.h
//   // and move it here
//
//   const std::string& latticeType = lattice.getLatticeType();
//   const LRL_Cell& originalCell = lattice.getCell();
//
//   // Use existing ToPrimitive for the matrix
//   centeringTransform = TransformerUtilities::ToPrimitive(latticeType, originalCell);
//
//   // Apply transformation to get primitive cell
//   LRL_Cell primitiveCell = centeringTransform * originalCell;
//   return primitiveCell;
//}
//
//// Alternative struct-based version
//TransformerUtilities::PrimitiveConversionResult
//TransformerUtilities::convertToPrimitiveComplete(
//   const LatticeCell& lattice) {
//
//   PrimitiveConversionResult result;
//   result.primitiveCell = convertToPrimitive(lattice, result.centeringTransform);
//   result.success = true; // Add validation logic if needed
//   return result;
//}
//
//
//// Alternative struct-based version
//TransformerUtilities::PrimitiveConversionResult
//TransformerUtilities::convertToPrimitiveComplete(
//   const LatticeCell& lattice,
//   const MultiTransformFinderControls& controls) {
//
//   PrimitiveConversionResult result;
//   result.primitiveCell = convertToPrimitive(lattice, result.centeringTransform, controls);
//   result.success = true; // Add validation logic if needed
//   return result;
//}
//
//static Matrix_3x3 ToPrimitive(const std::string& s, const LRL_Cell& cell) {
//   LatticeCell lc(cell, s);
//   TransformerUtilities::PrimitiveConversionResult pcr = TransformerUtilities::convertToPrimitive(lc);
//   return pcr.centeringTransform();
//}
