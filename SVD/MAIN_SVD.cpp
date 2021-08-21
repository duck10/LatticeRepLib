// SVD.cpp : Defines the entry point for the console application.
//
#include <iomanip>
#include <vector>

#include "DC.h"
#include "LatticeConverter.h"
#include "LRL_ToString.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "Niggli.h"
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

void DoSVD( std::vector<std::vector<double> >& a) {
   std::vector<std::vector<double> > v;
   std::vector<double> w;

   w.resize(a[0].size());
   v.resize(a.size());

   svdcmp(a, w, v);

   std::cout << std::endl;
   for (size_t i = 0; i < v.size(); ++i) {
      std::cout << "eigenvalue " << std::setw(5) << std::right << w[i] << "    \t vector  \t";
      for (size_t k = 0; k < v.size(); ++k) {
         std::cout << v[k][i] << " ";
      }
      std::cout << std::endl;
   }
   std::cout << std::endl;
}

int main()
{
   std::cout << "; SVD" << std::endl;
   std::vector<LRL_ReadLatticeData> inputList;
   const std::string type = "DC13";    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   if ( type != "DC13")
      inputList = LRL_ReadLatticeData().ReadLatticeData();
   std::vector<LRL_Cell> vcell;
   for (size_t i = 0; i < inputList.size(); ++i) {
      const LRL_Cell pCell = LatticeConverter::MakePrimitiveCell(inputList[i].GetLattice(), inputList[i].GetCell());
      vcell.push_back(pCell);
   }

   std::vector<std::vector<double> > a;


   if (type == "S6") {
      for (size_t n = 0; n < vcell.size(); ++n) {
         const S6& s = vcell[n];
         std::cout << "S6 from input cell " << s << std::endl;
         a.push_back(s.GetVector());  // get the data into the data matrix
      }
      DoSVD(a);
   }
   else if (type == "DC7" || type == "DC13") {
      vcell.clear();
      for (size_t i = 0; i < 1000; ++i) {
         G6 gout;
         const bool b = Niggli::Reduce(G6::rand(), gout);
         vcell.push_back(gout);
      }

      std::vector<double> d;
      for (size_t i = 0; i < vcell.size(); ++i) {
         if (type == "DC7")
            d = DC(vcell[i]).GetVector();
         else if (type == "DC13") {
            d = DC::G6_to_V13(vcell[i]);
            const size_t DCLength = 13;
            d.resize(DCLength);
            if (i == 0) std::cout << "DC vector length = " << DCLength << std::endl;
         }
         a.push_back(d);
      }
      DoSVD(a);
   }



   //TestCreator();
   return 0;
}

