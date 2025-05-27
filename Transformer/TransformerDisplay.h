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
#include "LatticeTransformer.h"

class TransformerDisplay {
private:
   const MultiTransformFinderControls& m_controls;
   void displayParameterDifferences(const LRL_Cell& transformed,
      const LRL_Cell& reference) const;


public:
   TransformerDisplay(const MultiTransformFinderControls& controls);

   // Display input cells
   void showInputCells(const LatticeCell& cellToTransform, const LatticeCell& referenceCell) const;

   // Display transformation results with complete centering
   void showCompleteTransformations(
      const CompleteTransformationResult& result,
      const LatticeCell& sourceCell,
      const LatticeCell& referenceCell) const;

};

#endif // TRANSFORMERDISPLAY_H

