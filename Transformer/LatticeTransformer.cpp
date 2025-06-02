#include "LatticeTransformer.h"
#include "LRL_Cell_Degrees.h"
#include "TransformationMatrices.h"
#include "TransformerUtilities.h"

LatticeTransformer::LatticeTransformer(const MultiTransformFinderControls& controls)
   : m_controls(controls) {
}

CompleteTransformationResult LatticeTransformer::transformLattice(
   const LatticeCell& cellToTransform,
   const LatticeCell& referenceCell) const
{
   CompleteTransformationResult result;

   // Calculate Niggli matrices if needed
   if (m_controls.shouldUseNiggliReduction()) {
      result.inputNiggliMatrix = TransformerUtilities::calculateNiggliMatrix(cellToTransform.getCell(), m_controls.shouldShowDetails());
      result.referenceNiggliMatrix = TransformerUtilities::calculateNiggliMatrix(referenceCell.getCell(), m_controls.shouldShowDetails());
   }
   else {
      // Set to identity matrices when Niggli reduction is disabled
      result.inputNiggliMatrix = Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
      result.referenceNiggliMatrix = Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
   }

   // Store centering types
   result.inputCenteringType = cellToTransform.getLatticeType();
   result.referenceCenteringType = referenceCell.getLatticeType();

   // Check if we need to handle centering
   const bool inputIsCentered = isCenteredLattice(result.inputCenteringType);
   const bool referenceIsCentered = isCenteredLattice(result.referenceCenteringType);
   result.hasCentering = inputIsCentered || referenceIsCentered;

   // -------------------- Layer 1: Get the centering matrices --------------------
   // Get the centering-to-primitive transformation matrices
   result.inputToPrimitiveMatrix = getCenteringToStandardMatrix(cellToTransform);
   result.referenceToPrimitiveMatrix = getCenteringToStandardMatrix(referenceCell);

   if ( m_controls.shouldShowDetails())
   {
      std::cout << result << std::endl;
   }

   // -------------------- Layer 2: Convert to primitive form --------------------
   // Calculate the primitive cells
   LRL_Cell inputPrimitiveCell = inputIsCentered ?
      result.inputToPrimitiveMatrix * cellToTransform.getCell() :
      cellToTransform.getCell();

   LRL_Cell refPrimitiveCell = referenceIsCentered ?
      result.referenceToPrimitiveMatrix * referenceCell.getCell() :
      referenceCell.getCell();

   // Debug output for primitive cells
   LRL_Cell_Degrees inputPrimitiveDeg(inputPrimitiveCell);
   LRL_Cell_Degrees refPrimitiveDeg(refPrimitiveCell);

   // Check primitive cell P3 distance
   double primP3 = P3::DistanceBetween(inputPrimitiveCell, refPrimitiveCell);
   if (m_controls.shouldShowDetails())
   {
      std::cout << "DEBUG: Input primitive cell: " << inputPrimitiveDeg << std::endl;
      std::cout << "DEBUG: Reference primitive cell: " << refPrimitiveDeg << std::endl;
      std::cout << "DEBUG: P3 distance between primitive cells: " << primP3 << std::endl;
   }

   // -------------------- NEW: Layer 2.5: Find lattice matching matrix --------------------

   // Apply Niggli reduction to the primitive cells
   LRL_Cell reducedInputCell = result.inputNiggliMatrix * inputPrimitiveCell;
   LRL_Cell reducedReferenceCell = result.referenceNiggliMatrix * refPrimitiveCell;

   if (m_controls.shouldShowDetails()) {
      std::cout << "DEBUG: Finding lattice matching between Niggli-reduced cells:" << std::endl;
      std::cout << "DEBUG: Reduced input: " << LRL_Cell_Degrees(reducedInputCell) << std::endl;
      std::cout << "DEBUG: Reduced reference: " << LRL_Cell_Degrees(reducedReferenceCell) << std::endl;
   }

   // Use B4Matcher to find the best transformation between Niggli-reduced cells
   B4Matcher matcher(reducedInputCell, reducedReferenceCell, m_controls);
   matcher.FindTransformationsWithReservoir(); // Use search depth 1 for lattice matching

   if (matcher.size() > 0) {
      // Get the best lattice matching transformation
      auto bestMatch = matcher.findBestTransformation();
      result.latticeMatchingMatrix = bestMatch.transformMatrix;

      if (m_controls.shouldShowDetails()) {
         std::cout << "DEBUG: Found lattice matching matrix (P3 distance = "
            << bestMatch.p3DistanceFinal << "):" << std::endl;
         std::cout << result.latticeMatchingMatrix << std::endl;
         std::cout << "DEBUG: Lattice matching matrix det: " << result.latticeMatchingMatrix.Det() << std::endl;
      }
   }
   else {
      // Fallback to identity if no good match found
      result.latticeMatchingMatrix = Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);

      if (m_controls.shouldShowDetails()) {
         std::cout << "DEBUG: No lattice matching found, using identity matrix" << std::endl;
      }
   }
   // -------------------- Layer 3: Get permutation matrices --------------------
   // Get the basic permutation matrices
   std::vector<Matrix_3x3> permMatrices = MatrixSets::getAllPermutationMatrices();

   // Identity matrix for reduction matrices (if not using Niggli)
   static const Matrix_3x3 identityMatrix(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);

   // Container to store all transformations
   std::vector<TransformResult> allTransforms;

   // -------------------- Layer 4: Process all permutation matrices --------------------
   if (m_controls.shouldShowDetails())
   {
      std::cout << "DEBUG: Testing " << permMatrices.size() << " matrices" << std::endl;
   }


   for (const auto& perm : permMatrices) {
      // DEBUG: Call the convergence debugger for the first permutation only
      if (perm == permMatrices[0] && m_controls.shouldShowDetails()) {
         debugConvergencePattern(cellToTransform, referenceCell, result, perm);
      }


      // Create a TransformResult for this permutation
      TransformResult transform;
      // Store the permutation matrix
      transform.transformMatrix = perm;

      // Construct the complete transformation matrix with correct order
    // Complete = M_1R^-1 * [P] * M_2R^-1 * M_MR * M_2M * M_1M
      transform.completeTransformMatrix =
         result.referenceToPrimitiveMatrix.Inverse() *  // M_1R^-1
         perm *                                         // [P]
         result.referenceNiggliMatrix.Inverse() *       // M_2R^-1
         result.latticeMatchingMatrix *                 // M_MR
         result.inputNiggliMatrix *                     // M_2M
         result.inputToPrimitiveMatrix;                 // M_1M





      Matrix_3x3 step1 = result.inputToPrimitiveMatrix;  // M_1M
      Matrix_3x3 step2 = result.inputNiggliMatrix * step1;  // M_2M * M_1M
      Matrix_3x3 step3 = result.latticeMatchingMatrix * step2;  // M_MR * M_2M * M_1M
      Matrix_3x3 step4 = result.referenceNiggliMatrix.Inverse() * step3;  // M_2R^-1 * M_MR * M_2M * M_1M
      Matrix_3x3 step5 = perm * step4;  // [P] * M_2R^-1 * M_MR * M_2M * M_1M
      Matrix_3x3 final = result.referenceToPrimitiveMatrix.Inverse() * step5;  // Complete

      //std::cout << "Step-by-step transformation matrices:\n";
      //std::cout << "Step 1 (M_1M): det=" << step1.Det() << "\n" << step1 << "\n";
      //std::cout << "Step 2 (M_2M*M_1M): det=" << step2.Det() << "\n" << step2 << "\n";
      //std::cout << "Step 3 (M_MR*M_2M*M_1M): det=" << step3.Det() << "\n" << step3 << "\n";
      //std::cout << "Step 4 (M_2R^-1*M_MR*M_2M*M_1M): det=" << step4.Det() << "\n" << step4 << "\n";
      //std::cout << "Step 5 ([P]*M_2R^-1*M_MR*M_2M*M_1M): det=" << step5.Det() << "\n" << step5 << "\n";
      //std::cout << "Final (M_1R^-1*[P]*M_2R^-1*M_MR*M_2M*M_1M): det=" << final.Det() << "\n" << final << "\n";





      // Apply the complete transformation to the input cell
      transform.transformedCompleteCell =
         transform.completeTransformMatrix * cellToTransform.getCell();

      // Calculate metrics for the final transformation
      S6 transformedS6(transform.transformedCompleteCell);
      S6 referenceS6(referenceCell.getCell());
      transform.s6AngleFinal = TransformerUtilities::angleS6(transformedS6, referenceS6);
      transform.p3DistanceFinal = P3::DistanceBetween(
         transform.transformedCompleteCell, referenceCell.getCell());

      // Debug output for this transformation
      if (m_controls.shouldShowDetails()) {
         double det = transform.completeTransformMatrix.Determinant();
         std::cout << "DEBUG: Permutation - P3=" << transform.p3DistanceFinal
            << " S6=" << transform.s6AngleFinal
            << " Det=" << det << std::endl;
      }

      // Add this transformation to our results
      allTransforms.push_back(transform);
   }




   // -------------------- Layer 5: Sort all transformations --------------------
   // Sort by P3 distance or S6 angle
   std::sort(allTransforms.begin(), allTransforms.end(),
      [this](const TransformResult& a, const TransformResult& b) {
         if (m_controls.shouldSortByP3()) {
            return a.p3DistanceFinal < b.p3DistanceFinal;
         }
         else {
            return a.s6AngleFinal < b.s6AngleFinal;
         }
      });

   // -------------------- Layer 6: Set up result structures --------------------
   result.primitiveResult.isValid = !allTransforms.empty();
   result.primitiveResult.transformations = allTransforms;

   // Debug output
   if (m_controls.shouldShowDetails())
   {
      std::cout << "DEBUG: Found " << allTransforms.size() << " transformations total" << std::endl;
   }

   // Explicitly print the user-requested maximum
   const int maxRequested = m_controls.getMaxTransformationsToShow();
   if (m_controls.shouldShowDetails())
   {
      std::cout << "DEBUG: User requested max = " << maxRequested << std::endl;
   }

   // Limit to the maximum number of transformations to show, but no more than available
   const int maxTransforms = std::min(maxRequested, static_cast<int>(allTransforms.size()));
   result.primitiveResult.transformations.resize(maxTransforms);

   if (m_controls.shouldShowDetails())
   {
      std::cout << "DEBUG: Returning exactly "
         << result.primitiveResult.transformations.size()
         << " transformations" << std::endl;
   }

   return result;
}










