#include <iostream>

#include "CmdToP3Controls.h"
#include "LRL_Cell.h"
#include "LRL_ReadLatticeData.h"
#include "P3.h"
#include "ProgramSetup.h"

int main() {
   std::cout << "; To P3" << std::endl;

   try {
      CmdToP3Controls controls;
      const BasicProgramInput<CmdToP3Controls> dc_setup("CmdToP3", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      for (const auto& input : dc_setup.getInputList()) {
         std::cout << "P3 " << P3(input.getCell()) << std::endl;
      }

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}