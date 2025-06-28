#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <map>
#include <set>
#include <sstream>

#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h" 
#include "LRL_StringTools.h"
#include "MultiTransformFinderControls.h"
#include "P3RelativeThresholds .h"
#include "QualityAssessment.h"
#include "TransformerUtilities.h"
#include "LatticeCell.h"
#include "LatticeMatchResult.h"
#include "MobileComparisonResult.h"
#include "ProductionLatticeMatcherSystem.h"
#include "InputHandler.h"
#include "ProgramSetup.h"

// Forward declaration for comparison mode
void displayCompactResult(const MobileComparisonResult& result, 
   const LatticeCell& reference, 
   const QualityAssessment& quality);


// Template function for deduplicating results by transformation matrix
template<typename ResultType>
std::vector<ResultType> deduplicateByMatrix(const std::vector<ResultType>& results) {
   std::vector<ResultType> uniqueResults;
   std::set<std::string> seenMatrices;

   for (const auto& result : results) {
      // Create a string key from the matrix with high precision
      const Matrix_3x3& matrix = result.getTransformationMatrix();
      std::ostringstream key;
      key << std::fixed << std::setprecision(2);
      for (int i = 0; i < 9; ++i) {
         key << matrix[i];
         if (i < 8) key << ",";
      }

      if (seenMatrices.find(key.str()) == seenMatrices.end()) {
         seenMatrices.insert(key.str());
         uniqueResults.push_back(result);
      }
   }

   return uniqueResults;
}


// Helper function to output cell with centering symbol
void outputCellWithCentering(const LRL_Cell& cell, const std::string& centeringSymbol) {
   std::cout << centeringSymbol << " " << LRL_Cell_Degrees(cell);
}

void displayP3DistanceHistogram(const std::vector<LatticeMatchResult>& allResults,
   const MultiTransformFinderControls& controls,
   const LatticeCell& reference) {  // Need reference for adaptive thresholds
   if (allResults.empty()) {
      std::cout << "No results to display in histogram." << std::endl;
      return;
   }

   if (controls.shouldShowDetails()) {
      // Show first few descriptions to understand the source
      std::cout << "Sample result descriptions:" << std::endl;
      int samplesToShow = std::min(5, (int)allResults.size());
      for (int i = 0; i < samplesToShow; ++i) {
         std::cout << "  " << (i + 1) << ": " << allResults[i].getDescription() << " (P3: "
            << std::scientific << std::setprecision(6) << allResults[i].getP3Distance() << ")" << std::endl;
      }
      std::cout << std::endl;
   }

   // Create a sorted copy to show distribution
   std::vector<std::pair<double, int>> distanceWithIndex;
   for (size_t i = 0; i < allResults.size(); ++i) {
      distanceWithIndex.push_back({ allResults[i].getP3Distance(), static_cast<int>(i + 1) });
   }
   std::sort(distanceWithIndex.begin(), distanceWithIndex.end());

   // Find min and max for scaling
   double minDist = distanceWithIndex[0].first;
   double maxDist = distanceWithIndex.back().first;
   double range = maxDist - minDist;

   // Extract distances for adaptive threshold calculation
   std::vector<double> distances;
   for (const auto& result : allResults) {
      distances.push_back(result.getP3Distance());
   }

   // Calculate adaptive P3-relative thresholds (same as comparison mode)
   auto thresholds = P3RelativeThresholds::calculateAdaptiveP3Thresholds(
      reference.getCell(), reference.getLatticeType(), distances);

   // Display range and P3 context
   std::cout << "Range: " << std::scientific << std::setprecision(2)
      << minDist << " to " << maxDist << " Å" << std::endl;

   std::cout << "Reference P3 norm: " << std::fixed << std::setprecision(3)
      << thresholds.referenceP3Norm << " Å" << std::endl;

   // Show adaptive threshold information (consistent with comparison mode)
   std::cout << "Method: " << thresholds.method << std::endl;
   std::cout << "Thresholds:" << std::endl;
   std::cout << "  EXCELLENT <= " << std::scientific << std::setprecision(2)
      << thresholds.excellentThreshold << " ("
      << std::fixed << std::setprecision(1)
      << (thresholds.excellentThreshold / thresholds.referenceP3Norm * 100) << "% of P3)" << std::endl;
   std::cout << "  GOOD      <= " << std::scientific << std::setprecision(2)
      << thresholds.goodThreshold << " ("
      << std::fixed << std::setprecision(1)
      << (thresholds.goodThreshold / thresholds.referenceP3Norm * 100) << "% of P3)" << std::endl;
   std::cout << "  POOR      <= " << std::scientific << std::setprecision(2)
      << thresholds.poorThreshold << " ("
      << std::fixed << std::setprecision(1)
      << (thresholds.poorThreshold / thresholds.referenceP3Norm * 100) << "% of P3)" << std::endl;

   // Use excellent threshold instead of fixed success threshold for quality labels
   std::string qualityLabel;
   if (minDist <= thresholds.excellentThreshold) {
      qualityLabel = "EXCELLENT";
   }
   else if (minDist <= thresholds.goodThreshold) {
      qualityLabel = "GOOD";
   }
   else if (minDist <= thresholds.poorThreshold) {
      qualityLabel = "POOR";
   }
   else {
      qualityLabel = "TERRIBLE";
   }

   std::cout << "Scale: Each '#' represents relative P3 distance" << std::endl;
   std::cout << std::endl;

   // Display results in groups of 10 for readability
   const int groupSize = 10;
   for (size_t start = 0; start < distanceWithIndex.size(); start += groupSize) {
      size_t end = std::min(start + groupSize, distanceWithIndex.size());

      std::cout << "Results " << (start + 1) << "-" << end << " (sorted by P3 distance):" << std::endl;

      for (size_t i = start; i < end; ++i) {
         double dist = distanceWithIndex[i].first;
         int originalIndex = distanceWithIndex[i].second;

         // Create bar (scale to 50 characters max)
         int barLength = 0;
         if (range > 1e-12) {  // Avoid division by zero
            barLength = static_cast<int>(((dist - minDist) / range) * 50);
         }

         // Show result number, distance, and bar
         std::cout << std::setw(3) << (i + 1) << " (orig #" << std::setw(2) << originalIndex << "): "
            << std::scientific << std::setprecision(3) << dist << " ";

         // Print bar with adaptive quality assessment
         if (barLength == 0) {
            // For minimum values, show vertical bar with adaptive quality
            std::cout << "| " << qualityLabel;
         }
         else {
            // For non-minimum values, show proportional bars
            for (int j = 0; j < barLength; ++j) {
               std::cout << "#";
            }
         }
         std::cout << std::endl;
      }
      std::cout << std::endl;
   }
}

