#ifndef TEST_HARNESS_H
#define TEST_HARNESS_H

// TestHarness.h
//
// Standalone diagnostic / research tools for the SELLA boundary-tie
// investigation -- NOT production code, NOT called from normal
// COMPARE/ProcessSella operation (RunNestingConsistencyCheck is the one
// exception: it's wired to the CHECKNESTING control flag rather than a
// swap-into-main()-and-exit call, but it's still diagnostic, not part
// of the real fit pipeline). Each function is meant to be called once,
// read from console output -- see CLAUDE.md for what each one is FOR
// and what it found.
//
// Consolidated here 2026-08-20 from CmdSella.cpp.

#include "CmdSellaControls.h"
#include "DeloneFitComputation.h"
#include "DeloneFitResults.h"
#include "DeloneParentageTest.h"
#include "ExpandAcrossBoundaryZeros.h"
#include "GenerateLatticeTypeExamples.h"
#include "InputHandler.h"
#include "LRL_Cell.h"
#include "MatS6.h"
#include "RemediatedFit.h"
#include "S6.h"
#include "Sella.h"
#include "Selling.h"
#include "SearchBoundaryNeighborsByTransform.h"
#include "StoreResults.h"

#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

// RunParentageRepeatabilityCheck -- one-off calibration diagnostic.
// TestDeloneParentage draws a fresh S6::rand() every call; this checks
// whether isNested and residual are stable across different seeds/trials
// for one known-nested and one known-not-nested pair, before trusting
// SearchAllBoundaryTies's output. See SELLA handoff 2026-08-20 session 2,
// Section 4 item 4.
void RunParentageRepeatabilityCheck() {
   std::cout << "; RunParentageRepeatabilityCheck: entered" << std::endl;

   std::cout << "; -- known-nested pair: O1B / O3 --" << std::endl;
   for (int seed = 1; seed <= 3; ++seed) {
      S6::SetSeed(seed);
      for (int trial = 0; trial < 5; ++trial) {
         const auto result = TestDeloneParentage(Delone_O1B(), Delone_O3());
         std::cout << "; seed=" << seed << " trial=" << trial
            << " isNested=" << result.isNested
            << " residual=" << result.residual << std::endl;
      }
   }

   std::cout << "; -- known-NOT-nested pair (same freeParams): M2B / M3 --" << std::endl;
   for (int seed = 1; seed <= 3; ++seed) {
      S6::SetSeed(seed);
      for (int trial = 0; trial < 5; ++trial) {
         const auto result = TestDeloneParentage(Delone_M2B(), Delone_M3());
         std::cout << "; seed=" << seed << " trial=" << trial
            << " isNested=" << result.isNested
            << " residual=" << result.residual << std::endl;
      }
   }

   std::cout << "; RunParentageRepeatabilityCheck: complete" << std::endl;
}

// FindMissingBoundaryTies -- compares the full set of incomparable
// (mutually non-nested) type pairs against what SearchAllTypesBoundary-
// Neighbors actually found, and reports every pair that is a candidate
// (incomparable) but never showed up as a genuine near-match. R1/R3
// (chat 2026-08-20) is the first confirmed instance of this gap -- this
// checks systematically whether it's the only one.
inline void FindMissingBoundaryTies(
   const StoreResults<std::string, BoundaryTieSample>& pairResults,
   const double tol = 1.0E-8) {

   const auto allTypes = GenerateLatticeTypeExamples::CreateListOfDeloneTypes();
   std::vector<std::string> foundKeys = pairResults.GetKeys();
   std::set<std::string> foundSet(foundKeys.begin(), foundKeys.end());

   size_t totalPairs = 0;
   size_t nestedPairs = 0;
   size_t incomparablePairs = 0;
   std::vector<std::string> missing;

   for (size_t i = 0; i < allTypes.size(); ++i) {
      for (size_t j = i + 1; j < allTypes.size(); ++j) {
         ++totalPairs;
         const auto& A = *allTypes[i];
         const auto& B = *allTypes[j];

         // Nested either direction? Skip -- not a tie candidate at all.
         const bool aNestsB = TestDeloneParentage(B, A, tol).isNested;  // A child of B
         const bool bNestsA = TestDeloneParentage(A, B, tol).isNested;  // B child of A
         if (aNestsB || bNestsA) {
            ++nestedPairs;
            continue;
         }

         ++incomparablePairs;
         const std::string key = MakeUnorderedPairKey(A.GetName(), B.GetName());
         if (!foundSet.count(key)) {
            missing.push_back(key);
         }
      }
   }

   std::cout << "; FindMissingBoundaryTies: " << totalPairs << " total pairs, "
      << nestedPairs << " nested (skipped), " << incomparablePairs
      << " incomparable (candidates), " << foundSet.size()
      << " found by search, " << missing.size() << " MISSING" << std::endl;

   for (const auto& m : missing) {
      std::cout << ";   MISSING: " << m << std::endl;
   }
}
// RunBoundaryTieSearchAndGapCheck -- standalone driver: runs the search,
// then separately checks its results against the full set of
// incomparable type pairs to find any that were never surfaced (R1/R3,
// chat 2026-08-20, was the first confirmed instance -- this checks
// whether there are others).
inline void RunBoundaryTieSearchAndGapCheck(
   const int nSamplesPerType = 10,
   const double matchTol = 1.0E-8) {

   const auto pairResults = SearchAllTypesBoundaryNeighbors(nSamplesPerType, matchTol);
   FindMissingBoundaryTies(pairResults, matchTol);
}

// SearchTwoHopBoundaryNeighbors -- tests whether a type's missing
// boundary partners are reachable via a SECOND boundary crossing, not
// a first. Motivated directly by the R1/C5/R3 case (chat 2026-08-20):
// C5 is R1's direct child (rrs->rrr substitution), and C5 ties to R3
// directly (one hop) -- so R1 and R3 may only connect THROUGH C5, a
// relationship this function can find but the one-hop search cannot.
//
// Mechanism: for each of seedType's first-hop candidates (144, already
// what SearchAllTypesBoundaryNeighbors computes), apply the SAME
// boundary-transform-plus-reflection construction again, producing a
// second generation of candidates. Check those against every type.
// nSamples x 144 x 144 candidates total -- kept small (nSamples default
// 3) since this is a targeted, expensive test, not a full 24-type run.
inline void SearchTwoHopBoundaryNeighbors(
   const std::string& seedTypeName,
   const int nSamples = 3,
   const double matchTol = 1.0E-8) {

   const auto allTypes = GenerateLatticeTypeExamples::CreateListOfDeloneTypes();

   std::shared_ptr<GenerateDeloneBase> seedTypePtr;
   for (const auto& t : allTypes) {
      if (t->GetName() == seedTypeName) { seedTypePtr = t; break; }
   }
   if (!seedTypePtr) {
      std::cout << "; SearchTwoHopBoundaryNeighbors: unknown type " << seedTypeName << std::endl;
      return;
   }

   StoreResults<std::string, BoundaryTieSample> twoHopResults(5);
   twoHopResults.SetTitle("Two-hop boundary tie candidates from " + seedTypeName);
   twoHopResults.SetKeyLabel("MatchedType");

   size_t totalSecondHopCandidates = 0;
   size_t rejectedInvalidSamples = 0;

   for (int s = 0; s < nSamples; ++s) {
      const S6 sample = GetValidTypeSample(seedTypePtr->GetPrj());
      if (!LRL_Cell(sample).IsValid()) {
         ++rejectedInvalidSamples;
         continue;
      }

      const auto firstHopCandidates = BuildBoundaryReflectionCandidates(sample);
      std::cout << "; " << seedTypeName << " sample " << s << ": "
         << firstHopCandidates.size() << " first-hop candidates, expanding each..." << std::endl;

      for (const auto& firstHop : firstHopCandidates) {
         const auto secondHopCandidates = BuildBoundaryReflectionCandidates(firstHop);

         for (const auto& secondHop : secondHopCandidates) {
            ++totalSecondHopCandidates;

            const auto ranked = RankAllTypesByResidual(secondHop, allTypes);
            for (const auto& tr : ranked) {
               if (tr.typeName == seedTypeName) continue;
               if (tr.typeName == "A1") continue;
               if (tr.residual >= matchTol) break;

               BoundaryTieSample bts;
               bts.seedType = seedTypeName;
               bts.matchedType = tr.typeName;
               bts.residual = tr.residual;
               bts.candidate = secondHop;

               twoHopResults.Store(tr.typeName, bts);
            }
         }
      }
   }

   if (rejectedInvalidSamples > 0) {
      std::cout << "; " << rejectedInvalidSamples << "/" << nSamples
         << " sample(s) rejected as invalid" << std::endl;
   }

   std::cout << "; SearchTwoHopBoundaryNeighbors(" << seedTypeName << ") complete: "
      << totalSecondHopCandidates << " second-hop candidates checked, "
      << twoHopResults.size() << " distinct type(s) reached at two hops" << std::endl;

   twoHopResults.ShowResults();
}

