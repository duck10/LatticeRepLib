
#include "stdafx.h"

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <utility>

#include "C3.h"
#include "Delone.h"
#include "LRL_inverse.h"
#include "LRL_ToString.h"
#include "PairReporter.h"
#include "S6Dist.h"
#include "Selling.h"


std::vector< S6(*)(const S6&)> S6Dist::m_reductionFunctions;
std::vector< S6(*)(const S6&)> S6Dist::m_UnReduceFunctions;
std::vector< S6(*)(const S6&)> S6Dist::m_reflectionFunctions;

std::vector< std::pair<S6(*)(const S6&), S6(*)(const S6&)> > S6Dist::SetUnreduceFunctionPairs() {
   std::vector< std::pair<S6(*)(const S6&), S6(*)(const S6&)> > vf;
   vf.push_back(std::make_pair(S6Dist::Unreduce11, S6Dist::Unreduce12));
   vf.push_back(std::make_pair(S6Dist::Unreduce21, S6Dist::Unreduce22));
   vf.push_back(std::make_pair(S6Dist::Unreduce31, S6Dist::Unreduce32));
   vf.push_back(std::make_pair(S6Dist::Unreduce41, S6Dist::Unreduce42));
   vf.push_back(std::make_pair(S6Dist::Unreduce51, S6Dist::Unreduce52));
   vf.push_back(std::make_pair(S6Dist::Unreduce61, S6Dist::Unreduce62));
   return vf;
}

void S6Dist::SetReduceFunctions() {
   m_reductionFunctions.push_back(S6Dist::Reduce11);
   m_reductionFunctions.push_back(S6Dist::Reduce21);
   m_reductionFunctions.push_back(S6Dist::Reduce31);
   m_reductionFunctions.push_back(S6Dist::Reduce41);
   m_reductionFunctions.push_back(S6Dist::Reduce51);
   m_reductionFunctions.push_back(S6Dist::Reduce61);
}

void S6Dist::SetUnreduceFunctions() {
   m_UnReduceFunctions.push_back(S6Dist::Unreduce11);
   m_UnReduceFunctions.push_back(S6Dist::Unreduce21);
   m_UnReduceFunctions.push_back(S6Dist::Unreduce31);
   m_UnReduceFunctions.push_back(S6Dist::Unreduce41);
   m_UnReduceFunctions.push_back(S6Dist::Unreduce51);
   m_UnReduceFunctions.push_back(S6Dist::Unreduce61);
}

S6Dist::S6Dist(const double dnearzero/* = 1.0*/)
   : m_nearzero(dnearzero)
   , m_dmin(DBL_MAX)
   , m_debug(false)
{
   SetReduceFunctions();
   SetUnreduceFunctions();
   m_reflectionFunctions = S6::SetRelectionFunctions();
}

S6 S6Dist::ApplyReductionFunction(const unsigned long n, const S6& d) const {
   const S6 dred(m_reductionFunctions[n](d));
   return dred;
}

S6 S6Dist::ApplyUnreduceFunction(const unsigned long n, const S6& d) const {
   const S6 dred(m_UnReduceFunctions[n](d));
   return dred;
}

void PrintVector(const std::vector<S6>& v1, const std::vector<S6>& v2) {
   S6Dist s6dist(50000.0);
   for (unsigned long i = 0; i < v2.size(); ++i) {
      const std::pair<double, unsigned long> p = s6dist.MinForListOfS6(v2[i], v1);
      std::cout << v2[i] << "              " << p.first << "   (" << p.second << "," << i << ")" << std::endl;
   }

}


void PrintVector(const std::vector<S6>& v) {
   for (unsigned long i = 0; i < v.size(); ++i) {
      std::cout << v[i] << std::endl;
   }
}

void PrintVector(const S6 v1, const std::vector<S6>& v2) {
   for (unsigned long i = 0; i < v2.size(); ++i) {
      std::cout << v2[i] << "              " << (v1, v2[i]).norm() << std::endl;
   }
}

void S6Dist::UnreduceAndAddToList(const S6& d, const unsigned long n, std::vector<S6>& v) const {
   const unsigned long vsize = (unsigned long)v.size();
   for (unsigned long i = 0; i < vsize; ++i) {
      // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! LCA
      S6 dtemp = ApplyUnreduceFunction(n, d);
      //S6 dtemp(v[i]);
      //dtemp[n] = 0.001 * dtemp.norm();
      v.push_back(dtemp);
   }
}

