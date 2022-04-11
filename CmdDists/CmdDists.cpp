
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
#include "TNear.h"
int main()
{
   std::string beforeInput;
   std::string afterInput;
   std::string afterTreeBuild;
   std::string afterMaxMinSearch;
   std::string afterKNearSearch;
   CNearTree<S6> tree;

   beforeInput = LRL_CreateFileName().Create("", "");     //TIME *******************

   std::vector<S6> reduced;
   std::cout << "; Distances" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();

   afterInput = LRL_CreateFileName().Create("", "");     //TIME *******************

   for (size_t i = 0; i < inputList.size(); ++i) {
      //const LRL_Cell pCell = LatticeConverter::MakePrimitiveCell(inputList[i].GetLattice(), inputList[i].GetCell());
      S6 s(inputList[i].GetCell());
      //const bool b = Selling::Reduce(S6(pCell), s);
      tree.insert(s);
   }
   afterTreeBuild = LRL_CreateFileName().Create("", "");     //TIME *******************
   std::cout << "tree size " << tree.size() << std::endl;

   double dmin = DBL_MAX;
   //double dmax = -dmin;
   //for (size_t kk = 0; kk < 20; ++kk) {
   //   for (size_t i = 0; i < inputList.size(); ++i) {
   //      const double d = CS6Dist(
   //         S6(inputList[i].GetCell()).data(),
   //         S6(inputList[0].GetCell()).data());
   //      dmax = std::max(dmax, d);
   //      dmin = std::min(dmin, d);
   //   }
   //}
   afterMaxMinSearch = LRL_CreateFileName().Create("", "");

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
            //if (countPairsFound > 30) exit(0);
         }
      }
   }

   afterKNearSearch = LRL_CreateFileName().Create("", "");

   std::cout << "before input " << beforeInput << std::endl;
   std::cout << "after input " << afterInput << std::endl;
   std::cout << "after tree buld " << afterTreeBuild << std::endl;
   //std::cout << "dmax  " << dmax << "  dmin  " << dmin << std::endl;
   std::cout << "max index returned Knear " << maxCount << std::endl;
   std::cout << "after min max search " << afterMaxMinSearch << std::endl; 
      std::cout << "after K-near search " << afterKNearSearch << std::endl; 
      std::cout << "count of near pairs " << countPairsFound << std::endl;

   //for (size_t i = 0; i < reduced.size()-1; ++i) {
   //   for (size_t kk = i + 1; kk < reduced.size(); ++kk) {
   //      std::cout << "i " << i << " kk " << kk << "  "
   //         << CS6Dist(reduced[i].data(), reduced[kk].data())
   //         << std::endl;
   //   }
   //}
   //std::string input;
   //std::string data;
   //std::vector<std::string> cell;
 
   //while (std::cin) {
   //   std::getline(std::cin, input);
   //   if ("data" == input.substr(0, 4)) data = " "+input;
   //   if ("_cell" == input.substr(0, 5)) cell.push_back(input);
   //   if (cell.size() == 6) {
   //      for (size_t i = 0; i < cell.size(); ++i) {
   //         std::cout << cell[i];
   //      }
   //      std::cout << data << std::endl;
   //      cell.clear();
   //      data.clear();
   //   }
   //}

   // read file line by line

}
