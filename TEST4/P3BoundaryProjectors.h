#ifndef P3_BOUNDARY_PROJECTORS_H
#define P3_BOUNDARY_PROJECTORS_H



// P3 Boundary Projectors - Working Mathematical Implementation

#include "P3.h"
#include <cmath>
#include <vector>

class P3BoundaryProjectors {
public:

   // Project P3 to p2 ⊥ p3 boundary (constraint: p2·p3 = 0)
   static P3 ProjectToP2PerpP3Boundary(const P3& input) {
      P3 result = input;

      // Constraint function: f(p) = p2.x*p3.x + p2.y*p3.y = 0
      // Gradient: ∇f = [0, 0, p3.x, p3.y, p2.x, p2.y]

      const auto& p2 = input[1];  // (p2.x, p2.y)
      const auto& p3 = input[2];  // (p3.x, p3.y)

      // Current constraint violation
      double constraint_value = p2.first * p3.first + p2.second * p3.second;

      if (std::abs(constraint_value) < 1e-10) {
         return result;  // Already on boundary
      }

      // Gradient components (6D vector)
      std::vector<double> grad = { 0.0, 0.0, p3.first, p3.second, p2.first, p2.second };

      // Gradient magnitude squared
      double grad_norm_sq = grad[2] * grad[2] + grad[3] * grad[3] + grad[4] * grad[4] + grad[5] * grad[5];

      if (grad_norm_sq < 1e-10) {
         return result;  // Gradient is zero
      }

      // Projection: v_projected = v - (f(v) / ||∇f||²) * ∇f
      double lambda = constraint_value / grad_norm_sq;

      // Apply projection (only p2 and p3 change)
      result[1].first -= lambda * grad[2];  // p2.x -= λ * p3.x
      result[1].second -= lambda * grad[3];  // p2.y -= λ * p3.y
      result[2].first -= lambda * grad[4];  // p3.x -= λ * p2.x
      result[2].second -= lambda * grad[5];  // p3.y -= λ * p2.y

      return result;
   }

   // Project P3 to |p2| = |p3| boundary
   static P3 ProjectToMagP2EqP3Boundary(const P3& input) {
      P3 result = input;

      const auto& p2 = input[1];
      const auto& p3 = input[2];

      double mag2_sq = p2.first * p2.first + p2.second * p2.second;
      double mag3_sq = p3.first * p3.first + p3.second * p3.second;

      // Constraint: f(p) = |p2|² - |p3|² = 0
      double constraint_value = mag2_sq - mag3_sq;

      if (std::abs(constraint_value) < 1e-10) {
         return result;  // Already on boundary
      }

      // Gradient: ∇f = [0, 0, 2*p2.x, 2*p2.y, -2*p3.x, -2*p3.y]
      std::vector<double> grad = { 0.0, 0.0, 2 * p2.first, 2 * p2.second, -2 * p3.first, -2 * p3.second };

      double grad_norm_sq = grad[2] * grad[2] + grad[3] * grad[3] + grad[4] * grad[4] + grad[5] * grad[5];

      if (grad_norm_sq < 1e-10) {
         return result;
      }

      double lambda = constraint_value / grad_norm_sq;

      result[1].first -= lambda * grad[2];  // p2.x -= λ * 2*p2.x
      result[1].second -= lambda * grad[3];  // p2.y -= λ * 2*p2.y  
      result[2].first -= lambda * grad[4];  // p3.x -= λ * (-2*p3.x)
      result[2].second -= lambda * grad[5];  // p3.y -= λ * (-2*p3.y)

      return result;
   }

   // Project P3 to p1.x = -p1.y boundary
   static P3 ProjectToP1XEqNegYBoundary(const P3& input) {
      P3 result = input;

      const auto& p1 = input[0];

      // Constraint: f(p) = p1.x + p1.y = 0
      double constraint_value = p1.first + p1.second;

      if (std::abs(constraint_value) < 1e-10) {
         return result;
      }

      // Gradient: ∇f = [1, 1, 0, 0, 0, 0]
      std::vector<double> grad = { 1.0, 1.0, 0.0, 0.0, 0.0, 0.0 };

      double grad_norm_sq = 2.0;  // 1² + 1² = 2
      double lambda = constraint_value / grad_norm_sq;

      result[0].first -= lambda;  // p1.x -= λ
      result[0].second -= lambda;  // p1.y -= λ

      return result;
   }

