// Testbed.cpp : Defines the entry point for the console application.
//

#include "CellInputData.h"
#include "CS6Dist.h"
#include "Delone.h"
#include "LRL_CreateFileName.h"
#include "CS6Dist.h"
#include "CS6Dist.c"
#include "DeloneTypeList.h"
#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_LatticeMatcher.h"
#include "LRL_MaximaTools.h"
#include "LRL_MinMaxTools.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "MatN.h"
#include "MatS6.h"
#include "NCDist.h"
#include "PairReporter.h"
#include "S6.h"
#include "S6Dist.h"
#include "TNear.h"

#include <algorithm>
#include <utility>
#include <complex>
#include <cstdlib>
#include <cstdio>

std::string Letters( void ) {
   return "V,G,D,S,P,A,B,C,I,F,R,C3,G6,S6,B4,D7,H";
}

std::vector<LRL_ReadLatticeData> GetInputCells( void ) {
   const std::string letters = Letters( );
   std::string lattice;
   std::vector<LRL_ReadLatticeData> cellDataList;
   LRL_ReadLatticeData rcd;
   while (lattice != "EOF") {
      rcd.read( );
      lattice = rcd.GetLattice( );
      if ((!lattice.empty( )) && (letters.find( static_cast<char>(toupper( lattice[0] )) ) != std::string::npos))
         cellDataList.push_back( rcd );
   }

   return cellDataList;
}

std::vector<S6> GetInputSellingReducedVectors( const std::vector<LRL_ReadLatticeData>& input ) {
   std::vector<S6> v;
   LatticeConverter converter;

   MatS6 mat;
   for (size_t i = 0; i < input.size( ); ++i) {
      const S6 s6 = converter.SellingReduceCell( input[i].GetLattice( ), input[i].GetCell( ), mat );
      v.push_back( s6 );
   }
   return v;
}

void Test_E3toS6() {

   // R1 is index 4  R3 is index 5
   std::string lattice( "R" );
   LRL_Cell cellacute( "10 10 10 65 65 65" );
   LRL_Cell cellobtuse( "10 10 10  15 15 15" );
   LatticeConverter lc;
   const  std::vector<std::pair<std::string, std::vector<double> > > vM3d = DeloneTypeList::Make3dCenteringMatrices(  );

   for (size_t i = 0; i < vM3d.size( ); ++i) std::cout << i << " " << vM3d[i].first << std::endl;

   const MatS6 m6_R1( MatS6::e3Tos6( vM3d[4].second ) );
   std::cout << "R1" << std::endl;
   std::cout << LRL_Cell_Degrees( m6_R1 * S6(cellacute) ) << std::endl;

   const MatS6 m6_R3( MatS6::e3Tos6( vM3d[4].second ) );
   std::cout << "R3" << std::endl;
   std::cout << LRL_Cell_Degrees( m6_R3 * cellobtuse ) << std::endl;
   std::cout << LRL_Cell_Degrees( lc.DeloneReduceCell("P", m6_R3 * S6(cellobtuse)) ) << std::endl;

}

class AngularS6 {
public:
   AngularS6( ) : m_isValid(false) {}
   AngularS6( const S6& s, const MatS6& m );
   double AngleBetween( const AngularS6& s1, const AngularS6& s2 ) const;

   AngularS6 operator-( const AngularS6& s ) const;
   double norm( ) const { return (*this).m_S6[0]; }

   bool m_isValid;
   S6 m_S6;
   MatS6 m_matS6;

};

double AngularS6::AngleBetween( const AngularS6& s1, const AngularS6& s2 ) const {
   double sum = 0.0;
   for (size_t i = 0; i < 6; ++i) sum += s1.m_S6[i] * s2.m_S6[i];
   return acos(sum/(s1.m_S6.norm()*s2.m_S6.norm()));
}

AngularS6 AngularS6::operator- ( const AngularS6&a ) const {
   AngularS6 as6;
   as6.m_S6[0] = AngleBetween( (*this), a );
   return as6;
}

std::vector<MatS6> BuildNCDist_Reflections() {
   //   static int S6Refl[24][36] = {
   std::vector<MatS6> cs6Dist_Refl;
   for (size_t i = 0; i < 24; ++i) {
      MatS6 ms6;
      for (size_t k = 0; k < 36; ++k) {
         ms6[k] = S6Refl[i][k];
      }
      cs6Dist_Refl.push_back(ms6);
   }
   if (cs6Dist_Refl.size() != 24) throw;
   return cs6Dist_Refl;
}

CNearTree<MatS6> BuildTree( const std::vector<MatS6>& v) {
   return CNearTree<MatS6>(v);
}

void CheckReflections() {
   // checking CS6Dist vs. the code in S6
   CNearTree<MatS6> nt_cs6dist_refl = BuildTree(BuildNCDist_Reflections());
   CNearTree<MatS6> nt_s6_refl(BuildTree(MatS6::GetReflections()));
   if (nt_cs6dist_refl.size() != 24) throw;
   if (nt_s6_refl.size() != 24) throw;

   for (size_t i = 0; i < 24; ++i) {
      if (nt_s6_refl.NearestNeighbor(0.01, nt_cs6dist_refl[i]) == nt_s6_refl.end()) throw;
   }

   for (size_t i = 0; i < 24; ++i) {
      if (nt_cs6dist_refl.NearestNeighbor(0.01, nt_s6_refl[i]) == nt_cs6dist_refl.end()) throw;
   }

}

std::vector<MatS6> CreateS6ReductionMatrices() {
   static const std::vector<std::pair<MatS6, MatS6> > redc = S6::SetUnreductionMatrices();
   std::vector<MatS6> v;
   for (size_t i = 0; i < redc.size(); ++i) {
      v.push_back(redc[i].first);
   }
   if (v.size() != 6) throw;
   return v;
}

