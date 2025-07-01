#ifndef MATRIX_SIMPLICITY_RANKER_H
#define MATRIX_SIMPLICITY_RANKER_H

#include "LatticeMatchResult.h"
#include "TransformerUtilities.h"
#include "Matrix_3x3.h"
#include "S6.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <limits>

class MatrixSimplicityRanker {
public:
   struct ScoredResult {
      LatticeMatchResult result;
      double qualityScore;
      double simplicityScore;
      double compositeScore;
      double distanceFromIdentity;
      std::string category;
   };

   // Calculate simplicity score using distance from identity matrix
   static double calculateSimplicityScore(const Matrix_3x3& matrix) {
      // Create identity matrix
      Matrix_3x3 identity(1, 0, 0, 0, 1, 0, 0, 0, 1);

      // Calculate matrix - identity
      Matrix_3x3 difference = matrix - identity;

      // The norm of the difference is our primary simplicity metric
      // Lower norm = closer to identity = simpler
      double distanceFromIdentity = difference.norm();

      // Optional: Also consider the original matrix norm for additional context
      double originalNorm = matrix.norm();

      // Combine both metrics - distance from identity is primary
      double score = distanceFromIdentity + 0.1 * originalNorm;

      return score;
   }

   // Pure distance from identity metric
   static double calculateDistanceFromIdentity(const Matrix_3x3& matrix) {
      Matrix_3x3 identity(1, 0, 0, 0, 1, 0, 0, 0, 1);
      Matrix_3x3 difference = matrix - identity;
      return difference.norm();
   }

   // Quality-aware ranking: only prefer simpler matrices if quality is comparable
   static std::vector<LatticeMatchResult> rankByQualityAwareSimplicity(
      const std::vector<LatticeMatchResult>& results,
      const LRL_Cell& referenceCell,
      double qualityToleranceRatio = 0.1) { // 10% quality tolerance for simplicity preference

      if (results.empty()) return results;

      std::vector<ScoredResult> scoredResults;

      // Find the best quality score to establish tolerance
      double bestQualityScore = std::numeric_limits<double>::max();
      for (const auto& result : results) {
         double qualityScore = calculateQualityScore(result, referenceCell);
         bestQualityScore = std::min(bestQualityScore, qualityScore);
      }

      double qualityTolerance = bestQualityScore * qualityToleranceRatio;

      // Score all results
      for (const auto& result : results) {
         ScoredResult scored;
         scored.result = result;
         scored.qualityScore = calculateQualityScore(result, referenceCell);
         scored.simplicityScore = calculateSimplicityScore(result.getTransformationMatrix());
         scored.distanceFromIdentity = calculateDistanceFromIdentity(result.getTransformationMatrix());
         scored.category = categorizeMatrix(result.getTransformationMatrix());

         // Quality-aware composite scoring
         if (scored.qualityScore <= bestQualityScore + qualityTolerance) {
            // Within quality tolerance - consider simplicity
            scored.compositeScore = scored.qualityScore + 0.1 * scored.simplicityScore;
         }
         else {
            // Outside quality tolerance - quality dominates
            scored.compositeScore = scored.qualityScore + 0.01 * scored.simplicityScore;
         }

         scoredResults.push_back(scored);
      }

      // Sort by composite score
      std::sort(scoredResults.begin(), scoredResults.end(),
         [](const ScoredResult& a, const ScoredResult& b) {
            return a.compositeScore < b.compositeScore;
         });

      // Extract sorted results
      std::vector<LatticeMatchResult> sortedResults;
      for (const auto& scored : scoredResults) {
         sortedResults.push_back(scored.result);
      }

      return sortedResults;
   }