const double g_fractionToAssessNearZero = 0.10;

std::vector<S6> S6Dist::UnreduceAndAddToList(const std::vector<S6>& v6, const unsigned long n) const {
   std::vector<S6> vToReduce(v6);
   if (n > 0) {
      for (unsigned long k = 0; k < v6.size(); ++k) {
         const double d = v6[k].norm();
         for (unsigned long i = 0; i < 6; ++i) {
            if (v6[k][i] > -d * g_fractionToAssessNearZero && v6[k][i] <= 0.0) {
               S6 stemp = ApplyUnreduceFunction(i, v6[k]);
               vToReduce.push_back(stemp);
            }
         }
      }
      vToReduce = UnreduceAndAddToList(vToReduce, n - 1);
   }
   return vToReduce;
}

std::vector<S6> S6Dist::UnreduceAndAddToList(const S6& s6, const unsigned long n) const {
   std::vector<S6> vToReflect;
   vToReflect.push_back(s6);
   return UnreduceAndAddToList(vToReflect, n);
}

std::vector<unsigned long> GetListOfNearZero(const S6& vs6) {
   const double d = vs6.norm();
   std::vector<unsigned long> v;
   for (unsigned long i = 0; i < 6; ++i) {
      if (vs6[i] > -d * g_fractionToAssessNearZero && vs6[i] <= 0.0) {
         v.push_back(i);
      }
   }
   return v;
}

bool HasNearZero(const S6& v6) {
   const double d = v6.norm();
   for (unsigned long i = 0; i < 6; ++i) {
      if (v6[i] > -d * g_fractionToAssessNearZero && v6[i] <= 0.0) return true;
   }
   return false;
}

S6 MakeOneNearZeroPositive(const S6& vv6) {
   S6 v6(vv6);
   const double d = v6.norm();
   const double cutoff = d * g_fractionToAssessNearZero;
   for (unsigned long i = 0; i < 6; ++i) {
      if (v6[i] > -cutoff && v6[i] <= 0.0) {
         v6[i] = 0.001 * d;
         break;
      }
   }
   return v6;
}

std::vector<S6> ResetNearZeroAndAddToList3(const std::vector<S6>& vs6, const unsigned long n) /*const*/ {
   std::vector<S6>v6(vs6);
   if (n < 3) return v6;

   for (unsigned long k = 0; k < v6.size(); ++k) {
      if (S6::CountPositive(v6[k]) < 3 && HasNearZero(v6[k])) {
         v6.push_back(MakeOneNearZeroPositive(v6[k]));
      }
   }
   return v6;
}

std::vector<S6> ResetNearZeroAndAddToList2(const std::vector<S6>& vs6, const unsigned long n) /*const*/ {
   std::vector<S6>v6(vs6);
   if (n < 2) return v6;

   for (unsigned long k = 0; k < v6.size(); ++k) {
      if (S6::CountPositive(v6[k]) < 2 && HasNearZero(v6[k])) {
         v6.push_back(MakeOneNearZeroPositive(v6[k]));
      }
   }
   ResetNearZeroAndAddToList3(v6, n);
   return v6;
}

std::vector<S6> ResetNearZeroAndAddToList1(const std::vector<S6>& vs6, const unsigned long n) /*const*/ {
   std::vector<S6>v6(vs6);
   if (n < 1) return v6;

   for (unsigned long k = 0; k < v6.size(); ++k) {
      if (S6::CountPositive(v6[k]) < 1 && HasNearZero(v6[k])) {
         v6.push_back(MakeOneNearZeroPositive(v6[k]));
      }
   }
   ResetNearZeroAndAddToList2(v6, n);
   return v6;
}

std::vector<S6> S6Dist::ResetNearZeroAndAddToList(const std::vector<S6>& v6, const unsigned long n) const {
   std::vector<S6> vToReduce = ResetNearZeroAndAddToList1(v6, n);
   vToReduce = ResetNearZeroAndAddToList1(v6, n);
   return vToReduce;
}

std::vector<S6> ResetNearZeros1(const S6& s6, const std::vector<unsigned long>& vul) {
   std::vector<S6> v;
   v.push_back(s6);
   S6 s6out;
   for (unsigned long i = 0; i < vul.size(); ++i) {
      S6 temp(s6);
      temp[vul[i]] = 0.1;
      v.push_back(temp);
      Selling::Reduce(temp, s6out);
      v.push_back(s6out);
   }
   return v;
}

