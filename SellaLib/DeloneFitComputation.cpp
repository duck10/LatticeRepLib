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
// 2026-10-09 -- UNGATED SEARCH. ComputeDeloneFits used to compute fits in
// three layers: a raw SellaFit pass (baked per-type projectors applied to
// the one Selling-reduced representative), RemediateIfShortlistChanges
// (36-candidate 2-hop Remediation, adopted only when the reported
// shortlist changed), and a reactive repair loop that ran Remediation168
// only on a type named by a Grimmer-chain violation. Chain checks compare
// Bravais nodes, and a Bravais node takes its best Delone sibling, so a
// type that fits badly next to a well-fitting sibling never triggered a
// violation and never got the full search.
//
// Confirmed case: exact O3 cell P 9.103 9.103 9.103 119.504 120.498 90.000
// (Test 23, cell 1). Main pipeline gave O2 = 1.24496 (unchanged since
// Aug 2026); SEQUENCE 168 alone gives O2 = 0.000613, below O3 (0.000866).
// The method was sufficient; the gate kept it from running.
//
// Now every type gets Remediation168ForOneSubtype (VCP candidates plus all
// 36 two-hop boundary-transform products, iterated with re-reduction)
// unconditionally and independently. This is the same per-type loop as
// DirectClassify.h's PopulateDirectFitsForAllTypes, moved into the main
// pipeline so COMPARE, TYPES, the SVGs and the test suite all use it.
// Remediation168 starts from the raw SellaFitXXXXXX fit and only accepts
// improvements, so no type's fit can be worse than the old raw value.
// After the independent pass, cross-seeding passes rerun every type with
// all types' winning candidates as extra seeds (see
// SearchAllTypesWithCrossSeeding) -- this recovers what the old reactive
// loop's extraSeeds did for one repaired type at a time.
//
// GrimmerChains is still built from the final fits and returned (ProcessSella
// draws from it; tests read HasFailure()), but there is no repair loop any
// more: every type has already had the full search, so a remaining chain
// violation is a real finding (genuine geometric edge case or a gap the
// 168 search itself cannot close), not a trigger for more searching.
//
// SearchForToCanon kept: validation only (throws if a type name is missing
// from LabeledSellaMatrices), does not affect any fit.
//
// selectBravaisCase (a CmdSella.cpp global, always "" and never
// reassigned anywhere in that file) is no longer needed -- the raw
// SellaFit("", ...) call it fed is gone.

#include "DeloneFitComputation.h"
#include "DeloneFitFiltering.h"
#include "GenerateLatticeTypeExamples.h"
#include "GrimmerTree.h"
#include "LabeledSellaMatrices.h"
#include "LatticeConverter.h"
#include "MatS6.h"
#include "RemediatedFit.h"
#include "S6.h"
#include "Sella.h"

#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

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

// SearchAllTypesWithCrossSeeding -- the per-type search itself, before any
// field filling. Returned fits carry each type's own winning transformed
// cell in OriginalInput (as Remediation168ForOneSubtype sets it).
//
// Pass 0: one independent Remediation168ForOneSubtype per type.
//
// Cross-seeding passes (2026-10-09): every type's search is rerun with ALL
// types' current winning candidates as extraSeeds, keeping a result only
// if it improves that type's fit; repeated until a pass improves nothing,
// capped at maxCrossSeedPasses. Reason: a child type's winning
// representation is also a good representation for any parent it nests
// in, and independent per-type search throws that away. Confirmed on
// Test 10 cell 2 (S -38.11642 -43.36206 -0.01851 -38.17840 -43.42403
// -0.04436): independent search gave M4 = 38.0721 above O4 = 33.2674
// (M4 nests O4, so a search failure), while the old reactive loop --
// which seeded Remediation168 with the previously repaired type's winner
// -- reached M4 = 25.6631. This pass generalizes that seeding to all
// types, with no type names or nesting tables involved.
//
// Ungated on purpose: a cheaper "only rerun if some foreign winner scores
// better directly" check would miss seeds that win only after the search
// expands from them, which is plausibly how the old loop found 25.66.
// Cost is at least one extra full pass per cell; revisit with timing data.
static std::vector<DeloneFitResults> SearchAllTypesWithCrossSeeding(
   const S6& oneLattice,
   const std::vector<std::shared_ptr<GenerateDeloneBase>>& sptypes) {

   static const int maxCrossSeedPasses = 3;

   std::vector<DeloneFitResults> fits;
   fits.reserve(sptypes.size());
   for (const auto& sptype : sptypes) {
      fits.emplace_back(Remediation168ForOneSubtype(oneLattice, sptype, 10, {}));
   }

   for (int pass = 0; pass < maxCrossSeedPasses; ++pass) {
      std::vector<S6> winners;
      winners.reserve(fits.size());
      for (const auto& f : fits) {
         winners.push_back(f.GetOriginalInput());
      }

      bool anyImproved = false;
      for (size_t i = 0; i < sptypes.size(); ++i) {
         const DeloneFitResults seeded =
            Remediation168ForOneSubtype(oneLattice, sptypes[i], 10, winners);
         if (seeded.GetRawFit() < fits[i].GetRawFit() - 1.0E-8) {
            fits[i] = seeded;
            anyImproved = true;
         }
      }
      if (!anyImproved) break;
   }
   return fits;
}

