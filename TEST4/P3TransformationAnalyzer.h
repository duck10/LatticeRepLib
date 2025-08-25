#ifndef P3_TRANSFORMATION_H
#define P3_TRANSFORMATION_H

#include "P3.h"
#include "MatMN.h"
#include "MatG6.h"
#include "VecN.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>

class P3TransformationAnalyzer {
private:
   // Helper function to set matrix element using linear indexing
   static void SetMatrixElement(MatMN& mat, size_t row, size_t col, double value) {
      size_t cols = mat.GetColDim();
      mat[row * cols + col] = value;
   }

   // Helper function to get matrix element using linear indexing
   static double GetMatrixElement(const MatMN& mat, size_t row, size_t col) {
      size_t cols = mat.GetColDim();
      return mat[row * cols + col];
   }

public:
   struct TransformationResult {
      MatMN transformMatrix;    // 6x6 transformation matrix
      MatMN projectorMatrix;    // 6x6 projector to boundary
      double residual;
      bool isValidTransform;
      bool preservesVolume;     // NEW: Check if transformation preserves unit cell volume
      double volumeRatio;       // NEW: Ratio of volumes (should be ≈ 1.0)

      TransformationResult() : transformMatrix(6, 6), projectorMatrix(6, 6),
         residual(1e20), isValidTransform(false),
         preservesVolume(false), volumeRatio(0.0) {
      }
   };

   // Convert P3 to vector using basic arrays
   static std::vector<double> P3ToStdVector(const P3& p3) {
      std::vector<double> vec(6);
      vec[0] = p3[0].first;   // p1.x
      vec[1] = p3[0].second;  // p1.y
      vec[2] = p3[1].first;   // p2.x
      vec[3] = p3[1].second;  // p2.y
      vec[4] = p3[2].first;   // p3.x
      vec[5] = p3[2].second;  // p3.y
      return vec;
   }

   // Calculate unit cell volume from P3 using LRL_Cell
   static double CalculateUnitCellVolume(const P3& p3) {
      // Use the actual LRL_Cell volume calculation
      LRL_Cell cell(p3);
      return cell.Volume();
   }

   // Alternative method kept for reference (but use LRL_Cell method above)
   static double CalculateUnitCellVolumeFromMetric(const P3& p3) {
      // Extract the three 2D vectors
      const auto& v1 = p3[0];  // (x1, y1)
      const auto& v2 = p3[1];  // (x2, y2)  
      const auto& v3 = p3[2];  // (x3, y3)

      // Compute dot products (treating 2D vectors as projections)
      double g11 = v1.first * v1.first + v1.second * v1.second;     // |v1|²
      double g22 = v2.first * v2.first + v2.second * v2.second;     // |v2|²
      double g33 = v3.first * v3.first + v3.second * v3.second;     // |v3|²
      double g12 = v1.first * v2.first + v1.second * v2.second;     // v1·v2
      double g13 = v1.first * v3.first + v1.second * v3.second;     // v1·v3
      double g23 = v2.first * v3.first + v2.second * v3.second;     // v2·v3

      // Determinant of 3x3 metric tensor
      double det = g11 * (g22 * g33 - g23 * g23) - g12 * (g12 * g33 - g13 * g23) + g13 * (g12 * g23 - g13 * g22);

      // Volume = sqrt(det(G))
      return (det > 0) ? std::sqrt(det) : 0.0;
   }

   // Alternative: Calculate volume using the actual 3D unit cell
   static double CalculateUnitCellVolumeFrom3D(const P3& p3) {
      // Use the LRL_Cell volume method - this is the correct approach
      return CalculateUnitCellVolume(p3);
   }

   // Check if transformation preserves volume
   static void CheckVolumePreservation(TransformationResult& result,
      const P3& p3_before, const P3& p3_after) {
      double volumeBefore = CalculateUnitCellVolume(p3_before);
      double volumeAfter = CalculateUnitCellVolume(p3_after);

      if (volumeBefore > 1e-12) {
         result.volumeRatio = volumeAfter / volumeBefore;
         result.preservesVolume = std::abs(result.volumeRatio - 1.0) < 0.01; // 1% tolerance
      } else {
         result.volumeRatio = 0.0;
         result.preservesVolume = false;
      }
   }

   // Convert vector back to P3
   static P3 StdVectorToP3(const std::vector<double>& vec) {
      return P3(vec[0], vec[1], vec[2], vec[3], vec[4], vec[5]);
   }

