#include <iostream>

#include "CmdToV7Controls.h"
#include "LatticeConverter.h"
#include "LRL_ReadLatticeData.h"
#include "ProgramSetup.h"
#include "V7.h"

int main() {
   std::cout << "; To V7" << std::endl;

   try {
      CmdToV7Controls controls;
      const BasicProgramInput<CmdToV7Controls> dc_setup("CmdToV7", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      for (const auto& input : dc_setup.getInputList()) {
         const LRL_Cell pCell = LatticeConverter::MakePrimitiveCell(input.getLatticeType(), input.getCell());
         std::cout << "V7 " << V7(pCell) << std::endl;
      }

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}