// BuildFaceProjectedCandidates -- forces a sample onto each of the 6
// boundary facets in turn (zero component i), REPROJECTS through the
// seed type's own projector to stay honestly inside its subspace after
// the forced zero (raw zeroing alone can drift off the type's own
// equality constraints if coordinate i isn't actually free in its
// character), then applies that facet's boundary transform. Mirrors
// GrimmerChains::Remediation's approach directly -- all 6 boundaries
// applied uniformly, no attempt to pick a "natural" single facet (Larry,
// 2026-08-20: "in Remediation, i just blindly applied all 6 boundary
// transforms. that's harmless.").
//
// Returns one candidate per boundary (6 total, not reflection-expanded).
// Unlike BuildBoundaryReflectionCandidates (interior sampling), a
// face-projected candidate can fail Selling validity -- boundaries are
// exactly where degeneracies concentrate. This function does NOT filter
// invalid results; the caller must check each candidate and should
// expect and report a real rejection rate, not treat it as an error.
inline std::vector<S6> BuildFaceProjectedCandidates(const S6& v, const MatS6& seedProjector) {
   static const std::vector<MatS6> boundaryTransforms = Selling::GetBoundaryTransforms();

   std::vector<S6> candidates;
   candidates.reserve(boundaryTransforms.size());

   for (size_t i = 0; i < 6; ++i) {
      S6 faceProjected = v;
      faceProjected[i] = 0.0;
      faceProjected = seedProjector * faceProjected;   // stay honest in seed's subspace

      candidates.push_back(boundaryTransforms[i] * faceProjected);
   }

   return candidates;
}

// SearchFaceProjectedBoundaryNeighbors -- targeted single-type probe
// using face-projected candidates instead of interior sampling. Reports
// the per-facet invalid-candidate rejection rate explicitly (expected to
// be nonzero -- see BuildFaceProjectedCandidates' comment), then checks
// every valid candidate against every other type.
inline void SearchFaceProjectedBoundaryNeighbors(
   const std::string& seedTypeName,
   const int nSamples = 10,
   const double matchTol = 1.0E-8) {

   const auto allTypes = GenerateLatticeTypeExamples::CreateListOfDeloneTypes();

   std::shared_ptr<GenerateDeloneBase> seedTypePtr;
   for (const auto& t : allTypes) {
      if (t->GetName() == seedTypeName) { seedTypePtr = t; break; }
   }
   if (!seedTypePtr) {
      std::cout << "; SearchFaceProjectedBoundaryNeighbors: unknown type "
         << seedTypeName << std::endl;
      return;
   }

   StoreResults<std::string, BoundaryTieSample> results(5);
   results.SetTitle("Face-projected boundary tie candidates from " + seedTypeName);
   results.SetKeyLabel("MatchedType");

   size_t totalCandidates = 0;
   size_t rejectedInvalidCandidates = 0;
   size_t rejectedInvalidSamples = 0;

   for (int s = 0; s < nSamples; ++s) {
      const S6 sample = GetValidTypeSample(seedTypePtr->GetPrj());
      if (!LRL_Cell(sample).IsValid()) {
         ++rejectedInvalidSamples;
         continue;
      }

      const auto faceCandidates = BuildFaceProjectedCandidates(sample, seedTypePtr->GetPrj());

      for (const auto& candidate : faceCandidates) {
         ++totalCandidates;

         if (!LRL_Cell(candidate).IsValid()) {
            ++rejectedInvalidCandidates;
            continue;
         }

         const auto ranked = RankAllTypesByResidual(candidate, allTypes);
         for (const auto& tr : ranked) {
            if (tr.typeName == seedTypeName) continue;
            if (tr.typeName == "A1") continue;
            if (tr.residual >= matchTol) break;

            BoundaryTieSample bts;
            bts.seedType = seedTypeName;
            bts.matchedType = tr.typeName;
            bts.residual = tr.residual;
            bts.candidate = candidate;

            results.Store(tr.typeName, bts);
         }
      }
   }

   std::cout << "; SearchFaceProjectedBoundaryNeighbors(" << seedTypeName << ") complete: "
      << totalCandidates << " face-projected candidates, "
      << rejectedInvalidCandidates << " rejected as invalid ("
      << (totalCandidates > 0 ? 100.0 * rejectedInvalidCandidates / totalCandidates : 0.0)
      << "%), " << rejectedInvalidSamples << " seed sample(s) rejected, "
      << results.size() << " distinct type(s) reached" << std::endl;

   results.ShowResults();
}

// SweepRemediatedFitNearR1R3Boundary -- the actual question (chat
// 2026-08-20): how far is a near-cubic cell from R1, using Remediation
// rather than raw fit? Sweeps the R1/R3/C5 boundary family, reporting
// raw vs remediated fit(R1) side by side with raw fit(R3)/fit(C5) for
// context.
inline void SweepRemediatedFitNearR1R3Boundary(
   const CmdSellaControls& controls, Sella& sella) {

   const std::vector<double> gammaDegrees = { 90.00, 89.99, 89.9, 89.5, 89.0, 87.0 };

   std::cout << "; gamma      raw_R1      remediated_R1   raw_R3   raw_C5" << std::endl;
   for (const double gamma : gammaDegrees) {
      std::ostringstream cellStr;
      cellStr << "p 10 10 10 90 90 " << gamma;
      const LatticeCell cell = InputHandler::processSingleLatticeInput(cellStr.str());

      const auto fits = ComputeDeloneFits(cell, controls, sella).fits;

      double rawR1 = -1.0, rawR3 = -1.0, rawC5 = -1.0;
      for (const auto& f : fits) {
         if (f.GetGeneralType() == "R1") rawR1 = f.GetRawFit();
         if (f.GetGeneralType() == "R3") rawR3 = f.GetRawFit();
         if (f.GetGeneralType() == "C5") rawC5 = f.GetRawFit();
      }

      const double remediatedR1 = GetRemediatedFitForType(cell, "R1");

      std::cout << "; " << gamma << "   " << rawR1 << "   " << remediatedR1
         << "   " << rawR3 << "   " << rawC5 << std::endl;
   }
}

// SearchAllTypeBoundaryDistances -- the enumeration counterpart to the
// exact-tie search, using GetRemediatedFitForType instead of exact
// residual matching. Where SearchAllTypesBoundaryNeighbors can only
// report "tied" or "not tied" (and structurally cannot see R1/R3-style
// smooth-but-inexact relationships, per chat 2026-08-20), this reports
// a real distance for every (seedType, otherType) pair -- the actual
// dictionary of boundary relationships, not just the exact subset.
//
// COST NOTE: this is a deliberate one-time enumeration cost, run
// offline to build the map -- NOT a proposal to call Remediation on
// every live fit (Larry, 2026-08-20: "the stupid approach"). Once the
// map exists, a live fit only needs Remediation for types the map
// already flags as having a real nearby boundary.
inline void SearchAllTypeBoundaryDistances(
   const int nSamplesPerType = 5,
   const double interestingDistanceThreshold = 5.0) {
   const auto allTypes = GenerateLatticeTypeExamples::CreateListOfDeloneTypes();
   StoreResults<std::string, BoundaryTieSample> distanceResults(5);
   distanceResults.SetTitle("Type-pair boundary distances (via Remediation)");
   distanceResults.SetKeyLabel("TypePair");
   for (const auto& seedTypePtr : allTypes) {
      const std::string seedName = seedTypePtr->GetName();
      for (int s = 0; s < nSamplesPerType; ++s) {
         const S6 sample = GetValidTypeSample(seedTypePtr->GetPrj());
         if (!LRL_Cell(sample).IsValid()) continue;
         std::ostringstream cellStr;
         cellStr << "S " << sample;
         const LatticeCell cell = InputHandler::processSingleLatticeInput(cellStr.str());
         for (const auto& otherTypePtr : allTypes) {
            const std::string otherName = otherTypePtr->GetName();
            if (otherName == seedName || otherName == "A1") continue;
            const double remediatedFit = GetRemediatedFitForType(cell, otherName);
            if (remediatedFit < 0.0 || remediatedFit > interestingDistanceThreshold) continue;
            BoundaryTieSample bts;
            bts.seedType = seedName;
            bts.matchedType = otherName;
            bts.residual = remediatedFit;
            bts.candidate = sample;
            distanceResults.Store(MakeUnorderedPairKey(seedName, otherName), bts);
         }
      }
   }
   std::cout << "; SearchAllTypeBoundaryDistances complete: "
      << distanceResults.size() << " type-pair(s) with remediated distance < "
      << interestingDistanceThreshold << std::endl;
   distanceResults.ShowResults();
}

