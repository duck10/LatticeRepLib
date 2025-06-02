#ifndef TRANSFORMATION_RESERVOIR_H
#define TRANSFORMATION_RESERVOIR_H

#include "Matrix_3x3.h"
#include "LRL_Cell.h"
#include "TransformerUtilities.h"
#include "S6.h"
#include <vector>
#include <algorithm>
#include <limits>

class TransformationReservoir {
public:
   struct TransformationCandidate {
      Matrix_3x3 matrix;
      double p3Distance;
      double ncDistance;
      double s6Angle;
      LRL_Cell transformedCell;

      // Comparison for sorting by quality (lower distance = better)
      bool operator<(const TransformationCandidate& other) const {
         return p3Distance < other.p3Distance;
      }

      // Equality check for avoiding duplicates
      bool operator==(const TransformationCandidate& other) const {
         return std::abs(p3Distance - other.p3Distance) < 1e-10 &&
            matrix == other.matrix;
      }
   };

private:
   std::vector<TransformationCandidate> reservoir;
   size_t maxReservoirSize;
   double maxAcceptableDistance;

public:
   TransformationReservoir(size_t reservoirSize = 50, double maxDist = 5.0)
      : maxReservoirSize(reservoirSize), maxAcceptableDistance(maxDist) {
      reservoir.reserve(reservoirSize);
   }

   // Try to add a transformation to the reservoir
   bool tryAdd(const Matrix_3x3& matrix, const LRL_Cell& sourceCell, const LRL_Cell& targetCell) {
      // Apply transformation
      LRL_Cell transformed = matrix * sourceCell;

      // Calculate quality metrics
      double p3Dist = TransformerUtilities::getP3Distance(transformed, targetCell);
      double ncDist = CalculateNCDistWithReduction(transformed, targetCell);
      double s6Angle = TransformerUtilities::angleS6(S6(transformed), S6(targetCell));

      // Early rejection for obviously bad transformations
      if (p3Dist > maxAcceptableDistance) {
         return false;
      }

      TransformationCandidate candidate{ matrix, p3Dist, ncDist, s6Angle, transformed };

      // Check for duplicates (avoid adding essentially identical transformations)
      for (const auto& existing : reservoir) {
         if (candidate == existing) {
            return false;  // Already have this transformation
         }
      }

      // If reservoir not full, just add it
      if (reservoir.size() < maxReservoirSize) {
         reservoir.push_back(candidate);
         std::sort(reservoir.begin(), reservoir.end());  // Keep sorted by quality
         return true;
      }

      // If better than worst in reservoir, replace worst
      if (candidate < reservoir.back()) {
         reservoir.back() = candidate;
         std::sort(reservoir.begin(), reservoir.end());  // Re-sort
         return true;
      }

      return false;  // Not good enough for reservoir
   }

   // Get the best N transformations
   std::vector<TransformationCandidate> getBest(size_t count) const {
      size_t actualCount = std::min(count, reservoir.size());
      return std::vector<TransformationCandidate>(reservoir.begin(), reservoir.begin() + actualCount);
   }

   // Accessors
   size_t size() const { return reservoir.size(); }
   bool empty() const { return reservoir.empty(); }
   double getBestDistance() const {
      return reservoir.empty() ? std::numeric_limits<double>::max() : reservoir[0].p3Distance;
   }
   double getWorstDistance() const {
      return reservoir.empty() ? 0.0 : reservoir.back().p3Distance;
   }
   size_t capacity() const { return maxReservoirSize; }

   // Debug information
   void printStats(std::ostream& os) const {
      os << "Reservoir: " << size() << "/" << capacity() << " candidates";
      if (!empty()) {
         os << ", best distance: " << getBestDistance() << " Å";
         if (size() > 1) {
            os << ", worst: " << getWorstDistance() << " Å";
         }
      }
      os << std::endl;
   }
};

#endif // TRANSFORMATION_RESERVOIR_H

