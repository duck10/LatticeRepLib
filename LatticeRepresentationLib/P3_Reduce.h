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
   static double P3Cost(const P3& p);
   static double Cost(const P3& p);

   static P3 CanonicalPresentation(const P3& input);

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
   static std::pair<LRL_Cell, Matrix_3x3> ReduceCellWith3480(const LRL_Cell& input);
   static bool VerifyTransformation(const LRL_Cell& input, const LRL_Cell& output, const Matrix_3x3& transform, bool verbose = true);

private:
   static bool VectorsEqual(const P3& a, const P3& b, double tolerance);
   static P3 Step1_MagnitudeOrdering(const P3& input);
   static P3 Step2_UpperHalfPlane(const P3& input);
   static P3 Step3_ReduceScalarProducts(const P3& input);
   static P3 ChooseBestScalarReduction(const P3& input, size_t i, size_t j);
   static P3 Step4_OptimizeAngles(const P3& input);
   static P3 Step5_LatticePreservingCombinations(const P3& input);



   static double P3ScalarAbsCost(const P3& p);
   static double P3ScalarCost(const P3& p);
   static double P3TraceCost(const P3& p);
   static double ComputeMetricCost(const LRL_Cell& cell);
};

#endif // P3_REDUCE_H
