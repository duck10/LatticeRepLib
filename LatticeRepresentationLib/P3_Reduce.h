#ifndef P3_REDUCE_H
#define P3_REDUCE_H

#include "P3.h"
#include "LRL_Cell.h"
#include "LRL_Vector3.h"

struct ReducedResult {
   P3 reduced;
   Matrix_3x3 transformation;
};


class P3_Reduce {
public:
   static P3 Reduce(const P3& input);
   static P3 ReduceScalarCost(const P3& input);
   static double ScalarCost(const P3& p);

   // Optional: expose cost calculation
   static double Cost(const P3& p);
   static P3 CanonicalPresentation(const P3& input);
   static ReducedResult MatrixAwareReduce(const P3& input);

public:
   struct Scalar {
      static double Projection(const std::pair<double, double>& pi,
         const std::pair<double, double>& pj);
      static double Dot(const std::pair<double, double>& a,
         const std::pair<double, double>& b);
      static double MagnitudeSquared(const std::pair<double, double>& v);
   };

   static P3 EnforceUpperHalfPlane(const P3& p);

   static bool CosineSignsUniform(const P3& p);
   static P3 StandardPresentation(const P3& input);
};

#endif // P3_REDUCE_H
