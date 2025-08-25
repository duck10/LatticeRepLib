#ifndef P3_SVD_PROJECTOR_BUILDER_H
#define P3_SVD_PROJECTOR_BUILDER_H

#include "svd.h"
#include "P3.h"
#include "P3_Reduce.h"
#include "EnhancedBoundaryDetector.h"
#include "P3Utilities.h"
#include "Polar.h"
#include <vector>
#include <iostream>
#include <iomanip>

class P3SVDProjectorBuilder {
private:
   // Convert P3 to 6D vector for SVD
   static std::vector<double> P3ToVector(const P3& p3) {
      return { p3[0].first, p3[0].second, p3[1].first,
              p3[1].second, p3[2].first, p3[2].second };
   }

   // Convert 6D vector back to P3
   static P3 VectorToP3(const std::vector<double>& v) {
      return P3(v[0], v[1], v[2], v[3], v[4], v[5]);
   }

   // NEW: Generate TRUE P3 boundary examples where constraints are exactly satisfied
   static std::vector<P3> GenerateOnBoundaryExamples(const std::string& boundaryType, int numExamples = 50) {
      std::vector<P3> examples;

      std::cout << "Generating " << numExamples << " TRUE P3 boundary examples for " << boundaryType << "..." << std::endl;

      if (boundaryType == "single_angle_p2_p3_90deg") {
         // Generate P3s where p2·p3 = 0 exactly
         for (int i = 0; i < numExamples; ++i) {
            // Generate p1 arbitrarily
            P3::Vec2 p1 = { 1.0 + 2.0 * (double)rand() / RAND_MAX,
                           0.5 + 1.0 * (double)rand() / RAND_MAX };

            // Generate p2 arbitrarily  
            P3::Vec2 p2 = { 1.0 + 2.0 * (double)rand() / RAND_MAX,
                           0.5 + 1.0 * (double)rand() / RAND_MAX };

            // Generate p3 such that p2·p3 = 0
            // If p2 = (a,b) and p3 = (x,y), then p2·p3 = ax + by = 0
            // So y = -ax/b (if b ≠ 0), or x = 0 (if b = 0)
            P3::Vec2 p3;
            if (std::abs(p2.second) > 1e-10) {
               p3.first = 1.0 + 1.0 * (double)rand() / RAND_MAX;  // Choose x freely
               p3.second = -(p2.first * p3.first) / p2.second;    // y = -ax/b
            } else {
               p3.first = 0.0;  // Force x = 0 if b = 0
               p3.second = 1.0 + 1.0 * (double)rand() / RAND_MAX; // Choose y freely
            }

            P3 example(p1.first, p1.second, p2.first, p2.second, p3.first, p3.second);

            // Apply StandardPresentation for consistency
            P3 standardized = P3_Reduce::StandardPresentation(example);
            examples.push_back(standardized);

            if (i < 5) {  // Show details for first 5
               double dot_product = standardized[1].first * standardized[2].first +
                  standardized[1].second * standardized[2].second;
               std::cout << "  Example " << (i + 1) << ": p2·p3 = "
                  << std::scientific << std::setprecision(6) << dot_product << std::endl;
            }
         }

      } else if (boundaryType == "single_angle_p1_p3_90deg") {
         // Generate P3s where p1·p3 = 0 exactly
         for (int i = 0; i < numExamples; ++i) {
            P3::Vec2 p2 = { 1.0 + 2.0 * (double)rand() / RAND_MAX,
                           0.5 + 1.0 * (double)rand() / RAND_MAX };
            P3::Vec2 p1 = { 1.0 + 2.0 * (double)rand() / RAND_MAX,
                           0.5 + 1.0 * (double)rand() / RAND_MAX };

            P3::Vec2 p3;
            if (std::abs(p1.second) > 1e-10) {
               p3.first = 1.0 + 1.0 * (double)rand() / RAND_MAX;
               p3.second = -(p1.first * p3.first) / p1.second;
            } else {
               p3.first = 0.0;
               p3.second = 1.0 + 1.0 * (double)rand() / RAND_MAX;
            }

            P3 example(p1.first, p1.second, p2.first, p2.second, p3.first, p3.second);
            P3 standardized = P3_Reduce::StandardPresentation(example);
            examples.push_back(standardized);
         }

      } else if (boundaryType == "single_mag_p1_eq_p2") {
         // Generate P3s where |p1| = |p2| exactly
         for (int i = 0; i < numExamples; ++i) {
            P3::Vec2 p1 = { 1.0 + 2.0 * (double)rand() / RAND_MAX,
                           0.5 + 1.0 * (double)rand() / RAND_MAX };

            // Generate p2 with same magnitude as p1
            double mag1 = std::sqrt(p1.first * p1.first + p1.second * p1.second);
            double angle2 = 2.0 * 3.14159265359 * (double)rand() / RAND_MAX;  // Random angle
            P3::Vec2 p2 = { mag1 * cos(angle2), mag1 * sin(angle2) };

            P3::Vec2 p3 = { 0.5 + 1.5 * (double)rand() / RAND_MAX,
                           0.5 + 1.5 * (double)rand() / RAND_MAX };

            P3 example(p1.first, p1.second, p2.first, p2.second, p3.first, p3.second);
            P3 standardized = P3_Reduce::StandardPresentation(example);
            examples.push_back(standardized);
         }
      }

      std::cout << "Generated " << examples.size() << " standardized P3 boundary examples" << std::endl;
      return examples;
   }

public:
   // Build constraint matrix for the specific boundary type
   static std::vector<std::vector<double>> BuildConstraintMatrix(
      const std::vector<P3>& examples,
      const std::string& boundaryType) {

      std::vector<std::vector<double>> constraintMatrix;

      std::cout << "Building constraint matrix for " << boundaryType << " using " << examples.size() << " examples" << std::endl;

      if (boundaryType == "single_angle_p2_p3_90deg") {
         // For p2⊥p3: constraint is p2·p3 = 0
         // Gradient: ∂f/∂p = [0, 0, p3.x, p3.y, p2.x, p2.y]

         for (size_t idx = 0; idx < examples.size(); ++idx) {
            const P3& p3_boundary = examples[idx];

            double constraint_value = p3_boundary[1].first * p3_boundary[2].first +
               p3_boundary[1].second * p3_boundary[2].second;

            std::vector<double> constraint_row(6, 0.0);
            constraint_row[0] = 0.0;  // ∂f/∂(p1.x) = 0
            constraint_row[1] = 0.0;  // ∂f/∂(p1.y) = 0
            constraint_row[2] = p3_boundary[2].first;   // ∂f/∂(p2.x) = p3.x
            constraint_row[3] = p3_boundary[2].second;  // ∂f/∂(p2.y) = p3.y
            constraint_row[4] = p3_boundary[1].first;   // ∂f/∂(p3.x) = p2.x
            constraint_row[5] = p3_boundary[1].second;  // ∂f/∂(p3.y) = p2.y

            // Normalize the constraint to avoid numerical issues
            double norm = 0.0;
            for (double coeff : constraint_row) {
               norm += coeff * coeff;
            }
            norm = std::sqrt(norm);

            if (norm > 1e-10) {
               for (double& coeff : constraint_row) {
                  coeff /= norm;
               }
               constraintMatrix.push_back(constraint_row);

               if (idx < 10) {
                  std::cout << "  Example " << (idx + 1) << ": constraint = "
                     << std::scientific << std::setprecision(6) << constraint_value << std::endl;
               }
            }
         }

      } else if (boundaryType == "single_angle_p1_p3_90deg") {
         for (const P3& p3_boundary : examples) {
            std::vector<double> constraint_row(6, 0.0);
            constraint_row[0] = p3_boundary[2].first;   // ∂f/∂(p1.x) = p3.x
            constraint_row[1] = p3_boundary[2].second;  // ∂f/∂(p1.y) = p3.y
            constraint_row[2] = 0.0;  // ∂f/∂(p2.x) = 0
            constraint_row[3] = 0.0;  // ∂f/∂(p2.y) = 0
            constraint_row[4] = p3_boundary[0].first;   // ∂f/∂(p3.x) = p1.x
            constraint_row[5] = p3_boundary[0].second;  // ∂f/∂(p3.y) = p1.y

            double norm = 0.0;
            for (double coeff : constraint_row) {
               norm += coeff * coeff;
            }
            norm = std::sqrt(norm);

            if (norm > 1e-10) {
               for (double& coeff : constraint_row) {
                  coeff /= norm;
               }
               constraintMatrix.push_back(constraint_row);
            }
         }

      } else if (boundaryType == "single_mag_p1_eq_p2") {
         for (const P3& p3_boundary : examples) {
            std::vector<double> constraint_row(6, 0.0);
            constraint_row[0] = 2.0 * p3_boundary[0].first;   // ∂f/∂(p1.x) = 2*p1.x
            constraint_row[1] = 2.0 * p3_boundary[0].second;  // ∂f/∂(p1.y) = 2*p1.y
            constraint_row[2] = -2.0 * p3_boundary[1].first;  // ∂f/∂(p2.x) = -2*p2.x
            constraint_row[3] = -2.0 * p3_boundary[1].second; // ∂f/∂(p2.y) = -2*p2.y
            constraint_row[4] = 0.0;  // ∂f/∂(p3.x) = 0
            constraint_row[5] = 0.0;  // ∂f/∂(p3.y) = 0

            double norm = 0.0;
            for (double coeff : constraint_row) {
               norm += coeff * coeff;
            }
            norm = std::sqrt(norm);

            if (norm > 1e-10) {
               for (double& coeff : constraint_row) {
                  coeff /= norm;
               }
               constraintMatrix.push_back(constraint_row);
            }
         }
      }

      std::cout << "Built constraint matrix: " << constraintMatrix.size()
         << " × " << (constraintMatrix.empty() ? 0 : constraintMatrix[0].size()) << std::endl;

      // Remove duplicate/similar constraints
      std::vector<std::vector<double>> uniqueConstraints;
      for (const auto& constraint : constraintMatrix) {
         bool isDuplicate = false;
         for (const auto& unique : uniqueConstraints) {
            double dotProduct = 0.0;
            for (size_t i = 0; i < 6; ++i) {
               dotProduct += constraint[i] * unique[i];
            }
            if (std::abs(std::abs(dotProduct) - 1.0) < 1e-6) { // Nearly parallel
               isDuplicate = true;
               break;
            }
         }
         if (!isDuplicate) {
            uniqueConstraints.push_back(constraint);
         }
      }

      std::cout << "After removing duplicates: " << uniqueConstraints.size() << " unique constraints" << std::endl;
      return uniqueConstraints;
   }

