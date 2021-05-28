// SVD.cpp : Defines the entry point for the console application.
//
#include <iomanip>
#include <vector>

#include "LRL_ToString.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "svd.h"
#include "S6.h"

//int seed = 19191;

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

//void TestCreator() {
//   GenerateRandomLattice<S6> generator(seed);
//   const S6 s = generator.randSellingReduced();
//   for (size_t i = 0; i<24; ++i) {
//      std::cout << Creator(MatS6::GetReflection(i) * s) << std::endl;
//   }
//
//   exit(0);
//}

double DotS6(const S6& s1, const S6& s2) {
   const double d = s1[0] * s2[0] + s1[1] * s2[1] + s1[2] * s2[2] + s1[3] * s2[3] +
      s1[4] * s2[4] + s1[5] * s2[5];
   return d;
}

int main()
{
   std::cout << "; SVD" << std::endl;
   const std::vector<LRL_ReadLatticeData> cellData = LRL_ReadLatticeData().ReadLatticeData();

   std::vector<std::vector<double> > a;
   std::vector<std::vector<double> > v;
   std::vector<double> w;
   for (size_t n = 0; n < cellData.size(); ++n) {
      const S6& s = cellData[n].GetCell();
      a.push_back(s.GetVector());  // get the data into the data matrix
   }

   w.resize(a[0].size());
   v.resize(a.size());

   svdcmp(a, w, v);

   for (size_t i = 0; i < v.size(); ++i) {
      std::cout << std::setw(5) << std::right << w[i] << "      " << LRL_ToString(v[i]) << std::endl;
   }
   std::cout << std::endl;

   std::vector<S6> s(6);
   s[0] = S6(v[0]);
   s[1] = S6(v[1]);
   s[2] = S6(v[2]);
   s[3] = S6(v[3]);
   s[4] = S6(v[4]);
   s[5] = S6(v[5]);


   for (size_t i = 0; i < cellData.size(); ++i) {
      for (size_t n = 0; n < 6; ++n) {
         std::cout << DotS6(s[n], cellData[i].GetCell()) << "  " ;
      }
      std::cout << std::endl;
   }

   //TestCreator();
   return 0;
}