// RunNestingConsistencyCheck -- general, name-free version of the
// O1B/O2/O3 diagnostic that started this investigation (see chat with
// Claude, 2026-08-14 through 2026-08-15). For every pair of Delone types
// (P, C) where C has strictly fewer free parameters than P, tests via
// TestDeloneParentage (DeloneParentageTest.h) whether C genuinely nests
// inside P -- purely from each type's own character/free-param count, no
// hand-picked list. For every confirmed pair, builds a genuine boundary
// example of C (BuildCharacterExample) and checks
// fit(P) <= fit(C) + epsilon using the REAL, currently-active fit
// pipeline (ComputeDeloneFits/sella.SellaFit -- whichever matrix library
// is active, static or BUILD), not the idealized canonical projector
// TestDeloneParentage itself uses. epsilon = controls.getNestingDelta()
// times the boundary example's own norm, same relative-tolerance
// convention as MAXDELTA.
//
// Enabled via the CHECKNESTING control (CmdSellaControls.h) -- off by
// default, so this needs no attention during normal use.
void RunNestingConsistencyCheck(const CmdSellaControls& controls, Sella& sella) {
   std::cout << "; RunNestingConsistencyCheck: entered" << std::endl;
   const auto allTypes = GenerateDeloneBase().Select("");
   std::cout << "; RunNestingConsistencyCheck: got " << allTypes.size()
      << " Delone types" << std::endl;

   size_t pairsNested = 0;
   size_t pairsTested = 0;
   size_t representativesTested = 0;
   size_t violatingPairs = 0;
   size_t errors = 0;

   for (const auto& parent : allTypes) {
      for (const auto& child : allTypes) {
         if (parent->GetName() == child->GetName()) continue;
         if (child->GetFreeParams() >= parent->GetFreeParams()) continue;

         const auto parentage = TestDeloneParentage(*parent, *child);
         if (!parentage.isNested) continue;
         ++pairsNested;

         try {
            const S6 childExample = BuildCharacterExample(child->GetCharacter());

            // Test every representative reachable by crossing the child's
            // own zero-boundaries, not just the one naive/obvious
            // alignment BuildCharacterExample happens to produce on its
            // own. That naive point trivially satisfies most parents'
            // canonical pairing by construction (all its equal/zero
            // slots already line up) and never exercises the actual
            // library-coverage gap -- see chat with Claude, 2026-08-15:
            // the O1B/O3 bug only shows up on the boundary-CROSSED
            // representative (oneLattice), never on the naive one, which
            // is exactly why the first version of this check found zero
            // violations despite the known bug still being present.
            const auto crossing = ExpandAcrossBoundaryZeros(childExample);
            std::cout << "; checking parent=" << parent->GetName()
               << " child=" << child->GetName()
               << " (" << crossing.representatives.size() << " representative(s))" << std::endl;

            if (crossing.rejectedNonReduced > 0) {
               std::cout << ";   WARNING: " << crossing.rejectedNonReduced
                  << " rejected non-reduced crossing(s)" << std::endl;
            }

            bool pairTested = false;
            bool pairViolated = false;

            for (const auto& representative : crossing.representatives) {
               // Route through InputHandler::processSingleLatticeInput --
               // the same parser the console uses for typed "S ..."
               // input (InputHandler::parseS6, via
               // isLattice/handleSingleLattice) -- rather than
               // LatticeCell's string constructor, which doesn't
               // recognize "S" as a designator at all (see chat with
               // Claude, 2026-08-15).
               std::ostringstream cellStr;
               cellStr << "S " << representative;
               const LatticeCell probeCell = InputHandler::processSingleLatticeInput(cellStr.str());

               const DeloneFitComputation probeFits = ComputeDeloneFits(probeCell, controls, sella);

               double fitParent = -1.0;
               double fitChild = -1.0;
               for (const auto& f : probeFits.fits) {
                  if (f.GetGeneralType() == parent->GetName()) fitParent = f.GetRawFit();
                  if (f.GetGeneralType() == child->GetName()) fitChild = f.GetRawFit();
               }
               // Type not present in this representative's fit results --
               // skip this representative rather than guess.
               if (fitParent < 0.0 || fitChild < 0.0) continue;

               pairTested = true;
               ++representativesTested;

               const double epsilon = controls.getNestingDelta() * representative.norm();
               if (fitParent > fitChild + epsilon) {
                  pairViolated = true;
                  std::cout << "; NESTING VIOLATION: parent=" << parent->GetName()
                     << " child=" << child->GetName()
                     << " representative=" << representative
                     << " fit(parent)=" << fitParent
                     << " fit(child)=" << fitChild
                     << " epsilon=" << epsilon
                     << " gap=" << (fitParent - fitChild) << std::endl;
               }
            }

            if (pairTested) ++pairsTested;
            if (pairViolated) ++violatingPairs;
         }
         catch (const std::exception& e) {
            ++errors;
            std::cout << ";   ERROR on parent=" << parent->GetName()
               << " child=" << child->GetName() << ": " << e.what() << std::endl;
         }
         catch (...) {
            ++errors;
            std::cout << ";   UNKNOWN ERROR on parent=" << parent->GetName()
               << " child=" << child->GetName() << std::endl;
         }
      }
   }

   std::cout << "; CHECKNESTING summary: " << pairsNested
      << " confirmed parent/child pair(s), " << pairsTested
      << " pair(s) tested (" << representativesTested << " representative(s) total), "
      << violatingPairs << " pair(s) with at least one violation, "
      << errors << " error(s)" << std::endl;
}

// SweepRemediatedFitTowardA2 -- targeted follow-up to A1|A2 showing up
// in SearchAllTypeBoundaryDistances (chat 2026-08-20). A2's character
// (rs0 tuv) has exactly one forced zero, at position 2. Base point
// confirmed valid and already Selling-reduced via checkinput/cmddelone
// (chat 2026-08-20): S -73 -41 -50 -19 -87 -33 -> P 10.488 14.491
// 12.124 114.550 108.810 109.207, cmddelone returns it unchanged.
// Sweeps component 2 toward zero, watching remediated fit(A2).
inline void SweepRemediatedFitTowardA2() {
   const std::vector<double> component2Values = { -50.0, -10.0, -2.0, -0.5, -0.05, 0.0 };

   std::cout << "; component2      remediated_A2" << std::endl;
   for (const double c2 : component2Values) {
      std::ostringstream cellStr;
      cellStr << "S -73.0 -41.0 " << c2 << " -19.0 -87.0 -33.0";
      const LatticeCell cell = InputHandler::processSingleLatticeInput(cellStr.str());

      const double remediatedA2 = GetRemediatedFitForType(cell, "A2");
      std::cout << "; " << c2 << "   " << remediatedA2 << std::endl;
   }
}

// SweepRemediatedFitTowardA3ViaM1B -- targeted follow-up to A3|M1B
// showing up in SearchAllTypeBoundaryDistances (chat 2026-08-20). A3's
// confirmed zero pattern (chat 2026-08-20, 24-reflection enumeration)
// is the ANTIPODAL PAIR (2,5) -- not a single component, unlike A2.
// M1B has no forced zeros at all (character rst rsu), so this mirrors
// A1|A2's setup: a fully generic type (M1B, playing A1's earlier role)
// approaching a type with a specific zero structure (A3). Base point
// confirmed valid and already Selling-reduced via checkinput/cmddelone
// (chat 2026-08-20): S -73 -41 c25 -19 -87 c25, cmddelone returns it
// unchanged for c25 = -50. Sweeps components 2 AND 5 TOGETHER toward
// zero (A3's antipodal pair, not a single coordinate), watching
// remediated fit(A3).
inline void SweepRemediatedFitTowardA3ViaM1B() {
   const std::vector<double> component25Values = { -50.0, -10.0, -2.0, -0.5, -0.05, 0.0 };

   std::cout << "; component25      remediated_A3" << std::endl;
   for (const double c25 : component25Values) {
      std::ostringstream cellStr;
      cellStr << "S -73.0 -41.0 " << c25 << " -19.0 -87.0 " << c25;
      const LatticeCell cell = InputHandler::processSingleLatticeInput(cellStr.str());

      const double remediatedA3 = GetRemediatedFitForType(cell, "A3");
      std::cout << "; " << c25 << "   " << remediatedA3 << std::endl;
   }
}

// SweepTowardTargetViaProjector -- GENERAL, pair-independent version of
// the hand-built sweeps (SweepRemediatedFitNearR1R3Boundary,
// SweepRemediatedFitTowardA2, SweepRemediatedFitTowardA3ViaM1B). Uses
// sourceType's projector to draw a genuinely valid source sample
// (mechanically enforces source's real constraints -- no character-
// string parsing, per Larry, 2026-08-20: "the constraint structure for
// each type is already encoded in projectors"), then uses targetType's
// projector to compute the direction toward target's subspace directly:
// d = P_target * v - v. Walks v + t*d for t in [0,1], checking validity
// at every step (cheap, per Larry) and recording remediated fit(target)
// at each valid step.
struct SweepStep {
   double t;
   bool valid;
   double remediatedFit;   // -1.0 if step was invalid, not computed
};

