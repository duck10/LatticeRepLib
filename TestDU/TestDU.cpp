
#include <iostream>

#include "LRL_ReadLatticeData.h"
#include "DC7u.h"
#include "MatDC7u.h"
#include <vector>

int main()
{
   //LRL_ReadLatticeData reader;
   std::cout << "; Test DC7unsrt" << std::endl;
   //const std::vector<LRL_ReadLatticeData> inputList = reader.ReadLatticeData();
   //for (size_t i = 0; i < inputList.size(); ++i) {
   //   std::cout << inputList[i].GetLattice() << ""
   //      << LRL_Cell_Degrees(inputList[i].GetCell()) << std::endl;
   //}

   MatDC7u m3; m3.SetVector({
  6. / 7, -1. / 7, -1. / 7, 1. / 7, 1. / 7, 1. / 7, -1. / 7,
  -1. / 7, 5. / 14, 5. / 14, 1. / 7, 1. / 7, 1. / 7, -1. / 7,
  -1. / 7, 5. / 14, 5. / 14, 1. / 7, 1. / 7, 1. / 7, -1. / 7,
  1. / 7, 1. / 7, 1. / 7, 6. / 7, -1. / 7, -1. / 7, 1. / 7,
  1. / 7, 1. / 7, 1. / 7, -1. / 7, 6. / 7, -1. / 7, 1. / 7,
  1. / 7, 1. / 7, 1. / 7, -1. / 7, -1. / 7, 6. / 7, 1. / 7,
  -1. / 7, -1. / 7, -1. / 7, 1. / 7, 1. / 7, 1. / 7, 6. / 7
      });
   m3 = m3 * 0.2;
   m3 = 5. * m3;
   m3 *= 3;
   m3 /= 3;
   m3 = m3 / 5;

   m3[0] = -39393;
   std::cout << m3 << std::endl << std::endl;
   std::cout << m3 * 2 << std::endl << std::endl;
   std::cout << m3 / 2 << std::endl << std::endl;
   std::cout << m3 * m3 << std::endl << std::endl;

   DC7u dc7u("1 1 1 1 1 1 1");
   std::cout << m3 * dc7u << std::endl;

   DC7u xxxx(LRL_Cell("10 10 10 90 90 90"));
   std::cout << xxxx << std::endl;

   DC7u d1(G6("100 100 100 0 0 0"));
   DC7u d2(G6("200 200 200 0 0 10"));

   std::cout << "d1  " << d1 << std::endl;
   std::cout << "d2  " << d2 << std::endl;
   std::cout << "minus  " << d2 - d1 << std::endl;
   std::cout << "plus   " << d2 + d1 << std::endl;
}
