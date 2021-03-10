
#include <iostream>

#include "LRL_ReadLatticeData.h"


int main()
{
   const std::string latticeTypes("aAbBcCiIfFpPhHrR");
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();

   std::cout << "; To Cell" << std::endl;
   for (size_t i = 0; i < inputList.size(); ++i) {
      const size_t n = latticeTypes.find(inputList[i].GetLattice());
      const std::string inputDataType = inputList[i].GetInputDataType();

      std::string lattice;
      if (inputDataType.size() > 2) lattice = "P";
      else if(n == std::string::npos) lattice = "P";
      else lattice = std::string(inputList[i].GetLattice());

      std::cout << lattice  << " " <<LRL_Cell_Degrees(inputList[i].GetCell()) << std::endl;
   }
}