std::vector<MatS6> CreateCS6ReductionMatrices() {
   // static int S6MSX[12][36] = {
   std::vector<MatS6> v;
   MatS6 m;
   for (size_t i = 0; i < 12; ++i) {
      for (size_t k = 0; k < 36; ++k) {
         m[k] = S6MSX[i][k];
      }
      v.push_back(m);
   }
   if (v.size() != 12) throw;
   return v;
}

void CheckBoundaries() {
   // checking CS6Dist vs. the code in S6
   // static int S6MS[6][36] = {
   //static const std::vector<MatS6> redc(CreateS6ReductionMatrices());
   //static const std::vector<MatS6> csMS(CreateCS6ReductionMatrices());
   CNearTree<MatS6> nt_cs6dist_refc = BuildTree(CreateCS6ReductionMatrices());
   CNearTree<MatS6> nt_s6_refc(BuildTree(CreateS6ReductionMatrices()));
   if (nt_cs6dist_refc.size() != 12) throw;
   if (nt_s6_refc.size() != 6) throw;

   CNearTree<MatS6> nt_s6_refl(BuildTree(MatS6::GetReflections()));

   int count = 0;
   bool success = false;
   std::vector<int> vcounts(12, 0);
   std::vector<int> vfoundWithoutReflection(12, 0);
   for (size_t i = 0; i < 12; ++i) {
      if (nt_s6_refc.NearestNeighbor(0.01, nt_cs6dist_refc[i]) != nt_s6_refc.end()) ++vfoundWithoutReflection[i];
      success = false;
      for (size_t k = 0; k < 24; ++k) {
         if (nt_s6_refc.NearestNeighbor(0.01, nt_s6_refl[k] * nt_cs6dist_refc[i]) != nt_s6_refc.end()) success = true;
         if (nt_s6_refc.NearestNeighbor(0.01, nt_s6_refl[k] * nt_cs6dist_refc[i]) != nt_s6_refc.end()) ++vcounts[i];
      }
      if (!success) throw;
   }

   count = 0;
   for (size_t i = 0; i < 6; ++i) {
      if (nt_cs6dist_refc.NearestNeighbor(0.01, nt_s6_refc[i]) == nt_cs6dist_refc.end()) ++count;
   }
   if (count != 0) throw;
}

void TestMatrices() {
//static int MS[15][36] = {
//   static int RS[NREFL][36] = {
   CheckReflections();

   CheckBoundaries();


   exit(0);
}

std::vector<size_t> LocateZeros(const S6& s) {
   std::vector<size_t> q;
   size_t locate = 0;
   for (size_t k = 0; k < 6; ++k) {
      if (s[k] == 0) {
         q.push_back(k);
      }
      ++locate;
   }
   return q;
}

std::vector<size_t> LocateNonZeros(const S6& s) {
   std::vector<size_t> q;
   size_t locate = 0;
   for (size_t k = 0; k < 6; ++k) {
      if (abs(s[k]) <1.0e-6) {
         q.push_back(k);
      }
      ++locate;
   }
   return q;
}

int CountZeros(const S6& s) {
   int count = 0;
   for (size_t k = 0; k < 6; ++k) {
      if (s[k] == 0) ++count;
   }
   return count;
}

int CodeZeros(const S6& s) {
   double values[] = { 1,2,4,8,16,32 };
   int code = 0;
   for (size_t k = 0; k < 6; ++k) {
      if (s[k] == 0) code += values[k];
   }
   return code;
}

//451 sqrt(s[s[ 0 ] + s[ 1 ] + s[ 2 ] + s[ 3 ] +
std::string WriteCodeForZeros(const S6& s) {
   std::string str = " sqrt(";
   const std::vector<size_t> q = LocateNonZeros(s);

   for (size_t i = 0; i < q.size(); ++i) {
      str += LRL_ToString("s[", q[i], "]", "*", "s[", q[i], "]", "+ ");
   }
   return str;
}

void ExamineBoundariesInS6OfInvalidLattices()
{
   RHrand storeRand;

   StoreResults<int, LRL_Cell_Degrees> store(1);
   StoreResults<int, std::string> storeStr(1);

   for (size_t i = 0; i < 10000; ++i) {
      S6 s;
      for (size_t k = 0; k < 6; ++k) {
         const double d = storeRand.urand();
         s[k] = (d < 0.5) ? 0.0 : -100.0;
      }
      const bool b = s.IsValid();
      const int count = CountZeros(s);
      const double sign = b ? -1.0 : 1.0;
      const int key = sign * (100 * count + CodeZeros(s));
      if (count == 3 || count == 4) {
         //store.Store(key, LRL_Cell_Degrees(s));
         const std::string code = LRL_ToString(key) + WriteCodeForZeros(s);
         storeStr.Store(key, code);
      }
   }
   store.ShowResults();
   storeStr.ShowResults();
   exit(0);
}

double DistanceTo_ValidThreeZeros(const S6& s) {
   const double mind = sqrt(minNC(
      minNC(
         s[2] * s[2] + s[4] * s[4] + s[5] * s[5],
         s[1] * s[1] + s[4] * s[4] + s[5] * s[5],
         s[0] * s[0] + s[4] * s[4] + s[5] * s[5],
         s[2] * s[2] + s[3] * s[3] + s[5] * s[5],
         s[1] * s[1] + s[3] * s[3] + s[5] * s[5],
         s[0] * s[0] + s[3] * s[3] + s[5] * s[5],
         s[1] * s[1] + s[2] * s[2] + s[5] * s[5]),

      minNC(
         s[0] * s[0] + s[2] * s[2] + s[5] * s[5],
         s[2] * s[2] + s[3] * s[3] + s[4] * s[4],
         s[1] * s[1] + s[3] * s[3] + s[4] * s[4],
         s[0] * s[0] + s[3] * s[3] + s[4] * s[4],
         s[1] * s[1] + s[2] * s[2] + s[4] * s[4],
         s[0] * s[0] + s[1] * s[1] + s[4] * s[4],
         s[0] * s[0] + s[2] * s[2] + s[3] * s[3]),

      minNC(
         s[0] * s[0] + s[1] * s[1] + s[3] * s[3],
         s[0] * s[0] + s[1] * s[1] + s[2] * s[2])));

   return mind;
}