   // Constrained projection that maintains unit cell validity
   static P3 ConstrainedProjectToP2PerpP3Boundary(const P3& input) {
      P3 current = input;

      // Iterative approach: small steps toward boundary while checking validity
      const int max_iterations = 100;
      const double step_size = 0.1;

      for (int iter = 0; iter < max_iterations; ++iter) {
         // Calculate current constraint violation
         const auto& p2 = current[1];
         const auto& p3 = current[2];
         double constraint_value = p2.first * p3.first + p2.second * p3.second;

         if (std::abs(constraint_value) < 1e-6) {
            break;  // Close enough to boundary
         }

         // Calculate gradient
         std::vector<double> grad = { 0.0, 0.0, p3.first, p3.second, p2.first, p2.second };
         double grad_norm_sq = grad[2] * grad[2] + grad[3] * grad[3] + grad[4] * grad[4] + grad[5] * grad[5];

         if (grad_norm_sq < 1e-10) break;

         // Take a small step toward boundary
         double lambda = step_size * constraint_value / grad_norm_sq;

         P3 candidate = current;
         candidate[1].first -= lambda * grad[2];
         candidate[1].second -= lambda * grad[3];
         candidate[2].first -= lambda * grad[4];
         candidate[2].second -= lambda * grad[5];

         // Check if candidate creates valid unit cell
         LRL_Cell test_cell(candidate);
         if (test_cell.IsValid()) {
            current = candidate;
         } else {
            // If invalid, try smaller step
            lambda *= 0.5;
            candidate = current;
            candidate[1].first -= lambda * grad[2];
            candidate[1].second -= lambda * grad[3];
            candidate[2].first -= lambda * grad[4];
            candidate[2].second -= lambda * grad[5];

            LRL_Cell test_cell2(candidate);
            if (test_cell2.IsValid()) {
               current = candidate;
            } else {
               break;  // Can't proceed without violating validity
            }
         }
      }

      return current;
   }

   // Iterative projection in unit cell space to reach boundary
   static P3 ProjectToAlpha90Boundary(const P3& input) {
      LRL_Cell cell(input);
      if (!cell.IsValid()) return input;

      const double target_alpha = 90.0;
      const double tolerance = 0.1;  // Within 0.1 degrees
      const int max_iterations = 50;

      for (int iter = 0; iter < max_iterations; ++iter) {
         double alpha_deg = cell[3] * 180.0 / 3.141592653589793;

         if (std::abs(alpha_deg - target_alpha) < tolerance) {
            break;  // Close enough to boundary
         }

         // Take 20% step toward 90°
         double new_alpha = alpha_deg + 0.2 * (target_alpha - alpha_deg);

         LRL_Cell candidate(cell[0], cell[1], cell[2],
            new_alpha * 3.141592653589793 / 180.0,
            cell[4], cell[5]);

         if (candidate.IsValid()) {
            cell = candidate;
         } else {
            // If 20% step fails, try 5% step
            new_alpha = alpha_deg + 0.05 * (target_alpha - alpha_deg);
            candidate = LRL_Cell(cell[0], cell[1], cell[2],
               new_alpha * 3.141592653589793 / 180.0,
               cell[4], cell[5]);
            if (candidate.IsValid()) {
               cell = candidate;
            } else {
               break;  // Can't proceed further
            }
         }
      }

      return P3(cell);
   }

   // Test projector effectiveness
   static void TestProjectors() {
      std::cout << "=== Testing P3 Boundary Projectors ===" << std::endl;

      // Test case 1: General P3
      P3 test_p3(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);

      std::cout << "Original P3: " << test_p3 << std::endl;

      // Test p2⊥p3 projector
      P3 projected = ProjectToP2PerpP3Boundary(test_p3);
      std::cout << "After p2⊥p3 projection: " << projected << std::endl;

      // Verify orthogonality
      double dot_product = projected[1].first * projected[2].first +
         projected[1].second * projected[2].second;
      std::cout << "p2·p3 after projection: " << dot_product << " (should be ≈ 0)" << std::endl;

      // Test magnitude equality projector
      P3 mag_projected = ProjectToMagP2EqP3Boundary(test_p3);
      std::cout << "After |p2|=|p3| projection: " << mag_projected << std::endl;

      double mag2 = std::sqrt(mag_projected[1].first * mag_projected[1].first +
         mag_projected[1].second * mag_projected[1].second);
      double mag3 = std::sqrt(mag_projected[2].first * mag_projected[2].first +
         mag_projected[2].second * mag_projected[2].second);
      std::cout << "|p2| = " << mag2 << ", |p3| = " << mag3
         << ", difference = " << std::abs(mag2 - mag3) << std::endl;

      // Test coordinate relationship projector
      P3 coord_projected = ProjectToP1XEqNegYBoundary(test_p3);
      std::cout << "After p1.x=-p1.y projection: " << coord_projected << std::endl;

      double coord_constraint = coord_projected[0].first + coord_projected[0].second;
      std::cout << "p1.x + p1.y = " << coord_constraint << " (should be ≈ 0)" << std::endl;
   }

   // Generate boundary example using projector
   static P3 GenerateProjectedBoundaryExample(const std::string& boundaryType) {
      // Start with a reasonable P3
      P3 seed(3.0, 4.0, 5.0, 6.0, 7.0, 8.0);

      P3 projected;
      if (boundaryType == "single_angle_p2_p3_90deg") {
         projected = ProjectToP2PerpP3Boundary(seed);
      } else if (boundaryType == "single_mag_p2_eq_p3") {
         projected = ProjectToMagP2EqP3Boundary(seed);
      } else if (boundaryType == "single_p1_x_eq_neg_y") {
         projected = ProjectToP1XEqNegYBoundary(seed);
      } else {
         return seed;  // No projector available
      }

      // Apply reduction to get final result
      auto [reduced, transform] = P3_Reduce::ReduceCellWith3480(projected);
      return P3(reduced);
   }
};

#endif // !P3_BOUNDARY_PROJECTORS_H
