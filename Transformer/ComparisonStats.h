#ifndef COMPARISON_STATS_H
#define COMPARISON_STATS_H

#include "UnifiedMatchResult.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>

class ComparisonStats {
public:
   struct AlgorithmStats {
      std::string name;
      int successCount = 0;
      int totalCount = 0;
      double totalP3Distance = 0.0;
      double totalNCDistance = 0.0;
      double totalIterations = 0.0;
      double minP3 = std::numeric_limits<double>::max();
      double maxP3 = 0.0;
      double minNC = std::numeric_limits<double>::max();
      double maxNC = 0.0;
      std::vector<double> p3Distances;
      std::vector<double> ncDistances;
      std::vector<int> iterations;

      double getSuccessRate() const {
         return totalCount > 0 ? (double)successCount / totalCount * 100.0 : 0.0;
      }

      double getAverageP3() const {
         return successCount > 0 ? totalP3Distance / successCount : 0.0;
      }

      double getAverageNC() const {
         return successCount > 0 ? totalNCDistance / successCount : 0.0;
      }

      double getAverageIterations() const {
         return successCount > 0 ? totalIterations / successCount : 0.0;
      }

      double getMedianP3() const {
         if (p3Distances.empty()) return 0.0;
         std::vector<double> sorted = p3Distances;
         std::sort(sorted.begin(), sorted.end());
         size_t n = sorted.size();
         return n % 2 == 0 ? (sorted[n / 2 - 1] + sorted[n / 2]) / 2.0 : sorted[n / 2];
      }

      double getStdDevP3() const {
         if (p3Distances.size() <= 1) return 0.0;
         double mean = getAverageP3();
         double variance = 0.0;
         for (double p3 : p3Distances) {
            variance += (p3 - mean) * (p3 - mean);
         }
         return std::sqrt(variance / (p3Distances.size() - 1));
      }
   };

   struct DetailedResult {
      double p3Distance;
      double ncDistance;
      std::string algorithm;
      bool success;
   };

private:
   std::map<std::string, AlgorithmStats> algorithmStats;
   std::vector<DetailedResult> detailedResults;
   int totalComparisons = 0;
   int totalSuccesses = 0;
   std::string referenceCell;

public:
   ComparisonStats() = default;

   void setReferenceCell(const std::string& ref) {
      referenceCell = ref;
   }

   void addResult(const UnifiedMatchResult& result) {
      totalComparisons++;

      // Store detailed result for individual analysis
      DetailedResult detailed;
      detailed.p3Distance = result.p3Distance;
      detailed.ncDistance = result.ncDistance;
      detailed.algorithm = result.algorithmUsed;
      detailed.success = result.success;
      detailedResults.push_back(detailed);

      if (result.success) {
         totalSuccesses++;

         // Get or create algorithm stats
         AlgorithmStats& stats = algorithmStats[result.algorithmUsed];
         stats.name = result.algorithmUsed;
         stats.successCount++;
         stats.totalP3Distance += result.p3Distance;
         stats.totalNCDistance += result.ncDistance;
         stats.totalIterations += result.iterationsUsed;

         // Update min/max
         stats.minP3 = std::min(stats.minP3, result.p3Distance);
         stats.maxP3 = std::max(stats.maxP3, result.p3Distance);
         stats.minNC = std::min(stats.minNC, result.ncDistance);
         stats.maxNC = std::max(stats.maxNC, result.ncDistance);

         // Store for median/stddev calculations
         stats.p3Distances.push_back(result.p3Distance);
         stats.ncDistances.push_back(result.ncDistance);
         stats.iterations.push_back(result.iterationsUsed);
      }

      // Count total attempts for each algorithm (for success rate)
      for (auto& pair : algorithmStats) {
         pair.second.totalCount++;
      }

      // If this is the first time seeing this algorithm, initialize total count
      if (algorithmStats.find(result.algorithmUsed) == algorithmStats.end()) {
         algorithmStats[result.algorithmUsed].totalCount = totalComparisons;
      }
   }