void displayResults(const std::vector<LatticeMatchResult>& allResults,
   const MultiTransformFinderControls& controls,
   const LatticeCell& reference) {

   if (allResults.empty()) {
      std::cout << "No results generated - this should never happen!" << std::endl;
      return;
   }

   std::cout << "Total results before processing: " << allResults.size() << std::endl;

   // Display NC Distance first
   std::cout << "NC Distance: " << allResults[0].getNcDistance() << std::endl;

   // Show P3 distance histogram for all results with adaptive thresholds
   displayP3DistanceHistogram(allResults, controls, reference);

   // Extract distances for adaptive threshold calculation
   std::vector<double> distances;
   for (const auto& result : allResults) {
      distances.push_back(result.getP3Distance());
   }

   // Calculate adaptive P3-relative thresholds (same as comparison mode)
   auto thresholds = P3RelativeThresholds::calculateAdaptiveP3Thresholds(
      reference.getCell(), reference.getLatticeType(), distances);

   // Deduplicate by transformation matrix - keep only the first occurrence of each unique matrix
   std::vector<LatticeMatchResult> uniqueResults = deduplicateByMatrix(allResults);

   if (controls.shouldShowDetails()) {
      // Show deduplication info if duplicates were found
      if (uniqueResults.size() < allResults.size()) {
         std::cout << "\nDeduplication Summary:" << std::endl;
         std::cout << "  Original results: " << allResults.size() << std::endl;
         std::cout << "  Unique matrices: " << uniqueResults.size() << std::endl;
         std::cout << "  Duplicates removed: " << (allResults.size() - uniqueResults.size()) << std::endl;
      }
      else {
         std::cout << "\nNo duplicate matrices found - all " << allResults.size() << " results have unique matrices." << std::endl;
      }
   }

   // Only show matrix groups if there are many results (>10) or if details are requested
   bool showMatrixGroups = (allResults.size() > 10) && controls.shouldShowDetails();

   if (showMatrixGroups) {
      // Group results by transformation matrix
      std::map<std::string, std::vector<LatticeMatchResult>> matrixGroups;
      for (const auto& result : allResults) {
         // Create a simple string key from the matrix
         const Matrix_3x3& matrix = result.getTransformationMatrix();
         std::ostringstream key;
         key << std::fixed << std::setprecision(3);
         for (int i = 0; i < 9; ++i) {
            key << matrix[i] << ",";
         }
         matrixGroups[key.str()].push_back(result);
      }

      // Display matrix groups (limit to 10 cells per group)
      std::cout << "Matrix Groups: " << matrixGroups.size() << " unique transformations" << std::endl;
      for (const auto& [matrixKey, results] : matrixGroups) {
         std::cout << "  Group with " << results.size() << " matches:" << std::endl;

         // Show up to 10 transformed cells from this group
         int cellsToShow = std::min(10, (int)results.size());
         for (int i = 0; i < cellsToShow; ++i) {
            std::cout << "    " << LRL_Cell_Degrees(results[i].getTransformedMobile()) << std::endl;
         }
         if (results.size() > 10) {
            std::cout << "    ... (" << (results.size() - 10) << " more)" << std::endl;
         }
      }
      std::cout << std::endl;
   }

   // Determine what constitutes a "good" match using adaptive P3-relative thresholds
   // Use adaptive thresholds instead of fixed ones
   double excellentThreshold = thresholds.excellentThreshold;
   double goodThreshold = thresholds.goodThreshold;
   double poorThreshold = thresholds.poorThreshold;

   // Adaptive analysis: look for natural breaks in the P3 distance distribution
   if (controls.shouldShowDetails())
   {
      // Debug output
      std::cout << "DEBUG: Distance analysis:" << std::endl;
      for (size_t i = 0; i < std::min(size_t(5), distances.size()); ++i) {
         std::cout << "  " << (i + 1) << ": " << std::scientific << distances[i] << std::endl;
      }
   }

   // Find the largest gap in distances to identify natural groupings
   double largestRatio = 0.0;
   double firstSignificantGap = 0.0;
   int gapAfterIndex = -1;

   if (distances.size() > 1) {
      for (size_t i = 0; i < distances.size() - 1; ++i) {
         double gap = distances[i + 1] - distances[i];
         double ratio = (distances[i] > 1e-12) ? gap / distances[i] : 0.0;

         if (controls.shouldShowDetails())
         {
            std::cout << "DEBUG: Gap " << (i + 1) << "->" << (i + 2) << ": " << gap
               << " (ratio: " << ratio << ")" << std::endl;
         }

         // Look for the FIRST significant gap with large ratio (>100x) and meaningful absolute gap (>0.001)
         if (gapAfterIndex == -1 && ratio > 100.0 && gap > 0.001) {
            firstSignificantGap = gap;
            gapAfterIndex = static_cast<int>(i);
            if (controls.shouldShowDetails()) {
               std::cout << "DEBUG: First significant gap found!" << std::endl;
            }
         }
      }
   }

   if (controls.shouldShowDetails())
   {
      std::cout << "DEBUG: Final gap analysis - first significant gap: " << firstSignificantGap
         << " after index: " << gapAfterIndex << std::endl;
   }

   // Determine adaptive threshold based on the gap analysis (but still use P3-relative base thresholds)
   double adaptiveThreshold = goodThreshold;  // Use P3-relative good threshold as base
   int adaptiveGoodCount = 0;

   if (gapAfterIndex >= 0 && firstSignificantGap > 0.001) {
      // Found a significant gap - use it to separate good from poor
      adaptiveThreshold = distances[gapAfterIndex] + firstSignificantGap / 2.0;
      adaptiveGoodCount = gapAfterIndex + 1;

      if (controls.shouldShowDetails())
      {
         std::cout << "DEBUG: Found large gap (" << firstSignificantGap << ") after result "
            << (gapAfterIndex + 1) << ", adaptive threshold: " << adaptiveThreshold << std::endl;
      }
   }
   else {
      if (controls.shouldShowDetails())
      {
         std::cout << "DEBUG: No significant gap found, using P3-relative thresholds" << std::endl;
      }
   }

   // Separate unique results into categories using P3-relative thresholds
   std::vector<LatticeMatchResult> excellentResults;
   std::vector<LatticeMatchResult> goodResults;
   std::vector<LatticeMatchResult> poorResults;
   std::vector<LatticeMatchResult> terribleResults;

   for (size_t i = 0; i < uniqueResults.size(); ++i) {
      const auto& result = uniqueResults[i];
      double distance = result.getP3Distance();

      // Use adaptive threshold if we found a significant gap, but prioritize P3-relative thresholds
      if (distance <= excellentThreshold) {
         excellentResults.push_back(result);
      }
      else if (adaptiveGoodCount > 0 && (int)i < adaptiveGoodCount) {
         goodResults.push_back(result);  // Everything before the gap is "good"
      }
      else if (distance <= goodThreshold) {
         goodResults.push_back(result);  // P3-relative good threshold
      }
      else if (distance <= poorThreshold) {
         poorResults.push_back(result);
      }
      else {
         terribleResults.push_back(result);
      }
   }

   // Decide what to show - prioritize excellent results
   std::vector<LatticeMatchResult> resultsToShow;
   std::string qualityMessage;

   if (!excellentResults.empty()) {
      // Show all excellent results
      for (const auto& result : excellentResults) {
         resultsToShow.push_back(result);
      }

      qualityMessage = "Found " + std::to_string(excellentResults.size()) + " excellent matches";

      // Only add good results for comparison if we have very few excellent results
      if (excellentResults.size() <= 2) {
         int goodToAdd = std::min(2, (int)goodResults.size());
         for (int i = 0; i < goodToAdd; ++i) {
            resultsToShow.push_back(goodResults[i]);
         }
      }
   }
   else if (!goodResults.empty()) {
      // Show all good results
      for (const auto& result : goodResults) {
         resultsToShow.push_back(result);
      }

      if (adaptiveGoodCount > 0) {
         qualityMessage = "Found " + std::to_string(goodResults.size()) + " good matches (adaptive gap analysis)";
      }
      else {
         qualityMessage = "Found " + std::to_string(goodResults.size()) + " good matches";
      }

      // Only add poor results for comparison if we have very few good results
      if (goodResults.size() <= 2) {
         int poorToAdd = std::min(2, (int)poorResults.size());
         for (int i = 0; i < poorToAdd; ++i) {
            resultsToShow.push_back(poorResults[i]);
         }
      }
   }
   else if (!poorResults.empty()) {
      // No good matches, show 1-2 best poor ones
      int poorToShow = std::min(2, (int)poorResults.size());
      for (int i = 0; i < poorToShow; ++i) {
         resultsToShow.push_back(poorResults[i]);
      }
      qualityMessage = "No good matches found. Showing " + std::to_string(poorToShow) +
         " best poor matches";
   }
   else {
      // Only terrible matches - show just the best one
      resultsToShow.push_back(terribleResults[0]);
      qualityMessage = "No reasonable matches found. Showing best terrible match";
   }

   // Display header with quality assessment
   std::cout << "=== LATTICE MATCHING RESULTS === ===================================================" << std::endl;
   std::cout << qualityMessage << std::endl;
   std::cout << "Success threshold: " << std::scientific << std::setprecision(2)
      << excellentThreshold << " Å (" << thresholds.method << ")" << std::endl;

   // Display the selected results
   for (size_t i = 0; i < resultsToShow.size(); ++i) {
      const auto& result = resultsToShow[i];
      double distance = result.getP3Distance();

      std::cout << "\n--- Match " << (i + 1) << " ---" << std::endl;

      // Add quality indicator using P3-relative thresholds
      std::string qualityString;
      if (distance <= excellentThreshold) {
         qualityString = "EXCELLENT";
      }
      else if (distance <= goodThreshold) {
         qualityString = "GOOD";
      }
      else if (distance <= poorThreshold) {
         qualityString = "POOR";
      }
      else {
         qualityString = "TERRIBLE";
      }

      std::cout << "Quality: " << qualityString << " ("
         << std::fixed << std::setprecision(3) << distance << " Å)" << std::endl;

      if (controls.shouldShowDetails())
      {
         std::cout << "Description: " << result.getDescription() << std::endl;
      }
      std::cout << "P3 Distance: " << std::fixed << std::setprecision(3) << distance << std::endl;

      // Display transformation matrix (keep your existing format)
      std::cout << "Transformation Matrix:" << std::endl;
      const Matrix_3x3& matrix = result.getTransformationMatrix();
      for (int row = 0; row < 3; ++row) {
         std::cout << "  [" << std::setw(8) << std::fixed << std::setprecision(4)
            << matrix[row * 3 + 0] << ", "
            << std::setw(8) << matrix[row * 3 + 1] << ", "
            << std::setw(8) << matrix[row * 3 + 2] << "]" << std::endl;
      }
      std::cout << "Matrix determinant: " << std::fixed << std::setprecision(4) << matrix.Det() << std::endl;

      // Show transformed cell with reference's centering symbol
      std::cout << "Transformed cell: ";
      outputCellWithCentering(result.getTransformedMobile(), reference.getLatticeType());
      std::cout << std::endl;

      // Mark successful matches using P3-relative thresholds
      if (distance <= excellentThreshold) {
         std::cout << "*** SUCCESS: Excellent match within P3-relative threshold ***" << std::endl;
      }
      else if (distance <= goodThreshold) {
         std::cout << "*** SUCCESS: Good match within P3-relative threshold ***" << std::endl;
      }
   }

   // Summary information
   if (allResults.size() > resultsToShow.size()) {
      std::cout << "\n(" << (allResults.size() - resultsToShow.size())
         << " additional matches not shown)" << std::endl;
   }
}