// ComputeFullSearchFitsForAllTypes -- runs SearchAllTypesWithCrossSeeding,
// then fills in the fields Sella::SellaFit used to set on top of
// SellaFitXXXXXX (Zscore, ReductionMatrix, Difference, Bravais/Delone/
// General type names), and sets OriginalInput to the shared reduced input
// cell -- SearchForToCanon and PrintCompareSummary both assume
// OriginalInput is identical across every type for one input. Each type's
// own winning transformed cell is kept separately as WinningCandidate.
static std::vector<DeloneFitResults> ComputeFullSearchFitsForAllTypes(
   const S6& oneLattice,
   const S6& oneErrors,
   const MatS6& oneReductionMatrix,
   Sella& sella) {

   static const std::vector<std::shared_ptr<GenerateDeloneBase>> sptypes =
      GenerateDeloneBase().Select("");

   const std::vector<DeloneFitResults> searched = SearchAllTypesWithCrossSeeding(oneLattice, sptypes);

   std::vector<DeloneFitResults> results;
   results.reserve(sptypes.size());

   for (size_t i = 0; i < sptypes.size(); ++i) {
      const std::shared_ptr<GenerateDeloneBase>& sptype = sptypes[i];
      DeloneFitResults fit = searched[i];
      fit.SetWinningCandidate(fit.GetOriginalInput());  // capture before
      // OriginalInput below is repurposed to mean "the shared input cell"

      const double zscore = sella.Zscore(oneLattice - fit.GetBestFit(), oneErrors, oneReductionMatrix)
         * sqrt(static_cast<double>(sptype->GetFreeParams()));
      fit.SetZscore(zscore);
      fit.SetLatticeType(sptype->GetBravaisType());
      fit.SetDeloneType(sptype->GetName());
      fit.SetReductionMatrix(oneReductionMatrix);
      fit.SetBavaisType(sptype->GetBravaisType());
      fit.SetGeneralType(sptype->GetName());
      fit.SetDifference(oneLattice - fit.GetBestFit());
      fit.SetOriginalInput(oneLattice);

      results.emplace_back(fit);
   }
   return results;
}

// ComputeDeloneFits -- computes the Delone/Bravais type fits for one input
// cell. See the file header (2026-10-09) for why every type now gets the
// full search unconditionally and the reactive repair loop is gone.
DeloneFitComputation ComputeDeloneFits(
   const LatticeCell& input,
   const CmdSellaControls& controls,
   Sella& sella)
{
   MatS6 oneReductionMatrix;
   const S6 oneLattice = GetInputSellingReducedVectors(input, oneReductionMatrix);
   const S6 oneErrors = 0.1 * input.getCell();

   if (controls.shouldDebug()) {
      std::cout << "; DEBUG oneLattice (post-SellingReduceCell, starting point for every type's search): "
         << oneLattice << std::endl;
   }

   const std::vector<DeloneFitResults> vDeloneFitResultsForOneInputLattice =
      ComputeFullSearchFitsForAllTypes(oneLattice, oneErrors, oneReductionMatrix, sella);

   // CreateMapOFDeloneFits / CreateMapOFBravaisFits take non-const refs
   // (they do not appear to mutate), so build from a local copy.
   std::vector<DeloneFitResults> fitsForMaps = vDeloneFitResultsForOneInputLattice;
   MapOFDeloneFits theDelonefits;
   theDelonefits.CreateMapOFDeloneFits(fitsForMaps);
   MapOfBravaisFits theBravaisfits;
   theBravaisfits.CreateMapOFBravaisFits(fitsForMaps);
   SearchForToCanon(vDeloneFitResultsForOneInputLattice);

   GrimmerChains gcs(oneLattice);
   gcs.CreateGrimmerChains(theDelonefits, theBravaisfits);
   gcs.updateChains(theDelonefits, theBravaisfits);
   gcs.CheckAllGrimmerChains();

   if (controls.shouldDebug()) {
      if (gcs.HasFailure()) {
         const auto failures = gcs.GetFirstFailure().GetFailures();
         std::cout << "; Grimmer chain violation after full per-type search for "
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

   return DeloneFitComputation{ vDeloneFitResultsForOneInputLattice, gcs };
}