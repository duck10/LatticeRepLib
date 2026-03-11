#include "DisplayLMP3.h"

#include "LRL_Cell_Degrees.h"
#include "Matrix_3x3.h"
#include "P3.h"
#include "S6.h"
#include "TransformerUtilities.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>

// ---------------------------------------------------------------------------
// Low-level cell output
// ---------------------------------------------------------------------------

void outputCellWithCentering(const LRL_Cell& cell, const std::string& centeringSymbol) {
   std::cout << centeringSymbol << " " << LRL_Cell_Degrees(cell);
}

// ---------------------------------------------------------------------------
// Single-mobile display helpers
// ---------------------------------------------------------------------------

void displayP3DistanceHistogram(const std::vector<LatticeMatchResult>& allResults,
   const MultiTransformFinderControls& controls,
   const LatticeCell& reference) {
   if (allResults.empty()) {
      std::cout << "No results to display in histogram." << std::endl;
      return;
   }

   if (controls.shouldShowDetails()) {
      std::cout << "Sample result descriptions:" << std::endl;
      int samplesToShow = std::min(5, (int)allResults.size());
      for (int i = 0; i < samplesToShow; ++i) {
         std::cout << "  " << (i + 1) << ": " << allResults[i].getDescription()
            << " (P3: " << std::scientific << std::setprecision(6)
            << allResults[i].getP3Distance() << ")" << std::endl;
      }
      std::cout << std::endl;
   }

   std::vector<std::pair<double, int>> distanceWithIndex;
   for (size_t i = 0; i < allResults.size(); ++i) {
      distanceWithIndex.push_back({ allResults[i].getP3Distance(), static_cast<int>(i + 1) });
   }
   std::sort(distanceWithIndex.begin(), distanceWithIndex.end());

   double minDist = distanceWithIndex[0].first;
   double maxDist = distanceWithIndex.back().first;

   std::vector<double> distances;
   for (const auto& result : allResults) {
      distances.push_back(result.getP3Distance());
   }

   auto thresholds = P3RelativeThresholds::calculateAdaptiveP3Thresholds(
      reference.getCell(), reference.getLatticeType(), distances);

   std::cout << "Range: " << std::scientific << std::setprecision(2)
      << minDist << " to " << maxDist << " Å" << std::endl;

   std::cout << "Reference P3 norm: " << std::fixed << std::setprecision(3)
      << thresholds.referenceP3Norm << " Å" << std::endl;

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

   std::cout << "Scale: Each '#' represents relative P3 distance" << std::endl;
   std::cout << std::endl;
}


