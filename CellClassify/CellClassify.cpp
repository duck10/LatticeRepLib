// CellClassify.cpp
//
// Batch classification/census tool for SELLA, built to answer a different
// question than CmdSella asks. CmdSella is interactive: one cell in, full
// per-type table and SVGs out. CellClassify is for volume -- from a few
// thousand cells up to the >1M unit cells a colleague plans to run from
// the PDB -- where per-cell output of any kind is unusable regardless of
// how it's formatted (see this session's chat: "even 1000 is a bit much
// for this human").
//
// Design, per this session's discussion:
//   - Calls ComputeDeloneFits directly, the exact function CmdSella.cpp
//     itself calls per cell, so results here are guaranteed to match
//     what CmdSella/production classification would have done -- no
//     second, parallel implementation to drift out of sync.
//   - No per-cell printing at all. Two StoreResults<std::string, std::string>
//     tallies instead, each keyed by a short signature, each holding only
//     NMAX example cells per key (default 3) -- output size is bounded by
//     the number of DISTINCT keys that occur (realistically a handful to
//     a few dozen), not by how many cells were run.
//   - Failure-signature store: only cells where HasFailure()==true after
//     remediation, keyed by the sorted set of type names in the chain
//     violation. Tells you which KINDS of failures actually occur in a
//     realistic population.
//   - Near-tie store: every cell, keyed by the unordered pair of general
//     types with the smallest and second-smallest raw fit (excluding aP,
//     which structurally fits everything near-zero and would otherwise
//     spam every single cell). This is the empirical replacement for the
//     boundary-enumeration approach: instead of proving analytically
//     where a tie sits, observe which type pairs actually come close
//     together across a realistic sample. M1A/M2A -- the one swap pair
//     this session left untested by the targeted-search method -- either
//     shows up here with real frequency, or it doesn't; either answer is
//     usable without having had to locate it by hand first.
//
// TIETOL (near-tie threshold) and NMAX (samples per key) are both
// CellClassifyControls settings, not hardcoded -- see that header for why.

#include "CellClassifyControls.h"
#include "CellClassifyTestMode.h"
#include "DeloneFitComputation.h"
#include "DeloneFitFiltering.h"
#include "GrimmerTree.h"
#include "ProgramSetup.h"
#include "Sella.h"
#include "StoreResults.h"
#include "WebIO.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <set>
#include <string>
#include <vector>

// MakeTiePairKey -- unordered pair key, e.g. "M1A/M2A" and "M2A/M1A" both
// produce "M1A/M2A". Deliberately local/self-contained rather than reusing
// TestHarness.h's MakeUnorderedPairKey: that header (and the boundary-
// search machinery it's paired with) belongs to the targeted-enumeration
// approach this tool exists to complement rather than depend on -- keeping
// CellClassify's own dependency list lean and not pulling diagnostic-only
// headers into a tool meant to run unattended over a million cells.
static std::string MakeTiePairKey(const std::string& a, const std::string& b) {
   return (a < b) ? (a + "/" + b) : (b + "/" + a);
}

// MakeFailureSignatureKey -- sorted, "/"-joined set of type names involved
// in a Grimmer chain violation, e.g. "mC/oC/tP". Sorting makes the key
// independent of the order GetFailures() happens to report them in.
static std::string MakeFailureSignatureKey(const GrimmerChainFailure& failure) {
   std::set<std::string> names;
   for (const auto& p : failure.GetFailures()) {
      names.insert(p.first);
   }
   std::string key;
   for (const auto& n : names) {
      if (!key.empty()) key += "/";
      key += n;
   }
   return key;
}

