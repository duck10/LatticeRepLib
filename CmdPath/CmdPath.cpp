
#include <iostream>
#include <vector>

#include "FollowerConstants.h"
#include"LatticeConverter.h"
#include "LRL_ReadLatticeData.h"
#include "G6.h"
#include "Niggli.h"

std::vector<S6> CreatePathBetweenTwoPoints(const int npts, const S6& delta,  const S6& s1, const S6& s2) {
   std::vector<S6> out(1,s1);
   for (int i = 0; i < npts-1; ++i) {
      const S6 diff = double(i+1) * delta;
      const S6 temp = s1 + diff;
      out.push_back(temp);
   }

   return out;
}

int main(int argc, char* argv[])
{
   std::cout << "; Path generator" << std::endl;
   int test = 20;
   int pathLength = 0;
   if (argc > 1) {
      test = atoi(argv[1]);
      if (test == 0) test = 20;
   }
   pathLength = test;



    const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
    std::vector<S6> s6Cells;
    for (int i = 0; i < inputList.size(); ++i) {
       s6Cells.push_back(inputList[i].GetCell());
    }

    if (inputList.empty()) exit(99);

    //const std::string lattice = std::string(inputList[0].GetStrCell()[0]);
    //const G6 g6red = LatticeConverter().NiggliReduceCell(lattice, s6Cells[0]);

    G6 g6red;
    if (s6Cells.size() == 1) {
       const std::string lattice = inputList[0].GetLattice();
       const G6 g6red = LatticeConverter().NiggliReduceCell(lattice, s6Cells[0]);
       s6Cells.push_back(g6red);
    }

   std::vector<S6> path;

   for (size_t i = 0; i < s6Cells.size()-1; ++i) {
      const S6 delta = (s6Cells[i + 1] - s6Cells[i]) / double(pathLength - 1);
      const std::vector<S6> pathStep = CreatePathBetweenTwoPoints(pathLength, delta, s6Cells[i], s6Cells[i+1]);
      path.insert(path.end(), pathStep.begin(), pathStep.end());
   }

   std::cout << "; no. of points output is " << path.size() << std::endl;
   for (size_t i = 0; i < path.size(); ++i) {
      std::cout << "S  " << path[i] << "   " << i+1 << std::endl;
   }

}
