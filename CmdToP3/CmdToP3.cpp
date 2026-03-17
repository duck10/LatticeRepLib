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

      const auto vcell = dc_setup.getInputList();

      for (const auto& input : dc_setup.getInputList()) {
         const double dist = P3::DistanceBetween(P3(vcell[0].getCell()), P3(input.getCell()));
         std::cout << "P3 " << P3(input.getCell()) << "  dist from first cell " << dist << std::endl;
      }

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}