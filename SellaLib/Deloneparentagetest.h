#ifndef DELONE_PARENTAGE_TEST_H
#define DELONE_PARENTAGE_TEST_H

// DeloneParentageTest.h
//
// Deterministic replacement for guessing Delone-type parentage from the
// Delone-grid SVG letter patterns (tried twice, wrong both times -- see
// SELLA_handoff.md item 1, escalation 1). 
// // Answers "does child type C genuinely nest inside parent type P" using
// each type's single, hand-derived, exact m_projector (via
// GenerateDeloneBase::GetPrj()), not the empirically sampled
// m_sellaProjectors library. NOTE (2026-08-20): the original rationale
// here -- that m_sellaProjectors "can be silently incomplete" -- was
// disproven by the 2026-08-19 investigation, which found O2's 6-entry
// m_sellaProjectors library complete (all 24 reflections collapse into
// exactly those 6, zero gap). GetPrj() is still the right choice here
// for a different reason: VCPResidualSearch already reorients the CHILD
// side via 168 candidates, so it only needs one fixed, correct PARENT
// projector to test against, not the parent's full multi-orientation
// library.
//
// Design (fifth revision -- see VCPResidualSearch's own comment for why
// the first four didn't work, and where this one comes from):
//   1. Seed with ONE genuine, validated sample of the child's subspace via
//      GetValidTypeSample(child.GetPrj()) -- a real random Selling-reduced
//      lattice, projected onto the child's subspace, retried until valid
//      (pattern matches CmdGen.cpp's proven TryToGetAGoodProjection).
//   2. VCPResidualSearch: build the 168 candidate representatives via
//      BuildVCPCandidates -- the exact, published algorithm from Andrews,
//      Bernstein & Sauter, "A Space for Lattice Representation and
//      Clustering" (Acta Cryst. A, submitted 2018), Section 4.1, not
//      reverse-engineered here -- and keep the minimum residual against
//      parent's canonical projector.
//   3. Child nests in parent iff the best residual found is ~0.
//
// ASSUMPTIONS NOT YET CONFIRMED AGAINST MatS6.h/.cpp (not posted this
// session) -- check these before compiling:
//   - MatS6::Inverse(M) is a true matrix inverse (confirmed USED elsewhere,
//     e.g. LabeledDeloneTypeMatrices.cpp, but its correctness for a
//     reflection matrix specifically -- i.e. that reflections are
//     invertible, which they must be if they are honest signed-permutation
//     matrices -- is assumed, not verified here).
//   - MatS6::GetReflections() returns the same 24-element group used
//     everywhere else (confirmed called this way in CmdSella.cpp's
//     SearchForToCanon).
//   - S6 has a string constructor taking "d d d d d d" (confirmed used for
//     the "goat" vector in GenerateLatticeTypeExamples.cpp and for
//     LatticeCell fixtures in FlexibleSellaTestMode.h) and supports
//     operator- and .norm() (confirmed used throughout CmdSella.cpp).
//   - MatS6 * MatS6 and MatS6 * S6 are both defined (confirmed: the former
//     in EnhancedS6.cpp's operator*, the latter in CmdSella.cpp's
//     SearchForToCanon).

#include "GenerateLatticeTypeExamples.h"
#include "LRL_Cell.h"
#include "MatS6.h"
#include "S6.h"
#include "Selling.h"

#include <cmath>
#include <map>
#include <sstream>
#include <string>
#include <vector>

// GetValidTypeSample -- draws a genuine, validated random sample of a
// Delone type's subspace: a fresh random S6 (S6::rand(), no shared static
// seed state -- see chat with Claude, 2026-08-15, where SellaBuild::
// MakeSampleType's static-seed determinism was asserted without actually
// checking GenerateRandomLattice's source, then correctly challenged),
// Selling-reduced, then projected onto the type's subspace via its own
// projector. Pattern matches CmdGen.cpp's TryToGetAGoodProjection(const
// MatS6&, ...) -- already the production mechanism for exactly this job.
// That function's loop condition had a real bug (`while ((!IsValid()) ||
// (count > trials))` -- with ||, the trials cap didn't actually cap
// anything once count exceeded it), fixed there directly rather than
// just deviated from here, so both now use a proper && cap.
inline S6 GetValidTypeSample(const MatS6& projector, const int trials = 50) {
   S6 probe;
   probe.SetValid(false);
   int count = 0;
   while (!LRL_Cell(probe).IsValid() && count < trials) {
      const S6 start = S6::rand();
      S6 reduced;
      Selling::Reduce(start, reduced);
      probe = projector * reduced;
      ++count;
   }
   return probe;
}