double DistanceTo_INValidThreeZeros(const S6& s) {
   const double mind = sqrt( minNC(     
         s[1] * s[1] + s[2] * s[2] + s[3] * s[3],
         s[0] * s[0] + s[2] * s[2] + s[4] * s[4],
         s[0] * s[0] + s[1] * s[1] + s[5] * s[5],
         s[3] * s[3] + s[4] * s[4] + s[5] * s[5]));

   const double d0 = sqrt(s[1] * s[1] + s[2] * s[2] + s[3] * s[3]);
   const double d1 = sqrt(s[0] * s[0] + s[2] * s[2] + s[4] * s[4]);
   const double d2 = sqrt(s[0] * s[0] + s[1] * s[1] + s[5] * s[5]);
   const double d3 = sqrt(s[3] * s[3] + s[4] * s[4] + s[5] * s[5]);


   return mind;
}
double DistanceTo_INValidFourZeros(const S6& s) {
   const double mind = sqrt(minNC(
      minNC(
      s[0] * s[0] + s[1] * s[1] + s[2] * s[2] + s[3] * s[3],
      s[0] * s[0] + s[1] * s[1] + s[2] * s[2] + s[4] * s[4],
      s[0] * s[0] + s[1] * s[1] + s[3] * s[3] + s[4] * s[4],
      s[0] * s[0] + s[2] * s[2] + s[3] * s[3] + s[4] * s[4],
      s[1] * s[1] + s[2] * s[2] + s[3] * s[3] + s[4] * s[4],
      s[0] * s[0] + s[1] * s[1] + s[2] * s[2] + s[5] * s[5],
      s[0] * s[0] + s[1] * s[1] + s[3] * s[3] + s[5] * s[5]),

      minNC(
      s[0] * s[0] + s[2] * s[2] + s[3] * s[3] + s[5] * s[5],
      s[1] * s[1] + s[2] * s[2] + s[3] * s[3] + s[5] * s[5],
      s[0] * s[0] + s[1] * s[1] + s[4] * s[4] + s[5] * s[5],
      s[0] * s[0] + s[2] * s[2] + s[4] * s[4] + s[5] * s[5],
      s[1] * s[1] + s[2] * s[2] + s[4] * s[4] + s[5] * s[5],
      s[0] * s[0] + s[3] * s[3] + s[4] * s[4] + s[5] * s[5],
      s[1] * s[1] + s[3] * s[3] + s[4] * s[4] + s[5] * s[5]),

      s[2] * s[2] + s[3] * s[3] + s[4] * s[4] + s[5] * s[5]));


      const double d00 =   sqrt(s[0] * s[0] + s[1] * s[1] + s[2] * s[2] + s[3] * s[3]);
      const double d01 =   sqrt(s[0] * s[0] + s[1] * s[1] + s[2] * s[2] + s[4] * s[4]);
      const double d02 =   sqrt(s[0] * s[0] + s[1] * s[1] + s[3] * s[3] + s[4] * s[4]);
      const double d03 =   sqrt(s[0] * s[0] + s[2] * s[2] + s[3] * s[3] + s[4] * s[4]);
      const double d04 =   sqrt(s[1] * s[1] + s[2] * s[2] + s[3] * s[3] + s[4] * s[4]);
      const double d05 =   sqrt(s[0] * s[0] + s[1] * s[1] + s[2] * s[2] + s[5] * s[5]);
      const double d06 =   sqrt(s[0] * s[0] + s[1] * s[1] + s[3] * s[3] + s[5] * s[5]);
      const double d07 =   sqrt(s[0] * s[0] + s[2] * s[2] + s[3] * s[3] + s[5] * s[5]);
      const double d08 =   sqrt(s[1] * s[1] + s[2] * s[2] + s[3] * s[3] + s[5] * s[5]);
      const double d09 =   sqrt(s[0] * s[0] + s[1] * s[1] + s[4] * s[4] + s[5] * s[5]);
      const double d10 =   sqrt(s[0] * s[0] + s[2] * s[2] + s[4] * s[4] + s[5] * s[5]);
      const double d11 =   sqrt(s[1] * s[1] + s[2] * s[2] + s[4] * s[4] + s[5] * s[5]);
      const double d12 =   sqrt(s[0] * s[0] + s[3] * s[3] + s[4] * s[4] + s[5] * s[5]);
      const double d13 =   sqrt(s[1] * s[1] + s[3] * s[3] + s[4] * s[4] + s[5] * s[5]);
      const double d14 =   sqrt(s[2] * s[2] + s[3] * s[3] + s[4] * s[4] + s[5] * s[5]);




   return mind;
}

