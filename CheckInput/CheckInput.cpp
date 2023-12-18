
#include <iostream>
#include <vector>

#include "LRL_ReadLatticeData.h"
#include "RI.h"

int main()
{
   std::cout << "; CheckInput" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   for (const auto& input: inputList) {
      std::cout << input.GetLattice() << " "
         << LRL_Cell_Degrees(input.GetCell()) << std::endl;
   }

   if (inputList.empty()) {
      std::cout << "; no valid data found " << std::endl;
   }
}
