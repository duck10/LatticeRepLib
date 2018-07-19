#pragma warning( disable : 4100) //  unreferenced formal parameter


#include "Selling.h"

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <climits>
#include <utility>

#include "S6Dist.h"

bool Selling::m_debugInstrument = false;

std::vector< S6(*)(const S6&)> FillReduceMatrixVector() {
   static std::vector< S6(*)(const S6&)> vf;
   if (vf.empty()) {
      // THESE ARE TRANSFORMATIONS IN S6 (I THINK), NOT NOT NOT IN G6 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      vf.push_back(&S6Dist::Reduce11);  // g or p
      vf.push_back(&S6Dist::Reduce21);  // h or q
      vf.push_back(&S6Dist::Reduce31);  // k or r
      vf.push_back(&S6Dist::Reduce41);  // l or s
      vf.push_back(&S6Dist::Reduce51);  // m or t
      vf.push_back(&S6Dist::Reduce61);  // n or u
   }
   return vf;
}

size_t CountPositive(const S6& s6) {
   size_t sum = 0;
   for (size_t i = 0; i < 6; ++i) sum += (s6[i] > 0.0) ? 1 : 0;
   return sum;
}

bool Selling::Reduce(const S6& in, S6& out) {
   static std::vector< S6(*)(const S6&)> vf = FillReduceMatrixVector();
   size_t maxIndex = INT_MAX;
   int cycle = 0;
   out = in;
   long reductionCycleCount = 0;
   while (true) {
      double maxScalar = -DBL_MAX;

      for (size_t i = 0; i < 6; ++i) {
         if (out[i] > maxScalar) {
            maxIndex = i;
            maxScalar = out[i];
         }
      }
      if (maxScalar <= 0.0) {
         return true;
      }

      out = vf[maxIndex](out);
      if (CountPositive(out) == 0) return true;

      ++cycle;
      if (reductionCycleCount > 1000)
      {
         return false;
      }
      if (maxScalar > 0.0) ++reductionCycleCount;
   }
}

   double Selling::MaxScalar(const S6& s6) {
      const std::vector<double>& v(s6.GetVector());
      return *std::max_element(v.begin(), v.end());
   }

   bool Selling::IsReduced(const S6& s6) {
      return (MaxScalar(s6) <= 0.0);
   }

   bool Selling::IsReduced(const S6& s6, const double delta) {
      return (MaxScalar(s6) <= delta);
   }
