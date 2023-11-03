
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

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

   std::vector<std::string> vLattices;
   std::vector<LRL_Cell> vcells;
   std::vector<G6> vg6Red;
   std::vector<S6> vs6Red;

   for (size_t i = 0; i < inputList.size(); ++i) {
      const std::string lattice = inputList[i].GetLattice();
      const LRL_Cell cell = inputList[i].GetCell();

      vcells.emplace_back(cell);
      vLattices.emplace_back(lattice);
      vg6Red.emplace_back(LatticeConverter::NiggliReduceCell (lattice, cell));
      vs6Red.emplace_back(LatticeConverter::SellingReduceCell(lattice, cell));
   }

   std::cout << ";   ordinals  G6-Euclidean     NCDist     CS6Dist ___  NCDist     CS6Dist"
      "    ordinals"<< std::endl;
   double* dG6_0 = vg6Red[0].data();
   double* dS6_0 = vs6Red[0].data();
   for (size_t i = 0; i < inputList.size()-1; ++i) {
      const double dG6 = NCDist (vg6Red[i].data(), vg6Red[i + 1].data());
      const double dS6 = CS6Dist(vs6Red[i].data(), vs6Red[i + 1].data());
      const double dEu = (G6(vcells[i]) - G6(vcells[i + 1])).norm();

      const double dG6A = NCDist(dG6_0, vg6Red[i + 1].data());
      const double dS6A = CS6Dist(dS6_0, vs6Red[i + 1].data());

      std::cout << ";   " << std::fixed << std::setprecision(3)
         << i+1 << " to " << i + 2 << "       " 
         << dEu << "       " << dG6 << "    " << dS6
         <<  " ___   " << dG6A << "    " << dS6A
         << "  " <<  1 << " to " << i + 2 << "       "
         << std::endl;
   }

   std::cout << std::endl;
   for (size_t i = 0; i < vcells.size(); ++i) {
      std::cout  << std::fixed << std::setprecision(3)
         << "  " + vLattices[i]
         << LRL_Cell_Degrees(vcells[i]) << "    " << i + 1 << std::endl;
   }

}