   // Build projection matrix using SVD of constraint matrix
   static std::vector<std::vector<double>> BuildProjectionMatrix(
      const std::vector<std::vector<double>>& constraintMatrix) {

      if (constraintMatrix.empty()) {
         std::cout << "Empty constraint matrix" << std::endl;
         return {};
      }

      const size_t numConstraints = constraintMatrix.size();
      const size_t dimension = constraintMatrix[0].size();  // Should be 6

      std::cout << "Performing SVD on " << numConstraints << " × " << dimension << " constraint matrix" << std::endl;

      // Prepare constraint matrix for SVD (transpose for svdcmp)
      std::vector<std::vector<double>> A(dimension, std::vector<double>(numConstraints));
      for (size_t i = 0; i < numConstraints; ++i) {
         for (size_t j = 0; j < dimension; ++j) {
            A[j][i] = constraintMatrix[i][j];  // Transpose
         }
      }

      std::vector<double> w(numConstraints);  // Singular values
      std::vector<std::vector<double>> V(numConstraints, std::vector<double>(numConstraints));  // Right singular vectors

      // Perform SVD: A = U * W * V^T
      svdcmp(A, w, V);

      // Print singular values
      std::cout << "Singular values: ";
      for (size_t i = 0; i < std::min(w.size(), (size_t)6); ++i) {
         std::cout << std::scientific << std::setprecision(3) << w[i] << " ";
      }
      std::cout << std::endl;

      // Find effective rank (number of significant singular values)
      double maxSingularValue = *std::max_element(w.begin(), w.end());
      const double tolerance = std::max(1e-10, maxSingularValue * 1e-8);
      int rank = 0;
      for (size_t i = 0; i < w.size(); ++i) {
         if (w[i] > tolerance) {
            rank++;
         }
      }

      std::cout << "Effective rank: " << rank << " / " << w.size() << std::endl;
      std::cout << "Null space dimension: " << (dimension - rank) << std::endl;

      if (rank == 0) {
         std::cout << "ERROR: No significant constraints found!" << std::endl;
         return {};
      }

      // Use only the most significant constraint direction
      int effectiveRank = std::min(rank, 1);  // Use only the TOP constraint

      std::cout << "Using effective rank: " << effectiveRank << std::endl;

      std::vector<std::vector<double>> projectionMatrix(dimension, std::vector<double>(dimension, 0.0));

      // Start with identity matrix
      for (size_t i = 0; i < dimension; ++i) {
         projectionMatrix[i][i] = 1.0;
      }

      // Subtract projection onto constraint space using only the most significant singular vectors
      for (int i = 0; i < effectiveRank; ++i) {
         if (w[i] > tolerance) {
            std::cout << "Using constraint " << i << " (σ = " << w[i] << ")" << std::endl;

            // Get the i-th left singular vector (column of U = A after SVD)
            std::vector<double> u_i(dimension);
            for (size_t j = 0; j < dimension; ++j) {
               u_i[j] = A[j][i];
            }

            // Subtract u_i ⊗ u_i from the projection matrix
            for (size_t row = 0; row < dimension; ++row) {
               for (size_t col = 0; col < dimension; ++col) {
                  projectionMatrix[row][col] -= u_i[row] * u_i[col];
               }
            }
         }
      }

      // Verify projection matrix properties
      double trace = 0.0;
      for (size_t i = 0; i < dimension; ++i) {
         trace += projectionMatrix[i][i];
      }

      std::cout << "Projection matrix trace: " << std::fixed << std::setprecision(4)
         << trace << " (should be " << (dimension - effectiveRank) << ")" << std::endl;

      return projectionMatrix;
   }

