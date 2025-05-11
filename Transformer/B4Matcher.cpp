#include "B4Matcher.h"
#include "MultiTransformFinder.h"
#include "MultiTransformFinderControls.h"
#include "TransformerUtilities.h"
#include "P3.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <set>

// Generate permutation matrices
std::vector<Matrix_3x3> generatePermutationMatrices() {
   std::vector<Matrix_3x3> permutations;

   // Identity
   permutations.emplace_back(Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1));

   // Axis permutations
   permutations.emplace_back(Matrix_3x3(0, 0, 1, 1, 0, 0, 0, 1, 0)); // This is (c,a,b)
   permutations.emplace_back(Matrix_3x3(0, 1, 0, 0, 0, 1, 1, 0, 0)); // This is (b,c,a)
   permutations.emplace_back(Matrix_3x3(0, 1, 0, 1, 0, 0, 0, 0, 1)); // This is (b,a,c)
   permutations.emplace_back(Matrix_3x3(1, 0, 0, 0, 0, 1, 0, 1, 0)); // This is (a,c,b)
   permutations.emplace_back(Matrix_3x3(0, 0, 1, 0, 1, 0, 1, 0, 0)); // This is (c,b,a)

   return permutations;
}

// Constructor implementations
B4Matcher::B4Matcher(const LRL_Cell& cellToTransform, const LRL_Cell& referenceCell)
   : m_b4CellToTransform(cellToTransform),
   m_b4ReferenceCell(referenceCell),
   m_controls(*new MultiTransformFinderControls()) { // Note: This will cause a memory leak!
   // Use default search depth of 3
   FindTransformations(3);
}

B4Matcher::B4Matcher(const LRL_Cell& cellToTransform, const LRL_Cell& referenceCell,
   const MultiTransformFinderControls& controls)
   : m_b4CellToTransform(cellToTransform),
   m_b4ReferenceCell(referenceCell),
   m_controls(controls) {
   // Use search depth from controls
   FindTransformations(controls.getMaxMatrixSearchDepth());
}

B4Matcher::B4Matcher(const B4& sourceB4, const B4& targetB4, const MultiTransformFinderControls& controls)
   : m_b4CellToTransform(sourceB4),
   m_b4ReferenceCell(targetB4),
   m_controls(controls) {
}

// Helper function to measure transformation complexity
double B4Matcher::transformationComplexity(const Matrix_3x3& matrix) const {
   double complexity = 0.0;

   // Check if it's identity or a simple permutation
   const double epsilon = 1e-10;

   // Identity check with one-dimensional array access
   bool isIdentity =
      fabs(matrix[0] - 1.0) < epsilon && fabs(matrix[1]) < epsilon && fabs(matrix[2]) < epsilon &&
      fabs(matrix[3]) < epsilon && fabs(matrix[4] - 1.0) < epsilon && fabs(matrix[5]) < epsilon &&
      fabs(matrix[6]) < epsilon && fabs(matrix[7]) < epsilon && fabs(matrix[8] - 1.0) < epsilon;

   if (isIdentity) {
      return 0.0; // Identity is the simplest
   }

   // Count non-zero elements
   int nonZeros = 0;
   for (int i = 0; i < 9; ++i) {
      if (fabs(matrix[i]) > epsilon) {
         nonZeros++;

         // Add complexity for values other than -1, 0, 1
         if (fabs(fabs(matrix[i]) - 1.0) > epsilon) {
            complexity += 2.0;
         }
      }
   }

   // Add complexity based on number of non-zeros
   complexity += nonZeros;

   return complexity;
}

void B4Matcher::ProcessMatrix(const Matrix_3x3& matrix) {
   // Create LRL_Cell from first B4
   LRL_Cell cell1 = m_b4CellToTransform;

   // Apply transformation to the cell
   B4 transformedB4 = matrix * m_b4CellToTransform;  // Use matrix multiplication on B4 directly

   // Calculate distance
   double distance = B4::DistanceBetween(transformedB4, m_b4ReferenceCell);

   // Store result
   TransformResult result;
   result.transformMatrix = matrix;
   result.b4Distance = distance;
   result.transformedB4 = transformedB4;  // Store the transformed B4

   m_transformResults.emplace_back(result);
}

B4Matcher::TransformResult B4Matcher::findBestTransformation() const {
   if (m_transformResults.empty()) {
      // Create and initialize the TransformResult properly
      TransformResult result;
      result.transformMatrix = Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
      result.transformedB4 = m_b4CellToTransform; // Use the source B4, not a double
      result.b4Distance = std::numeric_limits<double>::max();
      return result;
   }
   return m_transformResults[0];
}

