// DeloneFitComputation.h
//
// Single home for the DeloneFitComputation struct and the ComputeDeloneFits
// declaration, shared by CmdSella.cpp (where ComputeDeloneFits is defined)
// and FlexibleSellaTestMode.h (which calls it). Previously both files
// declared this struct independently (matching the pattern Cmdsellatests.cpp
// used, in the absence of a proper CmdSella.h) -- harmless across separate
// translation units, but CmdSella.cpp #includes FlexibleSellaTestMode.h
// directly, so the two copies collided in the same TU (MSVC C2011). Pulling
// it out here is the real fix; there is exactly one definition now, and
// nothing else should re-declare it.
#ifndef DELONE_FIT_COMPUTATION_H
#define DELONE_FIT_COMPUTATION_H

#include "CmdSellaControls.h"
#include "DeloneFitResults.h"
#include "GrimmerTree.h"
#include "LatticeCell.h"
#include "Sella.h"

#include <vector>

// Bundles what ComputeDeloneFits produces: the fit results themselves, and
// the final (post-remediation) GrimmerChains object, which callers such as
// ProcessSella and FlexibleSellaTestRunner both need afterward.
struct DeloneFitComputation {
   std::vector<DeloneFitResults> fits;
   GrimmerChains chains;
};

DeloneFitComputation ComputeDeloneFits(
   const LatticeCell& input,
   const CmdSellaControls& controls,
   Sella& sella);

#endif // DELONE_FIT_COMPUTATION_H
