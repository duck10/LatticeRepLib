#include <iostream>
#include <map>

#include "B4.h"
#include "P3.h"
#include "ProgramSetup.h"
#include "CmdP3ReduceControls.h"
#include "P3_Reduce.h"
#include "LRL_Vector3.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"

#include <iomanip>
#include <vector>
#include "P3Dist.h"
#include "P3Utilities.h"


int main() {
   CmdP3ReduceControls controls;
   const BasicProgramInput<CmdP3ReduceControls> dc_setup("CmdP3Reduce", controls);

   if (dc_setup.getInputList().empty()) {
      throw std::runtime_error("; No input vectors provided");
   }

   if (controls.shouldShowControls()) {
      std::cout << controls << std::endl;
   }

   for (const LatticeCell& cell : dc_setup.getInputList()) {
      const P3 projected(cell.getCell());
      std::cout << "; P " << LRL_Cell_Degrees(cell.getCell()) << "; input cell" << std::endl;
      std::cout << "; P3 " << projected << " ; input projected to P3\n";
   }

   for (const LatticeCell& cell : dc_setup.getInputList()) {
      const P3 projected(cell.getCell());
      const auto [reduced, transform] = P3_Reduce::ReduceCellWith3480(projected);
      if (!reduced.IsValid()) {
         const int i19191 = 19191;
      }
      //std::cout << "P3 " << P3(reduced) << " ; reduced P3\n";
      std::cout << "P " << LRL_Cell_Degrees(reduced) << " ; reduced P3\n";
   }

   return 0;
}