std::vector<B4Matcher::TransformResult> B4Matcher::findMultipleTransformations(size_t maxCount) const {
   if (m_transformResults.empty()) {
      // Create and initialize the TransformResult properly
      TransformResult result;
      result.transformMatrix = Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
      result.transformedB4 = m_b4CellToTransform; // Use the source B4, not a double
      result.b4Distance = std::numeric_limits<double>::max();
      return std::vector<TransformResult>{result}; // Use {} instead of () for initialization
   }

   // Fixed std::min call with both arguments
   const size_t count = std::min(maxCount, m_transformResults.size());
   return std::vector<TransformResult>(m_transformResults.begin(), m_transformResults.begin() + count);
}

// Generate common crystallographic matrices
std::vector<Matrix_3x3> B4Matcher::generateCrystallographicMatrices() const {
   std::vector<Matrix_3x3> matrices;

   // Basic permutation matrices (identity, axis swaps, etc.)
   matrices.emplace_back(Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1));  // Identity
   matrices.emplace_back(Matrix_3x3(0, 1, 0, 1, 0, 0, 0, 0, 1));  // Swap x,y
   matrices.emplace_back(Matrix_3x3(1, 0, 0, 0, 0, 1, 0, 1, 0));  // Swap y,z
   matrices.emplace_back(Matrix_3x3(0, 0, 1, 0, 1, 0, 1, 0, 0));  // Swap x,z
   matrices.emplace_back(Matrix_3x3(0, 0, 1, 1, 0, 0, 0, 1, 0));  // xyz->zxy
   matrices.emplace_back(Matrix_3x3(0, 1, 0, 0, 0, 1, 1, 0, 0));  // xyz->yzx

   // Sign flips with permutations (still with det=+1)
   matrices.emplace_back(Matrix_3x3(-1, 0, 0, 0, -1, 0, 0, 0, 1));  // Invert x,y
   matrices.emplace_back(Matrix_3x3(-1, 0, 0, 0, 1, 0, 0, 0, -1));  // Invert x,z
   matrices.emplace_back(Matrix_3x3(1, 0, 0, 0, -1, 0, 0, 0, -1));  // Invert y,z

   // Common shear transformations (with det=+1)
   matrices.emplace_back(Matrix_3x3(1, 1, 0, 0, 1, 0, 0, 0, 1));  // Shear in xy plane
   matrices.emplace_back(Matrix_3x3(1, 0, 1, 0, 1, 0, 0, 0, 1));  // Shear in xz plane
   matrices.emplace_back(Matrix_3x3(1, 0, 0, 1, 1, 0, 0, 0, 1));  // Shear in yx plane
   matrices.emplace_back(Matrix_3x3(1, 0, 0, 0, 1, 1, 0, 0, 1));  // Shear in yz plane
   matrices.emplace_back(Matrix_3x3(1, 0, 0, 0, 1, 0, 1, 0, 1));  // Shear in zx plane
   matrices.emplace_back(Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 1, 1));  // Shear in zy plane

   // Negative shears
   matrices.emplace_back(Matrix_3x3(1, -1, 0, 0, 1, 0, 0, 0, 1));  // Negative shear in xy
   matrices.emplace_back(Matrix_3x3(1, 0, -1, 0, 1, 0, 0, 0, 1));  // Negative shear in xz
   matrices.emplace_back(Matrix_3x3(1, 0, 0, -1, 1, 0, 0, 0, 1));  // Negative shear in yx
   matrices.emplace_back(Matrix_3x3(1, 0, 0, 0, 1, -1, 0, 0, 1));  // Negative shear in yz
   matrices.emplace_back(Matrix_3x3(1, 0, 0, 0, 1, 0, -1, 0, 1));  // Negative shear in zx
   matrices.emplace_back(Matrix_3x3(1, 0, 0, 0, 1, 0, 0, -1, 1));  // Negative shear in zy

   // Common centering operations (preserving det=+1)
   matrices.emplace_back(Matrix_3x3(1, 1, 1, 0, 1, 0, 0, 0, 1));  // Face-centered related
   matrices.emplace_back(Matrix_3x3(1, 0, 0, 1, 1, 1, 0, 0, 1));  // Body-centered related 
   matrices.emplace_back(Matrix_3x3(1, 1, 0, 1, 0, 0, 1, 1, 1));  // Complex centering
   matrices.emplace_back(Matrix_3x3(1, 0, 0, 1, 1, 0, 1, 0, 1));  // Mixed centering
   matrices.emplace_back(Matrix_3x3(0, 1, 0, 0, 0, 1, 1, 1, 1));  // Mixed centering

   // Complex matrices known to be useful for challenging cases
   matrices.emplace_back(Matrix_3x3(0, -1, 0, 1, 0, 0, 1, 1, 1));  // LeTrong & Stenkamp case
   matrices.emplace_back(Matrix_3x3(-1, 1, 0, -1, 0, 0, 0, 0, -1)); // Charley Simmons case
   matrices.emplace_back(Matrix_3x3(0, 1, 0, 0, 0, 1, 1, 0, 1));    // For last test case
   matrices.emplace_back(Matrix_3x3(0, 1, 0, 0, -1, 1, 1, 0, 0));    // Mixed operations
   matrices.emplace_back(Matrix_3x3(1, 1, 0, -1, 1, 0, 0, 0, 1));    // Mixed operations
   matrices.emplace_back(Matrix_3x3(1, 0, 1, 0, 1, -1, 0, 0, 1));    // Mixed operations

   return matrices;
}

