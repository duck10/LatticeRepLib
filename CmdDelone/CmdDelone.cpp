
#include <iostream>

#include "LatticeConverter.h"
#include "LRL_ReadLatticeData.h"
#include "S6.h"
#include "Selling.h"

int main()
{
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   std::cout << "; Delone/Selling reduced" << std::endl;
   for (size_t i = 0; i < inputList.size(); ++i) {
      const LRL_Cell pCell = LatticeConverter::MakePrimitiveCell(inputList[i].GetLattice(), inputList[i].GetCell());
      S6 s;
      const bool b = Selling::Reduce(S6(pCell), s);
      std::cout << "S  " << s << std::endl;
   }
}
