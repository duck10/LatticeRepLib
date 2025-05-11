#include "LatticeTransformer.h"
#include "TransformerUtilities.h"

LatticeTransformer::LatticeTransformer(const MultiTransformFinderControls& controls)
   : m_controls(controls) {
}

CompleteTransformationResult LatticeTransformer::transformLattice(
   const LatticeCell& cellToTransform,
   const LatticeCell& referenceCell) const {

   CompleteTransformationResult result;

   // Store centering types
   result.inputCenteringType = cellToTransform.getLatticeType();
   result.referenceCenteringType = referenceCell.getLatticeType();

   // Check if we need to handle centering
   const bool inputIsCentered = isCenteredLattice(result.inputCenteringType);
   const bool referenceIsCentered = isCenteredLattice(result.referenceCenteringType);
   result.hasCentering = inputIsCentered || referenceIsCentered;

   // -------------------- Layer 1: Centering conversion --------------------
   // Get centering-to-primitive transformation matrices
   result.inputToPrimitiveMatrix = getCenteringToStandardMatrix(cellToTransform);
   result.referenceToPrimitiveMatrix = getCenteringToStandardMatrix(referenceCell);

   // Convert cells to primitive form
   LRL_Cell inputPrimitiveCell = cellToTransform.getCell();
   LRL_Cell refPrimitiveCell = referenceCell.getCell();

   // Apply transformations to get primitive cells
   if (inputIsCentered) {
      inputPrimitiveCell = result.inputToPrimitiveMatrix * cellToTransform.getCell();
   }

   if (referenceIsCentered) {
      refPrimitiveCell = result.referenceToPrimitiveMatrix * referenceCell.getCell();
   }

   // Create LatticeCell objects for the primitive cells with "P" type
   LatticeCell primitiveInput(inputPrimitiveCell, "P");
   LatticeCell primitiveReference(refPrimitiveCell, "P");

   // -------------------- Layer 2: Core primitive transformation --------------------
   // Calculate primitive-to-primitive transformation
   result.primitiveResult = calculatePrimitiveTransformation(
      primitiveInput, primitiveReference, m_controls);

   // -------------------- Layer 3: Handle the complete transformation --------------------
   if (result.primitiveResult.isValid && !result.primitiveResult.transformations.empty()) {
      // For each primitive transformation, calculate the complete transformation
      for (auto& transform : result.primitiveResult.transformations) {
         // Complete transformation = A⁻¹ * P * B
         const Matrix_3x3 completeMatrix =
            result.referenceToPrimitiveMatrix.Inver() *
            transform.transformMatrix *
            result.inputToPrimitiveMatrix;

         // Store the complete matrix
         transform.completeTransformMatrix = completeMatrix;

         // Apply the complete transformation to get the final cell
         LRL_Cell transformedCell = completeMatrix * cellToTransform.getCell();
         transform.transformedCompleteCell = transformedCell;

         // Calculate additional metrics between transformed and reference cells
         S6 transformedS6(transformedCell);
         S6 referenceS6(referenceCell.getCell());

         // Calculate S6 angle (key metric for lattice matching)
         transform.s6AngleFinal = S6::AngleBetween(transformedS6, referenceS6);

         // P3 distance
         transform.p3DistanceFinal = 
            P3::DistanceBetween(transformedCell, referenceCell.getCell());

         // CS6 distance calculation is problematic - let's skip it for now
         // We'll initialize with a placeholder instead
         transform.cs6DistanceFinal = 19191.0;
      }

      // Re-sort transformations based on the complete transformation quality
      sortTransformationsByCompleteQuality(result.primitiveResult.transformations);
   }

   return result;
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

// Helper function to calculate transformations between primitive cells
TransformationModuleResult LatticeTransformer::calculatePrimitiveTransformation(
   const LatticeCell& cellToTransform,
   const LatticeCell& referenceCell,
   const MultiTransformFinderControls& controls) const {

   // Create the transformation module with controls
   TransformationModule transformModule(controls);

   // Calculate transformations
   TransformationModuleResult result = transformModule.FindBestTransformations(
      cellToTransform.getCell(), referenceCell.getCell());

   return result;
}

// Method to sort transformations based on complete transformation quality
void LatticeTransformer::sortTransformationsByCompleteQuality(
   std::vector<B4Matcher::TransformResult>& transformations) const {

   std::sort(transformations.begin(), transformations.end(),
      [this](const B4Matcher::TransformResult& a, const B4Matcher::TransformResult& b) {
         // Check sorting preference
         if (m_controls.shouldSortByP3()) {
            // Primary sorting by P3 distance
            if (std::abs(a.p3DistanceFinal - b.p3DistanceFinal) > 0.01) {
               return a.p3DistanceFinal < b.p3DistanceFinal;
            }

            // If P3 distances are very close, compare S6 angles
            if (std::abs(a.s6AngleFinal - b.s6AngleFinal) > 0.01) {
               return a.s6AngleFinal < b.s6AngleFinal;
            }
         }
         else {
            // Primary sorting by S6 angle
            if (std::abs(a.s6AngleFinal - b.s6AngleFinal) > 0.01) {
               return a.s6AngleFinal < b.s6AngleFinal;
            }

            // If S6 angles are very close, compare P3 distances
            if (std::abs(a.p3DistanceFinal - b.p3DistanceFinal) > 0.01) {
               return a.p3DistanceFinal < b.p3DistanceFinal;
            }
         }

         // If primary metrics are close, prefer simpler transformation matrices
         // Count non-zero elements in matrices
         int nonZerosA = 0;
         int nonZerosB = 0;

         for (int i = 0; i < 9; ++i) {
            if (std::abs(a.completeTransformMatrix[i]) > 1e-10) {
               nonZerosA++;
            }
            if (std::abs(b.completeTransformMatrix[i]) > 1e-10) {
               nonZerosB++;
            }
         }

         if (nonZerosA != nonZerosB) {
            return nonZerosA < nonZerosB;
         }

         // If still tied, use original B4 distance
         return a.b4Distance < b.b4Distance;
      });
}


