
#include <iostream>

#include "LRL_ReadLatticeData.h"
#include "C3.h"
#include <vector>

int main()
{
   std::cout << "; To C3" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   for (size_t i = 0; i < inputList.size(); ++i) {
      std::cout << "C3 " << C3(inputList[i].GetCell()) << std::endl;
   }
}
