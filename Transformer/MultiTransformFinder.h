#ifndef MULTI_TRANSFORM_FINDER_H
#define MULTI_TRANSFORM_FINDER_H

#include "LatticeMatcher.h"
#include "LRL_Vector3.h"
#include "Selling.h"
#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include "Delone.h"
#include "P3.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <sstream>
#include <iomanip>
#include <tuple>
#include <set>

//#ifndef M_PI
//#define M_PI 3.14159265358979323846
//#endif

// Uses composition with LatticeMatcher instead of inheritance
template <typename T = void>
class MultiTransformFinder {
private:
   LatticeMatcherDefault m_matcher;
   static constexpr double M_PI = 3.14159265358979323846;
public:
   // Structure to hold detailed distance metrics
   struct DistanceMetrics {
      double rawS6Distance;     // Original S6 distance (with reduction)
      double euclideanDistance; // Euclidean S6 distance (without reduction)
      double normalizedDistance; // Distance scaled by cell norms
      double lengthsDeviation;   // Deviation in cell lengths
      double anglesDeviation;    // Deviation in cell angles
      double overallQuality;     // Combined score (0-100, higher is better)
      double p3Distance;         // P3 distance (added for better comparisons)
      double s6Angle;            // S6 angle in degrees (critical for crystallography)

      DistanceMetrics()
         : rawS6Distance(0.0), euclideanDistance(0.0), normalizedDistance(0.0),
         lengthsDeviation(0.0), anglesDeviation(0.0), overallQuality(100.0),
         p3Distance(0.0), s6Angle(0.0) {
      }
   };

   // Structure to track transformation plus additional ranking criteria
   struct RankedTransformation {
      typename LatticeMatcherDefault::TransformationResult result;
      DistanceMetrics metrics;        // Detailed distance metrics
      double simplicityScore;         // Lower is better (based on complexity of coefficients)
      double magnitudeScore;          // Lower is better (based on size of coefficients)
      double angleScore;              // Lower is better (based on how close to common angles)
      double overallScore;            // Combined score for final ranking

      // Default constructor needed for std::vector
      RankedTransformation()
         : result(), simplicityScore(0.0), magnitudeScore(0.0), angleScore(0.0), overallScore(0.0) {
      }

      RankedTransformation(const typename LatticeMatcherDefault::TransformationResult& r)
         : result(r), simplicityScore(0.0), magnitudeScore(0.0), angleScore(0.0), overallScore(0.0) {
      }

      // For sorting (lower overall score is better)
      bool operator<(const RankedTransformation& other) const {
         return overallScore < other.overallScore;
      }
   };

   // Constructor
   MultiTransformFinder(const LatticeCell& sourceCell, const LatticeCell& targetCell)
      : m_matcher(sourceCell, targetCell) {
   }

   // Default constructor
   MultiTransformFinder()
      : m_matcher() {
   }

   // Apply transformation (exposed publicly for identity matrix handling)
   LatticeCell applyTransformation(const Matrix_3x3& matrix) const {
      return m_matcher.applyTransformation(matrix);
   }

   // Calculate S6 norm of a cell
   double calculateS6Norm(const LatticeCell& cell) const {
      const S6 s6 = S6(cell.getCell());
      return s6.norm();
   }

   // Calculate Euclidean distance between S6 vectors (without reduction)
   double calculateEuclideanS6Distance(const LatticeCell& cell1, const LatticeCell& cell2) const {
      S6 s1 = S6(cell1.getCell());
      S6 s2 = S6(cell2.getCell());
      return (s1 - s2).norm();
   }

