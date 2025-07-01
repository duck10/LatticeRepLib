#pragma warning(disable: 4101) // Visual Studio warning suppression
#ifndef PRODUCTIONCOMMON_H
#define PRODUCTIONCOMMON_H

// Common includes for the production lattice matcher
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "Matrix_3x3.h"
#include "B4.h"
#include "P3.h"
#include "G6.h"
#include "S6.h"
#include "Niggli.h"
#include "NCDist.h"

// Define M_PI if not already defined (Visual Studio compatibility)
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Include the critical operator first
#include "MatrixCellOperator.h"

// Include your existing utilities - but be careful about operator conflicts
// If TransformerUtilities.h also defines operator*, you may need to comment out
// one of the definitions to avoid redefinition errors

// For now, let's use the forward declarations approach
// #include "TransformerUtilities.h"

// Forward declare the functions we need from TransformerUtilities
namespace TransformerUtilities {
   double getP3Distance(const LRL_Cell& c1, const LRL_Cell& c2);
}

// Forward declare the global function
double CalculateNCDistWithReduction(const LRL_Cell& c1, const LRL_Cell& c2);

// Additional utility functions that don't conflict with existing ones
namespace ProductionUtilities {

   // Check if a matrix has determinant +1 (proper transformation)
   inline bool isProperTransformation(const Matrix_3x3& matrix) {
      return std::abs(matrix.Det() - 1.0) < 1e-10;
   }

   // Simple wrapper for the existing functions to avoid namespace issues
   inline double getP3Distance(const LRL_Cell& cell1, const LRL_Cell& cell2) {
      return TransformerUtilities::getP3Distance(cell1, cell2);
   }

   inline double getNCDistance(const LRL_Cell& cell1, const LRL_Cell& cell2) {
      return CalculateNCDistWithReduction(cell1, cell2);
   }

   // Simple cell comparison for debugging
   inline void compareCells(const LRL_Cell& cell1, const LRL_Cell& cell2,
      const std::string& description = "") {
      if (!description.empty()) {
         std::cout << "; " << description << std::endl;
      }
      std::cout << "; Cell 1: " << LRL_Cell_Degrees(cell1) << std::endl;
      std::cout << "; Cell 2: " << LRL_Cell_Degrees(cell2) << std::endl;
      std::cout << "; P3 Distance: " << getP3Distance(cell1, cell2) << std::endl;
      std::cout << "; NC Distance: " << getNCDistance(cell1, cell2) << std::endl;
   }
}

#endif // PRODUCTIONCOMMON_H