struct DeloneParentageResult {
   std::string parentType;
   std::string childType;
   double residual;                    // best (smallest) residual found across
   // all 168 VCP candidates; ~0 if nested
   bool isNested;
   bool seedValid = true;              // false if GetValidTypeSample could not
   // produce a valid child sample even
   // after retrying -- isNested should
   // not be trusted when this is false

   DeloneParentageResult(const std::string& parent, const std::string& child,
      const double res, const bool nested)
      : parentType(parent), childType(child), residual(res), isNested(nested) {
   }
};

// BuildCharacterExample -- one concrete, generic S6 point satisfying a
// Delone character pattern (e.g. "rs0 rs0"), using fixed, mutually
// distinct values per letter so no accidental extra symmetry sneaks in.
// Same idea as the "goat" vector in DoesProjectorAgreeWithCharacter,
// extended to cover every letter (r,s,t,u,v,w) that can appear.
//
// No longer used by TestDeloneParentage below (switched to
// GetValidTypeSample -- see that function's comment for why: this has no
// validity check, and a seed that isn't a genuine lattice can produce
// false results). Still used elsewhere (CmdSella.cpp's
// RunNestingConsistencyCheck) for building the actual probe cells fed
// into ComputeDeloneFits/SellaFit when reporting violations against the
// real library -- worth checking whether that call site has the same
// validity risk and should switch too.
inline S6 BuildCharacterExample(const std::string& character) {
   static const std::map<char, double> genericValue = {
      {'r', -13.0}, {'s', -29.0}, {'t', -47.0},
      {'u', -59.0}, {'v', -71.0}, {'w', -83.0}
   };
   std::vector<double> v;
   for (const char c : character) {
      if (c == ' ') continue;
      v.push_back(c == '0' ? 0.0 : genericValue.at(c));
   }
   std::ostringstream os;
   for (size_t i = 0; i < v.size(); ++i) {
      os << v[i];
      if (i + 1 < v.size()) os << " ";
   }
   return S6(os.str());
}

// BuildVCPCandidates -- the exact candidate-generation algorithm from
// Andrews, Bernstein & Sauter, "A Space for Lattice Representation and
// Clustering" (Acta Cryst. A, submitted 2018), Section 4.1 -- not derived
// here, taken directly from the published paper and cross-checked
// numerically before use: the paper's own published R1 ("b.c") boundary
// matrix, written out by hand in the paper as
// [-100000/110000/100010/-100100/101000/100001], was confirmed an EXACT
// match, entry by entry, to Selling::GetBoundaryTransforms()[0] (see chat
// with Claude, 2026-08-15) -- so Ri below is that same, already-verified
// matrix set, not a new guess.
//
// This replaces two earlier, both-guessed constructions (conjugation-based
// BuildFullBoundaryTransformSet, and the ad hoc single/pair
// BestResidualBoundedSearch built on top of it) that were reverse-engineered
// from partial information and got the count right by luck at best (see
// their retired comments in this file's history) while failing the
// self-inverse property the paper actually requires. This one is not
// reverse-engineered -- it is the published method.
//
// For a point p: the paper's "M" (all 24 reflections) applied to p
// directly, plus for each of the 6 boundaries i, one Virtual Cartesian
// Point vcp_i = Ri * pbi - pai, where pai is p with every component
// except i zeroed (the projection ONTO axis i) and pbi = p - pai (the
// projection onto the complementary 5-dimensional boundary, i.e. p with
// component i zeroed) -- then M applied to each vcp_i too. Total:
// 24 + 6*24 = 168 candidates, exactly as the paper states.
//
// The paper's own use of this (Section 4.1) is for POINT-TO-POINT
// distance (comparing two specific lattices, each contributing their own
// candidate set). Our use is different -- testing whether ANY candidate
// representative of one point lies on a fixed target SUBSPACE (a
// projector), not comparing two points -- but the candidate-generation
// half is the same problem (enumerate every representative worth
// checking), so it transfers directly: build the 168 candidates from the
// seed, test each against the target projector, keep the best.
inline std::vector<S6> BuildVCPCandidates(const S6& p) {
   static const std::vector<MatS6> reflections = MatS6::GetReflections();
   static const std::vector<MatS6> boundaryTransforms = Selling::GetBoundaryTransforms();

   std::vector<S6> candidates;
   candidates.reserve(168);

   for (const auto& M : reflections) {
      candidates.push_back(M * p);
   }

   for (size_t i = 0; i < 6; ++i) {
      S6 pai;
      for (size_t k = 0; k < 6; ++k) pai[k] = 0.0;
      pai[i] = p[i];
      const S6 pbi = p - pai;
      const S6 vcp_i = boundaryTransforms[i] * pbi - pai;

      for (const auto& M : reflections) {
         candidates.push_back(M * vcp_i);
      }
   }

   return candidates;
}