   // Apply the projection matrix to a P3, ensuring StandardPresentation
   static P3 ApplyProjectionMatrix(const P3& input,
      const std::vector<std::vector<double>>& projMatrix) {

      if (projMatrix.empty() || projMatrix.size() != 6 || projMatrix[0].size() != 6) {
         std::cout << "Invalid projection matrix dimensions" << std::endl;
         return input;
      }

      std::vector<double> v_input = P3ToVector(input);
      std::vector<double> v_output(6, 0.0);

      // Matrix multiplication: v_output = projMatrix * v_input
      for (size_t i = 0; i < 6; ++i) {
         for (size_t j = 0; j < 6; ++j) {
            v_output[i] += projMatrix[i][j] * v_input[j];
         }
      }

      P3 result = VectorToP3(v_output);
      return P3_Reduce::StandardPresentation(result);  // Always apply StandardPresentation
   }

   // Test orthogonality constraint for p2⊥p3 boundary
   static double TestOrthogonalityConstraint(const P3& p3, const std::string& boundaryType) {
      if (boundaryType == "single_angle_p2_p3_90deg") {
         return p3[1].first * p3[2].first + p3[1].second * p3[2].second;  // p2·p3
      } else if (boundaryType == "single_angle_p1_p3_90deg") {
         return p3[0].first * p3[2].first + p3[0].second * p3[2].second;  // p1·p3
      } else if (boundaryType == "single_angle_p1_p2_90deg") {
         return p3[0].first * p3[1].first + p3[0].second * p3[1].second;  // p1·p2
      } else if (boundaryType == "single_mag_p1_eq_p2") {
         double mag1 = std::sqrt(p3[0].first * p3[0].first + p3[0].second * p3[0].second);
         double mag2 = std::sqrt(p3[1].first * p3[1].first + p3[1].second * p3[1].second);
         return mag1 - mag2;  // |p1| - |p2|
      }
      return 0.0;
   }

