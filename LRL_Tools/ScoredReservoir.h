#ifndef SCORED_RESERVOIR_H
#define SCORED_RESERVOIR_H

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

template<typename ResultType>
class ScoredReservoir {
public:
   explicit ScoredReservoir(const size_t maxSizePerMode = 50)
      : m_maxSize(maxSizePerMode)
      , m_checkDuplicateKeys(false)
      , m_keyTolerance(1e-8)
      , m_title("") {
      m_floor.reserve(maxSizePerMode + 1);  // +1 for insertion buffer
      m_ceiling.reserve(maxSizePerMode + 1);
   }

   // Enable/disable duplicate key rejection with tolerance threshold
   void setCheckDuplicateKeys(bool enable, double threshold = 1e-8) {
      m_checkDuplicateKeys = enable;
      m_keyTolerance = std::abs(threshold);
   }

   bool getCheckDuplicateKeys() const { return m_checkDuplicateKeys; }
   double getKeyTolerance() const { return m_keyTolerance; }

   // Title management
   void setTitle(const std::string& t) { m_title = t; }
   std::string getTitle() const { return m_title; }

   void add(const double score, const ResultType& result) {
      // Reject invalid scores
      if (!std::isfinite(score)) {
         return;
      }

      // Check for duplicate results (exact match)
      if (contains(m_floor, result) || contains(m_ceiling, result)) {
         return;
      }

      // ISSUE 2 FIX: Check for near-duplicate keys if enabled (applied at add time)
      if (m_checkDuplicateKeys) {
         if (hasNearDuplicateKey(m_floor, score) || hasNearDuplicateKey(m_ceiling, score)) {
            return;  // Reject near-duplicate keys based on threshold
         }
      }

      // Insert into floor (minimum scores)
      insertSorted(m_floor, result, score, /*ascending=*/true);
      if (m_floor.size() > m_maxSize) {
         m_floor.pop_back();
      }

      // Insert into ceiling (maximum scores)
      insertSorted(m_ceiling, result, score, /*ascending=*/false);
      if (m_ceiling.size() > m_maxSize) {
         m_ceiling.pop_back();
      }
   }

   // Access methods
   const std::vector<std::pair<double, ResultType>>& getFloor() const {
      return m_floor;
   }

   const std::vector<std::pair<double, ResultType>>& getCeiling() const {
      return m_ceiling;
   }

   std::pair<
      const std::vector<std::pair<double, ResultType>>&,
      const std::vector<std::pair<double, ResultType>>&
   > getBoth() const {
      return { m_floor, m_ceiling };
   }

   double getFloorScore() const {
      return m_floor.empty() ? std::numeric_limits<double>::max() : m_floor.front().first;
   }

   double getCeilingScore() const {
      return m_ceiling.empty() ? std::numeric_limits<double>::lowest() : m_ceiling.front().first;
   }

   size_t sizeFloor() const { return m_floor.size(); }
   size_t sizeCeiling() const { return m_ceiling.size(); }
   size_t capacityPerMode() const { return m_maxSize; }

   bool empty() const {
      return m_floor.empty() && m_ceiling.empty();
   }

   void clear() {
      m_floor.clear();
      m_ceiling.clear();
   }

   // Optional: print function that doesn't require operator<< for ResultType
   void printScoresOnly(std::ostream& os) const {
      // ISSUE 1 FIX: Output title if present
      if (!m_title.empty()) {
         os << m_title << "\n";
      }

      os << "ScoredReservoir { floor: " << sizeFloor()
         << ", ceiling: " << sizeCeiling()
         << ", max: " << capacityPerMode() << " per mode }\n";
      os << "Floor scores: ";
      for (const auto& [score, _] : m_floor) os << score << " ";
      os << "\nCeiling scores: ";
      for (const auto& [score, _] : m_ceiling) os << score << " ";
      os << "\n";
   }

   // Friend operator<< - only compiles if ResultType has operator<<
   template<typename RT>
   friend std::ostream& operator<<(std::ostream& os, const ScoredReservoir<RT>& r);

private:
   std::vector<std::pair<double, ResultType>> m_floor;
   std::vector<std::pair<double, ResultType>> m_ceiling;
   size_t m_maxSize;
   bool m_checkDuplicateKeys;
   double m_keyTolerance;  // Threshold for rejecting near-duplicate keys
   std::string m_title;

   void insertSorted(std::vector<std::pair<double, ResultType>>& vec,
      const ResultType& result, double score, bool ascending) {
      auto pos = std::lower_bound(vec.begin(), vec.end(), score,
         [ascending](const auto& pair, double val) {
            return ascending ? pair.first < val : pair.first > val;
         });
      vec.insert(pos, std::make_pair(score, result));
   }

   bool contains(const std::vector<std::pair<double, ResultType>>& vec,
      const ResultType& result) const {
      return std::any_of(vec.begin(), vec.end(),
         [&result](const auto& pair) { return pair.second == result; });
   }

   bool hasNearDuplicateKey(const std::vector<std::pair<double, ResultType>>& vec,
      double score) const {
      if (vec.empty()) return false;

      // Check if any existing key is within fractional tolerance
      return std::any_of(vec.begin(), vec.end(),
         [this, score](const auto& pair) {
            double existing = pair.first;
            // Handle zero specially
            if (std::abs(existing) < 1e-100 && std::abs(score) < 1e-100) {
               return true;  // Both effectively zero
            }
            // Fractional difference: |a-b| / max(|a|, |b|)
            double denom = std::max(std::abs(existing), std::abs(score));
            double fractional_diff = std::abs(existing - score) / denom;
            return fractional_diff < m_keyTolerance;
         });
   }
};

// ISSUE 1 FIX: Implementation of friend operator<< - now outputs title first
template<typename RT>
std::ostream& operator<<(std::ostream& os, const ScoredReservoir<RT>& r) {
   // Output title if present
   // Output title if present
   if (!r.m_title.empty()) {
      os << r.m_title << "\n";
   }

   os << "ScoredReservoir { floor: " << r.sizeFloor()
      << ", ceiling: " << r.sizeCeiling()
      << ", max: " << r.capacityPerMode()
      << ", check_duplicates: " << (r.m_checkDuplicateKeys ? "yes" : "no");

   if (r.m_checkDuplicateKeys) {
      os << ", key_tolerance: " << r.m_keyTolerance;
   }

   os << " }\n";

   os << "Floor (lowest scores):\n";
   if (r.m_floor.empty()) {
      os << "  (empty)\n";
   } else {
      for (const auto& [score, result] : r.m_floor) {
         os << "  [" << std::fixed << std::setprecision(3) << score << "] " << result << "\n";
      }
   }

   os << "Ceiling (highest scores):\n";
   if (r.m_ceiling.empty()) {
      os << "  (empty)\n";
   } else {
      for (const auto& [score, result] : r.m_ceiling) {
         os << "  [" << std::fixed << std::setprecision(3) << score << "] " << result << "\n";
      }
   }

   return os;
}

#endif // SCORED_RESERVOIR_H