   // Compare matrices by their distance from identity only
   static std::vector<LatticeMatchResult> rankByDistanceFromIdentity(
      const std::vector<LatticeMatchResult>& results) {

      std::vector<std::pair<LatticeMatchResult, double>> scoredResults;

      for (const auto& result : results) {
         double distance = calculateDistanceFromIdentity(result.getTransformationMatrix());
         scoredResults.push_back({ result, distance });
      }

      // Sort by distance from identity (lower = simpler)
      std::sort(scoredResults.begin(), scoredResults.end(),
         [](const auto& a, const auto& b) {
            return a.second < b.second;
         });

      std::vector<LatticeMatchResult> sortedResults;
      for (const auto& scored : scoredResults) {
         sortedResults.push_back(scored.first);
      }

      return sortedResults;
   }

   // Alternative: Strict quality groups with simplicity within each group
   static std::vector<LatticeMatchResult> groupByQualityThenSimplicity(
      const std::vector<LatticeMatchResult>& results,
      const LRL_Cell& referenceCell) {

      if (results.empty()) return results;

      std::vector<ScoredResult> scoredResults;

      // Score all results
      for (const auto& result : results) {
         ScoredResult scored;
         scored.result = result;
         scored.qualityScore = calculateQualityScore(result, referenceCell);
         scored.simplicityScore = calculateSimplicityScore(result.getTransformationMatrix());
         scored.distanceFromIdentity = calculateDistanceFromIdentity(result.getTransformationMatrix());
         scored.category = categorizeMatrix(result.getTransformationMatrix());
         scoredResults.push_back(scored);
      }

      // Group by quality tiers
      std::sort(scoredResults.begin(), scoredResults.end(),
         [](const ScoredResult& a, const ScoredResult& b) {
            // First sort by quality tier
            double qualityTierA = std::floor(std::log10(std::max(a.qualityScore, 1e-10)));
            double qualityTierB = std::floor(std::log10(std::max(b.qualityScore, 1e-10)));

            if (std::abs(qualityTierA - qualityTierB) > 0.5) {
               return qualityTierA < qualityTierB;
            }

            // Within same quality tier, sort by simplicity
            return a.simplicityScore < b.simplicityScore;
         });

      // Extract sorted results
      std::vector<LatticeMatchResult> sortedResults;
      for (const auto& scored : scoredResults) {
         sortedResults.push_back(scored.result);
      }

      return sortedResults;
   }

   // Debug function to show scoring details including distance from identity
   static void displayScoringDetails(const std::vector<LatticeMatchResult>& results,
      const LRL_Cell& referenceCell) {
      std::cout << "\n=== MATRIX SIMPLICITY SCORING DETAILS ===" << std::endl;

      for (size_t i = 0; i < results.size(); ++i) {
         const auto& result = results[i];
         const auto& matrix = result.getTransformationMatrix();

         double qualityScore = calculateQualityScore(result, referenceCell);
         double simplicityScore = calculateSimplicityScore(matrix);
         double distanceFromIdentity = calculateDistanceFromIdentity(matrix);
         std::string category = categorizeMatrix(matrix);

         std::cout << "Result " << (i + 1) << ":" << std::endl;
         std::cout << "  Category: " << category << std::endl;
         std::cout << "  Matrix norm: " << std::fixed << std::setprecision(3) << matrix.norm() << std::endl;
         std::cout << "  Distance from identity: " << std::fixed << std::setprecision(3) << distanceFromIdentity << std::endl;
         std::cout << "  Quality score: " << std::scientific << std::setprecision(3) << qualityScore << std::endl;
         std::cout << "  Simplicity score: " << std::fixed << std::setprecision(3) << simplicityScore << std::endl;
         std::cout << "  Matrix: [";
         for (int j = 0; j < 9; ++j) {
            std::cout << std::fixed << std::setprecision(0) << matrix[j];
            if (j < 8) std::cout << " ";
         }
         std::cout << "]" << std::endl;
         std::cout << std::endl;
      }
   }

