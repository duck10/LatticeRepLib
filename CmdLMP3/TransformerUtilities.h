#pragma warning(disable: 4101) // Visual Studio 
#ifndef TRANSFORMER_UTILITIES_H
#define TRANSFORMER_UTILITIES_H

#include "LRL_Cell.h"
#include "LRL_Vector3.h"
#include "B4.h"
#include "P3.h"
#include "S6.h"
#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include "LatticeCell.h"
#include "LatticeConverter.h"
#include "G6.h"
#include "NCDist.h"

#include "Selling.h"
#include "MultiTransformFinderControls.h"
#include "LRL_StringTools.h"
#include "TransformerUtilities.h"

#include <vector>
#include <cmath>
#include <iostream>
#include <functional>
#include <set>


// Define M_PI if it's not already defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//// Operator to transform a cell using a matrix
//inline LRL_Cell operator*(const Matrix_3x3& matrix, const LRL_Cell& cell) {
//   // Convert cell to B4 representation
//   B4 b4Cell(cell);
//
//   // Apply transformation in B4 space
//   B4 transformedB4 = matrix * b4Cell;
//
//   // Convert back to LRL_Cell
//   return LRL_Cell(transformedB4);
//}


double CalculateNCDistWithReduction(const LRL_Cell& c1, const LRL_Cell& c2);


namespace TransformerUtilities {
   // Generic template function to sort any vector by a specified metric field
   template<typename T, typename FieldType>
   void SortVectorByField(
      std::vector<T>& items,
      std::function<FieldType(const T&)> getField) {

      std::sort(items.begin(), items.end(),
         [&getField](const T& a, const T& b) {
            return getField(a) < getField(b);
         });
   }


   // Keep these useful functions
   inline double getP3Distance(const LRL_Cell& c1, const LRL_Cell& c2) {
      const P3 p1(c1);
      const P3 p2(c2);
      return (p1 - p2).norm();
   }

   inline  double angleS6(const S6& s1, const S6& s2) {
      return acos(s1.Dot(s2) / s1.norm() / s2.norm()) * 180/4.0/atan(1.0);
   }


   // Simple matrix application helper
   inline LRL_Cell applyMatrixToCell(const LRL_Cell& cell, const Matrix_3x3& matrix) {
      const B4 b4Cell(cell);
      const B4 transformedB4 = matrix * b4Cell;
      return LRL_Cell(transformedB4);
   }

   inline double calculateP3Distance(const LRL_Cell& cell1, const LRL_Cell& cell2) {
      const P3 p3_1(cell1);
      const P3 p3_2(cell2);
      //std::cout << "DEBUG P3_1: " << p3_1 << std::endl;
      //std::cout << "DEBUG P3_2: " << p3_2 << std::endl;
      //std::cout << "DEBUG Difference: " << (p3_1 - p3_2) << std::endl;
      //std::cout << "DEBUG Distance: " << (p3_1 - p3_2).norm() << std::endl;
      return (p3_1 - p3_2).norm();
   }

   inline Matrix_3x3 calculateNiggliMatrix(const LRL_Cell& cell, bool showDetails = false) {
      G6 g6Cell = cell;
      G6 g6Reduced;
      MatG6 g6Matrix;
      Matrix_3x3 m3d;

      const bool success = Niggli::ReduceWithTransforms(g6Cell, g6Matrix, m3d, g6Reduced, 0.001);

      //{
      //   // verify that the 3D matrix is correct
      //   const LRL_Cell red(g6Reduced);
      //   const LRL_Cell trial = m3d * LRL_Cell(g6Reduced);
      //   const double P3diff = (P3(red) - P3(trial)).norm();
      //}

      if (!success) {
         std::cerr << "Warning: Niggli reduction failed" << std::endl;
         return Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
      }
      return m3d;
   }

   // Alternative: Return a struct with both
   struct PrimitiveConversionResult {
      LRL_Cell primitiveCell;
      Matrix_3x3 centeringTransform;
      bool success;
   };

   inline PrimitiveConversionResult convertToPrimitiveComplete(
      const LatticeCell& lattice);

   inline PrimitiveConversionResult convertToPrimitiveComplete(
      const LatticeCell& lattice,
      const MultiTransformFinderControls& controls);

   inline LRL_Cell convertToPrimitive(const LatticeCell& lattice,
      Matrix_3x3& centeringTransform,
      const MultiTransformFinderControls& controls);

} // namespace TransformerUtilities

