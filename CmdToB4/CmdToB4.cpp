
#include <iostream>

#include "LRL_ReadLatticeData.h"
#include "B4.h"
#include <vector>


int main()
{
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   std::cout << "; To B4" << std::endl;

   for (size_t i = 0; i < inputList.size(); ++i) {
      const B4 b4(inputList[i].GetCell());
      std::cout <<
         inputList[i].GetLattice() << "  " << LRL_Cell_Degrees(inputList[i].GetCell()) << std::endl
         << "; a: " << b4[0] << std::endl 
         << "; b: " << b4[1] << std::endl 
         << "; c: " << b4[2] << std::endl 
         << "; d: " << b4[3] << std::endl;
      std::cout << "; B4 lengths   " << b4[0].norm() << " " << b4[1].norm() << " " << b4[2].norm() << " " << b4[3].norm();
      std::cout << std::endl << std::endl;
   }
}
