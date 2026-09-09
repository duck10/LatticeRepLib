#ifndef DIRECT_CLASSIFY_H
#define DIRECT_CLASSIFY_H

// DirectClassify.h
//
// From-scratch rewrite of CmdSella's classification pipeline (see chat,
// Sept 2026). Reachable via CmdSellaControls' CLASSIFYDIRECT keyword,
// which diverges CmdSella.cpp's main() right after input/controls setup
// and exits before the existing pipeline runs -- one-way, for
// side-by-side comparison against the existing pipeline during the
// rebuild.
//
// PopulateDirectFitsForAllTypes -- per type, calls Remediation168ForOneSubtype
// (VCP candidates + boundary transforms, from RemediatedFit.h) directly,
// unconditionally, independently -- no raw SellaFitXXXXXX pass first, no
// shortlist-comparison gate, no reactive GrimmerChains-triggered repair
// loop. Confirmed (chat, Sept 2026) to exactly reproduce Test 22's M4
// true-minimum values (9.23988, 34.926) and to close the 4 historical
// cases that plain Remediation()/RemediateToStability() left unresolved
// -- VCP candidates specifically, not just "more iteration," were the
// missing ingredient.
//
// Fields Sella::SellaFit used to set beyond what SellaFitXXXXXX/
// Remediation168ForOneSubtype set on their own (Zscore, ReductionMatrix,
// Difference, LatticeType via GetBravaisType, OriginalInput as the real
// input cell, never a winning candidate) are reproduced explicitly here.
// WinningCandidate is new: Remediation168ForOneSubtype's own winning
// transformed cell, captured before OriginalInput is repurposed to mean
// "the shared input cell" (see chat, Sept 2026, on SearchForToCanon /
// PrintCompareSummary's existing assumption that OriginalInput is
// identical across every type for one input cell).
//
// Chain-consistency checking (GrimmerChains) deliberately NOT included --
// decided against, chat Sept 2026: its only real job in the old pipeline
// was triggering the reactive repair loop, which this design no longer
// has (every type already gets the full search unconditionally); as a
// pure diagnostic it can't distinguish genuine geometric edge cases from
// search bugs, and even after correcting it to compare only
// NestingCheckResults.h's confirmed-nesting siblings, it remains a weak
// tool for catching algorithm errors specifically. Revisit only if
// CellClassify-style population-level failure census becomes relevant
// again -- a distinct question from per-cell diagnosis.
//
// SearchForToCanon also NOT reproduced -- validation-only (throws on
// name mismatch, doesn't affect the classification decision) and
// depends on LabeledSellaMatrices's static aggregate library, already
// confirmed dead everywhere else in this rewrite.

#include "DeloneFitResults.h"
#include "GenerateLatticeTypeExamples.h"
#include "RemediatedFit.h"
#include "Sella.h"

#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "DeloneFitFiltering.h"
#include "GrimmerDeloneTree.h"
#include "LRL_Cell_Degrees.h"

#include <set>
#include <sstream>

#include "GrimmerTree.h"
#include "FileOperations.h"
#include "LRL_CreateFileName.h"
#include "LRL_DataToSVG.h"

inline std::string GenerateDirectFitPlotsSvg(
   const std::vector<DeloneFitResults>& fits,
   const S6& oneLattice,
   const std::string& inputEcho) {

   GrimmerChains gcs(oneLattice);
   gcs.UpdateFits(fits);
   return gcs.GenerateSortedFitPlots(900, 700, inputEcho);
}

inline std::string WriteDirectSvg(const std::string& svg, const std::string& filename) {
   std::ofstream fileout;
   FileOperations::OpenOutputFile(fileout, filename);
   if (fileout.is_open()) {
      fileout.seekp(0);
      fileout << svg << std::endl;
   }
   else {
      std::cout << "Could not open file " << filename << " for write in WriteDirectSvg" << std::endl;
   }
   fileout.close();
   return filename;
}