void displayResults(const std::vector<LatticeMatchResult>& allResults,
   const MultiTransformFinderControls& controls,
   const LatticeCell& reference) {
   if (controls.shouldShowDetails()) {
      std::cout << "DEBUG: First 5 results entering displayResults:" << std::endl;
      for (size_t i = 0; i < std::min(size_t(5), allResults.size()); ++i) {
         std::cout << "  " << (i + 1) << ": P3=" << allResults[i].getP3Distance() << std::endl;
      }
   }
   if (allResults.empty()) {
      std::cout << "No results generated - this should never happen!" << std::endl;
      return;
   }

   std::cout << "Total results before processing: " << allResults.size() << std::endl;
   std::cout << "NC Distance: " << allResults[0].getNcDistance() << std::endl;

   displayP3DistanceHistogram(allResults, controls, reference);

   std::vector<double> distances;
   for (const auto& result : allResults) {
      distances.push_back(result.getP3Distance());
   }

   auto thresholds = P3RelativeThresholds::calculateAdaptiveP3Thresholds(
      reference.getCell(), reference.getLatticeType(), distances);

   std::vector<LatticeMatchResult> uniqueResults = deduplicateByMatrix(allResults);

   if (controls.shouldShowDetails()) {
      if (uniqueResults.size() < allResults.size()) {
         std::cout << "\nDeduplication Summary:" << std::endl;
         std::cout << "  Original results: " << allResults.size() << std::endl;
         std::cout << "  Unique matrices: " << uniqueResults.size() << std::endl;
         std::cout << "  Duplicates removed: " << (allResults.size() - uniqueResults.size()) << std::endl;
      } else {
         std::cout << "\nNo duplicate matrices found - all " << allResults.size()
            << " results have unique matrices." << std::endl;
      }
   }

   double excellentThreshold = thresholds.excellentThreshold;
   double goodThreshold = thresholds.goodThreshold;
   double poorThreshold = thresholds.poorThreshold;

   if (controls.shouldShowDetails()) {
      std::cout << "DEBUG: Distance analysis:" << std::endl;
      for (size_t i = 0; i < std::min(size_t(5), distances.size()); ++i) {
         std::cout << "  " << (i + 1) << ": " << std::scientific << distances[i] << std::endl;
      }
   }

   double largestRatio = 0.0;
   double firstSignificantGap = 0.0;
   int    gapAfterIndex = -1;

   if (distances.size() > 1) {
      for (size_t i = 0; i < distances.size() - 1; ++i) {
         double gap = distances[i + 1] - distances[i];
         double ratio = (distances[i] > 1e-12) ? gap / distances[i] : 0.0;

         if (controls.shouldShowDetails()) {
            std::cout << "DEBUG: Gap " << (i + 1) << "->" << (i + 2) << ": " << gap
               << " (ratio: " << ratio << ")" << std::endl;
         }

         if (gapAfterIndex == -1 && ratio > 100.0 && gap > 0.001) {
            firstSignificantGap = gap;
            gapAfterIndex = static_cast<int>(i);
            if (controls.shouldShowDetails()) {
               std::cout << "DEBUG: First significant gap found!" << std::endl;
            }
         }
      }
   }

   if (controls.shouldShowDetails()) {
      std::cout << "DEBUG: Final gap analysis - first significant gap: " << firstSignificantGap
         << " after index: " << gapAfterIndex << std::endl;
   }

   double adaptiveThreshold = goodThreshold;
   int    adaptiveGoodCount = 0;

   if (gapAfterIndex >= 0 && firstSignificantGap > 0.001) {
      adaptiveThreshold = distances[gapAfterIndex] + firstSignificantGap / 2.0;
      adaptiveGoodCount = gapAfterIndex + 1;
      if (controls.shouldShowDetails()) {
         std::cout << "DEBUG: Found large gap (" << firstSignificantGap << ") after result "
            << (gapAfterIndex + 1) << ", adaptive threshold: " << adaptiveThreshold << std::endl;
      }
   } else {
      if (controls.shouldShowDetails()) {
         std::cout << "DEBUG: No significant gap found, using P3-relative thresholds" << std::endl;
      }
   }

   std::vector<LatticeMatchResult> excellentResults;
   std::vector<LatticeMatchResult> goodResults;
   std::vector<LatticeMatchResult> poorResults;
   std::vector<LatticeMatchResult> terribleResults;

   for (size_t i = 0; i < uniqueResults.size(); ++i) {
      const auto& result = uniqueResults[i];
      double      distance = result.getP3Distance();

      if (distance <= excellentThreshold) {
         excellentResults.push_back(result);
      } else if (adaptiveGoodCount > 0 && (int)i < adaptiveGoodCount) {
         goodResults.push_back(result);
      } else if (distance <= goodThreshold) {
         goodResults.push_back(result);
      } else if (distance <= poorThreshold) {
         poorResults.push_back(result);
      } else {
         terribleResults.push_back(result);
      }
   }

   std::vector<LatticeMatchResult> resultsToShow;
   std::string qualityMessage;

   if (!excellentResults.empty()) {
      for (const auto& result : excellentResults) { resultsToShow.push_back(result); }
      qualityMessage = "Found " + std::to_string(excellentResults.size()) + " excellent matches";
      if (excellentResults.size() <= 2) {
         int goodToAdd = std::min(2, (int)goodResults.size());
         for (int i = 0; i < goodToAdd; ++i) { resultsToShow.push_back(goodResults[i]); }
      }
   } else if (!goodResults.empty()) {
      for (const auto& result : goodResults) { resultsToShow.push_back(result); }
      qualityMessage = adaptiveGoodCount > 0
         ? "Found " + std::to_string(goodResults.size()) + " good matches (adaptive gap analysis)"
         : "Found " + std::to_string(goodResults.size()) + " good matches";
      if (goodResults.size() <= 2) {
         int poorToAdd = std::min(2, (int)poorResults.size());
         for (int i = 0; i < poorToAdd; ++i) { resultsToShow.push_back(poorResults[i]); }
      }
   } else if (!poorResults.empty()) {
      int poorToShow = std::min(2, (int)poorResults.size());
      for (int i = 0; i < poorToShow; ++i) { resultsToShow.push_back(poorResults[i]); }
      qualityMessage = "No good matches found. Showing " + std::to_string(poorToShow) +
         " best poor matches";
   } else {
      resultsToShow.push_back(terribleResults[0]);
      qualityMessage = "No reasonable matches found. Showing best terrible match";
   }

   std::cout << "=== LATTICE MATCHING RESULTS === ===================================================" << std::endl;
   std::cout << qualityMessage << std::endl;
   std::cout << "Success threshold: " << std::scientific << std::setprecision(2)
      << excellentThreshold << " Å (" << thresholds.method << ")" << std::endl;

   for (size_t i = 0; i < resultsToShow.size(); ++i) {
      const auto& result = resultsToShow[i];
      double      distance = result.getP3Distance();

      std::cout << "\n--- Match " << (i + 1) << " ---" << std::endl;

      std::string qualityString;
      if (distance <= excellentThreshold) qualityString = "EXCELLENT";
      else if (distance <= goodThreshold)      qualityString = "GOOD";
      else if (distance <= poorThreshold)      qualityString = "POOR";
      else                                     qualityString = "TERRIBLE";

      std::cout << "Quality: " << qualityString << " (P3 distance: "
         << std::fixed << std::setprecision(3) << distance << std::endl;

      if (controls.shouldShowDetails()) {
         std::cout << "Description: " << result.getDescription() << std::endl;
      }
      std::cout << "P3 Distance: " << std::fixed << std::setprecision(3) << distance << std::endl;

      const S6 referenceS6(reference.getCell());
      const S6 transformedS6(result.getTransformedMobile());
      double s6Angle = TransformerUtilities::angleS6(referenceS6, transformedS6);
      std::cout << "S6 Angle: " << std::fixed << std::setprecision(2) << s6Angle << " degrees" << std::endl;

      std::cout << "Transformation Matrix:" << std::endl;
      const Matrix_3x3& matrix = result.getTransformationMatrix();
      for (int row = 0; row < 3; ++row) {
         std::cout << "  [" << std::setw(8) << std::fixed << std::setprecision(4)
            << matrix[row * 3 + 0] << ", "
            << std::setw(8) << matrix[row * 3 + 1] << ", "
            << std::setw(8) << matrix[row * 3 + 2] << "]" << std::endl;
      }
      std::cout << "Matrix determinant: " << std::fixed << std::setprecision(4)
         << matrix.Det() << std::endl;

      std::cout << "Transformed cell: ";
      outputCellWithCentering(result.getTransformedMobile(), reference.getLatticeType());
      std::cout << std::endl;

      if (distance <= excellentThreshold) std::cout << "*** SUCCESS: Excellent match within P3-relative threshold ***" << std::endl;
      else if (distance <= goodThreshold)      std::cout << "*** SUCCESS: Good match within P3-relative threshold ***" << std::endl;
   }

   if (allResults.size() > resultsToShow.size()) {
      std::cout << "\n(" << (allResults.size() - resultsToShow.size())
         << " additional matches not shown)" << std::endl;
   }
}

