
#include <iostream>

#include "D13.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "LRL_Vector3.h"

#include <vector>

int main()
{
   D13 d13;
   d13.GetIndices();
   d13.GetStringIndices();
   std::cout << "; To D13" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   for (size_t i = 0; i < inputList.size(); ++i) {
      std::cout << "D13 " << LRL_ToString(D13(inputList[i].GetCell()).GetAreas()) << std::endl;
      std::cout << " " << LRL_ToString(d13.GetIndices()) << std::endl;
   }
}
