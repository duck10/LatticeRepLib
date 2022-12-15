


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
   , m_s6Debug(false)
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

void PrintVector(const std::vector<S6>& v1, const std::vector<S6>& v2) {
   S6Dist s6dist(50000.0);
   for (size_t i = 0; i < v2.size(); ++i) {
      const std::pair<double, size_t> p = s6dist.MinForListOfS6(v2[i], v1);
      std::cout << v2[i] << "              " << p.first << "   (" << p.second << "," << i << ")" << std::endl;
   }

}


void PrintVector(const std::vector<S6>& v) {
   for (size_t i = 0; i < v.size(); ++i) {
      std::cout << v[i] << std::endl;
   }
}

void PrintVector(const S6 v1, const std::vector<S6>& v2) {
   for (size_t i = 0; i < v2.size(); ++i) {
      std::cout << v2[i] << "              " << (v1, v2[i]).norm() << std::endl;
   }
}

StoreResults<double, std::string> g_debug(1);
StoreResults<double, std::pair<S6, S6> >g_bestVectors(1);

std::pair<double, size_t> S6Dist::MinForListOfS6(const std::vector<S6>& v1, const CNearTree<S6>& tree) {
   g_debug.clear();
   g_bestVectors.clear();
   g_debug.SetTitle("    MinForListOfS6 vector tree*************** *************** *************** *************** S6 Distance Calculations ********************");
   g_debug.SetFooter("    MinForListOfS6 vector tree*************** *************** *************** END END END END S6 Distance Calculations ********************\n");
   S6 s6min(tree[0]);
   double dmin = (v1[0] - s6min).norm();
   const double diff = std::abs(dmin - 37.183);
   std::pair<double, size_t> p = std::make_pair(m_dmin, 0);
   const std::string itemA = LRL_ToString(v1[0]) + std::string("\n") + LRL_ToString(s6min);
   const int nzero = s6min.CountZeros();
   if (m_s6Debug)
      g_debug.Store(dmin, itemA);
   g_bestVectors.Store(dmin, std::make_pair(v1[0], s6min));

   for (size_t i = 0; i < v1.size(); ++i) {
      const CNearTree<S6>::iterator it = tree.NearestNeighbor(m_dmin, v1[i]);
      if (it != tree.end()) {
         const double test = (*it)[0];
         std::string item;
         g_bestVectors.Store(std::min(dmin, (v1[i] - (*it)).norm()), std::make_pair(v1[i], *it));
         if (m_s6Debug /*&& it != tree.end()*/) {
            item = LRL_ToString(v1[i]) + std::string("\n") + LRL_ToString(*it);
            const int nzero2 = (*it).CountZeros();
            g_debug.Store(std::min(dmin, (v1[i] - (*it)).norm()), item);
         }
         if (it != tree.end()) {
            const double first = (*it)[0];
            const double dist = (v1[i] - (*it)).norm();
            std::pair<double, size_t> ptemp = std::make_pair(dist, 0);
            if (dist < dmin) {
               p.second = i;
               p.first = ptemp.first;
               dmin = p.first;
               s6min = *it;
            }
         }
         if (m_s6Debug) {
            std::cout << std::endl;
            std::cout << v1[p.second] << std::endl;
            std::cout << s6min << "    dmin A " << dmin  << std::endl;
         }
      }
   }
   g_bestVectors.Store(dmin, std::make_pair(v1[0], s6min));

   if (m_s6Debug) g_debug.ShowResultsByKeyDescending();
   return p;
}

std::pair<double, size_t> S6Dist::MinForListOfS6(const std::vector<S6>& v1, const std::vector<S6>& v2) {
   g_debug.SetTitle("    MinForListOfS6 vector*************** *************** *************** *************** S6 Distance Calculations ********************");
   g_debug.SetFooter("    MinForListOfS6 vector*************** *************** *************** END END END END S6 Distance Calculations ********************\n");
   std::pair<double, size_t> p;
   double dmin = DBL_MAX;
   std::pair<double, size_t> ptemp;

   {
      dmin = DBL_MAX;
      for (size_t iouter = 0; iouter < v1.size(); ++iouter) {
         if (v1[iouter].IsValid()) {
            ptemp = MinForListOfS6(v1[iouter], v2);
            g_bestVectors.Store(dmin, std::make_pair(v1[iouter], v2[ptemp.second]));
            if (m_s6Debug) {
               const std::string item = LRL_ToString(v1[iouter]) + std::string("\n ") + LRL_ToString(v2[ptemp.second]);
               const int nzero = v2[ptemp.second].CountZeros();
               g_debug.Store(dmin, item);
            }
            if (ptemp.first < dmin) {
               dmin = ptemp.first;
               p.first = ptemp.first;
               p.second = iouter;
            }
         }
      }
   }
//   if (m_s6Debug) g_debug.ShowResultsByKeyDescending(); //  LCA  LCA
   g_debug.clear();
   g_bestVectors.clear();
   return p;
}