// ---------------------------------------------------------------------------
// Multi-mobile / comparison display helpers
// ---------------------------------------------------------------------------

void displayComparisonHistogram(const std::vector<MobileComparisonResult>& allResults,
   const QualityAssessment& quality,
   const MultiTransformFinderControls& controls) {
   if (allResults.empty()) {
      std::cout << "No results to display in histogram." << std::endl;
      return;
   }

   std::cout << "\n=== P3 DISTANCE DISTRIBUTION (" << allResults.size() << " Mobiles) ===" << std::endl;

   std::vector<double> distances;
   for (const auto& result : allResults) { distances.push_back(result.distance); }
   std::sort(distances.begin(), distances.end());

   double minDist = distances[0];
   double maxDist = distances.back();

   std::cout << "Range: " << std::scientific << std::setprecision(2)
      << minDist << " to " << maxDist << " A" << std::endl;
   quality.displayThresholdInfo();

   const auto& stats = quality.getStats();

   const int numBins = 8;
   std::vector<int>                        binCounts(numBins, 0);
   std::vector<std::pair<double, double>>  binRanges(numBins);

   if (minDist == 0.0) {
      binRanges[0] = { 0.0, 0.0 };
      if (maxDist > 0.0) {
         double nonZeroMin = 1e12;
         for (double dist : distances) {
            if (dist > 0.0 && dist < nonZeroMin) nonZeroMin = dist;
         }
         if (maxDist / nonZeroMin > 10.0) {
            double logMin = std::log10(nonZeroMin);
            double logMax = std::log10(maxDist);
            double logStep = (logMax - logMin) / (numBins - 1);
            for (int i = 1; i < numBins; ++i) {
               double binStart = std::pow(10, logMin + (i - 1) * logStep);
               double binEnd = (i == numBins - 1) ? maxDist : std::pow(10, logMin + i * logStep);
               binRanges[i] = { binStart, binEnd };
            }
         } else {
            double step = (maxDist - nonZeroMin) / (numBins - 1);
            for (int i = 1; i < numBins; ++i) {
               double binStart = nonZeroMin + (i - 1) * step;
               double binEnd = (i == numBins - 1) ? maxDist : nonZeroMin + i * step;
               binRanges[i] = { binStart, binEnd };
            }
         }
      }
   } else {
      if (maxDist / minDist > 10.0 && maxDist > minDist) {
         double logMin = std::log10(std::max(minDist, 1e-12));
         double logMax = std::log10(maxDist);
         double logStep = (logMax - logMin) / numBins;
         for (int i = 0; i < numBins; ++i) {
            double binStart = std::pow(10, logMin + i * logStep);
            double binEnd = (i == numBins - 1) ? maxDist : std::pow(10, logMin + (i + 1) * logStep);
            binRanges[i] = { binStart, binEnd };
         }
      } else {
         double step = (maxDist - minDist) / numBins;
         for (int i = 0; i < numBins; ++i) {
            double binStart = minDist + i * step;
            double binEnd = (i == numBins - 1) ? maxDist : minDist + (i + 1) * step;
            binRanges[i] = { binStart, binEnd };
         }
      }
   }

   for (double dist : distances) {
      bool assigned = false;
      for (int i = 0; i < numBins; ++i) {
         if ((i == 0 && minDist == 0.0 && dist == 0.0) ||
            (dist >= binRanges[i].first && dist <= binRanges[i].second)) {
            binCounts[i]++;
            assigned = true;
            break;
         }
      }
      if (!assigned && binCounts[numBins - 1] >= 0) binCounts[numBins - 1]++;
   }

   quality.displayQualitySummary();

   std::cout << "\nDistance Distribution:" << std::endl;
   int maxBinCount = *std::max_element(binCounts.begin(), binCounts.end());

   for (int i = 0; i < numBins; ++i) {
      if (binCounts[i] > 0) {
         if (i == 0 && minDist == 0.0) {
            std::cout << "  0.00e+00 (perfect): " << std::setw(4) << binCounts[i] << " ";
         } else if (i < numBins - 1) {
            std::cout << "  " << std::scientific << std::setprecision(2) << binRanges[i].first
               << " - " << std::setprecision(2) << binRanges[i].second
               << ": " << std::setw(4) << binCounts[i] << " ";
         } else {
            std::cout << "  " << std::scientific << std::setprecision(2) << binRanges[i].first
               << " +        : " << std::setw(4) << binCounts[i] << " ";
         }
         int barLength = (maxBinCount > 0) ? (binCounts[i] * 30) / maxBinCount : 0;
         for (int j = 0; j < std::max(1, barLength); ++j) std::cout << "#";
         std::cout << std::endl;
      }
   }

   std::cout << std::endl;
}