// Helper function to sort transformations by P3 distance
void LatticeTransformer::sortTransformationsByCompleteQuality(
   std::vector<B4Matcher::TransformResult>& transformations) const {

   // Debug output before sorting
   std::cout << "DEBUG: Before sorting, first transform P3="
      << transformations[0].p3DistanceFinal << std::endl;

   std::sort(transformations.begin(), transformations.end(),
      [this](const B4Matcher::TransformResult& a, const B4Matcher::TransformResult& b) {
         // Check sorting preference
         if (m_controls.shouldSortByP3()) {
            return a.p3DistanceFinal < b.p3DistanceFinal;
         }
         else {
            return a.s6AngleFinal < b.s6AngleFinal;
         }
      });

   // Debug output after sorting
   std::cout << "DEBUG: After sorting, first transform P3="
      << transformations[0].p3DistanceFinal << std::endl;
}


Matrix_3x3 LatticeTransformer::getCenteringToStandardMatrix(const LatticeCell& cell) const {
   return ToPrimitive(cell.getLatticeType(), cell.getCell());
}

Matrix_3x3 LatticeTransformer::getStandardToCenteringMatrix(
   const std::string& centeringType, const LRL_Cell& cell) const {

   // The inverse of ToPrimitive gives us the transformation from primitive to centered
   Matrix_3x3 primitiveToCentered = ToPrimitive(centeringType, cell).Inver();
   return primitiveToCentered;
}

