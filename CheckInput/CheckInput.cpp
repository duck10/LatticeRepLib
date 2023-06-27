
#include <iostream>

#include "LRL_ReadLatticeData.h"
#include <vector>

int main()
{
   LRL_ReadLatticeData reader;
   //std::cout << "; CheckInput" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = reader.ReadLatticeData();
   //for (size_t i = 0; i < inputList.size(); ++i) {
   //   std::cout << LRL_Cell_Degrees(inputList[i].GetCell()) << std::endl;
   //}
}