void displayComparisonHistogram(const std::vector<MobileComparisonResult>& allResults,
   const QualityAssessment& quality, const MultiTransformFinderControls& controls) {
   if (allResults.empty()) {
      std::cout << "No results to display in histogram." << std::endl;
      return;
   }

   std::cout << "\n=== P3 DISTANCE DISTRIBUTION (" << allResults.size() << " Mobiles) ===" << std::endl;

   // Collect all distances and sort them
   std::vector<double> distances;
   for (const auto& result : allResults) {
      distances.push_back(result.distance);
   }
   std::sort(distances.begin(), distances.end());

   double minDist = distances[0];
   double maxDist = distances.back();

   std::cout << "Range: " << std::scientific << std::setprecision(2)
      << minDist << " to " << maxDist << " A" << std::endl;
   quality.displayThresholdInfo();  // Shows P3-relative thresholds

   const auto& stats = quality.getStats();


   // Create histogram bins for better distribution visualization
   const int numBins = 8;
   std::vector<int> binCounts(numBins, 0);
   std::vector<std::pair<double, double>> binRanges(numBins);

   // Special handling for cases with perfect matches (distance = 0)
   if (minDist == 0.0) {
      // First bin is for perfect matches (0.0)
      binRanges[0] = { 0.0, 0.0 };

      // Remaining bins cover the non-zero range
      if (maxDist > 0.0) {
         double nonZeroMin = 1e12;  // Find smallest non-zero distance
         for (double dist : distances) {
            if (dist > 0.0 && dist < nonZeroMin) {
               nonZeroMin = dist;
            }
         }

         // Use logarithmic bins for the non-zero range
         if (maxDist / nonZeroMin > 10.0) {
            double logMin = std::log10(nonZeroMin);
            double logMax = std::log10(maxDist);
            double logStep = (logMax - logMin) / (numBins - 1);

            for (int i = 1; i < numBins; ++i) {
               double binStart = std::pow(10, logMin + (i - 1) * logStep);
               double binEnd = (i == numBins - 1) ? maxDist : std::pow(10, logMin + i * logStep);
               binRanges[i] = { binStart, binEnd };
            }
         }
         else {
            // Linear bins for small non-zero range
            double step = (maxDist - nonZeroMin) / (numBins - 1);
            for (int i = 1; i < numBins; ++i) {
               double binStart = nonZeroMin + (i - 1) * step;
               double binEnd = (i == numBins - 1) ? maxDist : nonZeroMin + i * step;
               binRanges[i] = { binStart, binEnd };
            }
         }
      }
   }
   else {
      // No perfect matches - use regular binning
      if (maxDist / minDist > 10.0 && maxDist > minDist) {
         double logMin = std::log10(std::max(minDist, 1e-12));
         double logMax = std::log10(maxDist);
         double logStep = (logMax - logMin) / numBins;

         for (int i = 0; i < numBins; ++i) {
            double binStart = std::pow(10, logMin + i * logStep);
            double binEnd = (i == numBins - 1) ? maxDist : std::pow(10, logMin + (i + 1) * logStep);
            binRanges[i] = { binStart, binEnd };
         }
      }
      else {
         // Linear bins for small ranges
         double step = (maxDist - minDist) / numBins;
         for (int i = 0; i < numBins; ++i) {
            double binStart = minDist + i * step;
            double binEnd = (i == numBins - 1) ? maxDist : minDist + (i + 1) * step;
            binRanges[i] = { binStart, binEnd };
         }
      }
   }

   // Count distances in each bin
   for (double dist : distances) {
      bool assigned = false;
      for (int i = 0; i < numBins; ++i) {
         if ((i == 0 && minDist == 0.0 && dist == 0.0) ||  // Perfect match bin
            (dist >= binRanges[i].first && dist <= binRanges[i].second)) {
            binCounts[i]++;
            assigned = true;
            break;
         }
      }
      // Fallback for any unassigned distances
      if (!assigned && binCounts[numBins - 1] >= 0) {
         binCounts[numBins - 1]++;
      }
   }

   // Display quality summary - only show if there's a mix of qualities
   quality.displayQualitySummary();

   // Show distribution histogram 
   std::cout << "\nDistance Distribution:" << std::endl;
   int maxBinCount = *std::max_element(binCounts.begin(), binCounts.end());

   for (int i = 0; i < numBins; ++i) {
      if (binCounts[i] > 0) {
         if (i == 0 && minDist == 0.0) {
            // Special formatting for perfect matches
            std::cout << "  0.00e+00 (perfect): " << std::setw(4) << binCounts[i] << " ";
         }
         else if (i < numBins - 1) {
            std::cout << "  " << std::scientific << std::setprecision(2) << binRanges[i].first
               << " - " << std::setprecision(2) << binRanges[i].second << ": " << std::setw(4) << binCounts[i] << " ";
         }
         else {
            std::cout << "  " << std::scientific << std::setprecision(2) << binRanges[i].first
               << " +        : " << std::setw(4) << binCounts[i] << " ";
         }

         int barLength = (maxBinCount > 0) ? (binCounts[i] * 30) / maxBinCount : 0;
         for (int j = 0; j < std::max(1, barLength); ++j) {
            std::cout << "#";
         }
         std::cout << std::endl;
      }
   }

   std::cout << std::endl;
}

