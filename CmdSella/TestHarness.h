#ifndef TEST_HARNESS_H
#define TEST_HARNESS_H

// TestHarness.h
//
// The CHECKNESTING diagnostic (CmdSellaControls.h): RunRandomSeedSpotCheck
// and RunNestingConsistencyCheck, with their helpers SeededPerturbation and
// FitsForPair. Not part of the fit pipeline.
//
// 2026-11-07: cut down to these four functions. The rest of the file
// (August 2026 boundary-tie searches, sweeps, and comparisons of the
// Remediation variants removed from RemediatedFit.h) was no longer called.

#include "CmdSellaControls.h"
#include "DeloneFitComputation.h"
#include "DeloneFitResults.h"
#include "DeloneParentageTest.h"
#include "ExpandAcrossBoundaryZeros.h"
#include "GenerateLatticeTypeExamples.h"
#include "InputHandler.h"
#include "LatticeCell.h"
#include "LRL_Cell.h"
#include "S6.h"
#include "Sella.h"

#include <exception>
#include <iostream>
#include <sstream>
#include <string>

// RunRandomSeedSpotCheck -- minimal, direct proof that S6::SetSeed()
// actually changes what S6::rand() produces, for whatever seed is
// currently active. Prints the RAW drawn sample point itself, not a
// derived residual -- a residual for a genuinely-nested pair can stay
// near-zero regardless of seed (that's the point of the nesting being a
// whole-subspace property), so it wouldn't visibly prove anything
// changed. The sample point itself will be a completely different S6
// vector for a different seed, unambiguous either way. Run this twice
// with two different RANDOMSEED values (BaseControlVariables.hs) and diff
// the printed sample -- if they're identical, the seed genuinely isn't
// taking effect; if they differ, it is.
inline void RunRandomSeedSpotCheck() {
   std::cout << "; RunRandomSeedSpotCheck: entered" << std::endl;
   const S6 sample = GetValidTypeSample(Delone_O4().GetPrj());
   std::cout << ";   drawn sample of O4's subspace (should differ between"
      << " different RANDOMSEED values): " << sample << std::endl;
}

// SeededPerturbation -- one random perturbation of base, of length
// fraction * |base|, orthogonal to base. The direction comes from
// S6::rand(), so it follows the SEED control (BaseControlVariables.h
// calls S6::SetSeed); CmdPerturb's own generator (RHrand) does not.
// The difference of two draws gives a direction symmetric about zero.
// Returns false if no valid cell was found in maxAttempts tries.
inline bool SeededPerturbation(const S6& base, const double fraction, S6& out,
   const size_t maxAttempts = 100) {
   const double baseNorm = base.norm();
   if (baseNorm <= 0.0) return false;
   for (size_t attempt = 0; attempt < maxAttempts; ++attempt) {
      const S6 direction = S6::rand() - S6::rand();
      double dot = 0.0;
      for (size_t i = 0; i < 6; ++i) dot += direction[i] * base[i];
      const S6 ortho = direction - (dot / (baseNorm * baseNorm)) * base;
      const double orthoNorm = ortho.norm();
      if (orthoNorm < 1.0E-12) continue;
      S6 trial = base + (fraction * baseNorm / orthoNorm) * ortho;
      trial.SetValid(true);
      if (LRL_Cell(trial).IsValid()) {
         out = trial;
         return true;
      }
   }
   return false;
}

// FitsForPair -- raw fits of parent and child for one S6 point, through
// the same parser and fit pipeline as typed input. Returns false if
// either type is missing from the results.
inline bool FitsForPair(const S6& point, const std::string& parentName,
   const std::string& childName, const CmdSellaControls& controls, Sella& sella,
   double& fitParent, double& fitChild) {
   std::ostringstream cellStr;
   cellStr << "S " << point;
   const LatticeCell probeCell = InputHandler::processSingleLatticeInput(cellStr.str());
   const DeloneFitComputation probeFits = ComputeDeloneFits(probeCell, controls, sella);
   fitParent = -1.0;
   fitChild = -1.0;
   for (const auto& f : probeFits.fits) {
      if (f.GetGeneralType() == parentName) fitParent = f.GetRawFit();
      if (f.GetGeneralType() == childName) fitChild = f.GetRawFit();
   }
   return fitParent >= 0.0 && fitChild >= 0.0;
}

