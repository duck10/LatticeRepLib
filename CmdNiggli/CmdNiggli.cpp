
#include <iostream>

#include"LatticeConverter.h"
#include "LRL_ReadLatticeData.h"
#include "G6.h"
#include "Niggli.h"

int main()
{
   std::cout << "; Niggli reduced" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   for (size_t i = 0; i < inputList.size(); ++i) {
      const LRL_Cell pCell = LatticeConverter::MakePrimitiveCell(inputList[i].GetLattice(), inputList[i].GetCell());
      //std::cout << pCell << std::endl;
      G6 g;
      const bool b = Niggli::Reduce(G6(pCell), g);
      //std::cout << "cycles " << Niggli::GetCycles() << std::endl;
      std::cout << "G6 " << g << std::endl;
   }
}