inline std::vector<SweepStep> SweepTowardTargetViaProjector(
   const GenerateDeloneBase& sourceType,
   const GenerateDeloneBase& targetType,
   const std::vector<double>& tValues = { 0.0, 0.2, 0.4, 0.6, 0.8, 0.95, 1.0 }) {

   std::vector<SweepStep> steps;

   const S6 v = GetValidTypeSample(sourceType.GetPrj());
   if (!LRL_Cell(v).IsValid()) {
      std::cout << "; SweepTowardTargetViaProjector(" << sourceType.GetName()
         << " -> " << targetType.GetName() << "): source sample itself invalid, aborting"
         << std::endl;
      return steps;
   }

   const S6 projected = targetType.GetPrj() * v;
   const S6 d = projected - v;

   for (const double t : tValues) {
      const S6 point = v + t * d;
      SweepStep step;
      step.t = t;

      if (!LRL_Cell(point).IsValid()) {
         step.valid = false;
         step.remediatedFit = -1.0;
         steps.push_back(step);
         continue;
      }

      std::ostringstream cellStr;
      cellStr << "S " << point;
      const LatticeCell cell = InputHandler::processSingleLatticeInput(cellStr.str());

      step.valid = true;
      step.remediatedFit = GetRemediatedFitForType(cell, targetType.GetName());
      steps.push_back(step);
   }

   return steps;
}

// RunThreeCalibrationSweeps -- the three test cases discussed (chat
// 2026-08-20): A1->A2 (expected clean pass, regression check against
// the hand-built version), M1B->A3 REDONE PROPERLY with a genuine
// GetValidTypeSample(M1B) point (the earlier hand-typed M1B|A3 sweep
// used a vector that did NOT satisfy M1B's own character constraints
// -- this repeats it correctly), and C1->A2 (predicted incompatible --
// C1's rrr rrr ties every position to 'r', so approaching A2's zero at
// position 2 should force full collapse toward the origin, not a real
// boundary approach).
inline void RunThreeCalibrationSweeps() {
   const auto allTypes = GenerateLatticeTypeExamples::CreateListOfDeloneTypes();

   auto findType = [&](const std::string& name) -> std::shared_ptr<GenerateDeloneBase> {
      for (const auto& t : allTypes) if (t->GetName() == name) return t;
      return nullptr;
      };

   const std::vector<std::pair<std::string, std::string>> cases = {
      {"A1", "A2"}, {"M1B", "A3"}, {"C1", "A2"}
   };

   for (const auto& [sourceName, targetName] : cases) {
      const auto source = findType(sourceName);
      const auto target = findType(targetName);
      if (!source || !target) {
         std::cout << "; RunThreeCalibrationSweeps: unknown type in pair ("
            << sourceName << ", " << targetName << ")" << std::endl;
         continue;
      }

      std::cout << "; -- " << sourceName << " -> " << targetName << " --" << std::endl;
      const auto steps = SweepTowardTargetViaProjector(*source, *target);
      for (const auto& s : steps) {
         if (s.valid) {
            std::cout << ";   t=" << s.t << " valid=1 remediatedFit=" << s.remediatedFit << std::endl;
         }
         else {
            std::cout << ";   t=" << s.t << " valid=0 (rejected)" << std::endl;
         }
      }
   }
}


// PairSweepSummary -- one row of the general-method calibration pass.
// Note: fitAtEnd (t=1) is expected to be ~0 for essentially EVERY
// pair, by construction -- v + 1*(P_target*v - v) = P_target*v, which
// sits exactly on target's subspace, so Remediation should find near-
// zero residual there regardless of whether source and target have any
// real relationship. The actually informative signals are:
//   - allStepsValid: does the straight-line path stay a real,
//     Selling-valid lattice all the way, or does it pass through
//     invalid/degenerate territory to get there
//   - monotonic: does the fit decrease smoothly, or does Remediation's
//     discrete 6-alternative search cause plateaus/jumps (as seen in
//     A1->A2's calibration run)
//   - fitAtStart: how far the random source sample started out
struct PairSweepSummary {
   std::string sourceType;
   std::string targetType;
   bool allStepsValid;
   bool monotonic;
   double fitAtStart;     // t=0
   double fitAtNearEnd;   // t=0.95
   double fitAtEnd;       // t=1
};

inline PairSweepSummary SummarizeSweep(
   const std::string& sourceName,
   const std::string& targetName,
   const std::vector<SweepStep>& steps) {

   PairSweepSummary s;
   s.sourceType = sourceName;
   s.targetType = targetName;
   s.allStepsValid = true;
   s.monotonic = true;
   s.fitAtStart = -1.0;
   s.fitAtNearEnd = -1.0;
   s.fitAtEnd = -1.0;

   double lastValidFit = -1.0;
   for (size_t i = 0; i < steps.size(); ++i) {
      const auto& step = steps[i];
      if (!step.valid) {
         s.allStepsValid = false;
         continue;
      }
      if (lastValidFit >= 0.0 && step.remediatedFit > lastValidFit + 1.0E-9) {
         s.monotonic = false;
      }
      lastValidFit = step.remediatedFit;

      if (i == 0) s.fitAtStart = step.remediatedFit;
      if (i == steps.size() - 2) s.fitAtNearEnd = step.remediatedFit;
      if (i == steps.size() - 1) s.fitAtEnd = step.remediatedFit;
   }
   return s;
}

// RunGeneralSweepOverIncomparablePairs -- the actual 137-pair
// calibration pass (Larry, 2026-08-20). Reuses FindMissingBoundaryTies'
// own nested-pair exclusion logic directly (TestDeloneParentage both
// directions) rather than depending on a previously-run enumeration
// table, so this is self-contained. For each incomparable pair, picks
// whichever type is NOT in excludeAsSource as the sweep source (skips
// the pair entirely only if BOTH are excluded -- e.g. C1/C3 against
// each other). excludeAsSource defaults to {A1, C1, C3, C5} -- see
// CLAUDE.md: this is a CALIBRATION CONVENIENCE for this pass
// specifically, not a permanent exclusion; C1/C3/C5 still appear as
// TARGETS here, and get their own dedicated pass as sources later.
inline void RunGeneralSweepOverIncomparablePairs(
   const std::set<std::string>& excludeAsSource = { "A1", "C1", "C3", "C5" }) {

   const auto allTypes = GenerateLatticeTypeExamples::CreateListOfDeloneTypes();
   std::vector<PairSweepSummary> summaries;

   size_t totalPairs = 0;
   size_t skippedNested = 0;
   size_t skippedBothExcluded = 0;

   for (size_t i = 0; i < allTypes.size(); ++i) {
      for (size_t j = i + 1; j < allTypes.size(); ++j) {
         ++totalPairs;
         const auto& A = *allTypes[i];
         const auto& B = *allTypes[j];

         const bool aNestsB = TestDeloneParentage(B, A).isNested;
         const bool bNestsA = TestDeloneParentage(A, B).isNested;
         if (aNestsB || bNestsA) {
            ++skippedNested;
            continue;
         }

         const bool aExcluded = excludeAsSource.count(A.GetName()) > 0;
         const bool bExcluded = excludeAsSource.count(B.GetName()) > 0;
         if (aExcluded && bExcluded) {
            ++skippedBothExcluded;
            continue;
         }

         const auto& source = aExcluded ? B : A;
         const auto& target = aExcluded ? A : B;

         const auto steps = SweepTowardTargetViaProjector(source, target);
         summaries.push_back(SummarizeSweep(source.GetName(), target.GetName(), steps));

         std::cout << "; " << source.GetName() << " -> " << target.GetName()
            << "  allValid=" << summaries.back().allStepsValid
            << "  monotonic=" << summaries.back().monotonic
            << "  fit@0=" << summaries.back().fitAtStart
            << "  fit@0.95=" << summaries.back().fitAtNearEnd
            << "  fit@1=" << summaries.back().fitAtEnd << std::endl;
      }
   }

   std::cout << "; RunGeneralSweepOverIncomparablePairs complete: " << totalPairs
      << " total pairs, " << skippedNested << " nested (skipped), "
      << skippedBothExcluded << " both-source-excluded (skipped), "
      << summaries.size() << " swept" << std::endl;
}


// SweepTowardTargetViaProjectorWithReflections -- extends
// SweepTowardTargetViaProjector to test whether a straight-line path
// that fails against target's PRIMARY orientation succeeds against a
// REFLECTED orientation instead. Motivated directly by the O5
// reachability failures (chat 2026-08-20): 6 of 10 sources with their
// own forced zero failed to reach O5 via a straight line, while all
// zero-free sources succeeded. Hypothesis (Larry): if a straight path
// to one orientation of the target crosses invalid territory, a
// reflected orientation might offer a path that doesn't.
inline void SweepTowardTargetViaProjectorWithReflections(
   const GenerateDeloneBase& sourceType,
   const GenerateDeloneBase& targetType,
   const std::vector<double>& tValues = { 0.0, 0.2, 0.4, 0.6, 0.8, 0.95, 1.0 }) {

   static const std::vector<MatS6> reflections = MatS6::GetReflections();

   const S6 v = GetValidTypeSample(sourceType.GetPrj());
   if (!LRL_Cell(v).IsValid()) {
      std::cout << "; source sample invalid, aborting" << std::endl;
      return;
   }

   int reflectionsSucceeded = 0;
   for (size_t r = 0; r < reflections.size(); ++r) {
      const S6 reorientedTarget = reflections[r] * (targetType.GetPrj() * v);
      const S6 d = reorientedTarget - v;

      bool allValid = true;
      double finalFit = -1.0;
      for (const double t : tValues) {
         const S6 point = v + t * d;
         if (!LRL_Cell(point).IsValid()) { allValid = false; break; }
         std::ostringstream cellStr;
         cellStr << "S " << point;
         const LatticeCell cell = InputHandler::processSingleLatticeInput(cellStr.str());
         finalFit = GetRemediatedFitForType(cell, targetType.GetName());
      }

      std::cout << ";   reflection " << r << ": allValid=" << allValid
         << " finalFit=" << finalFit << std::endl;
      if (allValid) ++reflectionsSucceeded;
   }

   std::cout << "; " << sourceType.GetName() << " -> " << targetType.GetName()
      << " (with reflections): " << reflectionsSucceeded << "/24 reflection(s) gave a valid path"
      << std::endl;
}