   // Calculate S6 angle between two cells (important for crystallography)
   double calculateS6Angle(const LatticeCell& cell1, const LatticeCell& cell2) const {
      const S6 s1 = S6(cell1.getCell());
      const S6 s2 = S6(cell2.getCell());
      S6 reduced1, reduced2;
      Selling::Reduce(s1, reduced1);
      Selling::Reduce(s2, reduced2);

      // Normalize vectors
      double norm1 = reduced1.norm();
      double norm2 = reduced2.norm();

      if (norm1 < 1e-10 || norm2 < 1e-10) {
         return 0.0; // Handle zero vectors
      }

      S6 normalized1 = reduced1 * (1.0 / norm1);
      S6 normalized2 = reduced2 * (1.0 / norm2);

      // Calculate dot product
      double dotProduct = 0.0;
      for (int i = 0; i < 6; i++) {
         dotProduct += normalized1[i] * normalized2[i];
      }

      // Clamp to valid range for acos
      if (dotProduct > 1.0) dotProduct = 1.0;
      if (dotProduct < -1.0) dotProduct = -1.0;

      // Convert to degrees
      return acos(dotProduct) * 180.0 / M_PI;
   }

   // Calculate P3 distance between two cells
   double calculateP3Distance(const LatticeCell& cell1, const LatticeCell& cell2) const {
      P3 p1(cell1.getCell());
      P3 p2(cell2.getCell());
      return P3::DistanceBetween(p1, p2);
   }

   void testSpecificMatrices(const LRL_Cell& sourceCell, const LRL_Cell& targetCell) {
      std::cout << "Testing specific transformation matrices..." << std::endl;

      // Define matrices to test
      std::vector<Matrix_3x3> matricesToTest;

      // 1. Identity
      matricesToTest.push_back(Matrix_3x3(
         1, 0, 0,
         0, 1, 0,
         0, 0, 1
      ));

      // 2. Permutation (swap a and c)
      matricesToTest.push_back(Matrix_3x3(
         0, 0, 1,
         1, 0, 0,
         0, 1, 0
      ));

      // 3. Complex matrix from previous tests
      matricesToTest.push_back(Matrix_3x3(
         3, -3, -1,
         1, 2, 2,
         -3, -1, -2
      ));

      std::cout << "Testing " << matricesToTest.size() << " specific transformation matrices..." << std::endl;

      std::cout << "Source Cell:\n     " << LRL_Cell_Degrees(sourceCell) << std::endl;
      std::cout << "Target Cell:\n     " << LRL_Cell_Degrees(targetCell) << std::endl;

      std::cout << "\nTesting each transformation matrix:\n" << std::endl;

      // Test each matrix
      int count = 0;
      for (const auto& matrix : matricesToTest) {
         count++;
         std::cout << "Matrix " << count << ":" << std::endl;
         std::cout << matrix << std::endl;

         // Calculate determinant
         double det = matrix.Det();
         std::cout << "Determinant: " << det << std::endl;

         // Apply transformation directly to B4 vectors
         const B4 sourceB4(sourceCell);
         // Use the new matrix multiplication operator
         const B4 transformedB4 = matrix * sourceB4;

         std::cout << sourceB4 << std::endl;
         std::cout << "transformedB4 " << transformedB4 << std::endl;

         // Construct the transformed cell
         const LatticeCell transformedCell = LatticeCell(transformedB4);

         std::cout << "Transformed Cell (B):\n     " << (transformedCell.getCell()) << std::endl;
         std::cout << "Transformed Cell (B):\n     " << LRL_Cell_Degrees(transformedCell.getCell()) << std::endl;

         // Calculate distances
         S6 s1 = S6(transformedCell.getCell());
         S6 s2 = S6(targetCell);
         S6 reduced1, reduced2;
         Selling::Reduce(s1, reduced1);
         Selling::Reduce(s2, reduced2);
         const double cs6Dist = CS6Dist(reduced1.data(), reduced2.data());

         // Calculate Euclidean distance
         double euclideanDist = 0.0;
         for (int i = 0; i < 6; i++) {
            double diff = s1[i] - s2[i];
            euclideanDist += diff * diff;
         }
         euclideanDist = std::sqrt(euclideanDist);

         std::cout << "CS6Dist = " << cs6Dist << ", Euclidean Dist = " << euclideanDist << std::endl;

         std::cout << "--------------------------" << std::endl;
      }
   }

