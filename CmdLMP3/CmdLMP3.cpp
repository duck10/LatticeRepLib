#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "DisplayLMP3.h"
#include "FlexibleTestMode.h"
#include "LatticeCell.h"
#include "LatticeMatchResult.h"
#include "MobileComparisonResult.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "MultiTransformFinderControls.h"
#include "ProductionLatticeMatcherSystem.h"
#include "ProgramSetup.h"


// ---------------------------------------------------------------------------
// Input-list processing
// ---------------------------------------------------------------------------

void runInputListMode(const std::vector<LatticeCell>& inputList,
   const MultiTransformFinderControls& controls) {
   std::cout << "\n=== PROCESSING INPUT LIST ===" << std::endl;
   ProductionLatticeMatcherSystem matcher(controls);

   if (inputList.size() == 2) {
      // Single mobile – use detailed output
      std::cout << inputList[0].GetInputLine() << " REFERENCE " << std::endl;
      std::vector<LatticeMatchResult> results = matcher.processInputList(inputList);
      if (controls.shouldShowDetails()) {
         std::cout << "DEBUG: Total results returned from matcher: "
            << results.size() << std::endl;
      }
      displayResults(results, controls, inputList[0]);
   } else {
      const LatticeCell& reference = inputList[0];

      if (controls.shouldRunComparisonMode()) {
         // Comparison mode – collect best result per mobile, then summarise
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

            std::vector<LatticeCell>       singlePair = { reference, mobile };
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
         // Individual mode – full results for each mobile
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

            std::vector<LatticeCell>       singlePair = { reference, mobile };
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
// Self-contained reservoir test classes (kept here for unit-test purposes)
// ---------------------------------------------------------------------------

class TestResult {
public:
   explicit TestResult(double d) : distance(d) {}
   double getP3Distance() const { return distance; }
   bool operator<(const TestResult& other) const { return distance < other.distance; }
   bool operator==(const TestResult& other) const { return distance == other.distance; }
private:
   double distance;
};

class TestReservoir {
public:
   explicit TestReservoir(size_t maxSize) : m_maxReservoirSize(maxSize) {}

   void add(const TestResult& result) {
      std::cout << "Adding: " << result.getP3Distance() << std::endl;

      if (m_reservoir.size() < m_maxReservoirSize) {
         auto pos = std::lower_bound(m_reservoir.begin(), m_reservoir.end(), result);
         m_reservoir.insert(pos, result);
         std::cout << "  Inserted (reservoir not full)" << std::endl;
      } else {
         std::cout << "  Reservoir full. Worst distance: "
            << m_reservoir.back().getP3Distance() << std::endl;
         if (result < m_reservoir.back()) {
            auto pos = std::lower_bound(m_reservoir.begin(), m_reservoir.end(), result);
            m_reservoir.insert(pos, result);
            m_reservoir.pop_back();
            std::cout << "  Removed worst element" << std::endl;
         } else {
            std::cout << "  New result is worse, rejecting" << std::endl;
         }
      }

      std::cout << "  Current reservoir: ";
      for (const auto& r : m_reservoir) std::cout << r.getP3Distance() << " ";
      std::cout << std::endl << std::endl;
   }

   void printFinal() {
      std::cout << "FINAL RESERVOIR:" << std::endl;
      for (size_t i = 0; i < m_reservoir.size(); ++i) {
         std::cout << "  [" << i << "]: " << m_reservoir[i].getP3Distance() << std::endl;
      }
   }

private:
   std::vector<TestResult> m_reservoir;
   size_t m_maxReservoirSize;
};


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
         std::cout << ";CmdLMP3 requires at least 2 input cells" << std::endl;
         return 0;
      }

      runInputListMode(inputList, controls);
      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return -1;
   }
}