// RunReflectionRescueCheck -- runs SweepTowardTargetViaProjectorWithReflections
// against every pair that failed (allValid=0) in the original 137-pair
// straight-line batch (chat 2026-08-20): R3->T5, R3->O5, R3->H4,
// T2->O5, T5->O5, O2->O5, O3->O5, O3->H4, O4->O5. Tests whether a
// reflected orientation of the target rescues a path that fails
// against the target's primary orientation.
inline void RunReflectionRescueCheck() {
   const auto allTypes = GenerateLatticeTypeExamples::CreateListOfDeloneTypes();

   auto findType = [&](const std::string& name) -> std::shared_ptr<GenerateDeloneBase> {
      for (const auto& t : allTypes) if (t->GetName() == name) return t;
      return nullptr;
      };

   const std::vector<std::pair<std::string, std::string>> failingPairs = {
      {"R3", "T5"}, {"R3", "O5"}, {"R3", "H4"},
      {"T2", "O5"}, {"T5", "O5"},
      {"O2", "O5"}, {"O3", "O5"}, {"O3", "H4"}, {"O4", "O5"}
   };

   for (const auto& [sourceName, targetName] : failingPairs) {
      const auto source = findType(sourceName);
      const auto target = findType(targetName);
      if (!source || !target) {
         std::cout << "; RunReflectionRescueCheck: unknown type in pair ("
            << sourceName << ", " << targetName << ")" << std::endl;
         continue;
      }
      std::cout << "; -- " << sourceName << " -> " << targetName << " --" << std::endl;
      SweepTowardTargetViaProjectorWithReflections(*source, *target);
   }
}

// PrintOneSweepEndpoints -- diagnostic aid (chat 2026-08-20): prints
// the actual t=0 and t=1 S6 vectors for one source/target pair, so a
// dense manual path can be built and checked by hand for where/why
// validity breaks down. No sweep logic, just the two endpoints and the
// straight-line direction between them.
inline void PrintOneSweepEndpoints(
   const GenerateDeloneBase& sourceType,
   const GenerateDeloneBase& targetType) {

   const S6 v = GetValidTypeSample(sourceType.GetPrj());
   std::cout << "; source (" << sourceType.GetName() << ") v      = " << v << std::endl;
   std::cout << ";   v valid? " << LRL_Cell(v).IsValid() << std::endl;

   const S6 projected = targetType.GetPrj() * v;
   std::cout << "; target (" << targetType.GetName() << ") P*v    = " << projected << std::endl;
   std::cout << ";   P*v valid? " << LRL_Cell(projected).IsValid() << std::endl;

   const S6 d = projected - v;
   std::cout << "; direction d = P*v - v = " << d << std::endl;
}

// SweepTowardTargetViaBestProjector -- corrected version of
// SweepTowardTargetViaProjector. GetPrj() is unvalidated for
// multi-orientation types (confirmed AGAIN, 2026-08-20: O5 has 16
// stored orientations in GetSellaProjectors(), each a clean diagonal
// selector; T5 -> O5's GetPrj()*v collapsed to the exact origin,
// explaining all 9 straight-line-sweep failures involving O5/H4).
// Tries every stored orientation, picks the one giving a valid,
// non-degenerate P*v (checked via LRL_Cell validity, same cheap check
// used throughout this session), falls back to GetPrj() only if no
// stored orientation works.
inline std::vector<SweepStep> SweepTowardTargetViaBestProjector(
   const GenerateDeloneBase& sourceType,
   const GenerateDeloneBase& targetType,
   const std::vector<double>& tValues = { 0.0, 0.2, 0.4, 0.6, 0.8, 0.95, 1.0 }) {

   std::vector<SweepStep> steps;

   const S6 v = GetValidTypeSample(sourceType.GetPrj());
   if (!LRL_Cell(v).IsValid()) {
      std::cout << "; source sample invalid, aborting" << std::endl;
      return steps;
   }

   S6 bestProjected;
   bool found = false;
   for (const auto& prj : targetType.GetSellaProjectors()) {
      const S6 candidate = prj * v;
      if (LRL_Cell(candidate).IsValid()) {
         bestProjected = candidate;
         found = true;
         break;
      }
   }
   if (!found) {
      bestProjected = targetType.GetPrj() * v;   // fallback, same as before
      if (!LRL_Cell(bestProjected).IsValid()) {
         std::cout << "; " << sourceType.GetName() << " -> " << targetType.GetName()
            << ": NO valid orientation found (checked " << targetType.GetSellaProjectors().size()
            << " stored + GetPrj() fallback)" << std::endl;
         return steps;
      }
   }

   const S6 d = bestProjected - v;
   for (const double t : tValues) {
      const S6 point = v + t * d;
      SweepStep step;
      step.t = t;
      if (!LRL_Cell(point).IsValid()) {
         step.valid = false;
         step.remediatedFit = -1.0;
      }
      else {
         std::ostringstream cellStr;
         cellStr << "S " << point;
         const LatticeCell cell = InputHandler::processSingleLatticeInput(cellStr.str());
         step.valid = true;
         step.remediatedFit = GetRemediatedFitForType(cell, targetType.GetName());
      }
      steps.push_back(step);
   }
   return steps;
}

// RunReflectionRescueCheck's replacement -- reruns the 9 originally-
// failing pairs through SweepTowardTargetViaBestProjector (which tries
// every stored orientation, not just the unvalidated GetPrj()) instead
// of the reflection-based rescue attempt, which was tested and found
// to give 0/24 successes across all 9 pairs (chat 2026-08-20) -- ruled
// out as the actual mechanism, since the real problem turned out to be
// GetPrj() itself being degenerate for O5/H4, not the choice of
// reflection on top of it.
inline void RerunFailingPairsWithBestProjector() {
   const auto allTypes = GenerateLatticeTypeExamples::CreateListOfDeloneTypes();

   auto findType = [&](const std::string& name) -> std::shared_ptr<GenerateDeloneBase> {
      for (const auto& t : allTypes) if (t->GetName() == name) return t;
      return nullptr;
      };

   const std::vector<std::pair<std::string, std::string>> failingPairs = {
      {"R3", "T5"}, {"R3", "O5"}, {"R3", "H4"},
      {"T2", "O5"}, {"T5", "O5"},
      {"O2", "O5"}, {"O3", "O5"}, {"O3", "H4"}, {"O4", "O5"}
   };

   size_t nowValid = 0;
   size_t stillInvalid = 0;

   for (const auto& [sourceName, targetName] : failingPairs) {
      const auto source = findType(sourceName);
      const auto target = findType(targetName);
      if (!source || !target) {
         std::cout << "; unknown type in pair (" << sourceName << ", " << targetName << ")" << std::endl;
         continue;
      }

      const auto steps = SweepTowardTargetViaBestProjector(*source, *target);
      const auto summary = SummarizeSweep(sourceName, targetName, steps);

      std::cout << "; " << sourceName << " -> " << targetName
         << "  allValid=" << summary.allStepsValid
         << "  monotonic=" << summary.monotonic
         << "  fit@0=" << summary.fitAtStart
         << "  fit@0.95=" << summary.fitAtNearEnd
         << "  fit@1=" << summary.fitAtEnd << std::endl;

      if (summary.allStepsValid) ++nowValid; else ++stillInvalid;
   }

   std::cout << "; RerunFailingPairsWithBestProjector complete: "
      << nowValid << "/9 now valid, " << stillInvalid << "/9 still invalid" << std::endl;
}


