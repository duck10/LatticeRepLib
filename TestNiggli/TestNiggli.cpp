#include <iostream>
#include <vector>

#include "TestNiggliControls.h"
#include "ProgramSetup.h"
//#include "NCDist_McCoy.h"
//#include "NCDist.h"
//#include "NCDist_HJB.h"
//#include "S6M_SellingReduce.h"
#include "Polar.h"
#include "LRL_Cell_Degrees.h"
#include "KrivyGruber.h"
#include "Niggli.h"
#include "B4.h"

inline LRL_Cell operator*(const Matrix_3x3& matrix, const LRL_Cell& cell) {
   // Convert cell to B4 representation
   B4 b4Cell(cell);

   // Apply transformation in B4 space
   B4 transformedB4 = matrix * b4Cell;

   // Convert back to LRL_Cell
   return LRL_Cell(transformedB4);
}

// Compact test function that accumulates statistics
struct ComparisonStats {
   int totalTests = 0;
   int kgFailed = 0;
   int abFailed = 0;
   int bothFailed = 0;
   int kgPassedIsNiggli = 0;
   int abPassedIsNiggli = 0;
   int identical = 0;
   int metricallyEquivalent = 0;
   int different = 0;
   double maxG6Diff = 0.0;
   double maxNCDist = 0.0;
   G6 worstG6DiffInput;
   G6 worstNCDistInput;
};

void TestKrivyGruberTransformation(const G6& inputG6, ComparisonStats& stats, double tolerance = 1.0e-6, bool verbose = false) {
   stats.totalTests++;

   if (verbose) {
      std::cout << "\n" << std::string(70, '=') << std::endl;
      std::cout << "Input: " << LRL_Cell_Degrees(inputG6) << std::endl;
      std::cout << "Input G6: " << inputG6 << std::endl;
   }

   // Krivy-Gruber reduction
   KrivyGruber::ReductionResult kgResult = KrivyGruber::ReduceWithTransformation(inputG6, tolerance, false);

   // Niggli (Andrews-Bernstein) reduction
   G6 abReduced;
   bool abSuccess = Niggli::Reduce(inputG6, abReduced, tolerance);

   // Track failures
   if (!kgResult.success) stats.kgFailed++;
   if (!abSuccess) stats.abFailed++;
   if (!kgResult.success && !abSuccess) stats.bothFailed++;

   if (!kgResult.success || !abSuccess) {
      if (verbose) {
         if (!kgResult.success) std::cout << "KG: FAILED" << std::endl;
         if (!abSuccess) std::cout << "AB: FAILED" << std::endl;
      }
      return;
   }

   // Check IsNiggli for both
   if (Niggli::IsNiggli(kgResult.reducedG6, tolerance)) stats.kgPassedIsNiggli++;
   if (Niggli::IsNiggli(abReduced, tolerance)) stats.abPassedIsNiggli++;

   // Compare results
   double ncdist = NCDist(kgResult.reducedG6.data(), abReduced.data());
   double maxG6Diff = 0.0;
   bool g6Match = true;

   for (size_t i = 0; i < 6; ++i) {
      double diff = std::abs(kgResult.reducedG6[i] - abReduced[i]);
      maxG6Diff = std::max(maxG6Diff, diff);
      if (diff > tolerance) g6Match = false;
   }

   // Categorize results
   if (g6Match) {
      stats.identical++;
   } else if (ncdist < tolerance) {
      stats.metricallyEquivalent++;
   } else {
      stats.different++;
   }

   // Track worst cases
   if (maxG6Diff > stats.maxG6Diff) {
      stats.maxG6Diff = maxG6Diff;
      stats.worstG6DiffInput = inputG6;
   }
   if (ncdist > stats.maxNCDist) {
      stats.maxNCDist = ncdist;
      stats.worstNCDistInput = inputG6;
   }

   if (verbose) {
      std::cout << "KG G6: " << kgResult.reducedG6 << " IsNiggli: "
         << (Niggli::IsNiggli(kgResult.reducedG6, tolerance) ? "YES" : "NO") << std::endl;
      std::cout << "AB G6: " << abReduced << " IsNiggli: "
         << (Niggli::IsNiggli(abReduced, tolerance) ? "YES" : "NO") << std::endl;
      std::cout << "NCDist: " << ncdist << ", MaxG6Diff: " << maxG6Diff << std::endl;
      if (g6Match) std::cout << "=> IDENTICAL" << std::endl;
      else if (ncdist < tolerance) std::cout << "=> METRICALLY EQUIVALENT" << std::endl;
      else std::cout << "=> DIFFERENT" << std::endl;
   }
}

