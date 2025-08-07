#ifndef P3DIST_H
#define P3DIST_H

#include "P3.h"
#include "P3_Reduce.h"
#include "Matrix_3x3.h"
#include "LRL_Cell.h"
#include <limits>
#include <vector>
#include <algorithm>
#include <cmath>
#include "P3Utilities.h"

// External linkage assumed
extern std::vector<Matrix_3x3> GetReductionBasisWithNiggli();
//extern LRL_Cell ApplyMatrixToCell(const LRL_Cell&, const Matrix_3x3&);

class P3Dist {
public:
   // Unified entry point: reduces, permutes, repairs
   static double Distance(const P3& a, const P3& b) {
      if (!a.GetValid() || !b.GetValid()) {
         return 19191.0;
      }
      const P3 r1 = P3_Reduce::Reduce(a);
      const P3 r2 = P3_Reduce::Reduce(b);
      const double delta = DistanceBetween(r1, r2);

      if (ShouldTriggerEdgeRepair(r1, r2, delta))
         return MatrixInvariantMinimum(r1, r2);
      else
         return PermutedMinimum(r1, r2);
   }

private:
   static bool ShouldTriggerEdgeRepair(const P3& a, const P3& b, const double delta) {
      const double smallMag = 0.05;
      const double triggerDelta = 0.5;

      for (int i = 0; i < 3; ++i) {
         const double normA = std::sqrt(a[i].first * a[i].first + a[i].second * a[i].second);
         const double normB = std::sqrt(b[i].first * b[i].first + b[i].second * b[i].second);
         if (normA < smallMag || normB < smallMag) return true;
      }
      return delta > triggerDelta;
   }

   static double PermutedMinimum(const P3& a, const P3& b) {
      const std::vector<P3> permutations = generatePermutations(b);
      double minDist = std::numeric_limits<double>::max();
      for (const auto& permuted : permutations) {
         const double d = DistanceBetween(a, permuted);
         if (d < minDist) minDist = d;
      }
      return minDist;
   }

   static double MatrixInvariantMinimum(const P3& a, const P3& b) {
      const std::vector<Matrix_3x3> basis = GetReductionBasisWithNiggli();
      double minDist = std::numeric_limits<double>::max();

      for (const auto& M : basis) {
         const LRL_Cell transformed = ApplyMatrixToCell(LRL_Cell(b), M);
         const P3 transformedP3 = P3_Reduce::Reduce(P3(transformed));
         const double d = PermutedMinimum(a, transformedP3);
         if (d < minDist) minDist = d;
      }
      return minDist;
   }

   static std::vector<P3> generatePermutations(const P3& p) {
      const std::array<P3::Vec2, 3> v = p.GetVector();
      const std::vector<std::vector<int>> orders = {
         {0,1,2}, {0,2,1}, {1,0,2}, {1,2,0}, {2,0,1}, {2,1,0}
      };

      std::vector<P3> output;
      for (const auto& idx : orders) {
         output.emplace_back(P3(
            Vector_2(v[idx[0]].first, v[idx[0]].second),
            Vector_2(v[idx[1]].first, v[idx[1]].second),
            Vector_2(v[idx[2]].first, v[idx[2]].second)));
      }
      return output;
   }

   static double DistanceBetween(const P3& a, const P3& b) {
      return (a - b).norm();
   }
};

#endif // P3DIST_H
