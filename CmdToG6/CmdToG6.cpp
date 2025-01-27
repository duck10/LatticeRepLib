#include <iostream>

#include "CmdToG6Controls.h"
#include "LRL_ReadLatticeData.h"
#include "ProgramSetup.h"

int main() {
   std::cout << "; To G6" << std::endl;

   try {
      CmdToG6Controls controls;
      const BasicProgramInput<CmdToG6Controls> dc_setup("CmdToG6", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      for (const auto& input : dc_setup.getInputList()) {
         std::cout << "G6 " << G6(input.getCell()) << std::endl;
      }

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}