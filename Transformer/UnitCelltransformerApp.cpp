#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <memory>

#include "BaseControlVariables.h"
#include "CellTransformer.h"
#include "InputHandler.h"
#include "LRL_Vector3.h"

class UnitCellControlVariables : public BaseControlVariables {
public:
   bool showOrthogonalCells = true;
   bool showMinimumVolumeCells = true;
   bool showNiggliReducedCell = true;
   bool showDelaunayReducedCell = true;
   bool showAllPrimitiveCells = false;
   double maxCellVectorLength = 10.0;
   double minCellAngle = 30.0;
   size_t maxIterations = 1000;

   // Method required by BaseControlVariables interface
   void ProcessCommandLine(const std::string& commandLine)  {
      // Process command line arguments relevant to UnitCellTransformer
      std::vector<std::string> tokens = InputHandler::parseInputLine(commandLine);

      if (tokens.size() >= 2) {
         const std::string& command = tokens[0];
         const std::string& value = tokens[1];

         if (command == "orthogonal") {
            showOrthogonalCells = (value == "true" || value == "1" || value == "yes");
         }
         else if (command == "min_volume") {
            showMinimumVolumeCells = (value == "true" || value == "1" || value == "yes");
         }
         else if (command == "niggli") {
            showNiggliReducedCell = (value == "true" || value == "1" || value == "yes");
         }
         else if (command == "delaunay") {
            showDelaunayReducedCell = (value == "true" || value == "1" || value == "yes");
         }
         else if (command == "all_primitive") {
            showAllPrimitiveCells = (value == "true" || value == "1" || value == "yes");
         }
         else if (command == "max_length") {
            maxCellVectorLength = std::stod(value);
         }
         else if (command == "min_angle") {
            minCellAngle = std::stod(value);
         }
         else if (command == "max_iterations") {
            maxIterations = static_cast<size_t>(std::stoul(value));
         }
      }
   }
};

void displayCellAndMatrix(const std::string& label,
   const CellTransformer::TransformResult& result) {
   std::cout << "==================================================" << std::endl;
   std::cout << label << std::endl;
   std::cout << "--------------------------------------------------" << std::endl;

   // Display cell parameters
   double a, b, c, alpha, beta, gamma;
   result.transformedCell.getCellParameters(a, b, c, alpha, beta, gamma);

   std::cout << "Cell parameters:" << std::endl;
   std::cout << "  a = " << std::fixed << std::setprecision(4) << a << " Å" << std::endl;
   std::cout << "  b = " << std::fixed << std::setprecision(4) << b << " Å" << std::endl;
   std::cout << "  c = " << std::fixed << std::setprecision(4) << c << " Å" << std::endl;
   std::cout << "  α = " << std::fixed << std::setprecision(2) << alpha << "°" << std::endl;
   std::cout << "  β = " << std::fixed << std::setprecision(2) << beta << "°" << std::endl;
   std::cout << "  γ = " << std::fixed << std::setprecision(2) << gamma << "°" << std::endl;

   // Display transformation matrix
   std::cout << "Transformation matrix:" << std::endl;
   std::cout << result.transformMatrix << std::endl;

   // Display score
   std::cout << "Score: " << std::fixed << std::setprecision(6) << result.score << std::endl;
}

void displayAllPrimitiveCells(const std::vector<UnitCellTransformer::TransformResult>& results,
   size_t maxToShow = 5) {
   std::cout << "==================================================" << std::endl;
   std::cout << "All Primitive Cells (Top " << std::min(maxToShow, results.size()) << ")" << std::endl;
   std::cout << "--------------------------------------------------" << std::endl;

   size_t count = 0;
   for (const auto& result : results) {
      if (count++ >= maxToShow) break;

      // Display cell parameters
      double a, b, c, alpha, beta, gamma;
      result.transformedCell.getCellParameters(a, b, c, alpha, beta, gamma);

      std::cout << "Cell " << count << ":" << std::endl;
      std::cout << "  a = " << std::fixed << std::setprecision(4) << a << " Å" << std::endl;
      std::cout << "  b = " << std::fixed << std::setprecision(4) << b << " Å" << std::endl;
      std::cout << "  c = " << std::fixed << std::setprecision(4) << c << " Å" << std::endl;
      std::cout << "  α = " << std::fixed << std::setprecision(2) << alpha << "°" << std::endl;
      std::cout << "  β = " << std::fixed << std::setprecision(2) << beta << "°" << std::endl;
      std::cout << "  γ = " << std::fixed << std::setprecision(2) << gamma << "°" << std::endl;
      std::cout << "  Score: " << std::fixed << std::setprecision(6) << result.score << std::endl;
      std::cout << "  Transformation matrix:" << std::endl;
      std::cout << result.transformMatrix << std::endl;
      std::cout << "--------------------------------------------------" << std::endl;
   }
}

int main() {
   std::cout << "Unit Cell Transformer" << std::endl;
   std::cout << "====================" << std::endl;

   // Initialize control variables
   UnitCellControlVariables controls;

   // Create WebIO object for input handling
   WebIO webio;

   // Handle input using InputHandler
   std::vector<LatticeCell> inputCells = InputHandler::handleInput(controls, webio);

   if (inputCells.empty()) {
      std::cerr << "Error: No input cells provided." << std::endl;
      return 1;
   }

   // Process each input cell
   for (size_t i = 0; i < inputCells.size(); ++i) {
      const LatticeCell& inputCell = inputCells[i];

      std::cout << "\nProcessing Cell " << (i + 1) << ":" << std::endl;

      // Create UnitCellTransformer
      UnitCellTransformer transformer(inputCell);

      // Display original cell
      double a, b, c, alpha, beta, gamma;
      inputCell.getCellParameters(a, b, c, alpha, beta, gamma);

      std::cout << "Original Cell:" << std::endl;
      std::cout << "  a = " << std::fixed << std::setprecision(4) << a << " Å" << std::endl;
      std::cout << "  b = " << std::fixed << std::setprecision(4) << b << " Å" << std::endl;
      std::cout << "  c = " << std::fixed << std::setprecision(4) << c << " Å" << std::endl;
      std::cout << "  α = " << std::fixed << std::setprecision(2) << alpha << "°" << std::endl;
      std::cout << "  β = " << std::fixed << std::setprecision(2) << beta << "°" << std::endl;
      std::cout << "  γ = " << std::fixed << std::setprecision(2) << gamma << "°" << std::endl;

      // Find and display results based on control variables
      if (controls.showNiggliReducedCell) {
         auto result = transformer.findNiggliReducedCell();
         displayCellAndMatrix("Niggli-Reduced Cell", result);
      }

      if (controls.showDelaunayReducedCell) {
         auto result = transformer.findDelaunayReducedCell();
         displayCellAndMatrix("Delaunay-Reduced Cell", result);
      }

      if (controls.showOrthogonalCells) {
         auto result = transformer.findMostOrthogonalCell(controls.maxIterations);
         displayCellAndMatrix("Most Orthogonal Cell", result);
      }

      if (controls.showMinimumVolumeCells) {
         auto result = transformer.findMinimumVolumeCell(controls.maxIterations);
         displayCellAndMatrix("Minimum Volume Cell", result);
      }

      if (controls.showAllPrimitiveCells) {
         auto results = transformer.findAllPrimitiveCells(
            controls.maxCellVectorLength, controls.minCellAngle);
         displayAllPrimitiveCells(results);
      }
   }

   return 0;
}

