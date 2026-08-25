#ifndef CHECK_AB_REFLECTION_RELATION_H
#define CHECK_AB_REFLECTION_RELATION_H

// CheckABReflectionRelation.h
//
// Direct test of the hypothesis from the original O1B/O2/O3 boundary-
// candidate diagnosis: are candidates A and B (both produced by CmdSella
// COMPARE from the SAME input cell) actually the same physical lattice,
// just reduced to two different orientations? Per Bravais's theorem, the
// Selling-reduced (all-non-positive) representative of a lattice is
// unique modulo the 24 reflections in MatS6::GetReflections(). If A and
// B are genuinely the same lattice, EXACTLY one of the 24 reflections
// applied to A should land very close to B (near-zero residual). If no
// reflection gets close, A and B are not simply reoriented forms of one
// another, and the residual-table finding needs a different explanation
// than "orientation ambiguity in the reduction pipeline."
//
// PARAMETERIZED (this revision): originally this file hardcoded the
// O1B/O2/O3 candidates directly in main-line code, so every new boundary
// pair (M3's M1B/M2A, M2B's M1A/M1B, ...) needed a source edit + rebuild
// to check. That made it easy to accidentally re-run the OLD hardcoded
// check and mistake its output for a new pair's result -- which is
// exactly what happened this session. Now takes candidateA/candidateB
// (plus optional labels/tolerance) as parameters; the original hardcoded
// call is preserved ONLY as a no-argument overload below, so any existing
// call site that still says CheckABReflectionRelation() with no arguments
// keeps compiling unchanged and keeps checking the original O1B/O2/O3
// pair specifically -- it does NOT become a generic "whatever the last
// investigation was" call.

#include "MatS6.h"
#include "S6.h"

#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

struct ReflectionRelationResult {
   size_t bestIndex = 0;
   double bestResidual = -1.0;
   std::vector<double> allResiduals;
   bool sameLattice = false;   // bestResidual < tolerance
};

inline ReflectionRelationResult CheckABReflectionRelation(
   const S6& candidateA, const S6& candidateB,
   const std::string& labelA = "A", const std::string& labelB = "B",
   const double tolerance = 1.0E-3, const bool verbose = true) {

   const std::vector<MatS6> reflections = MatS6::GetReflections();  // 24 matrices

   ReflectionRelationResult result;
   for (size_t i = 0; i < reflections.size(); ++i) {
      const S6 reflectedA = reflections[i] * candidateA;
      const double residual = (reflectedA - candidateB).norm();
      result.allResiduals.push_back(residual);
      if (result.bestResidual < 0.0 || residual < result.bestResidual) {
         result.bestResidual = residual;
         result.bestIndex = i;
      }
   }
   result.sameLattice = (result.bestResidual < tolerance);

   if (verbose) {
      std::cout << std::fixed << std::setprecision(6);
      std::cout << "Checking whether " << labelA << " and " << labelB
         << " are the same lattice under one of the 24 reflections:" << std::endl;
      std::cout << "  best match: reflection index " << result.bestIndex
         << "  residual ||reflection(" << labelA << ") - " << labelB << "|| = "
         << result.bestResidual << std::endl;

      for (size_t i = 0; i < result.allResiduals.size(); ++i) {
         std::cout << "  [" << i << "] residual = " << result.allResiduals[i] << std::endl;
      }
      std::cout.unsetf(std::ios::floatfield);

      if (result.sameLattice) {
         std::cout << "CONCLUSION: " << labelA << " and " << labelB
            << " ARE the same lattice (near-zero residual under reflection "
            << result.bestIndex << ") -- reduction/remediation is producing "
            << "inconsistent orientations, not encountering distinct library gaps."
            << std::endl;
      }
      else {
         std::cout << "CONCLUSION: no reflection brings " << labelA << " close to "
            << labelB << " (smallest residual " << result.bestResidual
            << " is not near zero) -- " << labelA << " and " << labelB
            << " are NOT simply reoriented forms of the same lattice." << std::endl;
      }
   }

   return result;
}

// Backward-compatible no-argument entry point -- preserves the ORIGINAL
// hardcoded O1B/O2/O3 candidates so the existing (currently commented-out)
// CheckABReflectionRelation() call site in CmdSella.cpp main() still
// compiles and behaves exactly as before. Prefer calling the parameterized
// version above directly for any NEW pair -- do not extend this overload
// to take a "current investigation" pair; that ambiguity (which pair does
// a no-argument call check?) is the exact bug this revision fixes.
inline ReflectionRelationResult CheckABReflectionRelation() {
   const S6 candidateA("-43.34318 -38.15350 -0.00055 -38.13490 -43.34264 -0.02616");
   const S6 candidateB("-38.11642 -43.36206 -0.01851 -38.17840 -43.42403 -0.04436");
   return CheckABReflectionRelation(candidateA, candidateB,
      "O1B/O2/O3 candidate A", "O1B/O2/O3 candidate B");
}

#endif  // CHECK_AB_REFLECTION_RELATION_H
