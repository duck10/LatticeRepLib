#ifndef LATTICEMATCHRESERVOIR_H
#define LATTICEMATCHRESERVOIR_H

#include "LatticeMatchResult.h"
#include <vector>
#include <algorithm>
#include <limits>

class LatticeMatchReservoir {
public:
   // Constructor
   explicit LatticeMatchReservoir(const size_t maxSize = 50,
      const double maxAcceptableDistance = 5.0)
      : m_maxReservoirSize(maxSize)
      , m_maxAcceptableDistance(maxAcceptableDistance)
   {
      m_reservoir.reserve(maxSize);
   }

   // Try to add a result to the reservoir
   bool tryAdd(const LatticeMatchResult& result) {
      // Early rejection for bad results
      if (result.getP3Distance() > m_maxAcceptableDistance) {
         return false;
      }

      // Check for duplicates
      for (const auto& existing : m_reservoir) {
         if (result == existing) {
            return false;  // Already have this result
         }
      }

      // If reservoir not full, just add it
      if (m_reservoir.size() < m_maxReservoirSize) {
         m_reservoir.push_back(result);
         std::sort(m_reservoir.begin(), m_reservoir.end());
         return true;
      }

      // If better than worst in reservoir, replace worst
      if (result < m_reservoir.back()) {
         m_reservoir.back() = result;
         std::sort(m_reservoir.begin(), m_reservoir.end());
         return true;
      }

      return false;  // Not good enough for reservoir
   }

   // Get all results in the reservoir
   std::vector<LatticeMatchResult> getAllResults() const {
      return m_reservoir;
   }

   // Get the best N results
   std::vector<LatticeMatchResult> getBest(const size_t count) const {
      const size_t actualCount = std::min(count, m_reservoir.size());
      return std::vector<LatticeMatchResult>(m_reservoir.begin(),
         m_reservoir.begin() + actualCount);
   }

   // Accessors
   size_t size() const { return m_reservoir.size(); }
   bool empty() const { return m_reservoir.empty(); }
   double getBestDistance() const {
      return m_reservoir.empty() ? std::numeric_limits<double>::max() :
         m_reservoir[0].getP3Distance();
   }
   double getWorstDistance() const {
      return m_reservoir.empty() ? 0.0 : m_reservoir.back().getP3Distance();
   }
   size_t capacity() const { return m_maxReservoirSize; }

   // Clear the reservoir
   void clear() { m_reservoir.clear(); }

   // Debug information
   void printStats(std::ostream& os) const {
      os << "; Reservoir: " << size() << "/" << capacity() << " results";
      if (!empty()) {
         os << ", best distance: " << getBestDistance() << " Å";
         if (size() > 1) {
            os << ", worst: " << getWorstDistance() << " Å";
         }
      }
      os << "\n";
   }

   // Output operator
   friend std::ostream& operator<<(std::ostream& os, const LatticeMatchReservoir& reservoir) {
      os << "; LatticeMatchReservoir\n";
      reservoir.printStats(os);
      for (size_t i = 0; i < reservoir.m_reservoir.size(); ++i) {
         os << "; Result " << i << ":\n" << reservoir.m_reservoir[i] << "\n";
      }
      return os;
   }

private:
   std::vector<LatticeMatchResult> m_reservoir;
   size_t m_maxReservoirSize;
   double m_maxAcceptableDistance;
};

#endif // LATTICEMATCHRESERVOIR_H
#pragma once
