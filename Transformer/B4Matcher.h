#ifndef B4_MATCHER_H
#define B4_MATCHER_H

#include "LRL_Cell.h"
#include "Matrix_3x3.h"
#include "B4.h"
#include "LatticeCell.h"
#include <vector>

class MultiTransformFinderControls; // Forward declaration

class B4Matcher {
public:
   // Enhanced structure to store transformation results
   struct TransformResult {
      Matrix_3x3 transformMatrix;   // The primitive transformation matrix
      double b4Distance;            // B4 distance metric
      B4 transformedB4;             // Resulting transformed B4

      // New fields for complete transformation
      Matrix_3x3 completeTransformMatrix; // Complete transform including centering
      LRL_Cell transformedCompleteCell;   // Final transformed cell with centering
      double s6AngleFinal;               // S6 angle between final and reference
      double p3DistanceFinal;            // P3 distance between final and reference
      double cs6DistanceFinal;           // CS6 distance between final and reference

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

   // Constructors
   B4Matcher(const LRL_Cell& cellToTransform, const LRL_Cell& referenceCell);
   B4Matcher(const LRL_Cell& cellToTransform, const LRL_Cell& referenceCell,
      const MultiTransformFinderControls& controls);
   B4Matcher(const B4& sourceB4, const B4& targetB4, const MultiTransformFinderControls& controls);

   // Find transformations
   void FindTransformations(int searchDepth);

   // Get best transformation
   TransformResult findBestTransformation() const;

   // Get multiple transformations
   std::vector<TransformResult> findMultipleTransformations(size_t maxCount) const;

   // Get source and target B4 objects
   const B4& getSourceB4() const { return m_b4CellToTransform; }
   const B4& getTargetB4() const { return m_b4ReferenceCell; }

   // Get number of transformations found
   size_t size() const { return m_transformResults.size(); }

   // Get transform by index
   const TransformResult& getTransform(size_t index) const { return m_transformResults[index]; }

   // Sort transformations by quality metrics
   void sortTransformationsByQuality();

private:
   B4 m_b4CellToTransform;   // Source cell in B4 form
   B4 m_b4ReferenceCell;     // Target cell in B4 form
   const MultiTransformFinderControls& m_controls;  // Controls for transformation search
   std::vector<TransformResult> m_transformResults;  // Results from transformation

   // Helper methods for matrix generation and processing
   void ProcessMatrix(const Matrix_3x3& matrix);
   double transformationComplexity(const Matrix_3x3& matrix) const;
   std::vector<Matrix_3x3> generatePermutationMatrices() const;
   std::vector<Matrix_3x3> generateCrystallographicMatrices() const;
   std::vector<Matrix_3x3> generateComprehensiveIntegerMatrices(int maxCoeff) const;
   bool isValidTransformationMatrix(const Matrix_3x3& matrix) const;
};

#endif // B4_MATCHER_H

