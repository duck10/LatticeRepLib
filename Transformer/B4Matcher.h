﻿#ifndef B4_MATCHER_H
#define B4_MATCHER_H

#include "LRL_Cell.h"
#include "Matrix_3x3.h"
#include "B4.h"
#include "S6.h"
#include "LatticeCell.h"
#include "TransformResult.h" // Include the shared TransformResult
#include "TransformationReservoir.h"

#include <vector>

class MultiTransformFinderControls; // Forward declaration

class B4Matcher {
public:

   friend inline std::ostream& operator<<(std::ostream& os, const B4Matcher& matcher) {
      os << "B4Matcher:\n";
      os << "  Source B4: " << matcher.getSourceB4() << "\n";
      os << "  Target B4: " << matcher.getTargetB4() << "\n";
      os << "  Transformations found: " << matcher.size() << "\n";

      // Show first few transformations if any
      size_t maxShow = std::min(static_cast<size_t>(3), matcher.size());
      for (size_t i = 0; i < maxShow; ++i) {
         const auto& transform = matcher.getTransform(i);
         os << "  Transform " << (i + 1) << ": P3=" << transform.p3DistanceFinal
            << ", B4Dist=" << transform.b4Distance << "\n";
      }

      return os;
   }


   // Now using the global TransformResult struct
   typedef TransformResult TransformResult;

   // Constructors
   B4Matcher(const LRL_Cell& cellToTransform, const LRL_Cell& referenceCell,
      const MultiTransformFinderControls& controls);

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
   std::vector<Matrix_3x3> generateTransformationMatrices() const;
   void FindTransformationsWithReservoir();  // New method using reservoir

private:
   B4 m_b4CellToTransform;   // Source cell in B4 form
   B4 m_b4ReferenceCell;     // Target cell in B4 form    
   Matrix_3x3 m_sourceNiggliMatrix;
   Matrix_3x3 m_targetNiggliMatrix;
   const MultiTransformFinderControls& m_controls;  // Controls for transformation search
   std::vector<TransformResult> m_transformResults;  // Results from transformation
   std::vector<Matrix_3x3> generatePriorityOrderedMatrices() const;
   std::set<Matrix_3x3> generateExtendedShearMatrices() const;

   std::set<Matrix_3x3> loadBasicMatrices() const;
   std::set<Matrix_3x3> generatePermutationShearCombinations(
      const std::set<Matrix_3x3>& existingMatrices,
      size_t maxTotalMatrices) const;
   bool isPermutationMatrix(const Matrix_3x3& matrix) const;
   bool hasValidDeterminant(const Matrix_3x3& matrix) const;
   std::set<Matrix_3x3> generateTripleCombinations() const;
   std::set<Matrix_3x3> generateShearShearCombinations(
      const std::set<Matrix_3x3>& existingMatrices) const;

   // Helper methods for matrix generation and processing
   bool isValidTransformationMatrix(const Matrix_3x3& matrix) const;
};

#endif // B4_MATCHER_H

