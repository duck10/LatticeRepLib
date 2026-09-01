// ConfirmedDeloneNesting.h
//
// Confirmed algebraic nesting facts (P*Q=Q idempotent-projector test,
// TestDeloneNesting.h) for specific Delone-type pairs in the Grimmer
// partial order. Each entry names which of a parent Bravais node's
// Delone siblings are confirmed to actually nest into one specific
// Delone type of a child Bravais node -- not just have the best raw fit
// among siblings, which is what GrimmerTree.cpp's chain-check currently
// (incorrectly) uses as its comparison basis. That mismatch is the root
// cause behind several of the SELLA chain-violation failure modes (see
// /areas/cmdsella.md and sella_failure_status.md).
//
// Schema note (2026-08-28, second revision): the first version had only
// (parentBravais, childBravais, nestingSiblings). Real item-8 results
// forced a childDeloneType field (mC->oI splits cleanly by child: M1B
// nests O1B, M2A nests O2). This revision adds testedParentSiblings --
// the full set of parent siblings the audit actually tested against
// that child, not just the ones that passed. Reason: a fact is only
// *informative* for the chain-check bug when it excludes at least one
// tested sibling (proof that "pick by raw fit" can pick wrong) or
// excludes all of them (a real dead end). When nestingSiblings equals
// testedParentSiblings -- every tested sibling nests -- the fact is true
// but carries no corrective information: any choice the naive chain-
// check makes among them is fine. tI->cF/C3 is the one case so far
// where this happens (both T1 and T2 nest C3); GrimmerDeloneTree skips
// rendering a label for it while still keeping the row here as a record
// that the audit ran and found no problem.
//
// This is deliberately NOT the same kind of data as GrimmerTree.cpp's
// chainTemplates (which define chain topology and carry live per-cell fit
// values). This table is a small, separate, growable record of confirmed
// *algebraic* results, decoupled from rendering and from the chain-check
// machinery it will eventually help correct.
//
// A fact is edge-and-child-scoped, not sibling-scoped: knowing M1B nests
// O1B says nothing about whether M1B nests O2, or O1A, or R1/R3 -- those
// are separate facts.
//
// SELLA "item 8" is the systematic audit filling this table in, via
// RunItem8NestingAudit.h. Run 2026-08-28: 23 pairs tested across
// mC->oI, mC->oF, mC->hR, oI->tI, tI->cI, tI->cF. Results below.

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

   const std::vector<std::string> mCSiblings = { "M1A", "M1B", "M2A" };
   const std::vector<std::string> tISiblings = { "T1", "T2" };

   // mC -> oC (Aug 2026, original finding): O4 nests inside M1A only.
   // Explains the aP/mC/oC Grimmer chain failures.
   facts.push_back(ConfirmedNesting("mC", "oC", "O4", mCSiblings, { "M1A" }));

   // mC -> oI (item 8, 2026-08-28/29): splits cleanly by child. O1B and
   // O3 are BOTH reached via M1B specifically -- the only case so far
   // where one parent sibling is the answer for two different children.
   // M2A remains the sole answer for O2, disjoint from M1B's two.
   facts.push_back(ConfirmedNesting("mC", "oI", "O1B", mCSiblings, { "M1B" }));
   facts.push_back(ConfirmedNesting("mC", "oI", "O2", mCSiblings, { "M2A" }));
   facts.push_back(ConfirmedNesting("mC", "oI", "O3", mCSiblings, { "M1B" }));

   // mC -> oF (item 8): O1A nests M1A and M1B; M2A excluded -- still
   // informative (M2A is a dead end for this child specifically).
   facts.push_back(ConfirmedNesting("mC", "oF", "O1A", mCSiblings, { "M1A", "M1B" }));

   // mC -> hR (item 8, extended 2026-08-29 to the full 5-type mC family):
   // R1 nests M1A only. R3's mystery is SOLVED -- it nests M3, one of the
   // two lower-stratum types not shown in the ellipse (same category as
   // O3/A2/A3). Neither M2B nor any of M1A/M1B/M2A nest R3.
   const std::vector<std::string> mCFullFamily = { "M1A", "M1B", "M2A", "M2B", "M3" };
   facts.push_back(ConfirmedNesting("mC", "hR", "R1", mCFullFamily, { "M1A" }));
   facts.push_back(ConfirmedNesting("mC", "hR", "R3", mCFullFamily, { "M3" }));

   // oI -> tI (item 8, extended 2026-08-29 to include O3): clean 1-1 split
   // between O1B/T1 and O2/T2, unlike mC->oI's shape -- O3 is a genuine
   // dead end here (nests neither T1 nor T2), unlike M3's positive result
   // for mC->hR/R3. Two different lower-stratum types, two different
   // outcomes.
   const std::vector<std::string> oIFullFamily = { "O1B", "O2", "O3" };
   facts.push_back(ConfirmedNesting("oI", "tI", "T1", oIFullFamily, { "O1B" }));
   facts.push_back(ConfirmedNesting("oI", "tI", "T2", oIFullFamily, { "O2" }));

   // oF -> tI (item 8, 2026-08-29): O1A is oF's only Delone type, and it
   // nests BOTH T1 and T2 -- excludes nothing, same shape as tI->cF/C3.
   // No label rendered, but now a confirmed result, not an untested gap.
   const std::vector<std::string> oFSiblings = { "O1A" };
   facts.push_back(ConfirmedNesting("oF", "tI", "T1", oFSiblings, { "O1A" }));
   facts.push_back(ConfirmedNesting("oF", "tI", "T2", oFSiblings, { "O1A" }));

   // tI -> cI (item 8): only T1 nests C1 -- T2 excluded, informative.
   facts.push_back(ConfirmedNesting("tI", "cI", "C1", tISiblings, { "T1" }));

   // tI -> cF (item 8): BOTH T1 and T2 nest C3 -- excludes nothing.
   // Kept as a record that the audit ran and found no problem, but
   // ExcludesNothing() is true here, so GrimmerDeloneTree doesn't draw
   // a label for it: neither sibling is wrong for this child, so there
   // is nothing for the chain-check to get wrong here.
   facts.push_back(ConfirmedNesting("tI", "cF", "C3", tISiblings, { "T1", "T2" }));

   // aP -> mP / aP -> mC (item 8, 2026-08-29): A2/A3 are lower-dimensional
   // boundary strata within aP, not co-equal siblings of A1 (see the note
   // on m_btype_aP in GrimmerTree.cpp) -- but the audit was still run
   // against all three, and every result here excludes at least one of
   // them, so all four facts are informative.
   const std::vector<std::string> aPSiblings = { "A1", "A2", "A3" };
   facts.push_back(ConfirmedNesting("aP", "mP", "M4", aPSiblings, { "A1" }));
   facts.push_back(ConfirmedNesting("aP", "mC", "M1A", aPSiblings, { "A1" }));
   facts.push_back(ConfirmedNesting("aP", "mC", "M1B", aPSiblings, { "A1" }));
   facts.push_back(ConfirmedNesting("aP", "mC", "M2A", aPSiblings, { "A1", "A2" }));

   return facts;
}

#endif // CONFIRMEDDELONENESTING_H
