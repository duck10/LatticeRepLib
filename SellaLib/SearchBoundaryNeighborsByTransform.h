#ifndef SEARCH_BOUNDARY_NEIGHBORS_BY_TRANSFORM_H
#define SEARCH_BOUNDARY_NEIGHBORS_BY_TRANSFORM_H

// SearchBoundaryNeighborsByTransform.h
//
// (see file history for the full design discussion: interior sampling
// + 6 boundary transforms x 24 reflections, A1/A2/A3 built-in oracle,
// confirmed-complete A2/A3 zero patterns via 24-reflection enumeration,
// chat 2026-08-20.)
//
// REVISED 2026-08-20: replaced the "one closest OTHER type per seed"
// summary with StoreResults<std::string, BoundaryTieSample>, keyed by
// unordered type-pair name. The single-closest approach silently lost
// information whenever A1 (or any trivial/near-trivial match) won a
// tie at residual 0 against a genuine finding -- StoreResults gives
// every distinct pair its own bucket, so nothing gets hidden behind a
// tie-break. A1 is excluded from being stored as a pair member (it
// matches EVERY candidate unconditionally -- see AssertA1A2A3Invariants
// -- so including it would flood the table with 34,560 content-free
// "X|A1" entries). A2/A3 are NOT excluded by default -- their matches
// are conditional on the transformed point's actual zero-pattern
// (confirmed via 24-reflection enumeration, not assumed), so a
// candidate pairing with A2/A3 is a real, specific finding about that
// candidate, not guaranteed noise the way A1 is. Toggle via
// excludeFromPairing if they turn out to clutter the table in practice.

#include "DeloneParentageTest.h"        // GetValidTypeSample
#include "ExploreReductionTies.h"
#include "GenerateLatticeTypeExamples.h"
#include "LRL_Cell.h"
#include "S6.h"
#include "Selling.h"
#include "StoreResults.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

// CountNearZeroComponents -- number of S6 components within tol of 0.
inline int CountNearZeroComponents(const S6& v, const double tol) {
   int count = 0;
   for (int i = 0; i < 6; ++i) {
      if (std::abs(v[i]) < tol) ++count;
   }
   return count;
}

// MatchesA2ZeroPattern / MatchesA3ZeroPattern -- confirmed by direct
// 24-reflection enumeration (chat 2026-08-20, cmddelone | cmds6refl on
// real A2/A3 seeds), NOT by assumption from the character string alone.
// A2: exactly one near-zero component, at ANY position (6 stored
// orientations, each hit exactly 4/24 reflections, zero uncovered).
// A3: near-zeros at exactly one antipodal pair -- (0,3), (1,4), (2,5)
// -- NOT any 2-of-6 pattern (3 stored orientations, each hit exactly
// 8/24 reflections, zero uncovered).
inline bool MatchesA2ZeroPattern(const S6& v, const double tol) {
   return CountNearZeroComponents(v, tol) == 1;
}
inline bool MatchesA3ZeroPattern(const S6& v, const double tol) {
   auto isZero = [&](int i) { return std::abs(v[i]) < tol; };
   return (isZero(0) && isZero(3)) ||
      (isZero(1) && isZero(4)) ||
      (isZero(2) && isZero(5));
}

// ResidualAgainstType -- smallest residual of v against ANY stored
// orientation (GetSellaProjectors()) of one type.
inline double ResidualAgainstType(const S6& v, const GenerateDeloneBase& type) {
   double best = -1.0;
   for (const auto& prj : type.GetSellaProjectors()) {
      const double r = (prj * v - v).norm();
      if (best < 0.0 || r < best) best = r;
   }
   return best;
}

struct TypeResidual {
   std::string typeName;
   double residual;
};

inline std::vector<TypeResidual> RankAllTypesByResidual(
   const S6& v,
   const std::vector<std::shared_ptr<GenerateDeloneBase>>& allTypes) {

   std::vector<TypeResidual> ranked;
   ranked.reserve(allTypes.size());
   for (const auto& t : allTypes) {
      ranked.push_back({ t->GetName(), ResidualAgainstType(v, *t) });
   }
   std::sort(ranked.begin(), ranked.end(),
      [](const TypeResidual& a, const TypeResidual& b) { return a.residual < b.residual; });
   return ranked;
}

// AssertA1A2A3Invariants -- built-in correctness oracle, run on every
// candidate. A1 must always match (S_A1 = V, unconditional). A2/A3
// must match iff the candidate's own zero-pattern satisfies their
// confirmed-complete pattern above -- NOT "any >=1 or >=2 zeros"
// (that version produced false positives -- see chat 2026-08-20).
inline bool AssertA1A2A3Invariants(
   const S6& candidate,
   const std::vector<TypeResidual>& ranked,
   const double matchTol) {

   bool ok = true;
   auto residualOf = [&](const std::string& name) -> double {
      for (const auto& tr : ranked) if (tr.typeName == name) return tr.residual;
      return -1.0;
      };

   const double rA1 = residualOf("A1");
   if (rA1 < 0.0 || rA1 >= matchTol) {
      std::cout << "; INVARIANT VIOLATION: A1 residual=" << rA1
         << " for candidate=" << candidate << std::endl;
      ok = false;
   }

   if (MatchesA2ZeroPattern(candidate, matchTol)) {
      const double rA2 = residualOf("A2");
      if (rA2 < 0.0 || rA2 >= matchTol) {
         std::cout << "; INVARIANT VIOLATION: candidate matches A2 zero-pattern but"
            << " A2 residual=" << rA2 << ", candidate=" << candidate << std::endl;
         ok = false;
      }
   }
   if (MatchesA3ZeroPattern(candidate, matchTol)) {
      const double rA3 = residualOf("A3");
      if (rA3 < 0.0 || rA3 >= matchTol) {
         std::cout << "; INVARIANT VIOLATION: candidate matches A3 zero-pattern but"
            << " A3 residual=" << rA3 << ", candidate=" << candidate << std::endl;
         ok = false;
      }
   }

   return ok;
}

