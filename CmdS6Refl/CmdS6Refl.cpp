
#include <iostream>

#include "LRL_ReadLatticeData.h"


int main()
{
   static const std::vector<MatS6> refl_one = MatS6::GetReflections();
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   for (size_t i = 0; i < inputList.size(); ++i) {
      for (size_t kk = 0; kk < refl_one.size(); ++kk) {
         std::cout << "S6 " << refl_one[kk] * S6(inputList[i].GetCell()) << std::endl;
      }
   }
}
