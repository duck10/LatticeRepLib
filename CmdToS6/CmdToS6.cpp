
#include <iostream>

#include "LRL_ReadLatticeData.h"


int main()
{
   std::cout << "; To S6" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   for (size_t i = 0; i < inputList.size(); ++i) {
      std::cout << "S6 " << S6(inputList[i].GetCell()) << std::endl;
   }
}
