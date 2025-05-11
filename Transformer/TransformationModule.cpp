#include "TransformationModule.h"
#include "B4Matcher.h"
#include "MultiTransformFinderControls.h"
#include <algorithm>
#include <cmath>

TransformationModuleResult TransformationModule::FindBestTransformations(
   const LRL_Cell& cellToTransform,
   const LRL_Cell& referenceCell) const {

   TransformationModuleResult result;

   // Create B4Matcher with search depth from controls
   B4Matcher matcher(cellToTransform, referenceCell, m_controls);

   // Set higher search depth for challenging cases
   const double s6Angle = S6::AngleBetween(S6(cellToTransform), S6(referenceCell));
   const bool isChallengingCase = (s6Angle > 0.1 && s6Angle < 5.0);

   if (isChallengingCase) {
      // If it's a challenging case, manually call FindTransformations with deeper search
      matcher.FindTransformations(5); // Use deeper search for challenging cases
   }

   // Get transformations
   const int numTransforms = std::min(
      static_cast<int>(matcher.size()),
      m_controls.getMaxTransformationsToShow());

   // Check if transformations were found
   if (numTransforms > 0) {
      // Store valid transformations
      int duplicatesRemoved = 0;
      std::vector<B4Matcher::TransformResult> validTransforms;

      for (int i = 0; i < static_cast<int>(matcher.size()); ++i) {
         const B4Matcher::TransformResult& transform = matcher.getTransform(i);

         // Check for duplicates
         bool isDuplicate = false;
         for (const auto& existingTransform : validTransforms) {
            if (isMatrixDuplicate(transform.transformMatrix, existingTransform.transformMatrix)) {
               isDuplicate = true;
               duplicatesRemoved++;
               break;
            }
         }

         if (!isDuplicate) {
            validTransforms.push_back(transform);

            // Only keep up to the maximum number requested
            if (validTransforms.size() >= static_cast<size_t>(numTransforms)) {
               break;
            }
         }
      }

      // Store results
      result.isValid = !validTransforms.empty();
      result.transformations = validTransforms;
      result.duplicatesRemoved = duplicatesRemoved;
   }

   return result;
}


bool TransformationModule::isMatrixDuplicate(
   const Matrix_3x3& m1, const Matrix_3x3& m2, double tolerance) const {

   for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
         if (std::abs(m1[i * 3 + j] - m2[i * 3 + j]) > tolerance) {
            return false;
         }
      }
   }

   return true;
}

