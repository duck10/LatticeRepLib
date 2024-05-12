#include "Plots.h"

#include <cmath>

double Min(const S6& s)
{
   double out = DBL_MAX;;
   for (size_t i = 0; i < 6; ++i) {
      out = std::min(out, s[i]);
   }
   return out;
}
double Max(const S6& s)
{
   double out = -DBL_MAX;;
   for (size_t i = 0; i < 6; ++i) {
      out = std::max(out, s[i]);
   }
   return out;
}

double Min(const Polar& p)
{
   double out = DBL_MAX;;
   for (size_t i = 0; i < 3; ++i) {
      out = std::min(out,std::min(p[i][0],p[i][1]));
   }
   return out;
}
double Max(const Polar& p)
{
   double out = -DBL_MAX;;
   for (size_t i = 0; i < 3; ++i) {
      out = std::max(out, std::max(p[i][0], p[i][1]));
   }
   return out;
}