void HandleOneInputCell(
   const LatticeCell& inputCell,
   const CellClassifyControls& controls,
   const CmdSellaControls& cmdSellaControls,
   Sella& sella,
   StoreResults<std::string, std::string>& failureStore,
   StoreResults<std::string, std::string>& tieStore,
   size_t& totalProcessed,
   size_t& totalFailed) {

   ++totalProcessed;

   const DeloneFitComputation result = ComputeDeloneFits(inputCell, cmdSellaControls, sella);
   const std::string cellStr = inputCell.GetInput();

   if (controls.shouldShowDetails()) {
      std::cout << "; DETAILS " << cellStr << std::endl;
   }

   if (result.chains.HasFailure()) {
      ++totalFailed;
      const GrimmerChainFailure failure = result.chains.GetFirstFailure();
      const std::string key = MakeFailureSignatureKey(failure);
      failureStore.Store(key, cellStr);
      if (controls.shouldShowDetails()) {
         std::cout << ";   chain failure: " << key << std::endl;
      }
   }
   else if (controls.shouldShowDetails()) {
      std::cout << ";   no chain failure" << std::endl;
   }

   // Near-tie detection: one entry per general type (raw vs. remediated
   // duplicates collapsed -- see DeloneFitFiltering.h's own comment for
   // why this step isn't optional here), normalized by lattice norm so
   // the threshold means the same thing across cells of very different
   // scale. aP is excluded from the TIE decision -- it structurally fits
   // every cell near-zero (see e.g. any printed fit table: "aP A1 0"),
   // which would otherwise make aP tie with everything on every single
   // cell and drown out any real signal. DETAILS mode below still prints
   // aP's own fit (unfiltered), since seeing A1's actual value directly
   // is the point when investigating whether it's coming back as the
   // expected ~0 -- see this session's chat on the aP/mP/oC,aP/mC/oC
   // failure-signature finding.
   const std::vector<DeloneFitResults> filtered = FilterForBestExample(result.fits);
   if (!filtered.empty()) {
      // filtered[0] is one of the original raw SellaFit() entries (index
      // 0 always is -- appended remediated entries only ever come after),
      // so GetOriginalInput() reliably gives the real input S6 here even
      // though remediated entries themselves don't carry it.
      const double latticeNorm = filtered[0].GetOriginalInput().norm();

      if (controls.shouldShowDetails()) {
         std::cout << ";   fits (general type, raw fit, normalized fit):" << std::endl;
         for (const auto& type : filtered) {
            const double normalizedFit = (latticeNorm > 0.0)
               ? type.GetRawFit() / latticeNorm : type.GetRawFit();
            std::cout << ";     " << type.GetGeneralType() << "  "
               << type.GetRawFit() << "  " << normalizedFit << std::endl;
         }
      }

      std::string bestName, secondName;
      double bestFit = -1.0, secondFit = -1.0;
      for (const auto& type : filtered) {
         if (!type.GetBravaisType().empty() && type.GetBravaisType()[0] == 'a') continue;
         const double normalizedFit = (latticeNorm > 0.0)
            ? type.GetRawFit() / latticeNorm
            : type.GetRawFit();
         if (bestFit < 0.0 || normalizedFit < bestFit) {
            secondFit = bestFit;
            secondName = bestName;
            bestFit = normalizedFit;
            bestName = type.GetGeneralType();
         }
         else if (secondFit < 0.0 || normalizedFit < secondFit) {
            secondFit = normalizedFit;
            secondName = type.GetGeneralType();
         }
      }

      const bool isTie = !bestName.empty() && !secondName.empty() &&
         (secondFit - bestFit) < controls.getTieTol();

      if (controls.shouldShowDetails()) {
         std::cout << ";   best=" << bestName << " " << bestFit
            << "  second=" << secondName << " " << secondFit
            << "  " << (isTie ? "TIE" : "no tie") << std::endl;
      }

      if (isTie) {
         const std::string key = MakeTiePairKey(bestName, secondName);
         tieStore.Store(key, cellStr);
      }
   }
}

// ShowKeysCountsAndSamples -- StoreResults::ShowTableOfKeysVersusCount()
// prints only key/count, never the actual captured sample cells -- those
// were sitting in the store the whole time (up to NMAX per key) and
// simply never got surfaced. This prints both: the count table for an
// at-a-glance view, then the real example cells for each key, since
// investigating WHY a signature or pair occurred requires the actual
// cells, not just a tally.
static void ShowKeysCountsAndSamples(StoreResults<std::string, std::string>& store) {
   store.ShowTableOfKeysVersusCount();
   const std::vector<std::string> keys = store.GetKeys();
   for (const auto& key : keys) {
      std::cout << "\n; " << key << " (total seen: " << store.GetItemCount(key) << ")" << std::endl;
      const auto samples = store.GetResult(key);
      for (const auto& sample : samples) {
         std::cout << ";   " << sample.second << std::endl;
      }
   }
}

