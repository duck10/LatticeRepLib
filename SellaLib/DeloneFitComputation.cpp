// DeloneFitComputation.cpp
//
// Real definitions for DeloneFitComputation.h's ComputeDeloneFits, moved
// out of CmdSella.cpp (where they used to be the ONLY definition, per
// that file's own now-removed comment) so a second program -- CellClassify
// -- can actually link against them.
//
// This was NOT caught by DeloneFitFiltering.h's earlier extraction or by
// CmdSellaControls.h's move into SellaLib, because FlexibleSellaTestMode.h
// (the only other caller of ComputeDeloneFits) is #include'd directly into
// CmdSella.cpp -- it never crossed a real executable boundary before.
// CellClassify.cpp is the first thing that ever has, which is exactly why
// this gap only surfaced now as a link error rather than at either of the
// earlier compile stages.
//
// RemediateIfShortlistChanges and SearchForToCanon moved here too, since
// ComputeDeloneFits calls both directly and neither has any other call
// site left in CmdSella.cpp once ComputeDeloneFits itself moves out --
// both kept internal (static) rather than added to DeloneFitComputation.h,
// since nothing outside this file needs to call them individually.
//
// selectBravaisCase (a CmdSella.cpp global, always "" and never
// reassigned anywhere in that file) is inlined as a literal "" below
// rather than carried over as a second copy of a shared mutable global --
// simpler, and there was never a second value it actually took.

#include "DeloneFitComputation.h"
#include "DeloneFitFiltering.h"
#include "GrimmerTree.h"
#include "LabeledSellaMatrices.h"
#include "LatticeConverter.h"
#include "MatS6.h"
#include "RemediatedFit.h"
#include "S6.h"
#include "Sella.h"

#include <iostream>
#include <set>
#include <string>
#include <vector>

// RemediateIfShortlistChanges -- computes remediated fit for every type in
// vDeloneFitResultsForOneInputLattice, then checks whether the reported
// shortlist (GetMatchedTypeNames, the same threshold test ListMatchingTypes
// uses to decide what gets displayed) would differ if the remediated fits
// were used instead of the raw ones. Remediated fit is provably <= raw fit
// (Remediation tries the naive orientation plus 6 boundary-crossed
// alternatives and keeps the best), so this can only add a correctly-missed
// type to the shortlist or tighten an existing one, never wrongly demote a
// genuine match -- adopting it is always safe.
//
// Only ADOPTED when it actually changes the shortlist, so unaffected inputs
// pay the compute cost but produce identical output and no log noise.
// Returns the fit vector to use going forward: either the original
// (unchanged) vector, or the fully-remediated vector, never a mix -- so
// downstream code (SearchForToCanon, the Grimmer-chain loop, printing)
// always sees one internally consistent set of fits.
static std::vector<DeloneFitResults> RemediateIfShortlistChanges(
   const std::vector<DeloneFitResults>& vDeloneFitResultsForOneInputLattice,
   const S6& oneLattice,
   const CmdSellaControls& controls,
   const LatticeCell& input) {

   std::vector<DeloneFitResults> remediatedFitResults;
   remediatedFitResults.reserve(vDeloneFitResultsForOneInputLattice.size());
   for (const auto& f : vDeloneFitResultsForOneInputLattice) {
      remediatedFitResults.emplace_back(
         Remediation(oneLattice, f.GetGeneralType(), -1.0));
   }

   const double latticeNorm = oneLattice.norm();
   const double maxDeltaForMatch = controls.getMaxDeltaForMatch();
   const std::set<std::string> rawMatchNames = GetMatchedTypeNames(
      FilterForBestExample(vDeloneFitResultsForOneInputLattice), latticeNorm, maxDeltaForMatch);
   const std::set<std::string> remediatedMatchNames = GetMatchedTypeNames(
      FilterForBestExample(remediatedFitResults), latticeNorm, maxDeltaForMatch);

   if (remediatedMatchNames == rawMatchNames) {
      return vDeloneFitResultsForOneInputLattice;
   }

   if (controls.shouldDebug()) {
      std::cout << "; remediation changed shortlist for " << input.GetInput()
         << " -- raw: ";
      for (const auto& n : rawMatchNames) std::cout << n << " ";
      std::cout << " remediated: ";
      for (const auto& n : remediatedMatchNames) std::cout << n << " ";
      std::cout << std::endl;
   }
   return remediatedFitResults;
}

