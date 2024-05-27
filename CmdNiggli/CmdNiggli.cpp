
#include <iostream>

#include"LatticeConverter.h"
#include "LRL_ReadLatticeData.h"
#include "G6.h"
#include "Niggli.h"

int main()
{
   std::cout << "; Niggli reduced" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   for (const auto& input : inputList) {
      const LRL_Cell pCell = LatticeConverter::MakePrimitiveCell(input.GetLattice(), input.GetCell());
      G6 g;
      const bool b = Niggli::Reduce(G6(pCell), g);
      //std::cout << "cycles " << Niggli::GetCycles() << std::endl;
      std::cout << "G6 " << g << std::endl;
   }
}
