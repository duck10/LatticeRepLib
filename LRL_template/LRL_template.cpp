#include <iostream>

#include "TemplateControls.h"
#include "ProgramSetup.h"

int main() {
   std::cout << "; LRL_TEMPLATE" << std::endl;

   try {
      eISENSTEINControls controls;
      const BasicProgramInput<eISENSTEINControls> dc_setup("TEMPLATE", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      for (const auto& input : dc_setup.getInputList()) {
         std::cout << ";LRL_TEMPLATE " << input;
      }

      return 0;
   }
   catch (const std::exception& e) {
      std::cout << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}
