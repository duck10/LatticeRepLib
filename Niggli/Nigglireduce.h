#ifndef NIGGLI_REDUCE_H
#define NIGGLI_REDUCE_H

#include "G6.h"
#include "NiggliTypes.h"
#include "NiggliCheck.h"
#include <cmath>
#include <cfloat>
#include <iostream>
#include <iomanip>

// ============================================================
// NiggliReduce
//
// Single reduction engine implementing all algorithm variants
// controlled by NiggliOptions.
//
// Steps 5-8 always decrease Tr(G) when they fire (proven by
// direct arithmetic: the firing condition and the Tr decrease
// are the same inequality). Normalization (steps 1-2-sign) is
// required for completeness -- without it the algorithm
// terminates at non-Niggli cells on ~34% of inputs.
//
// NIGGLI_DEBUG: set to 1 to compile in per-step trace output.
// Pass debug=true to Reduce() to enable it for a specific cell.
// When NIGGLI_DEBUG is 0 the trace code compiles away entirely.
// ============================================================

#define NIGGLI_DEBUG 0

class NiggliReduce {
public:
   static NiggliResult Reduce(const G6& input, const NiggliOptions& opt,
      bool debug = false);

   // All step functions are public to allow diagnostic use in test programs.

   static bool Normalize(double& g1, double& g2, double& g3,
      double& g4, double& g5, double& g6,
      const double eps);

   static bool Step1(double& g1, double& g2, double& g4, double& g5,
      const double eps);
   static bool Step2(double& g2, double& g3, double& g5, double& g6,
      const double eps);
   static bool StepSign(double& g4, double& g5, double& g6, const double eps);
   static bool Step5(double& g2, double& g3, double& g4, double& g5,
      double& g6, const double eps);
   static bool Step6(double& g1, double& g3, double& g4, double& g5,
      double& g6, const double eps);
   static bool Step7(double& g1, double& g2, double& g4, double& g5,
      double& g6, const double eps);
   static bool Step8(double& g1, double& g2, double& g3, double& g4,
      double& g5, double& g6, const double eps);

private:
   static int Sign(const double x) { return (x >= 0.0) ? 1 : -1; }
};

// ------------------------------------------------------------
// Step implementations
// ------------------------------------------------------------

inline bool NiggliReduce::Step1(
   double& g1, double& g2, double& g4, double& g5, const double eps)
{
   if (g1 > g2 + eps ||
      (std::abs(g1 - g2) <= eps && std::abs(g4) > std::abs(g5) + eps))
   {
      std::swap(g1, g2);
      std::swap(g4, g5);
      return true;
   }
   return false;
}

inline bool NiggliReduce::Step2(
   double& g2, double& g3, double& g5, double& g6, const double eps)
{
   if (g2 > g3 + eps ||
      (std::abs(g2 - g3) <= eps && std::abs(g5) > std::abs(g6) + eps))
   {
      std::swap(g2, g3);
      std::swap(g5, g6);
      return true;
   }
   return false;
}

inline bool NiggliReduce::StepSign(
   double& g4, double& g5, double& g6, const double eps)
{
   // 8-case switch matching MKnormWithoutMatrices.
   // Zero is treated as negative: minusPattern uses <= eps.
   // Zero-pattern tie-breaking required for boundary cases.
   int minusPattern = 0;
   int zeroPattern = 0;

   if (g4 <= eps) minusPattern |= 4;
   if (g5 <= eps) minusPattern |= 2;
   if (g6 <= eps) minusPattern |= 1;

   if (std::abs(g4) <= eps) zeroPattern |= 4;
   if (std::abs(g5) <= eps) zeroPattern |= 2;
   if (std::abs(g6) <= eps) zeroPattern |= 1;

   // cases 0 (all-plus) and 7 (all-minus including zeros) are canonical
   if (minusPattern == 0 || minusPattern == 7) return false;

   switch (minusPattern) {
   case 1:  // ++- -> ---
      g4 = -g4; g5 = -g5;
      break;
   case 2:  // +-+ -> ---
      g4 = -g4; g6 = -g6;
      break;
   case 3:  // +-- : zero tie-break
      if ((zeroPattern & 2) == 2) { g4 = -g4; g5 = -g5; }  // +0-
      else if ((zeroPattern & 1) == 1) { g4 = -g4; g6 = -g6; }  // +-0
      else { g5 = -g5; g6 = -g6; }   // +-- -> +++
      break;
   case 4:  // -++ -> ---
      g5 = -g5; g6 = -g6;
      break;
   case 5:  // -+- : zero tie-break
      if ((zeroPattern & 4) == 4) { g4 = -g4; g5 = -g5; }  // 0+-
      else if ((zeroPattern & 1) == 1) { g5 = -g5; g6 = -g6; }  // -+0
      else { g4 = -g4; g6 = -g6; }   // -+- -> +++
      break;
   case 6:  // --+ : zero tie-break
      if ((zeroPattern & 4) == 4) { g4 = -g4; g6 = -g6; }  // 0-+
      else if ((zeroPattern & 2) == 2) { g5 = -g5; g6 = -g6; }  // -0+
      else { g4 = -g4; g5 = -g5; }   // --+ -> +++
      break;
   }
   return true;
}

