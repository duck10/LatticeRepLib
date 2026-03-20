#ifndef DISPLAYLMP3_H
#define DISPLAYLMP3_H

#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "LatticeCell.h"
#include "LatticeMatchResult.h"
#include "MultiTransformFinderControls.h"
#include "P3RelativeThresholds.h"

// ---------------------------------------------------------------------------
// Deduplication utility (template must live in the header)
// ---------------------------------------------------------------------------

template<typename ResultType>
std::vector<ResultType> deduplicateByMatrix(const std::vector<ResultType>& results) {
   std::vector<ResultType> uniqueResults;
   std::set<std::string>   seenMatrices;

   for (const auto& result : results) {
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

// ---------------------------------------------------------------------------
// Low-level cell output
// Optional label is appended as " [LABEL]" when non-empty.
// ---------------------------------------------------------------------------

void outputCellWithCentering(const LRL_Cell& cell,
   const std::string& centeringSymbol,
   const std::string& label = "");

// ---------------------------------------------------------------------------
// Single-mobile display helpers
// ---------------------------------------------------------------------------

void displayP3DistanceHistogram(const std::vector<LatticeMatchResult>& allResults,
   const MultiTransformFinderControls& controls,
   const LatticeCell& reference);

void displayResults(const std::vector<LatticeMatchResult>& allResults,
   const MultiTransformFinderControls& controls,
   const LatticeCell& reference,
   const LatticeCell& mobile);

// ---------------------------------------------------------------------------
// Matrix-display formatting utilities
// ---------------------------------------------------------------------------

std::vector<std::string> resizeStringsToUniformWidth(
   const std::vector<std::string>& strings,
   bool leftAlign = true);

std::vector<std::string> formatMatrixDisplay(
   const std::vector<std::pair<int, std::string>>& sortedMatrices,
   const std::map<std::string, double>& matrixDistances,
   const P3RelativeThresholds::P3ThresholdResult& thresholds,
   int maxEntriesToShow = 10);

void displayFormattedMatrices(
   const std::vector<std::pair<int, std::string>>& sortedMatrices,
   const std::map<std::string, double>& matrixDistances,
   const P3RelativeThresholds::P3ThresholdResult& thresholds,
   int maxEntriesToShow = 10);

#endif // DISPLAYLMP3_H