void DistancesToInvalidLattices() {
   for (size_t i = 0; i < 100; ++i) {
      S6 s(S6::rand());
      RHrand storeRand;

      s[size_t(rand()/double(RAND_MAX) * 6.0)] = 0.0;
      s[size_t(rand()/double(RAND_MAX) * 6.0)] = 0.0;
      //s[size_t(rand()/double(RAND_MAX) * 6.0)] = 0.0;

      const double d3v = DistanceTo_ValidThreeZeros(s)   ;
      const double d3i = DistanceTo_INValidThreeZeros(s) ;
      const double d4i = DistanceTo_INValidFourZeros(s)  ;

      const double test = abs(101.36766552387854 - d3i);
      if (d4i < d3i ) std::cout << d3i << "  " << d4i << "    " << s << std::endl;

      //if (minNC(d3v, d3i, d4i) < 70) {
      //   std::cout << std::endl;
      //   std::cout << DistanceTo_ValidThreeZeros(s) << "  valid 3 zeros" << std::endl;
      //   std::cout << DistanceTo_INValidThreeZeros(s) << "  invalid 3 zeros" << std::endl;
      //   std::cout << DistanceTo_INValidFourZeros(s) << "  4 zeros " << std::endl;
      //   std::cout << s.norm() << "       " << s << std::endl;
      //}
   }
   exit(0);
}

MatS6 FindProjector(const std::string& s) {
   static const std::vector<std::pair<std::string, MatS6> > prjBound = Delone::LoadLabeledLatticeTypeProjectors();
   for (size_t i = 0; i < prjBound.size(); ++i) {
      if (prjBound[i].first == s)
         return prjBound[i].second;
   }
   return MatS6().unit();
}
//S6 FindBestMatchAmongReflections(const S6& s2) {
//   static const std::vector<MatS6> refl_one = MatS6::GetReflections();
//   double bestDistance = DBL_MAX;
//   S6 best;
//   for (size_t i = 0; i < refl_one.size(); ++i) {
//      const S6 testS6 = refl_one[i] * s2;
//      const double test = (*this - testS6).norm();
//      if (test < bestDistance) {
//         best = testS6;
//         bestDistance = test;
//      }
//   }
//}

void ListDeloneLatticeTypes() {
   static const std::vector<std::pair<std::string, MatS6> > prjRefl = Delone::LoadLabeledLatticeTypeProjectors();
   std::string s = "Delone Lattice Types: ";
   for (size_t i = 0; i < prjRefl.size(); ++i) {
      s += prjRefl[i].first + " ";
   }
   std::cout << s << std::endl;
}

double AngleSum(const LRL_Cell_Degrees& c) {
   return c[3] + c[4] + c[5];
}

bool TestForPositive(const S6& s) {
   for (size_t i = 0; i < 6; ++i) if (s[i] > 0.0) return true;
   return false;
}

template<typename T>
void HowMuchBetterIsCS6DistThanJustDistanceInFundamentalUnit(const T& t = T())
{
   ListDeloneLatticeTypes();
   StoreResults<double, PairReporter<T, T> > store(5);
   StoreResults<double, PairReporter<double,double> > store2(5);
   const std::string lat1 = "M1B";
   const std::string lat2 = "M4";
   store.SetTitle("in Fundamental unit vs CS6Dist for " + lat1 + " & " + lat2);
   GenerateRandomLattice<S6> rl(1);

   const MatS6 m1 = FindProjector(lat1);
   const MatS6 m2 = FindProjector(lat2);
   for (size_t i = 0; i < 100000; ++i) {
      S6 s1red;
      S6 s2red;
      const S6 s1 = rl.randSellingReduced();
      const S6 s2 = rl.randSellingReduced();
      const bool b1 = Selling::Reduce(m1 * s1, s1red);
      const bool b2 = Selling::Reduce(m2 * s2, s2red);
      if (CountZeros(s2red) != 2) continue;
      if (b1 && b2) {
         s2red = s1red.FindBestMatchAmongReflections(s2red);
      //std::cout << s1red << "   " << s2red << std::endl << std::endl;
         const double dFundamental = (s2red - s1red).norm();
         const double dCs6Dist = CS6Dist(s1red.data(), s2red.data());

         const double ratio = dFundamental / dCs6Dist;
         const double logRatio = log10(ratio);
         const double key = pow(10.0, (int(10.0 * logRatio + 1.0)) / 10.0);

         PairReporter<T, T> reporter =
            std::make_pair(T(s1red), T(s2red));
         reporter.SetSeparator("  #  ");
         store.Store(key, reporter);

         PairReporter<double, double> fits(dFundamental, dCs6Dist);
         fits.SetSeparator("  ");
         store2.Store(key, fits);
      }
   }

   store.ShowResultsByKeyAscending();
   store2.ShowResultsByKeyAscending();
   exit(0);
}

class C3_ForCells {
public:
   C3_ForCells() {}
   C3_ForCells(const S6& s) {
      const LRL_Cell c (s);
      m_v[0] = std::polar(c[0],c[3]);
      m_v[1] = std::polar(c[1],c[4]);
      m_v[2] = std::polar(c[2],c[5]);
   }

   template<typename T> C3_ForCells(const T& t) {
      *this = C3_ForCells(s(t));
   }

   std::complex<double> operator[] (const size_t n) { return m_v[n]; }

   friend std::ostream& operator<< (std::ostream& o, const C3_ForCells& fc) {
      o << fc.m_v[0] << " " << fc.m_v[1] << " " << fc.m_v[2] << "\n";
      double r[3];
      double theta[3];
      for (size_t i = 0; i < 3; ++i)   r[i] = abs(fc.m_v[i]);
      for (size_t i = 0; i < 3; ++i)   theta[i] = arg(fc.m_v[i]);
      for (size_t i = 0; i < 3; ++i)   std::cout << r[i] << "," << 180.0/(4.0*atan(1.0))*theta[i] << "   ";

      std::cout << std::endl << std::endl;

      return o;
   }
private:
   std::complex<double> m_v[3];
};

