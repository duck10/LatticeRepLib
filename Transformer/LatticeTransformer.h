#ifndef LATTICETRANSFORMER_H
#define LATTICETRANSFORMER_H

#include "LatticeCell.h"
#include "LRL_Cell.h"
#include "LRL_Vector3.h"
#include "TransformationModule.h"
#include "MultiTransformFinderControls.h"
#include "S6.h"
#include "P3.h"
#include "Selling.h"
#include "CS6Dist.h"
#include "B4Matcher.h"

#include <algorithm>
#include <vector>
#include <string>

// Structure to hold the complete transformation with centering information
struct CompleteTransformationResult {
   TransformationModuleResult primitiveResult;
   Matrix_3x3 inputToPrimitiveMatrix;
   Matrix_3x3 referenceToPrimitiveMatrix;
   std::string inputCenteringType;
   std::string referenceCenteringType;
   bool hasCentering;

   CompleteTransformationResult() : hasCentering(false) {}
};

class LatticeTransformer {
private:
   const MultiTransformFinderControls& m_controls;

   // Helper methods for centering transformations
   Matrix_3x3 getCenteringToStandardMatrix(const LatticeCell& cell) const;
   Matrix_3x3 getStandardToCenteringMatrix(const std::string& centeringType, const LRL_Cell& cell) const;
   bool isCenteredLattice(const std::string& centeringType) const;

   // Helper for calculating primitive transformations
   TransformationModuleResult calculatePrimitiveTransformation(
      const LatticeCell& cellToTransform,
      const LatticeCell& referenceCell,
      const MultiTransformFinderControls& controls) const;

   // Helper to sort transformations based on complete transformation quality
   void sortTransformationsByCompleteQuality(
      std::vector<B4Matcher::TransformResult>& transformations) const;

public:
   LatticeTransformer(const MultiTransformFinderControls& controls);

   // Main transformation method
   CompleteTransformationResult transformLattice(
      const LatticeCell& cellToTransform,
      const LatticeCell& referenceCell) const;
};

#endif // LATTICETRANSFORMER_H