#pragma warning( disable : 4189) //   local variable is initialized but not referenced



#include "Delone.h"

#include "LRL_Cell.h"
#include "LRL_CoordinateConversionMatrices.h"
#include "S6.h"
#include "S6Dist.h"
#include "D7.h"
#include "D7Dist.h"
#include "G6.h"
#include "LRL_MinMaxTools.h"
#include "MatB4.h"
#include "MatS6.h"
#include "MatD7.h"
#include "Selling.h"

#include <algorithm>
#include <cfloat>
#include <map>
#include <utility>
#include <vector>

bool Delone::IsReduced(const G6& v) {
   return IsReduced(v, 0.0);
}

bool Delone::IsReduced(const G6& v, const double delta) {
   return v[3] <= delta && v[4] <= delta && v[5] <= delta;
}

bool Delone::Reduce(const S6& d, MatS6& m, S6& dd, const double delta) {
   delta;
   return Reduce(d, m, dd);
}

bool Delone::Reduce(const S6& d, S6& dd, const bool sellingFirst) {
   MatS6 m;
   return Reduce(d, m, dd);
}

bool Delone::Reduce(const D7& d, D7& dd, const bool sellingFirst) {
   MatS6 m;
   return Reduce(d, m, dd);
}

bool Delone::Reduce(const S6& d, S6& dd) {
   MatS6 m;
   return Reduce(d, m, dd);
}

bool Delone::Reduce(const D7& d, D7& dd) {
   MatS6 m;
   return Reduce(d, m, dd);
}

bool Delone::Reduce(const S6& d, MatS6& m, S6& dd) {
   const bool b = Selling::Reduce(d, m, dd);
   MatD7 m7;
   dd = sort(D7(dd), m7);
   m = m7 * MatD7(m);
   return true;
}

std::vector<MatD7> Delone::GetD7Reflections() {
   std::vector<MatD7> r;
   if (r.empty()) {
      MatD7 m;
      for (unsigned long i = 0; i < 24; ++i) {
         for (unsigned long j = 0; j < 49; ++j) {
            m[j] = D7Refl[i][j];
         }
         r.push_back(m);
      }
   }
   return r;
}

int Delone::GenMaxMinKey(const D7& d) {
   std::vector < std::pair<double, int> > v;
   for (int i = 0; i < 4; ++i) {
      v.push_back(std::make_pair(d[i], i));
   }
   const std::vector < std::pair<double, int> > vx(v);
   for (int j = 0; j < 3; ++j) {
      for (int i = 0; i < 3; ++i) {
         if (v[i].first > v[i + 1].first) {
            std::swap(v[i], v[i + 1]);
         }
      }
   }
   D7 dd;
   int key = 1000 * v[0].second + 100 * v[1].second + 10 * v[2].second + v[3].second;
   return key;
}

unsigned long Delone::FindRefl(const unsigned long key, const D7& random, std::set<unsigned long>& sr) {
   const static std::vector<MatD7> refl = GetD7Reflections();
   unsigned long n = INT_MAX;
   for (unsigned long i = 0; i < 24; ++i) {
      if (GenMaxMinKey(refl[i] * random) == 123) {
         sr.insert(key);
         n = i;
         break;
      }
   }
   return n;
}

bool Delone::IsDelone(const D7& v, const double delta) {
   const double Delonesum = v[0] + v[1] + v[2] + v[3] - v[4] - v[5] - v[6];
   return std::fabs(Delonesum) < delta;
}

D7 Delone::sort(const D7& d, MatD7& m) {
   m = MatD7::Eye();
   const static std::vector<MatD7> refl = GetD7Reflections();
   static std::set<unsigned long> sr;
   static std::map<unsigned long, unsigned long> sm;

   D7 random(d);
   unsigned long key = GenMaxMinKey(random);
   if (sr.find(key) == sr.end()) {
      const unsigned long n = FindRefl(key, random, sr);
      sm.insert(std::make_pair(key, n));
      m = (m*refl[n]).Reduce();
      return refl[n] * random;
   }
   else {
      m = (m*refl[sm[key]]).Reduce();
      return refl[sm[key]] * random;
   }
}


std::vector<MatS6> Delone::LoadLatticeTypeProjectors() {
   static const std::vector< std::pair<std::string, MatS6> > vLabledDeloneTypes = LoadLabeledLatticeTypeProjectors();
   static std::vector<MatS6> vDeloneTypes;

   if (!vDeloneTypes.empty()) return vDeloneTypes;

   for ( unsigned long i=0; i< vLabledDeloneTypes.size(); ++i) {
      vDeloneTypes.push_back(vLabledDeloneTypes[i].second);
   }

   return vDeloneTypes;
}

std::vector<std::pair<std::string, MatS6> > Delone::LoadLabeledLatticeTypeProjectors() {
   static const std::vector< std::pair<std::string, MatS6> > vDeloneTypes = PrivateLoadLabeledLatticeTypeProjectors();
   return vDeloneTypes;
}

