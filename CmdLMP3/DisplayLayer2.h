#ifndef DISPLAYLAYER2_H
#define DISPLAYLAYER2_H

#include "LatticeCell.h"
#include "Layer2Result.h"
#include "MultiTransformFinderControls.h"

// ---------------------------------------------------------------------------
// displayLayer2Result -- DISPLAYMODE-aware match display.
//
// DISPLAYMODE OLD  (default): identical output to displayResults (DisplayLMP3.h),
//                              primitive-frame cells throughout.
// DISPLAYMODE NEW:  conventional-frame cells (referenceFromPrimitive applied),
//                    correct centering symbol on both reference and transformed
//                    mobile. Suppressed with a short note per match when the
//                    P3 distance exceeds controls.getConventionalThreshold().
// DISPLAYMODE BOTH: prints the OLD block then the NEW block for each match.
// ---------------------------------------------------------------------------

void displayLayer2Result(const Layer2Result& layer2,
   const MultiTransformFinderControls& controls,
   const LatticeCell& reference,
   const LatticeCell& mobile);

#endif // DISPLAYLAYER2_H
