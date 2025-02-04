#include "GlitchTypes.h"
#include "OutlierFinder.h"

#include <vector>

std::vector<Glitch> Glitch::DetectGlitches(const std::vector<double>& data,
   double threshold, size_t window) {
   std::vector<Glitch> glitches;
   OutlierFinder finder(data);
   auto shifts = finder.FindDiscontinuities(threshold);

   for (const auto& sh : shifts) {
      const size_t idx = static_cast<size_t>(sh.first);
      if (idx < data.size()) {
         Glitch glitch(idx, data[idx], sh.second,
            S6Pair(), S6Pair(), S6Pair(), "");
         glitches.emplace_back(glitch);
      }
   }
   return glitches;
}

