#ifndef SEARCH_ALL_BOUNDARY_TIES_H
#define SEARCH_ALL_BOUNDARY_TIES_H

// SearchAllBoundaryTies.h
//
// This is the wiring step ExploreReductionTies.h's own header comment
// says is NOT yet done: "Wiring that seeding loop is a separate step
// from this file -- this file only walks from a single given starting
// sample." This file is that separate step.
//
// PURPOSE: this is the "intelligent search for currently-unknown
// boundary pairs" referenced (unidentified) in both the 2026-08-19 and
// 2026-08-20 SELLA handoffs. Rather than hand-picking pairs to check
// (the O1B/O2/O3-by-hand pattern both handoffs explicitly moved away
// from), this drives the search off TestDeloneParentage's confirmed
// nesting graph, so which boundaries get walked is a computed answer,
// not a curated list.
//
// PIPELINE:
//   1. Enumerate all 24 Delone types (GenerateLatticeTypeExamples::
//      CreateListOfDeloneTypes()).
//   2. For every (parent, child) pair with child.GetFreeParams() <
//      parent.GetFreeParams(), call TestDeloneParentage(parent, child)
//      (DeloneParentageTest.h). Keep only confirmed-nested pairs with
//      seedValid == true.
//      ASSUMPTION, NOT YET CONFIRMED: this duplicates what
//      DeloneParentageGraph.h already does (per 2026-08-20 handoff,
//      Section 5). If DeloneParentageGraph.h already exposes a
//      "confirmed nesting edges" list/graph type, step 2 below should
//      call that directly instead of re-deriving the pair loop here --
//      check DeloneParentageGraph.h's actual interface before compiling
//      and replace GetConfirmedNestingEdges() below with the real call.
//   3. For every confirmed-nested child, seed ONE ExploreReductionTies
//      walk from BuildCharacterExample(child.GetCharacter())
//      (Deloneparentagetest.h) -- exactly the seeding source
//      ExploreReductionTies.h's own header names as the principled
//      choice.
//   4. For every terminal representative produced by a walk, check it
//      against every OTHER type's stored projector library (not just
//      the immediate parent -- a tie-branch terminal may land in a
//      type that isn't the parent/child pair that seeded the walk,
//      which is the entire point of doing this search instead of
//      checking named pairs by hand).
//      ASSUMPTION, NOT YET CONFIRMED: "check against the stored
//      library" is written here using CheckDeloneProjectors::
//      CheckOneProjector, per the 2026-08-19 handoff Section 1a's note
//      that DoesProjectorAgreeWithCharacter alone does not validate
//      zero slots and CheckOneProjector is the pair needed for a
//      correct check. Confirm CheckOneProjector's actual signature
//      against CheckDeloneProjectors.h before compiling -- guessed here
//      as CheckOneProjector(const S6&, const GenerateDeloneBase&) ->
//      bool, matching the description in the handoff, not verified
//      against the header directly.
//   5. Flag any terminal representative that does NOT agree with ANY
//      type's stored library within tolerance -- that is a genuine
//      candidate for "orientation this session hasn't seen," the
//      actual target of this search.
//
// CALIBRATION, BEFORE TRUSTING THIS ON ANYTHING UNKNOWN: run this over
// the full 24-type set first and confirm the known confirmed-swap pairs
// come out of it WITHOUT having been hand-seeded -- O1B/O2 (expected via
// an O3 seed), R1/R3, M1B/M2A, M1A/M1B, and ideally T1/T2. If any of
// those does NOT fall out, that is a specific, real bug in either the
// seeding (step 2/3) or the walk (ExploreReductionTies itself) -- fix
// that before using this to look for anything new. Do not skip this
// step; a search tool that can't reproduce known cases has no claim on
// finding unknown ones.
//
// KNOWN DEPENDENCY NOT YET VERIFIED (carried from 2026-08-20 handoff,
// Section 5): TestDeloneParentage/GetValidTypeSample draws a fresh
// random seed on every call. The repeatability check that handoff
// flagged as not-yet-done (call TestDeloneParentage on a known-nested
// and a known-not-nested pair several times, confirm isNested doesn't
// flip) should happen before trusting step 2 here, not after -- this
// file inherits that open risk, it does not resolve it.

#include "DeloneParentageTest.h"      // TestDeloneParentage, BuildCharacterExample,
                                       // DeloneParentageResult
#include "ExploreReductionTies.h"     // ExploreReductionTies, BoundaryWalkResult
#include "GenerateLatticeTypeExamples.h"
#include "S6.h"

#include <iostream>
#include <string>
#include <vector>

// One terminal representative from one seeded walk, together with the
// (parent, child) pair whose confirmed nesting produced the seed, kept
// for provenance -- so a flagged "unrecognized orientation" result can
// be traced back to which boundary produced it, not just the raw S6.
struct BoundaryTieCandidate {
   std::string seedParentType;
   std::string seedChildType;
   S6 terminalRepresentative;
   std::vector<std::string> matchedTypeNames;   // empty == unmatched;
   // size() >= 2 == boundary tie
};

