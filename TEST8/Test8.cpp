// EisensteinReduction.h
#ifndef EISENSTEIN_REDUCTION_H
#define EISENSTEIN_REDUCTION_H

#include "LRL_Cell.h"
#include "LRL_Vector3.h"
#include "B4.h"
#include "G6.h"
#include "TransformationMatrices.h"

#include <unordered_map>
#include <vector>
#include <limits>
#include <map>
#include <unordered_set>

class EisensteinReduction {
public:
   struct ReductionResult {
      B4 reducedCell;           // The reduced cell (base vectors)
      G6 reducedG6;             // The reduced G6
      Matrix_3x3 transformation; // The transformation that produced it
      double trace;              // g1 + g2 + g3
      size_t duplicateCount;     // How many matrices gave same result

      ReductionResult() : trace(std::numeric_limits<double>::max()), duplicateCount(0) {}
   };

   // Main reduction function
   static ReductionResult Reduce(const LRL_Cell& inputCell, double tolerance = 1.0e-6);
   static ReductionResult Reduce(const B4& inputB4, double tolerance = 1.0e-6);
   static ReductionResult Reduce(const G6& inputG6, double tolerance = 1.0e-6);

private:
   // Helper to compute trace of metric tensor from base vectors
   static double ComputeTrace(const B4& cell);

   // Helper to check if two G6 are equivalent (within tolerance)
   static bool AreEquivalent(const G6& g1, const G6& g2, double tolerance);

   // Hash function for G6 to detect duplicates
   static size_t HashG6(const G6& g, double tolerance);

   // Apply transformation matrix to base vectors
   static B4 ApplyTransformation(const B4& input, const Matrix_3x3& transform);
};

// Implementation
inline double EisensteinReduction::ComputeTrace(const B4& cell) {
   // g1 = a·a, g2 = b·b, g3 = c·c
   const Vector_3& a = cell[0];
   const Vector_3& b = cell[1];
   const Vector_3& c = cell[2];

   return a.Norm() + b.Norm() + c.Norm();
}

inline bool EisensteinReduction::AreEquivalent(const G6& g1, const G6& g2, double tolerance) {
   for (size_t i = 0; i < 6; ++i) {
      if (std::abs(g1[i] - g2[i]) > tolerance) {
         return false;
      }
   }
   return true;
}

inline size_t EisensteinReduction::HashG6(const G6& g, double tolerance) {
   // Create a hash based on rounded values
   size_t hash = 0;
   const size_t prime = 31;

   for (size_t i = 0; i < 6; ++i) {
      // Round to tolerance grid
      long long rounded = static_cast<long long>(g[i] / tolerance + 0.5);
      hash = hash * prime + std::hash<long long>{}(rounded);
   }

   return hash;
}

inline B4 EisensteinReduction::ApplyTransformation(const B4& input, const Matrix_3x3& transform) {
   // Transform the base vectors
   // New basis = old basis * transformation matrix
   // Each new vector is a linear combination of old vectors

   const Vector_3& a = input[0];
   const Vector_3& b = input[1];
   const Vector_3& c = input[2];

   B4 result;

   // New a' = m00*a + m10*b + m20*c
   result[0] = transform[0] * a + transform[3] * b + transform[6] * c;

   // New b' = m01*a + m11*b + m21*c
   result[1] = transform[1] * a + transform[4] * b + transform[7] * c;

   // New c' = m02*a + m12*b + m22*c
   result[2] = transform[2] * a + transform[5] * b + transform[8] * c;

   return result;
}

inline EisensteinReduction::ReductionResult
EisensteinReduction::Reduce(const LRL_Cell& inputCell, double tolerance) {
   // Convert to B4
   B4 inputB4(inputCell);
   return Reduce(inputB4, tolerance);
}

inline EisensteinReduction::ReductionResult
EisensteinReduction::Reduce(const G6& inputG6, double tolerance) {
   // Convert G6 to Cell to B4
   LRL_Cell cell(inputG6);
   B4 inputB4(cell);
   return Reduce(inputB4, tolerance);
}

inline EisensteinReduction::ReductionResult
EisensteinReduction::Reduce(const B4& inputB4, double tolerance) {
   ReductionResult bestResult;

   // Track duplicates using G6 representations
   std::unordered_map<size_t, std::vector<Matrix_3x3>> duplicateMap;

   // Generate all 3480 unimodular matrices with elements {-1, 0, 1}
   std::vector<Matrix_3x3> matrices = TransformationMatrices::generateUnimodularMatrices();

   std::cout << "Eisenstein reduction: Testing " << matrices.size()
      << " unimodular matrices..." << std::endl;

   // Apply each transformation and find minimum trace
   for (const auto& transform : matrices) {
      // Apply transformation to base vectors
      B4 transformedCell = ApplyTransformation(inputB4, transform);

      // Compute trace of metric tensor
      double trace = ComputeTrace(transformedCell);

      // Convert to G6 for comparison
      LRL_Cell tempCell(transformedCell);
      G6 transformedG6(tempCell);

      // Hash for duplicate detection
      size_t hash = HashG6(transformedG6, tolerance);
      duplicateMap[hash].push_back(transform);

      // Keep track of best (minimum trace)
      if (trace < bestResult.trace - tolerance) {
         bestResult.reducedCell = transformedCell;
         bestResult.reducedG6 = transformedG6;
         bestResult.transformation = transform;
         bestResult.trace = trace;
      }
   }

   // Count duplicates for the best result
   size_t bestHash = HashG6(bestResult.reducedG6, tolerance);
   bestResult.duplicateCount = duplicateMap[bestHash].size();

   // Report statistics
   std::cout << "Eisenstein reduction complete:" << std::endl;
   std::cout << "  Minimum trace: " << bestResult.trace << std::endl;
   std::cout << "  Unique G6 cells found: " << duplicateMap.size() << std::endl;
   std::cout << "  Transformations giving best result: " << bestResult.duplicateCount << std::endl;

   return bestResult;
}

int main() {}

#endif // EISENSTEIN_REDUCTION_H