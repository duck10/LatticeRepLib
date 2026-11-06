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
#include "FaceCrossedArrangements.h"
#include "LabeledSellaMatrices.h"
#include "LRL_Cell_Degrees.h"
#include "LatticeConverter.h"
#include "MatS6.h"
#include "RemediatedFit.h"
#include "S6.h"
#include "Sella.h"

#include <cfloat>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <memory>
#include <set>
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

// DirectFitsForAllTypes -- the fit of the reduced cell itself for every
// type: SellaFitXXXXXX with the type's projector list (plus the face
// arrangements when FACECROSSED is on), no search. OriginalInput is set to
// the reduced cell so WinningCandidate means the same thing as after a
// search.
static std::vector<DeloneFitResults> DirectFitsForAllTypes(
   const S6& oneLattice,
   const std::vector<std::shared_ptr<GenerateDeloneBase>>& sptypes) {
   std::vector<DeloneFitResults> fits;
   fits.reserve(sptypes.size());
   for (const auto& sptype : sptypes) {
      DeloneFitResults fit = Sella::SellaFitXXXXXX(sptype, oneLattice);
      fit.SetOriginalInput(oneLattice);
      fits.emplace_back(fit);
   }
   return fits;
}

// ComputeFullSearchFitsForAllTypes -- runs SearchAllTypesWithCrossSeeding
// (or, with REMEDIATION off, DirectFitsForAllTypes),
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
   Sella& sella,
   const bool remediate) {

   static const std::vector<std::shared_ptr<GenerateDeloneBase>> sptypes =
      GenerateDeloneBase().Select("");

   const std::vector<DeloneFitResults> searched = remediate
      ? SearchAllTypesWithCrossSeeding(oneLattice, sptypes)
      : DirectFitsForAllTypes(oneLattice, sptypes);

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
      ComputeFullSearchFitsForAllTypes(oneLattice, oneErrors, oneReductionMatrix, sella,
         controls.shouldRemediate());

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
// ConeFit -- fit of s6 to one Delone type with the reduced-cone constraint:
// for each projector, project, then clamp every positive component to 0
// (all members of a letter group share one projected value, so this is
// min(0, group mean) per group -- the exact nearest point of the
// arrangement's subspace inside the cone). Same validity check and same
// projector list as SellaFitXXXXXX. For a reduced s6 no group mean is
// positive and this equals SellaFitXXXXXX's fit.
static double ConeFit(const std::shared_ptr<GenerateDeloneBase>& sptype, const S6& s6) {
   std::vector<MatS6> prjs = sptype->GetSellaProjectors();
   if (Sella::GetUseFaceCrossed()) {
      const FaceCrossedMatrices& fc = GetFaceCrossedMatrices(sptype->GetName());
      prjs.insert(prjs.end(), fc.prjs.begin(), fc.prjs.end());
   }
   double best = DBL_MAX;
   for (const auto& prj : prjs) {
      S6 projected = prj * s6;
      for (size_t i = 0; i < 6; ++i) {
         if (projected[i] > 0.0) projected[i] = 0.0;
      }
      if (!LRL_Cell_Degrees(projected).IsValid()) continue;
      const double fit = (s6 - projected).norm();
      if (fit < best) best = fit;
   }
   return best;
}

// TypeProjectorList -- the projectors SellaFitXXXXXX uses for this type:
// the library list, plus the face arrangements when FACECROSSED is on.
// libraryCount receives the number that come from the library.
static std::vector<MatS6> TypeProjectorList(const std::shared_ptr<GenerateDeloneBase>& sptype,
   size_t& libraryCount) {
   std::vector<MatS6> prjs = sptype->GetSellaProjectors();
   libraryCount = prjs.size();
   if (Sella::GetUseFaceCrossed()) {
      const FaceCrossedMatrices& fc = GetFaceCrossedMatrices(sptype->GetName());
      prjs.insert(prjs.end(), fc.prjs.begin(), fc.prjs.end());
   }
   return prjs;
}

// ArrangementOfProjector -- the 6-character arrangement of a projector
// ('0' for a zero slot, equal letters for slots in the same group), read
// from its columns: column j is P * e_j; a zero diagonal means a zero slot,
// equal columns mean the same group.
static std::string ArrangementOfProjector(const MatS6& prj) {
   std::vector<S6> columns;
   for (size_t j = 0; j < 6; ++j) {
      S6 e(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
      e[j] = 1.0;
      columns.push_back(prj * e);
   }
   std::string pattern(6, '0');
   std::vector<size_t> groupLeader;
   char nextLetter = 'a';
   std::vector<char> letterOf;
   for (size_t j = 0; j < 6; ++j) {
      if (std::fabs(columns[j][j]) < 1.0E-9) continue;
      char letter = 0;
      for (size_t g = 0; g < groupLeader.size(); ++g) {
         if ((columns[j] - columns[groupLeader[g]]).norm() < 1.0E-9) {
            letter = letterOf[g];
            break;
         }
      }
      if (letter == 0) {
         letter = nextLetter++;
         groupLeader.push_back(j);
         letterOf.push_back(letter);
      }
      pattern[j] = letter;
   }
   return pattern;
}

// BestConeProjector -- index of the projector giving the smallest cone fit
// of s6 (same clamp and validity check as ConeFit); prjs.size() if none.
static size_t BestConeProjector(const std::vector<MatS6>& prjs, const S6& s6) {
   size_t best = prjs.size();
   double bestFit = DBL_MAX;
   for (size_t i = 0; i < prjs.size(); ++i) {
      S6 projected = prjs[i] * s6;
      for (size_t k = 0; k < 6; ++k) {
         if (projected[k] > 0.0) projected[k] = 0.0;
      }
      if (!LRL_Cell_Degrees(projected).IsValid()) continue;
      const double fit = (s6 - projected).norm();
      if (fit < bestFit) {
         bestFit = fit;
         best = i;
      }
   }
   return best;
}

// PermutationMatrixTo -- finds a permutation of the six slots taking the
// reduced cell p to the candidate c ((Pi p)[i] = p[perm[i]]) and returns
// its matrix in pi. On a face (s_i = 0) the boundary transform R_i acts as a
// permutation, so a candidate inside the cone that is the same lattice as p
// is a permutation of p: one of the 24 reflections, or, when p has zeros, a
// composition of face permutations. Returns false if no permutation fits.
static bool PermutationMatrixTo(const S6& p, const S6& c, const double tolerance, MatS6& pi) {
   std::vector<size_t> perm = { 0, 1, 2, 3, 4, 5 };
   do {
      double d2 = 0.0;
      for (size_t i = 0; i < 6; ++i) d2 += (p[perm[i]] - c[i]) * (p[perm[i]] - c[i]);
      if (std::sqrt(d2) < tolerance) {
         double m[36] = { 0.0 };
         for (size_t i = 0; i < 6; ++i) m[6 * i + perm[i]] = 1.0;
         pi = MatS6(m[0], m[1], m[2], m[3], m[4], m[5],
            m[6], m[7], m[8], m[9], m[10], m[11],
            m[12], m[13], m[14], m[15], m[16], m[17],
            m[18], m[19], m[20], m[21], m[22], m[23],
            m[24], m[25], m[26], m[27], m[28], m[29],
            m[30], m[31], m[32], m[33], m[34], m[35]);
         return true;
      }
   } while (std::next_permutation(perm.begin(), perm.end()));
   return false;
}

// TypesStoringProjector -- names of the types whose library list
// (LabeledSellaMatrices::projectors) contains this projector.
static std::string TypesStoringProjector(const MatS6& prj) {
   std::string out;
   for (const auto& labeled : LabeledSellaMatrices::projectors) {
      for (const auto& stored : labeled.GetMatrices()) {
         if (SameMatS6(prj, stored)) {
            if (!out.empty()) out += ",";
            out += labeled.GetLabel();
            break;
         }
      }
   }
   return out.empty() ? std::string("none") : out;
}

// ReportInsideConeWin -- detail for a Remediation win whose candidate lies
// inside the reduced cone. If the candidate is a permutation Pi of the
// reduced cell p, then fit(Pi p, P) = fit(p, Pi^-1 P Pi), and the pull-back
// Pi^-1 P Pi is classified:
//   IN the list           -- the arrangement exists but lost on p (would
//                            need a closer look, e.g. the validity check).
//   MISSING (donor named) -- some type's library stores this projector, as
//                            for every Appendix B face arrangement: a real
//                            gap in this type's list.
//   cross-representation  -- no type stores it (2026-11-06: all 112 such
//                            wins in the first run). p lies exactly on
//                            several faces, so it has reduced forms that
//                            are not among the 24 reflections; the type
//                            lattice found is nearer to one of those. A face
//                            permutation is a lattice equivalence only where
//                            those slots are zero, so the pull-back is not
//                            an arrangement of any type. This is a distance
//                            through the faces -- Remediation's job -- not a
//                            gap in the list.
static void ReportInsideConeWin(const std::shared_ptr<GenerateDeloneBase>& sptype,
   const S6& reduced, const S6& candidate,
   std::map<std::string, std::map<std::string, std::string>>& missingByType,
   size_t& insideMissing, size_t& insidePresent, size_t& insideNotPermutation,
   size_t& insideCrossRepresentation) {

   size_t libraryCount = 0;
   const std::vector<MatS6> prjs = TypeProjectorList(sptype, libraryCount);
   const size_t winner = BestConeProjector(prjs, candidate);

   std::cout << ";     inside-cone candidate S " << candidate << std::endl;
   if (winner >= prjs.size()) {
      std::cout << ";     no valid projector for the candidate" << std::endl;
      return;
   }
   std::cout << ";     winning arrangement " << ArrangementOfProjector(prjs[winner])
      << ((winner < libraryCount) ? " (library)" : " (face)") << std::endl;

   MatS6 pi;
   if (!PermutationMatrixTo(reduced, candidate, 1.0E-6 * reduced.norm(), pi)) {
      ++insideNotPermutation;
      std::cout << ";     candidate is not a permutation of the reduced cell" << std::endl;
      return;
   }
   const MatS6 pulledBack = MatS6::Inverse(pi) * prjs[winner] * pi;
   bool inList = false;
   for (const auto& prj : prjs) {
      if (SameMatS6(prj, pulledBack)) {
         inList = true;
         break;
      }
   }
   const std::string pulledPattern = ArrangementOfProjector(pulledBack);
   const std::string donors = TypesStoringProjector(pulledBack);
   if (inList) {
      ++insidePresent;
      std::cout << ";     candidate = permutation of the reduced cell; pulled-back arrangement "
         << pulledPattern << " is IN the list" << std::endl;
   }
   else if (donors == "none") {
      ++insideCrossRepresentation;
      size_t zeros = 0;
      for (size_t i = 0; i < 6; ++i) {
         if (std::fabs(reduced[i]) < 1.0E-6 * reduced.norm()) ++zeros;
      }
      std::cout << ";     cross-representation win: cell on " << zeros
         << " face(s), candidate is another reduced form of it; pulled-back "
         << pulledPattern << " is not an arrangement of any type" << std::endl;
   }
   else {
      ++insideMissing;
      missingByType[sptype->GetName()][pulledPattern] = donors;
      std::cout << ";     candidate = permutation of the reduced cell; pulled-back arrangement "
         << pulledPattern << " MISSING from the list (stored for: " << donors << ")" << std::endl;
   }
}

// RunRemediationCheck (CHECKREMEDIATION, 2026-11-06) -- is Remediation still
// needed now that the face arrangements are in place? For every input cell
// and every Delone type, compares the direct fit (DirectFitsForAllTypes)
// with the remediated fit (SearchAllTypesWithCrossSeeding, the production
// search). Remediation starts from the direct fit and only accepts
// improvements, so remediated <= direct always. Each strict improvement (a
// "win", larger than 1.0E-6 * |reduced cell|) is printed with:
//   maxScalar -- largest scalar of the winning candidate; > 0 means the
//                candidate lies outside the reduced cone (Remediation admits
//                candidates up to 0.10 * |s6| outside).
//   cone      -- ConeFit of the winning candidate: its fit with the cone
//                constraint applied.
//   ARTIFACT  -- the cone fit is no better than the direct fit, so the win
//                came only from a projected point outside the cone.
//   SURVIVES  -- still lower than the direct fit with the cone constraint;
//                a real improvement the direct fit does not find.
// A surviving win whose candidate is INSIDE the cone (maxScalar <= 0) gets
// extra detail (ReportInsideConeWin): either a cross-representation win on a
// cell lying exactly on several faces (expected), or a missing arrangement
// that some other type stores (a real gap, listed by type in the summary).
// If no win survives on a large enough set of cells, Remediation can go.
void RunRemediationCheck(const std::vector<LatticeCell>& inputList, const CmdSellaControls& controls) {
   static const std::vector<std::shared_ptr<GenerateDeloneBase>> sptypes =
      GenerateDeloneBase().Select("");

   std::cout << "; CHECKREMEDIATION: " << inputList.size() << " cell(s), "
      << sptypes.size() << " Delone types, face arrangements "
      << (Sella::GetUseFaceCrossed() ? "on" : "off") << ", cone fit "
      << (Sella::GetUseConeFit() ? "on" : "off") << std::endl;

   size_t typeFits = 0;
   size_t wins = 0;
   size_t winsOutsideCone = 0;
   size_t artifacts = 0;
   size_t survivors = 0;
   size_t cellsWithSurvivor = 0;
   double largestSurvivingGap = 0.0;
   double directSeconds = 0.0;
   double remediatedSeconds = 0.0;
   std::map<std::string, size_t> survivorsByType;
   size_t insideWins = 0;
   size_t insideMissing = 0;
   size_t insidePresent = 0;
   size_t insideNotPermutation = 0;
   size_t insideCrossRepresentation = 0;
   std::map<std::string, std::map<std::string, std::string>> missingByType;

   for (size_t c = 0; c < inputList.size(); ++c) {
      MatS6 reductionMatrix;
      const S6 oneLattice = GetInputSellingReducedVectors(inputList[c], reductionMatrix);
      const double tolerance = 1.0E-6 * oneLattice.norm();

      const auto t0 = std::chrono::steady_clock::now();
      const std::vector<DeloneFitResults> direct = DirectFitsForAllTypes(oneLattice, sptypes);
      const auto t1 = std::chrono::steady_clock::now();
      const std::vector<DeloneFitResults> remediated = SearchAllTypesWithCrossSeeding(oneLattice, sptypes);
      const auto t2 = std::chrono::steady_clock::now();
      directSeconds += std::chrono::duration<double>(t1 - t0).count();
      remediatedSeconds += std::chrono::duration<double>(t2 - t1).count();

      bool headerPrinted = false;
      bool cellHasSurvivor = false;
      for (size_t i = 0; i < sptypes.size(); ++i) {
         ++typeFits;
         const double directFit = direct[i].GetRawFit();
         const double remediatedFit = remediated[i].GetRawFit();
         if (remediatedFit >= directFit - tolerance) continue;

         ++wins;
         const S6 candidate = remediated[i].GetOriginalInput();
         const double maxScalar = MaxS6Scalar(candidate);
         if (maxScalar > 0.0) ++winsOutsideCone;
         const double cone = ConeFit(sptypes[i], candidate);
         const bool survives = (cone < directFit - tolerance);
         if (survives) {
            ++survivors;
            ++survivorsByType[sptypes[i]->GetName()];
            cellHasSurvivor = true;
            if (directFit - cone > largestSurvivingGap) largestSurvivingGap = directFit - cone;
         }
         else {
            ++artifacts;
         }

         if (!headerPrinted) {
            std::cout << "; cell " << (c + 1) << ": " << inputList[c].GetInput()
               << "   reduced S " << oneLattice << std::endl;
            headerPrinted = true;
         }
         std::cout << ";   REMEDIATION WIN type=" << sptypes[i]->GetName()
            << " direct=" << directFit
            << " remediated=" << remediatedFit
            << " cone=" << cone
            << " maxScalar=" << maxScalar
            << (survives ? " SURVIVES" : " ARTIFACT") << std::endl;
         if (survives && maxScalar <= 0.0) {
            ++insideWins;
            ReportInsideConeWin(sptypes[i], oneLattice, candidate, missingByType,
               insideMissing, insidePresent, insideNotPermutation, insideCrossRepresentation);
         }
         else if (survives && controls.shouldDebug()) {
            std::cout << ";     candidate S " << candidate << std::endl;
         }
      }
      if (cellHasSurvivor) ++cellsWithSurvivor;
   }

   std::cout << "; CHECKREMEDIATION summary: " << inputList.size() << " cell(s), "
      << typeFits << " type fit(s), " << wins << " remediation win(s) ("
      << winsOutsideCone << " with candidate outside the cone), "
      << artifacts << " artifact(s), " << survivors << " surviving win(s) in "
      << cellsWithSurvivor << " cell(s), largest surviving gap " << largestSurvivingGap << std::endl;
   if (!survivorsByType.empty()) {
      std::cout << "; surviving wins by type:";
      for (const auto& entry : survivorsByType) {
         std::cout << " " << entry.first << "=" << entry.second;
      }
      std::cout << std::endl;
   }
   std::cout << "; inside-cone surviving wins: " << insideWins
      << " (" << insideCrossRepresentation << " cross-representation, "
      << insideMissing << " missing arrangement, "
      << insidePresent << " arrangement present, "
      << insideNotPermutation << " not a permutation)" << std::endl;
   for (const auto& entry : missingByType) {
      std::cout << "; missing arrangements for " << entry.first << ":";
      for (const auto& pattern : entry.second) {
         std::cout << " " << pattern.first << "(" << pattern.second << ")";
      }
      std::cout << std::endl;
   }
   std::cout << "; time: direct " << directSeconds << " s, remediated "
      << remediatedSeconds << " s" << std::endl;
}
