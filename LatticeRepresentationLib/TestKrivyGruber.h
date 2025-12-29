// written by KIMI.AI
//  test_KrivyGruberG6.h
//  Stand-alone unit-test for KrivyGruberG6.h
//  Public domain / CC-0  https://creativecommons.org/publicdomain/zero/1.0/
#ifndef TEST_KRIVYGRUBERG6_H_
#define TEST_KRIVYGRUBERG6_H_

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "KrivyGruberG6.h"   // the code under test

double DetG6(const G6& g) {
   return g[0] * (g[1] * g[2] - g[5] * g[5])
      - g[3] * (g[3] * g[2] - g[5] * g[4])
      + g[4] * (g[3] * g[5] - g[1] * g[4]);
}

namespace KrivyGruberTest {

   /* ---------- helpers ---------- */
   static double rand01() { return std::rand() / (RAND_MAX + 1.0); }

   // random G6 that is positive-definite (sufficient but not necessary condition)
   static G6 randomG6()
   {
      G6 g;
      // diagonal must be positive
      g[0] = 1.0 + 10.0 * rand01();
      g[1] = 1.0 + 10.0 * rand01();
      g[2] = 1.0 + 10.0 * rand01();
      // off-diagonal smaller than diagonals
      g[3] = 0.5 * (2.0 * rand01() - 1.0) * std::sqrt(g[0] * g[1]);
      g[4] = 0.5 * (2.0 * rand01() - 1.0) * std::sqrt(g[0] * g[2]);
      g[5] = 0.5 * (2.0 * rand01() - 1.0) * std::sqrt(g[1] * g[2]);
      return g;
   }

   // maximum absolute difference
   static double maxDiff(const G6& a, const G6& b)
   {
      double d = 0.0;
      for (int i = 0; i < 6; ++i) d = std::max(d, std::fabs(a[i] - b[i]));
      return d;
   }

   /* ---------- test cases ---------- */
   static bool testIdempotency()
   {
      const double tol = 1e-10;
      for (int t = 0; t < 500; ++t) {
         G6 in = randomG6();
         ReductionResult r1 = KrivyGruberG6<>::Reduce(in, tol, false);
         ReductionResult r2 = KrivyGruberG6<>::Reduce(r1.reducedCell, tol, false);
         if (maxDiff(r1.reducedCell, r2.reducedCell) > tol) {
            std::cout << "not ok - idempotency #" << t << "\n";
            return false;
         }
         if (r2.status != ReductionStatus::Success) {
            std::cout << "not ok - idempotency changed flag #" << t << "\n";
            return false;
         }
      }
      std::cout << "ok - idempotency 500 trials\n";
      return true;
   }

   // Published example from K?ivý & Gruber (1976) Table 1, line 1
   static bool testBibliographicExample()
   {
      // VALID input from Krivy & Gruber (1976) Table 1, line 1
      G6 in = { 86.0, 45.0, 19.0, -25.0, -16.0, -9.0 };
      const double tol = 1e-8;

      // Reduce
      auto r = KrivyGruberG6<>::Reduce(in, tol, false);

      // Invariants that MUST hold
      bool ok = true;

      // 1. Volume preserved
      const double volIn = sqrt(DetG6(in));
      const double volOut = sqrt(DetG6(r.reducedCell));
      if (fabs(volIn - volOut) > tol * volIn) {
         std::cout << "not ok - volume not preserved\n";
         ok = false;
      }

      // 2. Cell is reduced (no predicate true)
      if (!KrivyGruberG6<>::IsReduced(r.reducedCell, tol)) {
         std::cout << "not ok - cell not reduced\n";
         ok = false;
      }

      // 3. Idempotency
      auto r2 = KrivyGruberG6<>::Reduce(r.reducedCell, tol, false);
      if (r2.status != ReductionStatus::Success) {
         std::cout << "not ok - not idempotent\n";
         ok = false;
      }

      if (ok) {
         std::cout << "ok - bibliographic example (invariants hold)\n";
      }
      return ok;
   }

   // Ensure no step predicate is true after reduction
   static bool testConvergence()
   {
      const double tol = 1e-10;
      for (int t = 0; t < 100; ++t) {
         G6 in = randomG6();
         ReductionResult r = KrivyGruberG6<>::Reduce(in, tol, false);
         const G6& g = r.reducedCell;

         if (!KrivyGruberG6<>::IsReduced(g, tol))
         {
            std::cout << "not ok - convergence #" << t << "\n";
            return false;
         }
      }
      std::cout << "ok - convergence 100 trials\n";
      return true;
   }

   // Degenerate (metrically singular) cell must return unchanged and status==InvalidInput
   static bool testInvalidCell()
   {
      G6 bad;
      bad[0] = 1.0;
      bad[1] = 1.0;
      bad[2] = 1.0;
      bad[3] = 99.0;   // violates Cauchy-Schwarz
      bad[4] = 99.0;
      bad[5] = 99.0;

      ReductionResult r = KrivyGruberG6<>::Reduce(bad, 1e-10, false);
      if (r.status != ReductionStatus::InvalidInput) {
         std::cout << "not ok - invalid cell status wrong\n";
         return false;
      }
      if (maxDiff(r.reducedCell, bad) > 0.0) {
         std::cout << "not ok - invalid cell altered\n";
         return false;
      }
      std::cout << "ok - invalid cell untouched\n";
      return true;
   }

   /* ---------- runner ---------- */
   static int run()
   {
      std::srand(12345);            // deterministic
      bool ok = true;
      ok &= testBibliographicExample();
      ok &= testConvergence();
      ok &= testIdempotency();
      ok &= testInvalidCell();
      std::cout << (ok ? "# All tests passed\n" : "# Some tests FAILED\n");
      return ok ? 0 : 1;
   }

} // namespace KrivyGruberTest

/* ---------- entry point ---------- */
#if defined(KRIVYGRUBER_TEST_MAIN)
int main() { return KrivyGruberTest::run(); }
#endif
#endif // TEST_KRIVYGRUBERG6_H_
