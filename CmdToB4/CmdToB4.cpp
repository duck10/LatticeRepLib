#include <iostream>

#include "B4.h"
#include "CmdToB4Controls.h"
#include "LRL_ReadLatticeData.h"
#include "ProgramSetup.h"

int main() {
   std::cout << "; To B4" << std::endl;

   try {
      CmdToB4Controls controls;
      const BasicProgramInput<CmdToB4Controls> dc_setup("CmdToB4", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      for (const auto& input : dc_setup.getInputList()) {
         const B4 b4(input.getCell());
         std::cout <<
            input.getLatticeType() << "  " << LRL_Cell_Degrees(input.getCell()) << std::endl
            << "; a: " << b4[0] << std::endl
            << "; b: " << b4[1] << std::endl
            << "; c: " << b4[2] << std::endl
            << "; d: " << b4[3] << std::endl;
         std::cout << "; B4 lengths   " << b4[0].norm() << " " << b4[1].norm() << " "
            << b4[2].norm() << " " << b4[3].norm();
         std::cout << std::endl << std::endl;
      }

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}