   // Get scored results for detailed analysis
   static std::vector<ScoredResult> getScoredResults(
      const std::vector<LatticeMatchResult>& results,
      const LRL_Cell& referenceCell,
      double qualityToleranceRatio = 0.1) {

      std::vector<ScoredResult> scoredResults;

      if (results.empty()) return scoredResults;

      // Find best quality score
      double bestQualityScore = std::numeric_limits<double>::max();
      for (const auto& result : results) {
         double qualityScore = calculateQualityScore(result, referenceCell);
         bestQualityScore = std::min(bestQualityScore, qualityScore);
      }

      double qualityTolerance = bestQualityScore * qualityToleranceRatio;

      // Score all results
      for (const auto& result : results) {
         ScoredResult scored;
         scored.result = result;
         scored.qualityScore = calculateQualityScore(result, referenceCell);
         scored.simplicityScore = calculateSimplicityScore(result.getTransformationMatrix());
         scored.distanceFromIdentity = calculateDistanceFromIdentity(result.getTransformationMatrix());
         scored.category = categorizeMatrix(result.getTransformationMatrix());

         // Quality-aware composite scoring
         if (scored.qualityScore <= bestQualityScore + qualityTolerance) {
            scored.compositeScore = scored.qualityScore + 0.1 * scored.simplicityScore;
         }
         else {
            scored.compositeScore = scored.qualityScore + 0.01 * scored.simplicityScore;
         }

         scoredResults.push_back(scored);
      }

      return scoredResults;
   }

private:
   static double calculateQualityScore(const LatticeMatchResult& result, const LRL_Cell& referenceCell) {
      // Use only P3 distance for quality - S6 angle is for display only
      return result.getP3Distance();
   }


   static std::string categorizeMatrix(const Matrix_3x3& matrix) {
      if (isIdentityMatrix(matrix)) return "Identity";
      if (isDiagonalMatrix(matrix)) return "Diagonal";
      if (isPermutationMatrix(matrix)) return "Permutation";
      if (hasOnlyIntegerElements(matrix)) return "Integer";
      return "General";
   }

   static bool isIdentityMatrix(const Matrix_3x3& matrix, double tolerance = 1e-6) {
      for (int i = 0; i < 3; ++i) {
         for (int j = 0; j < 3; ++j) {
            double expected = (i == j) ? 1.0 : 0.0;
            if (std::abs(matrix[i * 3 + j] - expected) > tolerance) {
               return false;
            }
         }
      }
      return true;
   }

   static bool isDiagonalMatrix(const Matrix_3x3& matrix, double tolerance = 1e-6) {
      for (int i = 0; i < 3; ++i) {
         for (int j = 0; j < 3; ++j) {
            if (i != j && std::abs(matrix[i * 3 + j]) > tolerance) {
               return false;
            }
         }
      }
      return true;
   }

   static bool isPermutationMatrix(const Matrix_3x3& matrix, double tolerance = 1e-6) {
      // Each row and column should have exactly one ±1 and rest zeros
      for (int i = 0; i < 3; ++i) {
         int nonZeroCount = 0;
         for (int j = 0; j < 3; ++j) {
            double element = matrix[i * 3 + j];
            if (std::abs(element) > tolerance) {
               nonZeroCount++;
               if (std::abs(std::abs(element) - 1.0) > tolerance) {
                  return false;
               }
            }
         }
         if (nonZeroCount != 1) return false;
      }

      // Check columns
      for (int j = 0; j < 3; ++j) {
         int nonZeroCount = 0;
         for (int i = 0; i < 3; ++i) {
            if (std::abs(matrix[i * 3 + j]) > tolerance) {
               nonZeroCount++;
            }
         }
         if (nonZeroCount != 1) return false;
      }

      return true;
   }

   static bool hasOnlyIntegerElements(const Matrix_3x3& matrix, double tolerance = 1e-6) {
      for (int i = 0; i < 9; ++i) {
         if (std::abs(matrix[i] - std::round(matrix[i])) > tolerance) {
            return false;
         }
      }
      return true;
   }

   static int countNonZeroOffDiagonal(const Matrix_3x3& matrix, double tolerance = 1e-6) {
      int count = 0;
      for (int i = 0; i < 3; ++i) {
         for (int j = 0; j < 3; ++j) {
            if (i != j && std::abs(matrix[i * 3 + j]) > tolerance) {
               count++;
            }
         }
      }
      return count;
   }
};

#endif // MATRIX_SIMPLICITY_RANKER_H