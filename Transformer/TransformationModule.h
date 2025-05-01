#ifndef TRANSFORMATION_MODULE_H
#define TRANSFORMATION_MODULE_H

#include "LatticeCell.h"
#include "B4Matcher.h"
#include "MultiTransformFinderControls.h"
#include <vector>
#include <limits>

// Structure to hold transformation results
struct TransformationResult {
   std::vector<B4Matcher::TransformResult> transformations;
   double bestDistance;
   bool isValid;
   int duplicatesRemoved;

   TransformationResult()
      : bestDistance(std::numeric_limits<double>::max()),
      isValid(false),
      duplicatesRemoved(0) {
   }
};

// Core transformation function for primitive cells
TransformationResult calculatePrimitiveTransformation(
   const LatticeCell& cellToTransform,
   const LatticeCell& referenceCell,
   const MultiTransformFinderControls& controls);

#endif // TRANSFORMATION_MODULE_H

