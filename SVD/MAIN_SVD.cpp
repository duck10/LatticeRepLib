// SVD.cpp : Defines the entry point for the console application.
//
#include <vector>

#include "CellInputData.h"
#include "Delone.h"
#include "GenerateRandomLattice.h"
#include "LRL_ToString.h"
#include "LRL_ReadLatticeData.h"
#include "MatS6.h"
#include "svd.h"
#include "S6.h"
#include "S6Dist.h"

//S6 Creator( const S6&s ) {
//   S6 sout;
//   sout[0] = 0.408248*s[0] - 0.57735 * s[1] + 0.0*s[2] + 0.702284 * s[3] - 0.0661418*s[4] - 0.0492161*s[5];
//   sout[1] = 0.408248*s[0] + 0.288675* s[1] + 0.5*s[2] - 0.0241175* s[3] - 0.568472 *s[4] + 0.419831 *s[5];
//   sout[2] = 0.408248*s[0] + 0.288675* s[1] - 0.5*s[2] - 0.0788373* s[3] - 0.415289 *s[4] - 0.566851 *s[5];
//   sout[3] = 0.408248*s[0] - 0.57735 * s[1] - 0.0*s[2] - 0.702284 * s[3] + 0.0661418*s[4] + 0.0492161*s[5];
//   sout[4] = 0.408248*s[0] + 0.288675* s[1] + 0.5*s[2] + 0.0241175* s[3] + 0.568472 *s[4] - 0.419831 *s[5];
//   sout[5] = 0.408248*s[0] + 0.288675* s[1] - 0.5*s[2] + 0.0788373* s[3] +  0.415289*s[4] + 0.566851 *s[5];
//   return sout;
//}
//
//int seed = 19191;
//
//void TestCreator() {
//   GenerateRandomLattice<S6> generator(seed);
//   const S6 s = generator.randSellingReduced();
//   for (size_t i = 0; i<24; ++i) {
//      std::cout << Creator(MatS6::GetReflection(i) * s) << std::endl;
//   }
//
//   exit(0);
//}

int main()
{
   const std::vector<CellInputData> cellData = LRL_ReadLatticeData::ReadAllLatticeDataAndMakePrimitive(seed);  // read the data

   std::vector<std::vector<double> > a;
   std::vector<std::vector<double> > v;
   std::vector<double> w;
   for (size_t n = 0; n < cellData.size(); ++n) {
      const S6& s = cellData[n].GetCell();
      //std::cout << s << std::endl;
      a.push_back(s.GetVector());
   }
   std::cout << "; SVD" << std::endl;

   w.resize(a[0].size());
   v.resize(a.size());

   svdcmp(a, w, v);

   for (size_t i = 0; i<v.size(); ++i) {
      std::cout << w[i] << "      " << LRL_ToString(v[i]) << std::endl;
   }
   //std::cout << std::endl;

   //TestCreator();
      return 0;
}