// BuildBoundaryReflectionCandidates -- 6 boundary transforms x 24
// reflections = 144 candidates. Known redundant (only 24 distinct per
// boundary-transform structure) -- kept deliberately, per Larry:
// "144 is safe, perhaps excessively safe, but safe."
inline std::vector<S6> BuildBoundaryReflectionCandidates(const S6& v) {
   static const std::vector<MatS6> boundaryTransforms = Selling::GetBoundaryTransforms();
   static const std::vector<MatS6> reflections = MatS6::GetReflections();

   std::vector<S6> candidates;
   candidates.reserve(boundaryTransforms.size() * reflections.size());

   for (const auto& Ri : boundaryTransforms) {
      const S6 crossed = Ri * v;
      for (const auto& M : reflections) {
         candidates.push_back(M * crossed);
      }
   }
   return candidates;
}

// MakeUnorderedPairKey -- canonical "A|B" (alphabetically sorted) so
// (typeX, typeY) and (typeY, typeX) collapse to the same StoreResults
// key regardless of which side was the seed.
inline std::string MakeUnorderedPairKey(const std::string& a, const std::string& b) {
   return (a < b) ? (a + "|" + b) : (b + "|" + a);
}

// One stored example of a genuine (non-self, non-excluded) near-match,
// kept for provenance.
struct BoundaryTieSample {
   std::string seedType;
   std::string matchedType;
   double residual;
   S6 candidate;
};

inline std::ostream& operator<<(std::ostream& os, const BoundaryTieSample& s) {
   os << "seed=" << s.seedType << " matched=" << s.matchedType
      << " residual=" << s.residual << " candidate=" << s.candidate;
   return os;
}

// SearchAllTypesBoundaryNeighbors -- full pipeline. For every type,
// draws nSamplesPerType random valid samples, builds the 144-candidate
// set for each, runs the A1/A2/A3 oracle, and stores every genuine
// (non-self, non-excluded) near-match into a StoreResults table keyed
// by unordered type-pair -- so every distinct pair gets its own bucket,
// with a bounded number of example candidates and a running count of
// how often it was seen across the whole run. Returns pairResults so a
// caller (e.g. FindMissingBoundaryTies) can inspect it afterward.
inline StoreResults<std::string, BoundaryTieSample> SearchAllTypesBoundaryNeighbors(
   const int nSamplesPerType = 10,
   const double matchTol = 1.0E-8,
   const std::set<std::string>& excludeFromPairing = { "A1" },
   const size_t examplesPerPairToKeep = 5) {

   const auto allTypes = GenerateLatticeTypeExamples::CreateListOfDeloneTypes();
   std::cout << "; SearchAllTypesBoundaryNeighbors: " << allTypes.size()
      << " Delone types, " << nSamplesPerType << " sample(s) each, 144 candidates/sample"
      << std::endl;

   StoreResults<std::string, BoundaryTieSample> pairResults(examplesPerPairToKeep);
   pairResults.SetTitle("Boundary tie candidates by type pair");
   pairResults.SetKeyLabel("TypePair");

   size_t totalInvariantViolations = 0;
   size_t totalCandidates = 0;

   for (const auto& seedTypePtr : allTypes) {
      const std::string seedName = seedTypePtr->GetName();

      int rejectedInvalidSamples = 0;
      for (int s = 0; s < nSamplesPerType; ++s) {
         const S6 sample = GetValidTypeSample(seedTypePtr->GetPrj());
         if (!LRL_Cell(sample).IsValid()) {
            ++rejectedInvalidSamples;
            continue;
         }

         const auto candidates = BuildBoundaryReflectionCandidates(sample);

         for (const auto& candidate : candidates) {
            ++totalCandidates;

            const auto ranked = RankAllTypesByResidual(candidate, allTypes);
            if (!AssertA1A2A3Invariants(candidate, ranked, matchTol)) {
               ++totalInvariantViolations;
            }

            for (const auto& tr : ranked) {
               if (tr.typeName == seedName) continue;
               if (excludeFromPairing.count(tr.typeName)) continue;
               if (tr.residual >= matchTol) break;  // ranked ascending -- no more matches past here

               BoundaryTieSample sample_;
               sample_.seedType = seedName;
               sample_.matchedType = tr.typeName;
               sample_.residual = tr.residual;
               sample_.candidate = candidate;

               pairResults.Store(MakeUnorderedPairKey(seedName, tr.typeName), sample_);
            }
         }
      }

      if (rejectedInvalidSamples > 0) {
         std::cout << "; " << seedName << ": " << rejectedInvalidSamples << "/"
            << nSamplesPerType << " sample(s) rejected as invalid" << std::endl;
      }
   }

   std::cout << "; SearchAllTypesBoundaryNeighbors complete: " << totalCandidates
      << " total candidates, " << totalInvariantViolations
      << " invariant violation(s), " << pairResults.size()
      << " distinct type-pair(s) with a genuine near-match" << std::endl;

   pairResults.ShowResults();
   return pairResults;
}

#endif  // SEARCH_BOUNDARY_NEIGHBORS_BY_TRANSFORM_H
