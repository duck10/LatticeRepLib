#ifndef DISPLAYLMP3_H
#define DISPLAYLMP3_H

#include <iomanip>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "DisplayUtilities.h"
#include "LatticeCell.h"
#include "LatticeMatchResult.h"
#include "MultiTransformFinderControls.h"
#include "P3RelativeThresholds.h"

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
   const LatticeCell& mobile,
   const LRL_Cell& primitiveReference);

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
