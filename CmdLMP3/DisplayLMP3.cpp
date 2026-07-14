#include "DisplayLMP3.h"
#include "DisplayUtilities.h"

#include "LRL_Cell_Degrees.h"
#include "Matrix_3x3.h"
#include "P3.h"

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
   const LatticeCell& mobile,
   const LRL_Cell& primitiveReference) {
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
   std::cout << "   ; ";
   outputCellWithCentering(reference.getCell(), reference.getLatticeType(), "REFERENCE");
   std::cout << std::endl;
   std::cout << "   ; ";
   outputCellWithCentering(mobile.getCell(), mobile.getLatticeType(), "MOBILE");
   std::cout << std::endl;

   if (controls.shouldShowDetails()) {
      std::cout << "   ; NC Distance: " << allResults[0].getNcDistance() << std::endl;
      displayP3DistanceHistogram(allResults, controls, reference);
   }

   std::vector<double> distances;
   for (const auto& result : allResults) {
      distances.push_back(result.getP3Distance());
   }

   const ResultSelection selection = selectResultsToShow(allResults, reference);
   const auto& thresholds = selection.thresholds;
   const auto& resultsToShow = selection.resultsToShow;

   if (controls.shouldShowDetails()) {
      if (selection.uniqueResultCount < allResults.size()) {
         std::cout << "\n; Deduplication Summary:" << std::endl;
         std::cout << ";   Original results:   " << allResults.size() << std::endl;
         std::cout << ";   Unique matrices:    " << selection.uniqueResultCount << std::endl;
         std::cout << ";   Duplicates removed: " << (allResults.size() - selection.uniqueResultCount) << std::endl;
      } else {
         std::cout << "\n; No duplicate matrices found - all " << allResults.size()
            << " results have unique matrices." << std::endl;
      }
   }

   if (controls.shouldShowDetails()) {
      std::cout << "; DEBUG: Distance analysis:" << std::endl;
      for (size_t i = 0; i < std::min(size_t(5), distances.size()); ++i) {
         std::cout << ";   " << (i + 1) << ": " << std::scientific << distances[i] << std::endl;
      }
   }

   std::cout << "\n; === LATTICE MATCHING RESULTS ===" << std::endl;

   // If the best result is a supercell or subcell, announce it once before the match loop
   announceSupercellOrSubcell(resultsToShow);

   for (size_t i = 0; i < resultsToShow.size(); ++i) {
      const auto& result = resultsToShow[i];
      double      distance = result.getP3Distance();

      std::cout << "\n; --- Match " << (i + 1) << " ---" << std::endl;

      const MatchQuality quality = computeMatchQuality(result, reference.getCell(), thresholds);
      const double rawDet = result.getTransformationMatrix().Det();
      const int det = static_cast<int>(std::round(std::abs(rawDet)));

      if (controls.shouldShowDetails()) {
         std::cout << "; DEBUG primitiveReference: "
            << LRL_Cell_Degrees(primitiveReference) << std::endl;
         std::cout << "; DEBUG transformedMobile: "
            << LRL_Cell_Degrees(result.getTransformedMobile()) << std::endl;
      }

      const double p3Percent = (thresholds.referenceP3Norm > 0.0)
         ? (distance / thresholds.referenceP3Norm * 100.0) : 0.0;
      std::cout << "; " << quality.qualityString << "  P3 distance: "
         << std::fixed << std::setprecision(3) << distance
         << " (" << std::setprecision(1) << p3Percent << "% of P3 norm)"
         << "  S6 angle: " << std::setprecision(2) << quality.s6Angle << " deg" << std::endl;
      if (quality.s6Warning)
         std::cout << "; ** WARNING: S6 angle > 15 deg -- this is likely not a crystallographic match **" << std::endl;

      if (controls.shouldShowDetails()) {
         std::cout << "; Description: " << result.getDescription() << std::endl;
      }

      const Matrix_3x3& matrix = result.getTransformationMatrix();
      const Matrix_3x3  inverse = matrix.Inver();

      std::cout << "; Matrix:  ";
      printTransformMatrix(matrix);
      std::cout << " (Determinant: " << std::fixed << std::setprecision(3) << rawDet;
      if (det > 1)                       std::cout << "  [SUPERCELL]";
      else if (std::abs(rawDet) < 0.99)  std::cout << "  [SUBCELL]";
      std::cout << ")" << std::endl;

      std::cout << "; Inverse: ";
      printTransformMatrix(inverse);
      std::cout << std::endl;

      std::cout << "; ";
      outputCellWithCentering(reference.getCell(), reference.getLatticeType(), "REFERENCE");
      std::cout << std::endl;

      if (controls.shouldShowDetails()) {
         std::cout << "; DEBUG getTransformedMobile: "
            << LRL_Cell_Degrees(result.getTransformedMobile()) << std::endl;
      }

      std::cout << "; ";
      outputCellWithCentering(result.getTransformedMobile(),
         reference.getLatticeType(), "TRANSFORMED MOBILE");
      std::cout << std::endl;
   }

   if (controls.shouldShowDetails() && allResults.size() > resultsToShow.size()) {
      const size_t nHidden = allResults.size() - resultsToShow.size();
      std::cout << "; " << nHidden << " additional match"
         << (nHidden == 1 ? "" : "es") << " not shown (display limit reached)." << std::endl;
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
