#ifndef LATTICETRANSFORMER_H
#define LATTICETRANSFORMER_H

#include "LatticeCell.h"
#include "LRL_Cell.h"
#include "LRL_StringTools.h"
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
#include <sstream>

struct CompleteTransformationResult {
   // Existing members
   std::string inputCenteringType;
   std::string referenceCenteringType;
   bool hasCentering;

   Matrix_3x3 inputToPrimitiveMatrix;
   Matrix_3x3 referenceToPrimitiveMatrix;
   Matrix_3x3 inputNiggliMatrix;
   Matrix_3x3 referenceNiggliMatrix;
   Matrix_3x3 latticeMatchingMatrix;  // Add this for M_MR

   TransformationModuleResult primitiveResult;

   // Constructor
   CompleteTransformationResult()
      : hasCentering(false),
      latticeMatchingMatrix(1, 0, 0, 0, 1, 0, 0, 0, 1) {  // Initialize to identity
   }

   friend std::ostream& operator<< (std::ostream& o, const CompleteTransformationResult& c) {
      std::stringstream oss;
      oss << "CompleteTransformationResult\n";
      oss << "inputCenteringType  " << c.inputCenteringType << std::endl;
      oss << "referenceCenteringType  " << c.referenceCenteringType << std::endl;
      oss << "hasCentering      " << (c.hasCentering ? "true" : "false") << std::endl;
      oss << "referenceToPrimitiveMatrix\n" << c.referenceToPrimitiveMatrix << std::endl;
      oss << "referenceNiggliMatrix\n" << c.referenceNiggliMatrix << std::endl;
      oss << "latticeMatchingMatrix\n" << c.latticeMatchingMatrix << std::endl;  // Add this
      oss << "inputNiggliMatrix \n" << c.inputNiggliMatrix << std::endl;
      oss << "inputToPrimitiveMatrix\n" << c.inputToPrimitiveMatrix << std::endl;
      o << LRL_StringTools::indentLines(oss.str(), 3);
      return o;
   }
};



class LatticeTransformer {
private:
   const MultiTransformFinderControls& m_controls;

   // Helper methods for centering transformations
   Matrix_3x3 getCenteringToStandardMatrix(const LatticeCell& cell) const;
   Matrix_3x3 getStandardToCenteringMatrix(const std::string& centeringType, const LRL_Cell& cell) const;
   bool isCenteredLattice(const std::string& centeringType) const;

   // Helper to sort transformations based on complete transformation quality
   void sortTransformationsByCompleteQuality(
      std::vector<B4Matcher::TransformResult>& transformations) const;

public:
   LatticeTransformer(const MultiTransformFinderControls& controls);

   // Main transformation method
   CompleteTransformationResult transformLattice(
      const LatticeCell& cellToTransform,
      const LatticeCell& referenceCell);

   void debugConvergencePattern(
      const LatticeCell& cellToTransform,
      const LatticeCell& referenceCell,
      const CompleteTransformationResult& result,
      const Matrix_3x3& permutation) const;

};

#endif // LATTICETRANSFORMER_H