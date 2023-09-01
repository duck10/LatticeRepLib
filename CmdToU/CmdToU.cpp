#include <iostream>

#include "DC7u.h"
#include "LRL_ReadLatticeData.h"
#include "D7.h"
#include "B4.h"

int main()
{
   //DC7u x(G6("100 100 100 0 0 0"));

   //std::cout << DC7u(G6(G6("100 100 100 0 0 0"))) << std::endl;
   //std::cout << DC7u(S6(G6("100 100 100 0 0 0"))) << std::endl;
   //std::cout << DC7u(C3(G6("100 100 100 0 0 0"))) << std::endl;
   //std::cout << DC7u(D7(G6("100 100 100 0 0 0"))) << std::endl;
   //std::cout << DC7u(B4(G6("100 100 100 0 0 0"))) << std::endl;

   //DC7u xG6 = DC7u(G6("100 100 100 0 0 0"));
   std::cout << "; To u (DC7u)" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   for (size_t i = 0; i < inputList.size(); ++i) {
      std::cout << "u " << DC7u((const LRL_Cell)(inputList[i].GetCell())) << std::endl;
   }
}

