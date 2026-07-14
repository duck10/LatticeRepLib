#ifndef DISPLAYUTILITIES_H
#define DISPLAYUTILITIES_H

#include "LatticeCell.h"
#include "LatticeMatchResult.h"
#include "LRL_Cell.h"
#include "Matrix_3x3.h"
#include "P3RelativeThresholds.h"

#include <iomanip>
#include <set>
#include <sstream>
#include <string>
#include <vector>

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
// Pure matrix formatting -- no frame dependency.
// ---------------------------------------------------------------------------

// Format a single matrix element as an integer or a small rational fraction
// (denominator <= 6). Falls back to 1 decimal place if no small rational fits.
std::string formatMatrixElement(double v);

// Print a 3x3 matrix using formatMatrixElement, in the "[a b c  d e f  g h i]"
// style used throughout the match display.
void printTransformMatrix(const Matrix_3x3& m);

// ---------------------------------------------------------------------------
// Result selection: dedup by matrix and by transformed cell, bucket by
// quality tier, and pick the subset of results to actually print.
// ---------------------------------------------------------------------------

struct ResultSelection {
   P3RelativeThresholds::P3ThresholdResult thresholds;
   std::vector<LatticeMatchResult> resultsToShow;
   size_t totalResultCount = 0;   // allResults.size(), for "N additional matches not shown"
   size_t uniqueResultCount = 0;  // unique-by-matrix count, for dedup summary
};

ResultSelection selectResultsToShow(
   const std::vector<LatticeMatchResult>& allResults,
   const LatticeCell& reference);

// ---------------------------------------------------------------------------
// Per-match quality string and S6 angle. Frame-independent: always compares
// Niggli-reduced primitive reference vs Niggli-reduced primitive transformed
// mobile, regardless of which frame the cells are eventually displayed in.
// ---------------------------------------------------------------------------

struct MatchQuality {
   std::string qualityString;   // e.g. "EQUIVALENT", "order-2 supercell  GOOD"
   double      s6Angle = 0.0;
   bool        s6Warning = false;  // true when s6Angle > 15 deg
};

// referenceCell is the reference's ORIGINAL cell (reference.getCell()), not the
// primitive reference and not Niggli-re-reduced -- s6Angle is the angle between
// this cell and result.getTransformedMobile() as the matcher actually produced
// it. Comparing two re-reduced/normalized cells instead would make s6Angle
// scale-blind (it would read near 0 for same-shape-different-size cells, which
// misrepresents a real mismatch as a good match).
MatchQuality computeMatchQuality(
   const LatticeMatchResult& result,
   const LRL_Cell& referenceCell,
   const P3RelativeThresholds::P3ThresholdResult& thresholds);

// ---------------------------------------------------------------------------
// SUPERCELL/SUBCELL banner, printed once before the match loop based on the
// best (first) result in resultsToShow.
// ---------------------------------------------------------------------------

void announceSupercellOrSubcell(const std::vector<LatticeMatchResult>& resultsToShow);

inline void flagCenteringRelationship(
   const LatticeMatchResult& result,
   const LatticeCell& reference,
   const LatticeCell& mobile)
{
   const std::string& refType = reference.getLatticeType();
   const std::string& mobType = mobile.getLatticeType();

   if (refType == mobType) return;

   const double rawDet = result.getTransformationMatrix().Det();
   if (std::abs(rawDet - 1.0) >= 0.01) return;

   const bool eitherIsPrimitive = (refType == "P" || mobType == "P");

   std::cout << "; NOTE: The two input cells have different centering types ("
      << refType << " and " << mobType << ") but the transformation\n"
      << ";       matrix has determinant 1. This occurs because a centered\n"
      << ";       lattice is always constructed by adding extra lattice points\n"
      << ";       to a primitive lattice. The primitive sublattice of the\n"
      << ";       centered cell matches the primitive cell exactly, which is\n"
      << ";       why P3 distance is zero. This is not the same as two\n"
      << ";       lattices being equivalent -- the centered cell has more\n"
      << ";       lattice points per unit cell than the primitive cell.\n";

   if (eitherIsPrimitive) {
      std::cout << "; The NCDist metric (a measure of lattice difference\n"
         << ";       differences) has a value of "
         << std::fixed << std::setprecision(1) << result.getNcDistance()
         << " for this pair, confirming\n"
         << ";       they are different lattice types despite the perfect\n"
         << ";       P3 match.\n";
   } else {
      std::cout << "; The NCDist metric has a value of "
         << std::fixed << std::setprecision(1) << result.getNcDistance()
         << " for this pair.\n"
         << ";       A non-zero value indicates the lattices have different\n"
         << ";       point densities despite the perfect P3 match.\n";
   }
}


#endif // DISPLAYUTILITIES_H