std::vector<S6> ResetNearZeros2(const S6& s6, const std::vector<unsigned long>& vul) {
   std::vector<S6> v;
   if (vul.size() >= 2) {
      S6 s6out;
      for (unsigned long k = 0; k < vul.size() - 1; ++k) {
         S6 temp(s6);
         temp[vul[k]] = 0.1;
         for (unsigned long i = 0; i < vul.size(); ++i) {
            temp[vul[i]] = 0.1;
         }
         v.push_back(temp);
         Selling::Reduce(temp, s6out);
         v.push_back(s6out);
      }
   }
   return v;
}

std::vector<S6> ResetNearZeros3(const S6& s6, const std::vector<unsigned long>& vul) {
   std::vector<S6> v;
   if (vul.size() >= 3) {
      for (unsigned long j = 0; j < vul.size() - 2; ++j) {
         S6 temp(s6);
         temp[vul[j]] = 0.1;
         for (unsigned long k = j + 1; k < vul.size() - 1; ++k) {
            temp[vul[k]] = 0.1;
            for (unsigned long i = k + 1; i < vul.size(); ++i) {
               temp[vul[i]] = 0.1;
            }
            v.push_back(temp);
         }
      }
   }
   return v;
}

std::vector<S6> S6Dist::ResetNearZeroAndAddToList(const S6& s6) const {
   std::vector<S6> vs6;
   std::vector<unsigned long> v = GetListOfNearZero(s6);
   vs6 = ResetNearZeros1(s6, v);
   std::vector<S6> temp2 = ResetNearZeros2(s6, v);
   vs6.insert(vs6.end(), temp2.begin(), temp2.end());
   std::vector<S6> temp3 = ResetNearZeros3(s6, v);
   vs6.insert(vs6.end(), temp3.begin(), temp3.end());
   return vs6;
}

std::vector<S6> S6Dist::GenerateReflectionsAtZero(const S6& s6) const {
   std::vector<S6> vToReflect;
   vToReflect.push_back(s6);

   const double d = s6.norm();
   for (unsigned long i = 0; i < 6; ++i) {
      if (s6[i] > -d * g_fractionToAssessNearZero && s6[i] <= 0.0) {
         UnreduceAndAddToList(s6, i, vToReflect);
      }
   }

   const unsigned long vsize = (unsigned long)vToReflect.size();
   for (unsigned long i = 0; i < vsize; ++i) {
      for (unsigned long k = 0; k < 24; ++k) {
         vToReflect.push_back(MatS6::GetReflection(k) * vToReflect[i]);
      }
   }

   return vToReflect;
}

StoreResults<double, std::string> g_debug(1);
StoreResults<double, std::pair<S6, S6> >g_bestVectors(1);

std::pair<double, unsigned long> S6Dist::MinForListOfS6(const std::vector<S6>& v1, const CNearTree<S6>& tree) {
   g_debug.clear();
   g_bestVectors.clear();
   g_debug.SetTitle("    MinForListOfS6 vector tree*************** *************** *************** *************** S6 Distance Calculations ********************");
   g_debug.SetFooter("    MinForListOfS6 vector tree*************** *************** *************** END END END END S6 Distance Calculations ********************\n");
   S6 s6min(tree[0]);
   double dmin = (v1[0] - s6min).norm();
   const double diff = std::abs(dmin - 37.183);
   std::pair<double, unsigned long> p = std::make_pair(m_dmin, 0);
   const std::string itemA = LRL_ToString(v1[0]) + std::string("\n") + LRL_ToString(s6min);
   const int nzero = s6min.CountZeros();
   if (m_debug)
      g_debug.Store(dmin, itemA);
   g_bestVectors.Store(dmin, std::make_pair(v1[0], s6min));

   for (unsigned long i = 0; i < v1.size(); ++i) {
      const CNearTree<S6>::iterator it = tree.NearestNeighbor(m_dmin, v1[i]);
      if (it != tree.end()) {
         const double test = (*it)[0];
         std::string item;
         g_bestVectors.Store(std::min(dmin, (v1[i] - (*it)).norm()), std::make_pair(v1[i], *it));
         if (m_debug /*&& it != tree.end()*/) {
            item = LRL_ToString(v1[i]) + std::string("\n") + LRL_ToString(*it);
            const int nzero2 = (*it).CountZeros();
            g_debug.Store(std::min(dmin, (v1[i] - (*it)).norm()), item);
         }
         if (it != tree.end()) {
            const double first = (*it)[0];
            const double dist = (v1[i] - (*it)).norm();
            std::pair<double, unsigned long> ptemp = std::make_pair(dist, 0);
            if (dist < dmin) {
               p.second = i;
               p.first = ptemp.first;
               dmin = p.first;
               s6min = *it;
            }
         }
         if (m_debug) {
            //std::cout << std::endl;
            //std::cout << v1[p.second] << std::endl;
            //std::cout << s6min << "    dmin A " << dmin  << std::endl;
         }
      }
   }
   if (m_debug) g_debug.ShowResultsByKeyDescending();
   return p;
}

