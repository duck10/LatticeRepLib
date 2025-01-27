#include <iostream>

#include "CmdToCellControls.h"
#include "LRL_ReadLatticeData.h"
#include "ProgramSetup.h"

int main() {
   std::cout << "; To Cell" << std::endl;

   try {
      CmdToCellControls controls;
      const BasicProgramInput<CmdToCellControls> dc_setup("CmdToCell", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      const std::string latticeTypes("aAbBcCiIfFpPhHrR");

      for (const auto& input : dc_setup.getInputList()) {
         const size_t n = latticeTypes.find(input.getLatticeType());

         std::string lattice;
         if (n == std::string::npos) lattice = "P";
         else lattice = std::string(input.getLatticeType());

         std::cout << lattice << " " << LRL_Cell_Degrees(input.getCell()) << std::endl;
      }

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}