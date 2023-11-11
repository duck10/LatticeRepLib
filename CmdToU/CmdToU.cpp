#include <iostream>

#include "DC7u.h"
#include "LatticeConverter.h"
#include "LRL_ReadLatticeData.h"
#include "D7.h"

int main()
{
   std::cout << "; To u (DC7u)" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   for (size_t i = 0; i < inputList.size(); ++i) {
      const LRL_Cell pCell = LatticeConverter::MakePrimitiveCell(inputList[i].GetLattice(), inputList[i].GetCell());
      const DC7u du(pCell);
      if (du.GetValid()) {
         std::cout << "u " << DC7u(pCell) << std::endl;
      }
      else {
         std::cout << "; u invalid " << DC7u(pCell) << std::endl;
      }
   }
}
