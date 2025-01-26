#include <iostream>
#include <vector>

#include "CmdToRIControls.h"
#include "LRL_ReadLatticeData.h"
#include "ProgramSetup.h"
#include "RI.h"

int main() {
   std::cout << "; To RI (Root Invariant per V.Kurlin)" << std::endl;

   try {
      CmdToRiControls controls;
      const BasicProgramInput<CmdToRiControls> dc_setup("CmdToRi", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      if (controls.getShowControls()) {
         std::cout << controls << std::endl;
      }

      for (const auto& input : dc_setup.getInputList()) {
         std::cout << "RI " << RI(input.getCell()) << std::endl;
      }

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}