void StuffFor_a_alpha() {
   static const std::vector<MatS6> refl_one = MatS6::GetReflections();
   static const std::vector<std::pair<MatS6, MatS6> > redc = S6::SetUnreductionMatrices();

   const MatS6 m1 = FindProjector("C5");
   //const S6 testlattice = m1 * S6::randDeloneReduced();
   const S6 testlattice = S6(G6( "100 100 100  100 100 100"));
   std::cout << "input cell " << LRL_Cell_Degrees(testlattice) << std::endl;
   std::cout << "apply 24 S6 reflections to that " << std::endl << std::endl;
   for (size_t i = 0; i < redc.size(); ++i) {
      const C3_ForCells cfc(redc[i].first * testlattice);
      std::cout << cfc << std::endl;
   }
   exit(0);
}
void TestSomeCells() {
   std::vector<MatS6> refls = MatS6::GetReflections();
   const S6 cel = S6("0 -100 -200 0 -150 -250");
   for (size_t i = 0; i < refls.size(); ++i) {
      std::cout << i << "  " << LRL_Cell_Degrees
      (refls[i] * cel) << std::endl
         ;
   }
   exit(0);
}
void ComplexCell(const S6& s)
{

   static const double pi = 180.0 / 4.0 / atan(1.0);
   const double asq = -(s[3] + s[2] + s[1]);
   const double bsq = -(s[4] + s[2] + s[0]);
   const double csq = -(s[5] + s[1] + s[0]);
   const std::complex<double> a = std::sqrt(std::complex<double>(asq, 0.0));
   const std::complex<double> b = std::sqrt(std::complex<double>(bsq, 0.0));
   const std::complex<double> c = std::sqrt(std::complex<double>(csq,0.0));

   const std::complex<double> cosal = s[0] / (b * c);
   const std::complex<double> cosbe = s[1] / (a * c);
   const std::complex<double> cosga = s[2] / (a * b);

   const std::complex<double> al = std::acos(cosal) * 180.0 / pi;
   const std::complex<double >be = std::acos(cosbe) * 180.0 / pi;
   const std::complex<double> ga = std::acos(cosga) * 180.0 / pi;

   std::cout << std::endl;
   std::cout << "input S6 " << s << std::endl;
   std::cout << "as cell  " << a << " " << b << " " << c << "    " << al << " " << be << " " << ga << std::endl;
   std::cout << "G6    " << G6(s) << std::endl;
}


static std::vector<std::pair<MatS6, MatS6> > CreateReductionMatrices() {
   std::vector<std::pair<MatS6, MatS6> > m(1, std::make_pair(MatS6().unit(), MatS6().unit()));
   static const std::vector<std::pair<MatS6, MatS6> > redc = S6::SetUnreductionMatrices();
   m.insert(m.end(), redc.begin(), redc.end());
   return m;
}

void MakeBoundaryTransforms() {
   static const std::vector<MatS6> refl = MatS6::GetReflections();
   static const std::vector<std::pair<MatS6, MatS6> > redc(CreateReductionMatrices());
   for (size_t i = 0; i < refl.size(); ++i) {
      const MatS6 m = refl[i] * redc[1].first;
      std::cout << LRL_MaximaTools::MaximaFromMat(m) << std::endl;
   }
   exit(0);
}

void TestDistance(const S6& s1, const S6& s2) {
   const double d1 = CS6Dist(s1.data(), s2.data());
   static const std::vector<std::pair<MatS6, MatS6> > redc(CreateReductionMatrices());

   const double d2 = CS6Dist((redc[1].first*s1).data(), (redc[1].first*s2).data());

   const double d3 = (s1 - s2).norm();
   const double d4 = (redc[1].first * s1 - redc[1].first * s2).norm();
   const double d5 = (redc[1].first * (s1 - s2)).norm();
   const int i19191 = 19191;
}

const double r = -10;
const double s = -30;
const double t = -60;
const double u = -80;
const double v = -150;
const double w = -220;
const double zero = 0;

std::string ConvertS6ToCharacter(const S6& s6) {
   std::string sret;
   for (size_t i = 0; i < 6; ++i) {
      if (s6[i] == r) sret += 'r';
      if (s6[i] == s) sret += 's';
      if (s6[i] == t) sret += 't';
      if (s6[i] == u) sret += 'u';
      if (s6[i] == v) sret += 'v';
      if (s6[i] == w) sret += 'w';
      if (s6[i] == 0) sret += '0';
      if (i == 2) sret += ' ';
   }
   return sret;
}

void ConvertCellsToC3_Assym() {
   std::vector<std::pair<std::string, std::string> > chars = DeloneTypeList::CreateCharacterList();
   for (size_t ch = 0; ch < chars.size(); ++ch) {
      const std::string& name = chars[ch].first;
      const std::string& character = chars[ch].second;
      S6 s6;
      size_t counter = 0;
      for (size_t i = 0; i < character.length(); ++i) {
         if (character[i] == ' ') continue;
         if (character[i] == 'r') s6[counter] = r;
         if (character[i] == 's') s6[counter] = s;
         if (character[i] == 't') s6[counter] = t;
         if (character[i] == 'u') s6[counter] = u;
         if (character[i] == 'v') s6[counter] = v;
         if (character[i] == 'w') s6[counter] = w;
         if (character[i] == '0') s6[counter] = 0;
         ++counter;
      }
      const C3 c3 = C3::ConvertToFundamentalUnit(s6);
      std::cout << name << "  " << ConvertS6ToCharacter(s6) << "   " << ConvertS6ToCharacter(S6(c3)) << std::endl;
   }
   exit(0);
}

void CountCycles() {
   Selling sell;
   StoreResults<size_t, std::string> sr(5);
   for (size_t i = 0; i < 10000000; ++i) {
      S6 s6in = S6::rand();
      s6in[0] = 0.1;
      s6in[1] = 0.1;
      if (Selling::IsReduced(s6in)) continue;
      S6 vout;
      Selling::Reduce(s6in, vout);
      const size_t cycles = sell.GetCycles();
      if (cycles <10) continue;
      const size_t key = (cycles == 0 ) ? 0 : log10(cycles);
      sr.Store(key, LRL_ToString( cycles, "  " , s6in));
   }
   sr.ShowResults();
   exit(0);
}

