
#include <cmath>
#include <iostream>

#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_CreateFileName.h"
#include "LatticeConverter.h"
#include "LRL_ReadLatticeData.h"
#include "NCDist.h"
#include "S6.h"
#include "CS6Dist.h"
#include "CS6Dist.cpp"
int main()
{

   std::vector<S6> reduced;
   std::cout << "; Distances" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();

   for (size_t i = 0; i < inputList.size(); ++i) {
      //const LRL_Cell pCell = LatticeConverter::MakePrimitiveCell(inputList[i].GetLattice(), inputList[i].GetCell());
      S6 s(inputList[i].GetCell());
      //const bool b = Selling::Reduce(S6(pCell), s);

   std::vector<S6> vclose;
   std::vector<size_t> indices;
   int maxCount = 0;
   int countPairsFound = 0;
   for (size_t kk = 0; kk < inputList.size(); ++kk) {
      const int count = tree.FindK_NearestNeighbors(10, 20., vclose, indices, S6(inputList[kk].GetCell()));
      maxCount = std::max(maxCount, count);
      if (count > 1) {
         const double d = CS6Dist(vclose[1].data(), S6(inputList[kk].GetCell()).data());
         if (d < 5.0 && d > 1.0E-6) {
            std::cout << d << "\n" << inputList[kk].GetStrCell() << "\n" 
               << inputList[indices[1]].GetStrCell() << std::endl << std::endl;
            ++countPairsFound;
            ++kk;
         }
      }
   }




}
