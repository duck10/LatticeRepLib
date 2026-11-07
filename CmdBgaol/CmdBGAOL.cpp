#include <cmath>
#include <iostream>
#include <vector>

#include "CmdBgaolControls.h"
#include "ProgramSetup.h"

static const double pi = 4.0 * atan(1.0);

void proc_bgaol(const LatticeCell& cell, BgaolControls& controls) {
   const std::string lattice = cell.getLatticeType();
   const double a = cell.getCell()[0];
   const double b = cell.getCell()[1];
   const double c = cell.getCell()[2];
   const double alpha = cell.getCell()[3] * 180.0 / pi;
   const double beta  = cell.getCell()[4] * 180.0 / pi;
   const double gamma = cell.getCell()[5] * 180.0 / pi;

   const std::vector<double> sigmas = controls.getSigmas();
}

int main(int argc, char* argv[])
{
   std::cout << "; CmdBGAOL" << std::endl;

   try {
      WebIO webio(argc, argv, "CmdBGAOL", 0);

      BgaolControls controls;
      controls.setWebRun(webio.m_hasWebInstructions);

      const BasicProgramInput<BgaolControls> dc_setup("CmdBGAOL", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      for (const auto& input : dc_setup.getInputList()) {
         if ( controls.shouldShowDetails())
         {
            std::cout << ";CmdBGAOL " << input;
         }
         proc_bgaol(input, controls);
      }

      return 0;
   }

   catch (const std::exception& e) {
      std::cout << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}