int main(int argc, char* argv[])
{
   CountCycles();
   ConvertCellsToC3_Assym();

   const S6 s1 = C3("-10 -11- -90 -1190 -190 -190");
   const S6 s2 = C3("-10 -100 -200 -100 -200-200");
   TestDistance(s1.data(), s2.data());
   MakeBoundaryTransforms();
   //ComplexCell(S6(" 0 0 0 200  0 100"));
   //ComplexCell(S6(" 0 0 0 100 100 100"));
   //ComplexCell(S6(" 0 0 0 0 100 100"));
   //ComplexCell(S6(" 0 0 0 0 0 100"));
   //ComplexCell(S6(" 0 0 0 0 0 0"));
   ComplexCell(S6("100 100 100 -1 0 0"));
   ComplexCell(S6("100 100 100 1 0 0"));
   ComplexCell(S6("100 100 100 0 0 0"));
   ComplexCell(S6("-100 -100 -100 0 0 0"));
   ComplexCell(S6("-100 -100 -100 -1 0 0"));
   ComplexCell(S6("-100 -100 -100 1 0 0"));

   ComplexCell(S6("-100 -100 -100 -30 -20 -10"));
   ComplexCell(S6("100 100 100 30 20 10"));
   exit(0);


   TestSomeCells();
   StuffFor_a_alpha();
   HowMuchBetterIsCS6DistThanJustDistanceInFundamentalUnit<S6>();
   DistancesToInvalidLattices();
   ExamineBoundariesInS6OfInvalidLattices();
   TestMatrices();
   ////Test_E3toS6( );
   //S6 s1, s2;
   //CNearTree<S6> snt;
   //snt.insert( s1 );
   //snt.insert( s2 );
   //snt.NearestNeighbor( 0.0, s2 );

   std::cout << " text input" << std::endl;
   const std::vector<LRL_ReadLatticeData> input = GetInputCells();
   std::cout << "the first cell will be the REFERENCE" << std::endl;
   MatS6 mat_reference;
   const LRL_Cell cell_reference = LatticeConverter::SellingReduceCell( input[0].GetLattice(), input[0].GetCell(), mat_reference );
   const MV_Pair mv_reducedReference(S6( cell_reference ), MatN(mat_reference.GetMatrix()).inverse());

   const std::vector<S6> vLat = GetInputSellingReducedVectors( input );
   std::cout << std::endl << "Reduced input cells " << std::endl;
   for (size_t lat = 0; lat < vLat.size( ); ++lat) {
      std::cout << LRL_Cell_Degrees( vLat[lat] ) << std::endl;
   }
   std::cout << std::endl;

   if (vLat.size( ) > 0) {
      LRL_LatticeMatcher lm;
      lm.SetReferenceLattice( mv_reducedReference );
      //lm.SetReferenceLattice( MV_Pair( cell_reference, MatS6().unit() ));
      std::cout << "MV tree size = " << lm.size( ) << std::endl;
      //std::cout << "before lattice match " << LRL_CreateFileName( ).Create( "", "" ) << std::endl;

      const std::vector<S6> vs6( lm.MatchReference( vLat ) );
      //std::cout << "after lattice match " << LRL_CreateFileName( ).Create( "", "" ) << std::endl;

      std::cout << "matched cells including reference" << std::endl;
      for (size_t lat =0; lat < vLat.size(); ++lat) {
         std::cout<< LRL_Cell_Degrees(  vs6[lat]) << std::endl;
         //if (!vs6[lat].IsValid()) throw;
      }
      exit(0);
      S6Dist sd( 1 );
      sd.SetDebug( false );


      //LMDist lmd( vLat[0] );
      //double ddddd = lmd.DistanceBetween( vLat[1] );

      StoreResults<int, std::string> srlm(5);
      srlm.SetTitle( LRL_ToString( "reference ", vLat[0] ) );
      double dists6, distlm, distcs, disttest;
      //for (size_t lat = 1; lat < vLat.size( ); ++lat) {
      //   std::cout << vs6[0] << "   " << vs6[lat] << std::endl << std::endl;
      //   distlm = (vLat[0] - vs6[lat]).norm( );
      //   std::cout << "distlm A " << distlm << std::endl;
      //   distlm = lmd.DistanceBetween(vLat[lat]);
      //   std::cout << "distlm B " << distlm << std::endl;
      //   disttest = lm.DistanceBetween( vLat[0], vLat[lat] );
      //   std::cout << "disttest " << disttest << std::endl;
      //   distcs = CS6Dist( vLat[0].data( ), vLat[lat].data( ) );
      //   dists6 = sd.DistanceBetween( vLat[0], vLat[lat] );

      //   //std::cout << "lat match " << distlm << "  cs6dist " << distcs << "  s6dist " << dists6 << std::endl;
      //   double diff = (distlm - dists6) / dists6 * 100.0;
      //   //srlm.Store( int( diff ), LRL_ToString("# ", lat, "   ", vLat[lat] ) );
      //}
      //srlm.ShowResultsByKeyAscending( );
      //std::cout << std::endl;

      //std::cout << "vLat list" << std::endl;
      //for (size_t lat = 0; lat < vLat.size( ); ++lat) {
      //   std::cout << vLat[lat] << std::endl;
      //}
      //std::cout << std::endl;

      //std::cout << "vs6 list" << std::endl;
      //for (size_t lat = 0; lat < vLat.size( ); ++lat) {
      //   std::cout << ( vs6[lat] ) << std::endl;
      //}
      //std::cout << std::endl;
      //std::cout << "vs6 list" << std::endl;
      //for (size_t lat = 0; lat < vLat.size( ); ++lat) {
      //   std::cout << LRL_Cell_Degrees( vs6[lat] ) << std::endl;
      //}
      //std::cout << std::endl;








   //   double dist;
   //   lmd.SetReferenceLattice( MV_Pair(vLat[0], MatS6()) );
   //   std::cout << LRL_CreateFileName( ).Create( "", "" ) << std::endl;
   //   for (size_t lat = 0; lat < vLat.size( ); ++lat) {
   //      dist = lmd.DistanceBetween(vs6[lat]);
   //   }
   //   std::cout << LRL_CreateFileName( ).Create( "", "" ) << std::endl;
   //   for (size_t lat = 0; lat < vLat.size( ); ++lat) {
   //      dist = (vs6[0] - vs6[lat]).norm( );
   //   }
   //   std::cout << LRL_CreateFileName( ).Create( "", "" ) << std::endl;
   //   for (size_t lat = 0; lat < vLat.size( ); ++lat) {
   //      dist = (vs6[0] - vs6[lat]).norm( );
   //   }
   //   std::cout << LRL_CreateFileName( ).Create("","" ) << std::endl;
   //   for (size_t lat = 0; lat < vLat.size( ); ++lat) {
   //      dist = CS6Dist( vLat[0].data( ), vLat[lat].data( ) );
   //   }
   //   std::cout << LRL_CreateFileName( ).Create("","" ) << std::endl;
   //   for (size_t lat = 0; lat < vLat.size( ); ++lat) {
   //      dist = sd.DistanceBetween( vLat[0], vLat[lat] );
   //   }
   //   std::cout << LRL_CreateFileName( ).Create("","" ) << std::endl;
   }

   //return 0;
}

