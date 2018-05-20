// SVD.cpp : Defines the entry point for the console application.
//
#include <vector>

#include "Delone.h"
#include "GenerateRandomLattice.h"
#include "LRL_ToString.h"
#include "MatS6.h"
#include "svd.h"
#include "S6.h"
#include "S6Dist.h"

S6 Creator( const S6&s ) {
   S6 sout;
   sout[0] = 0.408248*s[0] - 0.57735 * s[1] + 0.0*s[2] + 0.702284 * s[3] - 0.0661418*s[4] - 0.0492161*s[5];
   sout[1] = 0.408248*s[0] + 0.288675* s[1] + 0.5*s[2] - 0.0241175* s[3] - 0.568472 *s[4] + 0.419831 *s[5];
   sout[2] = 0.408248*s[0] + 0.288675* s[1] - 0.5*s[2] - 0.0788373* s[3] - 0.415289 *s[4] - 0.566851 *s[5];
   sout[3] = 0.408248*s[0] - 0.57735 * s[1] - 0.0*s[2] - 0.702284 * s[3] + 0.0661418*s[4] + 0.0492161*s[5];
   sout[4] = 0.408248*s[0] + 0.288675* s[1] + 0.5*s[2] + 0.0241175* s[3] + 0.568472 *s[4] - 0.419831 *s[5];
   sout[5] = 0.408248*s[0] + 0.288675* s[1] - 0.5*s[2] + 0.0788373* s[3] +  0.415289*s[4] + 0.566851 *s[5];
   return sout;
}

int seed = 19191;

void TestCreator() {
   GenerateRandomLattice<S6> generator(seed);
   const S6 s = generator.randSellingReduced();
   for (unsigned long i = 0; i<24; ++i) {
      std::cout << Creator(MatS6::GetReflection(i) * s) << std::endl;
   }

   exit(0);
}

int main()
{
   // std::vector< std::pair<S6(*)(const S6&), S6(*)(const S6&)> > S6Dist::SetUnreduceFunctionPairs() {
   const std::vector< std::pair<S6(*)(const S6&), S6(*)(const S6&)> > red = S6Dist::SetUnreduceFunctionPairs();
   //TestCreator();
   GenerateRandomLattice<S6> generator(seed);

   std::vector<std::vector<double> > a;
   std::vector<std::vector<double> > v;
   std::vector<double> w;
   for (unsigned long n = 0; n < 36; ++n) {
      const S6 ss = generator.randSellingReduced();
      S6 s(ss);
      //Delone::Reduce(ss, s);

      for (unsigned long i = 1; i < 6; ++i) {
         std::vector<double> vs;
         //const S6 smod = MatS6::GetReflection(i) * s;
         const S6 smod = red[0].first(s);
         //for (unsigned long k = 0; k < 6; ++k) {
         //   vs.push_back(s[k]);
         //}
         for (unsigned long k = 0; k < 6; ++k) {
            vs.push_back(smod[k]);
         }
         a.push_back(vs);
      }
   }

   w.resize(a[0].size());
   v.resize(a.size());

   svdcmp(a, w, v);

   for (unsigned long i = 0; i<v.size(); ++i) {
      std::cout << w[i] << "      " << LRL_ToString(v[i]) << std::endl;
   }
   std::cout << std::endl;

   //TestCreator();
      return 0;
}