void PrintSummary(const ComparisonStats& stats, double tolerance) {
   std::cout << "\n" << std::string(70, '=') << std::endl;
   std::cout << "SUMMARY (tolerance = " << tolerance << ")" << std::endl;
   std::cout << std::string(70, '=') << std::endl;
   std::cout << "Total tests:              " << stats.totalTests << std::endl;
   std::cout << "\nFailures:" << std::endl;
   std::cout << "  KG failed:              " << stats.kgFailed
      << " (" << (100.0 * stats.kgFailed / stats.totalTests) << "%)" << std::endl;
   std::cout << "  AB failed:              " << stats.abFailed
      << " (" << (100.0 * stats.abFailed / stats.totalTests) << "%)" << std::endl;
   std::cout << "  Both failed:            " << stats.bothFailed << std::endl;

   int successful = stats.totalTests - std::max(stats.kgFailed, stats.abFailed);
   std::cout << "\nSuccessful reductions:    " << successful << std::endl;
   std::cout << "  KG passed IsNiggli:     " << stats.kgPassedIsNiggli
      << " (" << (successful > 0 ? 100.0 * stats.kgPassedIsNiggli / successful : 0) << "%)" << std::endl;
   std::cout << "  AB passed IsNiggli:     " << stats.abPassedIsNiggli
      << " (" << (successful > 0 ? 100.0 * stats.abPassedIsNiggli / successful : 0) << "%)" << std::endl;

   std::cout << "\nComparison results:" << std::endl;
   std::cout << "  Identical G6:           " << stats.identical
      << " (" << (successful > 0 ? 100.0 * stats.identical / successful : 0) << "%)" << std::endl;
   std::cout << "  Metrically equivalent:  " << stats.metricallyEquivalent
      << " (" << (successful > 0 ? 100.0 * stats.metricallyEquivalent / successful : 0) << "%)" << std::endl;
   std::cout << "  Different:              " << stats.different
      << " (" << (successful > 0 ? 100.0 * stats.different / successful : 0) << "%)" << std::endl;

   std::cout << "\nWorst cases:" << std::endl;
   std::cout << "  Max G6 difference:      " << stats.maxG6Diff << std::endl;
   if (stats.maxG6Diff > 0) {
      std::cout << "    Input: " << stats.worstG6DiffInput << std::endl;
   }
   std::cout << "  Max NCDist:             " << stats.maxNCDist << std::endl;
   if (stats.maxNCDist > 0) {
      std::cout << "    Input: " << stats.worstNCDistInput << std::endl;
   }
   std::cout << std::string(70, '=') << std::endl;
}

int main() {
   std::cout << "; LRL_TEMPLATE" << std::endl;

   TemplateControls controls;
   const BasicProgramInput<TemplateControls> dc_setup("TEMPLATE", controls);

   if (dc_setup.getInputList().empty()) {
      throw std::runtime_error("; No input vectors provided");
   }

   // Test at different tolerances
   std::vector<double> tolerances = { 0.0, 1.0e-6, 1.0e-4 };

   for (double tol : tolerances) {
      std::cout << "\n========================================" << std::endl;
      std::cout << "KRIVY-GRUBER vs ANDREWS-BERNSTEIN" << std::endl;
      std::cout << "Tolerance = " << tol << std::endl;
      std::cout << "========================================" << std::endl;

      ComparisonStats stats;

      for (const auto& input : dc_setup.getInputList()) {
         const G6 g6in = input.getCell();
         TestKrivyGruberTransformation(g6in, stats, tol, false);  // verbose=false
      }

      PrintSummary(stats, tol);
   }


   return 0;
}
