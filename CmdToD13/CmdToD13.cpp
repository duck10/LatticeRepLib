
#include <iostream>

#include "D13.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "LRL_Vector3.h"

#include <vector>

int main()
{
   //D13 a("10 10 10  90 90 90");
   //D13 b("P 10 10 10  90 90 90");
   //DirichletCell testDC("i 10 10 10  90 90 90");
   //D13 d13A;
   //d13A.GetIndices();
   //d13A.GetStringIndices();
   //D13 d13B(LRL_Cell(" 10 10 10   90 90 90"));
   //d13B.GetIndices();
   //d13B.GetStringIndices();
   std::cout << "; To D13" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   for (size_t i = 0; i < inputList.size(); ++i) {
      const D13 d13(inputList[i].GetStrCell());
      std::cout << "D13 " << LRL_ToString(d13.GetAreas()) << std::endl;
      std::cout << " " << LRL_ToString(d13.GetStringIndices()) << std::endl;
   }
}
