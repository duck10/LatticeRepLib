#include <iostream>

#include "CmdDeloneControls.h"
#include "LatticeConverter.h"
#include "ProgramSetup.h"
#include "S6.h"
#include "Selling.h"

int main() {
   std::cout << "; Delone/Selling reduced" << std::endl;

   try {
      CmdDeloneControls controls;
      const BasicProgramInput<CmdDeloneControls> dc_setup("CmdDelone", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      for (const auto& input : dc_setup.getInputList()) {
         const LRL_Cell pCell = LatticeConverter::MakePrimitiveCell(input.getLatticeType(), input.getCell());
         S6 s;
         const bool b = Selling::Reduce(S6(pCell), s);
         std::cout << "S  " << s << std::endl;
      }

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}