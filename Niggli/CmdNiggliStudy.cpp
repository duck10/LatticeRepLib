#include "G6.h"
#include "LRL_Cell.h"
#include "Polar.h"
#include "NiggliTypes.h"
#include "NiggliReduce.h"
#include "NiggliTiming.h"

#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>


// ============================================================
// Known boundary cells for correctness testing
// ============================================================

static const double s_pi = 4.0 * std::atan(1.0);

static std::vector<G6> BoundaryCells() {
   std::vector<G6> cells;

   // Cubic: a=b=c, alpha=beta=gamma=90
   cells.push_back(G6(4.0, 4.0, 4.0, 0.0, 0.0, 0.0));

   // Tetragonal: a=b, c different, alpha=beta=gamma=90
   cells.push_back(G6(4.0, 4.0, 9.0, 0.0, 0.0, 0.0));

   // Rhombohedral: a=b=c, alpha=75 deg
   {
      const double a2 = 9.0;
      const double off = 2.0 * a2 * std::cos(75.0 * s_pi / 180.0);
      cells.push_back(G6(a2, a2, a2, off, off, off));
   }

   // Hexagonal: a=b, gamma=120, alpha=beta=90
   {
      const double a2 = 9.0;
      const double c2 = 16.0;
      cells.push_back(G6(a2, a2, c2, 0.0, 0.0, -a2));
   }

   // FCC primitive: a=b=c, alpha=60
   {
      const double a2 = 4.0;
      const double off = 2.0 * a2 * std::cos(60.0 * s_pi / 180.0);
      cells.push_back(G6(a2, a2, a2, off, off, off));
   }

   // BCC primitive: a=b=c, alpha=109.47
   {
      const double a2 = 4.0;
      const double off = 2.0 * a2 * std::cos(109.47 * s_pi / 180.0);
      cells.push_back(G6(a2, a2, a2, off, off, off));
   }

   // General low-symmetry cell well away from boundaries
   cells.push_back(G6(4.0, 7.0, 11.0, 1.5, -2.3, 3.1));

   // Near-rhombohedral boundary (epsilon perturbation)
   {
      const double a2 = 9.0;
      const double off = 2.0 * a2 * std::cos(75.0 * s_pi / 180.0) + 1.0e-8;
      cells.push_back(G6(a2, a2 + 1.0e-8, a2 + 2.0e-8, off, off, off));
   }

   // -------------------------------------------------------
   // 2-cycle test cells: satisfy cycling conditions exactly
   // These should expose infinite loops in NoStepFired and
   // incorrect early termination in TracePlateau/GKeps.
   // -------------------------------------------------------

   // F2/F1 2-cycle: g4=-g2, g6<0, 2g5+g6<0, all-minus
   // StepSign stability requires all-minus: mixed signs are normalized
   // away before R9 fires, so cycling condition needs all-minus.
   // Algebra: R9 fires g4=-g2 -> g4_new=+g2, g5_new=g5-g6.
   // Then R9 fires again g4=+g2 -> g4_new=-g2, g5_new=g5_new-g6=g5.
   // Back to start -> infinite 2-cycle.
   cells.push_back(G6(3.0, 4.0, 6.0, -4.0, -1.0, -3.0));

   // F4/F3 2-cycle: g5=-g1, g6<0, 2g4+g6<0, all-minus
   // Verified algebraically: R10 cycles F4-open <-> F3-open indefinitely.
   cells.push_back(G6(4.0, 5.0, 7.0, -0.5, -4.0, -2.0));

   // F6/F5 2-cycle: g6=-g1, g5<0, 2g4+g5<0, all-minus
   // R11 cycles F6-open <-> F5-open indefinitely.
   cells.push_back(G6(4.0, 5.0, 7.0, -0.5, -2.0, -4.0));

   return cells;
}

// ============================================================
// Random valid G6 cells via Polar::rand()
// ============================================================

static std::vector<G6> RandomCells(const int n) {
   std::vector<G6> cells;
   cells.reserve(n);
   while (static_cast<int>(cells.size()) < n) {
      const G6 g(Polar::rand());
      if (g.IsValid()) cells.push_back(g);
   }
   return cells;
}

