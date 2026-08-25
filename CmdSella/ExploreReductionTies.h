#ifndef EXPLORE_REDUCTION_TIES_H
#define EXPLORE_REDUCTION_TIES_H

// ExploreReductionTies.h
//
// Deliberately explores the reduction-tie branches that Selling::Reduce's
// production pivot rule (pick the FIRST index tied for the current max --
// see the strict `>` in Selling::Reduce) silently discards. See this
// session's discussion: the O1B library-coverage gap is not a sampling-
// density problem (confirmed -- 84 randomly-sampled matrices via
// SellaBuild's BUILD path still miss the O1B/O2/O3-boundary fixture) but a
// structural one: O1B has no zero in a GENERIC sample, so
// SellaBuild::Expand/Expand2's boundary-walking never triggers for it at
// all, at any sample count. The only way to reach representatives that
// only exist at/near a boundary is to start from a sample that is already
// at that boundary and walk the tie-branches explicitly.
//
// WHICH TIES GET EXPLORED, WITHIN ONE WALK: all of them, always -- not a
// selected subset. At each step, every component tied for the current max
// (within tol) is forked into its own branch. This needs no type-specific
// knowledge and nothing to configure: away from any boundary, ties
// essentially never occur (two floating-point values matching exactly by
// chance is measure zero), so the walker naturally does nothing extra on
// generic input and only branches where a real near-zero/tied structure
// exists -- which is exactly where the coverage gap lives.
//
// WHICH STARTING SAMPLES (boundaries) TO SEED FROM, ACROSS WALKS: this is
// the part that needs a decision, and it should not be a hand-picked list
// (that is exactly the O1B/O2/O3-by-hand pattern this session moved away
// from). The principled source is DeloneParentageTest.h's
// TestDeloneParentage: for parent type P, every other Delone type C with
// C.GetFreeParams() < P.GetFreeParams() is a candidate boundary
// sub-stratum; test each with TestDeloneParentage(P, C), and for every C
// confirmed nested, seed one walk here with
// BuildCharacterExample(C.GetCharacter()) as the starting sample. That
// makes "which boundaries" a computed answer driven by the confirmed
// parentage graph, not a manually curated one -- and it generalizes
// automatically to the "unnamed adjoining manifolds" case from
// SELLA_handoff.md escalation 1, since it needs no type name, only the
// character string and free-parameter count already on every
// GenerateDeloneBase. Wiring that seeding loop is a separate step from
// this file -- this file only walks from a single given starting sample.

#include "MatS6.h"
#include "S6.h"
#include "Selling.h"

#include <cstddef>
#include <iostream>
#include <vector>

struct BoundaryWalkResult {
   std::vector<S6> terminalRepresentatives;
   size_t droppedBranches = 0;
};

// ExploreReductionTies -- walks every reduction-tie branch from `sample`
// down to termination (all components <= 0), instead of Selling::Reduce's
// single production path. Each branch is capped at maxCyclesPerBranch
// (pathological single-path reductions are known to run to hundreds of
// cycles or loop forever without a cap -- see this session's discussion);
// a branch that hits the cap is dropped and counted in droppedBranches
// rather than kept partially-reduced. Terminal representatives are
// deduped by S6::norm() of their difference, tolerance tol.
inline BoundaryWalkResult ExploreReductionTies(
   const S6& sample,
   const double tol = 1.0E-8,
   const size_t maxCyclesPerBranch = 50) {

   struct Branch {
      S6 v;
      size_t cycles;
   };

   static const std::vector<MatS6> vm = Selling::GetBoundaryTransforms();

   auto hasPositive = [](const S6& v) {
      for (size_t i = 0; i < 6; ++i) if (v[i] > 0.0) return true;
      return false;
   };

   BoundaryWalkResult result;

   auto alreadySeen = [&result, tol](const S6& v) {
      for (const auto& existing : result.terminalRepresentatives) {
         if ((existing - v).norm() < tol) return true;
      }
      return false;
   };

   std::vector<Branch> frontier{ Branch{ sample, 0 } };

   while (!frontier.empty()) {
      std::vector<Branch> nextFrontier;

      for (const auto& branch : frontier) {
         if (!hasPositive(branch.v)) {
            if (!alreadySeen(branch.v)) result.terminalRepresentatives.push_back(branch.v);
            continue;
         }
         if (branch.cycles >= maxCyclesPerBranch) {
            ++result.droppedBranches;
            continue;
         }

         double maxScalar = -1.0;
         for (size_t i = 0; i < 6; ++i) if (branch.v[i] > maxScalar) maxScalar = branch.v[i];

         for (size_t i = 0; i < 6; ++i) {
            if (branch.v[i] > 0.0 && branch.v[i] >= maxScalar - tol) {
               nextFrontier.push_back(Branch{ vm[i] * branch.v, branch.cycles + 1 });
            }
         }
      }

      frontier = nextFrontier;
   }

   if (result.droppedBranches > 0) {
      std::cout << "; WARNING: ExploreReductionTies hit the " << maxCyclesPerBranch
         << "-cycle limit on " << result.droppedBranches
         << " branch(es) -- library coverage may be incomplete for "
         << "representatives reachable only past that depth." << std::endl;
   }

   return result;
}

#endif  // EXPLORE_REDUCTION_TIES_H
