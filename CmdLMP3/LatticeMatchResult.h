#ifndef LATTICEMATCHRESULT_H
#define LATTICEMATCHRESULT_H

#include "ProductionCommon.h"  // Includes common utilities and operator*
#include <iostream>

class LatticeMatchResult {
public:
   // Constructor
   LatticeMatchResult(const Matrix_3x3& transform = Matrix_3x3(),
      const double p3Distance = 19191.0,
      const double ncDistance = 19191.0,
      const LRL_Cell& transformedMobile = LRL_Cell(),
      const std::string& description = "")
      : m_transformationMatrix(transform)
      , m_p3Distance(p3Distance)
      , m_ncDistance(ncDistance)
      , m_transformedMobile(transformedMobile)
      , m_description(description)
   {
   }

   // Accessors
   const Matrix_3x3& getTransformationMatrix() const { return m_transformationMatrix; }
   double getP3Distance() const { return m_p3Distance; }
   double getNcDistance() const { return m_ncDistance; }
   const LRL_Cell& getTransformedMobile() const { return m_transformedMobile; }
   const std::string& getDescription() const { return m_description; }

   void setNCDistance(const double d) { m_ncDistance = d; }
   // Comparison for sorting (lower P3 distance is better)
   bool operator<(const LatticeMatchResult& other) const {
      return m_p3Distance < other.m_p3Distance;
   }

   // Equality check
   bool operator==(const LatticeMatchResult& other) const {
      return 
         m_transformationMatrix == other.m_transformationMatrix;
   }

   // Output operator
   friend std::ostream& operator<<(std::ostream& os, const LatticeMatchResult& result) {
      os << "; LatticeMatchResult =====================\n";
      os << "; P3 distance: " << result.m_p3Distance << "\n";
      os << "; NC distance: " << result.m_ncDistance << "\n";
      os << "; Description: " << result.m_description << "\n";
      os << "; Transformation matrix:\n" << result.m_transformationMatrix << "\n";
      os << "; Transformed mobile: " << LRL_Cell_Degrees(result.m_transformedMobile) << "\n";
      return os;
   }

private:
   Matrix_3x3 m_transformationMatrix;
   double m_p3Distance;
   double m_ncDistance;
   LRL_Cell m_transformedMobile;
   std::string m_description;
};

#endif // LATTICEMATCHRESULT_H

