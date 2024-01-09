// Testbed.cpp : Defines the entry point for the console application.
//

#include "BasisBase.h"
#include "CellInputData.h"
#include "CS6Dist.h"
#include "C3.h"
#include "DC.h"
#include "Delone.h"
#include "LatticeConverter.h"
#include "LRL_CreateFileName.h"
#include "CS6Dist.h"
#include "CS6Dist.c"
#include "DeloneTypeList.h"
#include "FollowerPathGenerator.h"
#include "GenerateLatticeTypeExamples.h"
#include "G6.h"
#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_GetCommandArgs.h"
#include "LRL_LatticeMatcher.h"
#include "LRL_MaximaTools.h"
#include "LRL_MinMaxTools.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "MatN.h"
#include "MatS6.h"
#include "NCDist.h"
#include "Niggli.h"
#include "PairReporter.h"
#include "rhrand.h"
#include "RI.h"
#include "S6.h"
#include "S6Dist.h"
#include "TNear.h"

#include <algorithm>
#include <utility>
#include <complex>
#include <cstdlib>
#include <cstdio>

std::string Letters(void) {
   return "V,G,D,S,P,A,B,C,I,F,R,C3,G6,S6,B4,D7,H";
}


std::vector<LRL_ReadLatticeData> GetInputCells(void) {
   const std::string letters = Letters();
   std::string lattice;
   std::vector<LRL_ReadLatticeData> cellDataList;
   LRL_ReadLatticeData rcd;
   while (lattice != "EOF") {
      rcd.read();
      lattice = rcd.GetLattice();
      if ((!lattice.empty()) && (letters.find(static_cast<char>(toupper(lattice[0]))) != std::string::npos))
         cellDataList.push_back(rcd);
   }

   return cellDataList;
}

std::vector<S6> GetInputSellingReducedVectors(const std::vector<LRL_ReadLatticeData>& input) {
   std::vector<S6> v;
   LatticeConverter converter;

   MatS6 mat;
   for (size_t i = 0; i < input.size(); ++i) {
      const S6 s6 = converter.SellingReduceCell(input[i].GetLattice(), input[i].GetCell(), mat);
      v.push_back(s6);
   }
   return v;
}

void Test_E3toS6() {

   // R1 is index 4  R3 is index 5
   std::string lattice("R");
   LRL_Cell cellacute("10 10 10 65 65 65");
   LRL_Cell cellobtuse("10 10 10  15 15 15");
   LatticeConverter lc;
   const  std::vector<std::pair<std::string, std::vector<double> > > vM3d = DeloneTypeList::Make3dCenteringMatrices();

   for (size_t i = 0; i < vM3d.size(); ++i) std::cout << i << " " << vM3d[i].first << std::endl;

   const MatS6 m6_R1(MatS6::e3Tos6(vM3d[4].second));
   std::cout << "R1" << std::endl;
   std::cout << LRL_Cell_Degrees(m6_R1 * S6(cellacute)) << std::endl;

   const MatS6 m6_R3(MatS6::e3Tos6(vM3d[4].second));
   std::cout << "R3" << std::endl;
   std::cout << LRL_Cell_Degrees(m6_R3 * cellobtuse) << std::endl;
   std::cout << LRL_Cell_Degrees(lc.DeloneReduceCell("P", m6_R3 * S6(cellobtuse))) << std::endl;

}

class AngularS6 {
public:
   AngularS6() : m_isValid(false) {}
   AngularS6(const S6& s, const MatS6& m);
   double AngleBetween(const AngularS6& s1, const AngularS6& s2) const;

   AngularS6 operator-(const AngularS6& s) const;
   double norm() const { return (*this).m_S6[0]; }

   bool m_isValid;
   S6 m_S6;
   MatS6 m_matS6;

};

double AngularS6::AngleBetween(const AngularS6& s1, const AngularS6& s2) const {
   double sum = 0.0;
   for (size_t i = 0; i < 6; ++i) sum += s1.m_S6[i] * s2.m_S6[i];
   return acos(sum / (s1.m_S6.norm() * s2.m_S6.norm()));
}

