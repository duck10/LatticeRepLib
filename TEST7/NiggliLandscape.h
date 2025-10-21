// NiggliLandscape.h - REWRITTEN
// Proper diagnostic tool for Niggli reduction analysis
// Focuses on discrete transformations and condition violations
#ifndef NIGGLI_LANDSCAPE_H
#define NIGGLI_LANDSCAPE_H

#include "LRL_Cell.h"
#include "G6.h"
#include "Niggli.h"
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <cmath>

class NiggliLandscape {
public:
   struct ConditionCheck {
      std::string name;
      std::string description;
      bool passes;
      std::string details;
      std::string recommendation;
   };

   struct Analysis {
      G6 inputCell;
      G6 reducedCell;
      bool isReduced;
      double inputTrace;
      double reducedTrace;
      double inputVolume;
      double reducedVolume;
      int reductionCycles;

      std::vector<ConditionCheck> conditions;
      bool volumePreserved;
   };

   static Analysis AnalyzeLandscape(const G6& g6, double h = 1.0e-6);
   static Analysis AnalyzeLandscape(const LRL_Cell& cell, double h = 1.0e-6);

private:
   static ConditionCheck CheckC1(const G6& g6, double tol);
   static ConditionCheck CheckC2(const G6& g6, double tol);
   static ConditionCheck CheckC3(const G6& g6, double tol);
   static ConditionCheck CheckC4(const G6& g6, double tol);
   static ConditionCheck CheckC5(const G6& g6, double tol);

   static double ComputeTrace(const G6& g6) {
      return g6[0] + g6[1] + g6[2];
   }
};

// ===================================================================
// IMPLEMENTATION
// ===================================================================

inline NiggliLandscape::Analysis
NiggliLandscape::AnalyzeLandscape(const LRL_Cell& cell, double h) {
   return AnalyzeLandscape(G6(cell), h);
}

inline NiggliLandscape::Analysis
NiggliLandscape::AnalyzeLandscape(const G6& g6, double h) {
   Analysis result;
   result.inputCell = g6;
   result.inputTrace = ComputeTrace(g6);

   LRL_Cell cell(g6);
   result.inputVolume = cell.Volume();

   // Check if reduced
   result.isReduced = Niggli::IsNiggli(g6);

   // Check all conditions
   result.conditions.push_back(CheckC1(g6, h));
   result.conditions.push_back(CheckC2(g6, h));
   result.conditions.push_back(CheckC3(g6, h));
   result.conditions.push_back(CheckC4(g6, h));
   result.conditions.push_back(CheckC5(g6, h));

   // Perform reduction if needed
   if (!result.isReduced) {
      G6 reduced;
      bool success = Niggli::Reduce(g6, reduced, h);

      if (success) {
         result.reducedCell = reduced;
         result.reducedTrace = ComputeTrace(reduced);

         LRL_Cell reducedCellObj(reduced);
         result.reducedVolume = reducedCellObj.Volume();
         result.reductionCycles = Niggli::GetCycles();

         result.volumePreserved =
            std::abs(result.reducedVolume - result.inputVolume) < 0.01;
      }
   } else {
      result.reducedCell = g6;
      result.reducedTrace = result.inputTrace;
      result.reducedVolume = result.inputVolume;
      result.reductionCycles = 0;
      result.volumePreserved = true;
   }

   // Print report
   std::cout << "\n" << std::string(70, '=') << std::endl;
   std::cout << "NIGGLI REDUCTION ANALYSIS" << std::endl;
   std::cout << std::string(70, '=') << std::endl;

   std::cout << "\nInput Cell:" << std::endl;
   std::cout << "  G6: " << result.inputCell << std::endl;
   std::cout << "  Trace: " << std::fixed << std::setprecision(2)
      << result.inputTrace << std::endl;
   std::cout << "  Volume: " << result.inputVolume << std::endl;
   std::cout << "  Is Niggli reduced? "
      << (result.isReduced ? "YES" : "NO") << std::endl;

   // Condition checks
   std::cout << "\n" << std::string(70, '-') << std::endl;
   std::cout << "NIGGLI CONDITION CHECKS" << std::endl;
   std::cout << std::string(70, '-') << std::endl;

   for (const auto& check : result.conditions) {
      std::cout << "\n[" << check.name << "] " << check.description;
      std::cout << (check.passes ? " ?" : " ?") << std::endl;
      std::cout << "  " << check.details << std::endl;
      if (!check.passes) {
         std::cout << "  ? " << check.recommendation << std::endl;
      }
   }

   // Reduction result
   if (!result.isReduced) {
      std::cout << "\n" << std::string(70, '-') << std::endl;
      std::cout << "REDUCTION RESULT" << std::endl;
      std::cout << std::string(70, '-') << std::endl;

      std::cout << "\nReduced Cell:" << std::endl;
      std::cout << "  G6: " << result.reducedCell << std::endl;
      std::cout << "  Trace: " << result.reducedTrace;
      if (std::abs(result.reducedTrace - result.inputTrace) > h) {
         std::cout << " (changed from " << result.inputTrace << ")";
      } else {
         std::cout << " (unchanged)";
      }
      std::cout << std::endl;

      std::cout << "  Volume: " << result.reducedVolume;
      if (result.volumePreserved) {
         std::cout << " ? preserved" << std::endl;
      } else {
         std::cout << " ? NOT PRESERVED (BUG!)" << std::endl;
      }

      std::cout << "  Reduction cycles: " << result.reductionCycles << std::endl;
   }

   // Key insights
   std::cout << "\n" << std::string(70, '=') << std::endl;
   std::cout << "KEY INSIGHTS" << std::endl;
   std::cout << std::string(70, '=') << std::endl;

   std::cout << "\n• Niggli reduction uses DISCRETE transformations" << std::endl;
   std::cout << "  (unimodular matrices with determinant ±1)" << std::endl;

   std::cout << "\n• Volume MUST be preserved:" << std::endl;
   if (result.volumePreserved || result.isReduced) {
      std::cout << "  ? Volume preserved correctly" << std::endl;
   } else {
      std::cout << "  ? Volume NOT preserved - implementation bug!" << std::endl;
   }

   std::cout << "\n• This is NOT continuous optimization:" << std::endl;
   std::cout << "  - No gradient descent" << std::endl;
   std::cout << "  - No continuous paths between representations" << std::endl;
   std::cout << "  - Each step is a discrete matrix transformation" << std::endl;

   std::cout << "\n" << std::string(70, '=') << std::endl;

   return result;
}