   void outputSummary(std::ostream& os = std::cout) const {
      os << "\n; ========================================" << std::endl;
      os << "; COMPARISON MODE SUMMARY" << std::endl;
      os << "; ========================================" << std::endl;
      os << "; Reference: " << referenceCell << std::endl;
      os << "; Total comparisons: " << totalComparisons << std::endl;
      os << "; Total successes: " << totalSuccesses << std::endl;
      os << "; Overall success rate: " << std::fixed << std::setprecision(1)
         << (totalComparisons > 0 ? (double)totalSuccesses / totalComparisons * 100.0 : 0.0) << "%" << std::endl;
      os << std::endl;

      if (algorithmStats.empty()) {
         os << "; No successful algorithms" << std::endl;
         return;
      }

      // Table header
      os << "; " << std::left << std::setw(24) << "Algorithm"
         << std::right << std::setw(8) << "Success"
         << std::setw(8) << "Rate%"
         << std::setw(10) << "Avg_P3"
         << std::setw(10) << "Med_P3"
         << std::setw(10) << "Min_P3"
         << std::setw(10) << "Max_P3"
         << std::setw(10) << "StdDev"
         << std::setw(9) << "Avg_Iter" << std::endl;

      os << "; " << std::string(97, '-') << std::endl;

      // Sort algorithms by success count (descending)
      std::vector<std::pair<std::string, AlgorithmStats>> sortedAlgorithms;
      for (const auto& pair : algorithmStats) {
         sortedAlgorithms.push_back(pair);
      }
      std::sort(sortedAlgorithms.begin(), sortedAlgorithms.end(),
         [](const auto& a, const auto& b) {
            return a.second.successCount > b.second.successCount;
         });

      // Print algorithm statistics
      for (const auto& pair : sortedAlgorithms) {
         const AlgorithmStats& stats = pair.second;
         if (stats.successCount > 0) {
            os << "; " << std::left << std::setw(24) << stats.name
               << std::right << std::setw(8) << stats.successCount
               << std::setw(8) << std::fixed << std::setprecision(1) << stats.getSuccessRate()
               << std::setw(10) << std::setprecision(3) << stats.getAverageP3()
               << std::setw(10) << stats.getMedianP3()
               << std::setw(10) << stats.minP3
               << std::setw(10) << stats.maxP3
               << std::setw(10) << stats.getStdDevP3()
               << std::setw(9) << std::setprecision(1) << stats.getAverageIterations() << std::endl;
         }
      }
   }

   void outputCSV(std::ostream& os = std::cout) const {
      // Remove the old algorithm summary CSV - it's not useful
      // This method is now empty or can be removed entirely
      os << "; Note: Algorithm summary CSV removed - use outputCompactCSVToFile() instead" << std::endl;
   }

   void outputDetailedResults(std::ostream& os = std::cout) const {
      os << "\n; DETAILED INDIVIDUAL RESULTS" << std::endl;
      os << "; Index     P3_Distance    NC_Distance    Algorithm" << std::endl;
      os << "; " << std::string(60, '-') << std::endl;

      for (size_t i = 0; i < detailedResults.size(); ++i) {
         const DetailedResult& result = detailedResults[i];
         os << "; " << std::setw(5) << (i + 1)
            << "      " << std::fixed << std::setprecision(6) << std::setw(12) << result.p3Distance
            << "   " << std::setw(12) << result.ncDistance
            << "   " << result.algorithm << std::endl;
      }
   }

   void outputCompactResults(std::ostream& os = std::cout) const {
      os << "\n; COMPACT P3/NC RESULTS" << std::endl;
      os << "NCDist P3" << std::endl;

      for (const auto& result : detailedResults) {
         os << std::fixed << std::setprecision(6)
            << result.ncDistance << " " << result.p3Distance << std::endl;
      }
   }

   void outputCompactCSVToFile(const std::string& filename = "comparison_results.csv") const {
      std::ofstream csvFile(filename);
      if (csvFile.is_open()) {
         csvFile << "NCDist,P3,Algorithm,Success" << std::endl;

         for (const auto& result : detailedResults) {
            csvFile << std::fixed << std::setprecision(6)
               << result.ncDistance << ","
               << result.p3Distance << ","
               << result.algorithm << ","
               << (result.success ? "YES" : "NO") << std::endl;
         }
         csvFile.close();
         std::cout << "; CSV written to: " << filename << std::endl;
      }
      else {
         std::cout << "; Error: Could not write CSV file " << filename << std::endl;
      }
   }

   void outputIndividualCSV(std::ostream& os = std::cout) const {
      os << "\n; INDIVIDUAL RESULTS CSV" << std::endl;
      os << "Index,P3_Distance,NC_Distance,Algorithm,Success" << std::endl;

      for (size_t i = 0; i < detailedResults.size(); ++i) {
         const DetailedResult& result = detailedResults[i];
         os << (i + 1) << ","
            << std::fixed << std::setprecision(6) << result.p3Distance << ","
            << result.ncDistance << ","
            << result.algorithm << ","
            << (result.success ? "YES" : "NO") << std::endl;
      }
   }

   // Individual algorithm access
   const AlgorithmStats* getAlgorithmStats(const std::string& algorithmName) const {
      auto it = algorithmStats.find(algorithmName);
      return it != algorithmStats.end() ? &it->second : nullptr;
   }

   // Overall statistics
   int getTotalComparisons() const { return totalComparisons; }
   int getTotalSuccesses() const { return totalSuccesses; }
   double getOverallSuccessRate() const {
      return totalComparisons > 0 ? (double)totalSuccesses / totalComparisons * 100.0 : 0.0;
   }
};

#endif // COMPARISON_STATS_H
