#ifndef GLITCHDETECTOR_H
#define GLITCHDETECTOR_H

#include <algorithm>
#include <numeric>
#include <vector>

#include "ControlVariables.h"
#include "OutlierFinder.h"

class GlitchDetector {
public:
   struct Glitch {
      size_t index;
      double value;
      double changePercent;
      Glitch(size_t idx, double val, double chg) : index(idx), value(val), changePercent(chg) {}
   };

   std::vector<Glitch> detectGlitches(const std::vector<double>& data,
      double thresholdPercent = 10.0,
      int minSeparation = 5) {
      std::vector<Glitch> glitches;
      if (data.size() < 2) return glitches;

      OutlierFinder of(data);

      const double minPercentDiff = 1.0;
      const std::vector<std::pair<double,double>> shifts = of.FindDiscontinuities(thresholdPercent);
      for (const auto& sh : shifts)
      {
         const Glitch glitch(int(sh.first), 0.0, sh.second);
         glitches.emplace_back(glitch);
      }

      return glitches;
   }
};

#endif // GLITCHDETECTOR_H