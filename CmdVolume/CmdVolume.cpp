
#include <iostream>

#include "LRL_ReadLatticeData.h"


int main()
{
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   std::cout << "; Volume" << std::endl;
   for (size_t i = 0; i < inputList.size(); ++i) {
      const LRL_Cell cell(inputList[i].GetCell());
      const double volume = cell.Volume();
      std::cout << "input cell " << LRL_Cell_Degrees(cell) << "  Volume " << volume << std::endl;
   }
}
