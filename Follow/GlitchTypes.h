#ifndef GLITCH_TYPES_H
#define GLITCH_TYPES_H
#include <cstddef>
#include "S6.h"

struct S6Pair {
   S6 first;
   S6 second;
   S6Pair(const S6& s1 = S6(), const S6& s2 = S6()) : first(s1), second(s2) {}
   friend std::ostream& operator<<(std::ostream& os, const S6Pair& pair) {
      os << pair.first << " | " << pair.second;
      return os;
   }
};

struct Glitch {
   friend std::ostream& operator<<(std::ostream& os, const Glitch& glitch) {
      os << "; glitch" << std::endl
         << "; index " << glitch.index << std::endl
         << "; percent change " << glitch.changePercent << std::endl
         << ";  before pt S6 " << glitch.beforePoint << std::endl
         << ";  pt at     S6 " << glitch.atPoint << std::endl
         << ";  pt after  S6 " << glitch.afterPoint << std::endl
         << "; distance type " << glitch.distanceType << std::endl
         << "; value " << glitch.value << std::endl;
      return os;
   }

   size_t index;
   double value;          // Distance value at this point
   double changePercent;  // Percent change in distance
   S6Pair beforePoint;
   S6Pair atPoint;
   S6Pair afterPoint;
   std::string distanceType;

   Glitch(size_t idx, double val, double chg,
      const S6Pair& before, const S6Pair& at, const S6Pair& after,
      const std::string& distType)
      : index(idx), value(val), changePercent(chg),
      beforePoint(before), atPoint(at), afterPoint(after),
      distanceType(distType) {}

   static std::vector<Glitch> DetectGlitches(const std::vector<double>& distances,
      double threshold, size_t window);
};

#endif // GLITCH_TYPES_H