bool LatticeTransformer::isCenteredLattice(const std::string& centeringType) const {
   // Empty string or "P" is primitive
   return !(centeringType.empty() || centeringType == "P");
}


void LatticeTransformer::debugConvergencePattern(
   const LatticeCell& cellToTransform,
   const LatticeCell& referenceCell,
   const CompleteTransformationResult& result,
   const Matrix_3x3& permutation) const {

   std::cout << "\n=== CONVERGENCE PATTERN DEBUGGER ===" << std::endl;
   std::cout << "Reference DOWN the chain | Mobile UP the chain | P3 Distance" << std::endl;
   std::cout << "========================|====================|============" << std::endl;

   // Reference column: Start with original reference, apply transformations DOWN
   LRL_Cell ref_original = referenceCell.getCell();
   LRL_Cell ref_primitive = result.referenceToPrimitiveMatrix * ref_original;
   LRL_Cell ref_niggli = result.referenceNiggliMatrix * ref_primitive;
   // Reference stays in Niggli space (no lattice matching applied to reference)
   LRL_Cell ref_middle = ref_niggli;

   // Mobile column: Start with original mobile, apply transformations UP
   LRL_Cell mob_original = cellToTransform.getCell();
   LRL_Cell mob_primitive = result.inputToPrimitiveMatrix * mob_original;
   LRL_Cell mob_niggli = result.inputNiggliMatrix * mob_primitive;
   LRL_Cell mob_matched = result.latticeMatchingMatrix * mob_niggli;
   LRL_Cell mob_middle = mob_matched;

   // Step 1: Original cells
   std::cout << "Original reference      | Original mobile        | "
      << P3::DistanceBetween(ref_original, mob_original) << std::endl;
   std::cout << LRL_Cell_Degrees(ref_original) << " | " << LRL_Cell_Degrees(mob_original) << std::endl;
   std::cout << std::endl;

   // Step 2: After centering-to-primitive
   std::cout << "Ref → Primitive         | Mob → Primitive        | "
      << P3::DistanceBetween(ref_primitive, mob_primitive) << std::endl;
   std::cout << LRL_Cell_Degrees(ref_primitive) << " | " << LRL_Cell_Degrees(mob_primitive) << std::endl;
   std::cout << std::endl;

   // Step 3: After Niggli reduction  
   std::cout << "Ref → Niggli            | Mob → Niggli           | "
      << P3::DistanceBetween(ref_niggli, mob_niggli) << std::endl;
   std::cout << LRL_Cell_Degrees(ref_niggli) << " | " << LRL_Cell_Degrees(mob_niggli) << std::endl;
   std::cout << std::endl;

   // Step 4: After lattice matching (MIDDLE - should be nearly identical!)
   std::cout << "Ref (unchanged)         | Mob → Matched          | "
      << P3::DistanceBetween(ref_middle, mob_middle) << std::endl;
   std::cout << LRL_Cell_Degrees(ref_middle) << " | " << LRL_Cell_Degrees(mob_middle) << std::endl;
   std::cout << "^^^ THESE SHOULD BE NEARLY IDENTICAL ^^^" << std::endl;
   std::cout << std::endl;

   // Now go back DOWN the chain with transformations
   // Apply permutation to mobile
   LRL_Cell mob_permuted = permutation * mob_middle;

   std::cout << "Ref (unchanged)         | Mob → Permuted         | "
      << P3::DistanceBetween(ref_middle, mob_permuted) << std::endl;
   std::cout << LRL_Cell_Degrees(ref_middle) << " | " << LRL_Cell_Degrees(mob_permuted) << std::endl;
   std::cout << std::endl;

   // Apply inverse Niggli to reference, keep mobile in permuted state
   LRL_Cell ref_uniggli = result.referenceNiggliMatrix.Inverse() * ref_middle;

   std::cout << "Ref → Un-Niggli         | Mob (unchanged)        | "
      << P3::DistanceBetween(ref_uniggli, mob_permuted) << std::endl;
   std::cout << LRL_Cell_Degrees(ref_uniggli) << " | " << LRL_Cell_Degrees(mob_permuted) << std::endl;
   std::cout << std::endl;

   // Apply inverse centering to reference
   LRL_Cell ref_final = result.referenceToPrimitiveMatrix.Inverse() * ref_uniggli;

   std::cout << "Ref → Final             | Mob (unchanged)        | "
      << P3::DistanceBetween(ref_final, mob_permuted) << std::endl;
   std::cout << LRL_Cell_Degrees(ref_final) << " | " << LRL_Cell_Degrees(mob_permuted) << std::endl;
   std::cout << std::endl;

   // Verification: Does ref_final equal the original reference?
   std::cout << "=== VERIFICATION ===" << std::endl;
   std::cout << "Final reference equals original? " <<
      (LRL_Cell_Degrees(ref_final) == LRL_Cell_Degrees(ref_original) ? "YES" : "NO") << std::endl;
   std::cout << "Original reference: " << LRL_Cell_Degrees(ref_original) << std::endl;
   std::cout << "Final reference:    " << LRL_Cell_Degrees(ref_final) << std::endl;
   std::cout << std::endl;

   // Test complete matrix
   Matrix_3x3 completeMatrix =
      result.referenceToPrimitiveMatrix.Inverse() *
      permutation *
      result.referenceNiggliMatrix.Inverse() *
      result.latticeMatchingMatrix *
      result.inputNiggliMatrix *
      result.inputToPrimitiveMatrix;

   LRL_Cell mob_complete = completeMatrix * mob_original;

   std::cout << "Complete matrix result: " << LRL_Cell_Degrees(mob_complete) << std::endl;
   std::cout << "Step-by-step result:    " << LRL_Cell_Degrees(mob_permuted) << std::endl;
   std::cout << "Results match: " <<
      (LRL_Cell_Degrees(mob_complete) == LRL_Cell_Degrees(mob_permuted) ? "YES" : "NO") << std::endl;

   std::cout << "Final P3 distance (should transform mobile to match reference): "
      << P3::DistanceBetween(mob_complete, ref_original) << std::endl;

   std::cout << "=== END CONVERGENCE PATTERN ===\n" << std::endl;
}


