// TestC3.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

#include <iostream>

#include "C3.h"
#include "D7.h"
#include "G6.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "MatS6.h"
#include "Niggli.h"
#include "PairReporter.h"
#include "S6.h"
#include "S6Dist.h"
#include "Selling.h"
#include "StoreResults.h"

StoreResults<unsigned long, PairReporter<LRL_Cell_Degrees, LRL_Cell_Degrees> > g_storeResults(5);

G6 NiggliReduce(const LRL_Cell& cell) {
   G6 out;
   const bool b = Niggli::Reduce(G6(cell), out);
   if (!b) throw;
   return out;
}

unsigned long CountPositive(const S6& s6) {
   unsigned long sum = 0;
   for (unsigned long i = 0; i < 6; ++i)
      sum += (s6[i] > 0.0) ? 1 : 0;
   return sum;
}

unsigned long WhichPositive(const S6& s6) {
   for (unsigned long i = 0; i < 6; ++i) {
      if (s6[i] > 0.0) return i;
   }
   return 19191;
}

void TestC3Reduce() {
   //S6 s6in("0.1 -0.2 -0.25 -100 -110 -120");

      //const S6 s6refl = S6(MatS6::GetReflection(i) * S6(c3));
      for (unsigned long i = 0; i < 10000; ++i) {
      S6 s6in(S6::rand());
      if ( s6in.GetValid() && CountPositive(s6in) ==1 ) {
         S6 s6test;
         const unsigned long n24 = rand() % 24;
         s6in = S6(MatS6::GetReflection(n24) * s6in);
         const bool b = Selling::Reduce(s6in, s6test);
         if (CountPositive(s6test) == 0) {

            //std::cout << s6in << std::endl << std::endl << std::endl;
            //std::cout << s6test << std::endl << std::endl << std::endl;
            //std::cout << C3(s6in) << std::endl << std::endl << std::endl;
            //std::cout << C3(s6test) << std::endl << std::endl << std::endl;
            //std::cout << "reduce by function" << std::endl;
            g_storeResults.Store(WhichPositive(s6in), 
               PairReporter<LRL_Cell_Degrees, LRL_Cell_Degrees>(LRL_Cell_Degrees(LRL_Cell(s6in)), LRL_Cell_Degrees(LRL_Cell(s6test))));
         }
      }
   }
   g_storeResults.ShowResults();
   exit(0);

   //LRL_Cell cell1 = S6("0.1 -0.2 -0.25 -100 -110 -120");
   //LRL_Cell cell2 = S6("-0.2 -0.25 -100 0.1 -110 -120");
   //LRL_Cell cell3 = S6("-0.2 0.1 -0.25 -100 -110 -120");
   //LRL_Cell cell4 = S6("-0.2 -0.25 -100 -110 0.1 -120");
   //LRL_Cell cell5 = S6("-0.2 -0.25 0.1 -100 -110 -120");
   //LRL_Cell cell6 = S6("-0.2 -0.25 -100 -110 -120 0.1");
 /*
("-3.141 -88.485 -5.235 -134.578-6.280 -216.624")
("-134.578 -5.235-88.485 -3.141 -6.280 -216.624")
("-3.141 -88.485 -6.280 -216.624-5.235 -134.578")
("-216.624 -6.280-88.485 -3.141 -5.235 -134.578")
("-134.578 -5.235-6.280 -216.624-88.485 -3.141 ")
("-216.624 -6.280-5.235 -134.578-88.485 -3.141 ")
*/

   LRL_Cell cell1(("10 12 15 91 92 93"));
   LRL_Cell cell2(("10 12 15 91 92 93"));
   LRL_Cell cell3(("10 12 15 91 92 93"));
   LRL_Cell cell4(("10 12 15 91 92 93"));
   LRL_Cell cell5(("10 12 15 91 92 93"));
   LRL_Cell cell6(("10 12 15 91 92 93"));

   std::cout << "input cells " << std::endl;
    std::cout << LRL_Cell_Degrees(cell1) << std::endl;
    std::cout << LRL_Cell_Degrees(cell2) << std::endl;
    std::cout << LRL_Cell_Degrees(cell3) << std::endl;
    std::cout << LRL_Cell_Degrees(cell4) << std::endl;
    std::cout << LRL_Cell_Degrees(cell5) << std::endl;
    std::cout << LRL_Cell_Degrees(cell6) << std::endl;
    std::cout << std::endl;

    std::cout << "Niggli reduced G6 vectors" << std::endl;
    std::cout << NiggliReduce(cell1) << std::endl;
    std::cout << NiggliReduce(cell2) << std::endl;
    std::cout << NiggliReduce(cell3) << std::endl;
    std::cout << NiggliReduce(cell4) << std::endl;
    std::cout << NiggliReduce(cell5) << std::endl;
    std::cout << NiggliReduce(cell6) << std::endl;
    std::cout << std::endl;




   const C3 red1 = C3::m_reductionFunctions[0](S6(cell1));
   const C3 red2 = C3::m_reductionFunctions[1](S6(cell2));
   const C3 red3 = C3::m_reductionFunctions[2](S6(cell3));
   const C3 red4 = C3::m_reductionFunctions[3](S6(cell4));
   const C3 red5 = C3::m_reductionFunctions[4](S6(cell5));
   const C3 red6 = C3::m_reductionFunctions[5](S6(cell6));

   std::cout << "reduced C3 vectors " << std::endl;
   std::cout << red1 << std::endl;
   std::cout << red2 << std::endl;
   std::cout << red3 << std::endl;
   std::cout << red4 << std::endl;
   std::cout << red5 << std::endl;
   std::cout << red6 << std::endl;
   std::cout << std::endl;

   std::cout << "reduced C3 vectors as Niggli ReducedG6 " << std::endl;
   std::cout << NiggliReduce(G6(red1)) << std::endl;
   std::cout << NiggliReduce(G6(red2)) << std::endl;
   std::cout << NiggliReduce(G6(red3)) << std::endl;
   std::cout << NiggliReduce(G6(red4)) << std::endl;
   std::cout << NiggliReduce(G6(red5)) << std::endl;
   std::cout << NiggliReduce(G6(red6)) << std::endl;
   std::cout << std::endl;

   std::cout << "reduced C3 vectors as G6 " << std::endl;
   std::cout << G6(red1) << std::endl;
   std::cout << G6(red2) << std::endl;
   std::cout << G6(red3) << std::endl;
   std::cout << G6(red4) << std::endl;
   std::cout << G6(red5) << std::endl;
   std::cout << G6(red6) << std::endl;
   std::cout << std::endl;



   std::cout << " report reduced C3 vectors " << std::endl;
   red1.report();
   red2.report();
   red3.report();
   red4.report();
   red5.report();
   red6.report();
   std::cout << std::endl;

}
/*  0.10000  -0.20000  -0.25000 -100.00000 -110.00000 -120.00000


 -0.10000  -0.10000 -109.90000 -100.10000  -0.15000 -119.90000


(0.100,-100.000) (-0.200,-110.000) (-0.250,-120.000)


(-0.100,-100.100) (-0.100,-0.150) (-109.900,-119.900)


reduce by function
(-0.100,-100.100) (-0.100,-0.150) (-109.900,-119.900)
(-0.300,-0.100) (-0.150,-99.900) (-109.900,-119.900)
(-0.100,-0.150) (-0.100,-110.100) (-99.900,-119.900)
(-0.100,-99.900) (0.000,-0.100) (-109.900,-119.900)
(-0.100,-0.500) (-99.900,-110.250) (-0.100,-120.100)
(-0.100,-0.150) (-109.900,-119.900) (-100.100,-0.100)

*/

