#ifndef GLITCHDETECTOR_H
#define GLITCHDETECTOR_H

#include <algorithm>
#include <numeric>
#include <vector>

#include "GlitchTypes.h"
#include "OutlierFinder.h"

class GlitchDetector {
public:
   std::vector<Glitch> detectGlitches(const std::vector<double>& data,
      double thresholdPercent = 10.0,
      int minSeparation = 5) {
      std::vector<Glitch> glitches;
      if (data.size() < 2) return glitches;

      OutlierFinder of(data);

      const double minPercentDiff = 1.0;
      const std::vector<std::pair<double, double>> shifts = of.FindDiscontinuities(thresholdPercent);
      for (const auto& sh : shifts) {
         Glitch glitch(static_cast<size_t>(sh.first), data[static_cast<size_t>(sh.first)],
            sh.second, S6(), S6(), S6(), 0);  // Temporary S6 objects
         glitches.emplace_back(glitch);
      }


      return glitches;
   }
};

#endif // GLITCHDETECTOR_H
