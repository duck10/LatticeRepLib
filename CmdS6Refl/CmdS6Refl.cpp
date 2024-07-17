
#include <iostream>

#include "Filter_Duplicates.h"
#include "LRL_ReadLatticeData.h"

int main(int argc, char* argv[])
{
   double delta = 1.0E-6;

   if (argc > 1) {
      const double deltatest = atof(argv[1]);
      if (deltatest != 0) delta = deltatest;
   }

   static const std::vector<MatS6> refl_one = MatS6::GetReflections();
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   std::cout << "; S6 reflections" << std::endl;
   if (inputList.empty()) exit(2);

   std::vector<S6> reflected;
   for (const auto& input : inputList) {
      for (const auto& refl : refl_one) {
         reflected.emplace_back(refl * S6(input.GetCell()));
      }
   }

   const std::vector<S6> noDups = EliminateDuplicates<S6>(reflected, delta);

   for ( size_t i=0; i<noDups.size(); ++i )
   {
      std::cout << "S6 " << noDups[i] << "  " << i << std::endl;
   }

}