inline void RunGeneralSweepOverIncomparablePairsV2(
   const std::set<std::string>& excludeAsSource = { "A1", "C1", "C3", "C5" }) {

   const auto allTypes = GenerateLatticeTypeExamples::CreateListOfDeloneTypes();
   std::vector<PairSweepSummary> summaries;

   size_t totalPairs = 0;
   size_t skippedNested = 0;
   size_t skippedBothExcluded = 0;

   for (size_t i = 0; i < allTypes.size(); ++i) {
      for (size_t j = i + 1; j < allTypes.size(); ++j) {
         ++totalPairs;
         const auto& A = *allTypes[i];
         const auto& B = *allTypes[j];

         const bool aNestsB = TestDeloneParentage(B, A).isNested;
         const bool bNestsA = TestDeloneParentage(A, B).isNested;
         if (aNestsB || bNestsA) {
            ++skippedNested;
            continue;
         }

         const bool aExcluded = excludeAsSource.count(A.GetName()) > 0;
         const bool bExcluded = excludeAsSource.count(B.GetName()) > 0;
         if (aExcluded && bExcluded) {
            ++skippedBothExcluded;
            continue;
         }

         const auto& source = aExcluded ? B : A;
         const auto& target = aExcluded ? A : B;

         const auto steps = SweepTowardTargetViaBestProjector(source, target);
         summaries.push_back(SummarizeSweep(source.GetName(), target.GetName(), steps));

         std::cout << "; " << source.GetName() << " -> " << target.GetName()
            << "  allValid=" << summaries.back().allStepsValid
            << "  monotonic=" << summaries.back().monotonic
            << "  fit@0=" << summaries.back().fitAtStart
            << "  fit@0.95=" << summaries.back().fitAtNearEnd
            << "  fit@1=" << summaries.back().fitAtEnd << std::endl;
      }
   }

   std::cout << "; RunGeneralSweepOverIncomparablePairsV2 complete: " << totalPairs
      << " total pairs, " << skippedNested << " nested (skipped), "
      << skippedBothExcluded << " both-source-excluded (skipped), "
      << summaries.size() << " swept" << std::endl;
}

// RunRemediationShortlistSanityCheck -- quick manual check for
// RemediateIfShortlistChanges: one cell near the O1B/O2/O3 boundary
// (expected to change the shortlist) and one clean interior cell
// (expected NOT to trigger remediation at all), run through
// ComputeDeloneFits directly and printed for inspection.
void RunRemediationShortlistSanityCheck() {
   CmdSellaControls controls;   // default-constructed; we print fits directly
   // below rather than relying on shouldDebug()
   Sella sella;

   const std::vector<std::string> cells = {
      "P 9.030 9.030 9.030 117.884 122.162 90.000",  // original (unperturbed)
      "P 9.028 9.027 9.025 117.901 122.164 89.971",
      "P 9.030 9.030 9.027 117.863 122.195 90.006",
      "P 9.029 9.028 9.028 117.885 122.138 89.979",
      "P 9.025 9.030 9.033 117.909 122.147 89.965",
      "P 9.028 9.029 9.029 117.926 122.144 89.982",
      "P 9.031 9.031 9.029 117.869 122.126 90.013",
      "P 9.029 9.025 9.026 117.889 122.185 89.963",
      "P 9.032 9.027 9.025 117.872 122.165 89.996",
      "P 9.027 9.028 9.027 117.895 122.197 89.972",
      "P 9.031 9.029 9.036 117.889 122.164 89.998"
   };

   for (const auto& cellStr : cells) {
      const LatticeCell cell(cellStr);
      std::cout << "; --- " << cellStr << " ---" << std::endl;
      DeloneFitComputation result = ComputeDeloneFits(cell, controls, sella);
      for (const auto& f : result.fits) {
         std::cout << f.GetGeneralType() << " " << f.GetRawFit() << std::endl;
      }
      std::cout << std::endl;
   }
}

//// Remediation144 -- like Remediation, but searches the full 144-candidate
//// space (6 boundary transforms x 24 reflections) instead of the 6
//// boundary-transform-only search. Built to hand-compare against
//// Remediation on cases where the 6-only search plateaus without resolving
//// a Grimmer chain violation (see chat 2026-08-22 -- the oC violation on
//// "P 9.031 9.031 9.029 117.869 122.126 90.013" that Remediation could not
//// improve on a second pass).
////
//// ASSUMPTION FLAGGED: composition order is boundary transform first, then
//// reflection (candidate = reflection * (boundaryTransform * s6)), matching
//// this investigation's existing BuildBoundaryReflectionCandidates naming
//// ("boundary-reflection"). I have not seen BuildBoundaryReflectionCandidates
//// itself, so I could not confirm this against its actual implementation --
//// if the two disagree on order, this function and the boundary-tie search
//// tooling could silently be exploring different 144-candidate sets. Worth
//// checking directly before trusting a "144 doesn't help either" result as
//// definitive.
//inline DeloneFitResults Remediation144(const S6& s6, const std::string& bravaisName) {
//   const std::vector<std::shared_ptr<GenerateDeloneBase>> sptypes =
//      GenerateDeloneBase().Select(bravaisName);
//
//   const DeloneFitResults dfrInit = Sella::SellaFitXXXXXX(sptypes[0], s6);
//   auto dfr = dfrInit;
//
//   // Fixed lookup tables, do not depend on bravaisName or s6 -- safe to
//   // cache as static, unlike the sptypes caching bug found earlier this
//   // investigation (that bug was caching a bravaisName-DEPENDENT result).
//   static const std::vector<MatS6> boundaryTransforms = Selling::GetBoundaryTransforms();
//   static const std::vector<MatS6> reflections = MatS6::GetReflections();
//
//   for (const auto& boundary : boundaryTransforms) {
//      const S6 afterBoundary = boundary * s6;
//      for (const auto& reflection : reflections) {
//         const S6 candidate = reflection * afterBoundary;
//         const auto dfrTemp = Sella::SellaFitXXXXXX(sptypes[0], candidate);
//         if (dfrTemp.GetRawFit() < dfr.GetRawFit()) {
//            dfr = dfrTemp;
//         }
//      }
//   }
//   return dfr;
//}




// RunRemediation144Comparison -- hand-compare Remediation (6-candidate)
// against Remediation144 (144-candidate) on the oC violation that stalled
// after 2 passes on "P 9.031 9.031 9.029 117.869 122.126 90.013" (chat
// 2026-08-22): Remediation returned 38.135 both times and made no further
// progress, leaving the mP/oC/tP chain violation unresolved. If 144 does
// better, that's direct evidence the 6-only search is capacity-limited,
// not that this cell has hit a real floor.
void RunRemediation144Comparison() {
   const LatticeCell cell("P 9.031 9.031 9.029 117.869 122.126 90.013");
   MatS6 oneReductionMatrix;
   const S6 oneLattice = GetInputSellingReducedVectors(cell, oneReductionMatrix);

   const std::string targetType = "oC";
   const DeloneFitResults r6 = Remediation(oneLattice, targetType, -1.0);
   const DeloneFitResults r144 = Remediation144(oneLattice, targetType);

   std::cout << "; oC on P 9.031 9.031 9.029 117.869 122.126 90.013" << std::endl;
   std::cout << "; Remediation (6):    " << r6.GetRawFit() << std::endl;
   std::cout << "; Remediation144:     " << r144.GetRawFit() << std::endl;
   std::cout << "; improvement: " << (r6.GetRawFit() - r144.GetRawFit()) << std::endl;
}


// RunMpOcTpLastDigitCheck -- checks whether the oC/tP crossover on
// "P 9.031 9.031 9.029 117.869 122.126 90.013" (fit 38.135 vs 38.1165,
// see chat 2026-08-22) is a rounding artifact of that specific printed
// cell, by nudging the last printed digit of each parameter one at a
// time and re-checking mP/oC/tP fits. If the crossover persists across
// all variants, it's a real local feature, not print-precision noise.
void RunMpOcTpLastDigitCheck() {
   CmdSellaControls controls;
   Sella sella;

   const std::vector<std::string> cells = {
      "P 9.031 9.031 9.029 117.869 122.126 90.013",   // original
      "P 9.032 9.031 9.029 117.869 122.126 90.013",   // a +1
      "P 9.030 9.031 9.029 117.869 122.126 90.013",   // a -1
      "P 9.031 9.032 9.029 117.869 122.126 90.013",   // b +1
      "P 9.031 9.030 9.029 117.869 122.126 90.013",   // b -1
      "P 9.031 9.031 9.030 117.869 122.126 90.013",   // c +1
      "P 9.031 9.031 9.028 117.869 122.126 90.013",   // c -1
      "P 9.031 9.031 9.029 117.870 122.126 90.013",   // alpha +1
      "P 9.031 9.031 9.029 117.868 122.126 90.013",   // alpha -1
      "P 9.031 9.031 9.029 117.869 122.127 90.013",   // beta +1
      "P 9.031 9.031 9.029 117.869 122.125 90.013",   // beta -1
      "P 9.031 9.031 9.029 117.869 122.126 90.014",   // gamma +1
      "P 9.031 9.031 9.029 117.869 122.126 90.012"    // gamma -1
   };

   for (const auto& cellStr : cells) {
      const LatticeCell cell(cellStr);
      DeloneFitComputation result = ComputeDeloneFits(cell, controls, sella);
      double mP = -1.0, oC = -1.0, tP = -1.0;
      for (const auto& f : result.fits) {
         if (f.GetGeneralType() == "M4") mP = f.GetRawFit();
         if (f.GetGeneralType() == "O4") oC = f.GetRawFit();
         if (f.GetGeneralType() == "T5") tP = f.GetRawFit();
      }
      std::cout << cellStr
         << "  mP=" << mP << " oC=" << oC << " tP=" << tP
         << (oC < tP ? "  CROSSOVER" : "") << std::endl;
   }
}

