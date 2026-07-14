#include "DisplayLayer2.h"

#include "DisplayLMP3.h"
#include "DisplayUtilities.h"
#include "LRL_Cell_Degrees.h"
#include "MatchPresentation.h"
#include "Matrix_3x3.h"
#include "MatrixCellOperator.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>

namespace {

   // Print the quality line (qualityString, P3 distance, S6 angle, warning)
   // using the SAME frame whose cells will be printed immediately below it --
   // frame.p3Distance/p3Percent/s6Angle and frame.cell must always travel
   // together, since they were computed from the same cell pair.
   void printQualityLine(const std::string& qualityString, const MatchFrame& frame) {
      std::cout << "; " << qualityString << "  P3 distance: "
         << std::fixed << std::setprecision(3) << frame.p3Distance
         << " (" << std::setprecision(1) << frame.p3Percent << "% of P3 norm)"
         << "  S6 angle: " << std::setprecision(2) << frame.s6Angle << " deg" << std::endl;
      if (frame.s6Angle > 15.0)
         std::cout << "; ** WARNING: S6 angle > 15 deg -- this is likely not a crystallographic match **" << std::endl;
   }

   // Print the primitive-frame REFERENCE / TRANSFORMED MOBILE pair.
   void printPrimitiveFrame(const MatchFrame& frame,
      const MultiTransformFinderControls& controls,
      const LatticeCell& reference,
      const LRL_Cell& primitiveReference) {
      std::cout << "; ";
      outputCellWithCentering(primitiveReference, reference.getLatticeType(), "REFERENCE");
      std::cout << std::endl;

      if (controls.shouldShowDetails()) {
         std::cout << "; DEBUG transformedMobile: " << LRL_Cell_Degrees(frame.cell) << std::endl;
      }

      std::cout << "; ";
      outputCellWithCentering(frame.cell, reference.getLatticeType(), "TRANSFORMED MOBILE");
      std::cout << std::endl;
   }

   // Print the reference-frame REFERENCE / TRANSFORMED MOBILE pair.
   void printReferenceFrame(const MatchFrame& frame,
      const MultiTransformFinderControls& controls,
      const LatticeCell& reference) {
      std::cout << "; ";
      outputCellWithCentering(reference.getCell(), reference.getLatticeType(), "REFERENCE");
      std::cout << std::endl;

      if (controls.shouldShowDetails()) {
         std::cout << "; DEBUG transformedMobile (reference frame): " << LRL_Cell_Degrees(frame.cell) << std::endl;
      }

      std::cout << "; ";
      outputCellWithCentering(frame.cell, reference.getLatticeType(), "TRANSFORMED MOBILE (CONVENTIONAL)");
      std::cout << std::endl;
   }

} // namespace

void displayLayer2Result(const Layer2Result& layer2,
   const MultiTransformFinderControls& controls,
   const LatticeCell& reference,
   const LatticeCell& mobile) {
   const std::vector<LatticeMatchResult>& allResults = layer2.results;
   const LRL_Cell& primitiveReference = layer2.primitiveReference;

   if (controls.shouldShowDetails()) {
      std::cout << "; DEBUG: First 5 results entering displayLayer2Result:" << std::endl;
      for (size_t i = 0; i < std::min(size_t(5), allResults.size()); ++i) {
         std::cout << ";   " << (i + 1) << ": P3=" << allResults[i].getP3Distance() << std::endl;
      }
   }
   if (allResults.empty()) {
      std::cout << "; No results generated - this should never happen!" << std::endl;
      return;
   }

   if (controls.shouldShowDetails()) {
      std::cout << "   ; NC Distance: " << allResults[0].getNcDistance() << std::endl;
      displayP3DistanceHistogram(allResults, controls, reference);
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

   std::cout << "\n; === LATTICE MATCHING RESULTS ===" << std::endl;

   announceSupercellOrSubcell(resultsToShow);

   for (size_t i = 0; i < resultsToShow.size(); ++i) {
      const auto& result = resultsToShow[i];

      std::cout << "\n; --- Match " << (i + 1) << " ---" << std::endl;

      // qualityString (EXCELLENT/GOOD/POOR/DISTORTED, supercell/subcell label)
      // is a property of the match itself -- it is independent of which
      // frame ends up displayed, so it still comes from the matcher's
      // primitive-space P3 distance via computeMatchQuality.
      const MatchQuality quality = computeMatchQuality(result, primitiveReference, thresholds);
      const double rawDet = result.getTransformationMatrix().Det();
      const int det = static_cast<int>(std::round(std::abs(rawDet)));

      const MatchPresentation presentation = buildMatchPresentation(
         layer2, result, reference, thresholds.referenceP3Norm);

      // Decide which frame to show -- entirely this function's call, using
      // only data already on presentation. Both frames were computed
      // unconditionally; nothing upstream made this decision or hid either
      // value from us.
      const bool showReferenceFrame =
         presentation.referenceFrame.p3Percent <= controls.getConventionalThreshold();

      const MatchFrame& shownFrame = showReferenceFrame
         ? presentation.referenceFrame : presentation.primitive;

      printQualityLine(quality.qualityString, shownFrame);

      if (controls.shouldShowDetails()) {
         std::cout << "; Description: " << result.getDescription() << std::endl;
         std::cout << "; DEBUG primitive frame: P3=" << presentation.primitive.p3Distance
            << " (" << presentation.primitive.p3Percent << "%)  S6=" << presentation.primitive.s6Angle << " deg" << std::endl;
         std::cout << "; DEBUG reference frame: P3=" << presentation.referenceFrame.p3Distance
            << " (" << presentation.referenceFrame.p3Percent << "%)  S6=" << presentation.referenceFrame.s6Angle << " deg" << std::endl;
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

      flagCenteringRelationship(result, reference, mobile);

      if (showReferenceFrame) {
         printReferenceFrame(shownFrame, controls, reference);
      } else {
         // Only print the "primitive instead" note if centering differs
         const bool bothPrimitive = (reference.getLatticeType() == "P" &&
            mobile.getLatticeType() == "P");
         if (!bothPrimitive) {
            std::cout << "; NOTE: reference-frame match ("
               << std::fixed << std::setprecision(3)
               << presentation.referenceFrame.p3Percent
               << "% of P3 norm) exceeds threshold "
               << controls.getConventionalThreshold()
               << "% -- showing primitive match instead" << std::endl;
         } else {
            std::cout << "; NOTE: P3 distance ("
               << std::fixed << std::setprecision(3)
               << presentation.referenceFrame.p3Percent
               << "% of P3 norm) exceeds threshold "
               << controls.getConventionalThreshold()
               << "% -- match is distorted" << std::endl;
         }
         printPrimitiveFrame(shownFrame, controls, reference, primitiveReference);
      }
   }

   if (controls.shouldShowDetails() && allResults.size() > resultsToShow.size()) {
      const size_t nHidden = allResults.size() - resultsToShow.size();
      std::cout << "; " << nHidden << " additional match"
         << (nHidden == 1 ? "" : "es") << " not shown (display limit reached)." << std::endl;
   }
}