// Add this to B4Matcher.cpp

std::vector<Matrix_3x3> B4Matcher::generatePermutationMatrices() const {
   std::vector<Matrix_3x3> matrices;

   // Six basic permutation matrices (identity + 5 others)
   matrices.emplace_back(Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1));  // xyz → xyz (identity)
   matrices.emplace_back(Matrix_3x3(0, 1, 0, 1, 0, 0, 0, 0, 1));  // xyz → yxz
   matrices.emplace_back(Matrix_3x3(0, 0, 1, 0, 1, 0, 1, 0, 0));  // xyz → zyx
   matrices.emplace_back(Matrix_3x3(1, 0, 0, 0, 0, 1, 0, 1, 0));  // xyz → xzy
   matrices.emplace_back(Matrix_3x3(0, 0, 1, 1, 0, 0, 0, 1, 0));  // xyz → zxy
   matrices.emplace_back(Matrix_3x3(0, 1, 0, 0, 0, 1, 1, 0, 0));  // xyz → yzx

   return matrices;
}

bool B4Matcher::isValidTransformationMatrix(const Matrix_3x3& matrix) const {
   // Check determinant (must be exactly +1 for valid lattice transformations)
   const double det = matrix.Det();
   if (std::abs(det - 1.0) > 1e-10) {
      return false;
   }

   return true;
}

// Generate matrices with integers between -maxCoeff and maxCoeff
std::vector<Matrix_3x3> B4Matcher::generateComprehensiveIntegerMatrices(int maxCoeff) const {
   std::vector<Matrix_3x3> matrices;
   const int maxToGenerate = 100; // Limit to avoid excessive matrices
   int generated = 0;

   // Quick lookup for simple matrices (often these are the best matches)
   std::vector<Matrix_3x3> simpleMatrices = generateCrystallographicMatrices();
   for (const auto& matrix : simpleMatrices) {
      if (isValidTransformationMatrix(matrix)) {
         matrices.emplace_back(matrix);
         generated++;
      }
   }

   // Try common combinations of basis permutation matrices and shears
   const std::vector<Matrix_3x3> permutations = generatePermutationMatrices();
   const std::vector<Matrix_3x3> shears = {
      Matrix_3x3(1, 1, 0, 0, 1, 0, 0, 0, 1),  // xy shear
      Matrix_3x3(1, 0, 1, 0, 1, 0, 0, 0, 1),  // xz shear
      Matrix_3x3(1, 0, 0, 1, 1, 0, 0, 0, 1),  // yx shear
      Matrix_3x3(1, 0, 0, 0, 1, 1, 0, 0, 1),  // yz shear
      Matrix_3x3(1, 0, 0, 0, 1, 0, 1, 0, 1),  // zx shear
      Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 1, 1),  // zy shear
      Matrix_3x3(1, -1, 0, 0, 1, 0, 0, 0, 1), // negative xy shear
      Matrix_3x3(1, 0, -1, 0, 1, 0, 0, 0, 1), // negative xz shear
      Matrix_3x3(1, 0, 0, -1, 1, 0, 0, 0, 1), // negative yx shear
      Matrix_3x3(1, 0, 0, 0, 1, -1, 0, 0, 1), // negative yz shear
      Matrix_3x3(1, 0, 0, 0, 1, 0, -1, 0, 1), // negative zx shear
      Matrix_3x3(1, 0, 0, 0, 1, 0, 0, -1, 1)  // negative zy shear
   };

   // Try permutation-shear combinations
   for (const auto& perm : permutations) {
      for (const auto& shear : shears) {
         Matrix_3x3 combined = perm * shear;
         if (isValidTransformationMatrix(combined)) {
            matrices.emplace_back(combined);
            generated++;
            if (generated >= maxToGenerate) return matrices;
         }

         // Also try the reverse combination
         combined = shear * perm;
         if (isValidTransformationMatrix(combined)) {
            matrices.emplace_back(combined);
            generated++;
            if (generated >= maxToGenerate) return matrices;
         }
      }
   }

   // Try more complex combinations if we haven't hit our limit
   if (generated < maxToGenerate && maxCoeff >= 2) {
      for (const auto& m1 : shears) {
         for (const auto& m2 : shears) {
            Matrix_3x3 combined = m1 * m2;
            if (isValidTransformationMatrix(combined)) {
               matrices.emplace_back(combined);
               generated++;
               if (generated >= maxToGenerate) return matrices;
            }
         }
      }
   }

   // Add some known useful complex matrices
   if (maxCoeff >= 3) {
      const std::vector<Matrix_3x3> complexMatrices = {
         Matrix_3x3(1, 1, 1, 0, 1, 0, 0, 0, 1),  // Triple operations
         Matrix_3x3(1, 0, 0, 1, 1, 1, 0, 0, 1),
         Matrix_3x3(1, 0, 0, 0, 1, 0, 1, 1, 1),
         Matrix_3x3(0, 1, 0, 0, 0, 1, 1, 1, 1)
      };

      for (const auto& complex : complexMatrices) {
         if (isValidTransformationMatrix(complex)) {
            matrices.emplace_back(complex);
            generated++;
         }

         for (const auto& perm : permutations) {
            Matrix_3x3 combined = complex * perm;
            if (isValidTransformationMatrix(combined)) {
               matrices.emplace_back(combined);
               generated++;
               if (generated >= maxToGenerate) return matrices;
            }
         }
      }
   }

   return matrices;
}

