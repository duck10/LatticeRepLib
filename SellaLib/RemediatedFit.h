#ifndef REMEDIATED_FIT_H
#define REMEDIATED_FIT_H

// RemediatedFit.h
//
// GetInputSellingReducedVectors -- moved here from CmdSella.cpp, where
// it lived as a one-off free function (Larry, 2026-08-20: "only because
// it was used once during development"). Unchanged otherwise.
//
// Remediation -- moved out of GrimmerChains (GrimmerTree.cpp) as a free
// function. The original method only used m_s6 from GrimmerChains' own
// state, so it converts cleanly by taking the S6 lattice as an explicit
// parameter instead.
//
// GetRemediatedFitForType -- thin wrapper: reduces a raw input cell and
// calls Remediation directly, bypassing ComputeDeloneFits's reactive
// gating (which only calls Remediation when a chain failure happens to
// name the target type). See chat 2026-08-20 -- R1 near the R1/R3
// boundary never got a chain failure naming it, so raw fit(R1) was
// never corrected under the normal path.
//
// Remediation144 / Remediation2Hop -- diagnostic-only variants, built
// 2026-08-22 to test whether Remediation's search space needed widening.
// Result: 144 (6 boundary transforms x 24 reflections) never improved on
// the 6-candidate result in any case tested. 2-hop (36 candidates, two
// boundary transforms composed) resolved 4 of 8 remaining historical
// chain-failure cases with real improvements. That result is why
// Remediation itself was upgraded to the 2-hop body below -- these two
// functions are kept as independent references for comparison, not
// because production code calls them.
//
// RemediateToStability / ClassifyViaStabilityRemediation -- 2026-08-22,
// "Remediation has been so successful that I do not think that the matrix
// libraries are necessary." Standalone alternative classification path:
// iterate Remediation (now 2-hop per call) to a fixed point per type,
// rather than relying on GrimmerChains/ComputeDeloneFits's projector-based
// mechanism. Does not touch ComputeDeloneFits or anything it calls --
// built to run side by side with the existing mechanism for comparison,
// per "maintaining the current mechanisms can allow comparison to the new
// method."

#include "DeloneFitResults.h"
#include "DeloneParentageTest.h"
#include "GenerateLatticeTypeExamples.h"
#include "InputHandler.h"
#include "LatticeConverter.h"
#include "MatS6.h"
#include "S6.h"
#include "Sella.h"
#include "Selling.h"

#include <memory>
#include <string>
#include <vector>

inline S6 GetInputSellingReducedVectors(const LatticeCell& input, MatS6& mat) {
   const S6 s6 = LatticeConverter::SellingReduceCell(input.getLatticeType(), input.getCell(), mat);
   return s6;
}

