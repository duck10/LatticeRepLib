#include <iostream>
#include <vector>

#include "CheckInputControls.h"
#include "ProgramSetup.h"
#include "RI.h"

int main() {
   std::cout << "; CheckInput" << std::endl;

   try {
      CheckInputControls controls;
      const BasicProgramInput<CheckInputControls> dc_setup("CmdCheckInput", controls);

      if (dc_setup.getInputList().empty()) {
         std::cout << "; no valid data found " << std::endl;
         return 0;
      }

      if (controls.getShowControls()) {
         std::cout << controls << std::endl;
      }

      for (const auto& input : dc_setup.getInputList()) {
         std::cout << input.getLatticeType() << " "
            << LRL_Cell_Degrees(input.getCell()) << std::endl;
      }

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}