   // MAIN: Complete workflow using true P3 boundary examples
   static void BuildAndTestProjector(const std::string& boundaryType) {
      std::cout << "\n=== Building SVD-Based Projector for " << boundaryType << " using TRUE P3 Boundaries ===" << std::endl;

      // Step 1: Generate true boundary examples 
      auto boundaryExamples = GenerateOnBoundaryExamples(boundaryType, 30);

      if (boundaryExamples.size() < 10) {
         std::cout << "Not enough boundary examples found - aborting" << std::endl;
         return;
      }

      // Step 2: Build constraint matrix
      auto constraintMatrix = BuildConstraintMatrix(boundaryExamples, boundaryType);
      if (constraintMatrix.empty()) {
         std::cout << "Failed to build constraint matrix" << std::endl;
         return;
      }

      // Step 3: Build projection matrix
      auto projMatrix = BuildProjectionMatrix(constraintMatrix);
      if (projMatrix.empty()) {
         std::cout << "Failed to build projection matrix" << std::endl;
         return;
      }

      // Step 3.5: Display the projection matrix
      std::cout << "\n--- 6×6 Projection Matrix ---" << std::endl;
      for (size_t i = 0; i < 6; ++i) {
         std::cout << "  [";
         for (size_t j = 0; j < 6; ++j) {
            std::cout << std::setw(8) << std::fixed << std::setprecision(4) << projMatrix[i][j];
            if (j < 5) std::cout << ", ";
         }
         std::cout << "]" << std::endl;
      }

      // Show significant elements only
      std::cout << "\n--- Significant Matrix Elements (|value| > 0.01) ---" << std::endl;
      for (size_t i = 0; i < 6; ++i) {
         for (size_t j = 0; j < 6; ++j) {
            if (std::abs(projMatrix[i][j]) > 0.01) {
               std::cout << "  P[" << i << "," << j << "] = "
                  << std::setw(8) << std::fixed << std::setprecision(4)
                  << projMatrix[i][j] << std::endl;
            }
         }
      }

      // Step 4: Test the projector
      std::cout << "\n--- Testing Projector on Pure P3 Constraints ---" << std::endl;

      int improvedCount = 0;
      double totalImprovementRatio = 0.0;

      for (int test = 0; test < 10; ++test) {
         P3 testInput(Polar::rand());
         P3 testInputStd = P3_Reduce::StandardPresentation(testInput);

         P3 projected = ApplyProjectionMatrix(testInputStd, projMatrix);

         // Test constraint satisfaction
         double constraintBefore = std::abs(TestOrthogonalityConstraint(testInputStd, boundaryType));
         double constraintAfter = std::abs(TestOrthogonalityConstraint(projected, boundaryType));

         bool improved = constraintAfter < constraintBefore;
         if (improved) {
            improvedCount++;
            if (constraintBefore > 1e-10) {
               totalImprovementRatio += constraintAfter / constraintBefore;
            }
         }

         std::cout << "Test " << (test + 1) << ": "
            << std::scientific << std::setprecision(3) << constraintBefore
            << " → " << constraintAfter;
         if (improved) std::cout << " ✓";
         std::cout << std::endl;
      }

      std::cout << "\nSummary: " << improvedCount << "/10 improved P3 constraints" << std::endl;
      if (improvedCount > 0) {
         double avgRatio = totalImprovementRatio / improvedCount;
         std::cout << "Average improvement ratio: " << std::fixed << std::setprecision(3) << avgRatio << std::endl;
      }

      // Step 5: Verify training data quality
      std::cout << "\n--- Training Data Quality Check ---" << std::endl;
      double avgConstraint = 0.0;
      for (size_t i = 0; i < std::min(boundaryExamples.size(), (size_t)10); ++i) {
         double constraint = std::abs(TestOrthogonalityConstraint(boundaryExamples[i], boundaryType));
         avgConstraint += constraint;
         if (i < 5) {
            std::cout << "Training example " << (i + 1) << ": constraint = "
               << std::scientific << std::setprecision(3) << constraint << std::endl;
         }
      }
      avgConstraint /= std::min(boundaryExamples.size(), (size_t)10);
      std::cout << "Average training constraint: " << std::scientific << std::setprecision(3) << avgConstraint << std::endl;

      if (avgConstraint < 1e-6) {
         std::cout << "✓ Excellent training data quality" << std::endl;
      } else if (avgConstraint < 1e-3) {
         std::cout << "✓ Good training data quality" << std::endl;
      } else {
         std::cout << "⚠ Training data may need improvement" << std::endl;
      }
   }
};

#endif // P3_SVD_PROJECTOR_BUILDER_H