//// Remediation2Hop -- like Remediation, but composes TWO boundary transforms
//// instead of one (36 candidates: 6x6, including i==j since we don't know a
//// priori whether GetBoundaryTransforms()[i] is self-inverse). This is the
//// candidate space Remediation and Remediation144 do NOT cover: both of
//// those only ever apply a single boundary crossing (optionally dressed with
//// a reflection), never two crossings composed together. Built specifically
//// to test whether the oC floor found on "P 9.031 9.031 9.029 117.869
//// 122.126 90.013" (Remediation and Remediation144 both stall at 38.135,
//// chat 2026-08-22) is a genuine floor or an artifact of single-hop search
//// missing a two-boundary coincidence -- parallel in spirit to
//// SearchTwoHopBoundaryNeighbors's role in the R1/R3 investigation.
//inline DeloneFitResults Remediation2Hop(const S6& s6, const std::string& bravaisName) {
//   const std::vector<std::shared_ptr<GenerateDeloneBase>> sptypes =
//      GenerateDeloneBase().Select(bravaisName);
//
//   const DeloneFitResults dfrInit = Sella::SellaFitXXXXXX(sptypes[0], s6);
//   auto dfr = dfrInit;
//
//   static const std::vector<MatS6> boundaryTransforms = Selling::GetBoundaryTransforms();
//
//   for (const auto& first : boundaryTransforms) {
//      const S6 afterFirst = first * s6;
//      for (const auto& second : boundaryTransforms) {
//         const S6 candidate = second * afterFirst;
//         const auto dfrTemp = Sella::SellaFitXXXXXX(sptypes[0], candidate);
//         if (dfrTemp.GetRawFit() < dfr.GetRawFit()) {
//            dfr = dfrTemp;
//         }
//      }
//   }
//   return dfr;
//}

// RunRemediation2HopComparison -- extends RunRemediation144Comparison with
// the two-hop (36-candidate) search, to test whether the oC floor on this
// cell is genuine or a single-hop search-coverage gap.
void RunRemediation2HopComparison() {
   const LatticeCell cell("P 9.031 9.031 9.029 117.869 122.126 90.013");
   MatS6 oneReductionMatrix;
   const S6 oneLattice = GetInputSellingReducedVectors(cell, oneReductionMatrix);

   const std::string targetType = "oC";
   const DeloneFitResults r6 = Remediation(oneLattice, targetType, -1.0);
   const DeloneFitResults r144 = Remediation144(oneLattice, targetType);
   const DeloneFitResults r2hop = Remediation2Hop(oneLattice, targetType);

   std::cout << "; oC on P 9.031 9.031 9.029 117.869 122.126 90.013" << std::endl;
   std::cout << "; Remediation (6):        " << r6.GetRawFit() << std::endl;
   std::cout << "; Remediation144 (144):   " << r144.GetRawFit() << std::endl;
   std::cout << "; Remediation2Hop (36):   " << r2hop.GetRawFit() << std::endl;
   std::cout << "; improvement over 6:     " << (r6.GetRawFit() - r2hop.GetRawFit()) << std::endl;
}

// RunHistoricalBadCaseCheck -- re-runs the mP/oS/tP chain-failure cases
// logged in comments at the top of this file (2023-12-23, 2023-12-24 --
// "still bad after remediation" at the time) through the current
// ComputeDeloneFits, to see whether today's changes (RemediateIfShortlist
// Changes, plus the pre-existing vertical remediation loop) now resolve
// them. Unlike the mP/oC/tP artifact investigated 2026-08-22, these are
// real cases: mP/tP fits in the 0.06-2.8 range, genuinely plausible
// candidate matches, not ~38-scale non-matches.
void RunHistoricalBadCaseCheck(const  CmdSellaControls& controls) {
   Sella sella;

   const std::vector<std::string> cells = {
      "P 21.775 21.785 27.232  90.028  89.974  90.023",
      "P  1.307  3.134  3.150 144.338  90.898 113.499",
      "P  1.447  2.154  1.448  93.001  93.077 118.555",
      "P  1.511  3.987  3.918 149.756 103.646  94.613",
      "P  1.909  3.372  3.412 140.217  95.846 108.922",
      "P  1.916  4.033  1.656 105.411 101.400  96.056",
      "P  1.918  1.118  1.899  94.267 130.788 116.312",
      "P  2.044  2.594  3.604 121.251  93.338  95.754",
      "P  2.092  3.512  1.736  92.747  95.706 123.113",
      "P  2.212  3.228  3.111 124.239  96.056 120.065",
      "P  2.418  2.421  1.105  91.232 105.455 146.970",
      "P  2.778  1.881  3.996 114.240  97.653  94.507",
      "P  2.891  2.299  3.839  90.545 118.914 100.136",
      "P  2.973  2.869  1.853  93.424 114.956 138.239",
      "P  3.286  1.671  1.295  98.790  90.427 114.783",
      "P  3.345  3.397  1.230  91.705 101.386 151.360"
   };

   int resolvedCount = 0;
   for (const auto& cellStr : cells) {
      const LatticeCell cell(cellStr);
      DeloneFitComputation result = ComputeDeloneFits(cell, controls, sella);
      const bool resolved = !result.chains.HasFailure();
      if (resolved) ++resolvedCount;
      std::cout << cellStr << (resolved ? "  RESOLVED" : "  STILL FAILING") << std::endl;
   }
   std::cout << "; " << resolvedCount << " of " << cells.size() << " historical cases resolved" << std::endl;
}

// RunHistorical144And2HopCheck -- for each of the 8 historical cases that
// remained STILL FAILING after RunHistoricalBadCaseCheck, computes oC's
// fit via the 6-candidate Remediation, the 144-candidate Remediation144,
// and the 36-candidate Remediation2Hop, alongside tP's fit, to see which
// remaining gaps are genuine floors (144/2-hop agree with 6) versus real
// search-coverage gaps (144 or 2-hop find something better). Mirrors
// RunRemediation2HopComparison's method, applied to the historical set
// instead of the single artifact case from earlier this session.
void RunHistorical144And2HopCheck(const CmdSellaControls& controls) {
   const std::vector<std::string> stillFailingCells = {
      "P 21.775 21.785 27.232  90.028  89.974  90.023",
      "P  1.511  3.987  3.918 149.756 103.646  94.613",
      "P  1.909  3.372  3.412 140.217  95.846 108.922",
      "P  2.092  3.512  1.736  92.747  95.706 123.113",
      "P  2.418  2.421  1.105  91.232 105.455 146.970",
      "P  2.973  2.869  1.853  93.424 114.956 138.239",
      "P  3.286  1.671  1.295  98.790  90.427 114.783",
      "P  3.345  3.397  1.230  91.705 101.386 151.360"
   };

   for (const auto& cellStr : stillFailingCells) {
      const LatticeCell cell(cellStr);
      MatS6 oneReductionMatrix;
      const S6 oneLattice = GetInputSellingReducedVectors(cell, oneReductionMatrix);

      const DeloneFitResults r6 = Remediation(oneLattice, "oC", -1.0);
      const DeloneFitResults r144 = Remediation144(oneLattice, "oC");
      const DeloneFitResults r2hop = Remediation2Hop(oneLattice, "oC");
      const DeloneFitResults tP = Remediation(oneLattice, "tP", -1.0);

      const double best6 = r6.GetRawFit();
      const double best144 = r144.GetRawFit();
      const double best2hop = r2hop.GetRawFit();
      const double bestOverall = std::min({ best6, best144, best2hop });

      std::cout << cellStr << std::endl;
      std::cout << ";   tP:              " << tP.GetRawFit() << std::endl;
      std::cout << ";   oC Remediation:  " << best6 << std::endl;
      std::cout << ";   oC 144:          " << best144
         << (best144 < best6 - 1.0E-8 ? "  IMPROVED" : "") << std::endl;
      std::cout << ";   oC 2Hop:         " << best2hop
         << (best2hop < best6 - 1.0E-8 ? "  IMPROVED" : "") << std::endl;
      std::cout << ";   " << (bestOverall <= tP.GetRawFit() * (1.0 + 1.0E-8) ? "RESOLVED by wider search" : "still a gap")
         << std::endl << std::endl;
   }
}


