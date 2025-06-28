#ifndef PRIMITIVECONVERTER_H
#define PRIMITIVECONVERTER_H

#include "ProductionCommon.h"  // Includes operator* and common utilities
#include "LatticeCell.h"
#include <string>
#include <iostream>

class PrimitiveConverter {
public:
   // Structure to hold conversion results
   struct ConversionResult {
      LRL_Cell primitiveCell;
      Matrix_3x3 toPrimitiveMatrix;
      Matrix_3x3 fromPrimitiveMatrix;
      std::string originalLatticeType;

      ConversionResult()
         : primitiveCell()
         , toPrimitiveMatrix()
         , fromPrimitiveMatrix()
         , originalLatticeType("")
      {
      }

      ConversionResult(const LRL_Cell& primitive,
         const Matrix_3x3& toPrimitive,
         const Matrix_3x3& fromPrimitive,
         const std::string& latticeType)
         : primitiveCell(primitive)
         , toPrimitiveMatrix(toPrimitive)
         , fromPrimitiveMatrix(fromPrimitive)
         , originalLatticeType(latticeType)
      {
      }

      friend std::ostream& operator<<(std::ostream& os, const ConversionResult& result) {
         os << "; PrimitiveConverter::ConversionResult\n";
         os << "; Original lattice type: " << result.originalLatticeType << "\n";
         os << "; Primitive cell: " << LRL_Cell_Degrees(result.primitiveCell) << "\n";
         os << "; To primitive matrix:\n" << result.toPrimitiveMatrix;
         os << "; From primitive matrix:\n" << result.fromPrimitiveMatrix;
         return os;
      }
   };

   // Convert a centered cell to primitive
   static ConversionResult toPrimitive(const LatticeCell& centeredCell);

   // Convert a primitive cell back to centered using the conversion result
   static LRL_Cell fromPrimitive(const LRL_Cell& primitiveCell,
      const ConversionResult& conversionInfo);

   // Apply the decentering transformation to a transformation matrix
   static Matrix_3x3 applyDecenteringToMatrix(const Matrix_3x3& primitiveTransform,
      const ConversionResult& referenceConversion,
      const ConversionResult& mobileConversion);

private:
   // Get the transformation matrix for a specific lattice centering type
   static Matrix_3x3 getCenteringMatrix(const std::string& latticeType);

   // Get the inverse transformation matrix
   static Matrix_3x3 getDecenteringMatrix(const std::string& latticeType);
};

// Implementation
inline PrimitiveConverter::ConversionResult
PrimitiveConverter::toPrimitive(const LatticeCell& centeredCell) {
   const std::string latticeType = centeredCell.getLatticeType();
   const LRL_Cell originalCell = centeredCell.getCell();

   // Get the centering transformation matrix
   const Matrix_3x3 toPrimitiveMatrix = getCenteringMatrix(latticeType);
   const Matrix_3x3 fromPrimitiveMatrix = getDecenteringMatrix(latticeType);

   // Apply the transformation to get primitive cell
   // Use the operator* from TransformerUtilities.h
   const LRL_Cell primitiveCell = toPrimitiveMatrix * originalCell;

   return ConversionResult(primitiveCell, toPrimitiveMatrix,
      fromPrimitiveMatrix, latticeType);
}

inline LRL_Cell
PrimitiveConverter::fromPrimitive(const LRL_Cell& primitiveCell,
   const ConversionResult& conversionInfo) {
   return conversionInfo.fromPrimitiveMatrix * primitiveCell;
}

inline Matrix_3x3
PrimitiveConverter::applyDecenteringToMatrix(const Matrix_3x3& primitiveTransform,
   const ConversionResult& referenceConversion,
   const ConversionResult& mobileConversion) {
   // Transform from centered mobile to centered reference:
   // centeredRef = fromPrimRef * primTransform * toPrimMobile * centeredMobile
   return referenceConversion.fromPrimitiveMatrix *
      primitiveTransform *
      mobileConversion.toPrimitiveMatrix;
}

inline Matrix_3x3 PrimitiveConverter::getCenteringMatrix(const std::string& latticeType) {
   // Standard crystallographic centering transformations
   if (latticeType == "P") {
      return Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);  // Identity for primitive
   }
   else if (latticeType == "I") {
      // Body-centered
      return Matrix_3x3(-0.5, 0.5, 0.5, 0.5, -0.5, 0.5, 0.5, 0.5, -0.5);
   }
   else if (latticeType == "F") {
      // Face-centered
      return Matrix_3x3(0, 0.5, 0.5, 0.5, 0, 0.5, 0.5, 0.5, 0);
   }
   else if (latticeType == "C") {
      // C-centered
      return Matrix_3x3(0.5, -0.5, 0, 0.5, 0.5, 0, 0, 0, 1);
   }
   else if (latticeType == "A") {
      // A-centered
      return Matrix_3x3(1, 0, 0, 0, 0.5, -0.5, 0, 0.5, 0.5);
   }
   else if (latticeType == "B") {
      // B-centered
      return Matrix_3x3(0.5, 0, -0.5, 0, 1, 0, 0.5, 0, 0.5);
   }
   else {
      // Default to primitive if unknown
      return Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
   }
}

inline Matrix_3x3 PrimitiveConverter::getDecenteringMatrix(const std::string& latticeType) {
   // These are the inverse matrices of the centering transformations
   if (latticeType == "P") {
      return Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);  // Identity for primitive
   }
   else if (latticeType == "I") {
      // Body-centered inverse
      return Matrix_3x3(0, 1, 1, 1, 0, 1, 1, 1, 0);
   }
   else if (latticeType == "F") {
      // Face-centered inverse
      return Matrix_3x3(-1, 1, 1, 1, -1, 1, 1, 1, -1);
   }
   else if (latticeType == "C") {
      // C-centered inverse
      return Matrix_3x3(1, 1, 0, -1, 1, 0, 0, 0, 1);
   }
   else if (latticeType == "A") {
      // A-centered inverse
      return Matrix_3x3(1, 0, 0, 0, 1, 1, 0, -1, 1);
   }
   else if (latticeType == "B") {
      // B-centered inverse
      return Matrix_3x3(1, 0, 1, 0, 1, 0, -1, 0, 1);
   }
   else {
      // Default to primitive if unknown
      return Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
   }
}

#endif // PRIMITIVECONVERTER_H
