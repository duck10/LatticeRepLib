#ifndef NIGGLIREDUCER_H
#define NIGGLIREDUCER_H

#include "LRL_Cell.h"
#include "Matrix_3x3.h"
#include "G6.h"
#include "MatG6.h"
#include "TransformerUtilities.h"  // For operator* and utility functions
#include <iostream>

class NiggliReducer {
public:
   // Structure to hold reduction results
   struct ReductionResult {
      LRL_Cell reducedCell;
      Matrix_3x3 reductionMatrix;
      Matrix_3x3 inverseReductionMatrix;
      bool isAlreadyReduced;

      ReductionResult()
         : reducedCell()
         , reductionMatrix()
         , inverseReductionMatrix()
         , isAlreadyReduced(false)
      {
      }

      ReductionResult(const LRL_Cell& reduced,
         const Matrix_3x3& reductionMat,
         const Matrix_3x3& inverseReductionMat,
         const bool alreadyReduced)
         : reducedCell(reduced)
         , reductionMatrix(reductionMat)
         , inverseReductionMatrix(inverseReductionMat)
         , isAlreadyReduced(alreadyReduced)
      {
      }

      friend std::ostream& operator<<(std::ostream& os, const ReductionResult& result) {
         os << "; NiggliReducer::ReductionResult\n";
         os << "; Already reduced: " << (result.isAlreadyReduced ? "true" : "false") << "\n";
         os << "; Reduced cell: " << LRL_Cell_Degrees(result.reducedCell) << "\n";
         os << "; Reduction matrix:\n" << result.reductionMatrix;
         os << "; Inverse reduction matrix:\n" << result.inverseReductionMatrix;
         return os;
      }
   };

   // Reduce a cell to Niggli form
   static ReductionResult reduce(const LRL_Cell& inputCell,
      const double delta = 1.0e-5);

   // Check if a cell is already in Niggli form
   static bool isReduced(const LRL_Cell& cell, const double delta = 1.0e-5);

   // Apply the inverse reduction transformation to a transformation matrix
   static Matrix_3x3 applyInverseReductionToMatrix(const Matrix_3x3& reducedTransform,
      const ReductionResult& referenceReduction,
      const ReductionResult& mobileReduction);

private:
   // Helper function to compute inverse matrix
   static Matrix_3x3 computeInverse(const Matrix_3x3& matrix);
};

// Implementation
inline NiggliReducer::ReductionResult
NiggliReducer::reduce(const LRL_Cell& inputCell, const double delta) {
   // Check if already reduced
   const bool alreadyReduced = isReduced(inputCell, delta);

   if (alreadyReduced) {
      // Return identity transformation
      const Matrix_3x3 identity(1, 0, 0, 0, 1, 0, 0, 0, 1);
      return ReductionResult(inputCell, identity, identity, true);
   }

   // Perform Niggli reduction with transformation matrices
   G6 reducedG6;
   MatG6 matG6;
   Matrix_3x3 matrix3d;

   const bool success = Niggli::ReduceWithTransforms(G6(inputCell),
      matG6, matrix3d,
      reducedG6, delta);

   if (!success) {
      // Return original cell if reduction failed
      const Matrix_3x3 identity(1, 0, 0, 0, 1, 0, 0, 0, 1);
      return ReductionResult(inputCell, identity, identity, true);
   }

   const LRL_Cell reducedCell(reducedG6);
   const Matrix_3x3 inverseMatrix = computeInverse(matrix3d);

   return ReductionResult(reducedCell, matrix3d, inverseMatrix, false);
}

inline bool NiggliReducer::isReduced(const LRL_Cell& cell, const double delta) {
   return Niggli::IsNiggli(G6(cell), delta);
}

inline Matrix_3x3
NiggliReducer::applyInverseReductionToMatrix(const Matrix_3x3& reducedTransform,
   const ReductionResult& referenceReduction,
   const ReductionResult& mobileReduction) {
   // Transform from original mobile to original reference:
   // originalRef = invReductionRef * reducedTransform * reductionMobile * originalMobile
   return referenceReduction.inverseReductionMatrix *
      reducedTransform *
      mobileReduction.reductionMatrix;
}

inline Matrix_3x3 NiggliReducer::computeInverse(const Matrix_3x3& matrix) {
   try {
      return matrix.Inverse();
   }
   catch (...) {
      // Return identity if inverse computation fails
      return Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
   }
}

#endif // NIGGLIREDUCER_H