   void findBasicPermutationTransformation() const {
      std::cout << "Testing only basic permutation/sign-flip matrices..." << std::endl;

      // Generate the 24 permutation matrices with determinant +1
      std::vector<Matrix_3x3> permutationMatrices;

      // Basic permutation indices
      const int perms[6][3] = {
          {0, 1, 2}, // Identity
          {0, 2, 1}, // Swap b,c
          {1, 0, 2}, // Swap a,b
          {1, 2, 0}, // Cycle right
          {2, 0, 1}, // Cycle left
          {2, 1, 0}  // Swap a,c
      };

      // Generate permutation matrices with determinant +1
      for (int p = 0; p < 6; p++) {
         for (int signA = -1; signA <= 1; signA += 2) {
            for (int signB = -1; signB <= 1; signB += 2) {
               for (int signC = -1; signC <= 1; signC += 2) {
                  // Calculate permutation parity
                  int inversions = 0;
                  for (int i = 0; i < 3; i++) {
                     for (int j = i + 1; j < 3; j++) {
                        if (perms[p][i] > perms[p][j]) {
                           inversions++;
                        }
                     }
                  }
                  bool isOdd = (inversions % 2 == 1);

                  // Calculate determinant sign
                  int det = signA * signB * signC;
                  if (isOdd) det = -det;

                  // Only accept matrices with det = +1
                  if (det != 1) continue;

                  // Create matrix
                  Matrix_3x3 matrix;
                  // Zero out the matrix
                  for (int i = 0; i < 3; i++) {
                     for (int j = 0; j < 3; j++) {
                        matrix[i * 3 + j] = 0.0;
                     }
                  }

                  // Set the permuted and sign-flipped elements
                  int sign[3] = { signA, signB, signC };
                  for (int i = 0; i < 3; i++) {
                     matrix[i * 3 + perms[p][i]] = sign[i];
                  }

                  // Verify it's a valid transformation matrix
                  if (isValidTransformationMatrix(matrix)) {
                     permutationMatrices.push_back(matrix);
                  }
               }
            }
         }
      }

      std::cout << "Testing " << permutationMatrices.size() << " basic permutation matrices with det=+1" << std::endl;

      // Test each matrix
      // [Rest of the function remains the same...]
   }

   // Generate a comprehensive set of crystallographic matrices
   std::vector<Matrix_3x3> generateCrystallographicMatrices() const {
      std::vector<Matrix_3x3> matrices;

      // Basic permutation matrices (identity, axis swaps, etc.)
      matrices.push_back(Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1));  // Identity
      matrices.push_back(Matrix_3x3(0, 1, 0, 1, 0, 0, 0, 0, 1));  // Swap x,y
      matrices.push_back(Matrix_3x3(1, 0, 0, 0, 0, 1, 0, 1, 0));  // Swap y,z
      matrices.push_back(Matrix_3x3(0, 0, 1, 0, 1, 0, 1, 0, 0));  // Swap x,z
      matrices.push_back(Matrix_3x3(0, 0, 1, 1, 0, 0, 0, 1, 0));  // xyz->zxy
      matrices.push_back(Matrix_3x3(0, 1, 0, 0, 0, 1, 1, 0, 0));  // xyz->yzx

      // Sign flips with permutations (still with det=+1)
      matrices.push_back(Matrix_3x3(-1, 0, 0, 0, -1, 0, 0, 0, 1));  // Invert x,y
      matrices.push_back(Matrix_3x3(-1, 0, 0, 0, 1, 0, 0, 0, -1));  // Invert x,z
      matrices.push_back(Matrix_3x3(1, 0, 0, 0, -1, 0, 0, 0, -1));  // Invert y,z