// Reproduced from CmdSella.cpp -- pure function over a fits vector, no
// chain-check dependency. Returns one formatted "P <cell> <type> <fit>"
// line per matched type, for GrimmerDeloneTree::ProduceSVG's matches
// parameter.
inline std::vector<std::string> ListDirectMatchingTypes(
   const std::vector<DeloneFitResults>& vFilteredDeloneFitResults,
   const S6& LatI,
   const double maxDeltaForMatch) {

   const std::set<std::string> matchedNames =
      GetMatchedTypeNames(vFilteredDeloneFitResults, LatI.norm(), maxDeltaForMatch);
   std::vector<std::string> out;
   for (const auto& type : vFilteredDeloneFitResults) {
      if (matchedNames.find(type.GetGeneralType()) == matchedNames.end()) continue;
      std::ostringstream os;
      os << "P "
         << LRL_Cell_Degrees(type.GetBestFit()) << " "
         << type.GetDeloneType() << " "
         << type.GetRawFit();
      out.emplace_back(os.str());
   }
   return out;
}

inline std::string GenerateDirectDeloneTreeSvg(
   const std::vector<DeloneFitResults>& fits,
   const S6& oneLattice,
   const LatticeCell& input,
   const double maxDeltaForMatch) {

   GrimmerChains gcs(oneLattice);
   gcs.UpdateFits(fits);

   const std::vector<DeloneFitResults> filtered = FilterForBestExample(fits);
   const std::vector<std::string> matches = ListDirectMatchingTypes(filtered, oneLattice, maxDeltaForMatch);

   return GrimmerDeloneTree::ProduceSVG(gcs, input, matches, oneLattice);
}

inline std::vector<DeloneFitResults> PopulateDirectFitsForAllTypes(
   const S6& oneLattice,
   const S6& oneErrors,
   const MatS6& oneReductionMatrix,
   Sella& sella) {

   static const std::vector<std::shared_ptr<GenerateDeloneBase>> sptypes =
      GenerateDeloneBase().Select("");

   std::vector<DeloneFitResults> results;
   results.reserve(sptypes.size());

   for (const auto& sptype : sptypes) {
      DeloneFitResults fit = Remediation168ForOneSubtype(oneLattice, sptype, 10, {});
      fit.SetWinningCandidate(fit.GetOriginalInput());  // capture before
      // OriginalInput below is repurposed to mean "the shared input cell"

      const double zscore = sella.Zscore(oneLattice - fit.GetBestFit(), oneErrors, oneReductionMatrix)
         * sqrt(sptype->GetFreeParams());
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

// PrintCompact -- one line per type: name and raw fit. For scanning many
// types/cells at once.
inline void PrintCompact(const DeloneFitResults& fit) {
   std::cout << ";     " << fit.GetGeneralType() << "  " << fit.GetRawFit() << std::endl;
}

// PrintFull -- every field, for checking that a single type's result
// makes sense (RawFit matches GetBestFit().norm(), OriginalInput is
// constant across a cell's 24 types while WinningCandidate varies, etc.)
// Named function, not operator<<: DeloneType.cpp already defines
// operator<<(std::ostream&, const DeloneFitResults&) for its own,
// partially-stale format (its "Best Cell in original centering" line
// depends on m_toCanonicalDeloneType, which SellaFitXXXXXX never
// actually sets to anything but MatS6().unit() -- see chat, Sept 2026).
// A second operator<< with the same signature is an ODR violation;
// PrintFull sidesteps that without touching or trusting the existing one.
inline void PrintFull(const DeloneFitResults& fit) {
   std::cout << "; DeloneFitResults" << std::endl;
   std::cout << ";   general type    " << fit.GetGeneralType() << std::endl;
   std::cout << ";   Bravais type    " << fit.GetBravaisType() << std::endl;
   std::cout << ";   Delone type     " << fit.GetDeloneType() << std::endl;
   std::cout << ";   lattice type    " << fit.GetLatticeType() << std::endl;
   std::cout << ";   raw fit         " << fit.GetRawFit() << std::endl;
   std::cout << ";   Zscore          " << fit.GetZscore() << std::endl;
   std::cout << ";   best fit S6     " << fit.GetBestFit() << std::endl;
   std::cout << ";   difference      " << fit.GetDifference() << std::endl;
   std::cout << ";   original input  " << fit.GetOriginalInput() << std::endl;
   std::cout << ";   winning cand.   " << fit.GetWinningCandidate() << std::endl;
   std::cout << ";   reduction mat" << std::endl << fit.GetReductionMatrix() << std::endl;
   std::cout << ";   toCanon mat" << std::endl << fit.GetToCanon() << std::endl;
}

#endif // DIRECT_CLASSIFY_H