#ifndef UNIFIED_RESERVOIR_H
#define UNIFIED_RESERVOIR_H

#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

#include "Matrix_3x3.h"

// Template reservoir that works with any type that has:
// - operator< for sorting (quality comparison)
// - getP3Distance() method for quality metrics
// - getTransformationMatrix() returning Matrix_3x3
//
// Deduplication is by matrix norm: a candidate is rejected if any existing
// entry has (candidate.matrix - existing.matrix).norm() < 1e-4.
// The P3 distance check is applied first so that the norm scan only runs
// for candidates that could actually enter the reservoir.

template<typename ResultType>
class UnifiedReservoir {
public:
   explicit UnifiedReservoir(size_t maxSize = 100)
      : m_maxReservoirSize(maxSize) {
      m_reservoir.reserve(maxSize);
   }

   ResultType& operator[](size_t n) { return m_reservoir[n]; }
   const ResultType& operator[](size_t n) const { return m_reservoir[n]; }

   void add(const ResultType& result) {
      // Cheap rejection: if the reservoir is full and this result is no
      // better than the current worst, discard immediately -- no norm
      // computation needed.
      if (m_reservoir.size() == m_maxReservoirSize &&
         !(result < m_reservoir.back())) return;

      // Duplicate check: reject if an entry with the same matrix exists.
      const Matrix_3x3& newMatrix = result.getTransformationMatrix();
      for (const auto& existing : m_reservoir) {
         if ((newMatrix - existing.getTransformationMatrix()).norm() < 1e-4)
            return;
      }

      if (m_reservoir.size() < m_maxReservoirSize) {
         auto pos = std::lower_bound(m_reservoir.begin(), m_reservoir.end(), result);
         m_reservoir.insert(pos, result);
      } else {
         m_reservoir.pop_back();
         auto pos = std::lower_bound(m_reservoir.begin(), m_reservoir.end(), result);
         m_reservoir.insert(pos, result);
      }
   }

   // Accessors
   std::vector<ResultType> getAllResults() const { return m_reservoir; }
   std::vector<ResultType> getAll()        const { return m_reservoir; }

   std::vector<ResultType> getBest(size_t count) const {
      const size_t n = std::min(count, m_reservoir.size());
      return std::vector<ResultType>(m_reservoir.begin(), m_reservoir.begin() + n);
   }

   std::vector<ResultType> getWithinThreshold(double maxDistance) const {
      std::vector<ResultType> results;
      for (const auto& result : m_reservoir) {
         if (result.getP3Distance() <= maxDistance)
            results.push_back(result);
         else
            break;  // reservoir is sorted
      }
      return results;
   }

   size_t size()     const { return m_reservoir.size(); }
   bool   empty()    const { return m_reservoir.empty(); }
   size_t capacity() const { return m_maxReservoirSize; }
   void   clear() { m_reservoir.clear(); }

   double getBestDistance() const {
      return m_reservoir.empty()
         ? std::numeric_limits<double>::max()
         : m_reservoir.front().getP3Distance();
   }

   double getWorstDistance() const {
      return m_reservoir.empty() ? 0.0 : m_reservoir.back().getP3Distance();
   }

   void printStats(std::ostream& os) const {
      os << "; Reservoir: " << size() << "/" << capacity() << " results";
      if (!empty()) {
         os << ", best: " << getBestDistance();
         if (size() > 1)
            os << ", worst: " << getWorstDistance();
      }
      os << std::endl;
   }

   friend std::ostream& operator<<(std::ostream& os, const UnifiedReservoir& r) {
      r.printStats(os);
      for (size_t i = 0; i < r.m_reservoir.size(); ++i)
         os << "; Result " << i << ":\n" << r.m_reservoir[i] << "\n";
      return os;
   }

private:
   std::vector<ResultType> m_reservoir;
   size_t                  m_maxReservoirSize;
};

// Type alias for backward compatibility
#include "LatticeMatchResult.h"
using LatticeMatchReservoir = UnifiedReservoir<LatticeMatchResult>;

#endif // UNIFIED_RESERVOIR_H
