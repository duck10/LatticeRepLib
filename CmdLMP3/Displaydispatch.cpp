#include "DisplayDispatch.h"

#include "DisplayLMP3.h"
#include "DisplayLayer2.h"

void displayMatchResult(const Layer2Result& layer2,
   const MultiTransformFinderControls& controls,
   const LatticeCell& reference,
   const LatticeCell& mobile) {
   if (controls.showOldDisplay()) {
      displayResults(layer2.results, controls, reference, mobile, layer2.primitiveReference);
   }
   if (controls.showNewDisplay()) {
      displayLayer2Result(layer2, controls, reference, mobile);
   }
}