   // Compute basic transformation matrix using outer product
   static TransformationResult ComputeTransformation(const P3& p3_before, const P3& p3_after) {
      TransformationResult result;

      std::vector<double> v_before = P3ToStdVector(p3_before);
      std::vector<double> v_after = P3ToStdVector(p3_after);

      // Compute ||v_before||²
      double norm_sq = 0.0;
      for (size_t i = 0; i < 6; ++i) {
         norm_sq += v_before[i] * v_before[i];
      }

      if (norm_sq > 1e-12) {
         // T = (v_after ⊗ v_before) / ||v_before||²
         for (size_t i = 0; i < 6; ++i) {
            for (size_t j = 0; j < 6; ++j) {
               SetMatrixElement(result.transformMatrix, i, j,
                  (v_after[i] * v_before[j]) / norm_sq);
            }
         }

         // Verify transformation: compute residual
         std::vector<double> transformed(6);
         for (size_t i = 0; i < 6; ++i) {
            transformed[i] = 0.0;
            for (size_t j = 0; j < 6; ++j) {
               transformed[i] += GetMatrixElement(result.transformMatrix, i, j) * v_before[j];
            }
         }

         result.residual = 0.0;
         for (size_t i = 0; i < 6; ++i) {
            double diff = transformed[i] - v_after[i];
            result.residual += diff * diff;
         }
         result.residual = sqrt(result.residual);
         result.isValidTransform = result.residual < 1e-6;

         // NEW: Check volume preservation
         CheckVolumePreservation(result, p3_before, p3_after);
      }

      return result;
   }

   // Compute projector to p2⊥p3 boundary at a specific point
   static MatMN ComputeOrthogonalityProjector(const P3& p3) {
      MatMN projector(6, 6);

      // Initialize as identity
      for (size_t i = 0; i < 6; ++i) {
         for (size_t j = 0; j < 6; ++j) {
            SetMatrixElement(projector, i, j, (i == j) ? 1.0 : 0.0);
         }
      }

      std::vector<double> v = P3ToStdVector(p3);

      // Constraint: g(v) = v[2]*v[4] + v[3]*v[5] = 0 (p2·p3 = 0)
      // Gradient: ∇g = [0, 0, v[4], v[5], v[2], v[3]]
      std::vector<double> grad(6);
      grad[0] = 0.0;     grad[1] = 0.0;
      grad[2] = v[4];    grad[3] = v[5];
      grad[4] = v[2];    grad[5] = v[3];

      double grad_norm_sq = 0.0;
      for (size_t i = 0; i < 6; ++i) {
         grad_norm_sq += grad[i] * grad[i];
      }

      if (grad_norm_sq > 1e-12) {
         // P = I - (∇g ⊗ ∇g) / ||∇g||²
         for (size_t i = 0; i < 6; ++i) {
            for (size_t j = 0; j < 6; ++j) {
               double oldVal = GetMatrixElement(projector, i, j);
               double newVal = oldVal - (grad[i] * grad[j]) / grad_norm_sq;
               SetMatrixElement(projector, i, j, newVal);
            }
         }
      }

      return projector;
   }

   // Project a P3 point to the p2⊥p3 boundary
   static P3 ProjectToOrthogonalityBoundary(const P3& p3) {
      std::vector<double> v = P3ToStdVector(p3);

      // Constraint: g(v) = v[2]*v[4] + v[3]*v[5] = 0
      double constraint_value = v[2] * v[4] + v[3] * v[5];

      if (std::abs(constraint_value) < 1e-12) {
         return p3; // Already on boundary
      }

      // Gradient: ∇g = [0, 0, v[4], v[5], v[2], v[3]]
      std::vector<double> grad(6);
      grad[0] = 0.0;     grad[1] = 0.0;
      grad[2] = v[4];    grad[3] = v[5];
      grad[4] = v[2];    grad[5] = v[3];

      double grad_norm_sq = 0.0;
      for (size_t i = 0; i < 6; ++i) {
         grad_norm_sq += grad[i] * grad[i];
      }

      if (grad_norm_sq < 1e-12) {
         return p3; // Gradient is zero
      }

      // Project: v_proj = v - (g(v) / ||∇g||²) * ∇g
      std::vector<double> v_projected(6);
      for (size_t i = 0; i < 6; ++i) {
         v_projected[i] = v[i] - (constraint_value / grad_norm_sq) * grad[i];
      }

      return StdVectorToP3(v_projected);
   }

