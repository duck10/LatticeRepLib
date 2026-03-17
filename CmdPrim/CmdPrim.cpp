#include <iostream>
#include <string>
#include <vector>

#include "CenteringMatrices.h"
#include "CmdPrimControls.h"
#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_WriteLatticeData.h"
#include "Matrix_3x3.h"
#include "ProgramSetup.h"

int main() {
   std::cout << "; To Primitive" << std::endl;

   try {
      CmdPrimControls controls;
      const BasicProgramInput<CmdPrimControls> dc_setup("CmdPrim", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      LRL_WriteLatticeData writercout;
      writercout.SetPrecision(4);

      for (const auto& input : dc_setup.getInputList()) {
         const std::string lattice(input.getLatticeType());
         const LRL_Cell primitive = LatticeConverter::MakePrimitiveCell(lattice, input.getCell());
         const Matrix_3x3 M = CenteringMatrices::ForLattice(lattice);

         std::string line = writercout.WriteLatticeAndCell("P", primitive);
         while (!line.empty() && (line.back() == '\n' || line.back() == '\r'))
            line.pop_back();
         std::cout << line << " " << CenteringMatrices::ToString(M) << "\n";
      }

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}