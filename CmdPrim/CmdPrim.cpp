#include <iostream>
#include <string>
#include <vector>

#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_WriteLatticeData.h"

int main()
{
   std::cout << "; To Primitive" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();

   LRL_WriteLatticeData writercout;
   writercout.SetPrecision(4);

   for (const auto& input : inputList)
   {
      const std::string lattice(input.GetLattice());
      LRL_Cell primitive = LatticeConverter::MakePrimitiveCell(lattice, input.GetCell());
      std::cout << writercout.WriteLatticeAndCell("P", primitive);
   }
}
