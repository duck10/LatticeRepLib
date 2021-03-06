// S6Classifier.cpp : Defines the entry point for the console application.
//

#include <algorithm>

#include "C3.h"
#include "GenerateRandomLattice.h"
#include "LRL_ToString.h"
#include "PairReporter.h"
#include "S6.h"
#include "S6Dist.h"
#include "StoreResults.h"


int seed = 1;

size_t ClassifyAsymmetricUnit(const C3& c3) {
   const bool b1 = abs(c3[0]) < abs(c3[1]);
   const bool b2 = abs(c3[1]) < abs(c3[2]);
   const bool b3 = c3[0].real() < c3[0].imag();
   const bool b4 = c3[1].real() < c3[1].imag();
   const bool b5 = c3[2].real() < c3[2].imag();
   const size_t key = (b1 ? 16 : 0) + (b2 ? 8 : 0) + (b3 ? 4 : 0) + (b4 ? 2 : 0) + (b5 ? 1 : 0);
   return key;
}


void TestClassification() {
   StoreResults<size_t, std::string> store(10);
   GenerateRandomLattice<C3> generator(seed);

   for (size_t i = 0; i < 1000; ++i) {
      const C3 c3 = C3::ConvertToFundamentalUnit(generator.randSellingReduced());
      const size_t key = ClassifyAsymmetricUnit(c3);
      const std::string str = LRL_ToString("C3", c3, "  S6", S6(c3), "");
      store.Store(key, str);
   }
   store.ShowResultsByKeyAscending();
}

size_t PositivePosition(const S6& s) {
   if (s.CountPositive() != 1) return 19191;
   size_t pos = 0;
   for (size_t i = 0; i < 6; ++i) {
      if (s[i] > 0.0) pos = i;
   }
   return pos;
}

std::string C3ToInt(const C3& c) {
   return
      "(" + LRL_ToString( std::round(c[0].real())) + ", " + LRL_ToString(std::round(c[0].imag())) + ") " +
      "(" + LRL_ToString( std::round(c[1].real())) + ", " + LRL_ToString(std::round(c[1].imag())) + ") " +
      "(" + LRL_ToString( std::round(c[2].real())) + ", " + LRL_ToString(std::round(c[2].imag())) + ")";
}

void IdentifyReductions() {
   StoreResults<std::string, std::string> store(10);
   GenerateRandomLattice<C3> generator(seed);
   S6 out;

   for (size_t i = 0; i < 100000; ++i) {
      C3 c3 = generator.randSellingUnreduced();
      const double c3normA = c3.norm();
      const double s6norm = S6(c3).norm();
      c3 = c3 / c3.norm();
      c3 *= 1000.0;
      const double c3normB = c3.norm();
      S6 s6(c3);
      if (s6.CountPositive() == 1){
         s6 = C3::ConvertToFundamentalUnit(c3);
         const bool b = Selling::Reduce(s6, out);
         if (Selling::GetCycles() == 1) {
            const size_t key = ClassifyAsymmetricUnit(c3);
            /*if (key == 25 || key == 24)*/ {
               const std::string datastring = LRL_ToString("C3", C3ToInt(C3(out)), "  \tC3 ", C3ToInt(C3(s6)), "");
               const std::string keystring = LRL_ToString(key, PositivePosition(s6));
               store.Store(keystring, datastring);
            }
         }
      }
   }
   store.SetTitle(" In C3, Reduced Cell and Unreduced, keyed by which \n "+
      std::string("C3 reflection and where the single + element is in S6\n" +
         std::string(" (starting from the C3 fundamental unit)")));
   store.ShowResultsByKeyAscending();
}

void ExamineDistance() {
   GenerateRandomLattice<C3> generator(seed);
   for ( size_t i=0; i<10000; ++i ) {
      C3 p1 = generator.randSellingReduced();
      p1 = 1000.0*p1 / p1.norm();
      C3 p2 = generator.randSellingReduced();
      p2 = 1000.0*p2 / p2.norm();
      double d1 = (p1 - p2).norm();
      double d2 = ((p1) - C3::ConvertToFundamentalUnit(p2)).norm();
      double d3 = (C3::ConvertToFundamentalUnit(p1) - (p2)).norm();
      double d4 = (C3::ConvertToFundamentalUnit(p1) - C3::ConvertToFundamentalUnit(p2)).norm();
      //std::cout << d1 << "   " << d2 << "   " << d3 <<  "   " << d4 << "     " << d4/d1 << std::endl;

      S6 s1(p1);
      S6 s2(p2);
      S6 s1a(C3::ConvertToFundamentalUnit(p1));
      S6 s2a(C3::ConvertToFundamentalUnit(p2));
      const double dot1 = s1[0] * s2[0] + s1[1] * s2[1] + s1[2] * s2[2] + s1[3] * s2[3] + s1[4] * s2[4] + s1[5] * s2[5];
      const double dot2 = s1a[0] * s2a[0] + s1a[1] * s2a[1] + s1a[2] * s2a[2] + s1a[3] * s2a[3] + s1a[4] * s2a[4] + s1a[5] * s2a[5];
      std::cout << "  " << 57.295 * acos(dot1 / s1.norm() / s2.norm()) << "  " << 57.295 * acos(dot2 / s1a.norm() / s2a.norm()) << std::endl;

   }
   exit(0);
}

int main() {

   //ExamineDistance();
   //TestClassification();
   IdentifyReductions();
   return 0;
}

