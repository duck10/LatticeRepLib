#include "DisplayLMP3.h"
#include "G6.h"

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

void outputCellWithCentering(const LRL_Cell& cell,
   const std::string& centeringSymbol,
   const std::string& label) {
   std::cout << centeringSymbol << " " << LRL_Cell_Degrees(cell);
   if (!label.empty())
      std::cout << " [" << label << "]";
}

// ---------------------------------------------------------------------------
// Single-mobile display helpers
// ---------------------------------------------------------------------------

void displayP3DistanceHistogram(const std::vector<LatticeMatchResult>& allResults,
   const MultiTransformFinderControls& controls,
   const LatticeCell& reference) {
   if (allResults.empty()) {
      std::cout << "; No results to display in histogram." << std::endl;
      return;
   }

   if (controls.shouldShowDetails()) {
      std::cout << "; Sample result descriptions:" << std::endl;
      int samplesToShow = std::min(5, (int)allResults.size());
      for (int i = 0; i < samplesToShow; ++i) {
         std::cout << ";   " << (i + 1) << ": " << allResults[i].getDescription()
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

   std::cout << "; Range: " << std::scientific << std::setprecision(2)
      << minDist << " to " << maxDist << " A" << std::endl;

   std::cout << "; Reference P3 norm: " << std::fixed << std::setprecision(3)
      << thresholds.referenceP3Norm << " A" << std::endl;

   std::cout << "; Method: " << thresholds.method << std::endl;
   std::cout << "; Thresholds:" << std::endl;
   std::cout << ";   EXCELLENT <= " << std::scientific << std::setprecision(2)
      << thresholds.excellentThreshold << " ("
      << std::fixed << std::setprecision(1)
      << (thresholds.excellentThreshold / thresholds.referenceP3Norm * 100) << "% of P3)" << std::endl;
   std::cout << ";   GOOD      <= " << std::scientific << std::setprecision(2)
      << thresholds.goodThreshold << " ("
      << std::fixed << std::setprecision(1)
      << (thresholds.goodThreshold / thresholds.referenceP3Norm * 100) << "% of P3)" << std::endl;
   std::cout << ";   POOR      <= " << std::scientific << std::setprecision(2)
      << thresholds.poorThreshold << " ("
      << std::fixed << std::setprecision(1)
      << (thresholds.poorThreshold / thresholds.referenceP3Norm * 100) << "% of P3)" << std::endl;

   std::cout << "; Scale: Each '#' represents relative P3 distance" << std::endl;
   std::cout << std::endl;
}


void displayResults(const std::vector<LatticeMatchResult>& allResults,
   const MultiTransformFinderControls& controls,
   const LatticeCell& reference,
   const LatticeCell& mobile) {
   if (controls.shouldShowDetails()) {
      std::cout << "; DEBUG: First 5 results entering displayResults:" << std::endl;
      for (size_t i = 0; i < std::min(size_t(5), allResults.size()); ++i) {
         std::cout << ";   " << (i + 1) << ": P3=" << allResults[i].getP3Distance() << std::endl;
      }
   }
   if (allResults.empty()) {
      std::cout << "; No results generated - this should never happen!" << std::endl;
      return;
   }

   // Always show reference and mobile so the reader knows what was matched
   std::cout << "; ";
   outputCellWithCentering(reference.getCell(), reference.getLatticeType(), "REFERENCE");
   std::cout << std::endl;
   std::cout << "; ";
   outputCellWithCentering(mobile.getCell(), mobile.getLatticeType(), "MOBILE");
   std::cout << std::endl;

   if (controls.shouldShowDetails()) {
      std::cout << "; NC Distance: " << allResults[0].getNcDistance() << std::endl;
      displayP3DistanceHistogram(allResults, controls, reference);
   }

   std::vector<double> distances;
   for (const auto& result : allResults) {
      distances.push_back(result.getP3Distance());
   }

   auto thresholds = P3RelativeThresholds::calculateAdaptiveP3Thresholds(
      reference.getCell(), reference.getLatticeType(), distances);

   std::vector<LatticeMatchResult> uniqueResults = deduplicateByMatrix(allResults);

   if (controls.shouldShowDetails()) {
      if (uniqueResults.size() < allResults.size()) {
         std::cout << "\n; Deduplication Summary:" << std::endl;
         std::cout << ";   Original results:   " << allResults.size() << std::endl;
         std::cout << ";   Unique matrices:    " << uniqueResults.size() << std::endl;
         std::cout << ";   Duplicates removed: " << (allResults.size() - uniqueResults.size()) << std::endl;
      } else {
         std::cout << "\n; No duplicate matrices found - all " << allResults.size()
            << " results have unique matrices." << std::endl;
      }
   }

   double excellentThreshold = thresholds.excellentThreshold;
   double goodThreshold = thresholds.goodThreshold;
   double poorThreshold = thresholds.poorThreshold;

   if (controls.shouldShowDetails()) {
      std::cout << "; DEBUG: Distance analysis:" << std::endl;
      for (size_t i = 0; i < std::min(size_t(5), distances.size()); ++i) {
         std::cout << ";   " << (i + 1) << ": " << std::scientific << distances[i] << std::endl;
      }
   }

   std::vector<LatticeMatchResult> excellentResults;
   std::vector<LatticeMatchResult> goodResults;
   std::vector<LatticeMatchResult> poorResults;
   std::vector<LatticeMatchResult> terribleResults;

   for (const auto& result : uniqueResults) {
      const double distance = result.getP3Distance();
      if (distance <= excellentThreshold) excellentResults.push_back(result);
      else if (distance <= goodThreshold)      goodResults.push_back(result);
      else if (distance <= poorThreshold)      poorResults.push_back(result);
      else                                     terribleResults.push_back(result);
   }

   // Deduplicate by transformed cell -- multiple matrices may produce the
   // same lattice; keep only the first of each distinct result.
   auto deduplicateByCell = [](std::vector<LatticeMatchResult>& v) {
      std::vector<LatticeMatchResult> out;
      for (const auto& r : v) {
         bool dup = false;
         for (const auto& kept : out) {
            double diff = 0.0;
            const G6 ga(r.getTransformedMobile());
            const G6 gb(kept.getTransformedMobile());
            for (int k = 0; k < 6; ++k) diff += std::abs(ga[k] - gb[k]);
            if (diff < 1e-3) { dup = true; break; }
         }
         if (!dup) out.push_back(r);
      }
      v = out;
      };
   deduplicateByCell(excellentResults);
   deduplicateByCell(goodResults);
   deduplicateByCell(poorResults);
   deduplicateByCell(terribleResults);

   std::vector<LatticeMatchResult> resultsToShow;

   if (!excellentResults.empty()) {
      for (const auto& r : excellentResults) resultsToShow.push_back(r);
      if (excellentResults.size() <= 2) {
         int goodToAdd = std::min(2, (int)goodResults.size());
         for (int i = 0; i < goodToAdd; ++i) resultsToShow.push_back(goodResults[i]);
      }
   } else if (!goodResults.empty()) {
      for (const auto& r : goodResults) resultsToShow.push_back(r);
      if (goodResults.size() <= 2) {
         int poorToAdd = std::min(2, (int)poorResults.size());
         for (int i = 0; i < poorToAdd; ++i) resultsToShow.push_back(poorResults[i]);
      }
   } else if (!poorResults.empty()) {
      resultsToShow.push_back(poorResults[0]);
   } else {
      resultsToShow.push_back(terribleResults[0]);
   }

   std::cout << "\n; === LATTICE MATCHING RESULTS ===" << std::endl;

   // If the best result is a supercell or subcell, announce it once before the match loop
   if (!resultsToShow.empty()) {
      const double bestRawDet = resultsToShow[0].getTransformationMatrix().Det();
      const int    bestDet = static_cast<int>(std::round(std::abs(bestRawDet)));
      if (bestDet > 1) {
         std::cout << "; === SUPERCELL ===  prim. cell volume ratio = "
            << std::fixed << std::setprecision(3) << std::abs(bestRawDet) << std::endl;
      } else if (std::abs(bestRawDet) < 0.99) {
         std::cout << "; === SUBCELL ===  prim. cell volume ratio = "
            << std::fixed << std::setprecision(3) << std::abs(bestRawDet) << std::endl;
      }
   }

   for (size_t i = 0; i < resultsToShow.size(); ++i) {
      const auto& result = resultsToShow[i];
      double      distance = result.getP3Distance();

      std::cout << "\n; --- Match " << (i + 1) << " ---" << std::endl;

      const double rawDet = result.getTransformationMatrix().Det();
      const int det = static_cast<int>(std::round(std::abs(rawDet)));
      std::string qualityString;
      if (det > 1) {
         // Supercell: combine order label with quality tier
         std::string tier;
         if (distance <= excellentThreshold) tier = "EXCELLENT";
         else if (distance <= goodThreshold)      tier = "GOOD";
         else if (distance <= poorThreshold)      tier = "POOR";
         else                                     tier = "DISTORTED";
         qualityString = "order-" + std::to_string(det) + " supercell  " + tier;
      } else if (std::abs(rawDet) < 0.99) {
         // Subcell (e.g. det=0.5 from centred-to-primitive conversion)
         std::string tier;
         if (distance <= excellentThreshold) tier = "EXCELLENT";
         else if (distance <= goodThreshold)      tier = "GOOD";
         else if (distance <= poorThreshold)      tier = "POOR";
         else                                     tier = "DISTORTED";
         qualityString = "subcell  " + tier;
      } else {
         if (distance <= excellentThreshold) qualityString = "EQUIVALENT";
         else if (distance <= goodThreshold)      qualityString = "GOOD";
         else if (distance <= poorThreshold)      qualityString = "POOR";
         else                                     qualityString = "DISTORTED";
      }
      const double s6Angle = TransformerUtilities::angleS6(
         S6(result.getTransformedMobile()), S6(reference.getCell()));
      const double p3Percent = (thresholds.referenceP3Norm > 0.0)
         ? (distance / thresholds.referenceP3Norm * 100.0) : 0.0;
      std::cout << "; " << qualityString << "  P3 distance: "
         << std::fixed << std::setprecision(3) << distance
         << " (" << std::setprecision(1) << p3Percent << "% of P3 norm)"
         << "  S6 angle: " << std::setprecision(2) << s6Angle << " deg" << std::endl;
      if (s6Angle > 15.0)
         std::cout << "; ** WARNING: S6 angle > 15 deg -- this is likely not a crystallographic match **" << std::endl;

      if (controls.shouldShowDetails()) {
         std::cout << "; Description: " << result.getDescription() << std::endl;
      }

      const Matrix_3x3& matrix = result.getTransformationMatrix();
      std::cout << "; Matrix: [";

      // Format each element as an integer or rational fraction with denominator <= 6.
      // Try denominators 1..6 in order; use the first that fits within 1e-6.
      // Falls back to 1 decimal place if no small rational is found.
      auto formatElement = [](double v) -> std::string {
         for (int denom = 1; denom <= 6; ++denom) {
            const double scaled = v * denom;
            const double nearest = std::round(scaled);
            if (std::abs(scaled - nearest) < 1e-4) {
               const int numer = static_cast<int>(nearest);
               if (denom == 1) return std::to_string(numer);
               // Reduce by GCD
               const int g = [](int a, int b) {
                  a = std::abs(a); b = std::abs(b);
                  while (b) { int t = b; b = a % b; a = t; }
                  return a;
                  }(std::abs(numer), denom);
               const int rn = numer / g;
               const int rd = denom / g;
               if (rd == 1) return std::to_string(rn);
               return std::to_string(rn) + "/" + std::to_string(rd);
            }
         }
         // No small rational found -- fall back to 1 decimal place
         std::ostringstream oss;
         oss << std::fixed << std::setprecision(1) << v;
         return oss.str();
         };

      for (int j = 0; j < 9; ++j) {
         std::cout << formatElement(matrix[j]);
         if (j < 8) std::cout << " ";
         if (j == 2 || j == 5) std::cout << " ";  // extra space between rows
      }
      std::cout << "] (Determinant: "
         << std::fixed << std::setprecision(3) << rawDet;
      if (det > 1)                  std::cout << "  [SUPERCELL]";
      else if (std::abs(rawDet) < 0.99)  std::cout << "  [SUBCELL]";
      std::cout << ")" << std::endl;

      std::cout << "; ";
      outputCellWithCentering(reference.getCell(), reference.getLatticeType(), "REFERENCE");
      std::cout << std::endl;
      std::cout << "; ";
      outputCellWithCentering(result.getTransformedMobile(),
         reference.getLatticeType(), "TRANSFORMED MOBILE");
      std::cout << std::endl;
   }

   if (controls.shouldShowDetails() && allResults.size() > resultsToShow.size()) {
      const size_t nHidden = allResults.size() - resultsToShow.size();
      std::cout << "; " << nHidden << " additional match"
         << (nHidden == 1 ? "" : "es") << " below threshold not shown." << std::endl;
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
      size_t      padding = maxWidth - str.length();
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
      formattedLines.push_back("; No transformation matrices to display.");
      return formattedLines;
   }

   int entriesToShow = std::min(maxEntriesToShow, (int)sortedMatrices.size());

   std::vector<std::string> matrixColumns, countColumns, qualityColumns, distanceColumns;

   for (int i = 0; i < entriesToShow; ++i) {
      const int          count = sortedMatrices[i].first;
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
         ";   " + matrixColumns[i] + ": " + countColumns[i] +
         " mobiles (" + qualityColumns[i] + ", " + distanceColumns[i] + ")");
   }
   return formattedLines;
}

void displayFormattedMatrices(
   const std::vector<std::pair<int, std::string>>& sortedMatrices,
   const std::map<std::string, double>& matrixDistances,
   const P3RelativeThresholds::P3ThresholdResult& thresholds,
   int maxEntriesToShow) {
   const auto formattedLines = formatMatrixDisplay(
      sortedMatrices, matrixDistances, thresholds, maxEntriesToShow);
   std::cout << "; Most common transformation matrices:" << std::endl;
   for (const auto& line : formattedLines) std::cout << line << std::endl;
}