// ============================================================
// Random cells with large initial Tr (far from reduced).
// Filter: keep only cells where Tr drops by more than 10%.
// ============================================================

static std::vector<G6> RandomCellsFarFromReduced(const int n) {
   std::vector<G6> cells;
   cells.reserve(n);
   const NiggliOptions opt = PresetKrivyGruber1976();
   while (static_cast<int>(cells.size()) < n) {
      const G6 g(Polar::rand());
      if (!g.IsValid()) continue;
      const NiggliResult r = NiggliReduce::Reduce(g, opt);
      if (r.traceFinal > 0.0 && r.traceInitial / r.traceFinal > 1.1)
         cells.push_back(g);
   }
   return cells;
}

// ============================================================
// Main
// ============================================================

int main() {
   // Three viable presets. EntryOnly normalization fails on ~34% of
   // random cells and is excluded -- per-cycle normalization is required
   // for completeness, confirmed empirically.
   const std::vector<NiggliOptions> presets = {
      PresetKrivyGruber1976(),     // classical NoStepFired
      PresetTracePlateau(),        // TracePlateau: handles floating-point cycling
      PresetGrosseKunstleve()      // GK epsilon: incumbent solution for cycling
   };

   // ---- Test 1: boundary cells, verbose per-cell output ----
   {
      const std::vector<G6> boundary = BoundaryCells();
      std::cout << "\n=== Boundary cell correctness ===\n";
      for (const G6& g : boundary) {
         std::cout << "\nInput: " << g << "\n";
         for (const NiggliOptions& opt : presets) {
#if NIGGLI_DEBUG
            const NiggliResult r = NiggliReduce::Reduce(g, opt, true);
#else
            const NiggliResult r = NiggliReduce::Reduce(g, opt);
#endif
            std::cout << "  " << std::left << std::setw(36) << opt.Label()
               << "  iter=" << std::setw(5) << r.iterations
               << "  steps=" << std::setw(5) << r.steps
               << "  conv=" << (r.converged ? "Y" : "N")
               << "  isNig=" << (r.isReduced ? "Y" : "N")
               << "  Tr=" << std::fixed << std::setprecision(6) << r.traceFinal
               << "\n";
         }
      }
   }

   // ---- FCC full trace via NIGGLI_DEBUG ----
   // To see per-step output for TracePlateau on this cell:
   //   set #define NIGGLI_DEBUG 1 in NiggliReduce.h, recompile, run.
   // The trace below uses the manual substep version for cross-checking.
   {
      const G6 fcc(4.0, 4.0, 4.0, 4.0, 4.0, 4.0);
      std::cout << "\n=== FCC full step trace (EachCycle, exact) ===\n";
      std::cout << "Input: " << fcc << "\n";
      std::cout << "Expected output: (4,4,4,-4,-4,0) from cmdniggli\n\n";

      double g1 = fcc[0], g2 = fcc[1], g3 = fcc[2];
      double g4 = fcc[3], g5 = fcc[4], g6 = fcc[5];
      const double eps = 0.0;
      int substep = 0;

      auto printState = [&](const std::string& label) {
         std::cout << "  [" << std::setw(3) << ++substep << "] " << std::left << std::setw(20) << label
            << " g=(" << std::fixed << std::setprecision(4)
            << g1 << "," << g2 << "," << g3 << ","
            << g4 << "," << g5 << "," << g6 << ")"
            << "  Tr=" << g1 + g2 + g3 << "\n";
         };

      // Entry normalization substeps
      std::cout << "-- Entry normalization --\n";
      printState("initial");
      if (NiggliReduce::Step1(g1, g2, g4, g5, eps)) printState("step1");
      if (NiggliReduce::Step2(g2, g3, g5, g6, eps)) printState("step2");
      if (NiggliReduce::StepSign(g4, g5, g6, eps))  printState("stepsign");

      // Main loop, fully instrumented
      for (int iter = 1; iter <= 10; ++iter) {
         std::cout << "-- Iter " << iter << " normalization --\n";
         bool normChanged = false;
         bool changed;
         do {
            changed = false;
            if (NiggliReduce::Step1(g1, g2, g4, g5, eps))
            {
               printState("step1"); changed = normChanged = true;
            }
            if (NiggliReduce::Step2(g2, g3, g5, g6, eps))
            {
               printState("step2"); changed = normChanged = true; continue;
            }
            if (NiggliReduce::StepSign(g4, g5, g6, eps))
            {
               printState("stepsign"); changed = normChanged = true; continue;
            }
         } while (changed);
         if (!normChanged) std::cout << "  (no normalization needed)\n";

         std::cout << "-- Iter " << iter << " reduction steps --\n";
         bool anyFired = false;
         const double trBefore = g1 + g2 + g3;

         if (NiggliReduce::Step5(g2, g3, g4, g5, g6, eps)) {
            printState("step5");
            // normalization after step5
            bool c; do {
               c = false;
               if (NiggliReduce::Step1(g1, g2, g4, g5, eps)) { printState("step1 (post5)"); c = true; }
               if (NiggliReduce::Step2(g2, g3, g5, g6, eps)) { printState("step2 (post5)"); c = true; continue; }
               if (NiggliReduce::StepSign(g4, g5, g6, eps)) { printState("sign (post5)");  c = true; continue; }
            } while (c);
            anyFired = true;
         }
         if (NiggliReduce::Step6(g1, g3, g4, g5, g6, eps)) {
            printState("step6");
            bool c; do {
               c = false;
               if (NiggliReduce::Step1(g1, g2, g4, g5, eps)) { printState("step1 (post6)"); c = true; }
               if (NiggliReduce::Step2(g2, g3, g5, g6, eps)) { printState("step2 (post6)"); c = true; continue; }
               if (NiggliReduce::StepSign(g4, g5, g6, eps)) { printState("sign (post6)");  c = true; continue; }
            } while (c);
            anyFired = true;
         }
         if (NiggliReduce::Step7(g1, g2, g4, g5, g6, eps)) {
            printState("step7");
            bool c; do {
               c = false;
               if (NiggliReduce::Step1(g1, g2, g4, g5, eps)) { printState("step1 (post7)"); c = true; }
               if (NiggliReduce::Step2(g2, g3, g5, g6, eps)) { printState("step2 (post7)"); c = true; continue; }
               if (NiggliReduce::StepSign(g4, g5, g6, eps)) { printState("sign (post7)");  c = true; continue; }
            } while (c);
            anyFired = true;
         }
         if (NiggliReduce::Step8(g1, g2, g3, g4, g5, g6, eps)) {
            printState("step8");
            bool c; do {
               c = false;
               if (NiggliReduce::Step1(g1, g2, g4, g5, eps)) { printState("step1 (post8)"); c = true; }
               if (NiggliReduce::Step2(g2, g3, g5, g6, eps)) { printState("step2 (post8)"); c = true; continue; }
               if (NiggliReduce::StepSign(g4, g5, g6, eps)) { printState("sign (post8)");  c = true; continue; }
            } while (c);
            anyFired = true;
         }

         const double dTr = trBefore - (g1 + g2 + g3);
         if (!anyFired) {
            std::cout << "  --> NoStepFired at iter=" << iter
               << "  final=(" << g1 << "," << g2 << "," << g3 << "," << g4 << "," << g5 << "," << g6 << ")"
               << "  isNig=" << (IsNiggliReduced(G6(g1, g2, g3, g4, g5, g6)) ? "Y" : "N") << "\n";
            break;
         }
         std::cout << "  dTr this iter=" << dTr << "\n";
      }
   }

   //// ---- Test 2: random cells, general ----
   //{
   //   const std::vector<G6> random = RandomCells(10000);
   //   std::cout << "\n=== Random cells (general, n=10000) ===\n";
   //   RunComparison(random, presets, std::cout);
   //}

   //// ---- Test 3: random cells far from reduced ----
   //{
   //   const std::vector<G6> farCells = RandomCellsFarFromReduced(10000);
   //   std::cout << "\n=== Random cells (far from reduced, n=10000) ===\n";
   //   RunComparison(farCells, presets, std::cout);
   //}

   return 0;
}