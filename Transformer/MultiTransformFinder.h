#ifndef MULTI_TRANSFORM_FINDER_H
#define MULTI_TRANSFORM_FINDER_H

#include "LatticeMatcher.h"
#include "LRL_Vector3.h"
#include "Selling.h"
#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include "Delone.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <sstream>
#include <iomanip>
#include <tuple>
#include <set>

// Uses composition with LatticeMatcher instead of inheritance
template <typename T = void>
class MultiTransformFinder {
public:
   // Structure to hold detailed distance metrics
   struct DistanceMetrics {
      double rawS6Distance;     // Original S6 distance (with reduction)
      double euclideanDistance; // Euclidean S6 distance (without reduction)
      double normalizedDistance; // Distance scaled by cell norms
      double lengthsDeviation;   // Deviation in cell lengths
      double anglesDeviation;    // Deviation in cell angles
      double overallQuality;     // Combined score (0-100, higher is better)

      DistanceMetrics()
         : rawS6Distance(0.0), euclideanDistance(0.0), normalizedDistance(0.0),
         lengthsDeviation(0.0), anglesDeviation(0.0),
         overallQuality(100.0) {
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



   // Generate all valid integer matrices with determinant +1
   std::vector<Matrix_3x3> generateAllIntegerMatrices(int maxCoeff) const {
      // Get basic integer matrices from LatticeMatcher
      std::vector<Matrix_3x3> baseMatrices = m_matcher.generateIntegerMatrices(maxCoeff);

      // Filter to only those with determinant +1
      std::vector<Matrix_3x3> validBaseMatrices;
      for (const auto& matrix : baseMatrices) {
         if (isValidTransformationMatrix(matrix)) {
            validBaseMatrices.push_back(matrix);
         }
      }

      // Get all orientation matrices
      std::vector<Matrix_3x3> orientationMatrices = generateOrientationMatrices();

      // Use a set to automatically handle duplicates
      std::set<Matrix_3x3, Matrix3x3Comparator> uniqueMatricesSet;

      // Combine base matrices with orientations
      for (const auto& baseMatrix : validBaseMatrices) {
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

   // Calculate scores for a transformation (exposed publicly for identity matrix handling)
   void calculateRankingScores(RankedTransformation& ranked, const LatticeCell& transformedCell) const {
      const Matrix_3x3& matrix = ranked.result.transformMatrix;

      // Calculate detailed distance metrics
      ranked.metrics = calculateDistanceMetrics(transformedCell);

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
      // Modified weights to emphasize S6 distance more
      const double euclideanWeight = 2.0;    // Primary weight for Euclidean distance
      const double cs6DistWeight = 0.5;      // Secondary weight for CS6Dist
      const double simplicityWeight = 0.1;
      const double magnitudeWeight = 0.05;
      const double angleWeight = 0.15;

      // Use both distance metrics, but prioritize Euclidean distance
      ranked.overallScore =
         euclideanWeight * ranked.metrics.euclideanDistance +
         cs6DistWeight * ranked.metrics.rawS6Distance +
         simplicityWeight * ranked.simplicityScore +
         magnitudeWeight * ranked.magnitudeScore +
         angleWeight * ranked.angleScore;
   }

   // Find multiple transformations within a distance threshold
   std::vector<RankedTransformation> findMultipleTransformations(
      const double distanceThreshold = 50.0,
      const size_t maxResults = 10) const {

      std::vector<RankedTransformation> candidates;

      // Generate all valid integer matrices with determinant +1
      const std::vector<Matrix_3x3> allMatrices = generateAllIntegerMatrices(3);

      std::cout << "Generated " << allMatrices.size() << " valid integer matrices with det=+1" << std::endl;

      // Evaluate all matrices and collect those under threshold
      for (const auto& matrix : allMatrices) {
         const LatticeCell transformedCell = m_matcher.applyTransformation(matrix);

         // Skip invalid cells
         if (!isValidCell(transformedCell)) continue;

         // Calculate both distances
         const double cs6Distance = calculateCellDistance(transformedCell);
         const double euclideanDistance = calculateEuclideanS6Distance(transformedCell, m_matcher.getTargetCell());

         // Store if CS6 distance is below threshold
         if (cs6Distance < distanceThreshold) {
            typename LatticeMatcherDefault::TransformationResult result(matrix, cs6Distance, true);
            RankedTransformation rankedResult(result);
            calculateRankingScores(rankedResult, transformedCell);
            rankedResult.metrics.euclideanDistance = euclideanDistance;
            candidates.push_back(rankedResult);
         }
      }

      // Try some fractional matrices too, but only with determinant +1
      const std::vector<Matrix_3x3> fractionalMatrices = m_matcher.generateFractionalMatrices(3);

      for (const auto& matrix : fractionalMatrices) {
         if (!isValidTransformationMatrix(matrix)) {
            continue;  // Skip matrices without determinant +1
         }

         const LatticeCell transformedCell = m_matcher.applyTransformation(matrix);

         // Skip invalid cells
         if (!isValidCell(transformedCell)) continue;

         // Calculate both distances
         const double cs6Distance = calculateCellDistance(transformedCell);
         const double euclideanDistance = calculateEuclideanS6Distance(transformedCell, m_matcher.getTargetCell());

         // Store if CS6 distance is below threshold
         if (cs6Distance < distanceThreshold) {
            typename LatticeMatcherDefault::TransformationResult result(matrix, cs6Distance, true);
            RankedTransformation rankedResult(result);
            calculateRankingScores(rankedResult, transformedCell);
            rankedResult.metrics.euclideanDistance = euclideanDistance;
            candidates.push_back(rankedResult);
         }
      }

      // Remove duplicates (transformations that are essentially the same)
      removeDuplicateTransformations(candidates);

      // Sort primarily by CS6Dist, with cell parameter similarity as a tiebreaker
      std::sort(candidates.begin(), candidates.end(),
         [](const RankedTransformation& a, const RankedTransformation& b) {
            // For nearly identical CS6Dist values (cells representing the same lattice)
            if (std::abs(a.metrics.rawS6Distance - b.metrics.rawS6Distance) < 1.0) {
               // Sort by cell parameter differences (lengths and angles)
               return (a.metrics.lengthsDeviation + a.metrics.anglesDeviation) <
                  (b.metrics.lengthsDeviation + b.metrics.anglesDeviation);
            }
            // Otherwise, prioritize lower CS6Dist
            return a.metrics.rawS6Distance < b.metrics.rawS6Distance;
         });

      // Limit number of results
      if (candidates.size() > maxResults) {
         candidates.resize(maxResults);
      }

      return candidates;
   }

   void findBestSimpleTransformation() const {
      std::cout << "Testing simple axis permutation and sign-flip transformations..." << std::endl;

      // Generate the 24 basic permutation and sign-flip matrices with determinant +1
      std::vector<Matrix_3x3> permutationMatrices;

      // Basic permutation indices
      const int perms[6][3] = {
          {0, 1, 2}, // Identity
          {1, 0, 2}, // Swap a,b
          {0, 2, 1}, // Swap b,c
          {2, 1, 0}, // Swap a,c
          {1, 2, 0}, // Cycle right
          {2, 0, 1}  // Cycle left
      };

      // Generate all basic transformation matrices with determinant +1
      for (int p = 0; p < 6; p++) {
         for (int signA = -1; signA <= 1; signA += 2) {
            for (int signB = -1; signB <= 1; signB += 2) {
               for (int signC = -1; signC <= 1; signC += 2) {
                  // Calculate determinant
                  int det = signA * signB * signC;
                  if (p % 2 == 1) det = -det; // Odd permutations flip sign

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

                  permutationMatrices.push_back(matrix);
               }
            }
         }
      }

      std::cout << "Testing " << permutationMatrices.size() << " basic permutation/sign matrices with det=+1" << std::endl;

      // Test each matrix
      std::vector<std::pair<Matrix_3x3, double>> results;

      for (const auto& matrix : permutationMatrices) {
         const LatticeCell transformedCell = m_matcher.applyTransformation(matrix);

         // Calculate CS6 distance with reduction
         S6 s1 = S6(transformedCell.getCell());
         S6 s2 = S6(m_matcher.getTargetCell().getCell());
         S6 reduced1, reduced2;
         Selling::Reduce(s1, reduced1);
         Selling::Reduce(s2, reduced2);
         const double cs6Dist = CS6Dist(reduced1.data(), reduced2.data());

         // Store matrix and distance
         results.emplace_back(matrix, cs6Dist);
      }

      // Sort by CS6Distance
      std::sort(results.begin(), results.end(),
         [](const auto& a, const auto& b) { return a.second < b.second; });

      // Show best results
      std::cout << "Best transformations (sorted by CS6Dist):" << std::endl;
      size_t maxToShow = std::min(results.size(), size_t(10));

      for (size_t i = 0; i < maxToShow; ++i) {
         const auto& [matrix, distance] = results[i];
         std::cout << "Matrix " << (i + 1) << " (CS6Dist = " << distance << "):" << std::endl;
         std::cout << matrix << std::endl;

         // Show transformed cell
         const LatticeCell transformedCell = m_matcher.applyTransformation(matrix);
         const LRL_Cell& params = transformedCell.getCell();
         const LRL_Cell& targetParams = m_matcher.getTargetCell().getCell();

         std::cout << "Transformed Cell:" << std::endl;
         std::cout << "  " << LRL_Cell_Degrees(params) << std::endl;

         std::cout << "Target Cell:     " << std::endl;
         std::cout << "  " << LRL_Cell_Degrees(targetParams) << std::endl;

         std::cout << "--------------------------" << std::endl;
      }
   }



   // Struct for storing matrix distance info
   struct MatrixDistanceInfo {
      Matrix_3x3 matrix;
      double cs6Dist;
      double euclideanDist;
      LatticeCell transformedCell;

      MatrixDistanceInfo(const Matrix_3x3& m, double cs6, double eucl, const LatticeCell& cell)
         : matrix(m), cs6Dist(cs6), euclideanDist(eucl), transformedCell(cell) {
      }
   };


   // Format a list of ranked transformations as a string
   std::string formatMultipleTransformations(
      const std::vector<RankedTransformation>& transformations,
      const int precision = 4) const {

      std::ostringstream oss;
      oss << std::fixed << std::setprecision(precision);

      oss << "Found " << transformations.size() << " potential transformations:" << std::endl << std::endl;

      for (size_t i = 0; i < transformations.size(); ++i) {
         const auto& ranked = transformations[i];
         const auto& result = ranked.result;
         const auto& metrics = ranked.metrics;

         oss << "Transformation " << (i + 1) << ":" << std::endl;
         oss << "-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#" << std::endl;
         oss << "Matrix:" << std::endl;
         oss << result.transformMatrix << std::endl << std::endl;

         oss << "As Lattice Vector Operations:" << std::endl;
         oss << LatticeMatcherDefault::interpretTransformation(result.transformMatrix) << std::endl << std::endl;

         oss << "Distance Metrics:" << std::endl;
         oss << "  Euclidean S6 Distance: " << metrics.euclideanDistance << std::endl;
         oss << "  Raw S6 Distance (with reduction): " << metrics.rawS6Distance << std::endl;
         oss << "  Normalized Distance: " << metrics.normalizedDistance << "%" << std::endl;
         oss << "  Cell Lengths Deviation: " << metrics.lengthsDeviation << "%" << std::endl;
         oss << "  Cell Angles Deviation: " << metrics.anglesDeviation << "%" << std::endl;
         oss << "  Overall Match Quality: " << metrics.overallQuality << "%" << std::endl << std::endl;

         oss << "Matrix Properties:" << std::endl;
         oss << "  Determinant: " << result.transformMatrix.Det() << std::endl;
         oss << "  Simplicity Score: " << ranked.simplicityScore << std::endl;
         oss << "  Magnitude Score: " << ranked.magnitudeScore << std::endl;
         oss << "  Angle Score: " << ranked.angleScore << std::endl;
         oss << "  Overall Score: " << ranked.overallScore << std::endl << std::endl;

         // Display transformed cell
         const LatticeCell transformedCell = m_matcher.applyTransformation(result.transformMatrix);
         const LRL_Cell transformedParams = transformedCell.getCell();
         const LRL_Cell targetParams = m_matcher.getTargetCell().getCell();

         // Display transformed cell using LRL_Cell_Degrees for readable output
         oss << "Transformed Cell:" << std::endl;
         oss << LRL_Cell_Degrees(transformedParams) << std::endl << std::endl;

         // Display target cell using LRL_Cell_Degrees for readable output
         oss << "Target Cell:    " << std::endl;
         oss << LRL_Cell_Degrees(targetParams) << std::endl << std::endl;
      }

      return oss.str();
   }

   // Set source and target cells
   void setSourceCell(const LatticeCell& cell) {
      m_matcher.setSourceCell(cell);
   }

   void setTargetCell(const LatticeCell& cell) {
      m_matcher.setTargetCell(cell);
   }

   // Custom comparator for RankedTransformation
   struct TransformComparator {
      const MultiTransformFinder* finder;

      TransformComparator(const MultiTransformFinder* f) : finder(f) {}

      bool operator()(const RankedTransformation& a, const RankedTransformation& b) const {
         const Matrix_3x3& matA = a.result.transformMatrix;
         const Matrix_3x3& matB = b.result.transformMatrix;

         // Compare matrices directly using Matrix3x3Comparator
         Matrix3x3Comparator matComp;
         return matComp(matA, matB);
      }
   };

   // Remove transformations that are effectively duplicates
   void removeDuplicateTransformations(std::vector<RankedTransformation>& transformations) const {
      if (transformations.empty()) return;

      // Use a set with our custom comparator
      std::set<RankedTransformation, TransformComparator> uniqueSet(TransformComparator(this));

      // Insert all transformations - duplicates will be automatically removed
      for (const auto& transform : transformations) {
         uniqueSet.insert(transform);
      }

      // Convert back to vector
      transformations.clear();
      transformations.insert(transformations.end(), uniqueSet.begin(), uniqueSet.end());
   }

   // Stream output operator for MultiTransformFinder
   friend std::ostream& operator<<(std::ostream& os, const MultiTransformFinder& finder) {
      os << "MultiTransformFinder:" << std::endl;
      os << "  Source cell: " << std::endl;
      os << finder.m_matcher.getSourceCell() << std::endl;
      os << "  Target cell: " << std::endl;
      os << finder.m_matcher.getTargetCell() << std::endl;
      return os;
   }

private:
   LatticeMatcherDefault m_matcher;
   static constexpr double M_PI = 3.14159265358979323846;
};

// Type alias for convenience
using MultiTransformFinderDefault = MultiTransformFinder<void>;

#endif // MULTI_TRANSFORM_FINDER_H