std::pair<double, unsigned long> S6Dist::MinForListOfS6(const std::vector<S6>& v1, const std::vector<S6>& v2) {
   g_debug.SetTitle("    MinForListOfS6 vector*************** *************** *************** *************** S6 Distance Calculations ********************");
   g_debug.SetFooter("    MinForListOfS6 vector*************** *************** *************** END END END END S6 Distance Calculations ********************\n");
   std::pair<double, unsigned long> p;
   double dmin = DBL_MAX;
   std::pair<double, unsigned long> ptemp;

   {
      dmin = DBL_MAX;
      for (long iouter = 0; iouter < v1.size(); ++iouter) {
         ptemp = MinForListOfS6(v1[iouter], v2);
         if (ptemp.first < dmin) {
            dmin = ptemp.first;
         }
         g_bestVectors.Store(dmin, std::make_pair(v1[iouter], v2[ptemp.second]));
         if (m_debug) {
            const std::string item = LRL_ToString(v1[iouter]) + std::string("\n ") + LRL_ToString(v2[ptemp.second]);
            const int nzero = v2[ptemp.second].CountZeros();
            g_debug.Store(dmin, item);
         }
         p.second = iouter;
         p.first = ptemp.first;
      }
   }
   //if (m_debug) g_debug.ShowResultsByKeyDescending();   LCA  LCA
   //g_debug.clear();
   //g_bestVectors.clear();
   return p;
}

std::pair<double, unsigned long> S6Dist::MinForListOfS6(const S6& d1, const std::vector<S6>& v) {
   ////double dmin = std::min(dminSoFar,(d1 - v[0]).norm());
   //const double diff = std::abs(m_dmin - 37.183);
   //const std::string itemA = LRL_ToString(d1) + std::string("\n") + LRL_ToString(v[0]);
   //g_debug.Store(m_dmin, itemA);
   double dmin = m_dmin;
   std::pair<double, unsigned long> p = std::make_pair(m_dmin, 0);
   for (unsigned long i = 0; i < v.size(); ++i) {
      double dtemp = (d1 - v[i]).norm();
      if (dtemp < dmin) {
         p = std::make_pair(dtemp, i);
         dmin = p.first;
         g_bestVectors.Store(dmin, std::make_pair(d1, v[i]));
         if (m_debug) {
            const std::string item = LRL_ToString(d1) + std::string("\n") + LRL_ToString(v[i]);
            const int nzero = v[i].CountZeros();
            g_debug.Store(dmin, item);
         }
      }
   }
   return p;
}

const std::vector<S6> S6Dist::Generate24Reflections(const S6& s6in) {
   std::vector<S6> v;
   for (unsigned long i = 0; i < 24; ++i) {
      v.push_back(m_reflectionFunctions[i](s6in));
   }
   return v;
}

const std::vector<S6> S6Dist::Generate24Reflections(const std::vector<S6>& vin) {
   std::vector<S6> vout;
   for (unsigned long i = 0; i < vin.size(); ++i) {
      const std::vector<S6> vtemp = Generate24Reflections(vin[i]);
      vout.insert(vout.end(), vtemp.begin(), vtemp.end());
   }
   return vout;
}

//
// VCP = Virtual Cartesian Point
//
S6 ZeroOneScalar(const unsigned long n, const S6& s) {
   S6 s6(s);
   s6[n] = 0.0;
   return s6;
}

