// RunItem8NestingAudit.h
//
// Runs TestDeloneNesting::NestsInside over every still-unaudited pair
// SELLA item 8 covers: mC's three outgoing edges to oI/oF/hR, oI's own
// O1B/O2 ambiguity against tI, and tI's own T1/T2 ambiguity against
// cI/cF. 23 pairs total, none yet in ConfirmedDeloneNesting.h.
//
// One thing this will surface that ConfirmedDeloneNesting.h's current
// struct doesn't handle yet: mC->oC only needed one child (O4), but
// oI/tI have two Delone siblings each, so "M1A nests into oI" is really
// a question per child sibling (does M1A nest O1B? O2? both? neither?).
// The printout below reports at that full (parent sibling x child
// sibling) grain -- once real results come back, ConfirmedNesting may
// need a childDeloneType field rather than just childBravais, depending
// on what the results actually look like. Deliberately not guessing at
// that schema change ahead of having real data to shape it.

#ifndef RUNITEM8NESTINGAUDIT_H
#define RUNITEM8NESTINGAUDIT_H

#include "TestDeloneNesting.h"

#include <iostream>
#include <string>
#include <vector>

struct NestingAuditEdge {
   std::string parentBravais;
   std::string childBravais;
   std::vector<std::string> parentSiblings;
   std::vector<std::string> childSiblings;

   NestingAuditEdge(
      const std::string& parent,
      const std::string& child,
      const std::vector<std::string>& pSiblings,
      const std::vector<std::string>& cSiblings)
      : parentBravais(parent), childBravais(child)
      , parentSiblings(pSiblings), childSiblings(cSiblings) {
   }
};

inline std::vector<NestingAuditEdge> CreateItem8AuditEdges() {
   std::vector<NestingAuditEdge> edges;
   edges.push_back(NestingAuditEdge("mC", "oI", { "M1A", "M1B", "M2A" }, { "O1B", "O2", "O3" }));
   edges.push_back(NestingAuditEdge("mC", "oF", { "M1A", "M1B", "M2A" }, { "O1A" }));
   // mC->hR extended to M2B/M3 (2026-08-29): R3 came back "none confirmed"
   // against M1A/M1B/M2A. M2B and M3 are real mC-family Delone types
   // (m_freeParams=3, a genuinely lower stratum -- not co-equal siblings,
   // same category as O3/A2/A3) that were never tested against R3 at all.
   // Testing them here doesn't require adding them to the ellipse; it's
   // just the one missing data point needed to settle whether R3's real
   // nesting parent is one of these two, or whether the P*Q=Q test simply
   // doesn't apply to this edge (mC->hR is conjugate and skips a level --
   // see the earlier tI->cF discussion for the same caveat).
   edges.push_back(NestingAuditEdge("mC", "hR", { "M1A", "M1B", "M2A", "M2B", "M3" }, { "R1", "R3" }));
   // oI's own O3 included (2026-08-29) -- the same "M3 solved the R3
   // mystery" pattern applies here: O3 was never tested against tI's
   // siblings, or as a possible target for mC's siblings, before now.
   edges.push_back(NestingAuditEdge("oI", "tI", { "O1B", "O2", "O3" }, { "T1", "T2" }));
   // Speculative: O3 has m_freeParams=2, the same level as R1/R3 (hR) --
   // one full level below O1B/O2's 3, not "between" them in their own
   // parameter space. M3 (mC's own lower-stratum type) turned out to
   // bypass its normal siblings' children entirely and nest directly into
   // hR instead; testing whether O3 does the same thing, even though
   // oI->hR isn't a normal edge in this diagram.
   edges.push_back(NestingAuditEdge("oI", "hR", { "O1B", "O2", "O3" }, { "R1", "R3" }));
   // oF->tI: never tested. oF has only one Delone type (O1A), so there's
   // no sibling-ambiguity question on the oF side, but the same "does the
   // parent's type nest one or both of tI's two siblings" question that
   // oI->tI answers hasn't been asked here at all.
   edges.push_back(NestingAuditEdge("oF", "tI", { "O1A" }, { "T1", "T2" }));
   edges.push_back(NestingAuditEdge("tI", "cI", { "T1", "T2" }, { "C1" }));
   edges.push_back(NestingAuditEdge("tI", "cF", { "T1", "T2" }, { "C3" }));
   // aP's two outgoing edges. A2/A3 are lower-dimensional boundary strata
   // within aP, not co-equal siblings of A1 (see the note in GrimmerTree.cpp
   // on m_btype_aP) -- but included anyway, per explicit request, so the
   // ellipse's "via" labels can be driven by real data rather than left
   // unlabeled.
   edges.push_back(NestingAuditEdge("aP", "mP", { "A1", "A2", "A3" }, { "M4" }));
   edges.push_back(NestingAuditEdge("aP", "mC", { "A1", "A2", "A3" }, { "M1A", "M1B", "M2A" }));
   return edges;
}

inline void RunItem8NestingAudit() {
   const std::map<std::string, MatS6> projectors = TestDeloneNesting::BuildProjectorMap();
   const std::vector<NestingAuditEdge> edges = CreateItem8AuditEdges();

   std::cout << "; ---- SELLA item 8: Delone nesting audit ----" << std::endl;

   for (const auto& edge : edges) {
      std::cout << "; " << edge.parentBravais << " -> " << edge.childBravais << std::endl;
      for (const auto& child : edge.childSiblings) {
         for (const auto& parent : edge.parentSiblings) {
            const bool nests = TestDeloneNesting::TestPair(projectors, parent, child);
            std::cout << ";   " << parent << " nests " << child << "? "
               << (nests ? "YES" : "no") << std::endl;
         }
      }
   }

   std::cout << "; ---- end item 8 audit ----" << std::endl;
}

#endif // RUNITEM8NESTINGAUDIT_H