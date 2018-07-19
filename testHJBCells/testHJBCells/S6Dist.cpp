

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <utility>

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

S6 S6Dist::ApplyReductionFunction(const size_t n, const S6& d) const {
   const S6 dred(m_reductionFunctions[n](d));
   return dred;
}

S6 S6Dist::ApplyUnreduceFunction(const size_t n, const S6& d) const {
   const S6 dred(m_UnReduceFunctions[n](d));
   return dred;
}

void S6Dist::UnreduceAndAddToList(const S6& d, const size_t n, std::vector<S6>& v) const {
   const size_t vsize = (size_t)v.size();
   for (size_t i = 0; i < vsize; ++i) {
      // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! LCA
      S6 dtemp = ApplyUnreduceFunction(n, d);
      //S6 dtemp(v[i]);
      //dtemp[n] = 0.001 * dtemp.norm();
      v.push_back(dtemp);
   }
}

const double g_fractionToAssessNearZero = 0.10;

std::vector<S6> S6Dist::UnreduceAndAddToList(const std::vector<S6>& v6, const size_t n) const {
   std::vector<S6> vToReduce(v6);
   if (n > 0) {
      for (size_t k = 0; k < v6.size(); ++k) {
         const double d = v6[k].norm();
         for (size_t i = 0; i < 6; ++i) {
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

std::vector<S6> S6Dist::UnreduceAndAddToList(const S6& s6, const size_t n) const {
   std::vector<S6> vToReflect;
   vToReflect.push_back(s6);
   return UnreduceAndAddToList(vToReflect, n);
}

std::vector<size_t> GetListOfNearZero(const S6& vs6) {
   const double d = vs6.norm();
   std::vector<size_t> v;
   for (size_t i = 0; i < 6; ++i) {
      if (vs6[i] > -d * g_fractionToAssessNearZero && vs6[i] <= 0.0) {
         v.push_back(i);
      }
   }
   return v;
}

bool HasNearZero(const S6& v6) {
   const double d = v6.norm();
   for (size_t i = 0; i < 6; ++i) {
      if (v6[i] > -d * g_fractionToAssessNearZero && v6[i] <= 0.0) return true;
   }
   return false;
}

S6 MakeOneNearZeroPositive(const S6& vv6) {
   S6 v6(vv6);
   const double d = v6.norm();
   const double cutoff = d * g_fractionToAssessNearZero;
   for (size_t i = 0; i < 6; ++i) {
      if (v6[i] > -cutoff && v6[i] <= 0.0) {
         v6[i] = 0.001 * d;
         break;
      }
   }
   return v6;
}

std::vector<S6> ResetNearZeroAndAddToList3(const std::vector<S6>& vs6, const size_t n) /*const*/ {
   std::vector<S6>v6(vs6);
   if (n < 3) return v6;

   for (size_t k = 0; k < v6.size(); ++k) {
      if (S6::CountPositive(v6[k]) < 3 && HasNearZero(v6[k])) {
         v6.push_back(MakeOneNearZeroPositive(v6[k]));
      }
   }
   return v6;
}

std::vector<S6> ResetNearZeroAndAddToList2(const std::vector<S6>& vs6, const size_t n) /*const*/ {
   std::vector<S6>v6(vs6);
   if (n < 2) return v6;

   for (size_t k = 0; k < v6.size(); ++k) {
      if (S6::CountPositive(v6[k]) < 2 && HasNearZero(v6[k])) {
         v6.push_back(MakeOneNearZeroPositive(v6[k]));
      }
   }
   ResetNearZeroAndAddToList3(v6, n);
   return v6;
}

std::vector<S6> ResetNearZeroAndAddToList1(const std::vector<S6>& vs6, const size_t n) /*const*/ {
   std::vector<S6>v6(vs6);
   if (n < 1) return v6;

   for (size_t k = 0; k < v6.size(); ++k) {
      if (S6::CountPositive(v6[k]) < 1 && HasNearZero(v6[k])) {
         v6.push_back(MakeOneNearZeroPositive(v6[k]));
      }
   }
   ResetNearZeroAndAddToList2(v6, n);
   return v6;
}

std::vector<S6> S6Dist::ResetNearZeroAndAddToList(const std::vector<S6>& v6, const size_t n) const {
   std::vector<S6> vToReduce = ResetNearZeroAndAddToList1(v6, n);
   vToReduce = ResetNearZeroAndAddToList1(v6, n);
   return vToReduce;
}

std::vector<S6> ResetNearZeros1(const S6& s6, const std::vector<size_t>& vul) {
   std::vector<S6> v;
   v.push_back(s6);
   S6 s6out;
   for (size_t i = 0; i < vul.size(); ++i) {
      S6 temp(s6);
      temp[vul[i]] = 0.1;
      v.push_back(temp);
      Selling::Reduce(temp, s6out);
      v.push_back(s6out);
   }
   return v;
}

std::vector<S6> ResetNearZeros2(const S6& s6, const std::vector<size_t>& vul) {
   std::vector<S6> v;
   if (vul.size() >= 2) {
      S6 s6out;
      for (size_t k = 0; k < vul.size() - 1; ++k) {
         S6 temp(s6);
         temp[vul[k]] = 0.1;
         for (size_t i = 0; i < vul.size(); ++i) {
            temp[vul[i]] = 0.1;
         }
         v.push_back(temp);
         Selling::Reduce(temp, s6out);
         v.push_back(s6out);
      }
   }
   return v;
}

std::vector<S6> ResetNearZeros3(const S6& s6, const std::vector<size_t>& vul) {
   std::vector<S6> v;
   if (vul.size() >= 3) {
      for (size_t j = 0; j < vul.size() - 2; ++j) {
         S6 temp(s6);
         temp[vul[j]] = 0.1;
         for (size_t k = j + 1; k < vul.size() - 1; ++k) {
            temp[vul[k]] = 0.1;
            for (size_t i = k + 1; i < vul.size(); ++i) {
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
   std::vector<size_t> v = GetListOfNearZero(s6);
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
   for (size_t i = 0; i < 6; ++i) {
      if (s6[i] > -d*g_fractionToAssessNearZero && s6[i] <= 0.0) {
         UnreduceAndAddToList(s6, i, vToReflect);
      }
   }

   return vToReflect;
}


std::pair<double, size_t> S6Dist::MinForListOfS6(const std::vector<S6>& v1, const std::vector<S6>& v2) {
   std::pair<double, size_t> p;
   double dmin = DBL_MAX;
   for (size_t iouter = 0; iouter < v1.size(); ++iouter) {
      std::pair<double, size_t> ptemp = MinForListOfS6(v1[iouter], v2);
      if (ptemp.first < dmin) {
         dmin = ptemp.first;
         p.second = iouter;
         p.first = ptemp.first;
      }
   }
   return p;
}

std::pair<double, size_t> S6Dist::MinForListOfS6( const S6& d1, const std::vector<S6>& v) {
   double dmin = m_dmin;
   std::pair<double, size_t> p = std::make_pair(m_dmin, 0);
   for (size_t i = 0; i < v.size(); ++i) {
      double dtemp = (d1 - v[i]).norm();
      if (dtemp < dmin) {
         p = std::make_pair(dtemp, i);
         dmin = p.first;
      }
   }
   return p;
}

const std::vector<S6> S6Dist::Generate24Reflections(const S6& s6in) {
   std::vector<S6> v;
   for (size_t i = 0; i < 24; ++i) {
      v.push_back(m_reflectionFunctions[i](s6in));
   }
   return v;
}

const std::vector<S6> S6Dist::Generate24Reflections(const std::vector<S6>& vin) {
   std::vector<S6> vout;
   for (size_t i = 0; i < vin.size(); ++i) {
      const std::vector<S6> vtemp = Generate24Reflections(vin[i]);
      vout.insert(vout.end(), vtemp.begin(), vtemp.end());
   }
   return vout;
}

//
// VCP = Virtual Cartesian Point
//
S6 ZeroOneScalar(const size_t n, const S6& s) {
   S6 s6(s);
   s6[n] = 0.0;
   return s6;
}

std::vector<S6> S6Dist::Create_VCP_ForTwoScalars(const S6& s) {
   std::vector<S6> v;
   for (size_t j = 0; j < 5; ++j ) {
      const S6 s1 = Create_VCP_ForOneScalar(j, s);
      for (size_t k=j+1; k<6; ++k) {
         v.push_back(Create_VCP_ForOneScalar(k, s1));
      }
   }
   return Generate24Reflections(v);
}

S6 S6Dist::Create_VCP_ForOneScalar(const size_t n, const S6& s) {
   static const std::vector< S6(*)(const S6&)> unreducers(S6::SetUnreduceFunctions());

   const S6 szu(unreducers[n](ZeroOneScalar(n, s)));
   S6 vcp(szu);
   vcp[n] = -s[n];
   return vcp;
}

//
// VCP == Virtual Cartesian Point(s)
//
std::vector<S6> S6Dist::Create_VCP_s(const S6& s) {
   std::vector<S6> voutside(Generate24Reflections(s));

   for (size_t i = 0; i < 6; ++i) {
      const std::vector<S6> v(Generate24Reflections(Create_VCP_ForOneScalar(i, s)));
      voutside.insert(voutside.end(), v.begin(), v.end());
   }
   return voutside;
}

void S6Dist::OneBoundaryDistance(const S6& s1, const S6& s2) {
   std::vector<S6> vinside(1, s1);
   std::vector<S6> voutside(Create_VCP_s(s2));
   voutside.push_back(s2);
   //std::vector<S6> vtemp = ReduceIfLessThanDmin(m_dmin, s2);
   //voutside.insert(voutside.end(), vtemp.begin(), vtemp.end());
   std::pair<double, size_t> p = MinForListOfS6(vinside, voutside);
   m_dmin = std::min(m_dmin, p.first);
}

void S6Dist::TwoBoundaryDistance( const S6& s1, const S6& s2) {
   std::vector<S6> vinside(1, s1);
   std::vector<S6> voutside(Create_VCP_ForTwoScalars(s2));
   voutside.push_back(s2);
   std::pair<double, size_t> p = MinForListOfS6(vinside, voutside);
   m_dmin = std::min(m_dmin, p.first);
}

std::vector<S6> S6Dist::ReduceIfLessThanDmin(const double dmin, const S6 s) const {
   std::vector<S6> v;
   const static std::vector<S6(*)(const S6&)> redFun = S6::SetReduceFunctions();

   const std::vector<S6> vref = Generate24Reflections(s);
   for (size_t kr = 0; kr < vref.size(); ++kr) {
      for (size_t i = 0; i < 6; ++i) {
         if (vref[kr][i] <= 0.0 && std::abs(vref[kr][i]) <= dmin) {
            v.push_back(redFun[i](vref[kr]));
         }
      }
   }
   return Generate24Reflections(v);
}

double S6Dist::DistanceBetween(const S6& s1, const S6& s2) {
  m_dmin = (s1 - s2).norm();
   OneBoundaryDistance(s1, s2);
   //TwoBoundaryDistance(s1, s2);
   return m_dmin;
}
double S6Dist::DistanceBetween1(const S6& s1, const S6& s2) {
   m_dmin = (s1 - s2).norm();
   OneBoundaryDistance(s1, s2);
   return m_dmin;
}

double S6Dist::DistanceBetween2(const S6& s1, const S6& s2) {
   m_dmin = (s1 - s2).norm();
   OneBoundaryDistance(s1, s2);
   TwoBoundaryDistance(s1, s2);
   return m_dmin;
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

