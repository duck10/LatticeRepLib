#ifndef DISPLAYDISPATCH_H
#define DISPLAYDISPATCH_H

#include "LatticeCell.h"
#include "Layer2Result.h"
#include "MultiTransformFinderControls.h"

// ---------------------------------------------------------------------------
// displayMatchResult -- single entry point for match display.
//
// Reads controls.showOldDisplay() / controls.showNewDisplay() (set via the
// DISPLAYMODE keyword: OLD, NEW, BOTH) and calls displayResults (DisplayLMP3)
// and/or displayLayer2Result (DisplayLayer2) accordingly.
//
// Both CmdLMP3.cpp and FlexibleTestMode.h should call this rather than
// calling displayResults or displayLayer2Result directly, so DISPLAYMODE
// behaves identically in production and test runs.
// ---------------------------------------------------------------------------

void displayMatchResult(const Layer2Result& layer2,
   const MultiTransformFinderControls& controls,
   const LatticeCell& reference,
   const LatticeCell& mobile);

#endif // DISPLAYDISPATCH_H