// RunStabilityVsCurrentComparison -- runs both ComputeDeloneFits (current
// mechanism) and ClassifyViaStabilityRemediation (new, standalone) on the
// same historical cells, and reports any type whose fit disagrees beyond
// numerical noise -- so we can see whether the simpler approach reproduces
// the current results before considering it as a replacement.
void RunStabilityVsCurrentComparison(const CmdSellaControls& controls) {
   Sella sella;

   const std::vector<std::string> cells = {
      "P 21.775 21.785 27.232  90.028  89.974  90.023",
      "P  1.307  3.134  3.150 144.338  90.898 113.499",
      "P  1.447  2.154  1.448  93.001  93.077 118.555",
      "P  1.511  3.987  3.918 149.756 103.646  94.613",
      "P  1.909  3.372  3.412 140.217  95.846 108.922",
      "P  1.916  4.033  1.656 105.411 101.400  96.056",
      "P  1.918  1.118  1.899  94.267 130.788 116.312",
      "P  2.044  2.594  3.604 121.251  93.338  95.754",
      "P  2.092  3.512  1.736  92.747  95.706 123.113",
      "P  2.212  3.228  3.111 124.239  96.056 120.065",
      "P  2.418  2.421  1.105  91.232 105.455 146.970",
      "P  2.778  1.881  3.996 114.240  97.653  94.507",
      "P  2.891  2.299  3.839  90.545 118.914 100.136",
      "P  2.973  2.869  1.853  93.424 114.956 138.239",
      "P  3.286  1.671  1.295  98.790  90.427 114.783",
      "P  3.345  3.397  1.230  91.705 101.386 151.360"
   };

   for (const auto& cellStr : cells) {
      const LatticeCell cell(cellStr);
      DeloneFitComputation currentResult = ComputeDeloneFits(cell, controls, sella);
      std::vector<StabilityClassification> stabilityResult = ClassifyViaStabilityRemediation(cell);

      std::cout << "; --- " << cellStr << " ---" << std::endl;
      for (const auto& entry : stabilityResult) {
         const DeloneFitResults& newFit = entry.fit;
         double oldFit = -1.0;
         bool found = false;
         for (const auto& oldOne : currentResult.fits) {
            if (oldOne.GetGeneralType() == newFit.GetGeneralType()) {
               oldFit = oldOne.GetRawFit();
               found = true;
               break;
            }
         }
         const double delta = found ? std::abs(newFit.GetRawFit() - oldFit) : -1.0;
         std::cout << newFit.GetGeneralType()
            << "  current=" << (found ? std::to_string(oldFit) : "MISSING")
            << "  stability=" << newFit.GetRawFit()
            << "  rounds=" << entry.rounds
            << (entry.hitCap ? "  HIT_CAP" : "")
            << (found && delta > 0.001 ? "  DIFFERS" : "")
            << std::endl;
      }
      std::cout << std::endl;
   }
}
// RunStabilityTrace -- prints the fit at every round of RemediateToStability
// for one type on one cell, to check whether HIT_CAP cases (M2A, M4;
// chat 2026-08-22) show the zigzag signature of a two-boundary corner --
// alternating overshoot, geometrically diminishing gap -- versus simple
// slow-but-monotonic convergence or a stuck search.
// RunStabilityTrace -- prints fit AND the actual candidate cell (S6 raw
// components + P-cell degrees form) at every round of RemediateToStability,
// to check whether small-delta steps are approaching a sensible target
// cell (chat 2026-08-22) rather than drifting or cycling meaninglessly.
void RunStabilityTrace(const S6& s6, const std::string& bravaisName, const int maxRounds = 30) {
   S6 currentPoint = s6;
   DeloneFitResults currentFit = Remediation(currentPoint, bravaisName, -1.0);
   std::cout << bravaisName << " round 0: fit=" << currentFit.GetRawFit()
      << "  s6=" << currentPoint
      << "  cell=" << LRL_Cell_Degrees(currentPoint) << std::endl;

   for (int round = 1; round <= maxRounds; ++round) {
      MatS6 reductionMatrix;
      const S6 reducedNextPoint = LatticeConverter::SellingReduceCell(
         "P", currentFit.GetOriginalInput(), reductionMatrix);
      const DeloneFitResults nextFit = Remediation(reducedNextPoint, bravaisName, -1.0);
      std::cout << bravaisName << " round " << round
         << ": fit=" << nextFit.GetRawFit()
         << "  delta=" << (currentFit.GetRawFit() - nextFit.GetRawFit())
         << "  s6=" << reducedNextPoint
         << "  cell=" << LRL_Cell_Degrees(reducedNextPoint) << std::endl;
      currentFit = nextFit;
      currentPoint = reducedNextPoint;
   }
}

// RunRawFitCheck -- computes raw, unremediated Sella fit per type on one
// cell and compares against ComputeDeloneFits's "current" values, to test
// whether RemediateIfShortlistChanges fell back to raw fit for every type
// on this cell (i.e. the shortlist didn't change) -- chat 2026-08-22,
// checking whether the DIFFERS pattern in RunStabilityVsCurrentComparison
// is explained by current==raw rather than current==remediated.
void RunRawFitCheck(const CmdSellaControls& controls) {
   Sella sella;
   const LatticeCell cell("P  1.511  3.987  3.918 149.756 103.646  94.613");

   MatS6 oneReductionMatrix;
   const S6 oneLattice = GetInputSellingReducedVectors(cell, oneReductionMatrix);

   DeloneFitComputation currentResult = ComputeDeloneFits(cell, controls, sella);

   static const std::vector<std::string> allTypes = {
      "C1","C3","C5",
      "R1","R3",
      "T1","T2","T5",
      "O1A","O1B","O2","O3","O4","O5",
      "M1A","M1B","M2A","M2B","M3","M4",
      "A1","A2","A3",
      "H4"
   };

   for (const auto& typeName : allTypes) {
      const std::vector<std::shared_ptr<GenerateDeloneBase>> sptypes =
         GenerateDeloneBase().Select(typeName);
      const DeloneFitResults rawFit = Sella::SellaFitXXXXXX(sptypes[0], oneLattice);

      double currentFit = -1.0;
      bool found = false;
      for (const auto& oldOne : currentResult.fits) {
         if (oldOne.GetGeneralType() == typeName) {
            currentFit = oldOne.GetRawFit();
            found = true;
            break;
         }
      }

      const double delta = found ? std::abs(rawFit.GetRawFit() - currentFit) : -1.0;
      std::cout << typeName
         << "  raw=" << rawFit.GetRawFit()
         << "  current=" << (found ? std::to_string(currentFit) : "MISSING")
         << (found && delta < 0.001 ? "  MATCHES_RAW" : "")
         << std::endl;
   }
}



// RunRemediation168Check -- verifies Remediation168 (RemediatedFit.h) against
// the full 16-cell historical chain-failure set (same cells as
// RunStabilityVsCurrentComparison, not just the 8 still-failing-after-2-hop
// ones), so this also serves as a regression check on cells that were
// already fine under the current 36-candidate Remediation. Reports per-cell
// current-vs-widened O4 fit and whether the oC<=tP chain relationship now
// holds, plus an improved/unchanged/regressed summary count.
inline void RunRemediation168Check(const CmdSellaControls& controls) {
   const std::vector<std::string> cells = {
      "P 21.775 21.785 27.232  90.028  89.974  90.023",
      "P  1.307  3.134  3.150 144.338  90.898 113.499",
      "P  1.447  2.154  1.448  93.001  93.077 118.555",
      "P  1.511  3.987  3.918 149.756 103.646  94.613",
      "P  1.909  3.372  3.412 140.217  95.846 108.922",
      "P  1.916  4.033  1.656 105.411 101.400  96.056",
      "P  1.918  1.118  1.899  94.267 130.788 116.312",
      "P  2.044  2.594  3.604 121.251  93.338  95.754",
      "P  2.092  3.512  1.736  92.747  95.706 123.113",
      "P  2.212  3.228  3.111 124.239  96.056 120.065",
      "P  2.418  2.421  1.105  91.232 105.455 146.970",
      "P  2.778  1.881  3.996 114.240  97.653  94.507",
      "P  2.891  2.299  3.839  90.545 118.914 100.136",
      "P  2.973  2.869  1.853  93.424 114.956 138.239",
      "P  3.286  1.671  1.295  98.790  90.427 114.783",
      "P  3.345  3.397  1.230  91.705 101.386 151.360"
   };

   int improvedCount = 0;
   int unchangedCount = 0;
   int regressedCount = 0;

   for (const auto& cellStr : cells) {
      const LatticeCell cell(cellStr);
      MatS6 oneReductionMatrix;
      const S6 oneLattice = GetInputSellingReducedVectors(cell, oneReductionMatrix);

      const DeloneFitResults current = Remediation(oneLattice, "O4", -1.0);
      const DeloneFitResults widened = Remediation168(oneLattice, "O4", -1.0);
      const DeloneFitResults tP = Remediation(oneLattice, "tP", -1.0);

      const double delta = current.GetRawFit() - widened.GetRawFit();
      std::string verdict;
      if (delta > 1.0E-8) { verdict = "IMPROVED"; ++improvedCount; }
      else if (delta < -1.0E-8) { verdict = "REGRESSION"; ++regressedCount; }
      else { verdict = "no change"; ++unchangedCount; }

      std::cout << cellStr << std::endl;
      std::cout << ";   O4 current (36):  " << current.GetRawFit() << std::endl;
      std::cout << ";   O4 widened (168): " << widened.GetRawFit() << "  " << verdict << std::endl;
      std::cout << ";   tP:               " << tP.GetRawFit()
         << "  " << (widened.GetRawFit() <= tP.GetRawFit() * (1.0 + 1.0E-8) ? "oC<=tP OK" : "oC>tP still")
         << std::endl << std::endl;
   }

   std::cout << "; RunRemediation168Check summary: " << improvedCount << " improved, "
      << unchangedCount << " unchanged, " << regressedCount << " regressed (out of "
      << cells.size() << ")" << std::endl;
}

#endif //TEST_HARNESS_H