inline bool NiggliReduce::Step5(
   double& g2, double& g3, double& g4, double& g5, double& g6,
   const double eps)
{
   // Unconditional diagnostic -- always prints when Step5 is called
   bool fire = false;
   int which = 0;
   if (std::abs(g4) > g2 + eps) { fire = true; which = 1; }
   if (!fire && std::abs(g4 - g2) <= eps && 2.0 * g5 < g6 - eps) { fire = true; which = 2; }
   if (!fire && std::abs(g4 + g2) <= eps && g6 < -eps) { fire = true; which = 3; }
   if (fire)
      std::cout <<std::setprecision(17)  << "  [Step5] fired which=" << which
      << " g2=" << g2 << " g4=" << g4
      << " g5=" << g5 << " g6=" << g6
      << " eps=" << eps << "\n";
   if (!fire) return false;
   const int s = Sign(g4);
   g3 = g2 + g3 - s * g4;
   g5 = g5 - s * g6;
   g4 = g4 - 2.0 * s * g2;
   return true;
}

inline bool NiggliReduce::Step6(
   double& g1, double& g3, double& g4, double& g5, double& g6,
   const double eps)
{
   bool fire = false;
   if (std::abs(g5) > g1 + eps)                                    fire = true;
   if (!fire && std::abs(g5 - g1) <= eps && 2.0 * g4 < g6 - eps)    fire = true;
   if (!fire && std::abs(g5 + g1) <= eps && g6 < -eps)             fire = true;
   if (!fire) return false;
   const int s = Sign(g5);
   g3 = g1 + g3 - s * g5;
   g4 = g4 - s * g6;
   g5 = g5 - 2.0 * s * g1;
   return true;
}

inline bool NiggliReduce::Step7(
   double& g1, double& g2, double& g4, double& g5, double& g6,
   const double eps)
{
   bool fire = false;
   if (std::abs(g6) > g1 + eps)                                    fire = true;
   if (!fire && std::abs(g6 - g1) <= eps && 2.0 * g4 < g5 - eps)    fire = true;
   if (!fire && std::abs(g6 + g1) <= eps && g5 < -eps)             fire = true;
   if (!fire) return false;
   const int s = Sign(g6);
   g2 = g1 + g2 - s * g6;
   g4 = g4 - s * g5;
   g6 = g6 - 2.0 * s * g1;
   return true;
}

inline bool NiggliReduce::Step8(
   double& g1, double& g2, double& g3, double& g4, double& g5, double& g6,
   const double eps)
{
   const double sum = g4 + g5 + g6 + g1 + g2;
   bool fire = false;
   if (sum < -eps)                                                   fire = true;
   if (!fire && std::abs(sum) <= eps && 2.0 * (g1 + g5) + g6 > eps)       fire = true;
   if (!fire) return false;
   g3 = g1 + g2 + g3 + g4 + g5 + g6;
   g4 = 2.0 * g2 + g4 + g6;
   g5 = 2.0 * g1 + g5 + g6;
   return true;
}

// ------------------------------------------------------------
// Normalize: run steps 1, 2, sign until stable
// ------------------------------------------------------------

inline bool NiggliReduce::Normalize(
   double& g1, double& g2, double& g3,
   double& g4, double& g5, double& g6,
   const double eps)
{
   bool anyChanged = false;
   bool changed;
   int count = 0;
   const int maxNorm = 50;

   do {
      changed = false;
      if (Step1(g1, g2, g4, g5, eps)) { changed = true; anyChanged = true; }
      if (Step2(g2, g3, g5, g6, eps)) { changed = true; anyChanged = true; continue; }
      if (StepSign(g4, g5, g6, eps)) { changed = true; anyChanged = true; continue; }
      ++count;
      if (count > maxNorm) {
         std::cout << "WARNING: Normalize loop exceeded " << maxNorm
            << " iterations  g=("
            << g1 << "," << g2 << "," << g3 << ","
            << g4 << "," << g5 << "," << g6 << ")" << std::endl;
         break;
      }
   } while (changed);
   return anyChanged;
}

// ------------------------------------------------------------
// Main reduction
// ------------------------------------------------------------

