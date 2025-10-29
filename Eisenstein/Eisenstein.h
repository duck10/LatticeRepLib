#include <algorithm>
#include <vector>
#include "LRL_Cell.h"
#include "G6.h"
#include "B4.h"
#include "EdgeTransformationMatrices.h"

static const std::vector<Matrix_3x3> edgeMatrices = GetSimpleEdgeTransforms();;
static const std::vector<Matrix_3x3> allMatrices = GetSimpleEdgeTransforms();;

inline LRL_Cell operator*(const Matrix_3x3& matrix, const LRL_Cell& cell) {
   // Convert cell to B4 representation
   B4 b4Cell(cell);

   // Apply transformation in B4 space
   B4 transformedB4 = matrix * b4Cell;

   // Convert back to LRL_Cell
   return LRL_Cell(transformedB4);
}

inline std::pair<double, G6> ReduceToConvergence(const LRL_Cell& cell, const int nmax,
   const std::vector<Matrix_3x3>& matrices, const double tolerance) {

   G6 current = cell;
   G6 best = current;
   double minTrace = current[0] + current[1] + current[2];

   for (int loop = 0; loop < nmax; ++loop) {
      G6 candidateBest = best;
      double candidateMinTrace = minTrace;

      for (const auto& m : matrices) {
         const G6 transformed = m * best;
         const double trace = transformed[0] + transformed[1] + transformed[2];

         if (trace < candidateMinTrace) {
            candidateMinTrace = trace;
            candidateBest = transformed;
         }
      }

      if (std::abs(candidateMinTrace - minTrace) < tolerance) break;

      minTrace = candidateMinTrace;
      best = candidateBest;
   }

   return { minTrace, best };
}

inline std::pair<double, G6> Eisenstein(const LRL_Cell& cell, const int nmax,
   const double tolerance) {
   return ReduceToConvergence(cell, nmax, edgeMatrices, tolerance);
}

inline std::pair<double, G6> Eisenstein24(const LRL_Cell& cell, const int nmax,
   const double tolerance) {
   return ReduceToConvergence(cell, nmax, edgeMatrices, tolerance);
}

inline G6 Reduce(const LRL_Cell& cell, const int nmax=100,
   const double tolerance=1.0E-8) {
   return ReduceToConvergence(cell, nmax, edgeMatrices, tolerance).second;
}
