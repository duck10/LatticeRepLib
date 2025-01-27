#include <iostream>
#include <string>
#include <vector>

#include "CmdPrimControls.h"
#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_WriteLatticeData.h"
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
         LRL_Cell primitive = LatticeConverter::MakePrimitiveCell(lattice, input.getCell());
         std::cout << writercout.WriteLatticeAndCell("P", primitive);
      }

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}