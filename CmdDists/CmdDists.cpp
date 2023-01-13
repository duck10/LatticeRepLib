
#include <cmath>
#include <iomanip>
#include <iostream>

#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_CreateFileName.h"
#include "LatticeConverter.h"
#include "LRL_ReadLatticeData.h"
#include "NCDist.h"
#include "S6.h"

int main()
{
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   std::cout << "; CmdDists, calculate distances" << std::endl;
   std::vector<S6> s6Cells;
   for (int i = 0; i < inputList.size(); ++i) {
      s6Cells.push_back(inputList[i].GetCell());
   }

   if (inputList.size() < 2) {
      std::cout << "; CmdDists requires at least 2 cells input" << std::endl;
      exit(99);
   }

   std::vector<LRL_Cell> vcells;
   std::vector<S6> vg6Red;
   std::vector<G6> vs6Red;

   for (size_t i = 0; i < inputList.size(); ++i) {
      const std::string lattice = inputList[i].GetLattice();
      const LRL_Cell cell = inputList[i].GetCell();

      const LRL_Cell_Degrees test(cell);
      const LRL_Cell testforbad(test[0], test[1], test[2], test[3], test[4], test[5]);
      if (!testforbad.IsValid()) std::cout << "invalid " << i << std::endl;


      vcells.push_back(cell);
      vg6Red.push_back(LatticeConverter().NiggliReduceCell(lattice, G6(cell)));
      vs6Red.push_back(LatticeConverter().SellingReduceCell(lattice, S6(cell)));
   }

   std::cout << " ordinals  G6-Euclidean  NCDist CS6Dist  input cell" << std::endl;
   for (size_t i = 0; i < inputList.size()-1; ++i) {
      const double dG6 = NCDist (vg6Red[i].data(), vg6Red[i + 1].data());
      const double dS6 = CS6Dist(vs6Red[i].data(), vs6Red[i + 1].data());
      const double dEu = (vcells[i] - vcells[i + 1]).norm();

      std::cout << std::fixed << std::setprecision(3) 
         << i+1 << " to " << i + 2 << "   " 
         << dEu << " " << dG6 << " " << dS6 << "          "
         << LRL_Cell_Degrees(vcells[i]) << std::endl;
   }

   std::cout << std::fixed << std::setprecision(3) 
      << "                                      "
      << LRL_Cell_Degrees(vcells[vcells.size() - 1]) << std::endl;

}
