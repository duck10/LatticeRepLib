#include <iostream>

#include "DC7u.h"
#include "LRL_ReadLatticeData.h"

int main()
{
   std::cout << "; To u (DC7u)" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   for (size_t i = 0; i < inputList.size(); ++i) {
      std::cout << "u " << DC7u((const LRL_Cell)(inputList[i].GetCell())) << std::endl;
   }
}