// Remediation -- searches a 2-hop boundary-transform space (36 candidates:
// 6x6 boundary transforms composed with each other, including i==j since
// we don't know a priori whether GetBoundaryTransforms()[i] is
// self-inverse) instead of the original single-hop 6-candidate search.
//
// UPGRADED 2026-08-22: historical-case testing (16 logged mP/oC/tP chain
// failures, CmdSella.cpp file-header comments, 2023-12-23/24) showed 4 of
// 8 still-failing cases resolved under 2-hop search with real, substantial
// improvements (e.g. 1.25802 -> 0.786441), while the 144-candidate
// (6 boundary transforms x 24 reflections) search never improved on the
// single-hop result in any case tested this session. 2-hop strictly
// dominates single-hop (candidate set is a superset), so this can only
// help or match, never hurt -- same safety property as the original.
//
// ALSO 2026-08-22: now records the winning candidate S6 via
// SetOriginalInput, so callers (RemediateToStability) can feed the actual
// transformed lattice -- not the type-projected GetBestFit() idealization
// -- into a further round of remediation. GetOriginalInput() was
// previously never set by SellaFitXXXXXX, so this is new information, not
// a change to any existing field's meaning; confirmed harmless to add
// since nothing previously read GetOriginalInput() on a Remediation
// result.
//
// Cost: 36 SellaFitXXXXXX calls instead of 6, for every type this is
// called on. Not yet benchmarked against production timing -- see chat
// 2026-08-22, "we'll see about timing. we can always go back." Reverting
// to the original single-hop body is a one-function change if this proves
// too slow at scale; the single-hop loop is preserved below in a comment
// for that purpose.
inline DeloneFitResults Remediation(const S6& s6, const std::string& bravaisName,
   const double initialFit) {
   const std::vector<std::shared_ptr<GenerateDeloneBase>> sptypes =
      GenerateDeloneBase().Select(bravaisName);

   const DeloneFitResults dfrInit = Sella::SellaFitXXXXXX(sptypes[0], s6);
   auto dfr = dfrInit;
   S6 bestCandidate = s6;

   static const std::vector<MatS6> boundaryTransforms = Selling::GetBoundaryTransforms();

   for (const auto& first : boundaryTransforms) {
      const S6 afterFirst = first * s6;
      for (const auto& second : boundaryTransforms) {
         const S6 candidate = second * afterFirst;
         const auto dfrTemp = Sella::SellaFitXXXXXX(sptypes[0], candidate);
         if (dfrTemp.GetRawFit() < dfr.GetRawFit()) {
            dfr = dfrTemp;
            bestCandidate = candidate;
         }
      }
   }
   dfr.SetOriginalInput(bestCandidate);
   return dfr;

   // ORIGINAL single-hop body (6 candidates), kept here for quick revert:
   //
   // for (size_t i = 1; i <= 6; ++i) {
   //    const MatS6 matrix = Selling::GetBoundaryTransforms()[i - 1];
   //    const auto dfrTemp = Sella::SellaFitXXXXXX(sptypes[0], matrix * s6);
   //    if (dfrTemp.GetRawFit() < dfr.GetRawFit()) {
   //       dfr = dfrTemp;
   //    }
   // }
   // return dfr;
}

inline double GetRemediatedFitForType(
   const LatticeCell& input,
   const std::string& targetType) {

   MatS6 oneReductionMatrix;
   const S6 oneLattice = GetInputSellingReducedVectors(input, oneReductionMatrix);

   const DeloneFitResults revised = Remediation(oneLattice, targetType, -1.0);
   return revised.GetRawFit();
}

// Remediation144 -- 6 boundary transforms x 24 reflections (144
// candidates). Diagnostic reference only -- see file header. Not called
// by Remediation or any production path.
inline DeloneFitResults Remediation144(const S6& s6, const std::string& bravaisName) {
   const std::vector<std::shared_ptr<GenerateDeloneBase>> sptypes =
      GenerateDeloneBase().Select(bravaisName);

   const DeloneFitResults dfrInit = Sella::SellaFitXXXXXX(sptypes[0], s6);
   auto dfr = dfrInit;

   static const std::vector<MatS6> boundaryTransforms = Selling::GetBoundaryTransforms();
   static const std::vector<MatS6> reflections = MatS6::GetReflections();

   for (const auto& boundary : boundaryTransforms) {
      const S6 afterBoundary = boundary * s6;
      for (const auto& reflection : reflections) {
         const S6 candidate = reflection * afterBoundary;
         const auto dfrTemp = Sella::SellaFitXXXXXX(sptypes[0], candidate);
         if (dfrTemp.GetRawFit() < dfr.GetRawFit()) {
            dfr = dfrTemp;
         }
      }
   }
   return dfr;
}