int main(int argc, char* argv[]) {
   std::cout << "; CellClassify -- batch Delone/Bravais classification census" << std::endl;

   try {
      // CmdGen.cpp calls WebIO explicitly before constructing its
      // BasicProgramInput; CmdPerturb.cpp doesn't. BasicProgramInput
      // itself takes no argc/argv, so it's not obvious what (if
      // anything) depends on WebIO having run first -- included here
      // to match the more defensive of the two precedents rather than
      // assume CmdPerturb.cpp's omission was deliberate.
      WebIO webio(argc, argv, "CellClassify", 0);

      CellClassifyControls controls;
      const BasicProgramInput<CellClassifyControls> dc_setup("CellClassify", controls);

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      // TEST -- same convention as CmdSella (see CellClassifyControls.h's
      // TEST handler): dispatches to the regression suite instead of
      // normal batch processing, then returns without touching
      // dc_setup.getInputList() at all.
      if (controls.shouldRunTests()) {
         CellClassifyTestRunner runner;
         const std::vector<int> testNumbers = controls.getTestNumbers();
         if (testNumbers.empty()) {
            runner.RunAll();
         }
         else {
            runner.RunSelected(testNumbers);
         }
         return 0;
      }

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      CmdSellaControls cmdSellaControls;
      Sella sella;

      StoreResults<std::string, std::string> failureStore(controls.getNmax());
      failureStore.SetTitle("Grimmer chain-failure signatures (post-remediation)");
      failureStore.SetKeyLabel("failing types");

      StoreResults<std::string, std::string> tieStore(controls.getNmax());
      tieStore.SetTitle("Near-tie type pairs (empirical, TIETOL=" +
         std::to_string(controls.getTieTol()) + ")");
      tieStore.SetKeyLabel("type pair");

      size_t totalProcessed = 0;
      size_t totalFailed = 0;

      // Timing: CellClassify calls ComputeDeloneFits directly -- the exact
      // same function CmdSella itself calls -- so this measures that
      // function's real per-cell cost at volume, not anything specific to
      // CellClassify's own bookkeeping. Reported because that number is
      // exactly what's needed before committing to a colleague's planned
      // 1M-cell PDB run: see this session's chat for why a real measured
      // figure matters more here than an assumption either way.
      const auto startTime = std::chrono::steady_clock::now();

      for (const auto& inputCell : dc_setup.getInputList()) {
         HandleOneInputCell(inputCell, controls, cmdSellaControls, sella,
            failureStore, tieStore, totalProcessed, totalFailed);
         if (totalProcessed % 10000 == 0) {
            std::cerr << ";  processed " << totalProcessed << " cells..." << std::endl;
         }
      }

      const auto endTime = std::chrono::steady_clock::now();
      const double elapsedSeconds = std::chrono::duration<double>(endTime - startTime).count();
      const double msPerCell = (totalProcessed > 0)
         ? (elapsedSeconds * 1000.0 / static_cast<double>(totalProcessed))
         : 0.0;

      std::cout << "\n; CellClassify: " << totalProcessed << " cells processed, "
         << totalFailed << " unresolved chain failures" << std::endl;
      std::cout << "; elapsed: " << elapsedSeconds << " s ("
         << msPerCell << " ms/cell average)" << std::endl;
      if (totalProcessed > 0) {
         const double estimatedHoursFor1M = (msPerCell * 1.0e6) / 1000.0 / 3600.0;
         std::cout << "; at this rate, 1,000,000 cells would take roughly "
            << estimatedHoursFor1M << " hours -- measured on THIS run's mix "
            << "of cells, which may not represent real PDB cells' remediation "
            << "load one way or the other" << std::endl;
      }
      std::cout << std::endl;

      ShowKeysCountsAndSamples(failureStore);
      std::cout << std::endl;
      ShowKeysCountsAndSamples(tieStore);

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}
