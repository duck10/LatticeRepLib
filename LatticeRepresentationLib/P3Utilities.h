#pragma warning (disable: 4189) // Visual Studio -- local variable is initialized but not referenced

#ifndef P3UTILITIES_H
#define P3UTILITIES_H

#include <map>
#include <vector>

#include "B4.h"
#include "LRL_Cell.h"
#include "TransformationMatrices.h"
#include "Polar.h"


static std::map<size_t, int> transformUsageCount;



static double ComputeMetricCost(const LRL_Cell& cell) {
   const B4 b4(cell);
   const double ab = std::abs(b4[0].Dot(b4[1]));
   const double ac = std::abs(b4[0].Dot(b4[2]));
   const double bc = std::abs(b4[1].Dot(b4[2]));
   return ab + ac + bc;
}


static std::vector<Matrix_3x3> GetReductionBasisWithNiggli() {
   const std::vector<Matrix_3x3> all = TransformationMatrices::generateUnimodularMatrices();
   const std::vector<size_t> highUseIndices = {
       1, 7, 28, 103, 106, 112, 118, 146, 147, 373, 379, 384, 526
   };
   const std::vector<size_t> niggliIndices = {
       2945, 1275, 549, 534, 573, 2906, 2944, 2946
   };

   std::vector<Matrix_3x3> basis;
   Matrix_3x3 m;
   m.UnitMatrix();
   basis.emplace_back(m);
   for (const size_t i : highUseIndices) basis.push_back(all[i]);
   for (const size_t i : niggliIndices) basis.push_back(all[i]);
   return basis;
}


static std::vector<Matrix_3x3> theBasicMatrices = GetReductionBasisWithNiggli();

// Use canonical base vectors for transformation
static std::vector<Vector_3> GetBaseVectors(const LRL_Cell& cell) {
   const B4 b4(cell);
   return { b4[0], b4[1], b4[2] };
}

static LRL_Cell GetCellFromBaseVectors(const std::vector<Vector_3>& vecs) {
   const Vector_3 zero(0.0, 0.0, 0.0);
   const double a = vecs[0].norm();
   const double b = vecs[1].norm();
   const double c = vecs[2].norm();
   const double alpha = Angle(vecs[1], vecs[2]);
   const double beta = Angle(vecs[0], vecs[2]);
   const double gamma = Angle(vecs[0], vecs[1]);

   return LRL_Cell(a, b, c, alpha, beta, gamma);
}



static LRL_Cell ApplyMatrixToCell(const LRL_Cell& cell, const Matrix_3x3& m) {
   const std::vector<Vector_3> base = GetBaseVectors(cell);
   std::vector<Vector_3> transformed;
   for (const Vector_3& v : base)
   {
      transformed.push_back(m * v);
   }
   return GetCellFromBaseVectors(transformed);
}


static std::vector<P3> GeneratePermutedP3s(const P3& input) {
   const std::array<P3::Vec2, 3> triplet = input.GetVector();
   const std::vector<std::vector<int>> permutations = {
      {0,1,2}, {0,2,1}, {1,0,2}, {1,2,0}, {2,0,1}, {2,1,0}
   };

   std::vector<P3> output;
   for (const auto& idx : permutations) {
      output.emplace_back(P3(
         Vector_2(triplet[idx[0]].first, triplet[idx[0]].second),
         Vector_2(triplet[idx[1]].first, triplet[idx[1]].second),
         Vector_2(triplet[idx[2]].first, triplet[idx[2]].second)));
   }
   return output;
}


static LRL_Cell ReduceCellScalar(const LRL_Cell& input) {
   LRL_Cell current = input;
   double currentCost = ComputeMetricCost(current);
   const double epsilon = 1e-6;
   bool improved = true;
   const std::vector<Matrix_3x3> matrices = theBasicMatrices;

   while (improved) {
      improved = false;
      LRL_Cell bestCell = current;
      double bestCost = currentCost;

      for (size_t i = 0; i < matrices.size(); ++i) {
         const LRL_Cell transformed = ApplyMatrixToCell(current, matrices[i]);
         const P3 projected(transformed);

         const std::vector<P3> permutations = GeneratePermutedP3s(projected);
         for (const P3& permuted : permutations) {
            const LRL_Cell candidate(permuted);
            const double candidateCost = ComputeMetricCost(candidate);

            if (candidateCost + epsilon < bestCost) {
               bestCost = candidateCost;
               bestCell = candidate;
               improved = true;
               transformUsageCount[i]++;
            }
         }
      }

      if (improved) {
         current = bestCell;
         currentCost = bestCost;
      }
   }

   return current;
}
// Angle-aware reconstruction using norm and angle geometry



//static LRL_Cell ApplyMatrixToCell(const LRL_Cell& cell, const Matrix_3x3& m) {
//   const std::vector<Vector_3> base = GetBaseVectors(cell);
//   std::vector<Vector_3> transformed;
//   for (const Vector_3& v : base) transformed.push_back(m * v);
//   return GetCellFromBaseVectors(transformed);
//}




inline LRL_Cell operator*(const Matrix_3x3& matrix, const LRL_Cell& cell) {
   // Convert cell to B4 representation
   B4 b4Cell(cell);

   // Apply transformation in B4 space
   B4 transformedB4 = matrix * b4Cell;

   // Convert back to LRL_Cell
   return LRL_Cell(transformedB4);
}

static P3 PerturbP3(const P3& p3, const double magnitude) {
   // Step 1: Generate a random unit vector that's orthogonal to p3
   const P3 ortho = P3().CreateUnitOrthogonalComponent(p3);

   // Step 2: Apply a small perturbation along that orthogonal direction
   const P3 perturbed = p3 + magnitude * ortho;
   const LRL_Cell cellInput(p3);
   const LRL_Cell cellPerturbed(perturbed);

   return perturbed;
}


//static std::pair<P3, P3> GenerateRandomPair(const double magnitude) {
//   const LRL_Cell baseCell = P3(Polar::rand());
//   P3 p3Base(baseCell);
//   p3Base = 100.0 * p3Base / p3Base.norm();
//   const P3 p3Test = P3(baseCell);
//   if (!p3Test.GetValid() || !baseCell.IsValid()) {
//      const int i19191 = 19191;
//   }
//   const P3 perturbedP3 = PerturbP3(P3(baseCell), magnitude);
//   const P3 diff = p3Test - perturbedP3;
//   const LRL_Cell cellPerturbed(perturbedP3);
//   return{ p3Test, perturbedP3 };
//}



#endif // !P3UTILITIES_H
