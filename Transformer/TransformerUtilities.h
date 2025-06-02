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
#include "TransformResult.h" // Include the shared TransformResult
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

// Operator to transform a cell using a matrix
inline LRL_Cell operator*(const Matrix_3x3& matrix, const LRL_Cell& cell) {
   // Convert cell to B4 representation
   B4 b4Cell(cell);

   // Apply transformation in B4 space
   B4 transformedB4 = matrix * b4Cell;

   // Convert back to LRL_Cell
   return LRL_Cell(transformedB4);
}

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

   // Calculate the Niggli reduction matrix for a cell
   inline Matrix_3x3 calculateNiggliMatrix(const LRL_Cell& cell, bool showDetails = false) {
      // Get the G6 representation of the cell
      G6 g6Cell = cell; // Direct conversion using assignment operator

      // Output variables for the reduction
      G6 g6Reduced;
      MatG6 g6Matrix;
      Matrix_3x3 m3d;

      // Perform Niggli reduction with transform tracking
      bool success = Niggli::ReduceWithTransforms(g6Cell, g6Matrix, m3d, g6Reduced, 0.001);
      const LRL_Cell_Degrees cellRed = LRL_Cell(g6Reduced);
      //std::cout << "cellRed in calculateNiggliMatrix " << cellRed << std::endl;

      if (!success) {
         std::cerr << "Warning: Niggli reduction failed for cell " << LRL_Cell_Degrees(cell) << std::endl;
         // Return identity matrix as fallback
         return Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
      }

      // The m3d matrix is the transformation matrix we need
      if (showDetails) {
         std::cout << "Niggli reduction matrix determinant: " << m3d.Det() << std::endl;
         std::cout << "Original cell: " << LRL_Cell_Degrees(cell) << std::endl;
         std::cout << "Reduced cell: " << LRL_Cell_Degrees(g6Reduced) << std::endl;

         // Verify the transformation
         LRL_Cell reducedCell = m3d * cell;
         std::cout << "Transformed cell: " << LRL_Cell_Degrees(reducedCell) << std::endl;
         std::cout << "Distance between reduced and transformed: "
            << LRL_Cell::DistanceBetween(reducedCell, LRL_Cell(g6Reduced)) << std::endl;
      }

      return m3d;
   }


   // Simple matrix application helper
   static LRL_Cell applyMatrixToCell(const LRL_Cell& cell, const Matrix_3x3& matrix) {
      const B4 b4Cell(cell);
      const B4 transformedB4 = matrix * b4Cell;
      return LRL_Cell(transformedB4);
   }

   static double calculateP3Distance(const LRL_Cell& cell1, const LRL_Cell& cell2) {
      const P3 p3_1(cell1);
      const P3 p3_2(cell2);
      return (p3_1 - p3_2).norm();
   }


} // namespace TransformerUtilities


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
static const Matrix_3x3 rhombohedral{
    2.0 / 3.0, -1.0 / 3.0, -1.0 / 3.0,
    1.0 / 3.0,  1.0 / 3.0, -2.0 / 3.0,
    1.0 / 3.0,  1.0 / 3.0,  1.0 / 3.0
};

// Function to convert from centered to primitive lattice
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
      if ((gamma - 120.0 / 180.0 * M_PI) > 1.0E-2) {
         return primitive;
      }
      else {
         return rhombohedral;
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



static LRL_Cell NiggliReduce(const LatticeCell& cell) {

   const G6 g6Primitive = LatticeConverter::MakePrimitiveCell(cell.getLatticeType(), cell.getCell());

   G6 reduced;;
   const bool success = Niggli::Reduce(g6Primitive, reduced);

   return reduced;
}

static double CalculateNCDistWithReduction(const LRL_Cell& c1, const LRL_Cell& c2) {
   G6 out1;
   G6 out2;
   Niggli::Reduce(c1, out1);
   Niggli::Reduce(c2, out2);
   return NCDist(out1.data(),out2.data());
}


// Simple matrix application helper
static LRL_Cell applyMatrixToCell(const LRL_Cell& cell, const Matrix_3x3& matrix) {
   const B4 b4Cell(cell);
   const B4 transformedB4 = matrix * b4Cell;
   return LRL_Cell(transformedB4);
}

static double calculateP3Distance(const LRL_Cell& cell1, const LRL_Cell& cell2) {
   const P3 p3_1(cell1);
   const P3 p3_2(cell2);
   return (p3_1 - p3_2).norm();
}


#endif // TRANSFORMER_UTILITIES_H