std::vector<S6> S6Dist::Create_VCP_ForTwoScalars(const S6& s) {
   std::vector<S6> v;
   for (unsigned long j = 0; j < 5; ++j) {
      const S6 s1 = Create_VCP_ForOneScalar(j, s);
      for (unsigned long k = j + 1; k < 6; ++k) {
         v.push_back(Create_VCP_ForOneScalar(k, s1));
      }
   }
   return Generate24Reflections(v);
}

std::vector<S6> S6Dist::Create_VCP_ForTwoScalars(const std::vector<S6>& v) {
   std::vector<S6> voutside;
   for (unsigned long j = 0; j < v.size(); ++j) {
      const S6 s1 = Create_VCP_ForOneScalar(j, v[j]);
      for (unsigned long k = j + 1; k < 6; ++k) {
         voutside.push_back(Create_VCP_ForOneScalar(k, s1));
      }
   }
   return Generate24Reflections(voutside);
}

S6 S6Dist::Create_VCP_ForOneScalar(const unsigned long n, const S6& s) {
   static const std::vector< S6(*)(const S6&)> unreducers(S6::SetUnreduceFunctions());

   S6 szu(unreducers[n](ZeroOneScalar(n, s)));
   szu[n] = -s[n];
   const double nzero = szu.CountZeros();
   return szu;
}

std::vector<S6> S6Dist::Create_VCP_s(const std::vector<S6>& vs) {
   std::vector<S6> voutside;
   for ( unsigned long i=0; i<vs.size(); ++i ) {
      const std::vector<S6> v(Create_VCP_s(vs[i]));
      voutside.insert(voutside.end(), v.begin(), v.end());
   }
   return voutside;
}

//
// VCP == Virtual Cartesian Point(s)
//
std::vector<S6> S6Dist::Create_VCP_s(const S6& s) {
   std::vector<S6> voutside(Generate24Reflections(s));

   for (unsigned long i = 0; i < 6; ++i) {
      if (s[i] < m_dmin) {
         const std::vector<S6> v(Generate24Reflections(Create_VCP_ForOneScalar(i, s)));
         voutside.insert(voutside.end(), v.begin(), v.end());
      }
   }
   return voutside;
   /*
   
   for (unsigned long i24 = 0; i24 < 24; ++i24) {
      for (unsigned long i = 0; i < 6; ++i) {
         const std::vector<S6> v(Generate24Reflections(Create_VCP_ForOneScalar(i, voutside[i24])));
         voutside.insert(voutside.end(), v.begin(), v.end());
      }
   }
   */
}

void S6Dist::OneBoundaryDistance(const S6& s1, const S6& s2) {
   std::vector<S6> vinside(1, s1);
   std::vector<S6> voutside(Create_VCP_s(Generate24Reflections(s2)));
   voutside.push_back(s2);
   std::pair<double, unsigned long> p = MinForListOfS6(s1, voutside);
   m_dmin = std::min(m_dmin, p.first);
}

void S6Dist::TwoBoundaryDistance(const S6& s1, const S6& s2) {
   std::vector<S6> vinside(1, s1);
   std::vector<S6> voutside(Create_VCP_ForTwoScalars(s2));
   voutside.push_back(s2);
   std::pair<double, unsigned long> p = MinForListOfS6(vinside, voutside);
   m_dmin = std::min(m_dmin, p.first);
}

std::vector<S6> S6Dist::ReduceIfLessThanDmin(const double dmin, const S6 s) const {
   std::vector<S6> v;
   const static std::vector<S6(*)(const S6&)> redFun = S6::SetReduceFunctions();

   const std::vector<S6> vref = Generate24Reflections(s);
   for (unsigned long kr = 0; kr < vref.size(); ++kr) {
      S6 s6temp(vref[kr]);
      for (unsigned long i = 0; i < 6; ++i) {
         if (s6temp[i] <= 0.0 && std::abs(s6temp[i]) <= dmin) {
            const double temp = s6temp[i];
            s6temp[i] = 0.0;
            s6temp = redFun[i](s6temp);
            s6temp[i] = temp;
            v.push_back(s6temp);
         }
      }
   }
   return Generate24Reflections(v);
}

