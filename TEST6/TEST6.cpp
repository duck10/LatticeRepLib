
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "B4.h"
#include "G6.h"
#include "LRL_Cell_Degrees.h"
#include "TransformationMatrices.h"
#include "InputHandler.h"
#include "ProgramSetup.h"
#include "Test6Controls.h"

// Simple matrix application helper
inline LRL_Cell applyMatrixToCell(const LRL_Cell& cell, const Matrix_3x3& matrix) {
   const B4 b4Cell(cell);
   const B4 transformedB4 = matrix * b4Cell;
   return LRL_Cell(transformedB4);
}

int main() {

   try {
      // Setup input handling using your actual system
         // Setup controls and input handling
      MultiTransformFinderControls controls;
      const BasicProgramInput<MultiTransformFinderControls> program_setup("ProductionLatticeMatcher", controls);

      if (program_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }
      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      const std::vector<Matrix_3x3> matrices = TransformationMatrices::generateComprehensiveIntegerMatrices(1);

      // Process the input list using configured controls (from input handling)
      for (const auto& input : program_setup.getInputList()) {
         const std::string lattice = input.getLatticeType();
         const LRL_Cell cell = input.getCell();
         for (const auto& m : matrices) {
            std::cout << "P " << LRL_Cell_Degrees(applyMatrixToCell(input.getCell(), m)) << std::endl;
         }
      }


      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << ";Error: " << e.what() << std::endl;
      return -1;
   }
}
