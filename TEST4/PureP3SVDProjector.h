#ifndef PURE_P3_SVD_PROJECTOR_H
#define PURE_P3_SVD_PROJECTOR_H

#include "svd.h"
#include "P3.h"
#include "P3_Reduce.h"
#include "Polar.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>

static const double M_PI = 4.0 * atan(1.0);
static const double M_PI_2 = M_PI / 2.0;    // π/2 ≈ 1.5708 (90 degrees)
static const double M_PI_4 = M_PI / 4.0;    // π/4 ≈ 0.7854 (45 degrees)

class PureP3SVDProjector {
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

public:
   // Generate TRUE P3 boundary examples where p2⊥p3 (p2·p3 = 0)
   static std::vector<P3> GenerateP2OrthP3Examples(int numExamples = 50) {
      std::vector<P3> examples;

      std::cout << "Generating " << numExamples << " TRUE P3 examples where p2⊥p3..." << std::endl;

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

         // Apply StandardPresentation to ensure consistent representation
         P3 standardized = P3_Reduce::StandardPresentation(example);

         // Verify the constraint is satisfied in standardized form
         double dot_product = standardized[1].first * standardized[2].first +
            standardized[1].second * standardized[2].second;

         examples.push_back(standardized);

         if (i < 10) {  // Show details for first 10
            std::cout << "  Example " << (i + 1) << ": p2·p3 = "
               << std::fixed << std::setprecision(6) << dot_product << std::endl;
         }
      }