   // Enhanced transformation that respects boundary constraints
   static TransformationResult ComputeConstrainedTransformation(
      const P3& p3_before, const P3& p3_after) {

      TransformationResult result;

      // Project both points to boundary
      P3 p3_before_proj = ProjectToOrthogonalityBoundary(p3_before);
      P3 p3_after_proj = ProjectToOrthogonalityBoundary(p3_after);

      // Compute basic transformation between projected points
      TransformationResult basic_result = ComputeTransformation(p3_before_proj, p3_after_proj);

      // Get projector at midpoint
      std::vector<double> v_before = P3ToStdVector(p3_before_proj);
      std::vector<double> v_after = P3ToStdVector(p3_after_proj);
      std::vector<double> v_mid(6);
      for (size_t i = 0; i < 6; ++i) {
         v_mid[i] = 0.5 * (v_before[i] + v_after[i]);
      }
      P3 midpoint = StdVectorToP3(v_mid);

      result.projectorMatrix = ComputeOrthogonalityProjector(midpoint);

      // Apply projector to transformation: T_constrained = P * T * P
      MatMN temp(6, 6);
      // temp = T * P
      for (size_t i = 0; i < 6; ++i) {
         for (size_t j = 0; j < 6; ++j) {
            double sum = 0.0;
            for (size_t k = 0; k < 6; ++k) {
               sum += GetMatrixElement(basic_result.transformMatrix, i, k) *
                  GetMatrixElement(result.projectorMatrix, k, j);
            }
            SetMatrixElement(temp, i, j, sum);
         }
      }

      // result.transformMatrix = P * temp
      for (size_t i = 0; i < 6; ++i) {
         for (size_t j = 0; j < 6; ++j) {
            double sum = 0.0;
            for (size_t k = 0; k < 6; ++k) {
               sum += GetMatrixElement(result.projectorMatrix, i, k) *
                  GetMatrixElement(temp, k, j);
            }
            SetMatrixElement(result.transformMatrix, i, j, sum);
         }
      }

      result.residual = basic_result.residual;
      result.isValidTransform = basic_result.isValidTransform;

      // NEW: Check volume preservation for constrained transformation
      CheckVolumePreservation(result, p3_before, p3_after);

      return result;
   }

   // Convert to G6 matrix format if needed for crystallographic applications
   static MatG6 ToMatG6(const MatMN& mat6x6) {
      if (mat6x6.GetRowDim() != 6 || mat6x6.GetColDim() != 6) {
         throw std::runtime_error("Matrix must be 6x6 to convert to MatG6");
      }

      std::vector<double> vec = mat6x6.GetVector();
      return MatG6(vec);
   }

   // Helper function to get transformation matrix for first pair of a boundary type
   static TransformationResult GetTransformationForBoundary(
      const StoreResults<std::string, PairReporter<P3, P3>>& store,
      const std::string& boundaryKey) {

      TransformationResult result;

      if (!store.HasKey(boundaryKey)) {
         std::cout << "Boundary key '" << boundaryKey << "' not found!" << std::endl;
         return result;
      }

      auto pairs = store.GetResult(boundaryKey);
      if (pairs.empty()) {
         std::cout << "No pairs found for boundary key '" << boundaryKey << "'" << std::endl;
         return result;
      }

      // Use first pair for transformation
      const P3& p3_before = pairs[0].second.GetFirst();
      const P3& p3_after = pairs[0].second.GetSecond();

      return ComputeConstrainedTransformation(p3_before, p3_after);
   }

   // Comprehensive analysis function
   static void AnalyzeBoundaryPair(const P3& p3_before, const P3& p3_after,
      const std::string& pairName = "") {

      std::cout << "=== P3 Boundary Transformation Analysis";
      if (!pairName.empty()) std::cout << " for " << pairName;
      std::cout << " ===" << std::endl;

      // Check orthogonality before and after
      double dot_before = p3_before[1].first * p3_before[2].first +
         p3_before[1].second * p3_before[2].second;
      double dot_after = p3_after[1].first * p3_after[2].first +
         p3_after[1].second * p3_after[2].second;

      std::cout << "p2·p3 before: " << dot_before << std::endl;
      std::cout << "p2·p3 after:  " << dot_after << std::endl;

      // Compute transformations
      auto basic_result = ComputeTransformation(p3_before, p3_after);
      auto constrained_result = ComputeConstrainedTransformation(p3_before, p3_after);

      std::cout << "\nBasic Transformation:" << std::endl;
      std::cout << "Residual: " << basic_result.residual << std::endl;
      std::cout << "Valid: " << (basic_result.isValidTransform ? "Yes" : "No") << std::endl;
      std::cout << "Volume Ratio: " << basic_result.volumeRatio << std::endl;
      std::cout << "Preserves Volume: " << (basic_result.preservesVolume ? "Yes" : "No") << std::endl;

      std::cout << "\nConstrained Transformation:" << std::endl;
      std::cout << "Residual: " << constrained_result.residual << std::endl;
      std::cout << "Valid: " << (constrained_result.isValidTransform ? "Yes" : "No") << std::endl;
      std::cout << "Volume Ratio: " << constrained_result.volumeRatio << std::endl;
      std::cout << "Preserves Volume: " << (constrained_result.preservesVolume ? "Yes" : "No") << std::endl;

      std::cout << "\nTransformation Matrix (6x6):" << std::endl;
      std::cout << constrained_result.transformMatrix << std::endl;

      std::cout << "\nProjector Matrix (6x6):" << std::endl;
      std::cout << constrained_result.projectorMatrix << std::endl;

      // Test the transformation
      std::vector<double> v_before = P3ToStdVector(p3_before);
      std::vector<double> v_transformed(6);

      for (size_t i = 0; i < 6; ++i) {
         v_transformed[i] = 0.0;
         for (size_t j = 0; j < 6; ++j) {
            v_transformed[i] += GetMatrixElement(constrained_result.transformMatrix, i, j) * v_before[j];
         }
      }

      P3 p3_transformed = StdVectorToP3(v_transformed);

      std::cout << "\nVerification:" << std::endl;
      std::cout << "Original after: " << p3_after << std::endl;
      std::cout << "Transformed:    " << p3_transformed << std::endl;
      std::cout << "Difference:     " << (p3_after - p3_transformed).norm() << std::endl;

      // Try to convert to G6 format
      try {
         MatG6 g6_transform = ToMatG6(constrained_result.transformMatrix);
         std::cout << "\nG6 Transformation Matrix:" << std::endl;
         std::cout << g6_transform << std::endl;
      }
      catch (const std::exception& e) {
         std::cout << "\nNote: Could not convert to G6 format: " << e.what() << std::endl;
      }

      std::cout << "\n" << std::string(60, '=') << std::endl;
   }

