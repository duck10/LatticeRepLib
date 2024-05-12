#ifndef PLOTS_H
#define PLOTS_H

#include <algorithm>
#include <vector>

#include "LRL_ReadLatticeData.h"
#include "Polar.h"

template<typename T>
static std::pair<double, double> GetMinMaxPlot(const std::vector<T>& v) {
   double minv = DBL_MAX;
   double maxv = -DBL_MAX;

   for (const auto& vv : v) {
      minv = minNC(minv, Min(vv));
      maxv = maxNC(maxv, Max(vv));
   }
   return { minv,maxv };
}

double Min(const S6& s);
double Min(const Polar& p);

double Max(const S6& s);
double Max(const Polar& p);

template<typename T>
std::vector<T> ConvertInput(const std::vector<LRL_ReadLatticeData>& inputList) {
   std::vector<T> v;

   for (const auto& input : inputList) {
      v.emplace_back(T(input.GetCell()));
   }
   return v;
}


#endif  // PLOTS_H