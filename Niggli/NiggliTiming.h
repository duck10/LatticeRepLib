#ifndef NIGGLI_TIMING_H
#define NIGGLI_TIMING_H

#include "NiggliTypes.h"
#include "NiggliReduce.h"
#include "G6.h"
#include <chrono>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <iomanip>

// ============================================================
// Statistics over a batch of runs
// ============================================================

// Threshold for "near-cycling": iterations above this are suspicious
static const int NEAR_CYCLE_THRESHOLD = 100;

struct BatchStats {
   std::string  label;
   int          nTotal = 0;
   int          nConverged = 0;
   int          nReduced = 0;   // isReduced true
   int          nNearCycle = 0;   // iterations > NEAR_CYCLE_THRESHOLD
   double       meanIter = 0.0;
   double       maxIter = 0.0;
   double       meanSteps = 0.0;
   double       meanWallUs = 0.0; // microseconds per cell
   double       maxWallUs = 0.0;
   double       meanTraceRatio = 0.0; // traceInitial / traceFinal

   void Print(std::ostream& os) const {
      os << std::left << std::setw(28) << label
         << "  conv=" << std::setw(6) << nConverged
         << "  isNig=" << std::setw(6) << nReduced
         << "  nearCyc=" << std::setw(5) << nNearCycle
         << "  iter avg=" << std::fixed << std::setprecision(1)
         << std::setw(7) << meanIter
         << "  max=" << std::setw(6) << (int)maxIter
         << "  us avg=" << std::setw(7) << std::setprecision(3) << meanWallUs
         << "  trRatio=" << std::setw(7) << std::setprecision(3) << meanTraceRatio
         << std::endl;
   }
};

// ============================================================
// Run one preset over a vector of input cells, return stats
// ============================================================

inline BatchStats RunBatch(
   const std::vector<G6>& inputs,
   const NiggliOptions& opt)
{
   BatchStats stats;
   stats.label = opt.Label();
   stats.nTotal = static_cast<int>(inputs.size());

   std::vector<double> iters, steps, wallUs, traceRatios;
   iters.reserve(inputs.size());
   steps.reserve(inputs.size());
   wallUs.reserve(inputs.size());
   traceRatios.reserve(inputs.size());

   for (const G6& g : inputs) {
      const auto t0 = std::chrono::high_resolution_clock::now();
      const NiggliResult r = NiggliReduce::Reduce(g, opt);
      const auto t1 = std::chrono::high_resolution_clock::now();

      const double us = std::chrono::duration<double, std::micro>(t1 - t0).count();

      if (r.converged) ++stats.nConverged;
      if (r.isReduced) ++stats.nReduced;

      iters.push_back(static_cast<double>(r.iterations));
      steps.push_back(static_cast<double>(r.steps));
      wallUs.push_back(us);

      const double ratio = (r.traceFinal > 0.0)
         ? r.traceInitial / r.traceFinal : 1.0;
      traceRatios.push_back(ratio);

      if (r.iterations > NEAR_CYCLE_THRESHOLD) ++stats.nNearCycle;
   }

   auto mean = [](const std::vector<double>& v) {
      return v.empty() ? 0.0
         : std::accumulate(v.begin(), v.end(), 0.0) / v.size();
      };
   auto maxv = [](const std::vector<double>& v) {
      return v.empty() ? 0.0 : *std::max_element(v.begin(), v.end());
      };

   stats.meanIter = mean(iters);
   stats.maxIter = maxv(iters);
   stats.meanSteps = mean(steps);
   stats.meanWallUs = mean(wallUs);
   stats.maxWallUs = maxv(wallUs);
   stats.meanTraceRatio = mean(traceRatios);

   return stats;
}

// ============================================================
// Run all presets over the same input set and print a table
// ============================================================

inline void RunComparison(
   const std::vector<G6>& inputs,
   const std::vector<NiggliOptions>& presets,
   std::ostream& os)
{
   os << "\n=== Niggli Reduction Comparison  n=" << inputs.size() << " ===\n";
   os << std::string(120, '-') << "\n";

   std::vector<BatchStats> allStats;
   for (const NiggliOptions& opt : presets) {
      const BatchStats s = RunBatch(inputs, opt);
      allStats.push_back(s);
      s.Print(os);
   }

   os << std::string(120, '-') << "\n";

   // Flag any preset where isReduced < nConverged
   bool anyFailures = false;
   for (const BatchStats& s : allStats) {
      if (s.nReduced < s.nConverged) {
         os << "WARNING: " << s.label
            << " produced " << (s.nConverged - s.nReduced)
            << " cells that are not Niggli-reduced\n";
         anyFailures = true;
      }
   }
   if (!anyFailures)
      os << "All presets: every converged cell passed IsNiggliReduced\n";
}

#endif // NIGGLI_TIMING_H