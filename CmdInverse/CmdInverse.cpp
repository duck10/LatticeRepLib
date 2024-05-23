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

   std::vector<LRL_Cell> inverses;
   for (const auto& data : inputList)
   {
      const LRL_Cell pCell = LatticeConverter::MakePrimitiveCell(data.GetLattice(), data.GetCell());
      inverses.emplace_back(pCell.Inverse());
   }

   for (const auto& inv : inverses) {
      std::cout << reader.Write(G6(LRL_Cell()), inv);
   }
}
/*
QUESTIONS
how to choose output data type?
should output type for each input match that item's type?
*/