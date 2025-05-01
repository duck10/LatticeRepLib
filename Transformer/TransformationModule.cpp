#include "TransformationModule.h"

// Function to calculate transformations between primitive cells
TransformationResult calculatePrimitiveTransformation(
   const LatticeCell& cellToTransform,
   const LatticeCell& referenceCell,
   const MultiTransformFinderControls& controls) {

   TransformationResult result;

   // Create B4Matcher with the cells directly - no need for additional conversions
   // for primitive cells
   B4Matcher matcher(cellToTransform.getCell(), referenceCell.getCell());

   // Get maximum number of results to find based on controls
   size_t maxResults = controls.shouldShowDetails() ?
      controls.getMaxResults() :
      std::min(size_t(3), controls.getMaxResults());

   // Find the best transformations
   auto allTransformations = matcher.findBestParameterFit(maxResults, 2);

   // Store the results
   result.transformations = allTransformations;
   result.duplicatesRemoved = 0;  // We're not filtering duplicates here

   // Check if we have valid results
   result.isValid = !result.transformations.empty();

   // Store best distance if valid
   if (result.isValid) {
      result.bestDistance = result.transformations[0].b4Distance;
   }

   return result;
}