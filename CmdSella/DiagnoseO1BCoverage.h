#ifndef DIAGNOSE_O1B_COVERAGE_H
#define DIAGNOSE_O1B_COVERAGE_H

// DiagnoseO1BCoverage.h
//
// Follow-up prompted by this session's real-data finding: candidates A and
// B (nearby physical lattices, per cmddists' NCDist/CS6Dist) reduced to
// two DIFFERENT orientations near the O1B/O2/O3 triple boundary (confirmed
// via cmddelone -- A pairs (v0,v4)/(v1,v3), B pairs (v0,v3)/(v1,v4)). A's
// orientation fits O2 well (residual 0.013, of 6 stored O2 projectors).
// B's orientation fits O1B well (residual 0.065) but O1B has only 1
// stored projector, versus O2's 6 (confirmed complete across all 24
// reflections earlier this session) and O3's 9. A library with only 1
// entry out of up to 24 possible orientations is a strong candidate for
// being genuinely incomplete -- this checks that directly.
//
// Unlike DiagnoseO2Gap.h, this does NOT use character-string clustering
// (CheckDeloneProjectors::DoesProjectorAgreeWithCharacter). O1B's
// character has no zero slots (fully generic, all six components
// independent within the reduction constraint), so there's no clean
// equality-pattern to cluster on the way O2/O3's forced-zero slots
// provide. Uses the same fixed-point RESIDUAL approach validated in
// DiagnoseBoundaryCandidates.h instead (confirmed there to reproduce
// SellaFit's own reported numbers almost exactly).
//
// Seeded from candidate B directly (a real, confirmed-good O1B point from
// actual CmdSella output this session) rather than a synthetic generator,
// since B is already known-good and avoids introducing a new unverified
// seed-generation assumption.

#include "LabeledSellaMatrices.h"
#include "MatS6.h"
#include "S6.h"

#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <vector>

inline void DiagnoseO1BCoverage() {
   const S6 seed("-38.11642 -43.36206 -0.01851 -38.17840 -43.42403 -0.04436");  // candidate B

   const std::vector<MatS6> knownO1B = LabeledSellaMatrices::CreatePrjs_O1B().GetMatrices();
   const std::vector<MatS6> reflections = MatS6::GetReflections();  // 24 matrices

   std::cout << "O1B coverage check: " << knownO1B.size()
      << " stored projector(s), 24 orientations of seed B tested" << std::endl;
   std::cout << std::fixed << std::setprecision(5);

   size_t wellFitCount = 0;
   const double wellFitThreshold = 0.5;  // loose threshold; residuals for a
   // genuinely covered orientation
   // should look like B's own 0.065,
   // not merely "small-ish"

   for (size_t i = 0; i < reflections.size(); ++i) {
      const S6 oriented = reflections[i] * seed;

      double bestResidual = -1.0;
      size_t bestIndex = 0;
      for (size_t p = 0; p < knownO1B.size(); ++p) {
         const S6 projected = knownO1B[p] * oriented;
         const double residual = (projected - oriented).norm();
         if (bestResidual < 0.0 || residual < bestResidual) {
            bestResidual = residual;
            bestIndex = p;
         }
      }

      std::cout << "  [" << i << "] " << oriented
         << " best O1B residual=" << bestResidual
         << " (projector " << bestIndex << ")"
         << (bestResidual < wellFitThreshold ? "  WELL-FIT" : "  POORLY-FIT")
         << std::endl;

      if (bestResidual < wellFitThreshold) ++wellFitCount;
   }

   std::cout.unsetf(std::ios::floatfield);
   std::cout << wellFitCount << " of 24 orientations well-fit by the "
      << knownO1B.size() << " stored O1B projector(s)." << std::endl;
   if (wellFitCount < 24) {
      std::cout << "This is consistent with an incomplete O1B library: most orientations "
         << "of a physically valid O1B lattice are NOT well described by the single "
         << "stored projector." << std::endl;
   }
}

#endif  // DIAGNOSE_O1B_COVERAGE_H
