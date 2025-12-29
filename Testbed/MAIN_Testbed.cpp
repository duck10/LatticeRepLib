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
   std::cout << "; TESTBED" << std::endl;
   std::vector<Matrix_3x3> matrices = TransformationMatrices::getAllTransformationMatrices();
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
         const LRL_Cell cell(input.getCell());
         double dmax = -1.0E30;
         double dmin = 1.0E30;
         for (const auto& m : matrices) {
            const double volume = (m * cell).Volume();
            dmax = std::max(dmax, volume);
            dmin = std::min(dmin, volume);
         }
         std::cout << "max = " << dmax << std::endl;
         std::cout << "min = " << dmin << std::endl;
      }
      return 0;
   }
   catch (const std::exception& e) {
      std::cout << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}