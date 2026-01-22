#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <limits>
#include "G6.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "ProgramSetup.h"
#include "TestbedControls.h"
#include "TransformationMatrices.h"
#include "B4.h"

inline LRL_Cell operator*(const Matrix_3x3& matrix, const LRL_Cell& cell) {
   // Convert cell to B4 representation
   B4 b4Cell(cell);

   // Apply transformation in B4 space
   B4 transformedB4 = matrix * b4Cell;

   // Convert back to LRL_Cell
   return LRL_Cell(transformedB4);
}


int main() {
   LRL_Cell cell(" 1 1 1   90 90 90");
   static const std::vector<Matrix_3x3> matrices = TransformationMatrices::generateComprehensiveIntegerMatrices(1);
   static const std::vector<Matrix_3x3> matrices2 = TransformationMatrices::generateUnimodularMatrices();

   std::cout << "; TESTBED" << std::endl;
   try {
      TestbedControls controls;
      const BasicProgramInput<TestbedControls> dc_setup("CmdToB4", controls);
      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }
      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }
      for (const auto& input : dc_setup.getInputList()) {
         for (const auto& matrix : matrices) {
            const auto out = matrix * input.getCell();
            std::cout << "P " << LRL_Cell_Degrees(out) << std::endl;
         }
         std::cout << "P " << LRL_Cell_Degrees(input.getCell()) << std::endl;
      }
      return 0;
   }
   catch (const std::exception& e) {
      std::cout << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}