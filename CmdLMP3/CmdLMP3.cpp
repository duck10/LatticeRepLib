#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "DisplayLMP3.h"
#include "FlexibleTestMode.h"
#include "LatticeCell.h"
#include "LatticeConverter.h"
#include "LatticeMatchResult.h"
#include "LRL_Cell_Degrees.h"
#include "MobileComparisonResult.h"
#include "MultiTransformFinderControls.h"
#include "MatchPair.h"
#include "ProductionLatticeMatcherSystem.h"
#include "ProgramSetup.h"


// ---------------------------------------------------------------------------
// Input-list processing  (unchanged)
// ---------------------------------------------------------------------------

void runInputListMode(const std::vector<LatticeCell>& inputListIn,
   const MultiTransformFinderControls& controls) {
   std::cout << "\n=== PROCESSING INPUT LIST ===" << std::endl;
   ProductionLatticeMatcherSystem matcher(controls);

   // In ALL or SUPER mode, ensure the larger-volume cell is the reference
   // so that supercell matrices (det>1) can expand it to match the mobile.
   // In EQUIVALENT mode the user's reference choice is respected -- swapping
   // would break multi-input comparison mode.
   std::vector<LatticeCell> inputList = inputListIn;

   if (inputList.size() == 2) {
      const auto mr = matchPair(inputList[0], inputList[1], controls);
      if (mr.swapped)
         std::cout << "; Note: cells swapped -- larger volume cell is reference\n";
      std::cout << mr.reference.GetInputLine() << " REFERENCE " << std::endl;
      if (controls.shouldShowDetails()) {
         std::cout << "; Total results from matcher: " << mr.results.size() << "\n";
      }
      displayResults(mr.results, controls, mr.reference);
   } else {
      const LatticeCell& reference = inputList[0];

      if (controls.shouldRunComparisonMode()) {
         std::vector<MobileComparisonResult> comparisonResults;

         if (controls.shouldShowDetails()) {
            std::cout << "; === COMPARISON MODE ===" << std::endl;
            std::cout << "; Reference: " << LRL_Cell_Degrees(reference.getCell())
               << " (" << reference.getLatticeType() << ")" << std::endl;
            std::cout << "; Processing " << (inputList.size() - 1)
               << " mobile lattices in comparison mode" << std::endl;
         }

         for (size_t i = 1; i < inputList.size(); ++i) {
            const LatticeCell& mobile = inputList[i];
            if (controls.shouldShowDetails()) {
               std::cout << "\n; --- Processing Mobile " << i << " ---" << std::endl;
               std::cout << "; Mobile: " << LRL_Cell_Degrees(mobile.getCell())
                  << " (" << mobile.getLatticeType() << ")" << std::endl;
            }

            std::vector<LatticeCell>        singlePair = { reference, mobile };
            std::vector<LatticeMatchResult> results = matcher.processInputList(singlePair);

            if (!results.empty()) {
               LatticeMatchResult bestResult = results[0];
               double             bestDistance = results[0].getP3Distance();
               for (const auto& result : results) {
                  if (result.getP3Distance() < bestDistance) {
                     bestDistance = result.getP3Distance();
                     bestResult = result;
                  }
               }
               comparisonResults.push_back({
                  static_cast<int>(i), mobile, bestResult, bestDistance });

               if (controls.shouldShowDetails()) {
                  std::cout << "; Found " << results.size()
                     << " transformations, best distance: "
                     << std::scientific << std::setprecision(3) << bestDistance << std::endl;
               }
            }
         }

         displayComparisonResults(reference, comparisonResults, controls);
      } else {
         if (controls.shouldShowDetails()) {
            std::cout << "=== INDIVIDUAL MOBILE MATCHING ===" << std::endl;
            std::cout << "Reference: " << LRL_Cell_Degrees(reference.getCell())
               << " (" << reference.getLatticeType() << ")" << std::endl;
            std::cout << "Processing " << (inputList.size() - 1)
               << " mobile lattices individually" << std::endl;
         }

         for (size_t i = 1; i < inputList.size(); ++i) {
            const LatticeCell& mobile = inputList[i];

            std::cout << "\n=== MOBILE " << i << " RESULTS ===" << std::endl;
            std::cout << "Mobile: " << LRL_Cell_Degrees(mobile.getCell())
               << " (" << mobile.getLatticeType() << ")" << std::endl;

            std::vector<LatticeCell>        singlePair = { reference, mobile };
            std::vector<LatticeMatchResult> results = matcher.processInputList(singlePair);

            if (results.empty()) {
               std::cout << "No matching transformations found for this mobile." << std::endl;
            } else {
               displayResults(results, controls, reference);
            }
         }

         std::cout << "\n=== OVERALL SUMMARY ===" << std::endl;
         std::cout << "Processed " << (inputList.size() - 1)
            << " mobile lattices individually." << std::endl;
      }
   }
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------

int main() {
   try {
      std::cout << "=== P3 LATTICE MATCHING ===" << std::endl;

      MultiTransformFinderControls controls;
      const BasicProgramInput<MultiTransformFinderControls> program_setup(
         "ProductionLatticeMatcher", controls);

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      if (controls.shouldRunTests()) {
         runFlexibleTestMode(controls);
         return 0;
      }

      const std::vector<LatticeCell>& inputList = program_setup.getInputList();

      if (inputList.size() < 2) {
         std::cout << "; CmdLMP3 requires at least 2 input cells\n";
         return 0;
      }

      runInputListMode(inputList, controls);

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; Error: " << e.what() << std::endl;
      return -1;
   }
}