inline NiggliResult NiggliReduce::Reduce(
   const G6& input, const NiggliOptions& opt, bool debug)
{
   NiggliResult result;
   result.traceInitial = input[0] + input[1] + input[2];

   double g1 = input[0], g2 = input[1], g3 = input[2];
   double g4 = input[3], g5 = input[4], g6 = input[5];

   const double eps = opt.epsilon;
   const bool tracePlateau =
      (opt.termination == NiggliOptions::Termination::TracePlateau);

#if NIGGLI_DEBUG
   if (debug) {
      std::cout << std::fixed << std::setprecision(6);
      std::cout << "Reduce input: ("
         << g1 << "," << g2 << "," << g3 << "," << g4 << "," << g5 << "," << g6 << ")"
         << "  preset=" << opt.Label() << "\n";
   }
#endif

   // Entry normalization
   Normalize(g1, g2, g3, g4, g5, g6, eps);

#if NIGGLI_DEBUG
   if (debug)
      std::cout << "After entry normalize: ("
      << g1 << "," << g2 << "," << g3 << "," << g4 << "," << g5 << "," << g6 << ")"
      << "  Tr=" << g1 + g2 + g3 << "\n";
#endif

   int iter = 0;

   while (iter < opt.maxIterations) {
      ++iter;

      Normalize(g1, g2, g3, g4, g5, g6, eps);
      const double trBefore = g1 + g2 + g3;

#if NIGGLI_DEBUG
      if (debug)
         std::cout << "iter=" << iter
         << "  g=(" << g1 << "," << g2 << "," << g3 << ","
         << g4 << "," << g5 << "," << g6 << ")"
         << "  Tr=" << trBefore << "\n";
#endif

      const bool s5 = Step5(g2, g3, g4, g5, g6, eps);
      if (s5) {
         ++result.steps;
         Normalize(g1, g2, g3, g4, g5, g6, eps);
#if NIGGLI_DEBUG
         if (debug)
            std::cout << "  R5/R9  fired -> ("
            << g1 << "," << g2 << "," << g3 << "," << g4 << "," << g5 << "," << g6 << ")"
            << "  Tr=" << g1 + g2 + g3
            << "  dTr=" << trBefore - (g1 + g2 + g3) << "\n";
#endif
      }

      const bool s6 = Step6(g1, g3, g4, g5, g6, eps);
      if (s6) {
         ++result.steps;
         Normalize(g1, g2, g3, g4, g5, g6, eps);
#if NIGGLI_DEBUG
         if (debug)
            std::cout << "  R6/R10 fired -> ("
            << g1 << "," << g2 << "," << g3 << "," << g4 << "," << g5 << "," << g6 << ")"
            << "  Tr=" << g1 + g2 + g3
            << "  dTr=" << trBefore - (g1 + g2 + g3) << "\n";
#endif
      }

      const bool s7 = Step7(g1, g2, g4, g5, g6, eps);
      if (s7) {
         ++result.steps;
         Normalize(g1, g2, g3, g4, g5, g6, eps);
#if NIGGLI_DEBUG
         if (debug)
            std::cout << "  R7/R11 fired -> ("
            << g1 << "," << g2 << "," << g3 << "," << g4 << "," << g5 << "," << g6 << ")"
            << "  Tr=" << g1 + g2 + g3
            << "  dTr=" << trBefore - (g1 + g2 + g3) << "\n";
#endif
      }

      const bool s8 = Step8(g1, g2, g3, g4, g5, g6, eps);
      if (s8) {
         ++result.steps;
         Normalize(g1, g2, g3, g4, g5, g6, eps);
#if NIGGLI_DEBUG
         if (debug)
            std::cout << "  R8/R12 fired -> ("
            << g1 << "," << g2 << "," << g3 << "," << g4 << "," << g5 << "," << g6 << ")"
            << "  Tr=" << g1 + g2 + g3
            << "  dTr=" << trBefore - (g1 + g2 + g3) << "\n";
#endif
      }

      const bool anyFired = s5 || s6 || s7 || s8;

      if (tracePlateau) {
         if (!anyFired) {
            result.converged = true;
            break;
         }
         const double trAfter = g1 + g2 + g3;
         const double decrease = trBefore - trAfter;
         const double fracDecrease = (trBefore > 0.0)
            ? decrease / trBefore : decrease;
         if (fracDecrease < opt.traceTol) {
            result.converged = true;
#if NIGGLI_DEBUG
            if (debug)
               std::cout << "  TracePlateau termination: anyFired but dTr="
               << decrease << " < traceTol\n";
#endif
            break;
         }
      } else {
         if (!anyFired) {
            result.converged = true;
            break;
         }
      }
   }

   // Exit normalization
   Normalize(g1, g2, g3, g4, g5, g6, eps);

   result.cell = G6(g1, g2, g3, g4, g5, g6);
   result.iterations = iter;
   result.traceFinal = g1 + g2 + g3;
   result.isReduced = IsNiggliReduced(result.cell, 0.0);

#if NIGGLI_DEBUG
   if (debug)
      std::cout << "Result: ("
      << g1 << "," << g2 << "," << g3 << "," << g4 << "," << g5 << "," << g6 << ")"
      << "  isNig=" << (result.isReduced ? "Y" : "N") << "\n";
#endif

   return result;
}

#endif // NIGGLI_REDUCE_H