      // Common shear transformations (with det=+1)
      matrices.push_back(Matrix_3x3(1, 1, 0, 0, 1, 0, 0, 0, 1));  // Shear in xy plane
      matrices.push_back(Matrix_3x3(1, 0, 1, 0, 1, 0, 0, 0, 1));  // Shear in xz plane
      matrices.push_back(Matrix_3x3(1, 0, 0, 1, 1, 0, 0, 0, 1));  // Shear in yx plane
      matrices.push_back(Matrix_3x3(1, 0, 0, 0, 1, 1, 0, 0, 1));  // Shear in yz plane
      matrices.push_back(Matrix_3x3(1, 0, 0, 0, 1, 0, 1, 0, 1));  // Shear in zx plane
      matrices.push_back(Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 1, 1));  // Shear in zy plane

      // Negative shears
      matrices.push_back(Matrix_3x3(1, -1, 0, 0, 1, 0, 0, 0, 1));  // Negative shear in xy
      matrices.push_back(Matrix_3x3(1, 0, -1, 0, 1, 0, 0, 0, 1));  // Negative shear in xz
      matrices.push_back(Matrix_3x3(1, 0, 0, -1, 1, 0, 0, 0, 1));  // Negative shear in yx
      matrices.push_back(Matrix_3x3(1, 0, 0, 0, 1, -1, 0, 0, 1));  // Negative shear in yz
      matrices.push_back(Matrix_3x3(1, 0, 0, 0, 1, 0, -1, 0, 1));  // Negative shear in zx
      matrices.push_back(Matrix_3x3(1, 0, 0, 0, 1, 0, 0, -1, 1));  // Negative shear in zy

      // Common centering operations (preserving det=+1)
      matrices.push_back(Matrix_3x3(1, 1, 1, 0, 1, 0, 0, 0, 1));  // Face-centered related
      matrices.push_back(Matrix_3x3(1, 0, 0, 1, 1, 1, 0, 0, 1));  // Body-centered related 
      matrices.push_back(Matrix_3x3(1, 1, 0, 1, 0, 0, 1, 1, 1));  // Complex centering
      matrices.push_back(Matrix_3x3(1, 0, 0, 1, 1, 0, 1, 0, 1));  // Mixed centering
      matrices.push_back(Matrix_3x3(0, 1, 0, 0, 0, 1, 1, 1, 1));  // Mixed centering

      // Complex matrices known to be useful for challenging cases
      matrices.push_back(Matrix_3x3(0, -1, 0, 1, 0, 0, 1, 1, 1));  // LeTrong & Stenkamp case
      matrices.push_back(Matrix_3x3(-1, 1, 0, -1, 0, 0, 0, 0, -1)); // Charley Simmons case
      matrices.push_back(Matrix_3x3(0, 1, 0, 0, 0, 1, 1, 0, 1));    // For last test case
      matrices.push_back(Matrix_3x3(0, 1, 0, 0, -1, 1, 1, 0, 0));    // Mixed operations
      matrices.push_back(Matrix_3x3(1, 1, 0, -1, 1, 0, 0, 0, 1));    // Mixed operations

      return matrices;
   }

   // Calculate detailed distance metrics between two cells
   DistanceMetrics calculateDistanceMetrics(const LatticeCell& cell1, const LatticeCell& cell2) const {
      DistanceMetrics metrics;

      // 1. Calculate raw S6 distance with explicit Selling reduction
      S6 s61 = S6(cell1.getCell());
      S6 s62 = S6(cell2.getCell());
      S6 reduced1, reduced2;
      Selling::Reduce(s61, reduced1);
      Selling::Reduce(s62, reduced2);
      metrics.rawS6Distance = CS6Dist(reduced1.data(), reduced2.data());

      // 2. Calculate Euclidean S6 distance (without reduction)
      metrics.euclideanDistance = calculateEuclideanS6Distance(cell1, cell2);

      // 3. Calculate normalized distance (scale by norms)
      const double normCell1 = reduced1.norm();
      const double normCell2 = reduced2.norm();
      const double avgNorm = (normCell1 + normCell2) / 2.0;

      if (avgNorm > 1e-10) {
         metrics.normalizedDistance = (metrics.rawS6Distance / avgNorm) * 100.0;
      }
      else {
         metrics.normalizedDistance = metrics.rawS6Distance * 100.0;
      }

      // 4. Calculate length deviation (as percentage)
      const LRL_Cell& c1 = cell1.getCell();
      const LRL_Cell& c2 = cell2.getCell();

      double lengthSumSquaredDiff = 0.0;
      for (int i = 0; i < 3; ++i) {
         if (std::abs(c2[i]) > 1e-10) {
            double relDiff = std::abs(c1[i] - c2[i]) / c2[i]; // Relative to target
            lengthSumSquaredDiff += relDiff * relDiff;
         }
      }
      metrics.lengthsDeviation = std::sqrt(lengthSumSquaredDiff / 3.0) * 100.0;

      // 5. Calculate angle deviation (as percentage of 180 degrees)
      double angleSumSquaredDiff = 0.0;
      const double rad2deg = 180.0 / M_PI;
      for (int i = 3; i < 6; ++i) {
         double diffDeg = std::abs((c1[i] - c2[i]) * rad2deg);
         if (diffDeg > 180.0) diffDeg = 360.0 - diffDeg;
         double relDiff = diffDeg / 180.0;
         angleSumSquaredDiff += relDiff * relDiff;
      }
      metrics.anglesDeviation = std::sqrt(angleSumSquaredDiff / 3.0) * 100.0;

      // 6. Calculate overall quality (invert so higher is better)
      // Weight lengths and angles equally
      double rawQuality = 100.0 - (metrics.lengthsDeviation + metrics.anglesDeviation) / 2.0;
      metrics.overallQuality = rawQuality > 0.0 ? rawQuality : 0.0;

      // 7. Add P3 distance
      metrics.p3Distance = calculateP3Distance(cell1, cell2);

      // 8. Add S6 angle
      metrics.s6Angle = calculateS6Angle(cell1, cell2);

      return metrics;
   }

   // Calculate distance metrics between transformed cell and target
   DistanceMetrics calculateDistanceMetrics(const LatticeCell& transformedCell) const {
      return calculateDistanceMetrics(transformedCell, m_matcher.getTargetCell());
   }

   // Standard cell distance for compatibility - single cell version (compares with target)
   // This is CS6Dist with reduction
   double calculateCellDistance(const LatticeCell& transformedCell) const {
      // Calculate with explicit Selling reduction for CS6Dist
      S6 s1 = S6(transformedCell.getCell());
      S6 s2 = S6(m_matcher.getTargetCell().getCell());
      S6 reduced1, reduced2;
      Selling::Reduce(s1, reduced1);
      Selling::Reduce(s2, reduced2);
      return CS6Dist(reduced1.data(), reduced2.data());
   }

   // Two-parameter version that compares any two cells
   double calculateCellDistance(const LatticeCell& cell1, const LatticeCell& cell2) const {
      // Calculate with explicit Selling reduction for CS6Dist
      S6 s1 = S6(cell1.getCell());
      S6 s2 = S6(cell2.getCell());
      S6 reduced1, reduced2;
      Selling::Reduce(s1, reduced1);
      Selling::Reduce(s2, reduced2);
      return CS6Dist(reduced1.data(), reduced2.data());
   }


   // Check if matrix has integer coefficients (within tolerance)
   bool isIntegerMatrix(const Matrix_3x3& matrix) const {
      for (int i = 0; i < 9; ++i) {
         if (std::abs(matrix[i] - std::round(matrix[i])) > 1e-10) {
            return false;
         }
      }
      return true;
   }

   // Check if a cell is valid (non-zero lengths and angles)
   bool isValidCell(const LatticeCell& cell) const {
      const LRL_Cell& params = cell.getCell();

      // Check for zero or negative lengths
      for (int i = 0; i < 3; ++i) {
         if (params[i] <= 1e-6) return false;
      }

      // Check for zero or invalid angles
      for (int i = 3; i < 6; ++i) {
         if (params[i] <= 1e-6 || params[i] >= M_PI) return false;
      }

      return true;
   }

   // Matrix3x3 comparator for std::set
   struct Matrix3x3Comparator {
      bool operator()(const Matrix_3x3& lhs, const Matrix_3x3& rhs) const {
         constexpr double tolerance = 1e-10;
         for (int i = 0; i < 9; ++i) {
            if (std::abs(lhs[i] - rhs[i]) > tolerance) {
               return lhs[i] < rhs[i];
            }
         }
         return false;  // Matrices are equal
      }
   };

   // Generate all 24 orientation matrices in E3 (corresponding to S6 reflections)
   std::vector<Matrix_3x3> generateOrientationMatrices() const {
      // Use a set to automatically handle duplicates
      std::set<Matrix_3x3, Matrix3x3Comparator> matricesSet;

      // Add basic permutation matrices with determinant +1
      // Identity
      matricesSet.insert(Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1));

      // Permutations
      matricesSet.insert(Matrix_3x3(0, 1, 0, 1, 0, 0, 0, 0, 1));
      matricesSet.insert(Matrix_3x3(0, 0, 1, 1, 0, 0, 0, 1, 0));
      matricesSet.insert(Matrix_3x3(0, 0, 1, 0, 1, 0, 1, 0, 0));
      matricesSet.insert(Matrix_3x3(1, 0, 0, 0, 0, 1, 0, 1, 0));
      matricesSet.insert(Matrix_3x3(0, 1, 0, 0, 0, 1, 1, 0, 0));

      // Add inversions and combinations that maintain determinant +1
      std::vector<Matrix_3x3> initialMatrices(matricesSet.begin(), matricesSet.end());
      for (const auto& matrix : initialMatrices) {
         // Create modified versions with sign changes that maintain det +1
         Matrix_3x3 m1(matrix);
         m1[0] = -m1[0]; m1[1] = -m1[1]; m1[2] = -m1[2];
         if (isValidTransformationMatrix(m1))
            matricesSet.insert(m1);

         Matrix_3x3 m2(matrix);
         m2[3] = -m2[3]; m2[4] = -m2[4]; m2[5] = -m2[5];
         if (isValidTransformationMatrix(m2))
            matricesSet.insert(m2);

         Matrix_3x3 m3(matrix);
         m3[6] = -m3[6]; m3[7] = -m3[7]; m3[8] = -m3[8];
         if (isValidTransformationMatrix(m3))
            matricesSet.insert(m3);
      }

      // Convert set to vector
      std::vector<Matrix_3x3> uniqueMatrices(matricesSet.begin(), matricesSet.end());
      return uniqueMatrices;
   }

   // Check if matrix has determinant +1 exactly
   //bool isValidTransformationMatrix(const Matrix_3x3& matrix) const {
   //   // Check determinant (must be exactly +1 for valid lattice transformations)
   //   const double det = matrix.Det();
   //   if (std::abs(det - 1.0) > 1e-10) {
   //      return false;
   //   }

   //   return true;
   //}

   // Generate all valid integer matrices with determinant +1
   std::vector<Matrix_3x3> generateAllIntegerMatrices(int maxCoeff) const {
      // Start with a comprehensive set of crystallographic matrices
      std::vector<Matrix_3x3> allMatrices = generateCrystallographicMatrices();

      // Add standard integer matrices from LatticeMatcher
      std::vector<Matrix_3x3> baseMatrices = m_matcher.generateIntegerMatrices(maxCoeff);

      // Filter to only those with determinant +1
      for (const auto& matrix : baseMatrices) {
         if (isValidTransformationMatrix(matrix)) {
            allMatrices.push_back(matrix);
         }
      }

      // Get all orientation matrices
      std::vector<Matrix_3x3> orientationMatrices = generateOrientationMatrices();

      // Use a set to automatically handle duplicates
      std::set<Matrix_3x3, Matrix3x3Comparator> uniqueMatricesSet;

      // Combine base matrices with orientations
      for (const auto& baseMatrix : allMatrices) {
         uniqueMatricesSet.insert(baseMatrix);

         for (const auto& orientMatrix : orientationMatrices) {
            Matrix_3x3 combined = baseMatrix * orientMatrix;
            if (isValidTransformationMatrix(combined) && isIntegerMatrix(combined)) {
               uniqueMatricesSet.insert(combined);
            }
         }
      }

      // Convert set to vector
      std::vector<Matrix_3x3> uniqueMatrices(uniqueMatricesSet.begin(), uniqueMatricesSet.end());
      return uniqueMatrices;
   }

   // Helper for matrix equality checking
   bool matrixEquals(const Matrix_3x3& m1, const Matrix_3x3& m2, double tolerance = 1e-10) const {
      for (int i = 0; i < 9; ++i) {
         if (std::abs(m1[i] - m2[i]) > tolerance) {
            return false;
         }
      }
      return true;
   }

   // Calculate scores for a transformation (exposed publicly for identity matrix handling)
   void calculateRankingScores(RankedTransformation& ranked, const LatticeCell& transformedCell) const {
      const Matrix_3x3& matrix = ranked.result.transformMatrix;

      // Calculate detailed distance metrics
      ranked.metrics = calculateDistanceMetrics(transformedCell);

      // Calculate P3 distance explicitly and store
      double p3Distance = calculateP3Distance(transformedCell, m_matcher.getTargetCell());
      ranked.metrics.p3Distance = p3Distance;

      // Calculate S6 angle explicitly and store
      double s6Angle = calculateS6Angle(transformedCell, m_matcher.getTargetCell());
      ranked.metrics.s6Angle = s6Angle;

      // Simplicity score: count non-zero elements and complexity of fractions
      double simplicityScore = 0.0;
      for (int i = 0; i < 9; ++i) {
         if (std::abs(matrix[i]) > 1e-10) {
            simplicityScore += 1.0;

            // Add penalty for non-integer or large values
            const double fractionalPart = std::abs(matrix[i] - std::round(matrix[i]));
            if (fractionalPart > 1e-10) {
               // Penalty for fractional values
               simplicityScore += 2.0;

               // Special case for common fractions
               if (std::abs(fractionalPart - 0.5) < 1e-10) {
                  // Less penalty for 1/2
                  simplicityScore -= 1.0;
               }
               else if (std::abs(fractionalPart - 1.0 / 3.0) < 1e-10 ||
                  std::abs(fractionalPart - 2.0 / 3.0) < 1e-10) {
                  // Medium penalty for 1/3 or 2/3
                  simplicityScore -= 0.5;
               }
            }
         }
      }

      // Magnitude score: sum of absolute values of coefficients
      double magnitudeScore = 0.0;
      for (int i = 0; i < 9; ++i) {
         magnitudeScore += std::abs(matrix[i]);
      }

      // Angle score: how close angles are to common crystallographic angles
      double angleScore = 0.0;
      const LRL_Cell cell = transformedCell.getCell();
      const LRL_Cell targetCell = m_matcher.getTargetCell().getCell();

      // Common angles in radians - add the target cell angles
      std::vector<double> commonAngles = {
          M_PI / 2.0,       // 90 degrees
          M_PI / 3.0,       // 60 degrees
          2.0 * M_PI / 3.0, // 120 degrees
          M_PI / 4.0,       // 45 degrees
          M_PI / 6.0,       // 30 degrees
          targetCell[3],    // Target alpha
          targetCell[4],    // Target beta
          targetCell[5]     // Target gamma
      };

      // Check each cell angle against common angles
      for (int i = 3; i < 6; ++i) {
         double minDiff = M_PI;
         for (const double commonAngle : commonAngles) {
            const double diff = std::abs(cell[i] - commonAngle);
            minDiff = std::min(minDiff, diff);
         }
         angleScore += minDiff;
      }

      // Store scores
      ranked.simplicityScore = simplicityScore;
      ranked.magnitudeScore = magnitudeScore;
      ranked.angleScore = angleScore * 10.0; // Weight angle score more heavily

      // Calculate overall score (weighted combination)
      // Modified weights to emphasize S6 angle and P3 distance
      const double s6AngleWeight = 5.0;      // Primary weight - angles are most important
      const double p3DistWeight = 3.0;       // Secondary weight - P3 distance
      const double euclideanWeight = 1.0;    // Tertiary weight - Euclidean S6 distance
      const double cs6DistWeight = 0.3;      // CS6Dist as supporting metric
      const double simplicityWeight = 0.2;   // Minor factor - prefer simpler matrices
      const double magnitudeWeight = 0.1;    // Minor factor - prefer smaller coefficients

      // Use all metrics with appropriate weighting
      ranked.overallScore =
         s6AngleWeight * s6Angle +
         p3DistWeight * p3Distance +
         euclideanWeight * ranked.metrics.euclideanDistance +
         cs6DistWeight * ranked.metrics.rawS6Distance +
         simplicityWeight * ranked.simplicityScore +
         magnitudeWeight * ranked.magnitudeScore;
   }
};
#endif // MULTI_TRANSFORM_FINDER_H