#ifndef TRANSFORMATIONMODULE_H
#define TRANSFORMATIONMODULE_H

#include "B4Matcher.h"
#include "LRL_Cell.h"
#include <vector>

// In TransformationModule.h, make sure the TransformationModuleResult struct has:
struct TransformationModuleResult {
   Matrix_3x3 transformMatrix;
   LRL_Cell transformedCell;
   double p3Distance;
   double s6Angle;
   bool isValid;
   std::vector<B4Matcher::TransformResult> transformations;
   size_t duplicatesRemoved;

   TransformationModuleResult()
      : transformMatrix()
      , transformedCell()
      , p3Distance(0.0)
      , s6Angle(0.0)
      , isValid(false)
      , duplicatesRemoved(0)
   {
   }
};



// Forward declarations
class MultiTransformFinderControls;

class TransformationModule {
private:
   const MultiTransformFinderControls& m_controls;

public:
   // Constructor
   TransformationModule(const MultiTransformFinderControls& controls)
      : m_controls(controls) {
   }
};

#endif // TRANSFORMATIONMODULE_H