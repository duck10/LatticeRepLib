#ifndef DISPLAYLMP3_H
#define DISPLAYLMP3_H

#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "LatticeCell.h"
#include "LatticeMatchResult.h"
#include "MobileComparisonResult.h"
#include "MultiTransformFinderControls.h"
#include "P3RelativeThresholds .h"
#include "QualityAssessment.h"

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
// ---------------------------------------------------------------------------

void outputCellWithCentering(const LRL_Cell& cell, const std::string& centeringSymbol);

// ---------------------------------------------------------------------------
// Single-mobile display helpers
// ---------------------------------------------------------------------------

void displayP3DistanceHistogram(const std::vector<LatticeMatchResult>& allResults,
   const MultiTransformFinderControls& controls,
   const LatticeCell& reference);

void displayResults(const std::vector<LatticeMatchResult>& allResults,
   const MultiTransformFinderControls& controls,
   const LatticeCell& reference);

// ---------------------------------------------------------------------------
// Multi-mobile / comparison display helpers
// ---------------------------------------------------------------------------

void displayComparisonHistogram(const std::vector<MobileComparisonResult>& allResults,
   const QualityAssessment& quality,
   const MultiTransformFinderControls& controls);

void displayP3RelativeQuality(const LRL_Cell& referenceCell,
   const std::string& latticeType,
   const std::vector<MobileComparisonResult>& allResults);

void displayCompactResult(const MobileComparisonResult& result,
   const LatticeCell& reference,
   const QualityAssessment& quality);

void displayComparisonResults(const LatticeCell& reference,
   const std::vector<MobileComparisonResult>& allResults,
   const MultiTransformFinderControls& controls);

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

// ---------------------------------------------------------------------------
// Diagnostic / verification output
// ---------------------------------------------------------------------------

void verifyMatrixCellConsistency(const std::vector<LatticeMatchResult>& results,
   const LRL_Cell& originalMobile);

#endif // DISPLAYLMP3_H
