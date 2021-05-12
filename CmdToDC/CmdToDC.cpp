
#include <algorithm>
#include <iostream>
#include <list>
#include <utility>
#include <vector>

#include "BasisBase.h"
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
void test2() {
   const LRL_Cell cell1("10 20 40 95 110 118");
   const LRL_Cell targetCell("10 20 40 66 77 88");
   const LRL_Cell_Degrees targetCelld(targetCell);
   const S6 targetS6 = targetCell;
   S6 red2;
   Selling::Reduce(targetS6, red2);

   const const int cycles = 100;
   const int start = 0;
   for (int i = -20; i < cycles+1; ++i) {
      const S6 cellX = (double(i) * S6(targetCell) + double(cycles - i) * S6(cell1))/double(cycles-start);
      S6 redX;
      Selling::Reduce(cellX, redX);
      const double dcDistance = DC::DistanceBetween(DC(cellX), DC(targetS6));
      const double csDistance = CS6Dist(redX.data(), red2.data());
      std::cout << dcDistance << "  " << csDistance << "  " << LRL_Cell_Degrees(cellX) << std::endl;
   }
   std::cout << "target cell " << LRL_Cell_Degrees(targetS6) << std::endl;

   exit(0);
}

void  Test1() {
   StoreResults<std::string, std::string> store(3);
   std::vector<std::pair<std::string, MatS6> > proj = Delone::LoadLabeledLatticeTypeProjectors();
   for (size_t kk = 0; kk < 100000; ++kk) {
      S6 r(S6::rand());
      r = proj[2].second * r;
      std::vector<std::pair<double, Vector_3> > v(DC::Cell_to_V13(LRL_Cell(r)));

      std::ostringstream f;
      std::ostringstream s;
      std::ostringstream t;
      for (size_t i = 0; i < 7; ++i) {
         f << v[i].second << "  ";
      }
      for (size_t i = 7; i < 13; ++i) {
         s << v[i].second << "  ";
      }
      for (size_t i = 0; i < 13; ++i) {
         t << v[i].first << "  ";
      }
      store.Store(f.str(), s.str() + "   " + t.str());
   }
   store.ShowResults();

   exit(0);
}

int main()
{
   std::cout << "; To DC" << std::endl;
   test2();
   Test1();
   //DC a("10 10 10  90 90 90");
   //DC b("P 10 10 10  90 90 90");
   //DirichletCell testDC("i 10 10 10  90 90 90");
   //DC DCA;
   //DCA.GetIndices();
   //DCA.GetStringIndices();
   //DC DCB(LRL_Cell(" 10 10 10   90 90 90"));
   //DCB.GetIndices();
   //DCB.GetStringIndices();

   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();

   for (size_t i = 0; i < inputList.size()-1; ++i) {
      S6 sred1;
      S6 sred2;

      const bool b1 = Selling::Reduce(inputList[i].GetCell(), sred1);
      const bool b2 = Selling::Reduce(inputList[i + 1].GetCell(), sred2);

      sred1 *= double(i + 1);
      G6 gred1;
      const bool b3 = Niggli::Reduce(sred1, gred1);
      G6 gred2;
      const bool b4 = Niggli::Reduce(sred2, gred2);
      const double d1 = (NCDist(gred1.GetVector().data(), gred2.GetVector().data())) / 1.0;
      const double d2 = (D7Dist(D7(sred1).data(), D7(sred2).data())) / 1.0;
      std::cout << "NCDist,D7Dist,DCdist " << d1 << "  " << d2 << "  " << DC::DistanceBetween(DC(sred1),DC(sred2)) << std::endl;
   }

   //std::vector<DC> vds(inputList.size());
   //for (size_t i = 0; i < inputList.size(); ++i) {
   //   vds[i] = inputList[i].GetCell();
   //}

   //std::clock_t start = std::clock();
   //for (size_t i = 0; i < inputList.size(); ++i) {
   //   if (i > 0) std::cout << DC().DistanceBetween(inputList[i].GetCell(), inputList[i - 1].GetCell()) << std::endl;
   //}
   //const int timex = std::clock() - start;
   //std::cout << "DC msec   " << timex << std::endl;
}
