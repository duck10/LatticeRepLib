#ifndef TRANSFORMATIONMODULE_H
#define TRANSFORMATIONMODULE_H

#include "B4Matcher.h"
#include "LRL_Cell.h"
#include <vector>

// Forward declarations
class MultiTransformFinderControls;

// Result structure
struct TransformationModuleResult {
   bool isValid;
   std::vector<B4Matcher::TransformResult> transformations;
   int duplicatesRemoved;

   TransformationModuleResult() : isValid(false), duplicatesRemoved(0) {}
};

class TransformationModule {
private:
   const MultiTransformFinderControls& m_controls;

   // Helper method to check for duplicate matrices
   bool isMatrixDuplicate(
      const Matrix_3x3& m1,
      const Matrix_3x3& m2,
      double tolerance = 1e-6) const;

public:
   // Constructor
   TransformationModule(const MultiTransformFinderControls& controls)
      : m_controls(controls) {
   }

   // Main method to find transformations
   TransformationModuleResult FindBestTransformations(
      const LRL_Cell& cellToTransform,
      const LRL_Cell& referenceCell) const;
};

#endif // TRANSFORMATIONMODULE_H