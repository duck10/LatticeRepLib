#include <iostream>

#include "CmdToUControls.h"
#include "DC7u.h"
#include "LatticeConverter.h"
#include "LRL_ReadLatticeData.h"
#include "D7.h"
#include "ProgramSetup.h"

int main() {
   std::cout << "; To u (DC7u)" << std::endl;

   try {
      CmdToUControls controls;
      const BasicProgramInput<CmdToUControls> dc_setup("CmdToU", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      for (const auto& input : dc_setup.getInputList()) {
         const LRL_Cell pCell = LatticeConverter::MakePrimitiveCell(input.getLatticeType(), input.getCell());
         const DC7u du(pCell);
         if (du.GetValid()) {
            std::cout << "U " << DC7u(pCell) << std::endl;
         }
         else {
            std::cout << "; U invalid " << DC7u(pCell) << std::endl;
         }
      }

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}