struct SearchAllBoundaryTiesResult {
   std::vector<BoundaryTieCandidate> allTerminals;
   std::vector<BoundaryTieCandidate> unmatchedTerminals;   // matchedTypeNames.empty()
   std::vector<BoundaryTieCandidate> tieTerminals;         // matchedTypeNames.size() >= 2 --
   // the actual search target
   size_t nestedPairsFound = 0;
   size_t seedsAttempted = 0;
   size_t seedsInvalid = 0;   // TestDeloneParentage seedValid == false
};

// FindAllMatchingLibraries -- tests one terminal representative against
// EVERY known type's stored, VALIDATED projector library
// (GetSellaProjectors() only -- not GetPrj(), which for O2/O3 is a
// different, unvalidated matrix not confirmed to belong to the same
// comparison; see chat 2026-08-20). Does NOT return on first match:
// every type is checked, so a terminal sitting close to more than one
// type's library is reported as matching all of them -- a genuine
// boundary tie. This replaces CheckAgainstAllLibraries (first-match-
// wins), which could never surface a tie because it stopped looking
// after the first hit.
inline std::vector<std::string> FindAllMatchingLibraries(
   const S6& representative,
   const std::vector<std::shared_ptr<GenerateDeloneBase>>& allTypes,
   const double tol = 1.0E-8) {

   std::vector<std::string> matches;
   for (const auto& candidateType : allTypes) {
      for (const auto& prj : candidateType->GetSellaProjectors()) {
         const double residual = (prj * representative - representative).norm();
         if (residual < tol) {
            matches.push_back(candidateType->GetName());
            break;
         }
      }
   }
   return matches;
}

// SearchAllBoundaryTies -- the full pipeline described above. Prints
// progress to std::cout as it goes (this is a diagnostic/calibration
// tool, not production code) since a full 24-type run is expected to
// take real time and silent long-running loops are hard to debug.
inline SearchAllBoundaryTiesResult SearchAllBoundaryTies(const double tol = 1.0E-8) {
   SearchAllBoundaryTiesResult result;

   // ASSUMPTION: CreateListOfDeloneTypes() returns something iterable of
   // GenerateDeloneBase (or pointers to it) with GetFreeParams(),
   // GetCharacter(), GetName(), GetPrj() all available -- matches usage
   // already confirmed elsewhere (Deloneparentagetest.h). Exact return
   // type (value vs pointer, container type) not confirmed here --
   // adjust the loop below to match the real signature.
   const auto allTypes = GenerateLatticeTypeExamples::CreateListOfDeloneTypes();

   std::cout << "; SearchAllBoundaryTies: " << allTypes.size()
      << " Delone types loaded" << std::endl;

   for (const auto& parent : allTypes) {
      for (const auto& child : allTypes) {
         auto theChild = *child;
         auto theParent = *parent;
         if (theChild.GetName() == theParent.GetName()) continue;
         if (!(theChild.GetFreeParams() < theParent.GetFreeParams())) continue;

         const DeloneParentageResult parentage = TestDeloneParentage(theParent, theChild, tol);

         if (!parentage.seedValid) {
            ++result.seedsInvalid;
            std::cout << "; WARNING: seed invalid for theParent=" << theParent.GetName()
               << " theChild=" << theChild.GetName() << " -- skipped" << std::endl;
            continue;
         }
         if (!parentage.isNested) continue;

         ++result.nestedPairsFound;
         std::cout << "; confirmed nested: theParent=" << theParent.GetName()
            << " theChild=" << theChild.GetName()
            << " residual=" << parentage.residual << std::endl;

         const S6 seed = BuildCharacterExample(theChild.GetCharacter());
         ++result.seedsAttempted;

         const BoundaryWalkResult walk = ExploreReductionTies(seed, tol);

         for (const auto& terminal : walk.terminalRepresentatives) {
            BoundaryTieCandidate candidate;
            candidate.seedParentType = theParent.GetName();
            candidate.seedChildType = theChild.GetName();
            candidate.terminalRepresentative = terminal;

            candidate.matchedTypeNames = FindAllMatchingLibraries(terminal, allTypes, tol);

            result.allTerminals.push_back(candidate);

            if (candidate.matchedTypeNames.empty()) {
               result.unmatchedTerminals.push_back(candidate);
            }
            else if (candidate.matchedTypeNames.size() >= 2) {
               result.tieTerminals.push_back(candidate);
               std::cout << "; TIE CANDIDATE: seed(" << candidate.seedParentType
                  << "/" << candidate.seedChildType << ") terminal matches "
                  << candidate.matchedTypeNames.size() << " types:";
               for (const auto& name : candidate.matchedTypeNames) {
                  std::cout << " " << name;
               }
               std::cout << std::endl;
            }
         }
      }
   }

   std::cout << "; SearchAllBoundaryTies complete: " << result.nestedPairsFound
      << " nested pairs, " << result.seedsAttempted << " walks run, "
      << result.seedsInvalid << " invalid seeds skipped, "
      << result.allTerminals.size() << " terminal representatives, "
      << result.unmatchedTerminals.size() << " unmatched (no type matched), "
      << result.tieTerminals.size() << " tie candidate(s) (2+ types matched)"
      << std::endl;

   return result;
}

#endif  // SEARCH_ALL_BOUNDARY_TIES_H