// Integration example for displayComparisonResults:
void displayP3RelativeQuality(
   const LRL_Cell& referenceCell,
   const std::string& latticeType,
   const std::vector<MobileComparisonResult>& allResults) {

   // Extract distances
   std::vector<double> distances;
   for (const auto& result : allResults) {
      distances.push_back(result.distance);
   }

   // Calculate P3-relative thresholds
   auto thresholds = P3RelativeThresholds::calculateAdaptiveP3Thresholds(
      referenceCell, latticeType, distances);

   std::cout << "=== P3-RELATIVE QUALITY ANALYSIS ===" << std::endl;
   std::cout << "Reference P3 norm: " << std::fixed << std::setprecision(3)
      << thresholds.referenceP3Norm << " Å" << std::endl;
   std::cout << "Method: " << thresholds.method << std::endl;

   // Show thresholds as both absolute and relative
   std::cout << "Thresholds:" << std::endl;
   std::cout << "  EXCELLENT <= " << std::scientific << std::setprecision(2)
      << thresholds.excellentThreshold << " ("
      << std::fixed << std::setprecision(1)
      << (thresholds.excellentThreshold / thresholds.referenceP3Norm * 100) << "% of P3)" << std::endl;
   std::cout << "  GOOD      <= " << std::scientific << std::setprecision(2)
      << thresholds.goodThreshold << " ("
      << std::fixed << std::setprecision(1)
      << (thresholds.goodThreshold / thresholds.referenceP3Norm * 100) << "% of P3)" << std::endl;
   std::cout << "  POOR      <= " << std::scientific << std::setprecision(2)
      << thresholds.poorThreshold << " ("
      << std::fixed << std::setprecision(1)
      << (thresholds.poorThreshold / thresholds.referenceP3Norm * 100) << "% of P3)" << std::endl;

   // Count using P3-relative thresholds
   int excellent = 0, good = 0, poor = 0, terrible = 0;
   for (double dist : distances) {
      if (dist <= thresholds.excellentThreshold) excellent++;
      else if (dist <= thresholds.goodThreshold) good++;
      else if (dist <= thresholds.poorThreshold) poor++;
      else terrible++;
   }

   std::cout << "P3-relative quality: " << excellent << " excellent, "
      << good << " good, " << poor << " poor, " << terrible << " terrible" << std::endl;

   // Show best match as percentage of P3 norm
   double bestDistance = *std::min_element(distances.begin(), distances.end());
   std::cout << "Best match: " << std::fixed << std::setprecision(4)
      << (bestDistance / thresholds.referenceP3Norm * 100) << "% of P3 norm" << std::endl;
}


