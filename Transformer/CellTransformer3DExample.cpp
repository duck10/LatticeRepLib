#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <memory>
#include <vector>

#include "BaseControlVariables.h"
#include "CellTransformer3D.h"
#include "InputHandler.h"
#include "ProgramSetup.h"
#include "WebIO.h"

#include "LatticeCell.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "ProgramSetup.h"

int main(int argc, char* argv[]) {
   std::cout << "Cell Transformer 3D Example" << std::endl;
   std::cout << "=========================" << std::endl;

   try {
      // Create WebIO object for input handling (if using your existing framework)
      WebIO webio(argc, argv, "CellTransformer3D", 0);

      // Create or load cells
      // Option 1: Create cells from parameters
      LRL_Cell_Degrees sourceParams(LRL_Cell(10.0, 10.0, 10.0, 90.0, 90.0, 90.0));
      LRL_Cell_Degrees targetParams(LRL_Cell(10.0, 10.0, 10.0, 90.0, 90.0, 120.0));

      LatticeCell sourceCell(sourceParams);
      LatticeCell targetCell(targetParams);

      // Option 2: Load cells from input (if using your existing input framework)
      // std::vector<LatticeCell> inputCells = InputHandler::handleInput(controls, webio);
      // if (inputCells.size() >= 2) {
      //     sourceCell = inputCells[0];
      //     targetCell = inputCells[1];
      // }

      // Display input cells
      std::cout << "\nSource Cell: Cubic" << std::endl;
      std::cout << "a = b = c = 10.0, α = β = γ = 90°" << std::endl;

      std::cout << "\nTarget Cell: Hexagonal" << std::endl;
      std::cout << "a = b = 10.0, c = 10.0, α = β = 90°, γ = 120°" << std::endl;

      // Create transformer
      CellTransformer3D transformer(sourceCell, targetCell);

      // Find best transformation
      std::cout << "\nFinding transformation..." << std::endl;
      CellTransformer3D::TransformationResult result = transformer.findBestTransformation();

      // Display result
      std::cout << "\n" << transformer.formatTransformation(result) << std::endl;

      // Alternatively, use the other methods
      std::cout << "\nFinding direct transformation..." << std::endl;
      CellTransformer3D::TransformationResult directResult = transformer.findDirectTransformation();
      std::cout << "\nDistance: " << directResult.distance << std::endl;

      std::cout << "\nFinding least squares transformation..." << std::endl;
      CellTransformer3D::TransformationResult lsResult = transformer.findLeastSquaresTransformation();
      std::cout << "\nDistance: " << lsResult.distance << std::endl;

      std::cout << "\nFinding integer transformations..." << std::endl;
      std::vector<CellTransformer3D::TransformationResult> intResults =
         transformer.findIntegerTransformations(2, 3);

      std::cout << "\nFound " << intResults.size() << " integer transformations:" << std::endl;
      for (size_t i = 0; i < intResults.size(); ++i) {
         std::cout << "\nTransformation " << (i + 1) << ":" << std::endl;
         std::cout << "Distance: " << intResults[i].distance << std::endl;
         std::cout << "Matrix:" << std::endl;
         std::cout << intResults[i].transformMatrix << std::endl;
      }

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return 1;
   }
}