void displayP3RelativeQuality(const LRL_Cell& referenceCell,
   const std::string& latticeType,
   const std::vector<MobileComparisonResult>& allResults) {
   std::vector<double> distances;
   for (const auto& result : allResults) { distances.push_back(result.distance); }

   auto thresholds = P3RelativeThresholds::calculateAdaptiveP3Thresholds(
      referenceCell, latticeType, distances);

   std::cout << "=== P3-RELATIVE QUALITY ANALYSIS ===" << std::endl;
   std::cout << "Reference P3 norm: " << std::fixed << std::setprecision(3)
      << thresholds.referenceP3Norm << " Å" << std::endl;
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

   int excellent = 0, good = 0, poor = 0, terrible = 0;
   for (double dist : distances) {
      if (dist <= thresholds.excellentThreshold) ++excellent;
      else if (dist <= thresholds.goodThreshold)      ++good;
      else if (dist <= thresholds.poorThreshold)      ++poor;
      else                                            ++terrible;
   }

   std::cout << "P3-relative quality: " << excellent << " excellent, "
      << good << " good, " << poor << " poor, " << terrible << " terrible" << std::endl;

   double bestDistance = *std::min_element(distances.begin(), distances.end());
   std::cout << "Best match: " << std::fixed << std::setprecision(4)
      << (bestDistance / thresholds.referenceP3Norm * 100) << "% of P3 norm" << std::endl;
}