/**
 * Helper function to resize all strings in a vector to uniform width
 * @param strings Vector of strings to resize
 * @param leftAlign If true, pad on right (left-align). If false, pad on left (right-align)
 * @return New vector with all strings padded to same width
 */
std::vector<std::string> resizeStringsToUniformWidth(const std::vector<std::string>& strings, bool leftAlign = true) {
   if (strings.empty()) {
      return strings;
   }

   // Find maximum width
   size_t maxWidth = 0;
   for (const auto& str : strings) {
      maxWidth = std::max(maxWidth, str.length());
   }

   // Create resized vector
   std::vector<std::string> resizedStrings;
   resizedStrings.reserve(strings.size());

   for (const auto& str : strings) {
      std::string resized = str;
      size_t paddingNeeded = maxWidth - str.length();

      if (leftAlign) {
         // Left-align: pad on the right
         resized.append(paddingNeeded, ' ');
      }
      else {
         // Right-align: pad on the left
         resized.insert(0, paddingNeeded, ' ');
      }

      resizedStrings.push_back(resized);
   }

   return resizedStrings;
}

/**
 * Formats transformation matrix display with proper alignment using column-based approach
 * @param sortedMatrices Vector of pairs (count, matrixKey) sorted by count descending
 * @param matrixDistances Map from matrixKey to P3 distance
 * @param thresholds P3RelativeThresholds for quality assessment
 * @param maxEntriesToShow Maximum number of entries to format (default 10)
 * @return Vector of formatted strings ready for output
 */
