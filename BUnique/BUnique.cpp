
#include <iostream>
#include <utility>
#include <vector>

#include "C3.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_Cell_Degrees.h"

int main()
{
   std::cout << "; To B Unique" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   for (size_t i = 0; i < inputList.size(); ++i) {
      const std::string lattice = inputList[i].GetLattice();
      if (lattice.empty()) continue;
      const S6 s1 = inputList[i].GetCell();
      if (s1[1] != 0.0) {
         std::cout << lattice << " " << LRL_Cell_Degrees(inputList[i].GetCell()) << std::endl; //beta already unique
      }
      else if (s1[0] == 0.0) { // gamma is unique
         C3 c(s1);
         std::swap(c[1], c[2]);
         std::cout << lattice << " " << LRL_Cell_Degrees(c) << std::endl;
      }
      else if (s1[2] == 0.0) { // alpha is unique
         C3 c(s1);
         std::swap(c[0], c[1]);
         std::cout << lattice << " " << LRL_Cell_Degrees(c) << std::endl;
      }
      else {
         std::cout << "input cell does not have two 90 degree angles" << std::endl;
      }
   }
}
