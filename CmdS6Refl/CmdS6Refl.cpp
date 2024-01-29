
#include <iostream>

#include "Filter_Duplicates.h"
#include "LRL_ReadLatticeData.h"

int main()
{
   static const std::vector<MatS6> refl_one = MatS6::GetReflections();
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   std::cout << "; S6 reflections" << std::endl;
   if (inputList.empty()) exit(2);

   std::vector<S6> reflected;
   for (size_t i = 0; i < inputList.size(); ++i) {
      for (size_t kk = 0; kk < refl_one.size(); ++kk) {
         reflected.emplace_back(refl_one[kk] * S6(inputList[i].GetCell()));
      }
   }

   const std::vector<S6> noDups = FilterRemoveDups<S6>(reflected);

   for ( size_t i=0; i<noDups.size(); ++i )
   {
      std::cout << "S6 " << noDups[i] << "  " << i << std::endl;
   }

}
