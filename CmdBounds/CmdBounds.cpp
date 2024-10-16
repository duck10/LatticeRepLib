
#include <iostream>

#include "LRL_ReadLatticeData.h"

#include <vector>


int main()
{
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   std::cout << "; S6 boundries" << std::endl;

   for (size_t i = 0; i < inputList.size(); ++i) {
      std::cout << std::endl << std::endl;
   }
}
