#include <iostream>
#include <string>
#include <vector>

#include "CmdInverseControls.h"
#include "LRL_Cell.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_WriteLatticeData.h"
#include "ProgramSetup.h"

static std::string UpdateInverseLatticeTypeToInverseType(const std::string& lattice) {
   if (LRL_StringTools::strToupper(lattice) == "F") return "I";
   else if (LRL_StringTools::strToupper(lattice) == "I") return "F";
   else return lattice;
}

int main() {
   std::cout << "; ToInverse (to reciprocal cell)" << std::endl;

   try {
      CmdInverseControls controls;
      const BasicProgramInput<CmdInverseControls> dc_setup("CmdInverse", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      LRL_WriteLatticeData writercout;
      writercout.SetPrecision(7);

      for (const auto& input : dc_setup.getInputList()) {
         const std::string lattice(UpdateInverseLatticeTypeToInverseType(input.getLatticeType()));
         const LRL_Cell inverse(input.getCell().Inverse());
         std::cout << writercout.WriteLatticeAndCell(lattice, inverse);
      }

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}