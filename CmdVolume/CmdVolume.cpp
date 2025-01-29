#include <iostream>

#include "CmdVolumeControls.h"
#include "ProgramSetup.h"

int main() {
   std::cout << "; Volume" << std::endl;

   try {
      CmdVolumeControls controls;
      const BasicProgramInput<CmdVolumeControls> dc_setup("CmdVolume", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      for (const auto& input : dc_setup.getInputList()) {
         const LRL_Cell cell(input.getCell());
         const double volume = cell.Volume();
         std::cout << "  " << input.getLatticeType() << " "
            << LRL_Cell_Degrees(cell) << "  Volume = " << volume << std::endl;
      }

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}