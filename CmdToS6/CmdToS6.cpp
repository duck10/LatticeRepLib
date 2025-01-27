#include <iostream>

#include "CmdToS6Controls.h"
#include "LRL_ReadLatticeData.h"
#include "ProgramSetup.h"

int main() {
   std::cout << "; To S6" << std::endl;

   try {
      CmdToS6Controls controls;
      const BasicProgramInput<CmdToS6Controls> dc_setup("CmdToS6", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      for (const auto& input : dc_setup.getInputList()) {
         std::cout << "S6 " << S6(input.getCell()) << std::endl;
      }

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}