// VCPSearchResult -- best residual found across all 168 VCP candidates.
struct VCPSearchResult {
   double residual;
};

inline VCPSearchResult VCPResidualSearch(const S6& seed, const MatS6& targetProjector) {
   const auto candidates = BuildVCPCandidates(seed);

   double best = -1.0;
   for (const auto& c : candidates) {
      const S6 diff = c - targetProjector * c;
      const double r = diff.norm();
      if (best < 0.0 || r < best) best = r;
   }
   return VCPSearchResult{ best };
}

// TestDeloneParentage -- does childType genuinely nest inside parentType?
// See file header for the design. tol governs both
// VCPResidualSearch's early-out and the final "is this nested"
// threshold; 1.0E-8 matches the tolerance already used for the analogous
// check in LabeledDeloneTypeMatrices::AlreadyHasThisProjector.
//
// SEEDING, revised twice (see chat with Claude, 2026-08-15):
//   Round 1: originally seeded from BuildCharacterExample -- fixed,
//   hand-picked values plugged into whatever slots a character string
//   names, with no check that the result is actually a valid lattice.
//   Flagged as a real risk, not a formality: a seed that isn't a genuine
//   lattice makes everything downstream (the boundary crossings, the
//   projector test) explore from a point that may not mean anything
//   physically, which can produce a false "not nested" independent of
//   whether real nesting holds.
//   Round 2: moved to SellaBuild::MakeSampleType, then moved again to
//   GetValidTypeSample (defined above) after MakeSampleType's static-seed
//   determinism was asserted without evidence and then correctly
//   challenged -- rather than resolve that uncertainty, switched to a
//   mechanism with no shared static state to question at all, matching
//   CmdGen.cpp's already-proven TryToGetAGoodProjection pattern. Also
//   drops the SellaBuild.h dependency this file briefly had.
// seedValid on the returned result reports whether a valid seed was ever
// obtained; isNested should not be trusted when it is false.
//
// This also makes the result non-deterministic run-to-run (a different
// random child sample each call) -- accepted deliberately: if the child
// genuinely nests inside the parent as a whole linear subspace (or its
// boundary-crossed orbit), that holds for every point in the subspace,
// not just one, so which particular valid sample gets drawn should not
// change the yes/no conclusion, only which specific residual gets
// reported. A hand-picked point risked the opposite failure mode this
// file already hit once: the O1B/O3 seed happened to trivially satisfy
// O1B's pairing by construction, which was luck, not evidence of a
// general relationship.
//
// TESTING METHOD, revised three times more (see chat with Claude,
// 2026-08-15): went through a projector-conjugation approach, a
// sequential-greedy approach, and an ad hoc single/pair combinatorial
// search before this one -- see BuildVCPCandidates' own comment for the
// full story, including why sequential greedy provably gets trapped in
// local optima on this exact problem (O2/O3), and why the ad hoc search
// was itself replaced once a published, exact source (Andrews, Bernstein
// & Sauter's VCP algorithm) became available instead of continuing to
// guess. This version seeds ONE valid child sample, then hands it to
// VCPResidualSearch, which builds the 168 published candidates and keeps
// the global best.
inline DeloneParentageResult TestDeloneParentage(
   const GenerateDeloneBase& parent,
   const GenerateDeloneBase& child,
   const double tol = 1.0E-8) {

   const S6 childExample = GetValidTypeSample(child.GetPrj());
   const bool seedValid = LRL_Cell(childExample).IsValid();

   if (!seedValid) {
      DeloneParentageResult result(parent.GetName(), child.GetName(), -1.0, false);
      result.seedValid = false;
      return result;
   }

   const VCPSearchResult search = VCPResidualSearch(childExample, parent.GetPrj());
   const bool nested = (search.residual < tol);
   return DeloneParentageResult(parent.GetName(), child.GetName(),
      search.residual, nested);
}

#endif  // DELONE_PARENTAGE_TEST_H