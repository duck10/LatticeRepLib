// ConfirmedDeloneNesting.h
//
// Confirmed nesting facts for specific Delone-type pairs in the Grimmer
// partial order. Each entry names which of a parent Bravais node's
// Delone siblings are confirmed to actually nest into one specific
// Delone type of a child Bravais node -- not just have the best raw fit
// among siblings, which is what GrimmerTree.cpp's chain-check currently
// (incorrectly) uses as its comparison basis. That mismatch is the root
// cause behind several of the SELLA chain-violation failure modes (see
// /areas/cmdsella.md and sella_failure_status.md).
//
// Data source (2026-09-01 revision): every row below is read off a
// RunNestingConsistencyCheck (CHECKNESTING) log. The audit runs in two
// stages, and BOTH stages count as "tested" for this table:
//   1. TestDeloneParentage (168-candidate VCP search, Andrews/Bernstein/
//      Sauter) decides whether a (parent, child) Delone-type pair is
//      even an algebraic candidate. A sibling that never appears in the
//      log against a given child is not a coverage gap -- it means this
//      stage already confirmed that sibling does NOT nest. This is a
//      real, confirmed exclusion, on the same footing as a fit failure.
//   2. For pairs that pass stage 1, RunNestingConsistencyCheck builds a
//      genuine boundary example of the child (BuildCharacterExample),
//      crosses it across every symmetry-equivalent boundary realization
//      (ExpandAcrossBoundaryZeros), and checks fit(parent) <= fit(child)
//      + epsilon against the real, currently-active fit pipeline for
//      each one. A representative that fails this is a NESTING VIOLATION
//      -- also a confirmed exclusion, just reached the opposite way.
// testedParentSiblings is therefore the FULL Delone-type membership of
// the parent Bravais node (all 5 for mC, all 3 for aP/oI, all 2 for tI,
// etc.) -- every member was evaluated by stage 1 at minimum, whether or
// not it proceeded to stage 2. nestingSiblings is the subset that
// cleared both stages clean.
//
// (Earlier same-day revision of this file scoped testedParentSiblings to
// only pairs with an explicit "checking parent=... child=..." log line,
// treating everything else as untested. That was wrong: it silently
// discarded every stage-1 exclusion, which is most of the informative
// data in this table. Confirmed independently -- O3 never appearing
// against T1/T2 in the log matches the original investigation's
// separately-established finding that O3 is a genuine dead end for tI.)
//
// This REPLACES the original data source, TestDeloneNesting.h's P*Q=Q
// idempotent-projector test, which checked exactly one canonical
// embedding per type instead of the full symmetry-equivalent boundary
// family and was shown to systematically under-detect real nestings
// (e.g. it reported "M1A alone nests O4" when M1A, M1B, and M2A all do).
// TestDeloneNesting.h/RunItem8NestingAudit.h/RunFullNestingSweep.h are no
// longer the source for this file's data -- see CmdSella.cpp's 2026-08-29
// removal comment.
//
// SCOPE NOTE: this table only records facts for direct edges in
// GrimmerTree.cpp's chainTemplates (e.g. aP->mC, mC->hR, oI->tI) --
// the pairs the chain-check can actually compare siblings across. The
// same CHECKNESTING run also found A3 failing to nest O4, O5, and H4,
// and M3 (an mC sibling) failing to nest H4 -- real violations, but
// aP has no direct chain edge to oC/oP/hP, and mC has no direct chain
// edge to hP (reached via oC), so the chain-check this table exists to
// correct can never compare those siblings directly. Left out of this
// table for that reason.
//
// This is deliberately NOT the same kind of data as GrimmerTree.cpp's
// chainTemplates (which define chain topology and carry live per-cell fit
// values). This table is a small, separate, growable record of confirmed
// nesting-test results -- both positive and negative -- decoupled from
// rendering and from the chain-check machinery it will eventually help
// correct. "Confirmed" describes the test outcome (we ran it, we know
// the answer), not that the result was necessarily positive: a sibling
// confirmed NOT to nest -- whether by stage-1 non-candidacy or a stage-2
// fit violation -- is just as much a confirmed result as one that does,
// and is usually the more useful one for this table's purpose, since a
// row where every sibling passes gives the chain-check no corrective
// information (any choice among them is safe).
//
// A fact is edge-and-child-scoped, not sibling-scoped: knowing M1B nests
// O1B says nothing about whether M1B nests O2, or O1A, or R1/R3 -- those
// are separate facts.

#ifndef CONFIRMEDDELONENESTING_H
#define CONFIRMEDDELONENESTING_H

#include <string>
#include <vector>

struct ConfirmedNesting {
   std::string parentBravais;
   std::string childBravais;
   std::string childDeloneType;
   std::vector<std::string> testedParentSiblings;
   std::vector<std::string> nestingSiblings;

   ConfirmedNesting(const std::string& parent, const std::string& child,
      const std::string& childDelone,
      const std::vector<std::string>& tested,
      const std::vector<std::string>& siblings)
      : parentBravais(parent), childBravais(child)
      , childDeloneType(childDelone)
      , testedParentSiblings(tested), nestingSiblings(siblings) {
   }