static void SearchForToCanon(const std::vector<DeloneFitResults>& vfit) {
   std::vector<LabeledSellaMatrices> matrices = LabeledSellaMatrices().GetPrjs();
   static const std::vector<MatS6> refl_one = MatS6::GetReflections();
   std::vector<MatS6> prjs;
   bool matchprj = false;
   for (const auto& f : vfit) {
      const S6 bestfit = f.GetBestFit();
      const S6 inputVector = f.GetOriginalInput();
      bool match = false;
      matchprj = false;
      const auto name = f.GetGeneralType();
      for (const LabeledSellaMatrices& m : matrices) {
         if (name == m.GetLabel()) {
            match = true;
            matchprj = true;
            prjs = m.GetMatrices();
            for (const auto& prj : refl_one) {
               const S6 projected = prj * inputVector;
               const double test = (bestfit - projected).norm();
               if (test < 1.0E-8) {
                  matchprj = true;
               }
            }
            const int i19191 = 19191;
         }
      }
      if (!matchprj) {
         // did not find the name
         throw("did not find the name");
      }
   }
}

// ComputeDeloneFits -- computes the Delone/Bravais type fits for one input
// cell, including Grimmer-chain violation detection and remediation.
//
// Factored out of ProcessSella (CmdSella.cpp) so the fit computation can
// be reused (by COMPARE mode, by CmdSella's own tests, and now by
// CellClassify) without paying for or emitting the per-cell
// graphics/printing that ProcessSella also does.
//
// NOTE: remediation now loops (while) rather than firing once (if), so a
// cell with more than one simultaneous Grimmer-chain violation gets every
// fixable violation repaired, not just the first one detected. The pass
// count is bounded by maxRemediationPasses as a safety cap, in case some
// violation is not actually resolvable by Remediation and would otherwise
// keep gcs.HasFailure() == true forever.
DeloneFitComputation ComputeDeloneFits(
   const LatticeCell& input,
   const CmdSellaControls& controls,
   Sella& sella)
{
   MatS6 oneReductionMatrix;
   const S6 oneLattice = GetInputSellingReducedVectors(input, oneReductionMatrix);
   const S6 oneErrors = 0.1 * input.getCell();

   if (controls.shouldDebug()) {
      std::cout << "; DEBUG oneLattice (post-SellingReduceCell, this is what SellaFit actually sees): "
         << oneLattice << std::endl;
   }

   if (controls.shouldDebug()) {
      const auto perps = sella.GetPerps();
      const auto prjs = sella.GetProjectors();
      if (perps.size() != prjs.size()) {
         std::cout << "ERROR: the sizes of perps and projectors do not match" << std::endl;
         std::cout << "size of pjrs  " << prjs.size() << std::endl;
         std::cout << "size of perps " << perps.size() << std::endl;
      }
      else {
         std::cout << "size(prjs)   size(perps)" << std::endl;
         for (size_t i = 0; i < prjs.size(); ++i) {
            std::cout << prjs[i].size() << "    " << perps[i].size() << std::endl;
         }
      }
   }

   // selectBravaisCase in CmdSella.cpp was always "" (never reassigned
   // anywhere in that file) -- inlined directly rather than carried over
   // as a second copy of a shared mutable global that only ever held one
   // value.
   std::vector<DeloneFitResults> vDeloneFitResultsForOneInputLattice =
      sella.SellaFit("", oneLattice, oneErrors, oneReductionMatrix);


   // NEW: remediate every type and adopt the correction only if it changes
// the reported shortlist. See RemediateIfShortlistChanges for why this
// is always safe (remediated fit <= raw fit) and why it's gated rather
// than unconditional (no log noise / no behavior change on unaffected
// inputs).
   vDeloneFitResultsForOneInputLattice = RemediateIfShortlistChanges(
      vDeloneFitResultsForOneInputLattice, oneLattice, controls, input);


   MapOFDeloneFits theDelonefits;
   theDelonefits.CreateMapOFDeloneFits(vDeloneFitResultsForOneInputLattice);
   MapOfBravaisFits theBravaisfits;
   theBravaisfits.CreateMapOFBravaisFits(vDeloneFitResultsForOneInputLattice);
   SearchForToCanon(vDeloneFitResultsForOneInputLattice);

   GrimmerChains gcs(oneLattice);
   gcs.CreateGrimmerChains(theDelonefits, theBravaisfits);
   gcs.updateChains(theDelonefits, theBravaisfits);
   gcs.CheckAllGrimmerChains();

   // TEMPORARY: gated on shouldDebug() (DETAILS) rather than
   // shouldShowChainFailures() (SHOWCHAINFAILURES), since only DETAILS has
   // been merged into the live CmdSellaControls.h so far. Switch all four
   // sites below back to shouldShowChainFailures() once SHOWCHAINFAILURES
   // is merged, so this output doesn't require enabling all of DETAILS's
   // other (much noisier) output just to see chain-failure detail.
   if (controls.shouldDebug()) {
      if (gcs.HasFailure()) {
         const auto failures = gcs.GetFirstFailure().GetFailures();
         std::cout << "; Grimmer chain violation (before remediation) for "
            << input.GetInput() << ":" << std::endl;
         for (const auto& f : failures) {
            std::cout << ";   " << f.first << " = " << f.second << std::endl;
         }
      }
      else {
         std::cout << "; No Grimmer chain violation for "
            << input.GetInput() << std::endl;
      }
   }

   static const int maxRemediationPasses = 10; // generous cap; no catalogued
   // Delone type has more than a handful of structural zeros, so a
   // well-formed input should never need anywhere near this many passes.
   // Also serves as a hard backstop now that CheckAllGrimmerChains can be
   // re-run mid-loop (via ReplaceRemediation) and could in principle keep
   // surfacing new violations pass after pass.
   int remediationPasses = 0;
   std::string lastRepairedType;
   double lastRepairedFit = -1.0;
   std::vector<S6> lastRepairedCandidates;  // winning point(s) from the
   // previous pass, fed forward as extra seeds -- see Remediation168's
   // extraSeeds parameter for why.
   bool madeProgress = true;
   while (gcs.HasFailure() && remediationPasses < maxRemediationPasses && madeProgress) {
      GrimmerChainFailure gcf = gcs.GetFirstFailure();
      const std::vector<std::pair<std::string, double>> firstFail = gcf.GetFailures();
      const std::string& targetType = firstFail[1].first;
      if (controls.shouldDebug()) {
         std::cout << ";   remediation pass " << (remediationPasses + 1)
            << ": repairing " << targetType
            << " (fit " << firstFail[1].second << ")" << std::endl;
      }
      const DeloneFitResults revisedFit = Remediation168(oneLattice, targetType,
         firstFail[1].second, 10, lastRepairedCandidates);

      // "No progress" means Remediation was asked to fix the SAME type
      // again and returned the SAME fit (within numerical noise) as last
      // time -- i.e. it has genuinely exhausted its search for that
      // target, not just that this pass's fit differs numerically from
      // whatever unrelated type was repaired in a previous pass. A newly
      // surfaced, different violation (different targetType) is always
      // worth attempting, regardless of how its fit compares to the last
      // pass's target.
      const double newFit = revisedFit.GetRawFit();
      madeProgress = (targetType != lastRepairedType) ||
         (remediationPasses == 0 && std::abs(newFit - firstFail[1].second) > 1.0E-8) ||
         (remediationPasses > 0 && std::abs(newFit - lastRepairedFit) > 1.0E-8);

      if (madeProgress) {
         vDeloneFitResultsForOneInputLattice.emplace_back(revisedFit);
         gcs = gcs.ReplaceRemediation(revisedFit);
         lastRepairedType = targetType;
         lastRepairedFit = newFit;
         lastRepairedCandidates = { revisedFit.GetOriginalInput() };
      }
      else if (controls.shouldDebug()) {
         std::cout << ";   remediation pass " << (remediationPasses + 1)
            << " made no progress (fit unchanged at " << newFit
            << ") -- stopping" << std::endl;
      }
      ++remediationPasses;
   }


   if (controls.shouldDebug() && remediationPasses > 0) {
      if (gcs.HasFailure()) {
         const auto failuresAfter = gcs.GetFirstFailure().GetFailures();
         std::cout << "; Grimmer chain violation REMAINS after "
            << remediationPasses << " remediation pass(es) for "
            << input.GetInput() << ":" << std::endl;
         for (const auto& f : failuresAfter) {
            std::cout << ";   " << f.first << " = " << f.second << std::endl;
         }
      }
      else {
         std::cout << "; Grimmer chain violation resolved after "
            << remediationPasses << " remediation pass(es) for "
            << input.GetInput() << std::endl;
      }
   }
   if (remediationPasses >= maxRemediationPasses && controls.shouldDebug()) {
      std::cout << "; WARNING: remediation pass cap reached for input "
         << input.GetInput() << " -- a chain violation may remain unresolved"
         << std::endl;
   }

   return DeloneFitComputation{ vDeloneFitResultsForOneInputLattice, gcs };
}