#ifndef LATTICE_TRANSFORMER_H
#define LATTICE_TRANSFORMER_H

#include "LatticeCell.h"
#include "TransformationModule.h"
#include "MultiTransformFinderControls.h"
#include "LRL_Vector3.h"  // For Matrix_3x3
#include "TransformerUtilities.h"  // For ToPrimitive and custom operators
#include <string>

// Structure to hold the complete transformation with centering information
struct CompleteTransformationResult {
   TransformationResult primitiveResult;       // Result from primitive transformation
   Matrix_3x3 inputToPrimitiveMatrix;          // Matrix to convert input cell to primitive
   Matrix_3x3 referenceToPrimitiveMatrix;      // Matrix to convert reference cell to primitive
   std::string inputCenteringType;             // Centering type of input cell
   std::string referenceCenteringType;         // Centering type of reference cell
   bool hasCentering;                          // Flag indicating whether centering conversions were needed

   CompleteTransformationResult() : hasCentering(false) {}
};

// LatticeTransformer - handles centered and primitive lattices
class LatticeTransformer {
public:
   LatticeTransformer(const MultiTransformFinderControls& controls);

   // Main function to transform any lattice cell (centered or primitive) to match a reference cell
   CompleteTransformationResult transformLattice(
      const LatticeCell& cellToTransform,
      const LatticeCell& referenceCell);

private:
   const MultiTransformFinderControls& m_controls;

   // Helper functions
   Matrix_3x3 getCenteringToStandardMatrix(const LatticeCell& cell) const;
   Matrix_3x3 getStandardToCenteringMatrix(const std::string& centeringType, const LRL_Cell& cell) const;
   bool isCenteredLattice(const std::string& centeringType) const;
};

#endif // LATTICE_TRANSFORMER_H