double S6Dist::DistanceBetween(const S6& s1, const S6& s2) {
   g_debug.clear();
   g_bestVectors.clear();
   m_dmin = (s1 - s2).norm();
   g_bestVectors.Store(m_dmin, std::make_pair(s1, s2));
   if (m_debug) {
      const std::string item = LRL_ToString(s1) + std::string("\n ") + LRL_ToString(s2);
      g_debug.Store(m_dmin, item);
   }
   OneBoundaryDistance(s1, s2);
   //TwoBoundaryDistance(s1, s2);
   return m_dmin;
}

double S6Dist::DistanceBetween1(const S6& s1, const S6& s2) {
   m_dmin = (s1 - s2).norm();
   g_bestVectors.Store(m_dmin, std::make_pair(s1, s2));
   if (m_debug) {
      const std::string item = LRL_ToString(s1) + std::string("\n ") + LRL_ToString(s2);
      g_debug.Store(m_dmin, item);
   }
   OneBoundaryDistance(s1, s2);
   g_debug.clear();
   g_bestVectors.clear();
   return m_dmin;
}

double S6Dist::DistanceBetween2(const S6& s1, const S6& s2) {
   m_dmin = (s1 - s2).norm();
   g_bestVectors.Store(m_dmin, std::make_pair(s1, s2));
   if (m_debug) {
      const std::string item = LRL_ToString(s1) + std::string("\n ") + LRL_ToString(s2);
      g_debug.Store(m_dmin, item);
   }
   OneBoundaryDistance(s1, s2);
   TwoBoundaryDistance(s1, s2);
   g_debug.clear();
   g_bestVectors.clear();
   return m_dmin;
}

MatS6 Inverse(const MatS6& min) {
   MatS6 m(min);
   std::vector<double> arout(36);
   inverse(6, min.GetVector().data(), arout.data());
   m.SetVector(arout);
   return m;
}

std::vector<std::pair<MatS6, MatS6> > S6Dist::SetunreductionReductionMatricesFromReductionMatrices() {
   std::vector<std::pair<MatS6, MatS6> > vRed;

   // first and second of each are the two alternate unreduce matrices - related by a reflection

   vRed.push_back(std::make_pair(Inverse(MatS6("-1 0 0 0 0 0   1 1 0 0  0 0   1 0 0 0 1 0   -1 0 0 1 0 0   1 0 1 0 0 0   1 0 0 0 0 1")),
      Inverse(MatS6("-1 0 0 0 0 0   1 0 0 0  0 1   1 0 1 0 0 0   -1 0 0 1 0 0   1 0 0 0 1 0   1 1 0 0 0 0"))));  // g or p

   vRed.push_back(std::make_pair(Inverse(MatS6(" 1 1 0 0 0 0   0 -1 0 0 0 0   0 1 0 1 0 0   0 1 1 0 0 0   0 -1 0 0 1 0   0 1 0 0 0 1")),
      Inverse(MatS6(" 0 1 0 0 0 1   0 -1 0 0 0 0   0 1 1 0 0 0   0 1 0 1 0 0   0 -1 0 0 1 0   1 1 0 0 0 0"))));  // h or q

   vRed.push_back(std::make_pair(Inverse(MatS6(" 1 0 1 0 0 0   0  0 1 1 0 0   0 0 -1 0 0 0   0 1 1 0 0 0   0 0 1 0  1 0   0 0 -1 0 0 1")),
      Inverse(MatS6(" 0 0 1 0 1 0   0  1 1 0 0 0   0 0 -1 0 0 0   0 0 1 1 0 0   1 0 1 0  0 0   0 0 -1 0 0 1"))));  // k or r

   vRed.push_back(std::make_pair(Inverse(MatS6(" 1 0 0 -1 0 0   0 0 1 1 0 0   0 1 0 1 0 0   0 0 0 -1 0 0   0 0 0 1 1 0   0 0 0 1 0 1")),
      Inverse(MatS6(" 1 0 0 -1 0 0   0 1 0 1 0 0   0 0 1 1 0 0   0 0 0 -1 0 0   0 0 0 1 0 1   0 0 0 1 1 0"))));  // l or s

   vRed.push_back(std::make_pair(Inverse(MatS6(" 0 0 1 0 1 0   0 1 0 0 -1 0   1 0 0 0 1 0   0 0 0 1 1 0   0 0 0 0 -1 0   0 0 0 0 1 1")),
      Inverse(MatS6(" 1 0 0 0 1 0   0 1 0 0 -1 0   0 0 1 0 1 0   0 0 0 0 1 1   0 0 0 0 -1 0   0 0 0 1 1 0"))));  // m or t

   vRed.push_back(std::make_pair(Inverse(MatS6(" 0 1 0 0 0 1   1 0 0 0 0 1   0 0 1 0 0 -1   0 0 0 1 0 1   0 0 0 0 1 1   0 0 0 0 0 -1")),
      Inverse(MatS6(" 1 0 0 0 0 1   0 1 0 0 0 1   0 0 1 0 0 -1   0 0 0 0 1 1   0 0 0 1 0 1   0 0 0 0 0 -1"))));  // n or u

   for (unsigned long i = 0; i < vRed.size(); ++i) {
      for (unsigned long k = 0; k < 36; ++k) {
         if (abs(vRed[i].first[k]) < 0.001) vRed[i].first[k] = 0.0;
         if (abs(vRed[i].second[k]) < 0.001) vRed[i].second[k] = 0.0;
      }
   }
   return vRed;
}