std::vector<std::pair<std::string, MatS6> > Delone::PrivateLoadLabeledLatticeTypeProjectors() {
   static std::vector< std::pair<std::string, MatS6> > vDeloneTypes;
   if (!vDeloneTypes.empty()) return vDeloneTypes;
   vDeloneTypes.push_back(std::make_pair(std::string("K1 "), MatS6("1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1  1 1 1 1 1 1")));  //    K1  (cI)
   vDeloneTypes.push_back(std::make_pair(std::string("K3 "), MatS6("1 1 0 1 1 0  1 1 0 1 1 0  0 0 0 0 0 0  1 1 0 1 1 0  1 1 0 1 1 0  0 0 0 0 0 0")));  //    K3  (cF)
   vDeloneTypes.push_back(std::make_pair(std::string("K5 "), MatS6("0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 1 1 1  0 0 0 1 1 1  0 0 0 1 1 1")));  //    K5  (cP)
   vDeloneTypes.push_back(std::make_pair(std::string("Q1 "), MatS6("1 1 0 1 1 0  1 1 0 1 1 0  0 0 1 0 0 1  1 1 0 1 1 0  1 1 0 1 1 0  0 0 1 0 0 1")));  //    Q1  (tI)
   vDeloneTypes.push_back(std::make_pair(std::string("Q2 "), MatS6("1 1 0 1 1 0  1 1 0 1 1 0  0 0 0 0 0 0  1 1 0 1 1 0  1 1 0 1 1 0  0 0 0 0 0 1")));  //    Q2  (tI)
   vDeloneTypes.push_back(std::make_pair(std::string("Q5 "), MatS6("0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 1 1 0  0 0 0 1 1 0  0 0 0 0 0 1")));  //    Q5  (tP)
   vDeloneTypes.push_back(std::make_pair(std::string("R1 "), MatS6("1 1 1 0 0 0  1 1 1 0 0 0  1 1 1 0 0 0  0 0 0 1 1 1  0 0 0 1 1 1  0 0 0 1 1 1")));  //    R1  (rP)
   vDeloneTypes.push_back(std::make_pair(std::string("R3 "), MatS6("1 0 1 0 1 0  0 0 0 0 0 0  1 0 1 0 1 0  0 0 0 1 0 0  1 0 1 0 1 0  0 0 0 0 0 0")));  //    R3  (rP)
   vDeloneTypes.push_back(std::make_pair(std::string("O1A"), MatS6("1 1 0 1 1 0  1 1 0 1 1 0  0 0 1 0 0 0  1 1 0 1 1 0  1 1 0 1 1 0  0 0 0 0 0 1")));  //    O1A (oF)
   vDeloneTypes.push_back(std::make_pair(std::string("O1B"), MatS6("1 1 0 1 1 0  1 1 0 1 1 0  0 0 1 0 0 1  1 1 0 1 1 0  1 1 0 1 1 0  0 0 1 0 0 1")));  //    O1B (oI)
   vDeloneTypes.push_back(std::make_pair(std::string("O2 "), MatS6("1 0 0 0 1 0  0 1 0 1 0 0  0 0 0 0 0 0  0 1 0 1 0 0  1 0 0 0 1 0  0 0 0 0 0 0")));  //    O2  (oI)
   vDeloneTypes.push_back(std::make_pair(std::string("O3 "), MatS6("1 0 0 1 0 0  0 1 0 0 1 0  0 0 0 0 0 0  1 0 0 1 0 0  0 1 0 0 1 0  0 0 0 0 0 0")));  //    O3  (oI)
   vDeloneTypes.push_back(std::make_pair(std::string("O4 "), MatS6("0 0 0 0 0 0  0 0 0 0 0 0  0 0 1 0 0 0  0 0 0 1 1 0  0 0 0 1 1 0  0 0 0 0 0 1")));  //    O4  (oP)
   vDeloneTypes.push_back(std::make_pair(std::string("O5 "), MatS6("0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1")));  //    O5  (oP)
   vDeloneTypes.push_back(std::make_pair(std::string("M1A"), MatS6("1 1 0 0 0 0  1 1 0 0 0 0  0 0 1 0 0 0  0 0 0 1 1 0  0 0 0 1 1 0  0 0 0 0 0 1")));  //    M1A (mC)
   vDeloneTypes.push_back(std::make_pair(std::string("M1B"), MatS6("1 0 0 1 0 0  0 1 0 0 1 0  0 0 1 0 0 0  1 0 0 1 0 0  0 1 0 0 1 0  0 0 0 0 0 1")));  //    M1B (mC)
   vDeloneTypes.push_back(std::make_pair(std::string("M2A"), MatS6("1 0 0 1 0 0  0 1 0 0 1 0  0 0 0 0 0 0  1 0 0 1 0 0  0 1 0 0 1 0  0 0 0 0 0 1")));  //    M2A (mC)
   vDeloneTypes.push_back(std::make_pair(std::string("M2B"), MatS6("1 0 0 0 0 0  0 1 0 1 0 0  0 0 0 0 0 0  0 1 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1")));  //    M2B (mC)
   vDeloneTypes.push_back(std::make_pair(std::string("M3 "), MatS6("1 0 0 0 0 0  0 1 0 0 1 0  0 0 0 0 0 0  0 0 0 1 0 0  0 1 0 0 1 0  0 0 0 0 0 0")));  //    M3  (mC)
   vDeloneTypes.push_back(std::make_pair(std::string("M4 "), MatS6("0 0 0 0 0 0  0 0 0 0 0 0  0 0 1 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1")));  //    M4  (mP)
   vDeloneTypes.push_back(std::make_pair(std::string("T1 "), MatS6("1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1")));  //    T1  (aP)
   vDeloneTypes.push_back(std::make_pair(std::string("T2 "), MatS6("1 0 0 0 0 0  0 1 0 0 0 0  0 0 0 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1")));  //    T2  (aP)
   vDeloneTypes.push_back(std::make_pair(std::string("T3 "), MatS6("1 0 0 0 0 0  0 1 0 0 0 0  0 0 0 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 0")));  //    T3  (aP)
   vDeloneTypes.push_back(std::make_pair(std::string("H4 "), MatS6("0 0 0 0 0 0  0 0 0 0 0 0  0 0 1 1 1 0  0 0 1 1 1 0  0 0 1 1 1 0  0 0 0 0 0 1")));  //    H4  (hP)

   return vDeloneTypes;
}