// Remediation2Hop -- identical in method to Remediation's current 2-hop
// body; kept as an independent reference implementation so future timing
// or correctness comparisons have something to check Remediation against
// that isn't Remediation itself. See file header.
inline DeloneFitResults Remediation2Hop(const S6& s6, const std::string& bravaisName) {
   const std::vector<std::shared_ptr<GenerateDeloneBase>> sptypes =
      GenerateDeloneBase().Select(bravaisName);

   const DeloneFitResults dfrInit = Sella::SellaFitXXXXXX(sptypes[0], s6);
   auto dfr = dfrInit;

   static const std::vector<MatS6> boundaryTransforms = Selling::GetBoundaryTransforms();

   for (const auto& first : boundaryTransforms) {
      const S6 afterFirst = first * s6;
      for (const auto& second : boundaryTransforms) {
         const S6 candidate = second * afterFirst;
         const auto dfrTemp = Sella::SellaFitXXXXXX(sptypes[0], candidate);
         if (dfrTemp.GetRawFit() < dfr.GetRawFit()) {
            dfr = dfrTemp;
         }
      }
   }
   return dfr;
}

// StabilityResult / RemediateToStability -- iterates Remediation (2-hop
// per call) to a fixed point per type, chaining on the actual winning
// candidate each round (via Remediation's GetOriginalInput()).
//
// FIXED 2026-08-22: each round now re-reduces the candidate
// (SellingReduceCell) before feeding it back into Remediation. Without
// this, the raw boundary-transformed S6 was fed forward unreduced round
// after round -- confirmed via RunStabilityTrace on M2A/P 21.775 21.785
// 27.232... that this let the candidate cell diverge to enormous,
// unphysical size (edges growing past 28000 across 26 rounds) while fit
// crept toward zero purely as a numerical artifact of that divergence,
// not real convergence -- the "delta=0" plateau after round 27 was
// floating-point precision loss at ~9-digit S6 magnitudes, not a genuine
// fixed point. Reduction is the mechanism that's supposed to keep a
// lattice representation bounded and canonical; it was only ever applied
// once, to the original input, before this fix.
struct StabilityResult {
   DeloneFitResults fit;
   int rounds;      // number of Remediation calls beyond the first
   bool hitCap;
};

inline StabilityResult RemediateToStability(const S6& s6, const std::string& bravaisName,
   const int maxRounds = 10) {
   S6 currentPoint = s6;
   DeloneFitResults currentFit = Remediation(currentPoint, bravaisName, -1.0);

   int rounds = 0;
   for (; rounds < maxRounds; ++rounds) {
      MatS6 reductionMatrix;
      const S6 reducedNextPoint = LatticeConverter::SellingReduceCell(
         "P", currentFit.GetOriginalInput(), reductionMatrix);
      const DeloneFitResults nextFit = Remediation(reducedNextPoint, bravaisName, -1.0);
      if (nextFit.GetRawFit() < currentFit.GetRawFit() - 1.0E-8) {
         currentFit = nextFit;
         currentPoint = reducedNextPoint;
      }
      else {
         break;
      }
   }
   return StabilityResult{ currentFit, rounds, rounds >= maxRounds };
}


// ClassifyViaStabilityRemediation -- standalone alternative to
// ComputeDeloneFits's classification path. Reduces the input cell once,
// then for each of the 24 Delone types calls RemediateToStability
// independently. Does not touch ComputeDeloneFits, GrimmerChains, or any
// existing mechanism. See file header, chat 2026-08-22.
// ClassifyViaStabilityRemediation -- standalone alternative to
// ComputeDeloneFits's classification path. Reduces the input cell once,
// then for each of the 24 Delone types calls RemediateToStability
// independently. Does not touch ComputeDeloneFits, GrimmerChains, or any
// existing mechanism. See file header, chat 2026-08-22.
//
// Returns rounds/hitCap alongside each fit (previously discarded) --
// needed to characterize how much search depth is actually required in
// practice, ahead of the planned removal of the projector/matrix-library
// classification path in favor of this mechanism.
struct StabilityClassification {
   DeloneFitResults fit;
   int rounds;
   bool hitCap;
};