std::vector<std::string> formatMatrixDisplay(
   const std::vector<std::pair<int, std::string>>& sortedMatrices,
   const std::map<std::string, double>& matrixDistances,
   const P3RelativeThresholds::P3ThresholdResult& thresholds,
   int maxEntriesToShow = 10) {

   std::vector<std::string> formattedLines;

   if (sortedMatrices.empty()) {
      formattedLines.push_back("No transformation matrices to display.");
      return formattedLines;
   }

   int entriesToShow = std::min(maxEntriesToShow, (int)sortedMatrices.size());

   // Step 1: Create vectors for each column
   std::vector<std::string> matrixColumns;
   std::vector<std::string> countColumns;
   std::vector<std::string> qualityColumns;
   std::vector<std::string> distanceColumns;

   // Step 2: Populate all column vectors with raw data
   for (int i = 0; i < entriesToShow; ++i) {
      const int count = sortedMatrices[i].first;
      const std::string& matrixKey = sortedMatrices[i].second;

      // Matrix column: "[matrixKey]"
      matrixColumns.push_back("[" + matrixKey + "]");

      // Count column: convert to string
      countColumns.push_back(std::to_string(count));

      // Get distance
      double distance = 0.0;
      auto distanceIt = matrixDistances.find(matrixKey);
      if (distanceIt != matrixDistances.end()) {
         distance = distanceIt->second;
      }

      // Quality column
      std::string qualityStr;
      if (distance <= thresholds.excellentThreshold) {
         qualityStr = "EXCELLENT";
      }
      else if (distance <= thresholds.goodThreshold) {
         qualityStr = "GOOD";
      }
      else if (distance <= thresholds.poorThreshold) {
         qualityStr = "POOR";
      }
      else {
         qualityStr = "TERRIBLE";
      }
      qualityColumns.push_back(qualityStr);

      // Distance column: format to 3 decimal places
      std::ostringstream distanceStream;
      distanceStream << std::fixed << std::setprecision(3) << distance;
      distanceColumns.push_back(distanceStream.str());
   }

   // Step 3: Resize each column using helper function
   matrixColumns = resizeStringsToUniformWidth(matrixColumns, true);    // left-aligned
   countColumns = resizeStringsToUniformWidth(countColumns, false);     // right-aligned
   qualityColumns = resizeStringsToUniformWidth(qualityColumns, true);  // left-aligned
   distanceColumns = resizeStringsToUniformWidth(distanceColumns, false); // right-aligned

   // Step 4: Assemble final formatted lines by concatenating columns
   for (size_t i = 0; i < matrixColumns.size(); ++i) {
      std::string line = "  " +                    // leading spaces
         matrixColumns[i] +          // [matrix]
         ": " +                      // separator
         countColumns[i] +           // count (right-aligned)
         " mobiles (" +              // literal text
         qualityColumns[i] +         // quality (left-aligned)
         ", " +                      // separator
         distanceColumns[i] +        // distance (right-aligned)
         ")";                        // closing

      formattedLines.push_back(line);
   }

   return formattedLines;
}

/**
 * Convenience function to display formatted matrix results
 * Usage example:
 *
 * auto formattedLines = formatMatrixDisplay(sortedMatrices, matrixDistances, thresholds, 10);
 * std::cout << "Most common transformation matrices:" << std::endl;
 * for (const auto& line : formattedLines) {
 *     std::cout << line << std::endl;
 * }
 */
void displayFormattedMatrices(
   const std::vector<std::pair<int, std::string>>& sortedMatrices,
   const std::map<std::string, double>& matrixDistances,
   const P3RelativeThresholds::P3ThresholdResult& thresholds,
   int maxEntriesToShow = 10) {

   auto formattedLines = formatMatrixDisplay(sortedMatrices, matrixDistances, thresholds, maxEntriesToShow);

   std::cout << "Most common transformation matrices:" << std::endl;
   for (const auto& line : formattedLines) {
      std::cout << line << std::endl;
   }
}

