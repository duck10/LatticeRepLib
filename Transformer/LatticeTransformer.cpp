#include "LatticeTransformer.h"

LatticeTransformer::LatticeTransformer(const MultiTransformFinderControls& controls)
   : m_controls(controls) {
}

CompleteTransformationResult LatticeTransformer::transformLattice(
   const LatticeCell& cellToTransform,
   const LatticeCell& referenceCell) {

   CompleteTransformationResult result;

   // Store centering types
   result.inputCenteringType = cellToTransform.getLatticeType();
   result.referenceCenteringType = referenceCell.getLatticeType();

   // Check if we need to handle centering
   bool inputIsCentered = isCenteredLattice(result.inputCenteringType);
   bool referenceIsCentered = isCenteredLattice(result.referenceCenteringType);
   result.hasCentering = inputIsCentered || referenceIsCentered;

   // If both are primitive, we can skip the centering conversions
   if (!result.hasCentering) {
      // Just do primitive transformation directly
      result.primitiveResult = calculatePrimitiveTransformation(
         cellToTransform, referenceCell, m_controls);

      // Set identity matrices for completeness
      result.inputToPrimitiveMatrix = Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
      result.referenceToPrimitiveMatrix = Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);

      return result;
   }

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

   // Create LatticeCell objects for the primitive cells
   LatticeCell primitiveInput(inputPrimitiveCell, "P");
   LatticeCell primitiveReference(refPrimitiveCell, "P");

   // -------------------- Layer 2: Core primitive transformation --------------------

   // Calculate primitive-to-primitive transformation
   result.primitiveResult = calculatePrimitiveTransformation(
      primitiveInput, primitiveReference, m_controls);

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