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

int main()
{
   std::cout << "; SVD" << std::endl;
   const std::vector<LRL_ReadLatticeData> cellData = LRL_ReadLatticeData().ReadLatticeData();

   std::vector<std::vector<double> > a(cellData.size());
   for (size_t n = 0; n < cellData.size(); ++n) {
      const S6& s = cellData[n].GetCell();
      //std::cout << s << std::endl;
      a[n] = s.GetVector();  // get the data into the data matrix
   }

   std::vector<std::vector<double> > v(a[0].size());
   std::vector<double> w(a.size());

   svdcmp(a, w, v);

   for (size_t i = 0; i < v.size(); ++i) {
      std::cout << std::setw(5) << std::right << w[i] << "      " << LRL_ToString(v[i]) << std::endl;
   }

   //TestCreator();
   return 0;
}

