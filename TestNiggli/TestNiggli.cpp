#include <iostream>

#include "TestNiggliControls.h"
#include "ProgramSetup.h"
#include "NCDist_McCoy.h"
#include "NCDist.h"
#include "NCDist_HJB.h"
#include "S6M_SellingReduce.h"
#include "Polar.h"
#include "LRL_Cell_Degrees.h"


int main() {
   std::cout << "; LRL_TEMPLATE" << std::endl;

   TemplateControls controls;
   const BasicProgramInput<TemplateControls> dc_setup("TEMPLATE", controls);

   if (dc_setup.getInputList().empty()) {
      throw std::runtime_error("; No input vectors provided");
   }

   std::cout << "NCDist phasertng NCDist/phasertng" << std::endl;

   G6 g6M(Polar::rand());
   g6M=G6(Polar::rand());
   for (const auto& input : dc_setup.getInputList()) {
      //std::cout << ";Test McCoy NCDist " << input;
      const G6 g6in = input.getCell();

      const double NCDistDistance = NCDist(g6in.data(), g6M.data());
      const double McCOllDistance = phasertng::NCDist(g6in.data(), g6M.data());

      std::cout << "P " << LRL_Cell_Degrees(g6in) << std::endl;
      std::cout << "P " << LRL_Cell_Degrees(g6M) << std::endl;


      std::cout << NCDistDistance << "  " << McCOllDistance
         << "  " << NCDistDistance / McCOllDistance << std::endl << std::endl;
   }

   return 0;
}