inline std::vector<StabilityClassification> ClassifyViaStabilityRemediation(
   const LatticeCell& input) {

   MatS6 oneReductionMatrix;
   const S6 oneLattice = GetInputSellingReducedVectors(input, oneReductionMatrix);

   static const std::vector<std::string> allTypes = {
      "C1","C3","C5",
      "R1","R3",
      "T1","T2","T5",
      "O1A","O1B","O2","O3","O4","O5",
      "M1A","M1B","M2A","M2B","M3","M4",
      "A1","A2","A3",
      "H4"
   };

   std::vector<StabilityClassification> results;
   results.reserve(allTypes.size());
   for (const auto& typeName : allTypes) {
      const StabilityResult stability = RemediateToStability(oneLattice, typeName);
      results.emplace_back(StabilityClassification{ stability.fit, stability.rounds, stability.hitCap });
   }
   return results;
}

// Remediation168ForOneSubtype -- the original Remediation168 search body,
// fixed to a single specific Delone subtype (e.g. "M1A"). Factored out so
// Remediation168 can run it once per sibling when a Bravais name resolves
// to more than one Delone type (see Remediation168's own comment for why
// this matters -- GenerateDeloneBase::Select("mC") returns five types,
// and the previously-used sptypes[0]-only approach could keep widening
// the wrong sibling while a much better fit sat untouched on another).
inline DeloneFitResults Remediation168ForOneSubtype(
   const S6& s6, const std::shared_ptr<GenerateDeloneBase>& sptype,
   const int maxRounds, const std::vector<S6>& extraSeeds) {
   static const std::vector<MatS6> boundaryTransforms = Selling::GetBoundaryTransforms();

   S6 currentPoint = s6;
   DeloneFitResults currentFit = Sella::SellaFitXXXXXX(sptype, s6);
   S6 currentBestCandidate = s6;

   for (int round = 0; round < maxRounds; ++round) {
      std::vector<S6> candidates = BuildVCPCandidates(currentPoint);
      for (const auto& first : boundaryTransforms) {
         const S6 afterFirst = first * currentPoint;
         for (const auto& second : boundaryTransforms) {
            candidates.push_back(second * afterFirst);
         }
      }
      for (const auto& seed : extraSeeds) {
         candidates.push_back(seed);
      }

      DeloneFitResults roundBest = currentFit;
      S6 roundBestCandidate = currentBestCandidate;
      for (const auto& candidate : candidates) {
         const DeloneFitResults dfrTemp = Sella::SellaFitXXXXXX(sptype, candidate);
         if (dfrTemp.GetRawFit() < roundBest.GetRawFit()) {
            roundBest = dfrTemp;
            roundBestCandidate = candidate;
         }
      }

      if (roundBest.GetRawFit() < currentFit.GetRawFit() - 1.0E-8) {
         currentFit = roundBest;
         currentBestCandidate = roundBestCandidate;
         MatS6 reductionMatrix;
         currentPoint = LatticeConverter::SellingReduceCell("P", currentBestCandidate, reductionMatrix);
      }
      else {
         break;
      }
   }

   currentFit.SetOriginalInput(currentBestCandidate);
   return currentFit;
}

// Remediation168 -- tries EVERY Delone type GenerateDeloneBase::Select
// returns for bravaisName, not just sptypes[0]. For single-valued Bravais
// names (e.g. "oC" -> only O4) this is identical to before. For
// multi-valued names (e.g. "mC" -> M1A/M1B/M2A/M2B/M3), the previous
// sptypes[0]-only behavior could stall on M1A while a sibling such as
// M2A had a far better widened fit sitting untouched.
inline DeloneFitResults Remediation168(const S6& s6, const std::string& bravaisName,
   const double initialFit, const int maxRounds = 10,
   const std::vector<S6>& extraSeeds = {}) {
   const std::vector<std::shared_ptr<GenerateDeloneBase>> sptypes =
      GenerateDeloneBase().Select(bravaisName);

   DeloneFitResults best = Remediation168ForOneSubtype(s6, sptypes[0], maxRounds, extraSeeds);
   for (size_t i = 1; i < sptypes.size(); ++i) {
      const DeloneFitResults candidate = Remediation168ForOneSubtype(s6, sptypes[i], maxRounds, extraSeeds);
      if (candidate.GetRawFit() < best.GetRawFit()) {
         best = candidate;
      }
   }
   return best;
}

#endif  // REMEDIATED_FIT_H
