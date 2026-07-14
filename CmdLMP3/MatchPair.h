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
#include "Layer2Result.h"
#include "MultiTransformFinderControls.h"
#include "ProductionLatticeMatcherSystem.h"

#include <iomanip>
#include <iostream>
#include <vector>

struct MatchPairResult {
   Layer2Result layer2;
   LatticeCell  reference;   // cell[0] after any swap
   LatticeCell  mobile;      // cell[1] after any swap
   bool         swapped;
};

inline MatchPairResult matchPair(
   const LatticeCell& cell0,
   const LatticeCell& cell1,
   const MultiTransformFinderControls& controls)
{
   LatticeCell ref = cell0;
   LatticeCell mob = cell1;
   bool swapped = false;

   double volRef = LatticeConverter::MakePrimitiveCell(
      ref.getLatticeType(), ref.getCell()).Volume();
   double volMob = LatticeConverter::MakePrimitiveCell(
      mob.getLatticeType(), mob.getCell()).Volume();
   if (controls.getRunMode() != MultiTransformFinderControls::RunMode::EQUIVALENT) {
      if (volMob > volRef) {
         std::swap(ref, mob);
         std::swap(volMob, volRef);
         swapped = true;
      }
   }
   std::cout << "   ; primitive cell volume ratio, mobile/reference" << std::endl;
   std::cout << std::fixed << std::setprecision(3) <<
      "   ;  ratio = " << volRef / volMob << (swapped ? "   swapped" : "") <<
      std::endl;

   ProductionLatticeMatcherSystem matcher(controls);
   Layer2Result layer2 = matcher.matchSinglePair(ref, mob);
   return { layer2, ref, mob, swapped };
}

#endif // MATCHPAIR_H