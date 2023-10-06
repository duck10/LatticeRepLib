
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
   std::cout << "; CmdDists, calculate distances" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
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

      vcells.push_back(cell);
      vg6Red.push_back(G6(LatticeConverter().NiggliReduceCell(lattice, cell)));
      const S6 s6temp = LatticeConverter().SellingReduceCell(lattice, cell);
      vs6Red.push_back(s6temp);
      //std::cout << "vs6Red.back() " << vs6Red.back() << std::endl;
      //std::cout << S6(LatticeConverter().SellingReduceCell(lattice, cell)) << std::endl;
   }

   std::cout << ";   ordinals  G6-Euclidean   NCDist   CS6Dist" << std::endl;
   for (size_t i = 0; i < inputList.size()-1; ++i) {
      const double dG6 = NCDist (vg6Red[i].data(), vg6Red[i + 1].data());
      const double dS6 = CS6Dist(vs6Red[i].data(), vs6Red[i + 1].data());
      const double dEu = (G6(vcells[i]) - G6(vcells[i + 1])).norm();

      //std::cout << "vcell" << vcells[i] << std::endl;
      //std::cout << "vcell" << vcells[i+1] << std::endl;
      //std::cout << "vg6Red" << vg6Red[i] << std::endl << std::endl;
      //std::cout << "vg6Red" << vg6Red[i+1] << std::endl;
      //std::cout << "vs6Red" << vs6Red[i] << std::endl;
      //std::cout << "vs6Red" << vs6Red[i+1] << std::endl;

      std::cout << ";   " << std::fixed << std::setprecision(3)
         << i+1 << " to " << i + 2 << "       " 
         << dEu << "       " << dG6 << "    " << dS6 << std::endl;
   }

   std::cout << std::endl;
   for (size_t i = 0; i < vcells.size(); ++i) {
      std::cout << ";    " << i+1 << std::fixed << std::setprecision(3)
         << "  "
         << LRL_Cell_Degrees(vcells[i]) << std::endl;
   }

}
