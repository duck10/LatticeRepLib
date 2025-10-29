#ifndef TEST_TO_CONVERGENCE_H
#define TEST_TO_CONVERGENCE_H

#include <tuple>
#include <vector>

#include "LRL_Cell.h"

static std::tuple<double, G6, int> TestToConvergence(const LRL_Cell& cell, const int nmax,
   const std::vector<Matrix_3x3>& matrices, const double tolerance) {

   G6 best = cell;
   double minTrace = GetTrace(best);
   int iterations = 0;

   for (int loop = 0; loop < nmax; ++loop) {
      ++iterations;
      G6 candidateBest = best;
      double candidateMinTrace = minTrace;

      for (const auto& m : matrices) {
         const G6 transformed = m * best;
         if (!transformed.IsValid()) continue;
         const double trace = GetTrace(transformed);

         if (trace < candidateMinTrace) {
            candidateMinTrace = trace;
            candidateBest = transformed;
         }
      }

      if (std::abs(candidateMinTrace - minTrace) < tolerance) break;

      minTrace = candidateMinTrace;
      best = candidateBest;
   }

   return { minTrace, best, iterations };
}

#endif // TEST_TO_CONVERGENCE_H