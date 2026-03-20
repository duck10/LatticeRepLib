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
#include "MultiTransformFinderControls.h"
#include "MatchPair.h"
#include "ProductionLatticeMatcherSystem.h"
#include "ProgramSetup.h"


// ---------------------------------------------------------------------------
// Input-list processing
// ---------------------------------------------------------------------------

void runInputListMode(const std::vector<LatticeCell>& inputList,
   const MultiTransformFinderControls& controls) {
   std::cout << "; === PROCESSING INPUT LIST ===" << std::endl;

   if (inputList.size() == 2) {
      const auto mr = matchPair(inputList[0], inputList[1], controls);
      if (mr.swapped)
         std::cout << "; Note: cells swapped -- larger volume cell is reference" << std::endl;
      if (controls.shouldShowDetails()) {
         std::cout << "; Total results from matcher: " << mr.results.size() << std::endl;
      }
      displayResults(mr.results, controls, mr.reference, mr.mobile);
   } else {
      // Multi-cell: first cell is user's reference, remaining are mobiles.
      // Each pair is routed through matchPair() so the volume-based swap
      // is applied consistently with the 2-cell path.
      if (controls.shouldShowDetails()) {
         std::cout << "; === INDIVIDUAL MOBILE MATCHING ===" << std::endl;
         std::cout << "; Reference: " << LRL_Cell_Degrees(inputList[0].getCell())
            << " (" << inputList[0].getLatticeType() << ")" << std::endl;
         std::cout << "; Processing " << (inputList.size() - 1)
            << " mobile lattices individually" << std::endl;
      }

      for (size_t i = 1; i < inputList.size(); ++i) {
         std::cout << "\n; === MOBILE " << i << " ===" << std::endl;

         const auto mr = matchPair(inputList[0], inputList[i], controls);
         if (mr.swapped && controls.shouldShowDetails()) {
            std::cout << "; Note: cells swapped for mobile " << i
               << " -- larger volume cell is reference" << std::endl;
         }

         if (mr.results.empty()) {
            std::cout << "; No matching transformations found for this mobile." << std::endl;
         } else {
            displayResults(mr.results, controls, mr.reference, mr.mobile);
         }
      }

      std::cout << "\n; === OVERALL SUMMARY ===" << std::endl;
      std::cout << "; Processed " << (inputList.size() - 1)
         << " mobile lattices individually." << std::endl;
   }
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------

int main() {
   try {
      std::cout << "; === P3 LATTICE MATCHING ===" << std::endl;

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


      Matrix_3x3 mmmmmm; mmmmmm.norm();
      const std::vector<LatticeCell>& inputList = program_setup.getInputList();

      if (inputList.size() < 2) {
         std::cout << "; CmdLMP3 requires at least 2 input cells" << std::endl;
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