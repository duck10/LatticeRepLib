#include "DisplayUtilities.h"

#include "G6.h"
#include "Niggli.h"
#include "S6.h"
#include "TransformerUtilities.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>

// ---------------------------------------------------------------------------
// Matrix formatting
// ---------------------------------------------------------------------------

std::string formatMatrixElement(double v) {
   for (int denom = 1; denom <= 6; ++denom) {
      const double scaled = v * denom;
      const double nearest = std::round(scaled);
      if (std::abs(scaled - nearest) < 1e-4) {
         const int numer = static_cast<int>(nearest);
         if (denom == 1) return std::to_string(numer);
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
   std::ostringstream oss;
   oss << std::fixed << std::setprecision(1) << v;
   return oss.str();
}

void printTransformMatrix(const Matrix_3x3& m) {
   std::cout << "[";
   for (int j = 0; j < 9; ++j) {
      std::cout << formatMatrixElement(m[j]);
      if (j < 8) std::cout << " ";
      if (j == 2 || j == 5) std::cout << "  ";
   }
   std::cout << "]";
}

// ---------------------------------------------------------------------------
// Result selection
// ---------------------------------------------------------------------------

ResultSelection selectResultsToShow(
   const std::vector<LatticeMatchResult>& allResults,
   const LatticeCell& reference) {
   ResultSelection selection;
   selection.totalResultCount = allResults.size();

   if (allResults.empty()) {
      return selection;
   }

   std::vector<double> distances;
   for (const auto& result : allResults) {
      distances.push_back(result.getP3Distance());
   }

   selection.thresholds = P3RelativeThresholds::calculateAdaptiveP3Thresholds(
      reference.getCell(), reference.getLatticeType(), distances);

   std::vector<LatticeMatchResult> uniqueResults = deduplicateByMatrix(allResults);
   selection.uniqueResultCount = uniqueResults.size();

   const double excellentThreshold = selection.thresholds.excellentThreshold;
   const double goodThreshold = selection.thresholds.goodThreshold;
   const double poorThreshold = selection.thresholds.poorThreshold;

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
         G6 ra;
         Niggli::Reduce(G6(r.getTransformedMobile()), ra);
         for (const auto& kept : out) {
            G6 rb;
            Niggli::Reduce(G6(kept.getTransformedMobile()), rb);
            double diff = 0.0;
            for (int k = 0; k < 6; ++k) diff += std::abs(ra[k] - rb[k]);
            if (diff < 1e-3) { dup = true; break; }
         }
         if (!dup) out.push_back(r);
      }
      v = out;
      };
   deduplicateByMatrix(excellentResults);
   deduplicateByMatrix(goodResults);
   deduplicateByMatrix(poorResults);
   deduplicateByMatrix(terribleResults);

   if (!excellentResults.empty()) {
      for (const auto& r : excellentResults) selection.resultsToShow.push_back(r);
      if (excellentResults.size() <= 2) {
         int goodToAdd = std::min(2, (int)goodResults.size());
         for (int i = 0; i < goodToAdd; ++i) selection.resultsToShow.push_back(goodResults[i]);
      }
   } else if (!goodResults.empty()) {
      for (const auto& r : goodResults) selection.resultsToShow.push_back(r);
      if (goodResults.size() <= 2) {
         int poorToAdd = std::min(2, (int)poorResults.size());
         for (int i = 0; i < poorToAdd; ++i) selection.resultsToShow.push_back(poorResults[i]);
      }
   } else if (!poorResults.empty()) {
      selection.resultsToShow.push_back(poorResults[0]);
   } else if (!terribleResults.empty()) {
      selection.resultsToShow.push_back(terribleResults[0]);
   }

   return selection;
}

// ---------------------------------------------------------------------------
// Per-match quality string and S6 angle
// ---------------------------------------------------------------------------

MatchQuality computeMatchQuality(
   const LatticeMatchResult& result,
   const LRL_Cell& referenceCell,
   const P3RelativeThresholds::P3ThresholdResult& thresholds) {
   MatchQuality quality;

   const double distance = result.getP3Distance();
   const double excellentThreshold = thresholds.excellentThreshold;
   const double goodThreshold = thresholds.goodThreshold;
   const double poorThreshold = thresholds.poorThreshold;

   const double rawDet = result.getTransformationMatrix().Det();
   const int det = static_cast<int>(std::round(std::abs(rawDet)));

   std::string tier;
   if (distance <= excellentThreshold) tier = "EXCELLENT";
   else if (distance <= goodThreshold)      tier = "GOOD";
   else if (distance <= poorThreshold)      tier = "POOR";
   else                                     tier = "DISTORTED";

   if (det > 1) {
      quality.qualityString = "order-" + std::to_string(det) + " supercell  " + tier;
   } else if (std::abs(rawDet) < 0.99) {
      quality.qualityString = "subcell  " + tier;
   } else {
      quality.qualityString = (tier == "EXCELLENT") ? "EQUIVALENT" : tier;
   }

   // S6 angle between the actual matched cells: the reference's original cell
   // (no centering removed, no re-reduction) and the transformed mobile as
   // produced by the matcher. This is the angle a reader sees reflected in
   // the REFERENCE / TRANSFORMED MOBILE lines printed alongside it.
   quality.s6Angle = TransformerUtilities::angleS6(
      S6(result.getTransformedMobile()), S6(referenceCell));
   quality.s6Warning = quality.s6Angle > 15.0;

   return quality;
}

// ---------------------------------------------------------------------------
// SUPERCELL/SUBCELL banner
// ---------------------------------------------------------------------------

void announceSupercellOrSubcell(const std::vector<LatticeMatchResult>& resultsToShow) {
   if (resultsToShow.empty()) return;

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