/*
p 2 4 4 60 79.2 75.5
p 2 4 4 60 86.45 75.5
p 2 4 4 120 93.51 100.666
p 2 4 4 117.95 93.52 104.5
p 2 4 4 113.95 100.5 104.5
end

g 4 16 16 16.1 3 4
g 4 16 16 16 1 4.1
g 4 16 16  -16.2 -1 -3
g 4 16 16 -15.2 -1 -4.2
g 4 16 16 -13.3 -3 -4
end


p 2 4 4 60 79.2 75.5
p 2 4 4 60 86.45 75.5
p 2 4 4 120 93.51 100.666
p 2 4 4 117.95 93.52 104.5
p 2 4 4 113.95 100.5 104.5
end

g 4 16 16 16.1 3 4
g 4 16 16 16 1 4.1
g 4 16 16  -16.2 -1 -3
g 4 16 16 -15.2 -1 -4.2
g 4 16 16 -13.3 -3 -4
end


f 10 10.001 10.002  89 89 89
f 10 10 10  90 90 90
f 11 10 10  90 90 90
f 10 11 10  90 90 90  this is the problem one 
f 10 10 11  90 90 90
end



f 10 10.001 10.002  90.01 90.02 90.03
f 10 10 10  90 90 90
f 11 10 10  90 90 90
f 10 11 10  90 90 90  this is the problem one
f 10 10 11  90 90 90
end


f 10 10.001 10.002  90 90 90
f 10 10 10  90 90 90
f 11 10 10  90 90 90
f 10 11 10  90 90 90  this is the problem one
f 10 10 11  90 90 90
end


f 10 10 10  90 90 90
f 11 10 10  90 90 90
f 10 11 10  90 90 90  this is the problem one
f 10 10 11  90 90 90
end

f 10 10 10  90 90 90
f 11 10 10  90 90 90
f 10 11 10  90 90 90  this is the problem one
end



f 10 10 10  90 90 90
i 10 10 10  90 90 90
p  10 10 10  90 90 90
f 10 10 10  90 90 90
f 10 10 11  90 90 90
end

c 10 20 30 90 118 90
p   27.14100  11.18034  26.80144     93.90767 158.64514  95.59396
p   27.14100  11.18034  26.80144     93.90767 158.64514  95.59396
p   27.14100  11.18034  26.80144     93.90767 158.64514  95.59396
p   27.14100  11.18034  11.18034    126.86990  95.59396  95.59396
p   27.14100  26.80144  11.18034     93.90767  95.59396 158.64514
p   27.14100  26.80144  11.18034     93.90767  95.59396 158.64514
p   27.14100  11.18034  11.18034    126.86990  95.59396  95.59396
p   27.14100  11.18034  26.80144     93.90767 158.64514  95.59396
p   11.18034  27.14100  26.80144    158.64514  93.90767  95.59396
p   11.18034  27.14100  11.18034     95.59396 126.86990  95.59396
p   11.18034  26.80144  27.14100    158.64514  95.59396  93.90767
p   11.18034  26.80144  11.18034     93.90767 126.86990  93.90767
p   11.18034  11.18034  27.14100     95.59396  95.59396 126.86990
p   11.18034  11.18034  26.80144     93.90767  93.90767 126.86990
p   26.80144  27.14100  11.18034     95.59396  93.90767 158.64514
p   26.80144  27.14100  11.18034     95.59396  93.90767 158.64514
p   26.80144  11.18034  27.14100     95.59396 158.64514  93.90767
p   26.80144  11.18034  11.18034    126.86990  93.90767  93.90767
p   26.80144  11.18034  27.14100     95.59396 158.64514  93.90767
p   26.80144  11.18034  11.18034    126.86990  93.90767  93.90767
p   11.18034  27.14100  11.18034     95.59396 126.86990  95.59396
p   11.18034  27.14100  26.80144    158.64514  93.90767  95.59396
p   11.18034  11.18034  27.14100     95.59396  95.59396 126.86990
p   11.18034  11.18034  26.80144     93.90767  93.90767 126.86990
p   11.18034  26.80144  27.14100    158.64514  95.59396  93.90767
p   11.18034  26.80144  11.18034     93.90767 126.86990  93.90767
p   28.32800  11.18034  26.80144     86.09233 156.81153 107.53456
p   27.14100  28.32800  26.80144    156.81153  21.35486 156.87148
p   27.14100  11.18034  10.00000    116.56505 102.58967  84.40604
p   27.14100  28.32800  10.00000     88.14729 102.58967 156.87148
p   28.32800  11.18034  28.32800    109.28080 138.65723 107.53456
p   10.00000  28.32800  26.80144    156.81153  98.76519  91.85271
p   28.32800  11.18034  11.18034     53.13010 109.28080 107.53456
p   27.14100  28.32800  11.18034    109.28080  84.40604 156.87148
p   27.14100  11.18034  28.32800    109.28080 156.87148  84.40604
p   27.14100  28.32800  28.32800     41.34277 156.87148 156.87148
p   28.32800  11.18034  10.00000    116.56505  91.85271 107.53456
p   28.32800  10.00000  11.18034    116.56505 107.53456  91.85271
p   10.00000  26.80144  11.18034     86.09233 116.56505  98.76519
p   27.14100  10.00000  11.18034    116.56505  84.40604 102.58967
p   27.14100  26.80144  28.32800    156.81153 156.87148  21.35486
p   27.14100  10.00000  28.32800     88.14729 156.87148 102.58967
p   10.00000  26.80144  28.32800    156.81153  91.85271  98.76519
p   28.32800  28.32800  11.18034    109.28080 107.53456 138.65723
p   28.32800  27.14100  26.80144     21.35486 156.81153 156.87148
p   11.18034  28.32800  26.80144    156.81153  86.09233 107.53456
p   11.18034  27.14100  28.32800    156.87148 109.28080  84.40604
p   11.18034  28.32800  28.32800    138.65723 109.28080 107.53456
p   28.32800  27.14100  10.00000    102.58967  88.14729 156.87148
p   28.32800  10.00000  26.80144     98.76519 156.81153  91.85271
p   28.32800  27.14100  11.18034     84.40604 109.28080 156.87148
p   11.18034  28.32800  11.18034    109.28080  53.13010 107.53456
p   11.18034  27.14100  10.00000    102.58967 116.56505  84.40604
p   11.18034  28.32800  10.00000     91.85271 116.56505 107.53456
p   28.32800  27.14100  28.32800    156.87148  41.34277 156.87148
p   10.00000  28.32800  11.18034    107.53456 116.56505  91.85271
p   28.32800  26.80144  27.14100     21.35486 156.87148 156.81153
p   11.18034  28.32800  27.14100    156.87148  84.40604 109.28080
p   11.18034  26.80144  28.32800    156.81153 107.53456  86.09233
p   11.18034  28.32800  28.32800    138.65723 107.53456 109.28080
p   28.32800  26.80144  10.00000     98.76519  91.85271 156.81153
p   28.32800  10.00000  27.14100    102.58967 156.87148  88.14729
p   28.32800  26.80144  11.18034     86.09233 107.53456 156.81153
p   11.18034  28.32800  11.18034    107.53456  53.13010 109.28080
p   11.18034  26.80144  10.00000     98.76519 116.56505  86.09233
p   11.18034  28.32800  10.00000     88.14729 116.56505 109.28080
p   28.32800  26.80144  28.32800    156.81153  41.34277 156.81153
p   10.00000  28.32800  11.18034    109.28080 116.56505  88.14729
p   10.00000  11.18034  27.14100     84.40604 102.58967 116.56505
p   11.18034  10.00000  27.14100    102.58967  84.40604 116.56505
p   11.18034  11.18034  28.32800    109.28080 107.53456  53.13010
p   11.18034  10.00000  28.32800     91.85271 107.53456 116.56505
p   10.00000  11.18034  28.32800    107.53456  91.85271 116.56505
p   28.32800  28.32800  27.14100    156.87148 156.87148  41.34277
p   10.00000  11.18034  26.80144     86.09233  98.76519 116.56505
p   11.18034  10.00000  26.80144     98.76519  86.09233 116.56505
p   11.18034  11.18034  28.32800    107.53456 109.28080  53.13010
p   11.18034  10.00000  28.32800     88.14729 109.28080 116.56505
p   10.00000  11.18034  28.32800    109.28080  88.14729 116.56505
p   28.32800  28.32800  26.80144    156.81153 156.81153  41.34277
p   10.00000  27.14100  11.18034     84.40604 116.56505 102.58967
p   26.80144  10.00000  11.18034    116.56505  86.09233  98.76519
p   26.80144  27.14100  28.32800    156.87148 156.81153  21.35486
p   26.80144  10.00000  28.32800     91.85271 156.81153  98.76519
p   10.00000  27.14100  28.32800    156.87148  88.14729 102.58967
p   28.32800  28.32800  11.18034    107.53456 109.28080 138.65723
p   28.32800  11.18034  27.14100     84.40604 156.87148 109.28080
p   26.80144  28.32800  27.14100    156.87148  21.35486 156.81153
p   26.80144  11.18034  10.00000    116.56505  98.76519  86.09233
p   26.80144  28.32800  10.00000     91.85271  98.76519 156.81153
p   28.32800  11.18034  28.32800    107.53456 138.65723 109.28080
p   10.00000  28.32800  27.14100    156.87148 102.58967  88.14729
p   28.32800  11.18034  11.18034     53.13010 107.53456 109.28080
p   26.80144  28.32800  11.18034    107.53456  86.09233 156.81153
p   26.80144  11.18034  28.32800    107.53456 156.81153  86.09233
p   26.80144  28.32800  28.32800     41.34277 156.81153 156.81153
p   28.32800  11.18034  10.00000    116.56505  88.14729 109.28080
p   28.32800  10.00000  11.18034    116.56505 109.28080  88.14729
end

*/