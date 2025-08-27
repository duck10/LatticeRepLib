#ifndef UNIFIED_RESERVOIR_H
#define UNIFIED_RESERVOIR_H

#include <vector>
#include <algorithm>
#include <limits>
#include <iostream>

// Template reservoir that works with any type that has:
// - operator< for sorting (quality comparison)
// - operator== for duplicate detection  
// - getP3Distance() method for quality metrics
template<typename ResultType>
class UnifiedReservoir {
private:
   std::vector<ResultType> m_reservoir;
   size_t m_maxReservoirSize;

public:
   // Constructor - no distance filtering, that's the caller's job
   explicit UnifiedReservoir(size_t maxSize = 500)
      : m_maxReservoirSize(maxSize) {
      m_reservoir.reserve(maxSize);
   }
   ResultType& operator[] (const size_t n) { return m_reservoir[n]; }
   ResultType operator[]  (const size_t n) const { return m_reservoir[n]; }


   //void add_simple(const ResultType& result) {
   //   std::cout << "Adding result: " << result.getP3Distance() << std::endl;

   //   // Always add the result
   //   m_reservoir.push_back(result);

   //   // Sort the entire reservoir
   //   std::sort(m_reservoir.begin(), m_reservoir.end());

   //   // Keep only top 3
   //   if (m_reservoir.size() > 3) {
   //      std::cout << "Removing worst: " << m_reservoir.back().getP3Distance() << std::endl;
   //      m_reservoir.pop_back();
   //   }

   //   // Print current state
   //   std::cout << "Top 3: ";
   //   for (const auto& r : m_reservoir) {
   //      std::cout << r.getP3Distance() << " ";
   //   }
   //   std::cout << std::endl;
   //}

   // Add a result to the reservoir - ALWAYS accepts, no filtering
   void add(const ResultType& result) {
      // Check for true duplicates (identical matrices only)
      //for (const auto& existing : m_reservoir) {
      //   if (result == existing) {
      //      return;  // Already have this exact result
      //   }
      //}
      //if (result.getP3Distance() < 50.0) {  // Only log potentially good results
      //   std::cout << "DEBUG: Adding good result: " << result.getP3Distance() << std::endl;
      //}
      if (m_reservoir.size() < m_maxReservoirSize) {
         // Reservoir not full - insert in sorted order
         auto insert_pos = std::lower_bound(m_reservoir.begin(), m_reservoir.end(), result);
         m_reservoir.insert(insert_pos, result);
      }
      else {
         // Reservoir is full - check if better than worst (last element)
         if (result < m_reservoir.back()) {
            // Insert new result in correct sorted position
            auto insert_pos = std::lower_bound(m_reservoir.begin(), m_reservoir.end(), result);
            m_reservoir.insert(insert_pos, result);

            // Remove worst element
            m_reservoir.pop_back();
         }
      }
   }


   // Get all results, sorted by quality
   std::vector<ResultType> getAllResults() const {
      return m_reservoir;  // Already sorted
   }

   // Get all results (alternative name for compatibility)
   std::vector<ResultType> getAll() const {
      return m_reservoir;
   }

   // Get the best N results
   std::vector<ResultType> getBest(size_t count) const {
      size_t actualCount = std::min(count, m_reservoir.size());
      return std::vector<ResultType>(m_reservoir.begin(), m_reservoir.begin() + actualCount);
   }

   // Get results that meet a quality threshold
   std::vector<ResultType> getWithinThreshold(double maxDistance) const {
      std::vector<ResultType> results;
      for (const auto& result : m_reservoir) {
         if (result.getP3Distance() <= maxDistance) {
            results.push_back(result);
         }
         else {
            break;  // Sorted, so we can stop here
         }
      }
      return results;
   }

   // Accessors
   size_t size() const { return m_reservoir.size(); }
   bool empty() const { return m_reservoir.empty(); }

   double getBestDistance() const {
      return m_reservoir.empty() ? std::numeric_limits<double>::max() :
         m_reservoir[0].getP3Distance();
   }

   double getWorstDistance() const {
      return m_reservoir.empty() ? 0.0 :
         m_reservoir.back().getP3Distance();
   }

   size_t capacity() const { return m_maxReservoirSize; }

   // Clear the reservoir
   void clear() { m_reservoir.clear(); }

   // Debug information
   void printStats(std::ostream& os) const {
      os << "Reservoir: " << size() << "/" << capacity() << " results";
      if (!empty()) {
         os << ", best distance: " << getBestDistance() << " Å";
         if (size() > 1) {
            os << ", worst: " << getWorstDistance() << " Å";
         }
      }
      os << std::endl;
   }

   // Output operator
   friend std::ostream& operator<<(std::ostream& os, const UnifiedReservoir& reservoir) {
      os << "UnifiedReservoir" << std::endl;
      reservoir.printStats(os);
      for (size_t i = 0; i < reservoir.m_reservoir.size(); ++i) {
         os << "Result " << i << ":" << std::endl << reservoir.m_reservoir[i] << std::endl;
      }
      return os;
   }
};

// Type aliases for backward compatibility
#include "LatticeMatchResult.h"
using LatticeMatchReservoir = UnifiedReservoir<LatticeMatchResult>;

// For TransformationCandidate if you need it
// using TransformationReservoir = UnifiedReservoir<TransformationCandidate>;

#endif // UNIFIED_RESERVOIR_H