   // True when every tested sibling nests -- the fact is real but
   // excludes nothing, so it gives the chain-check no corrective
   // information (any tested sibling is a safe pick for this child).
   bool ExcludesNothing() const {
      return !nestingSiblings.empty() && nestingSiblings.size() == testedParentSiblings.size();
   }
};

inline std::vector<ConfirmedNesting> CreateConfirmedNestingFacts() {
   std::vector<ConfirmedNesting> facts;

   const std::vector<std::string> mCFamily = { "M1A", "M1B", "M2A", "M2B", "M3" };
   const std::vector<std::string> oIFamily = { "O1B", "O2", "O3" };
   const std::vector<std::string> tIFamily = { "T1", "T2" };
   const std::vector<std::string> aPFamily = { "A1", "A2", "A3" };

   // mC -> oC/O4: M2A now included alongside M1A/M1B (old data said M1A
   // alone). M2B/M3 confirmed non-candidates (TestDeloneParentage stage).
   facts.push_back(ConfirmedNesting("mC", "oC", "O4",
      mCFamily, { "M1A", "M1B", "M2A" }));

   // mC -> oI: splits cleanly by child, same shape the original
   // investigation found. O1B: M1A/M1B only (M2A confirmed non-candidate
   // here, unlike O2/O4). O2: M1A/M1B/M2A. O3: all 5 -- the one case
   // where nothing is excluded (kept, ExcludesNothing()==true).
   facts.push_back(ConfirmedNesting("mC", "oI", "O1B",
      mCFamily, { "M1A", "M1B" }));
   facts.push_back(ConfirmedNesting("mC", "oI", "O2",
      mCFamily, { "M1A", "M1B", "M2A" }));
   facts.push_back(ConfirmedNesting("mC", "oI", "O3",
      mCFamily, { "M1A", "M1B", "M2A", "M2B", "M3" }));

   // mC -> oF/O1A: M1A/M1B nest; M2A/M2B/M3 confirmed non-candidates.
   facts.push_back(ConfirmedNesting("mC", "oF", "O1A",
      mCFamily, { "M1A", "M1B" }));

   // mC -> hR: R1 reaches only M1A/M1B. R3 reaches M1A/M1B/M2A/M3 --
   // M2B the sole exclusion. Both a substantial change from the old
   // data (R3 was previously M3-only).
   facts.push_back(ConfirmedNesting("mC", "hR", "R1",
      mCFamily, { "M1A", "M1B" }));
   facts.push_back(ConfirmedNesting("mC", "hR", "R3",
      mCFamily, { "M1A", "M1B", "M2A", "M3" }));

   // oI -> tI: clean 1-1 split, same shape as always -- O1B reaches T1
   // only, O2 reaches T2 only, O3 reaches neither (confirmed dead end,
   // matching the original investigation's separate finding).
   facts.push_back(ConfirmedNesting("oI", "tI", "T1",
      oIFamily, { "O1B" }));
   facts.push_back(ConfirmedNesting("oI", "tI", "T2",
      oIFamily, { "O2" }));

   // oF -> tI: O1A is oF's only Delone type; nests both T1 and T2.
   // Unchanged -- already excludes nothing.
   facts.push_back(ConfirmedNesting("oF", "tI", "T1",
      { "O1A" }, { "O1A" }));
   facts.push_back(ConfirmedNesting("oF", "tI", "T2",
      { "O1A" }, { "O1A" }));

   // tI -> cI/C1: only T1 nests; T2 confirmed non-candidate against C1.
   facts.push_back(ConfirmedNesting("tI", "cI", "C1",
      tIFamily, { "T1" }));

   // tI -> cF/C3: both T1 and T2 nest -- excludes nothing. Unchanged.
   facts.push_back(ConfirmedNesting("tI", "cF", "C3",
      tIFamily, { "T1", "T2" }));

   // aP -> mP/M4: A1/A2 nest; A3 fails a real fit violation (6
   // representatives, consistent epsilon~0.008, gaps of 13 or 29 --
   // confirmed independently via cmdgen/cmdsella compare, 2026-08-31:
   // M4-generated cells fit M4 at 0.000 and fit A3 at 7-50).
   facts.push_back(ConfirmedNesting("aP", "mP", "M4",
      aPFamily, { "A1", "A2" }));

   // aP -> mC: A1 nests M1A/M1B alone (A2/A3 confirmed non-candidates
   // for both). M2A reaches A1 and A2; A3 excluded there too.
   facts.push_back(ConfirmedNesting("aP", "mC", "M1A",
      aPFamily, { "A1" }));
   facts.push_back(ConfirmedNesting("aP", "mC", "M1B",
      aPFamily, { "A1" }));
   facts.push_back(ConfirmedNesting("aP", "mC", "M2A",
      aPFamily, { "A1", "A2" }));

   return facts;
}

#endif // CONFIRMEDDELONENESTING_H