AngularS6 AngularS6::operator- (const AngularS6& a) const {
   AngularS6 as6;
   as6.m_S6[0] = AngleBetween((*this), a);
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

CNearTree<MatS6> BuildTree(const std::vector<MatS6>& v) {
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
      if (abs(s[k]) < 1.0e-6) {
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
   const double mind = sqrt(minNC(
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


   const double d00 = sqrt(s[0] * s[0] + s[1] * s[1] + s[2] * s[2] + s[3] * s[3]);
   const double d01 = sqrt(s[0] * s[0] + s[1] * s[1] + s[2] * s[2] + s[4] * s[4]);
   const double d02 = sqrt(s[0] * s[0] + s[1] * s[1] + s[3] * s[3] + s[4] * s[4]);
   const double d03 = sqrt(s[0] * s[0] + s[2] * s[2] + s[3] * s[3] + s[4] * s[4]);
   const double d04 = sqrt(s[1] * s[1] + s[2] * s[2] + s[3] * s[3] + s[4] * s[4]);
   const double d05 = sqrt(s[0] * s[0] + s[1] * s[1] + s[2] * s[2] + s[5] * s[5]);
   const double d06 = sqrt(s[0] * s[0] + s[1] * s[1] + s[3] * s[3] + s[5] * s[5]);
   const double d07 = sqrt(s[0] * s[0] + s[2] * s[2] + s[3] * s[3] + s[5] * s[5]);
   const double d08 = sqrt(s[1] * s[1] + s[2] * s[2] + s[3] * s[3] + s[5] * s[5]);
   const double d09 = sqrt(s[0] * s[0] + s[1] * s[1] + s[4] * s[4] + s[5] * s[5]);
   const double d10 = sqrt(s[0] * s[0] + s[2] * s[2] + s[4] * s[4] + s[5] * s[5]);
   const double d11 = sqrt(s[1] * s[1] + s[2] * s[2] + s[4] * s[4] + s[5] * s[5]);
   const double d12 = sqrt(s[0] * s[0] + s[3] * s[3] + s[4] * s[4] + s[5] * s[5]);
   const double d13 = sqrt(s[1] * s[1] + s[3] * s[3] + s[4] * s[4] + s[5] * s[5]);
   const double d14 = sqrt(s[2] * s[2] + s[3] * s[3] + s[4] * s[4] + s[5] * s[5]);




   return mind;
}

void DistancesToInvalidLattices() {
   for (size_t i = 0; i < 100; ++i) {
      S6 s(S6::rand());
      RHrand storeRand;

      s[size_t(rand() / double(RAND_MAX) * 6.0)] = 0.0;
      s[size_t(rand() / double(RAND_MAX) * 6.0)] = 0.0;
      //s[size_t(rand()/double(RAND_MAX) * 6.0)] = 0.0;

      const double d3v = DistanceTo_ValidThreeZeros(s);
      const double d3i = DistanceTo_INValidThreeZeros(s);
      const double d4i = DistanceTo_INValidFourZeros(s);

      const double test = abs(101.36766552387854 - d3i);
      if (d4i < d3i) std::cout << d3i << "  " << d4i << "    " << s << std::endl;

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
   StoreResults<double, PairReporter<double, double> > store2(5);
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
      const LRL_Cell c(s);
      m_v[0] = std::polar(c[0], c[3]);
      m_v[1] = std::polar(c[1], c[4]);
      m_v[2] = std::polar(c[2], c[5]);
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
      for (size_t i = 0; i < 3; ++i)   std::cout << r[i] << "," << 180.0 / (4.0 * atan(1.0)) * theta[i] << "   ";

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
   const S6 testlattice = S6(G6("100 100 100  100 100 100"));
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
   const std::complex<double> c = std::sqrt(std::complex<double>(csq, 0.0));

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

   const double d2 = CS6Dist((redc[1].first * s1).data(), (redc[1].first * s2).data());

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
      if (cycles < 10) continue;
      const size_t key = (cycles == 0) ? 0 : log10(cycles);
      sr.Store(key, LRL_ToString(cycles, "  ", s6in));
   }
   sr.ShowResults();
   exit(0);
}

void LookAtS6BoundaryCrossings() {
   for (size_t i = 0; i < 20; ++i) {
      S6 start = S6::randDeloneReduced();
      start[0] = 0.01;
      S6 end;
      const bool b = Selling::Reduce(start, end);
      //std::cout << start << "    " << end << std::endl;
      std::cout << S6(C3::ConvertToFundamentalUnit(start)) << "    " << S6(C3::ConvertToFundamentalUnit(end)) << std::endl;
      //std::cout << C3::ConvertToFundamentalUnit(start) << "    " << C3::ConvertToFundamentalUnit(end) << std::endl;
      //std::cout << LRL_Cell_Degrees(start) << "    " << LRL_Cell_Degrees(end) << std::endl;
   }
   exit(0);
}

void MakeRow_a(const size_t n, Matrix_3x3& m) {
   m[3 * n + 0] = 1;
   m[3 * n + 1] = 0;
   m[3 * n + 2] = 0;
}

void MakeRow_b(const size_t n, Matrix_3x3& m) {
   m[3 * n + 0] = 0;
   m[3 * n + 1] = 1;
   m[3 * n + 2] = 0;
}

void MakeRow_c(const size_t n, Matrix_3x3& m) {
   m[3 * n + 0] = 0;
   m[3 * n + 1] = 0;
   m[3 * n + 2] = 1;
}

Matrix_3x3 MakeMatrix(const LRL_Cell_Degrees& reference, const LRL_Cell_Degrees& cell) {
   Matrix_3x3 m;

   for (size_t i = 0; i < 3; ++i) {
      if (abs(cell[i] - reference[0]) < 1.0E-4) {
         MakeRow_a(i, m);
      }
      else if (abs(cell[i] - reference[1]) < 1.0E-4) {
         MakeRow_b(i, m);
      }
      else if (abs(cell[i] - reference[2]) < 1.0E-4) {
         MakeRow_c(i, m);
      }
      else {
         m[0] = -19191;
         return m;
      } {
      }
   }
   return m;
}

void Matrices() {
   const std::vector<MatS6>  vm = MatS6::GetReflections();
   const LRL_Cell cell("1 7 20 66 77 88:");

   for (size_t i = 0; i < vm.size(); ++i) {
      const LRL_Cell_Degrees cd = vm[i] * S6(cell);
      const Matrix_3x3 m = MakeMatrix(cell, cd);
      const double d = m[0] + 19191;
      const bool b = m[0] == -19191;
      if (m[0] != -19191) {
         std::cout << cd << std::endl;
         std::cout << vm[i] << std::endl;
         std::cout << m << std::endl;
         const std::string st = LRL_MaximaTools::MaximaFromMat(m);
         //std::cout << LRL_MaximaTools::MaximaFromMat(m) << std::endl;
      }
      else {
         const int i19191 = 19191;
      }
   }
   exit(0);
}

void GenerateFollowerPath() {
   std::vector<std::pair<S6, S6> > result;

   std::unique_ptr<FollowerPathGenerator> fpg(new FollowerPoint(5, S6::randDeloneUnreduced()));
   std::cout << (*fpg) << std::endl;

   std::cout << FollowerLine(5, S6::randDeloneUnreduced(), S6::randDeloneUnreduced()) << std::endl;
   std::cout << FollowerChord(5, S6("0 0 2 -100 -100 -100"), S6("0 0 4 -110 -110 -110")) << std::endl;
   std::cout << FollowerChord3(5, S6("0 0 0 -95 -95 -95"), S6("0 0 0 -105 -105 -105"), S6("0 0 0 -200 -200 -200")) << std::endl;
   std::cout << FollowerTriangle(5, S6("0 0 0 -95 -95 -95"), S6("0 0 0 -105 -105 -105"), S6("0 0 0 -200 -200 -200")) << std::endl;
   exit(0);
}
void TestDC1() {  // 
   StoreResults<int, std::string> storeSmall(20);
   StoreResults<int, std::string> storeLarge(20);
   StoreResults<int, std::string> storeOther(20);
   StoreResults<int, double> storeRatio(10);
   const double enlargeBy = 1000.0;
   for (size_t i = 0; i < 100000000; ++i) {
      G6 gRan = G6::randDeloneUnreduced();
      G6 g1;
      const bool b = Niggli::Reduce(gRan, g1);
      const G6 gdelta = CreateUnitOrthogonalVector(g1);
      g1 *= enlargeBy / g1.norm();
      G6 g2;
      const bool b2 = Niggli::Reduce(g1 + gdelta, g2);

      const DC d1(g1);
      const DC d2(g2);

      const double d = DC::DistanceBetween(d1, d2, 7);
      //const int key = int(log10(10.0 * d / (d1).Norm()));
      const double ncdist = NCDist(g1.data(), g2.data());
      int key = int(log10(d / ncdist));
      const double minRange = 0.5;
      const double maxRange = 3.0;
      {
         storeSmall.SetTitle(LRL_ToString("selected range less than ", minRange));
         storeLarge.SetTitle(LRL_ToString("selected range greater than ", maxRange));
         storeOther.SetTitle(LRL_ToString("outside range less than ", minRange, " or greater than ", maxRange));
         const G6& g1red = g1;
         const G6& g2red = g2;
         S6 s1red;
         S6 s2red;
         Selling::Reduce(g1, s1red);
         Selling::Reduce(g2, s2red);
         if (d < minRange)storeSmall.Store(key, LRL_ToString("d=", d, "  d1.norm  ", d1.norm(), "  ncdist ", ncdist, "    ", g1red, "  ---- ", g2red));
         else if (d > maxRange)storeLarge.Store(key, LRL_ToString("d=", d, "  d1.norm  ", d1.norm(), "  ncdist ", ncdist, "    ", g1red, "  ---- ", g2red));
         else storeOther.Store(key, LRL_ToString("d=", d, "  d1.norm  ", d1.norm(), "  ncdist ", ncdist, "    ", g1red, "  ---- ", g2red));

         storeRatio.Store(int(log10(d / ncdist)), d / ncdist);


         const double norm1 = g1red.norm();
         const double norm2 = g2red.norm();
         const double norm3 = d1.norm();
         const double norm4 = d2.norm();
         const double distCS = CS6Dist(s1red.data(), s2red.data());
         const double normDC = (d1 - d2).norm();
         const DC d1X(g1red);
         const DC d2X(g2red);
         //std::cout << "------------------------------------------------------------" << std::endl;
         //std::cout << " g1 " << g1 << std::endl;
         //std::cout << " g2 " << g2 << std::endl;
         //std::cout << "distances NC, CS, DC " << ncdist << " " << distCS << " " << d << std::endl;
         //std::cout << " d1 " << d1 << std::endl;
         //std::cout << " d2 " << d2 << std::endl;
         //std::cout << " g1red " << g1red << std::endl;
         //std::cout << " g2red " << g2red << std::endl;
         //std::cout << " S6_1 reduced " << S6(s1red) << std::endl;
         //std::cout << " S6_2 reduced " << S6(s2red) << std::endl;
         //std::cout << " cell1 " << LRL_Cell_Degrees(g1) << std::endl;
         //std::cout << " cell2 " << LRL_Cell_Degrees(g2) << std::endl << std::endl;
         //std::cout << " cell1 reduced " << LRL_Cell_Degrees(g1red) << std::endl;
         //std::cout << " cell2 reduced " << LRL_Cell_Degrees(g2red) << std::endl << std::endl;
         //std::cout << d1X << std::endl;
         //std::cout << d2X << std::endl;
      }
   }
   //storeSmall.ShowResultsByKeyDescending();
   //storeLarge.ShowResultsByKeyDescending();
   //storeOther.ShowResultsByKeyDescending();
   storeRatio.ShowResultsByKeyAscending();
   exit(0);
}

void TestDC3() {
   C3 c0(S6::rand());
   C3 c1(c0);
   C3 c2(c0);
   C3 c3(c0);
   C3 c4(c0);
   C3 c5(c0);

   std::cout << "c0 input " << c0 << std::endl;
   std::cout << "c1 input " << c1 << std::endl;
   std::cout << "c2 input " << c2 << std::endl;
   std::cout << std::endl;

   std::cout << "cell0 input " << LRL_Cell_Degrees(c0) << std::endl;
   std::cout << "cell1 input " << LRL_Cell_Degrees(c1) << std::endl;
   std::cout << "cell2 input " << LRL_Cell_Degrees(c2) << std::endl;
   std::cout << std::endl;

   S6 x0(c0);
   S6 x1(c0);
   S6 x2(c0);


   S6 s0;
   S6 s1;
   S6 s2;

   Selling::Reduce(x0, s0);
   Selling::Reduce(x1, s1);
   Selling::Reduce(x2, s2);


   std::swap(s1[0], s1[2]);
   std::swap(s2[1], s2[2]);

   std::cout << " swapped entries in reduced s1 and s2 " << std::endl;

   Selling::Reduce(s0, s0);
   Selling::Reduce(s1, s1);
   Selling::Reduce(s2, s2);

   std::cout << "c0           reduced " << c0 << std::endl;
   std::cout << "c1 swapped and reduced " << C3(s1) << std::endl;
   std::cout << "c2 swapped and reduced " << C3(s2) << std::endl;
   std::cout << std::endl;



   S6Dist s6dist;

   std::cout << "cell0 " << LRL_Cell_Degrees(s0) << "  Reduced" << std::endl;
   std::cout << "cell1 " << LRL_Cell_Degrees(s1) << "  Reduced" << std::endl;
   std::cout << "cell2 " << LRL_Cell_Degrees(s2) << "  Reduced" << std::endl;
   std::cout << std::endl;

   const double d1 = s6dist.DistanceBetween(s0, s1);
   const double d2 = s6dist.DistanceBetween(s0, s2);

   std::cout << "DC0 " << DC(s0) << std::endl;
   std::cout << "DC1 " << DC(s1) << std::endl;
   std::cout << "DC2 " << DC(s2) << std::endl;
   std::cout << std::endl;

   std::cout << "s0-s1 " << CS6Dist(s0.data(), s1.data()) << std::endl;
   std::cout << "s0-s2 " << CS6Dist(s0.data(), s2.data()) << std::endl;
   std::cout << "s1-s2 " << CS6Dist(s1.data(), s2.data()) << std::endl;
   std::cout << std::endl;


   exit(0);
}

void TestDC2()
{

}

void Gen_55A() {
   for (size_t i = 0; i < 10; ++i) {
      G6 g6 = G6::rand();
      g6[3] = 0;
      g6[5] = 0;
      const B4 b4(g6);
      const double d = (b4[0] + b4[1]).norm();
      if (d / 2.0 < sqrt(g6[2])) {
         std::cout << "I " << LRL_Cell_Degrees(g6) << std::endl;
      }
   }
   exit(0);
}

G6 TryToGetAGoodProjection(const MatG6& m, const int trials = 5) {
   G6 probe;
   int count = 0;
   while (!probe.IsValid() && count < trials) {
      const bool b = probe.IsValid();
      const G6 start = G6::randDeloneReduced();
      Niggli::Reduce(start, probe);
      probe = m * probe;
      ++count;
   }
   return probe;
}

void TestOneToCentered(const size_t samples, const std::string& str) {
   for (size_t k = 0; k < samples; ++k) {
      const std::vector<std::shared_ptr<GenerateNiggliBase> >
         vglb = GenerateNiggliBase().Select(str);

      for (size_t i = 0; i < vglb.size(); ++i) {
         const std::shared_ptr<GenerateNiggliBase> pt = vglb[i];
         const G6 probe = TryToGetAGoodProjection(pt->GetPrj(), 50);

         const G6 test = pt->GetToCenter() * probe;
         std::cout
            << pt->GetBravaisType() << " "
            << pt->GetITNumber() << " "
            << LRL_Cell_Degrees(test)
            << std::endl;
      }
   }
}

void TestToCentered() {
   TestOneToCentered(1, ""); // blank for all lattices.
   exit(0);
}

void TestGeneratedTypes() {
   //std::vector<std::shared_ptr<GenerateNiggliBase> > vglb;
   //std::vector<std::shared_ptr<GenerateNiggliBase> > vx;
   //std::vector<std::shared_ptr<GenerateNiggliBase> > vy;
   //std::vector<std::shared_ptr<GenerateNiggliBase> > vout;

   std::vector<G6> vg;
   vg = GenerateNiggliBase::Generate("", 10);
   std::cout << std::endl << std::endl;
   vg = GenerateNiggliBase::Generate("3", 10);
   std::cout << std::endl << std::endl;
   vg = GenerateNiggliBase::Generate("55A", 10);
   std::cout << std::endl << std::endl;
   vg = GenerateNiggliBase::Generate("o", 10);
   std::cout << std::endl << std::endl;
   vg = GenerateNiggliBase::Generate("mC", 10);
   std::cout << std::endl << std::endl;
   vg = GenerateNiggliBase::Generate("oS", 10);


   //vglb = GenerateNiggliBase().Select("");
   //vglb.clear();
   //vglb = GenerateNiggliBase().Select("3");
   //vx = GenerateNiggliBase().Select("1");
   //vout = GenerateLatticesBase::VecAdd(vglb, vx);
   //vglb.clear();
   //vglb = GenerateNiggliBase().Select("33");
   //vglb.clear();
   //vglb = GenerateNiggliBase().Select("o");
   //vglb.clear();
   //vglb = GenerateNiggliBase().Select("mC");
   //vglb.clear();

   exit(0);
}

void MatchLatticeType(const G6& g) {
   std::cout << "GenerateLatticesBase " << g << std::endl;
   static const std::vector<std::shared_ptr<GenerateNiggliBase> >
      vglb = GenerateNiggliBase().Select("");
   for (size_t i = 0; i < vglb.size(); ++i) {
      const std::shared_ptr<GenerateNiggliBase> pt = vglb[i];
      G6 probe = g;
      //Niggli::Reduce(g, probe);
      const G6 perpV = pt->GetPerp() * probe;
      double d = perpV.norm();
      if (d < 1.0E-8) d = 0.0;

      std::cout
         << pt->GetITNumber() << " "
         << pt->GetBravaisType() << " "
         << d << std::endl;
   }
}

void TestMatchLatticeType() {
   const std::vector<std::shared_ptr<GenerateNiggliBase> >
      vglb = GenerateNiggliBase().Select("");
   const G6 g(G6::randDeloneReduced());
   G6 preProbe;
   const bool b = Niggli::Reduce(g, preProbe);
   for (size_t i = 0; i < vglb.size(); ++i) {
      const G6 probe = vglb[i]->GetPrj() * preProbe;
      std::cout << "Match to " << vglb[i]->GetITNumber() << " " << probe << std::endl;
      MatchLatticeType(probe);
   }
   exit(0);
}

class SellaSearchResult {
public:
   friend std::ostream& operator<< (std::ostream&, const SellaSearchResult&);

   SellaSearchResult() {}
   std::string GetDeloneType() const { return m_DeloneType; }
   std::string GetBravaisType() const { return m_BravaisType; }
   S6 GetProbe() const { return m_probe; }
   S6 GetProjected() const { return m_projected; }

   void SetDeloneType(const std::string& str) { m_DeloneType = str; }
   void SetBravaisType(const std::string& str) { m_BravaisType = str; }
   void SetProbe(const S6& str) { m_probe = str; }
   void SetProjected(const S6& str) { m_projected = str; }

protected:
   std::string m_DeloneType;
   std::string m_BravaisType;
   S6 m_probe;
   S6 m_projected;
};

class SellaResult {
public:
   friend std::ostream& operator<< (std::ostream&, const SellaResult&);

   SellaResult() {}
   void SetDistance(const double a) { m_distance = a; }
   void SetIT_Type(const std::string& a) { m_IT_Type = a; }
   void SetBravaisClass(const std::string& a) { m_BravaisClass = a; }
   void SetGeneralBravaisClass(const std::string& a) { m_GeneralBravaisClass = a; }
   void SetProbe(const G6& a) { m_probe = a; }
   void SetProjected(const G6& a) { m_projected = a; }
   void SetCentered(const G6& a) { m_centered = a; }

   double GetDistance() const { return m_distance; }
   std::string GetIT_Type() const { return m_IT_Type; }
   std::string GetBravaisClass() const { return m_BravaisClass; }
   std::string GetGeneralBravaisClass() const { return m_GeneralBravaisClass; }
   G6 GetProbe() const { return m_probe; }
   G6 GetProjected() const { return m_projected; }
   G6 GetCentered() const { return m_centered; }
   bool operator< (const SellaResult& sr) const {
      return (*this).m_distance < sr.m_distance;
   }
protected:
   double m_distance;
   std::string m_IT_Type;
   std::string m_BravaisClass;
   std::string m_GeneralBravaisClass;
   G6 m_probe;
   G6 m_projected;
   G6 m_centered;
};


std::ostream& operator<< (std::ostream& o, const SellaSearchResult& sr) {
   o << "SELLA SEARCH RESULTS" << std::endl;
   o << "Delone Type " << sr.m_DeloneType << std::endl;
   o << "Bravais Type " << sr.m_BravaisType << std::endl;
   o << "Initial Probe S6 " << sr.m_probe << std::endl;
   o << "Projected Probe S6 " << sr.m_projected << std::endl;
   return o;
}

   std::ostream& operator<< (std::ostream& o , const SellaResult& sr) {
      o << "SELLA RESULT" << std::endl;
      o << "distance " << sr.m_distance << std::endl;
      o << "IT Type " << sr.m_IT_Type << std::endl;
      o << "Bravais Type " << sr.m_BravaisClass << std::endl;
      o << "Bravais General Class " << sr.m_GeneralBravaisClass << std::endl;
      o << "Initial reduced probe G6 " << sr.m_probe << std::endl;
      o << "Projected G6 " << sr.m_projected << std::endl;
      o << "Restore Cell Centering " << LRL_Cell_Degrees(sr.m_centered) << std::endl;
      o << " " << std::endl;
      return o;
   }


   StoreResults<double, std::string> store(100);
   std::vector<std::pair<double, std::string> > vstore;
   std::vector<SellaResult> sellaResult;

   void NiggliSearchForCenteredCell(
      const std::shared_ptr<GenerateNiggliBase>& pt, const G6& red) {
      const G6 proj = pt->GetPrj() * red;
      const double dtest = proj.norm();
      const double dist = (pt->GetPerp() * red).norm() *
         sqrt(pt->GetFreeParams());

      const MatG6& toCentered = pt->GetToCenter();
      const G6 centeredProjected = toCentered * proj;
      const G6 centeredReduced = toCentered * red;
      std::cout << pt->GetBravaisType() << std::endl;
      std::cout << pt->GetITNumber() << std::endl;
      std::cout << " proj " << centeredProjected << std::endl;
      std::cout << " red  " << red << std::endl << std::endl;
      std::cout << " centered  " << LRL_Cell_Degrees(centeredReduced) << std::endl << std::endl;

      const std::string blnk("  ");
      const std::string str = LRL_ToString(
         dist, " ", pt->GetITNumber(), blnk, pt->GetBravaisType(),
         blnk, proj, "\n");
      store.Store(dist, str);
      vstore.push_back(std::make_pair(dist, str));
      SellaResult sr;
      sr.SetDistance(dist);
      sr.SetBravaisClass(pt->GetBravaisType());
      sr.SetGeneralBravaisClass(pt->GetBravaisLatticeGeneral());
      sr.SetIT_Type(pt->GetITNumber());
      sr.SetProbe(red);
      sr.SetProjected(proj);
      sr.SetCentered(centeredReduced);
      sellaResult.push_back(sr);
   }

   int randInt(const int n) {
      return rand() % n;
   }

   G6 MakeOneSellaSample(const std::string& lat) {
      const std::vector<std::shared_ptr<GenerateNiggliBase> >
         vglb = GenerateNiggliBase().Select(lat);
      if (vglb.empty()) return G6();
      G6 gout;
      G6 gfinal;
      LRL_Cell_Degrees cel;
      gout.SetValid(false);
      gfinal.SetValid(false);
      int count = 0;
      while (((!gfinal.IsValid()) && count < 100) || cel[0] < 0.001) {
         ++count;
         const G6 g = G6::rand();
         Niggli::Reduce(g, gout);
         const G6 projected = vglb[0]->GetPrj() * gout;
         gfinal = MatS6::GetReflection(randInt(24)) * S6(vglb[0]->GetPrj() * gout);


         G6 what;
         Niggli::Reduce(gfinal, what);
         gfinal = PerturbVector(0.10, what);
         cel = gfinal;
         //const LRL_Cell_Degrees celwhat(what);
         //std::cout << "what    " << what << std::endl;
         //std::cout << "celwhat " << celwhat << std::endl;
         //std::cout << "centered what " << vglb[0]->GetToCenter() * what << std::endl;
         //std::cout << "centered what " << LRL_Cell_Degrees(vglb[0]->GetToCenter() * what) << std::endl;


         //const G6 test = vglb[0]->GetToCenter() * gfinal;
         //const LRL_Cell_Degrees celTest(test);
         //std::cout << "G6 to be returned " << gfinal << std::endl;
         //std::cout << "Cell to be returned " << cel << std::endl;
         //std::cout << "should be centered " << celTest << std::endl;
         //std::cout << "Projector" << std::endl << vglb[0]->GetPrj() << std::endl << std::endl;
      }

      SellaSearchResult ssr;
      ssr.SetDeloneType(lat);
      ssr.SetBravaisType(lat);
      ssr.SetProbe(gfinal);
      //ssr.SetProjected(const S6 & s) { m_projected = s; }
      std::cout << ssr << std::endl;




      if (gfinal.IsValid() && cel[0] > 0.001) return gfinal;
      //const G6 test = vglb[0]->GetToCenter() * v[0];
      ////std::cout << LRL_Cell_Degrees(test) << std::endl;
      return G6();
   }

   std::vector<G6> MakeSellaSamples(const size_t ngen, const std::string& lat) {
      const std::vector<std::shared_ptr<GenerateNiggliBase> >
         vglb = GenerateNiggliBase().Select(lat);
      std::vector<G6> vSample;
      std::vector<SellaSearchResult> vSellaSearchResult;
      while (vSample.size() < ngen) {
         const G6 gout = MakeOneSellaSample(lat);
         if (gout.IsValid()) vSample.push_back(gout);
      }

      //std::cout << vglb[0]->GetCharacter() << std::endl;
      //for (size_t i = 0; i < vSample.size(); ++i)
      //   std::cout << vSample[i] << std::endl;
      return vSample;
   }

   std::vector<G6> MakeSellaSamples(const std::string& lat, const size_t n) {
      return MakeSellaSamples(n, lat);
   }

   void TestSellaTypeSearch(const std::vector<G6> vg) {
      const std::vector<std::shared_ptr<GenerateNiggliBase> >
         vglb = GenerateNiggliBase().Select("mC");

      for (size_t ran = 0; ran < vg.size(); ++ran) {
         std::cout << "initial vector " << vg[ran] << std::endl;
         std::cout << "initial cell   " << LRL_Cell_Degrees(vg[ran]) << std::endl;
         G6 red;
         Niggli::Reduce(vg[ran], red);
         const LRL_Cell_Degrees celred(red);
         std::cout << "redu in TestSellaTypeSearch " << red << std::endl;
         std::cout << "redu in TestSellaTypeSearch " << celred << std::endl;
         for (size_t lat = 0; lat < vglb.size(); ++lat) {
            NiggliSearchForCenteredCell(vglb[lat], red);
            vglb[lat]->GetToCenter()* red;
            std::cout << "generated and centered " <<
               LRL_Cell_Degrees(vglb[lat]->GetToCenter() * red) << std::endl;
         }
      }

      ////store.ShowResults();
      //std::sort(vstore.begin(), vstore.end());
      //size_t stopAt = vstore.size();
      //if (vstore.size() > 2) {
      //   const double min = vstore[0].first;
      //   for (size_t i = 2; i < vstore.size(); ++i) {
      //      if (vstore[i].first > 50.0 * min) {
      //         stopAt = i;
      //         break;
      //      }
      //   }
      //}
      //for (size_t i = 0; i < stopAt; ++i) {
      //   std::cout << i << "  distance = " << vstore[i].first << std::endl
      //      << vstore[i].second << std::endl << std::endl;
      //}
      std::sort(sellaResult.begin(), sellaResult.end());
      size_t stopAtSR = sellaResult.size();
      if (sellaResult.size() > 2) {
         const double min = sellaResult[0].GetDistance();
         for (size_t i = 2; i < sellaResult.size(); ++i) {
            if (sellaResult[i].GetDistance() > 50.0 * min) {
               stopAtSR = i;
               break;
            }
         }
      }
      for (size_t i = 0; i < stopAtSR; ++i) {
         std::cout << sellaResult[i] << std::endl;
      }
      exit(0);
   }

   void TestNiggli() {
      const G6 ref("100252.964 16816.866 34424.733 4771.70 -1146.32 -81789.15");
      G6 xxxx = ref;
      const LRL_Cell referenceCell = LatticeConverter::NiggliReduceCell("P", ref);
      if (!referenceCell.IsValid()) {
         std::cout << "the reference cell is invalid " << std::endl;
         exit(99);
      }
      //const LRL_Cell_Degrees referenceCell(G6("109.004	115.054	147.436	83.538	46.654	85.14"));
      const G6 referenceG6(referenceCell);
      const double referenceNorm = referenceG6.norm();
      std::cout << "Reference" << referenceG6 << "   " <<
         LRL_Cell_Degrees(referenceCell) << std::endl << std::endl;

      for (size_t i = 0; i < 30000000; ++i) {
         std::cout << "=== i =================================== " << i << std::endl;
         const G6 ran(G6::rand());

         G6 redTest;
         MatG6 m = MatG6::Eye();
         const bool b = Niggli::Reduce(ran, m, redTest, 0.001);
         std::cout << "m after Niggli::Reduce\n" << m << std::endl << std::endl;
         if (!b) break;

         const G6 scaledReducedRan = redTest * referenceNorm / redTest.norm();
         const size_t count = Niggli::GetCycles();
         const double dist = (scaledReducedRan - referenceG6).norm() / referenceNorm;
         if (dist < 0.2 && dist != 0 && count > 4) {
            std::cout << i << " " << count << "  " << dist
               << "\n in main, scaled reduced ran    " << scaledReducedRan
               << "\n  ran as cell   " << LRL_Cell_Degrees(ran) << std::endl << std::endl;
            std::cout << m << std::endl << std::endl;
            std::cout << "after Niggli::Reduce\n" << m << std::endl << std::endl;
            std::cout << " m * ran  " << m * ran << std::endl;
            std::cout << " reduced  " << redTest << std::endl << std::endl;
         }
         if (count > 20) {
            std::cout << i << " " << count << "  " << dist << "     " << ran
               << "     " << LRL_Cell_Degrees(ran) << std::endl;
            std::cout << "after Niggli::Reduce\n" << m << std::endl << std::endl;
            std::cout << " m * ran  " << m * ran << std::endl;
            std::cout << " reduced  " << redTest << std::endl << std::endl;
            //std::cout << m << std::endl << std::endl;
         }
      }
      exit(0);
   }

   static void TestRI() {

      /*      */ {
         std::vector<double> vtest;
         RI ri1 = RI(vtest);
         vtest.resize(5);
         RI ri2 = RI(vtest);
         vtest.resize(6);
         RI ri3 = RI(vtest);
         vtest.resize(6);
         RI ri4 = RI(vtest);
         S6 s6l;
         RI ri5 = (s6l);
         G6 g6x;
         RI ri6 = (g6x);
         std::cout << "ri1 " << ri1 << std::endl;
         std::cout << "ri2 " << ri2 << std::endl;
         std::cout << "ri3 " << ri3 << std::endl;
         std::cout << "ri4 " << ri4 << std::endl;
         std::cout << "ri5" << ri5 << std::endl;
      }


      std::cout << "---------------------------" << std::endl;
      std::vector<S6> vs6;
      for (size_t i = 0; i < 10; ++i) {
         vs6.emplace_back(S6::randDeloneReduced());
      }
      for (size_t i = 0; i < vs6.size(); ++i) {
         std::cout << RI(vs6[i]) << std::endl;
      }

      exit(0);
   }

static void TestCommandArgs(int argc, char* argv[]) {
   const LRL_GetCommandArgs commandArgs(argc, argv);

   std::cout << "arg count = " << commandArgs.GetArgc() << std::endl;
   std::cout << "first arg = " << commandArgs.GetArgv(0) << std::endl;
   std::cout << "host = " << commandArgs.GetHost() << std::endl;
   std::cout << "raw prefix = " << commandArgs.GetRawPrefix() << std::endl;
   std::cout << "html prefix = " << commandArgs.GetHtmlPrefix() << std::endl;
   std::cout << "help = " << commandArgs.GetHelp() << std::endl;

   for (size_t i = 0; i < commandArgs.size(); ++i) {
      std::cout << " int = " << commandArgs.GetIntFromArgv(i) << std::endl;;
   }
   for (size_t i = 0; i < commandArgs.size(); ++i) {
      std::cout << " double = " << commandArgs.GetDoubleFromArgv(i) << std::endl;
   }
   for (size_t i = 0; i < commandArgs.size(); ++i) {
      std::cout << " string = " << commandArgs.GetStringFromArgv(i) << std::endl;
   }
   exit(0);
}

   int main(int argc, char* argv[])
   {
      TestCommandArgs(argc, argv);
      TestRI();
      C3::Test_C3();

      TestNiggli();



      std::vector<std::shared_ptr<GenerateDeloneBase> > sptest =
         GenerateLatticeTypeExamples::CreateListOfDeloneTypes();
      for (size_t i = 0; i < sptest.size(); ++i) {
         std::cout << *sptest[i] << std::endl;
      }
      // 10 14 17 20 25 27 28 29 30 37 39 41 43 (FAILS 17 43, fixed by multiple sampling,
      // some projections were invalid lattices)
      const std::vector<G6> vg = MakeSellaSamples(1, "57C");
      TestSellaTypeSearch(vg);
      exit(0);
      TestMatchLatticeType();
      TestToCentered();
      //std::cout << CreateUnitOrthogonalVector(G6("1 0 0  0 0 0")).Norm() << std::endl;
      //std::cout << CreateUnitOrthogonalVector(S6("1 0 0  0 0 0")).Norm() << std::endl;

      //std::cout << CreateUnitOrthogonalVector(G6(" 0 1 0  0 0 0")).Norm() << std::endl;
      //std::cout << CreateUnitOrthogonalVector(S6(" 0 1 0  0 0 0")).Norm() << std::endl;

      //std::cout << CreateUnitOrthogonalVector(G6("0 0 1  0 0 0")).Norm() << std::endl;
      //std::cout << CreateUnitOrthogonalVector(S6("0 0 1  0 0 0")).Norm() << std::endl;

      //std::cout << CreateUnitOrthogonalVector(G6("0 0  0 1 0 0")).Norm() << std::endl;
      //std::cout << CreateUnitOrthogonalVector(S6("0 0  0 1 0 0")).Norm() << std::endl;

      //std::cout << CreateUnitOrthogonalVector(G6("0 0  0 0 1 0")).Norm() << std::endl;
      //std::cout << CreateUnitOrthogonalVector(S6("0 0  0 0 1 0")).Norm() << std::endl;

      //std::cout << CreateUnitOrthogonalVector(G6("0 0  0 0 0 1")).Norm() << std::endl;
      //std::cout << CreateUnitOrthogonalVector(S6("0 0  0 0 0 1")).Norm() << std::endl;

      //TestDC1();
      TestGeneratedTypes();
      Gen_55A();
      TestDC3();
      exit(0);
      GenerateFollowerPath();
      Matrices();
      LookAtS6BoundaryCrossings();
      C3Matrices xxxxxxxx;
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