template<typename T1, typename T2>
int CheckEquality(const T1& t1, const T2& t2) {
   const double diff = (G6(t1) - G6(t2)).norm();
   int error = 0;
   if (diff / t1.norm() > 1.0E-10) {
      error = 1;
      std::cout << std::endl;
      std::cout << " DIFF " << diff << "  " << diff / t1.norm() << std::endl;
      std::cout << " t1 " << t1.GetName() << "  " << t1 << std::endl;
      std::cout << " t1 " << S6::GetName() << "  " << S6(t1) << std::endl;
      std::cout << " t2 " << t2.GetName() << "  " << t2 << std::endl;
   }
   return error;
}

bool CheckReflectionsC3(const C3& c3) {
   G6 g6out;
   std::vector< C3(*)(const C3&)> fnRefl = C3::SetReflections();
   const C3 checkValue = NiggliReduce(fnRefl[0](c3));
   unsigned long errorcount = 0;

   for (unsigned long i = 0; i < fnRefl.size(); ++i) {
      const S6 s6refl = fnRefl[i]( c3);
      const bool b = Niggli::Reduce(G6(s6refl), g6out);
      if (CheckEquality(g6out, checkValue) != 0) {
         std::cout << "reflect by C3 function fails " << i << "  " << g6out << std::endl;
         ++errorcount;
      }
   }
   if (errorcount == 0) std::cout << "no errors in CheckReflectionsC3" << std::endl;
   return errorcount == 0;
}

