 
#include <iostream>
#include <iomanip>

#include "LRL_ReadLatticeData.h"
#include "B4.h"
#include <vector>


int main()
{
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   std::cout << "; To B4" << std::endl;

   for (size_t i = 0; i < inputList.size(); ++i) {
      const B4 b4(inputList[i].GetCell());
      std::cout << std::fixed  
         << "a: " << std::setprecision(6) << b4[0] << std::endl 
         << "b: " << std::setprecision(6) << b4[1] << std::endl 
         << "c: " << std::setprecision(6) << b4[2] << std::endl 
         << "d: " << std::setprecision(6) << b4[3] << std::endl;
      std::cout << "B4 lengths   " << b4[0].norm() << " " << b4[1].norm() << " " << b4[2].norm() << " " << b4[3].norm();
      std::cout << std::endl << std::endl;
   }
}
