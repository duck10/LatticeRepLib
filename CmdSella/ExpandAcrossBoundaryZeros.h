#ifndef EXPAND_ACROSS_BOUNDARY_ZEROS_H
#define EXPAND_ACROSS_BOUNDARY_ZEROS_H

// ExpandAcrossBoundaryZeros.h
//
// From a seed that is ALREADY fully reduced (all six S6 components <= 0,
// e.g. built by ExploreReductionTies.h's BuildCharacterExample) but sits
// on one or more genuine zero boundaries, generates every equivalent
// representative reachable by crossing those boundaries.
//
// This is a distinct mechanism from ExploreReductionTies.h's
// ExploreReductionTies function, which forks on TIES AMONG POSITIVE
// components -- modeling ambiguity during ordinary reduction of a
// not-yet-reduced sample. That function returns a seed like this one
// unchanged (nothing positive to trigger it), which is why this second,
// separate function exists.
//
// Uses Selling::GetBoundaryTransforms() -- genuine matrix-vector
// multiplication, remapping all six scalars -- NOT VCPVector::applyVCP,
// which zeros one target component and copies the other five unchanged.
// Per this session's discussion: VCP (Virtual Cartesian Point) is a
// projection onto the boundary, valid for certain measures, but not the
// same operation as a genuine boundary-crossing reflection, and not
// trusted here for generating new equivalent representatives.
//
// Recursion structure mirrors SellaBuild::ApplyVCPSequence's already-sound
// design (re-find zero positions at each level rather than using stale
// slot indices; skip the immediate parent, since each boundary transform
// is self-inverse, to avoid oscillation) -- only the transform mechanics
// differ.
//
// SAFETY CHECK, not an assumption: crossing at a position that is
// genuinely zero is expected (per the SELLA paper, Section 6.3) to land
// on another valid, fully-reduced representative -- but that is checked
// per-result (isReduced) rather than trusted. Anything that comes out
// with a new positive component is rejected and counted in
// rejectedNonReduced rather than silently kept.

#include "MatS6.h"
#include "S6.h"
#include "Selling.h"

#include <cmath>
#include <cstddef>
#include <vector>

struct BoundaryCrossingResult {
   std::vector<S6> representatives;
   size_t rejectedNonReduced = 0;  // see SAFETY CHECK above
};

inline BoundaryCrossingResult ExpandAcrossBoundaryZeros(
   const S6& seed,
   const double tol = 1.0E-8,
   const size_t maxDepth = 6) {

   static const std::vector<MatS6> vm = Selling::GetBoundaryTransforms();

   auto findZeros = [tol](const S6& v) {
      std::vector<size_t> out;
      for (size_t i = 0; i < 6; ++i) if (std::abs(v[i]) < tol) out.push_back(i);
      return out;
   };

   auto isReduced = [](const S6& v) {
      for (size_t i = 0; i < 6; ++i) if (v[i] > 0.0) return false;
      return true;
   };

   BoundaryCrossingResult result;

   auto alreadySeen = [&result, tol](const S6& v) {
      for (const auto& existing : result.representatives) {
         if ((existing - v).norm() < tol) return true;
      }
      return false;
   };

   struct Branch {
      S6 v;
      S6 parent;
      bool hasParent;
      size_t depth;
   };

   std::vector<Branch> frontier{ Branch{ seed, seed, false, 0 } };
   result.representatives.push_back(seed);

   while (!frontier.empty()) {
      std::vector<Branch> nextFrontier;

      for (const auto& branch : frontier) {
         if (branch.depth >= maxDepth) continue;

         for (const size_t z : findZeros(branch.v)) {
            const S6 candidate = vm[z] * branch.v;

            // Skip the single immediately-reversing crossing (each
            // boundary transform is self-inverse) to avoid oscillation --
            // same reasoning as ApplyVCPSequence's parent-check.
            if (branch.hasParent && (candidate - branch.parent).norm() < tol) continue;

            if (!isReduced(candidate)) { ++result.rejectedNonReduced; continue; }
            if (!alreadySeen(candidate)) result.representatives.push_back(candidate);

            nextFrontier.push_back(Branch{ candidate, branch.v, true, branch.depth + 1 });
         }
      }

      frontier = nextFrontier;
   }

   return result;
}

#endif  // EXPAND_ACROSS_BOUNDARY_ZEROS_H
