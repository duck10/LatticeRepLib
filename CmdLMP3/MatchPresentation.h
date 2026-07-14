#ifndef MATCHPRESENTATION_H
#define MATCHPRESENTATION_H

#include "LatticeCell.h"
#include "LatticeMatchResult.h"
#include "Layer2Result.h"
#include "LRL_Cell.h"

// ---------------------------------------------------------------------------
// MatchFrame -- one self-contained representation of a match: a cell paired
// with the P3 distance and S6 angle computed FROM THAT SAME CELL against its
// counterpart. P3 distance and S6 angle must never be taken from one frame
// and printed next to a cell from a different frame -- that produces numbers
// that do not describe what is on screen (e.g. a primitive-space P3 distance
// of 0.000 printed next to a conventional reference cell and an undecented
// primitive mobile cell, which visibly disagree).
// ---------------------------------------------------------------------------
struct MatchFrame {
   LRL_Cell cell;          // the transformed mobile cell, in this frame
   double   p3Distance = 0.0;  // P3 distance between cell and its reference counterpart
   double   p3Percent = 0.0;   // p3Distance as a percentage of referenceP3Norm
   double   s6Angle = 0.0;     // S6 angle between cell and its reference counterpart
};

// ---------------------------------------------------------------------------
// MatchPresentation -- both available representations of one match, computed
// once, upstream of any display decision. This struct makes NO decision
// about which frame is better or worth showing -- both frames are always
// computed unconditionally and returned as plain values. All judgment
// (which to show, whether to show both, what threshold counts as "good
// enough") belongs entirely to the caller (DisplayLayer2.cpp).
//
// referenceFrame is computed via layer2.referenceFromPrimitive regardless of
// whether that matrix is the identity -- when it is, referenceFrame.cell is
// simply numerically equal to primitive.cell, which is a valid, meaningful
// outcome on its own (no special case needed to produce or interpret it).
// ---------------------------------------------------------------------------
struct MatchPresentation {
   // primitiveReference vs result.getTransformedMobile(), as produced by the
   // matcher, with no additional Niggli reduction of either side (re-reducing
   // both cells before comparing made the S6 angle scale-blind for
   // same-shape-different-size cells).
   MatchFrame primitive;

   // reference.getCell() vs (layer2.referenceFromPrimitive * transformedMobile).
   MatchFrame referenceFrame;
};

// Build both representations for one match. referenceP3Norm is passed in
// (rather than recomputed here) since callers already compute it once for
// quality-tier purposes and it should not be derived twice.
MatchPresentation buildMatchPresentation(
   const Layer2Result& layer2,
   const LatticeMatchResult& result,
   const LatticeCell& reference,
   double referenceP3Norm);

#endif // MATCHPRESENTATION_H
