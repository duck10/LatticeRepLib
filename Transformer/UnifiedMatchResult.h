#ifndef UNIFIEDMATCHRESULTS_H
#define UNIFIEDMATCHRESULTS_H

#include <string>
#include "LRL_Vector3.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"

#include <iomanip>


struct UnifiedMatchResult {
   bool success;
   std::string algorithmUsed;
   Matrix_3x3 transformationMatrix;
   LRL_Cell transformedMobileCell;
   double p3Distance;
   double ncDistance;
   int algorithmStep;
   int iterationsUsed;
   std::string notes;

   // Constructor with sentinel values
   UnifiedMatchResult()
      : success(false)
      , algorithmUsed("")
      , transformationMatrix()
      , transformedMobileCell()
      , p3Distance(19191.0)
      , ncDistance(19191.0)
      , algorithmStep(19191)
      , iterationsUsed(19191)
      , notes("") {
      transformationMatrix.UnitMatrix(); // Initialize to identity
   }

   friend std::ostream& operator<<(std::ostream& os, const UnifiedMatchResult& result) {
      os << "UnifiedMatchResult:\n"
         << "  Success: " << (result.success ? "YES" : "NO") << "\n"
         << "  Algorithm: " << result.algorithmUsed << "\n"
         << "  Algorithm Step: " << result.algorithmStep << "\n"
         << "  P3 Distance: " << std::fixed << std::setprecision(6) << result.p3Distance << "\n"
         << "  NC Distance: " << std::fixed << std::setprecision(6) << result.ncDistance << "\n"
         << "  Iterations: " << result.iterationsUsed << "\n";

      if (!result.notes.empty()) {
         os << "  Notes: " << result.notes << "\n";
      }

      if (result.success) {
         os << "  Transformed Mobile: " << LRL_Cell_Degrees(result.transformedMobileCell) << "\n"
            << "  Transformation Matrix:\n" << result.transformationMatrix;
      }

      return os;
   }
};

#endif // UNIFIEDMATCHRESULTS_H