void displayComparisonResults(const LatticeCell& reference,
   const std::vector<MobileComparisonResult>& allResults,
   const MultiTransformFinderControls& controls) {

   if (allResults.empty()) {
      std::cout << "No matches found for any mobile." << std::endl;
      return;
   }

   // Create single quality assessment object (single source of truth)
   std::vector<double> distances;
   for (const auto& result : allResults) {
      distances.push_back(result.distance);
   }
   QualityAssessment quality(reference, distances);

   // Display histogram with unified quality assessment
   displayComparisonHistogram(allResults, quality, controls);

   std::cout << "=== COMPARISON SUMMARY ===" << std::endl;
   std::cout << "Reference: " << LRL_Cell_Degrees(reference.getCell())
      << " (" << reference.getLatticeType() << ")" << std::endl;

   bool showDetailedList = (allResults.size() <= 5) ||
      (controls.shouldShowDetails() && allResults.size() <= 20);

   if (showDetailedList) {
      // Small dataset - show individual mobiles
      std::cout << "\n=== ALL MOBILES (Input Order) ===" << std::endl;
      for (const auto& result : allResults) {
         displayCompactResult(result, reference, quality);
      }
   }
   else {
      // Large dataset - show only summary statistics
      std::cout << "Total mobiles: " << allResults.size() << std::endl;

      // Group by transformation matrix
      std::map<std::string, int> matrixCounts;
      std::map<std::string, double> matrixDistances;

      for (const auto& result : allResults) {
         const Matrix_3x3& matrix = result.bestResult.getTransformationMatrix();
         std::ostringstream key;
         key << std::fixed << std::setprecision(0);
         for (int i = 0; i < 9; ++i) {
            key << static_cast<int>(std::round(matrix[i]));
            if (i < 8) key << ",";
         }
         std::string matrixKey = key.str();
         matrixCounts[matrixKey]++;

         // Store the first distance for each matrix (representative)
         if (matrixDistances.find(matrixKey) == matrixDistances.end()) {
            matrixDistances[matrixKey] = result.distance;
         }
      }

      std::cout << "Unique transformation matrices: " << matrixCounts.size() << std::endl;

      // Sort matrices by frequency (most common first)
      std::vector<std::pair<int, std::string>> sortedMatrices;
      for (const auto& [matrix, count] : matrixCounts) {
         sortedMatrices.push_back({ count, matrix });
      }
      std::sort(sortedMatrices.rbegin(), sortedMatrices.rend());

      // Show top transformation matrices
// Show top transformation matrices
// Calculate thresholds for quality assessment  
      std::vector<double> distances;
      for (const auto& result : allResults) {
         distances.push_back(result.distance);
      }

      auto thresholds = P3RelativeThresholds::calculateAdaptiveP3Thresholds(
         reference.getCell(), reference.getLatticeType(), distances);

      // Use the new formatter function
      displayFormattedMatrices(sortedMatrices, matrixDistances, thresholds, 10);

      // Find best match
      double bestDistance = allResults[0].distance;
      int bestMobileIndex = allResults[0].mobileIndex;
      for (const auto& result : allResults) {
         if (result.distance < bestDistance) {
            bestDistance = result.distance;
            bestMobileIndex = result.mobileIndex;
         }
      }

      // Show detailed information for best matches
      std::cout << "\n=== BEST MATCH DETAILS ===" << std::endl;

      std::vector<MobileComparisonResult> sortedResults = allResults;
      std::sort(sortedResults.begin(), sortedResults.end(),
         [](const MobileComparisonResult& a, const MobileComparisonResult& b) {
            return a.distance < b.distance;
         });

      int detailsToShow = std::min(3, (int)sortedResults.size());
      for (int i = 0; i < detailsToShow; ++i) {
         const auto& result = sortedResults[i];
         const Matrix_3x3& matrix = result.bestResult.getTransformationMatrix();

         std::cout << "Mobile " << result.mobileIndex << ": "
            << std::scientific << std::setprecision(2) << result.distance
            << " (" << quality.assessQualityString(result.distance) << ")" << std::endl;

         // Show transformation matrix
         std::cout << "  Matrix: [";
         for (int j = 0; j < 9; ++j) {
            std::cout << std::fixed << std::setprecision(0) << static_cast<int>(std::round(matrix[j]));
            if (j < 8) std::cout << " ";
         }
         std::cout << "]" << std::endl;

         // Show transformed cell
         const LRL_Cell& transformedMobile = result.bestResult.getTransformedMobile();
         std::cout << "  Transformed: " << LRL_Cell_Degrees(transformedMobile) << std::endl;
         std::cout << std::endl;
      }
   }
}


void displayCompactResult(const MobileComparisonResult& result, 
   const LatticeCell& reference,
   const QualityAssessment& quality) {
   // Get matrix as single line of 9 values
   const auto& matrix = result.bestResult.getTransformationMatrix();
   std::cout << "Mobile " << result.mobileIndex << ": "
      << LRL_Cell_Degrees(result.mobile.getCell()) << " (" << result.mobile.getLatticeType() << ")" << std::endl;
   // Check if matrix has only integer values
   bool isInteger = true;
   for (int i = 0; i < 9; ++i) {
      if (std::abs(matrix[i] - std::round(matrix[i])) > 1e-10) {
         isInteger = false;
         break;
      }
   }
   std::cout << "  P3: " << std::fixed << std::setprecision(3) << result.distance
      << "  Matrix: [";
   if (isInteger) {
      // Show integers only
      for (int i = 0; i < 9; ++i) {
         std::cout << static_cast<int>(std::round(matrix[i]));
         if (i < 8) std::cout << " ";
      }
   }
   else {
      // Show with decimals
      std::cout << std::setprecision(4);
      for (int i = 0; i < 9; ++i) {
         std::cout << matrix[i];
         if (i < 8) std::cout << " ";
      }
   }
   std::cout << "]" << std::endl;
   std::cout << "  Transformed: ";
   outputCellWithCentering(result.bestResult.getTransformedMobile(), reference.getLatticeType());
   std::cout << std::endl;
}

