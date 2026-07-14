#include "MatchPresentation.h"

#include "Matrix_3x3.h"
#include "MatrixCellOperator.h"
#include "S6.h"
#include "TransformerUtilities.h"

namespace {

   MatchFrame buildFrame(const LRL_Cell& cell, const LRL_Cell& referenceCell, double referenceP3Norm) {
      MatchFrame frame;
      frame.cell = cell;
      frame.p3Distance = TransformerUtilities::calculateP3Distance(cell, referenceCell);
      frame.p3Percent = (referenceP3Norm > 0.0)
         ? (frame.p3Distance / referenceP3Norm * 100.0) : 0.0;
      frame.s6Angle = TransformerUtilities::angleS6(S6(cell), S6(referenceCell));
      return frame;
   }

} // namespace

MatchPresentation buildMatchPresentation(
   const Layer2Result& layer2,
   const LatticeMatchResult& result,
   const LatticeCell& reference,
   double referenceP3Norm) {
   MatchPresentation presentation;

   // Primitive frame: matcher's natural working frame, no re-reduction of
   // either side.
   presentation.primitive = buildFrame(
      result.getTransformedMobile(), layer2.primitiveReference, referenceP3Norm);

   // Reference frame: always computed, unconditionally. When
   // layer2.referenceFromPrimitive is the identity, this cell is simply
   // numerically equal to the primitive frame's cell -- a valid outcome
   // requiring no special case.
   const LRL_Cell referenceFrameCell =
      layer2.referenceFromPrimitive * result.getTransformedMobile();
   presentation.referenceFrame = buildFrame(
      referenceFrameCell, reference.getCell(), referenceP3Norm);

   return presentation;
}