void displayCompactResult(const MobileComparisonResult& result,
   const LatticeCell& reference,
   const QualityAssessment& quality) {
   const auto& matrix = result.bestResult.getTransformationMatrix();

   std::cout << "Mobile " << result.mobileIndex << ": "
      << LRL_Cell_Degrees(result.mobile.getCell())
      << " (" << result.mobile.getLatticeType() << ")" << std::endl;

   bool isInteger = true;
   for (int i = 0; i < 9; ++i) {
      if (std::abs(matrix[i] - std::round(matrix[i])) > 1e-10) { isInteger = false; break; }
   }

   std::cout << "  P3: " << std::fixed << std::setprecision(3) << result.distance
      << "  Matrix: [";
   if (isInteger) {
      for (int i = 0; i < 9; ++i) {
         std::cout << static_cast<int>(std::round(matrix[i]));
         if (i < 8) std::cout << " ";
      }
   } else {
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


void displayComparisonResults(const LatticeCell& reference,
   const std::vector<MobileComparisonResult>& allResults,
   const MultiTransformFinderControls& controls) {
   if (allResults.empty()) {
      std::cout << "No matches found for any mobile." << std::endl;
      return;
   }

   std::vector<double> distances;
   for (const auto& result : allResults) { distances.push_back(result.distance); }
   QualityAssessment quality(reference, distances);

   displayComparisonHistogram(allResults, quality, controls);

   std::cout << "=== COMPARISON SUMMARY ===" << std::endl;
   std::cout << "Reference: " << LRL_Cell_Degrees(reference.getCell())
      << " (" << reference.getLatticeType() << ")" << std::endl;

   bool showDetailedList = (allResults.size() <= 5) ||
      (controls.shouldShowDetails() && allResults.size() <= 20);

   if (showDetailedList) {
      std::cout << "\n=== ALL MOBILES (Input Order) ===" << std::endl;
      for (const auto& result : allResults) {
         displayCompactResult(result, reference, quality);
      }
   } else {
      std::cout << "Total mobiles: " << allResults.size() << std::endl;

      std::map<std::string, int>    matrixCounts;
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
         if (matrixDistances.find(matrixKey) == matrixDistances.end()) {
            matrixDistances[matrixKey] = result.distance;
         }
      }

      std::cout << "Unique transformation matrices: " << matrixCounts.size() << std::endl;

      std::vector<std::pair<int, std::string>> sortedMatrices;
      for (const auto& [matrix, count] : matrixCounts) {
         sortedMatrices.push_back({ count, matrix });
      }
      std::sort(sortedMatrices.rbegin(), sortedMatrices.rend());

      std::vector<double> allDistances;
      for (const auto& result : allResults) { allDistances.push_back(result.distance); }
      auto thresholds = P3RelativeThresholds::calculateAdaptiveP3Thresholds(
         reference.getCell(), reference.getLatticeType(), allDistances);

      displayFormattedMatrices(sortedMatrices, matrixDistances, thresholds, 10);

      // Best match details
      std::vector<MobileComparisonResult> sortedResults = allResults;
      std::sort(sortedResults.begin(), sortedResults.end(),
         [](const MobileComparisonResult& a, const MobileComparisonResult& b) {
            return a.distance < b.distance;
         });

      std::cout << "\n=== BEST MATCH DETAILS ===" << std::endl;
      int detailsToShow = std::min(3, (int)sortedResults.size());
      for (int i = 0; i < detailsToShow; ++i) {
         const auto& result = sortedResults[i];
         const Matrix_3x3& matrix = result.bestResult.getTransformationMatrix();

         std::cout << "Mobile " << result.mobileIndex << ": "
            << std::scientific << std::setprecision(2) << result.distance
            << " (" << quality.assessQualityString(result.distance) << ")" << std::endl;

         std::cout << "  Matrix: [";
         for (int j = 0; j < 9; ++j) {
            std::cout << std::fixed << std::setprecision(0)
               << static_cast<int>(std::round(matrix[j]));
            if (j < 8) std::cout << " ";
         }
         std::cout << "]" << std::endl;

         std::cout << "  Transformed: "
            << LRL_Cell_Degrees(result.bestResult.getTransformedMobile()) << std::endl;
         std::cout << std::endl;
      }
   }
}

// ---------------------------------------------------------------------------
// Matrix-display formatting utilities
// ---------------------------------------------------------------------------

std::vector<std::string> resizeStringsToUniformWidth(
   const std::vector<std::string>& strings, bool leftAlign) {
   if (strings.empty()) return strings;

   size_t maxWidth = 0;
   for (const auto& str : strings) { maxWidth = std::max(maxWidth, str.length()); }

   std::vector<std::string> resized;
   resized.reserve(strings.size());
   for (const auto& str : strings) {
      std::string s = str;
      size_t padding = maxWidth - str.length();
      if (leftAlign) s.append(padding, ' ');
      else           s.insert(0, padding, ' ');
      resized.push_back(s);
   }
   return resized;
}


std::vector<std::string> formatMatrixDisplay(
   const std::vector<std::pair<int, std::string>>& sortedMatrices,
   const std::map<std::string, double>& matrixDistances,
   const P3RelativeThresholds::P3ThresholdResult& thresholds,
   int maxEntriesToShow) {
   std::vector<std::string> formattedLines;

   if (sortedMatrices.empty()) {
      formattedLines.push_back("No transformation matrices to display.");
      return formattedLines;
   }

   int entriesToShow = std::min(maxEntriesToShow, (int)sortedMatrices.size());

   std::vector<std::string> matrixColumns, countColumns, qualityColumns, distanceColumns;

   for (int i = 0; i < entriesToShow; ++i) {
      const int         count = sortedMatrices[i].first;
      const std::string& matrixKey = sortedMatrices[i].second;

      matrixColumns.push_back("[" + matrixKey + "]");
      countColumns.push_back(std::to_string(count));

      double distance = 0.0;
      auto it = matrixDistances.find(matrixKey);
      if (it != matrixDistances.end()) distance = it->second;

      std::string qualityStr;
      if (distance <= thresholds.excellentThreshold) qualityStr = "EXCELLENT";
      else if (distance <= thresholds.goodThreshold)      qualityStr = "GOOD";
      else if (distance <= thresholds.poorThreshold)      qualityStr = "POOR";
      else                                                qualityStr = "TERRIBLE";
      qualityColumns.push_back(qualityStr);

      std::ostringstream ds;
      ds << std::fixed << std::setprecision(3) << distance;
      distanceColumns.push_back(ds.str());
   }

   matrixColumns = resizeStringsToUniformWidth(matrixColumns, true);
   countColumns = resizeStringsToUniformWidth(countColumns, false);
   qualityColumns = resizeStringsToUniformWidth(qualityColumns, true);
   distanceColumns = resizeStringsToUniformWidth(distanceColumns, false);

   for (size_t i = 0; i < matrixColumns.size(); ++i) {
      formattedLines.push_back(
         "  " + matrixColumns[i] + ": " + countColumns[i] +
         " mobiles (" + qualityColumns[i] + ", " + distanceColumns[i] + ")");
   }
   return formattedLines;
}


void displayFormattedMatrices(
   const std::vector<std::pair<int, std::string>>& sortedMatrices,
   const std::map<std::string, double>& matrixDistances,
   const P3RelativeThresholds::P3ThresholdResult& thresholds,
   int maxEntriesToShow) {
   auto formattedLines = formatMatrixDisplay(
      sortedMatrices, matrixDistances, thresholds, maxEntriesToShow);
   std::cout << "Most common transformation matrices:" << std::endl;
   for (const auto& line : formattedLines) std::cout << line << std::endl;
}

// ---------------------------------------------------------------------------
// Diagnostic / verification output
// ---------------------------------------------------------------------------

void verifyMatrixCellConsistency(const std::vector<LatticeMatchResult>& results,
   const LRL_Cell& originalMobile) {
   std::cout << "\n=== MATRIX-CELL CONSISTENCY CHECK ===" << std::endl;
   std::cout << "Original mobile: " << LRL_Cell_Degrees(originalMobile) << std::endl;

   for (size_t i = 0; i < results.size(); ++i) {
      const auto& result = results[i];
      const Matrix_3x3& matrix = result.getTransformationMatrix();

      std::cout << "\nResult " << (i + 1) << ":" << std::endl;
      std::cout << "Matrix: [";
      for (int j = 0; j < 9; ++j) {
         std::cout << std::fixed << std::setprecision(4) << matrix[j];
         if (j < 8) std::cout << " ";
      }
      std::cout << "]" << std::endl;

      LRL_Cell calculatedTransformed = matrix * originalMobile;
      std::cout << "Calculated (matrix x mobile): " << LRL_Cell_Degrees(calculatedTransformed) << std::endl;
      std::cout << "Stored transformed cell:      " << LRL_Cell_Degrees(result.getTransformedMobile()) << std::endl;

      double cellDifference = LRL_Cell::DistanceBetween(result.getTransformedMobile(), calculatedTransformed);
      std::cout << "Difference: " << cellDifference << std::endl;

      if (cellDifference > 1e-6)
         std::cout << "*** ERROR: Matrix and stored cell are inconsistent! ***" << std::endl;
      else
         std::cout << "*** OK: Matrix and cell are consistent ***" << std::endl;
   }
   std::cout << "=== END CONSISTENCY CHECK ===" << std::endl;
}