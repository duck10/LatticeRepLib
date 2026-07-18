
#include <algorithm>
#include <iostream>
#include <list>
#include <utility>
#include <vector>

#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include "D7Dist.h"
#include "Delone.h"
#include "DC.h"
#include "LRL_CreateFileName.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "LRL_Vector3.h"
#include "NCDist.h"
#include "Niggli.h"
#include "StoreResults.h"

int main()
{
   std::cout << "; To DC (squares)" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();

   std::vector<DC> vds(inputList.size());
   for (size_t i = 0; i < inputList.size(); ++i) {
      vds[i] = inputList[i].GetCell();
   }

   for (size_t i = 0; i < inputList.size(); ++i)
   {
      const DC dcA(inputList[i].GetCell());
      std::cout << "DC " << dcA << std::endl;
   }
}
