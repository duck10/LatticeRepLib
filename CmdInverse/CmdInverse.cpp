#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_WriteLatticeData.h"


int main()
{
   std::cout << "; ToInverse (to reciprocal cell)" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();

   LRL_WriteLatticeData reader;
   reader.SetPrecision(7);

   std::vector<LRL_Cell> inverses;
   for (const auto& data : inputList)
   {
      //const LRL_Cell pCell = LatticeConverter::MakePrimitiveCell(data.GetLattice(), data.GetCell());
      inverses.emplace_back(data.GetCell().Inverse());
   }

   for (const auto& inv : inverses) {
      std::cout << reader.Write(LRL_Cell(), inv);
   }
}
