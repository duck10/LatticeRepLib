#include "spline_under_tension.hpp"
#include <algorithm>
#include <cfloat>
#include <limits>
#include <vector>

class OutlierFinder {
private:
   const std::vector<double>& m_data;
   spline_under_tension m_sut;
   std::vector<double> m_intrp;

public:
   explicit OutlierFinder(const std::vector<double>& data)
      : m_data(data)
      , m_sut(spline_under_tension(0.1, data, 3))
   {
      // Initialize interpolated values
      for (size_t i = 0; i < data.size(); ++i) {
         m_intrp.push_back(GetValue(static_cast<double>(i)));
      }
   }

   std::vector<std::pair<double, double>> FindDiscontinuities(const double cutoff) {
      const std::vector<std::pair<double, double>> candidates = SelectCandidateTransitionPoints(cutoff);
      const std::vector<std::pair<double, double>> separated = InsertSeparatorsBetweenRuns(candidates);
      return FilterForPeakOfRun(separated);
   }

   double GetValue(const double d) {
      return static_cast<double>(m_sut.getInterpValue(static_cast<float>(d)));
   }

   ~OutlierFinder() = default;

private:
   std::vector<std::pair<double, double>> SelectCandidateTransitionPoints(const double cutoff) {
      std::vector<std::pair<double, double>> candidates;
      double prevDataDiff = 0.0;

      for (size_t i = 0; i < m_data.size() - 1; ++i) {
         // Skip the first and last few points
         if (i <= 3 || i >= m_data.size() - 4) {
            prevDataDiff = 0.0;
            continue;
         }

         const double& current = m_data[i];
         const double& next = m_data[i + 1];

         // Calculate percentage difference between adjacent points
         const double absDataDiff = 100.0 * std::abs(current - next) / std::max(current, next);

         if (absDataDiff > cutoff) {
            const double avg = (current + next) / 2.0;
            const double interpValue = std::abs(GetValue(static_cast<double>(i) + 0.5));
            const double diffFromInterp = std::abs(avg - interpValue);
            const double normalizedPercent = 100.0 * diffFromInterp / interpValue;

            // Check if the difference is significant and larger than the previous difference
            if (normalizedPercent > cutoff && absDataDiff > 1.1 * prevDataDiff) {
               candidates.emplace_back(static_cast<double>(i), normalizedPercent);
            }
         }
         prevDataDiff = absDataDiff;
      }
      return candidates;
   }

   std::vector<std::pair<double, double>> InsertSeparatorsBetweenRuns(
      const std::vector<std::pair<double, double>>& candidates) const {
      std::vector<std::pair<double, double>> separated;
      if (candidates.size() <= 1) return candidates;
      constexpr double limit = -std::numeric_limits<double>::max();
      constexpr std::pair<double, double> separator(limit, limit);

      auto it = candidates.begin();
      for (const auto& candidate : candidates) {
         separated.push_back(candidate);
         const auto next = std::next(it);
         if (next == candidates.end()) break;

         // Insert separator if points are not adjacent
         if (candidate.first + 1 != next->first) {
            separated.push_back(separator);
         }
         ++it;
      }
      return separated;
   }

   std::vector<std::pair<double, double>> FilterForPeakOfRun(
      const std::vector<std::pair<double, double>>& separated) const {
      std::vector<std::pair<double, double>> filtered;
      std::vector<std::pair<double, double>> currentRun;

      for (const auto& point : separated) {
         if (point.first == -DBL_MAX) {
            // End of a run, process it
            if (!currentRun.empty()) {
               const auto peak = std::max_element(currentRun.begin(), currentRun.end(),
                  [](const auto& a, const auto& b) { return a.second < b.second; });
               filtered.push_back(*peak);
               currentRun.clear();
            }
         }
         else {
            currentRun.push_back(point);
         }
      }

      // Process the last run if it exists
      if (!currentRun.empty()) {
         auto peak = std::max_element(currentRun.begin(), currentRun.end(),
            [](const auto& a, const auto& b) { return a.second < b.second; });
         filtered.push_back(*peak);
      }

      return filtered;
   }
};