std::vector<std::pair<MatS6, MatS6> > S6Dist::SetUnreductionMatrices() {
   return S6::SetUnreductionMatrices();
}

S6 S6Dist::Reduce11(const S6& din) {
   return S6::Reduce11(din);
}

S6 S6Dist::Reduce12(const S6& din) {
   return S6::Reduce12(din);
}

S6 S6Dist::Reduce21(const S6& din) {
   return S6::Reduce21(din);
}

S6 S6Dist::Reduce22(const S6& din) {
   return S6::Reduce22(din);
}

S6 S6Dist::Reduce31(const S6& din) {
   return S6::Reduce31(din);
}

S6 S6Dist::Reduce32(const S6& din) {
   return S6::Reduce32(din);

}

S6 S6Dist::Reduce41(const S6& din) {
   return S6::Reduce41(din);
}

S6 S6Dist::Reduce42(const S6& din) {
   return S6::Reduce42(din);
}

S6 S6Dist::Reduce51(const S6& din) {
   return S6::Reduce51(din);
}

S6 S6Dist::Reduce52(const S6& din) {
   return S6::Reduce52(din);
}

S6 S6Dist::Reduce61(const S6& din) {
   return S6::Reduce61(din);
}

S6 S6Dist::Reduce62(const S6& din) {
   return S6::Reduce62(din);
}

S6 S6Dist::Unreduce11(const S6& din) {
   return S6::Unreduce11(din);
}

S6 S6Dist::Unreduce12(const S6& din) {
   return S6::Unreduce12(din);

}

S6 S6Dist::Unreduce21(const S6& din) {
   return S6::Unreduce21(din);

}

S6 S6Dist::Unreduce22(const S6& din) {
   return S6::Unreduce22(din);

}

S6 S6Dist::Unreduce31(const S6& din) {
   return S6::Unreduce31(din);

}

S6 S6Dist::Unreduce32(const S6& din) {
   return S6::Unreduce32(din);

}

S6 S6Dist::Unreduce41(const S6& din) {
   return S6::Unreduce41(din);

}

S6 S6Dist::Unreduce42(const S6& din) {
   return S6::Unreduce42(din);

}

S6 S6Dist::Unreduce51(const S6& din) {
   return S6::Unreduce51(din);

}

S6 S6Dist::Unreduce52(const S6& din) {
   return S6::Unreduce52(din);

}

S6 S6Dist::Unreduce61(const S6& din) {
   return S6::Unreduce61(din);

}

S6 S6Dist::Unreduce62(const S6& din) {
   return S6::Unreduce62(din);
}

std::pair< double, std::pair<S6, S6> > S6Dist::GetBestPosition() const {
   if (g_bestVectors.empty())
      return std::make_pair(DBL_MAX, std::make_pair(S6(), S6()));
   const double dmin = g_bestVectors.GetMinKey();
   const std::pair< double, std::pair<S6, S6> >p = g_bestVectors.GetFirstItem(dmin);
   return p;
}

std::string S6Dist::ReportS6Best(const S6Dist& s6dist) {
   const std::pair<double, std::pair<S6, S6> > p_report(s6dist.GetBestPosition());
   if ( p_report.first > 1.0E20) {
      return "NO S6 RESULTS";
   }
   else {
      return LRL_ToString( " ",
      p_report.first, "  ",
      p_report.second.first, "   ",
      p_report.second.second);
   }
}