// RunNestingConsistencyCheck -- general form of the O1B/O2/O3 check that
// started the boundary investigation (August 2026). For every pair of
// Delone types (P, C) where C has fewer free parameters than P, asks
// TestDeloneParentage (DeloneParentageTest.h) whether C nests in P, i.e.
// whether a generic C sample lies on an arrangement of P's complete list
// (library plus face arrangements). No hand-picked list of pairs.
//
// For each nested pair, builds an example of C (BuildCharacterExample),
// expands it across its zero boundaries (ExpandAcrossBoundaryZeros) to get
// every reduced representative, and checks fit(P) <= fit(C) + epsilon on
// each one, using the production fits (ComputeDeloneFits). epsilon is
// controls.getNestingDelta() times the representative's norm.
//
// Perturbed phase (2026-11-04): each representative also gets
// nPerturbations random 1% perturbations (SeededPerturbation, controlled
// by SEED). Nesting is a set property -- if C lies inside P, every point
// is at least as close to P as to C -- so fit(P) <= fit(C) must hold off
// the boundary too. This is the check that would have caught the O2/O3
// gap: the exact representatives passed, the perturbed O3 cells did not.
//
// Enabled via the CHECKNESTING control (CmdSellaControls.h) -- off by
// default, so this needs no attention during normal use.
inline void RunNestingConsistencyCheck(const CmdSellaControls& controls, Sella& sella) {
   std::cout << "; RunNestingConsistencyCheck: entered" << std::endl;
   const auto allTypes = GenerateDeloneBase().Select("");
   std::cout << "; RunNestingConsistencyCheck: got " << allTypes.size()
      << " Delone types" << std::endl;

   size_t pairsNested = 0;
   size_t pairsTested = 0;
   size_t representativesTested = 0;
   size_t violatingPairs = 0;
   size_t errors = 0;

   const size_t nPerturbations = 3;
   const double perturbFraction = 0.01;
   const size_t maxPrintedPerPair = 3;
   size_t perturbedTested = 0;
   size_t perturbedFailedToGenerate = 0;
   size_t perturbedViolations = 0;
   size_t perturbedViolatingPairs = 0;

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
            bool pairPerturbedViolated = false;
            size_t printedThisPair = 0;

            for (const auto& representative : crossing.representatives) {
               // FitsForPair goes through InputHandler::processSingleLatticeInput,
               // the console's own "S ..." parser (LatticeCell's string
               // constructor doesn't recognize "S"; chat 2026-08-15).
               double fitParent = -1.0;
               double fitChild = -1.0;
               // Type not present in this representative's fit results --
               // skip this representative rather than guess.
               if (!FitsForPair(representative, parent->GetName(), child->GetName(),
                  controls, sella, fitParent, fitChild)) continue;

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

               for (size_t k = 0; k < nPerturbations; ++k) {
                  S6 perturbed;
                  if (!SeededPerturbation(representative, perturbFraction, perturbed)) {
                     ++perturbedFailedToGenerate;
                     continue;
                  }
                  double pertParent = -1.0;
                  double pertChild = -1.0;
                  if (!FitsForPair(perturbed, parent->GetName(), child->GetName(),
                     controls, sella, pertParent, pertChild)) continue;
                  ++perturbedTested;
                  if (pertParent > pertChild + epsilon) {
                     ++perturbedViolations;
                     pairPerturbedViolated = true;
                     if (printedThisPair < maxPrintedPerPair) {
                        ++printedThisPair;
                        std::cout << "; PERTURBED NESTING VIOLATION: parent=" << parent->GetName()
                           << " child=" << child->GetName()
                           << " cell=S " << perturbed
                           << " fit(parent)=" << pertParent
                           << " fit(child)=" << pertChild
                           << " gap=" << (pertParent - pertChild) << std::endl;
                     }
                  }
               }
            }

            if (pairTested) ++pairsTested;
            if (pairViolated) ++violatingPairs;
            if (pairPerturbedViolated) ++perturbedViolatingPairs;
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
   std::cout << "; CHECKNESTING perturbed: " << perturbedTested
      << " perturbed cell(s) tested (" << nPerturbations << " per representative, "
      << perturbFraction * 100.0 << "% of |s6|), "
      << perturbedViolations << " violation(s) in "
      << perturbedViolatingPairs << " pair(s), "
      << perturbedFailedToGenerate << " could not be generated" << std::endl;
}

#endif  // TEST_HARNESS_H
