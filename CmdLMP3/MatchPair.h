#ifndef MATCHPAIR_H
#define MATCHPAIR_H

// ---------------------------------------------------------------------------
// matchPair -- canonical entry point for matching two lattice cells.
//
// Applies the volume-based swap (larger primitive volume = reference) so that
// supercell matrices (det>1) can expand the mobile to match the reference.
// The swap is suppressed in EQUIVALENT mode to preserve user reference choice.
//
// Used by both runInputListMode (CmdLMP3.cpp) and FlexibleTestRunner so that
// test behavior exactly matches production behavior.
// ---------------------------------------------------------------------------

#include "LatticeCell.h"
#include "LatticeConverter.h"
#include "LatticeMatchResult.h"
#include "MultiTransformFinderControls.h"
#include "ProductionLatticeMatcherSystem.h"

#include <iostream>
#include <vector>

struct MatchPairResult {
   std::vector<LatticeMatchResult> results;
   LatticeCell                     reference;   // cell[0] after any swap
   LatticeCell                     mobile;      // cell[1] after any swap
   bool                            swapped;
};

inline MatchPairResult matchPair(
   const LatticeCell& cell0,
   const LatticeCell& cell1,
   const MultiTransformFinderControls& controls)
{
   LatticeCell ref = cell0;
   LatticeCell mob = cell1;
   bool swapped = false;

   if (controls.getRunMode() != MultiTransformFinderControls::RunMode::EQUIVALENT) {
      const double vol0 = LatticeConverter::MakePrimitiveCell(
         ref.getLatticeType(), ref.getCell()).Volume();
      const double vol1 = LatticeConverter::MakePrimitiveCell(
         mob.getLatticeType(), mob.getCell()).Volume();
      if (vol1 > vol0) {
         std::swap(ref, mob);
         swapped = true;
      }
   }

   ProductionLatticeMatcherSystem matcher(controls);
   const std::vector<LatticeCell> pair = { ref, mob };
   return { matcher.processInputList(pair), ref, mob, swapped };
}

#endif // MATCHPAIR_H
