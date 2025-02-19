#include <iostream>

#include "CmdNiggliControls.h"
#include "G6.h"
#include "LatticeConverter.h"
#include "LRL_ReadLatticeData.h"
#include "Niggli.h"
#include "ProgramSetup.h"

int main() {
   std::cout << "; Niggli reduced" << std::endl;

   try {
      CmdNiggliControls controls;
      const BasicProgramInput<CmdNiggliControls> dc_setup("CmdNiggli", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      for (const auto& input : dc_setup.getInputList()) {
         LRL_Cell pCell = LatticeConverter::MakePrimitiveCell(input.getLatticeType(), input.getCell());
         int count = 0;
         if (!pCell.IsValid()) {
            continue;
         }
         G6 g;
         const bool b = Niggli::Reduce(G6(pCell), g);
         std::cout << "G6 " << g << std::endl;
      }

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}