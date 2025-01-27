#include <iostream>

#include "C3.h"
#include "CmdToC3Controls.h"
#include "LRL_ReadLatticeData.h"
#include "ProgramSetup.h"

int main() {
   std::cout << "; To C3" << std::endl;

   try {
      CmdToC3Controls controls;
      const BasicProgramInput<CmdToC3Controls> dc_setup("CmdToC3", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      LRL_ReadLatticeData reader;
      for (const auto& input : dc_setup.getInputList()) {
         std::cout << "C3 " << C3(input.getCell()) << std::endl;
      }

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}