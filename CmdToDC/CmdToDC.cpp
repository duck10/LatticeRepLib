
#include <algorithm>
#include <iostream>
#include <list>
#include <utility>
#include <vector>

#include "BasisBase.h"
#include "Delone.h"
#include "DC.h"
#include "LRL_CreateFileName.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "LRL_Vector3.h"
#include "StoreResults.h"

void  Test1() {
   StoreResults<std::string, std::string> store(3);
   //static std::vector<std::pair<std::string, MatS6> > LoadLabeledLatticeTypeProjectors();
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
   std::cout << "; To DC" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   std::vector<DC> vds(inputList.size());
   for (size_t i = 0; i < inputList.size(); ++i) {
      vds[i] = inputList[i].GetCell();
   }

   std::clock_t start = std::clock();
   for (size_t i = 0; i < inputList.size(); ++i) {
      if (i > 0) std::cout << DC().DistanceBetween(inputList[i].GetCell(), inputList[i - 1].GetCell()) << std::endl;
   }
   const int timex = std::clock() - start;
   std::cout << "DC msec   " << timex << std::endl;
}
