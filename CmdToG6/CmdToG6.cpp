
#include <iostream>

#include "LRL_ReadLatticeData.h"
#include <vector>


int main()
{
   std::cout << "; To G6" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   for (size_t i = 0; i < inputList.size(); ++i) {
      std::cout << "G6 " << G6(inputList[i].GetCell()) << std::endl;
   }
}
