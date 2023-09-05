
#include <iostream>

#include "LRL_MinMaxTools.h"
#include "LRL_ToString.h"
#include "LRL_ReadLatticeData.h"
#include "DC7u.h"
#include "MatDC7u.h"
#include <vector>

template<typename T>
double sqr(const T a) { return a * a; }

double DC7uReflDist(const DC7u& dc_1, const DC7u dc_2) {
   const double d1 = sqrt(sqr(dc_1[0] - dc_2[0]) + sqr(dc_1[1] - dc_2[1]) + sqr(dc_1[2] - dc_2[2]) + sqr(dc_1[3] - dc_2[3]) + sqr(dc_1[4] - dc_2[4]) + sqr(dc_1[5] - dc_2[5]) + sqr(dc_1[6] - dc_2[6]));
   const double d2 = sqrt(sqr(dc_1[0] - dc_2[0]) + sqr(dc_1[1] - dc_2[2]) + sqr(dc_1[1] - dc_2[1]) + sqr(dc_1[3] - dc_2[3]) + sqr(dc_1[4] - dc_2[5]) + sqr(dc_1[5] - dc_2[4]) + sqr(dc_1[6] - dc_2[6]));
   const double d3 = sqrt(sqr(dc_1[0] - dc_2[1]) + sqr(dc_1[1] - dc_2[0]) + sqr(dc_1[1] - dc_2[2]) + sqr(dc_1[3] - dc_2[4]) + sqr(dc_1[4] - dc_2[3]) + sqr(dc_1[5] - dc_2[5]) + sqr(dc_1[6] - dc_2[6]));
   const double d4 = sqrt(sqr(dc_1[0] - dc_2[1]) + sqr(dc_1[1] - dc_2[2]) + sqr(dc_1[1] - dc_2[0]) + sqr(dc_1[3] - dc_2[4]) + sqr(dc_1[4] - dc_2[5]) + sqr(dc_1[5] - dc_2[3]) + sqr(dc_1[6] - dc_2[6]));
   const double d5 = sqrt(sqr(dc_1[0] - dc_2[2]) + sqr(dc_1[1] - dc_2[0]) + sqr(dc_1[1] - dc_2[1]) + sqr(dc_1[3] - dc_2[5]) + sqr(dc_1[4] - dc_2[3]) + sqr(dc_1[5] - dc_2[4]) + sqr(dc_1[6] - dc_2[6]));
   const double d6 = sqrt(sqr(dc_1[0] - dc_2[2]) + sqr(dc_1[1] - dc_2[1]) + sqr(dc_1[1] - dc_2[0]) + sqr(dc_1[3] - dc_2[5]) + sqr(dc_1[4] - dc_2[4]) + sqr(dc_1[5] - dc_2[3]) + sqr(dc_1[6] - dc_2[6]));
   const double dist = minNC(d1, d2, d3, d4, d5, d6);
   //std::cout << dc7_1 << std::endl;
   //std::cout << dc7_2 << std::endl;
   return dist;
}

static bool CheckDC7u( const DC7u& dc7, const int& line=0) {
   bool good = true;
   std::string s(line!=0?LRL_ToString(" line=",line) : "");
   std::string issues;
   if (dc7.GetVector().size() != 7) {
      good = false;
      issues.append(LRL_ToString("   vector size equal to  ", 
         dc7.GetVector().size(),"\n"));
   }
   if (dc7.GetDim() != 7) {
      good = false;
      issues.append(LRL_ToString("   m_dim not equal to 7",
         dc7.GetDim(), "\n"));
      issues.append(LRL_ToString("   ", "\n"));
   }
   if (dc7.GetCell().GetVector().size() != 6) {
      good = false;
      issues.append(LRL_ToString("   cell dim not 6 ",
         dc7.GetCell().GetVector().size(),"\n"));
   }
   if (dc7.norm() == 0.0) {
      good = false;
      issues.append(LRL_ToString("   norm == 0.0", "\n"));
   }
   if (!dc7.GetValid()) {
      good = false;
      issues.append(LRL_ToString("   m_valid == false", "\n"));
   }

   if (!good) {
      std::cout << LRL_ToString("\n*********** problem DC7u, ", s, "\n")
         << "DC7U " << dc7 << std::endl
         << " m_cell " << dc7.GetCell() << std::endl
         << issues << std::endl;
   }
   return good;
}

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

  // m3[0] = -39393;
  // std::cout << m3 << std::endl << std::endl;
  // std::cout << m3 * 2 << std::endl << std::endl;
  // std::cout << m3 / 2 << std::endl << std::endl;
  // std::cout << m3 * m3 << std::endl << std::endl;

   LRL_Cell cell("10 12 14 86 87 88");
   DC7u fromCell(cell);
   CheckDC7u(fromCell);
   CheckDC7u(m3 * fromCell, __LINE__);
   //std::cout << m3 * dc7u << std::endl;

   //DC7u dc7u("1 1 1 1 1 1 1"); // expected to be invalid
   //CheckDC7u(dc7u, __LINE__); // expected to be invalid

   DC7u xxxx(LRL_Cell("10 10 10 90 90 90"));
   //std::cout << xxxx << std::endl;
   CheckDC7u(xxxx, __LINE__);

   DC7u dc1(G6("100 100 100 0 0 0"));
   CheckDC7u(dc1, __LINE__);
   DC7u dc2(G6("200 200 200 0 0 10"));
   CheckDC7u(dc2);

   //std::cout << "d1  " << d1 << std::endl;
   //std::cout << "d2  " << d2 << std::endl;
   //std::cout << "minus  " << d2 - d1 << std::endl;
   CheckDC7u(5.* dc1, __LINE__);
   ////std::cout << "plus   " << d2 + d1 << std::endl;
   CheckDC7u(dc2*5., __LINE__);

   DC7u dc7_1(LRL_Cell("10 10 10 90 90 90"));
   DC7u dc7_2 = 2 * dc7_1;
   std::cout << "dc7_1 " << dc7_1 << std::endl;
   std::cout << "dc7_2 " << dc7_2 << std::endl;
   std::cout << DC7uReflDist(dc7_1, dc7_2) << std::endl;
}