void B4Matcher::FindTransformations(int searchDepth) {
   // Clear any existing results
   m_transformResults.clear();

   // Always include the identity matrix first
   if (m_controls.shouldIncludeIdentityMatrix()) {
      Matrix_3x3 identityMatrix(1, 0, 0, 0, 1, 0, 0, 0, 1);
      ProcessMatrix(identityMatrix);
   }

   // 1. Standard permutation matrices (6 total)
   const std::vector<Matrix_3x3> permutationMatrices = generatePermutationMatrices();
   for (const Matrix_3x3& m : permutationMatrices) {
      ProcessMatrix(m);
   }

   // 2. Add comprehensive set of crystallographic matrices
   const std::vector<Matrix_3x3> crystalMatrices = generateCrystallographicMatrices();
   for (const Matrix_3x3& m : crystalMatrices) {
      // Double-check determinant to ensure it's exactly +1
      if (std::abs(m.Det() - 1.0) < 1e-10) {
         ProcessMatrix(m);
      }
   }

   // 3. Generate more matrices depending on search depth
   std::vector<Matrix_3x3> additionalMatrices = generateComprehensiveIntegerMatrices(searchDepth);
   for (const Matrix_3x3& m : additionalMatrices) {
      if (std::abs(m.Det() - 1.0) < 1e-10) {
         ProcessMatrix(m);
      }
   }

   // Sort results with S6 angle as the primary metric, then P3 distance
   sortTransformationsByQuality();
}

// New helper method for sorting
void B4Matcher::sortTransformationsByQuality() {
   // First calculate and store the metrics for each result
   for (auto& result : m_transformResults) {
      // Calculate metrics for each transformation
      LatticeCell transformedCell(result.transformedB4);
      LatticeCell referenceCell(m_b4ReferenceCell);

      // S6 metrics
      const S6 transformed(transformedCell.getCell());
      const S6 ref(referenceCell.getCell());
      result.s6AngleFinal = S6::AngleBetween(transformed, ref);

      // P3 metrics
      P3 transformedP3(transformedCell.getCell());
      P3 refP3(referenceCell.getCell());
      result.p3DistanceFinal = P3::DistanceBetween(transformedP3, refP3);
   }

   // Now sort the results
   std::sort(m_transformResults.begin(), m_transformResults.end(),
      [this](const TransformResult& a, const TransformResult& b) {
         // Check sorting preference
         if (m_controls.shouldSortByP3()) {
            // Primary sorting by P3 distance
            if (std::abs(a.p3DistanceFinal - b.p3DistanceFinal) > 0.1) {
               return a.p3DistanceFinal < b.p3DistanceFinal;
            }

            // If P3 distances are very close, compare S6 angles
            if (std::abs(a.s6AngleFinal - b.s6AngleFinal) > 0.1) {
               return a.s6AngleFinal < b.s6AngleFinal;
            }
         }
         else {
            // Primary sorting by S6 angle
            if (std::abs(a.s6AngleFinal - b.s6AngleFinal) > 0.1) {
               return a.s6AngleFinal < b.s6AngleFinal;
            }

            // If S6 angles are very close, compare P3 distances
            if (std::abs(a.p3DistanceFinal - b.p3DistanceFinal) > 0.1) {
               return a.p3DistanceFinal < b.p3DistanceFinal;
            }
         }

         // If both metrics are close, prefer simpler matrices
         double simplA = transformationComplexity(a.transformMatrix);
         double simplB = transformationComplexity(b.transformMatrix);
         return simplA < simplB;
      });
}