   // Analysis for StoreResults containing PairReporter<P3, P3> data
   static void AnalyzeStoredResults(const StoreResults<std::string, PairReporter<P3, P3>>& store,
      const std::string& boundaryKey) {
      std::cout << "=== Batch Analysis of Boundary Transformations ===" << std::endl;
      std::cout << "Analyzing key: " << boundaryKey << std::endl;

      if (!store.HasKey(boundaryKey)) {
         std::cout << "Key '" << boundaryKey << "' not found in results!" << std::endl;
         return;
      }

      // Get all pairs for this boundary type
      auto pairs = store.GetResult(boundaryKey);
      std::cout << "Found " << pairs.size() << " pairs for analysis" << std::endl;
      std::cout << "Total count for this key: " << store.GetItemCount(boundaryKey) << std::endl;

      for (size_t i = 0; i < pairs.size() && i < 3; ++i) { // Limit to first 3 pairs
         std::cout << "\n--- Pair " << (i + 1) << " of " << pairs.size() << " ---" << std::endl;

         // Extract P3 objects from PairReporter
         const P3& p3_before = pairs[i].second.GetFirst();   // First P3 in the pair
         const P3& p3_after = pairs[i].second.GetSecond();   // Second P3 in the pair

         std::ostringstream pairName;
         pairName << boundaryKey << "_pair_" << (i + 1);

         AnalyzeBoundaryPair(p3_before, p3_after, pairName.str());
      }
   }

   // Analysis for a specific boundary key, showing summary statistics
   static void AnalyzeBoundarySummary(const StoreResults<std::string, PairReporter<P3, P3>>& store,
      const std::string& boundaryKey) {
      std::cout << "=== Boundary Summary for: " << boundaryKey << " ===" << std::endl;

      if (!store.HasKey(boundaryKey)) {
         std::cout << "Key '" << boundaryKey << "' not found!" << std::endl;
         return;
      }

      auto pairs = store.GetResult(boundaryKey);
      std::cout << "Number of stored pairs: " << pairs.size() << std::endl;
      std::cout << "Total occurrences: " << store.GetItemCount(boundaryKey) << std::endl;

      if (pairs.empty()) return;

      // Analyze transformation consistency across pairs
      std::vector<double> residuals;

      for (size_t i = 0; i < pairs.size() && i < 10; ++i) { // Limit analysis
         const P3& p3_before = pairs[i].second.GetFirst();
         const P3& p3_after = pairs[i].second.GetSecond();

         auto result = ComputeConstrainedTransformation(p3_before, p3_after);
         if (result.isValidTransform) {
            residuals.push_back(result.residual);
         }
      }

      if (!residuals.empty()) {
         double avg_residual = 0.0;
         for (double r : residuals) avg_residual += r;
         avg_residual /= residuals.size();

         std::cout << "Average transformation residual: " << avg_residual << std::endl;
         std::cout << "Valid transformations: " << residuals.size()
            << " out of " << std::min(pairs.size(), size_t(10)) << " analyzed" << std::endl;
      }
   }
};

#endif // P3_TRANSFORMATION_H