void runInputListMode(const std::vector<LatticeCell>& inputList, const MultiTransformFinderControls& controls) {
   std::cout << "\n=== PROCESSING INPUT LIST ===" << std::endl;
   ProductionLatticeMatcherSystem matcher(controls);

   if (inputList.size() == 2) {
      // Single mobile - use existing detailed output
      std::vector<LatticeMatchResult> results = matcher.processInputList(inputList);
      if (controls.shouldShowDetails()) {
         std::cout << "DEBUG: Total results returned from matcher: " << results.size() << std::endl;
      }
      displayResults(results, controls, inputList[0]);
   }
   else {
      // Multiple mobiles - behavior depends on comparison mode
      const LatticeCell& reference = inputList[0];

      if (controls.shouldRunComparisonMode()) {
         // Comparison mode - collect all results and show summary statistics
         std::vector<MobileComparisonResult> comparisonResults;

         if (controls.shouldShowDetails()) {
            std::cout << "; === PRODUCTION LATTICE MATCHING SYSTEM ===" << std::endl;
            std::cout << "; Reference: " << LRL_Cell_Degrees(reference.getCell())
               << " (" << reference.getLatticeType() << ")" << std::endl;
            std::cout << "; Processing " << (inputList.size() - 1) << " mobile lattices in comparison mode" << std::endl;
         }

         // Process each mobile and collect best result for each
         for (size_t i = 1; i < inputList.size(); ++i) {
            const LatticeCell& mobile = inputList[i];
            if (controls.shouldShowDetails()) {
               std::cout << "\n; --- Processing Mobile " << i << " ---" << std::endl;
               std::cout << "; Mobile: " << LRL_Cell_Degrees(mobile.getCell())
                  << " (" << mobile.getLatticeType() << ")" << std::endl;
            }

            std::vector<LatticeCell> singlePair = { reference, mobile };
            std::vector<LatticeMatchResult> results = matcher.processInputList(singlePair);

            if (!results.empty()) {
               // Find the result with minimum distance
               LatticeMatchResult bestResult = results[0];
               double bestDistance = results[0].getP3Distance();
               for (const auto& result : results) {
                  if (result.getP3Distance() < bestDistance) {
                     bestDistance = result.getP3Distance();
                     bestResult = result;
                  }
               }

               comparisonResults.push_back({
                   static_cast<int>(i),
                   mobile,
                   bestResult,
                   bestDistance
                  });

               if (controls.shouldShowDetails()) {
                  std::cout << "; Found " << results.size() << " transformations, best distance: "
                     << std::scientific << std::setprecision(3) << bestDistance << std::endl;
               }
            }
         }

         // Display comparison results with matrix summary
         displayComparisonResults(reference, comparisonResults, controls);
      }
      else {
         // Individual mode - show detailed results for each mobile separately
         if (controls.shouldShowDetails()) {
            std::cout << "=== INDIVIDUAL MOBILE MATCHING ===" << std::endl;
            std::cout << "Reference: " << LRL_Cell_Degrees(reference.getCell())
               << " (" << reference.getLatticeType() << ")" << std::endl;
            std::cout << "Processing " << (inputList.size() - 1) << " mobile lattices individually" << std::endl;
         }

         // Process each mobile individually and show full results
         for (size_t i = 1; i < inputList.size(); ++i) {
            const LatticeCell& mobile = inputList[i];

            std::cout << "\n=== MOBILE " << i << " RESULTS ===" << std::endl;
            std::cout << "Mobile: " << LRL_Cell_Degrees(mobile.getCell())
               << " (" << mobile.getLatticeType() << ")" << std::endl;

            // Create input pair for this mobile
            std::vector<LatticeCell> singlePair = { reference, mobile };
            std::vector<LatticeMatchResult> results = matcher.processInputList(singlePair);

            if (results.empty()) {
               std::cout << "No matching transformations found for this mobile." << std::endl;
            }
            else {
               // Display full detailed results for this mobile
               displayResults(results, controls, reference);
            }
         }

         // Show overall summary at the end
         std::cout << "\n=== OVERALL SUMMARY ===" << std::endl;
         std::cout << "Processed " << (inputList.size() - 1) << " mobile lattices individually." << std::endl;
      }
   }
}


      int main() {
         try {
            std::cout << "=== PRODUCTION LATTICE MATCHER SYSTEM ===" << std::endl;
            std::cout << "4-Layer Architecture Demonstration" << std::endl;

            // Setup input handling using your actual system
             // Setup controls and input handling
            MultiTransformFinderControls controls;
            const BasicProgramInput<MultiTransformFinderControls> program_setup("ProductionLatticeMatcher", controls);

            if (controls.shouldShowControls()) {
               std::cout << controls << std::endl;
            }

            // Get input list
            const std::vector<LatticeCell>& inputList = program_setup.getInputList();

            if (inputList.size() < 2) {
               std::cout << "Need at least 2 input cells." << std::endl;
               return 0;
            }

            // Process the input list using configured controls (from input handling)
            runInputListMode(inputList, controls);

            return 0;
         }
         catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return -1;
         }
      }
