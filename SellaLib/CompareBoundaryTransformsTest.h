#ifndef COMPARE_BOUNDARY_TRANSFORMS_TEST_H
#define COMPARE_BOUNDARY_TRANSFORMS_TEST_H

// CompareBoundaryTransformsTest.h
//
// S6BoundaryTransforms::generateOneBoundaryTransform(i) and
// Selling::GetBoundaryTransforms()[i-1] are two independently-derived
// sources for "the" boundary-crossing transform at position i. Only the
// Selling version has a documented cross-check against the published
// Andrews/Bernstein/Sauter paper (see DeloneParentageTest.h's header).
// generateOneBoundaryTransform's row-interchange construction has no
// equivalent check.
//
// A hand-diff of boundary 1 showed the two matrices are NOT entry-for-
// entry equal, but agree after swapping rows 1<->5 and 2<->4 (0-indexed).
// That could be a harmless relabeling or a genuine divergence -- matrix
// printouts alone can't distinguish those. This generates a genuine
// random cell, transforms it with all six matrices from each source, and
// compares the two resulting lists directly.

#include "G6.h"
#include "MatS6.h"
#include "Polar.h"
#include "S6.h"
#include "S6BoundaryTransforms.h"
#include "Selling.h"

#include <iomanip>
#include <iostream>
#include <vector>

inline void CompareBoundaryTransformSets() {
   const G6 randomCell(Polar::rand());
   const S6 s6(randomCell);

   std::cout << "; random cell S6 = " << s6 << std::endl;

   std::vector<S6> generatedResults;
   for (size_t boundary = 1; boundary <= 6; ++boundary) {
      const MatS6 generatedM = S6BoundaryTransforms::generateOneBoundaryTransform(boundary);
      generatedResults.push_back(generatedM * s6);
   }

   const std::vector<MatS6> sellingTransforms = Selling::GetBoundaryTransforms();
   std::vector<S6> sellingResults;
   for (size_t i = 0; i < 6; ++i) {
      sellingResults.push_back(sellingTransforms[i] * s6);
   }

   int mismatches = 0;
   for (size_t i = 0; i < 6; ++i) {
      double maxDiff = 0.0;
      for (size_t k = 0; k < 6; ++k) {
         const double d = std::abs(generatedResults[i][k] - sellingResults[i][k]);
         if (d > maxDiff) maxDiff = d;
      }
      const bool agree = (maxDiff < 1.0E-8);
      if (!agree) ++mismatches;

      std::cout << "boundary " << (i + 1) << ": " << (agree ? "AGREE" : "DIFFER")
         << "  maxDiff = " << std::scientific << std::setprecision(3) << maxDiff << std::endl;
      std::cout << "   generateOneBoundaryTransform : " << generatedResults[i] << std::endl;
      std::cout << "   Selling::GetBoundaryTransforms: " << sellingResults[i] << std::endl;
   }

   std::cout << "; " << (6 - mismatches) << "/6 boundaries agreed" << std::endl;
}

#endif  // COMPARE_BOUNDARY_TRANSFORMS_TEST_H
