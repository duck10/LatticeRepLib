#ifndef TRANSFORMERDISPLAY_H
#define TRANSFORMERDISPLAY_H

#include "MultiTransformFinderControls.h"
#include "B4Matcher.h"
#include "LatticeCell.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "S6.h"
#include "P3.h"
#include "LRL_Vector3.h"

class TransformerDisplay {
private:
   const MultiTransformFinderControls& m_controls;

public:
   TransformerDisplay(const MultiTransformFinderControls& controls);

   void showInputCells(const LatticeCell& cellToTransform, const LatticeCell& referenceCell) const;

   void displayTransformResult(
      const B4Matcher::TransformResult& result,
      const LRL_Cell& cellToTransform,
      const LRL_Cell& referenceCell) const;

   // New method for centered lattices
   void displayCenteredTransformResult(
      const B4Matcher::TransformResult& result,
      const LatticeCell& cellToTransform,
      const LatticeCell& referenceCell,
      const Matrix_3x3& inputToPrimitiveMatrix,
      const Matrix_3x3& primitiveTransformMatrix,
      const Matrix_3x3& referenceToPrimitiveMatrix) const;

   void showB4MatcherResults(const B4Matcher& matcher,
      const LatticeCell& cellToTransform,
      const LatticeCell& referenceCell) const;

   void showB4Transformations(const B4Matcher& matcher,
      const LatticeCell& cellToTransform,
      const LatticeCell& referenceCell) const;

   // Original method - keep the same parameter names for backward compatibility
   void showParameterTransformations(
      const std::vector<B4Matcher::TransformResult>& paramTransformations,
      const LatticeCell& sourceCell,
      const LatticeCell& targetCell,
      int duplicatesRemoved = 0) const;

   // New overloaded method for centered lattices
   void showParameterTransformationsWithCentering(
      const std::vector<B4Matcher::TransformResult>& paramTransformations,
      const LatticeCell& sourceCell,
      const LatticeCell& targetCell,
      int duplicatesRemoved,
      const Matrix_3x3& inputToPrimitiveMatrix,
      const Matrix_3x3& referenceToPrimitiveMatrix) const;
};

#endif // TRANSFORMERDISPLAY_H