// Simple debugging function - add outside any namespace
inline void compareLatticesSimple(const LRL_Cell& cell1, const LRL_Cell& cell2) {
   std::cout << "Cell 1: " << LRL_Cell_Degrees(cell1) << std::endl;
   std::cout << "Cell 2: " << LRL_Cell_Degrees(cell2) << std::endl;

   double p3Dist = P3::DistanceBetween(cell1, cell2);
   double ncDist = CalculateNCDistWithReduction(cell1, cell2);

   std::cout << "P3 Distance: " << p3Dist << std::endl;
   std::cout << "NC Distance: " << ncDist << std::endl;
}
// Centering matrices definitions
// None - Primitive(P)
static const Matrix_3x3 primitive{
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0
};
// Body - Centered(I)
static const Matrix_3x3 bodyCentered{
    0.5, 0.5, 0.5,
    -0.5, 0.5, 0.5,
    -0.5, -0.5, 0.5
};
// Face - Centered(F)
static const Matrix_3x3 faceCentered{
    0.5, 0.0, 0.5,
    0.5, 0.5, 0.0,
    0.0, 0.5, 0.5
};
// Base - Centered(A) - correctly places lattice points at the centers of the bc faces
static const Matrix_3x3 aCentered{
    1.0, 0.0, 0.0,
    0.0, 0.5, 0.5,
    0.0, -0.5, 0.5
};
// Base - Centered(B) - correctly places lattice points at the centers of the ac faces
static const Matrix_3x3 bCentered{
    0.5, 0.0, 0.5,
    0.0, 1.0, 0.0,
    -0.5, 0.0, 0.5
};
// Base - Centered(C) - correctly places lattice points at the centers of the ab faces
static const Matrix_3x3 cCentered{
    0.5, 0.5, 0.0,
    -0.5, 0.5, 0.0,
    0.0, 0.0, 1.0
};
// Rhombohedral(R)
// this is the matrix for coordinate transformation. It is
// the transpose of the matrix needed for base vectors.
static const Matrix_3x3 rhombohedral{
    2.0 / 3.0,  1.0 / 3.0,  1.0 / 3.0,
    -1.0 / 3.0, 1.0 / 3.0,  1.0 / 3.0,
    -1.0 / 3.0, -2.0 / 3.0, 1.0 / 3.0
};

// Fixed ToPrimitive function in TransformerUtilities
inline Matrix_3x3 ToPrimitive(const std::string& s, const LRL_Cell& cell) {
   const std::string upper = LRL_StringTools::strToupper(s);
   if (s.empty() || s == "P") return primitive;
   if (s == "I") return bodyCentered;
   if (s == "F") return faceCentered;
   if (s == "A") return aCentered;
   if (s == "B") return bCentered;
   if (s == "C") return cCentered;
   if (s == "H" || s == "R") {
      const double& alpha = cell[3];
      const double& beta = cell[4];
      const double& gamma = cell[5];

      static const double tolerance = 1.0E-4;

      // Check if all angles are equal (or nearly equal)
      bool anglesEqual = (std::abs(alpha - beta) < tolerance) &&
         (std::abs(beta - gamma) < tolerance) &&
         (std::abs(alpha - gamma) < tolerance);

      if (anglesEqual) {
         return primitive;     // H or R with equal angles = primitive
      }
      else {
         // Check if α = β = 90°, γ = 120°
         static const double degrees90 = M_PI / 2.0;
         static const double degrees120 = 2.0 * M_PI / 3.0;

         bool alphaIs90 = std::abs(alpha - degrees90) < tolerance;
         bool betaIs90 = std::abs(beta - degrees90) < tolerance;
         bool gammaIs120 = std::abs(gamma - degrees120) < tolerance;

         if (alphaIs90 && betaIs90 && gammaIs120) {
            if (s == "H") {
               return primitive;     // H with 90°,90°,120° = primitive hexagonal
            }
            else { // s == "R"
               return rhombohedral;  // R with 90°,90°,120° = triply-primitive rhombohedral
            }
         }
         else {
            return primitive;        // Fallback for malformed H/R
         }
      }
   }
   // unknown input or empty lattice
   return primitive;
}

// Function to display detailed transformation information
void showTransformationInfo(const LatticeCell& mobileCell, const LatticeCell& referenceCell);

// Function to show usage information
void showUsageInformation(const MultiTransformFinderControls& controls);

std::vector<LatticeCell> RunTests( MultiTransformFinderControls& controls, const int n);

inline LRL_Cell NiggliReduce(const LatticeCell& cell) {
   const G6 g6Primitive = LatticeConverter::MakePrimitiveCell(cell.getLatticeType(), cell.getCell());
   G6 reduced;;
   const bool success = Niggli::Reduce(g6Primitive, reduced);
   return reduced;
}


// Simple matrix application helper
inline LRL_Cell applyMatrixToCell(const LRL_Cell& cell, const Matrix_3x3& matrix) {
   const B4 b4Cell(cell);
   const B4 transformedB4 = matrix * b4Cell;
   return LRL_Cell(transformedB4);
}

inline double calculateP3Distance(const LRL_Cell& cell1, const LRL_Cell& cell2) {
   const P3 p3_1(cell1);
   const P3 p3_2(cell2);
   //std::cout << "DEBUG P3_1: " << p3_1 << std::endl;
   //std::cout << "DEBUG P3_2: " << p3_2 << std::endl;
   //std::cout << "DEBUG Difference: " << (p3_1 - p3_2) << std::endl;
   //std::cout << "DEBUG Distance: " << (p3_1 - p3_2).norm() << std::endl;
   return (p3_1 - p3_2).norm();
}


#endif // TRANSFORMER_UTILITIES_H