      std::cout << "Generated " << examples.size() << " standardized P3 boundary examples" << std::endl;
      return examples;
   }

   // Generate TRUE P3 boundary examples where p1⊥p3 (p1·p3 = 0)
   static std::vector<P3> GenerateP1OrthP3Examples(int numExamples = 50) {
      std::vector<P3> examples;

      std::cout << "Generating " << numExamples << " TRUE P3 examples where p1⊥p3..." << std::endl;

      for (int i = 0; i < numExamples; ++i) {
         // Generate p2 arbitrarily
         P3::Vec2 p2 = { 1.0 + 2.0 * (double)rand() / RAND_MAX,
                        0.5 + 1.0 * (double)rand() / RAND_MAX };

         // Generate p1 arbitrarily  
         P3::Vec2 p1 = { 1.0 + 2.0 * (double)rand() / RAND_MAX,
                        0.5 + 1.0 * (double)rand() / RAND_MAX };

         // Generate p3 such that p1·p3 = 0
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

         double dot_product = standardized[0].first * standardized[2].first +
            standardized[0].second * standardized[2].second;

         examples.push_back(standardized);

         if (i < 10) {
            std::cout << "  Example " << (i + 1) << ": p1·p3 = "
               << std::fixed << std::setprecision(6) << dot_product << std::endl;
         }
      }

      std::cout << "Generated " << examples.size() << " standardized P3 boundary examples" << std::endl;
      return examples;
   }

   // Generate TRUE P3 boundary examples where |p1| = |p2|
   static std::vector<P3> GenerateMagP1EqP2Examples(int numExamples = 50) {
      std::vector<P3> examples;

      std::cout << "Generating " << numExamples << " TRUE P3 examples where |p1| = |p2|..." << std::endl;

      for (int i = 0; i < numExamples; ++i) {
         // Generate p1 arbitrarily
         P3::Vec2 p1 = { 1.0 + 2.0 * (double)rand() / RAND_MAX,
                        0.5 + 1.0 * (double)rand() / RAND_MAX };

         // Generate p2 with same magnitude as p1
         double mag1 = std::sqrt(p1.first * p1.first + p1.second * p1.second);
         double angle2 = 2.0 * M_PI * (double)rand() / RAND_MAX;  // Random angle
         P3::Vec2 p2 = { mag1 * cos(angle2), mag1 * sin(angle2) };

         // Generate p3 arbitrarily (different magnitude)
         P3::Vec2 p3 = { 0.5 + 1.5 * (double)rand() / RAND_MAX,
                        0.5 + 1.5 * (double)rand() / RAND_MAX };

         P3 example(p1.first, p1.second, p2.first, p2.second, p3.first, p3.second);
         P3 standardized = P3_Reduce::StandardPresentation(example);

         double mag1_std = std::sqrt(standardized[0].first * standardized[0].first +
            standardized[0].second * standardized[0].second);
         double mag2_std = std::sqrt(standardized[1].first * standardized[1].first +
            standardized[1].second * standardized[1].second);

         examples.push_back(standardized);

         if (i < 10) {
            std::cout << "  Example " << (i + 1) << ": |p1| = " << std::fixed << std::setprecision(3)
               << mag1_std << ", |p2| = " << mag2_std
               << ", diff = " << std::abs(mag1_std - mag2_std) << std::endl;
         }
      }

      std::cout << "Generated " << examples.size() << " standardized P3 boundary examples" << std::endl;
      return examples;
   }

   // Build constraint matrix for P3 orthogonality boundary
   static std::vector<std::vector<double>> BuildConstraintMatrix(
      const std::vector<P3>& examples,
      const std::string& boundaryType) {

      std::vector<std::vector<double>> constraintMatrix;

      std::cout << "Building constraint matrix for " << boundaryType << " using " << examples.size() << " examples" << std::endl;

      if (boundaryType == "p2_orth_p3") {
         // For p2⊥p3: constraint is p2·p3 = 0
         // Gradient: ∂f/∂[p1.x, p1.y, p2.x, p2.y, p3.x, p3.y] = [0, 0, p3.x, p3.y, p2.x, p2.y]

         for (size_t idx = 0; idx < examples.size(); ++idx) {
            const P3& p3_boundary = examples[idx];

            // Check constraint satisfaction
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
                     << std::fixed << std::setprecision(6) << constraint_value << std::endl;
               }
            }
         }

      } else if (boundaryType == "p1_orth_p3") {
         // For p1⊥p3: constraint is p1·p3 = 0
         for (const P3& p3_boundary : examples) {
            std::vector<double> constraint_row(6, 0.0);
            constraint_row[0] = p3_boundary[2].first;   // ∂f/∂(p1.x) = p3.x
            constraint_row[1] = p3_boundary[2].second;  // ∂f/∂(p1.y) = p3.y
            constraint_row[2] = 0.0;  // ∂f/∂(p2.x) = 0
            constraint_row[3] = 0.0;  // ∂f/∂(p2.y) = 0
            constraint_row[4] = p3_boundary[0].first;   // ∂f/∂(p3.x) = p1.x
            constraint_row[5] = p3_boundary[0].second;  // ∂f/∂(p3.y) = p1.y

            // Normalize
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

      } else if (boundaryType == "mag_p1_eq_p2") {
         // For |p1| = |p2|: constraint is |p1|² - |p2|² = 0
         // Let f = p1.x² + p1.y² - p2.x² - p2.y²
         // Gradient: [2*p1.x, 2*p1.y, -2*p2.x, -2*p2.y, 0, 0]

         for (const P3& p3_boundary : examples) {
            std::vector<double> constraint_row(6, 0.0);
            constraint_row[0] = 2.0 * p3_boundary[0].first;   // ∂f/∂(p1.x) = 2*p1.x
            constraint_row[1] = 2.0 * p3_boundary[0].second;  // ∂f/∂(p1.y) = 2*p1.y
            constraint_row[2] = -2.0 * p3_boundary[1].first;  // ∂f/∂(p2.x) = -2*p2.x
            constraint_row[3] = -2.0 * p3_boundary[1].second; // ∂f/∂(p2.y) = -2*p2.y
            constraint_row[4] = 0.0;  // ∂f/∂(p3.x) = 0
            constraint_row[5] = 0.0;  // ∂f/∂(p3.y) = 0

            // Normalize
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

   // Build projection matrix using SVD
   static std::vector<std::vector<double>> BuildProjectionMatrix(
      const std::vector<std::vector<double>>& constraintMatrix) {

      if (constraintMatrix.empty()) {
         std::cout << "Empty constraint matrix" << std::endl;
         return {};
      }

      const size_t numConstraints = constraintMatrix.size();
      const size_t dimension = constraintMatrix[0].size();  // Should be 6

      std::cout << "Performing SVD on " << numConstraints << " × " << dimension << " constraint matrix" << std::endl;

      // Prepare constraint matrix for SVD
      std::vector<std::vector<double>> A(dimension, std::vector<double>(numConstraints));
      for (size_t i = 0; i < numConstraints; ++i) {
         for (size_t j = 0; j < dimension; ++j) {
            A[j][i] = constraintMatrix[i][j];  // Transpose
         }
      }

      std::vector<double> w(numConstraints);
      std::vector<std::vector<double>> V(numConstraints, std::vector<double>(numConstraints));

      // Perform SVD
      svdcmp(A, w, V);

      // Print singular values
      std::cout << "Singular values: ";
      for (size_t i = 0; i < std::min(w.size(), (size_t)6); ++i) {
         std::cout << std::scientific << std::setprecision(3) << w[i] << " ";
      }
      std::cout << std::endl;

      // Find effective rank
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

      // Use only the most significant constraint
      int effectiveRank = std::min(rank, 1);
      std::cout << "Using effective rank: " << effectiveRank << std::endl;

      std::vector<std::vector<double>> projectionMatrix(dimension, std::vector<double>(dimension, 0.0));

      // Start with identity matrix
      for (size_t i = 0; i < dimension; ++i) {
         projectionMatrix[i][i] = 1.0;
      }

      // Subtract projection onto constraint space
      for (int i = 0; i < effectiveRank; ++i) {
         if (w[i] > tolerance) {
            std::cout << "Using constraint " << i << " (σ = " << w[i] << ")" << std::endl;

            // Get the i-th left singular vector
            std::vector<double> u_i(dimension);
            for (size_t j = 0; j < dimension; ++j) {
               u_i[j] = A[j][i];
            }

            // Subtract u_i ⊗ u_i from projection matrix
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

   // Apply projection with validity checking
   static P3 ProjectP3(const P3& input, const std::vector<std::vector<double>>& projMatrix) {
      if (projMatrix.empty()) return input;

      std::vector<double> v_input = P3ToVector(input);
      std::vector<double> v_projected(6, 0.0);

      // Apply projection matrix
      for (size_t i = 0; i < 6; ++i) {
         for (size_t j = 0; j < 6; ++j) {
            v_projected[i] += projMatrix[i][j] * v_input[j];
         }
      }

      P3 result = VectorToP3(v_projected);
      return P3_Reduce::StandardPresentation(result);  // Always apply StandardPresentation
   }

   // Test orthogonality constraint
   static double TestConstraint(const P3& p3, const std::string& boundaryType) {
      if (boundaryType == "p2_orth_p3") {
         return p3[1].first * p3[2].first + p3[1].second * p3[2].second;  // p2·p3
      } else if (boundaryType == "p1_orth_p3") {
         return p3[0].first * p3[2].first + p3[0].second * p3[2].second;  // p1·p3
      } else if (boundaryType == "mag_p1_eq_p2") {
         double mag1 = std::sqrt(p3[0].first * p3[0].first + p3[0].second * p3[0].second);
         double mag2 = std::sqrt(p3[1].first * p3[1].first + p3[1].second * p3[1].second);
         return mag1 - mag2;  // |p1| - |p2|
      }
      return 0.0;
   }

   // MAIN: Build and test pure P3 projector
   static void BuildAndTestPureP3Projector(const std::string& boundaryType) {
      std::cout << "\n" << std::string(80, '=') << std::endl;
      std::cout << "BUILDING PURE P3 SVD PROJECTOR FOR " << boundaryType << std::endl;
      std::cout << std::string(80, '=') << std::endl;

      // Step 1: Generate TRUE boundary examples
      std::vector<P3> boundaryExamples;
      if (boundaryType == "p2_orth_p3") {
         boundaryExamples = GenerateP2OrthP3Examples(30);
      } else if (boundaryType == "p1_orth_p3") {
         boundaryExamples = GenerateP1OrthP3Examples(30);
      } else if (boundaryType == "mag_p1_eq_p2") {
         boundaryExamples = GenerateMagP1EqP2Examples(30);
      } else {
         std::cout << "Unknown boundary type: " << boundaryType << std::endl;
         return;
      }

      if (boundaryExamples.size() < 10) {
         std::cout << "Not enough boundary examples generated" << std::endl;
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

      // Step 4: Test the projector
      std::cout << "\n--- Testing Pure P3 Projector ---" << std::endl;

      int improvedCount = 0;
      double totalImprovementRatio = 0.0;

      for (int test = 0; test < 10; ++test) {
         P3 testInput(Polar::rand());
         P3 testInputStd = P3_Reduce::StandardPresentation(testInput);

         P3 projected = ProjectP3(testInputStd, projMatrix);

         // Test constraint satisfaction
         double constraintBefore = std::abs(TestConstraint(testInputStd, boundaryType));
         double constraintAfter = std::abs(TestConstraint(projected, boundaryType));

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
      std::cout << "\n--- Verifying Training Data Quality ---" << std::endl;
      double avgConstraint = 0.0;
      for (size_t i = 0; i < std::min(boundaryExamples.size(), (size_t)10); ++i) {
         double constraint = std::abs(TestConstraint(boundaryExamples[i], boundaryType));
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

#endif // PURE_P3_SVD_PROJECTOR_H
