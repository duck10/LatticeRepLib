// TransformResult.h
#ifndef TRANSFORM_RESULT_H
#define TRANSFORM_RESULT_H

#include "Matrix_3x3.h"
#include "LRL_Cell.h"
#include "B4.h"

// Define TransformResult in its own header to avoid circular dependencies
struct TransformResult {
   Matrix_3x3 transformMatrix;       // The primitive transformation matrix
   double b4Distance;                // B4 distance metric
   B4 transformedB4;                 // Resulting transformed B4

   // Complete transformation
   Matrix_3x3 completeTransformMatrix; // Complete transform including centering
   LRL_Cell transformedCompleteCell;   // Final transformed cell with centering
   double s6AngleFinal;                // S6 angle between final and reference
   double p3DistanceFinal;             // P3 distance between final and reference
   double cs6DistanceFinal;            // CS6 distance between final and reference

   // The primitive-to-primitive matrix
   //Matrix_3x3 primitiveToPrimitiveMatrix;

   TransformResult()
      : transformMatrix()
      , b4Distance(0.0)
      , transformedB4()
      , completeTransformMatrix()
      , transformedCompleteCell()
      , s6AngleFinal(19191.0)
      , p3DistanceFinal(19191.0)
      , cs6DistanceFinal(19191.0)
   {
   }
};

#endif // TRANSFORM_RESULT_H
