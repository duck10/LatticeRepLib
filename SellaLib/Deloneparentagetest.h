#ifndef DELONE_PARENTAGE_TEST_H
#define DELONE_PARENTAGE_TEST_H

// DeloneParentageTest.h
//
// 2026-11-04 -- TestDeloneParentage REPLACED (sixth revision). The VCP
// search below gave false "nested" answers: CHECKNESTING reported 18
// violating pairs, and for every one of them none of the child's
// arrangements lies on any arrangement of the parent (checked against
// LabeledSellaMatrices.cpp plus the Appendix B face arrangements), while
// all 117 other pairs are genuinely nested. Cause: a VCP candidate
// vcp_i = R_i*pbi - pai is p MIRRORED through s_i = 0, not another
// representation of p's lattice unless s_i = 0 (the same flaw already
// fixed in the fitting search -- see BuildExactBoundaryCandidates in
// RemediatedFit.h). E.g. for an O3 point (a,b,0,a,b,0), vcp_1 =
// (-a,b,b,a,0,0) lies on O4's canonical arrangement 00abbc, but it is not
// a reduced form of that lattice (its first scalar -a is positive).
//
// New rule (white paper v4): a reduced sample of the child is a parent
// lattice iff it lies on some arrangement of the parent's COMPLETE list --
// its own projectors plus its face arrangements (FaceCrossedArrangements.h).
// No candidate search is needed: the complete list is closed under the
// reflections and the face permutations, so every reduced form of every
// parent lattice is already on it. The face arrangements are required here
// regardless of the FACECROSSED switch, since nesting is geometry, not a
// fitting option: without them 15 genuine pairs are missed (e.g. O3 in M1A
// and M2A, C5 in R3, M3 in M2A). Checked offline against all 135 pairs
// CHECKNESTING tests: 117 nested, 18 not, matching the arrangement rule.
//
// The VCP candidate functions (BuildVCPCandidates, VCPResidualSearch) were
// removed 2026-11-07: nothing called them once TestDeloneParentage stopped
// using them. The older design notes for the VCP method went with them.

#include "FaceCrossedArrangements.h"
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
   double residual;                    // best (smallest) residual over the
   // parent's complete projector list; ~0 if nested
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
// false results). Still used elsewhere (TestHarness.h's
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

// CompleteProjectorList -- a type's own projectors (m_sellaProjectors) plus
// its face arrangements from white paper v5, Appendix B.
inline std::vector<MatS6> CompleteProjectorList(const GenerateDeloneBase& type) {
   std::vector<MatS6> prjs = type.GetSellaProjectors();
   const FaceCrossedMatrices& fc = GetFaceCrossedMatrices(type.GetName());
   prjs.insert(prjs.end(), fc.prjs.begin(), fc.prjs.end());
   return prjs;
}

// TestDeloneParentage -- does childType genuinely nest inside parentType?
// (2026-11-04 rule; see the file header.) The child sample is reduced by
// construction: GetValidTypeSample projects a Selling-reduced vector onto
// the child's canonical arrangement, and averaging non-positive scalars
// keeps them non-positive, with zero slots exactly 0. It is a generic member
// of the child type (random letter values), so if it lies on a parent
// arrangement, every child lattice does. tol is an absolute residual on
// that one sample; a nested pair gives ~1e-15.
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

   double best = -1.0;
   for (const auto& prj : CompleteProjectorList(parent)) {
      const double r = (childExample - prj * childExample).norm();
      if (best < 0.0 || r < best) best = r;
   }
   const bool nested = (best >= 0.0 && best < tol);
   return DeloneParentageResult(parent.GetName(), child.GetName(), best, nested);
}

#endif  // DELONE_PARENTAGE_TEST_H