bool CheckReductionC3(const C3& c3) {
   G6 g6out;
   const C3 checkValue = NiggliReduce(S6(c3));
   const std::vector< C3(*)(const C3&)> fnRedn = C3::SetReduceFunctions();
   unsigned long errorcount = 0;

   for (unsigned long i = 0; i < fnRedn.size(); ++i) {
      const bool b = Niggli::Reduce(G6(fnRedn[i](c3)), g6out);
      if (CheckEquality(g6out, checkValue) != 0) {
         std::cout << i << " reduce by C3 function fails " << g6out << std::endl;
         ++errorcount;
      }
   }
   if (errorcount == 0) std::cout << "no errors in CheckReductionC3" << std::endl;
   return errorcount == 0;
}

bool CheckReductionS6(const C3& c3) {
   G6 g6out;
   const S6 checkValue = NiggliReduce(S6(c3));
   const std::vector< S6(*)(const S6&)> fnRedn = S6::SetReduceFunctions();
   unsigned long errorcount = 0;

   for (unsigned long i = 0; i < fnRedn.size(); ++i) {
      const bool b = Niggli::Reduce(G6(S6(fnRedn[i](S6(c3)))), g6out);
      if (CheckEquality(g6out, checkValue) != 0) {
         std::cout << i << " reflect by S6 function fails " << g6out << std::endl;
         ++errorcount;
      }
   }
   if (errorcount == 0) std::cout << "no errors in CheckReductionS6" << std::endl;
   return errorcount == 0;
}

bool CheckReflectionsS6(const std::vector<C3>& vReflections, const C3& c3) {
   G6 g6out;
   const C3 checkValue = NiggliReduce(S6(c3));
   unsigned long errorcount = 0;

   for (unsigned long i = 0; i < vReflections.size(); ++i) {
      const S6 s6refl = S6(MatS6::GetReflection(i) * S6(c3));
      const bool b = Niggli::Reduce(G6(s6refl), g6out);
      if (CheckEquality(g6out, checkValue) != 0) {
         std::cout << "reflect by S6 matrix fails " << i << "  " << g6out << std::endl;
         ++errorcount;
      }
   }
   if (errorcount == 0) std::cout << "no errors in CheckReflectionsS6" << std::endl;
   return errorcount == 0;
}

std::vector<C3> CreateVectorOfReflections(const C3& c3) {
   std::vector<C3> v;
   for (unsigned long i = 0; i < 24; ++i) {
      v.push_back( C3(D7(MatS6::GetReflection(i) * S6(c3))));
   }
   return v;
}
int main()
{
   LRL_Cell cell("10 12 15 89 88 87");
   S6 s6(cell);
   const C3 c3(s6);
   CheckReductionC3(c3);
   CheckReflectionsC3(c3);
   const std::vector<C3> vReflections(CreateVectorOfReflections(c3));
   CheckReductionS6(c3);
   CheckReflectionsS6(vReflections, c3);
   exit(0);
   TestC3Reduce();

   std::cout << c3 << std::endl << std::endl << std::endl;
   c3.report();
   std::cout << std::endl << std::endl;

   S6Dist s6dist(10000);
   for (unsigned long i = 0; i < 24; ++i) {
      std::cout << C3(S6(MatS6::GetReflection(i) * S6(c3))) << std::endl;
   }
   std::cout << std::endl << std::endl;
   for (unsigned long i = 0; i < 24; ++i) {
      const C3 temp = C3(S6(MatS6::GetReflection(i) * S6(c3)));
      temp.report();
   }
   std::cout << std::endl << std::endl;

   //std::cout << C3(S6(S6Dist::Unreduce11(S6(c3)))) << std::endl;
   //std::cout << C3(S6(S6Dist::Unreduce21(S6(c3)))) << std::endl;
   //std::cout << C3(S6(S6Dist::Unreduce31(S6(c3)))) << std::endl;
   //std::cout << C3(S6(S6Dist::Unreduce41(S6(c3)))) << std::endl;
   //std::cout << C3(S6(S6Dist::Unreduce51(S6(c3)))) << std::endl;
   //std::cout << C3(S6(S6Dist::Unreduce61(S6(c3)))) << std::endl;
   //std::cout << std::endl << std::endl;

   //const std::vector< C3(*)(const C3&)> vr = C3::SetUnreduceFunctionPairs();

   //for (unsigned long i = 0; i < vr.size(); ++i) {
   //   std::cout << vr[i](c3) << std::endl;
   //}

   std::cout << std::endl << std::endl << LRL_ToString(c3) << std::endl;

   return 0;
}