inline NiggliLandscape::ConditionCheck
NiggliLandscape::CheckC1(const G6& g6, double tol) {
   ConditionCheck check;
   check.name = "C1";
   check.description = "Axis ordering: g1 ? g2 ? g3";

   double g1 = g6[0], g2 = g6[1], g3 = g6[2];
   bool part1 = (g1 <= g2 + tol);
   bool part2 = (g2 <= g3 + tol);
   check.passes = part1 && part2;

   std::ostringstream oss;
   oss << std::fixed << std::setprecision(3);
   oss << "g1=" << g1 << ", g2=" << g2 << ", g3=" << g3;

   if (!part1) {
      oss << " ? g1 > g2 FAILS";
      check.recommendation = "Apply Step 1: Swap (A,?) ? (B,?)";
   } else if (!part2) {
      oss << " ? g2 > g3 FAILS";
      check.recommendation = "Apply Step 2: Swap (B,?) ? (C,?)";
   } else {
      oss << " ? PASS";
   }

   check.details = oss.str();
   return check;
}

inline NiggliLandscape::ConditionCheck
NiggliLandscape::CheckC2(const G6& g6, double tol) {
   ConditionCheck check;
   check.name = "C2";
   check.description = "Scalar product bound: |?| ? B/2";

   double g2 = g6[1], xi = g6[3];
   double limit = g2 / 2.0;
   check.passes = (std::abs(xi) <= limit + tol);

   std::ostringstream oss;
   oss << std::fixed << std::setprecision(3);
   oss << "|?|=" << std::abs(xi) << " vs B/2=" << limit;

   if (!check.passes) {
      oss << " ? FAILS";
      check.recommendation = "Apply Step 5: Reduce ?";
   } else {
      oss << " ? PASS";
   }

   check.details = oss.str();
   return check;
}

inline NiggliLandscape::ConditionCheck
NiggliLandscape::CheckC3(const G6& g6, double tol) {
   ConditionCheck check;
   check.name = "C3";
   check.description = "Scalar product bound: |?| ? A/2";

   double g1 = g6[0], eta = g6[4];
   double limit = g1 / 2.0;
   check.passes = (std::abs(eta) <= limit + tol);

   std::ostringstream oss;
   oss << std::fixed << std::setprecision(3);
   oss << "|?|=" << std::abs(eta) << " vs A/2=" << limit;

   if (!check.passes) {
      oss << " ? FAILS";
      check.recommendation = "Apply Step 6: Reduce ?";
   } else {
      oss << " ? PASS";
   }

   check.details = oss.str();
   return check;
}

inline NiggliLandscape::ConditionCheck
NiggliLandscape::CheckC4(const G6& g6, double tol) {
   ConditionCheck check;
   check.name = "C4";
   check.description = "Scalar product bound: |?| ? A/2";

   double g1 = g6[0], zeta = g6[5];
   double limit = g1 / 2.0;
   check.passes = (std::abs(zeta) <= limit + tol);

   std::ostringstream oss;
   oss << std::fixed << std::setprecision(3);
   oss << "|?|=" << std::abs(zeta) << " vs A/2=" << limit;

   if (!check.passes) {
      oss << " ? FAILS";
      check.recommendation = "Apply Step 7: Reduce ?";
   } else {
      oss << " ? PASS";
   }

   check.details = oss.str();
   return check;
}

inline NiggliLandscape::ConditionCheck
NiggliLandscape::CheckC5(const G6& g6, double tol) {
   ConditionCheck check;
   check.name = "C5";
   check.description = "Special conditions";

   double A = g6[0], B = g6[1];
   double xi = g6[3], eta = g6[4], zeta = g6[5];

   int numPos = (xi > tol ? 1 : 0) + (eta > tol ? 1 : 0) + (zeta > tol ? 1 : 0);

   bool passes = true;
   std::ostringstream oss;
   oss << std::fixed << std::setprecision(3);

   if (numPos == 3) {
      double sum = xi + eta + zeta + A + B;
      passes = (sum <= tol);
      oss << "All positive: sum=" << sum << " (should be ? 0)";
      if (!passes) {
         check.recommendation = "Apply Step 8: Special reduction";
      }
   } else {
      oss << "Sign pattern OK";
   }

   check.passes = passes;
   check.details = oss.str();
   return check;
}

#endif // NIGGLI_LANDSCAPE_H