std::pair<double, size_t> S6Dist::MinForListOfS6(const S6& d1, const std::vector<S6>& v) {
   const std::string itemA = LRL_ToString(d1) + std::string("\n") + LRL_ToString(v[0]);
   g_debug.Store(m_dmin, itemA);
   double dmin = m_dmin;
   std::pair<double, size_t> p = std::make_pair(m_dmin, 0);
   for (size_t i = 0; i < v.size(); ++i) {
      if (v[i].IsValid()) {
         double dtemp = (d1 - v[i]).norm();
         if (dtemp < dmin) {
            p = std::make_pair(dtemp, i);
            dmin = p.first;
            g_bestVectors.Store(dmin, std::make_pair(d1, v[i]));
            if (m_s6Debug) {
               const std::string item = LRL_ToString(d1) + std::string("\n") + LRL_ToString(v[i]);
               const int nzero = v[i].CountZeros();
               g_debug.Store(dmin, item);
            }
         }
      }
   }
   return p;
}

const std::vector<S6> S6Dist::Generate24Reflections(const S6& s6in) {
   std::vector<S6> v;
   v.resize(24);
   for (size_t i = 0; i < 24; ++i) {
      v[i] = m_reflectionFunctions[i](s6in);
   }
   return v;
}

const std::vector<S6> S6Dist::Generate24Reflections(const std::vector<S6>& vin) {
   std::vector<S6> vout;
   vout.reserve(24*vin.size());
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

std::vector<S6> S6Dist::Create_VCP_s(const std::vector<S6>& vs) {
   std::vector<S6> voutside;
   voutside.reserve(vs.size() * 6);
   for ( size_t i=0; i<vs.size(); ++i ) {
      const std::vector<S6> v(Create_VCP_s(vs[i]));
      voutside.insert(voutside.end(), v.begin(), v.end());
   }
   return voutside;
}

//
// VCP == Virtual Cartesian Point(s)
//
std::vector<S6> S6Dist::Create_VCP_s(const S6& s) {
   std::vector<S6> voutside(6);
   static const std::vector< S6(*)(const S6&)> fnRedn = S6Dist::SetVCPFunctions();

   for (size_t i = 0; i < 6; ++i) {
      voutside[i] = fnRedn[i]( s);
   }
   return voutside;
}

std::vector<S6> S6Dist::CreateSecondBoundary_VCP_s(const S6& s) {
   std::vector<S6> voutside(36);
   static const std::vector< S6(*)(const S6&)> fnRedn = S6Dist::SetVCPFunctions();

   size_t pos = 0;
   for (size_t i = 0; i < 6; ++i) {
      const S6 si = m_UnReduceFunctions[i](s);
      for (size_t k = 0; k < 6; ++k) {
         if (si[k] <= 0.0) {
            voutside[pos] = fnRedn[k]( si);
            ++pos;
         }
      }
   }

   return voutside;
}

std::vector<S6> Insert(const std::vector<S6>& tar, const std::vector<S6>& from) {
   std::vector<S6> target(tar);
   target.reserve(tar.size() + from.size());
   target.insert(target.end(), from.begin(), from.end());
   return target;
}

std::vector<S6> Insert(const std::vector<S6>& tar, const S6& from) {
   std::vector<S6> target(tar);
   target.push_back(from);
   return target;
}

void S6Dist::OneBoundaryDistance(const S6& s1, const S6& s2) {
   std::vector<S6> vinside(1, s1);
   m_dmin = DBL_MAX;

   std::vector<S6> voutside(Generate24Reflections(s2));
   voutside.push_back(s2);
   //vinside = Insert(vinside, (/*Create_VCP_s*/(s1)));
   voutside = Insert(voutside, ((((Create_VCP_s(s2))))));
   //voutside = Insert(voutside, ((Generate24Reflections((CreateSecondBoundary_VCP_s(s2))))));
   const std::pair<double, size_t> p = MinForListOfS6(vinside, voutside);
   m_dmin = std::min(m_dmin, p.first);
   if (m_dmin < p.first) {
      m_dmin = std::min(m_dmin, p.first);
      g_bestVectors.Store(m_dmin, std::make_pair(s1, s2));
   }
   else {
      m_dmin = p.first;
   }
}

void S6Dist::TwoBoundaryDistance(const S6& s1, const S6& s2) {
   m_dmin = DBL_MAX;
   std::vector<S6> vinside(1, s1);

   std::vector<S6> voutside(Generate24Reflections(s2));
   voutside.push_back(s2);
   vinside = Insert(vinside, (Create_VCP_s(s1)));
   voutside = Insert(voutside, ((Generate24Reflections((Create_VCP_s(s2))))));
   //voutside = Insert(voutside, ((Generate24Reflections((CreateSecondBoundary_VCP_s(s2))))));
   const std::pair<double, size_t> p = MinForListOfS6(vinside, voutside);
   if (m_dmin < p.first) {
      m_dmin = std::min(m_dmin, p.first);
      g_bestVectors.Store(m_dmin, std::make_pair(s1, s2));
   }
   else {
      m_dmin = p.first;
   }
}

double S6Dist::DistanceBetween(const S6& s1, const S6& s2) {

   g_debug.clear();
   g_bestVectors.clear();

   m_dmin = ( s1.IsValid() && s2.IsValid() ) ? (s1 - s2).norm() : DBL_MAX;
   g_bestVectors.Store(m_dmin, std::make_pair(s1, s2));
   if (m_s6Debug) {
      const std::string item = LRL_ToString(s1) + std::string("\n ") + LRL_ToString(s2);
      g_debug.Store(m_dmin, item);
   }
   TwoBoundaryDistance(s1, s2);
   //if (m_s6Debug) g_debug.ShowResultsByKeyDescending();
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

   vRed.push_back(std::make_pair(Inverse(MatS6("-1 0 0 0 0 0   1 1 0 0 0 0   1 0 0 0 1 0   -1 0 0 1 0 0   1 0 1 0 0 0   1 0 0 0 0 1")),
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

   for (size_t i = 0; i < vRed.size(); ++i) {
      for (size_t k = 0; k < 36; ++k) {
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


S6 S6Dist::VCP1(const S6& din) {
   // For unreducing scalar 11
   // MatS6(“-1 0 0 0 0 0    1 1 0 0 0 0    1 0 0 0 1 0    -1 0 0 1 0 0    1 0 1 0 0 0    1 0 0 0 0 1”);
   S6 d;
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss1 = -s1;
   ss2 = s2;
   ss3 = s5;
   ss4 = s4;
   ss5 = s3;
   ss6 = s6;
   return d;
}
S6 S6Dist::VCP2(const S6& din) {
   // For unreducing scalar 21
   // MatS6(“1 1 0 0 0 0    0 -1 0 0 0 0    0 1 0 1 0 0    0 1 1 0 0 0    0 -1 0 0 1 0    0 1 0 0 0 1”);
   S6 d;
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss2 = -s2;
   ss1 = s1;
   ss3 = s4;
   ss4 = s3;
   ss5 = s5;
   ss6 = s6;
   return d;
}

S6 S6Dist::VCP3(const S6& din) {
   // For unreducing scalar 31
   // MatS6(“1 0 1 0 0 0    0 0 1 1 0 0    0 0 -1 0 0 0    0 1 1 0 0 0    0 0 1 0 1 0    0 0 -1 0 0 1”);
   S6 d;
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss3 = -s3;
   ss1 = s1;
   ss2 = s4;
   ss4 = s2;
   ss5 = s5;
   ss6 = s6;
   return d;
}

S6 S6Dist::VCP4(const S6& din) {
   // For unreducing scalar 41
   // MatS6(“1 0 0 -1 0 0    0 0 1 1 0 0    0 1 0 1 0 0    0 0 0 -1 0 0    0 0 0 1 1 0    0 0 0 1 0 1”);
   S6 d;
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss4 = -s4;
   ss1 = s1;
   ss2 = s3;
   ss3 = s2;
   ss5 = s5;
   ss6 = s6;
   return d;
}

S6 S6Dist::VCP5(const S6& din) {
   // For unreducing scalar 51
   // MatS6(“0 0 1 0 1 0    0 1 0 0 -1 0    1 0 0 0 1 0    0 0 0 1 1 0    0 0 0 0 -1 0    0 0 0 0 1 1”);
   S6 d;
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss5 = -s5;
   ss1 = s3;
   ss2 = s2;
   ss3 = s1;
   ss4 = s4;
   ss6 = s6;
   return d;
}

S6 S6Dist::VCP6(const S6& din) {
   // For unreducing scalar 61
   // MatS6(“0 1 0 0 0 1    1 0 0 0 0 1    0 0 1 0 0 -1    0 0 0 1 0 1    0 0 0 0 1 1    0 0 0 0 0 -1”);
   S6 d;
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss6 = -s6;
   ss1 = s2;
   ss2 = s1;
   ss3 = s3;
   ss4 = s4;
   ss5 = s5;
   return d;
}

std::vector< S6(*)(const S6&)> S6Dist::SetVCPFunctions() {
   std::vector< S6(*)(const S6&)> fn;
   fn.push_back(VCP1);
   fn.push_back(VCP2);
   fn.push_back(VCP3);
   fn.push_back(VCP4);
   fn.push_back(VCP5);
   fn.push_back(VCP6);
   return fn;
}
