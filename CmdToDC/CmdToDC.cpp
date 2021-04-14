
#include <iostream>
#include <list>
#include <vector>

#include "BasisBase.h"
#include "DC.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "LRL_Vector3.h"


int main()
{
   std::list<BasisBase<DC>*> theList;
   //DC a("10 10 10  90 90 90");
   //DC b("P 10 10 10  90 90 90");
   //DirichletCell testDC("i 10 10 10  90 90 90");
   //DC DCA;
   //DCA.GetIndices();
   //DCA.GetStringIndices();
   //DC DCB(LRL_Cell(" 10 10 10   90 90 90"));
   //DCB.GetIndices();
   //DCB.GetStringIndices();
   std::cout << "; To DC" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   for (size_t i = 0; i < inputList.size(); ++i) {
      const DC dc(inputList[i].GetStrCell());
      //std::cout << "DC " << LRL_ToString(dc.GetAreas()) << std::endl;
      std::cout << " " << LRL_ToString(dc.GetStringIndices()) << std::endl;
      std::cout << " " << LRL_ToString(dc.GetVector()) << std::endl;
   }
}
