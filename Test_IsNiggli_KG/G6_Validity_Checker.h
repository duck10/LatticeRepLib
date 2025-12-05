// G6_Validity.h
// Comprehensive validity checker for G6 (lattice metric tensors)
// Tests if a G6 represents a physically valid lattice

#ifndef G6_VALIDITY_H
#define G6_VALIDITY_H

#include "G6.h"
#include <vector>
#include <string>
#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct G6Violation {
   std::string condition;
   std::string description;

   G6Violation(const std::string& cond, const std::string& desc)
      : condition(cond), description(desc) {
   }
};

class G6_Validity {
public:
   // Main checking function - returns true if G6 is valid
   // Populates violations vector with any failed conditions
   static bool Check(const G6& v, std::vector<G6Violation>& violations, double tolerance = 1.0e-10) {
      violations.clear();

      const double g1 = v[0];  // a^2
      const double g2 = v[1];  // b^2
      const double g3 = v[2];  // c^2
      const double g4 = v[3];  // 2bc*cos(alpha)
      const double g5 = v[4];  // 2ac*cos(beta)
      const double g6 = v[5];  // 2ab*cos(gamma)

      // ===================================================================
      // Basic positivity: squared lengths must be positive
      // ===================================================================
      if (g1 <= tolerance) {
         violations.push_back(G6Violation("g1 > 0",
            "g1 (a^2) must be positive"));
      }
      if (g2 <= tolerance) {
         violations.push_back(G6Violation("g2 > 0",
            "g2 (b^2) must be positive"));
      }
      if (g3 <= tolerance) {
         violations.push_back(G6Violation("g3 > 0",
            "g3 (c^2) must be positive"));
      }

      // If any squared length is non-positive, other tests are meaningless
      if (g1 <= tolerance || g2 <= tolerance || g3 <= tolerance) {
         return false;
      }

      // ===================================================================
      // Compute angle cosines (used by multiple checks below)
      // ===================================================================
      const double cos_alpha = g4 / (2.0 * std::sqrt(g2 * g3));
      const double cos_beta = g5 / (2.0 * std::sqrt(g1 * g3));
      const double cos_gamma = g6 / (2.0 * std::sqrt(g1 * g2));
      const double angle_tolerance = 1.0e-6;  // Slightly larger tolerance for angle comparisons

      // ===================================================================
      // Schwarz inequalities: |scalar_product| <= product_of_lengths
      // ===================================================================
      // For vectors a and b: |a.b| <= |a||b|
      // In G6 terms: |g4| <= 2*sqrt(g2)*sqrt(g3)
      //              |g5| <= 2*sqrt(g1)*sqrt(g3)
      //              |g6| <= 2*sqrt(g1)*sqrt(g2)

      const double max_g4 = 2.0 * std::sqrt(g2) * std::sqrt(g3);
      if (std::abs(g4) > max_g4 + tolerance) {
         violations.push_back(G6Violation("|g4| <= 2*sqrt(g2*g3)",
            "Schwarz inequality violated: |g4|=" + std::to_string(std::abs(g4)) +
            " > 2*sqrt(g2*g3)=" + std::to_string(max_g4)));
      }

      const double max_g5 = 2.0 * std::sqrt(g1) * std::sqrt(g3);
      if (std::abs(g5) > max_g5 + tolerance) {
         violations.push_back(G6Violation("|g5| <= 2*sqrt(g1*g3)",
            "Schwarz inequality violated: |g5|=" + std::to_string(std::abs(g5)) +
            " > 2*sqrt(g1*g3)=" + std::to_string(max_g5)));
      }

      const double max_g6 = 2.0 * std::sqrt(g1) * std::sqrt(g2);
      if (std::abs(g6) > max_g6 + tolerance) {
         violations.push_back(G6Violation("|g6| <= 2*sqrt(g1*g2)",
            "Schwarz inequality violated: |g6|=" + std::to_string(std::abs(g6)) +
            " > 2*sqrt(g1*g2)=" + std::to_string(max_g6)));
      }



      // Each cosine must be in [-1, 1] (already checked by Schwarz above)
      // But we need to verify they can form a valid parallelepiped

      // ===================================================================
      // Metric tensor positive definiteness
      // The determinant of the metric tensor must be positive
      // ===================================================================
      // Metric tensor G = [g1  g6/2 g5/2]
      //                   [g6/2 g2  g4/2]
      //                   [g5/2 g4/2 g3 ]
      //
      // det(G) = g1*g2*g3 + (g4*g5*g6)/4 - (g1*g4^2 + g2*g5^2 + g3*g6^2)/4

      const double det = g1 * g2 * g3
         + 0.25 * g4 * g5 * g6
         - 0.25 * (g1 * g4 * g4 + g2 * g5 * g5 + g3 * g6 * g6);

      if (det < -tolerance) {
         violations.push_back(G6Violation("det(G) > 0",
            "Metric tensor determinant = " + std::to_string(det) +
            " (negative - impossible geometry, not positive definite)"));
      } else if (std::abs(det) <= tolerance) {
         // Compute angle sum to provide helpful diagnostic
         const double alpha = std::acos(std::max(-1.0, std::min(1.0, cos_alpha)));
         const double beta = std::acos(std::max(-1.0, std::min(1.0, cos_beta)));
         const double gamma = std::acos(std::max(-1.0, std::min(1.0, cos_gamma)));
         const double angle_sum = alpha + beta + gamma;
         const double angle_sum_deg = angle_sum * 180.0 / M_PI;

         std::string explanation = "Metric tensor determinant = " + std::to_string(det) +
            " (zero or near-zero - degenerate lattice). ";

         if (std::abs(angle_sum - 2.0 * M_PI) < 0.01) {
            explanation += "Lattice vectors are coplanar (angle sum = " +
               std::to_string(angle_sum_deg) + " degrees ~= 360 degrees, vectors lie in a plane).";
         } else if (std::abs(cos_alpha - 1.0) < angle_tolerance ||
            std::abs(cos_beta - 1.0) < angle_tolerance ||
            std::abs(cos_gamma - 1.0) < angle_tolerance) {
            explanation += "One or more vectors are parallel (angle = 0 degrees).";
         } else if (std::abs(cos_alpha + 1.0) < angle_tolerance ||
            std::abs(cos_beta + 1.0) < angle_tolerance ||
            std::abs(cos_gamma + 1.0) < angle_tolerance) {
            explanation += "One or more vectors are antiparallel (angle = 180 degrees).";
         } else {
            explanation += "Lattice has zero volume (vectors are linearly dependent).";
         }

         violations.push_back(G6Violation("det(G) > 0", explanation));
      }

      // ===================================================================
      // Principal minors must be positive (for positive definiteness)
      // ===================================================================
      // 1x1 minors: g1, g2, g3 (already checked above)

      // 2x2 minors:
      // M1 = |g1   g6/2| = g1*g2 - g6^2/4
      //      |g6/2  g2 |
      const double minor1 = g1 * g2 - 0.25 * g6 * g6;
      if (minor1 <= tolerance) {
         violations.push_back(G6Violation("g1*g2 - g6^2/4 > 0",
            "2x2 minor (a,b plane) = " + std::to_string(minor1) +
            " (must be positive)"));
      }

      // M2 = |g1   g5/2| = g1*g3 - g5^2/4
      //      |g5/2  g3 |
      const double minor2 = g1 * g3 - 0.25 * g5 * g5;
      if (minor2 <= tolerance) {
         violations.push_back(G6Violation("g1*g3 - g5^2/4 > 0",
            "2x2 minor (a,c plane) = " + std::to_string(minor2) +
            " (must be positive)"));
      }

      // M3 = |g2   g4/2| = g2*g3 - g4^2/4
      //      |g4/2  g3 |
      const double minor3 = g2 * g3 - 0.25 * g4 * g4;
      if (minor3 <= tolerance) {
         violations.push_back(G6Violation("g2*g3 - g4^2/4 > 0",
            "2x2 minor (b,c plane) = " + std::to_string(minor3) +
            " (must be positive)"));
      }

      // ===================================================================
      // Triangle inequalities for angles
      // The three angles alpha, beta, gamma must satisfy:
      // 1. Each angle in (0, pi) - equivalently, each cos in (-1, 1)
      // 2. Sum of angles must satisfy geometric constraints
      // ===================================================================
      // cos(alpha) = g4/(2*sqrt(g2*g3))
      // cos(beta) = g5/(2*sqrt(g1*g3))
      // cos(gamma) = g6/(2*sqrt(g1*g2))

      // Note: Schwarz inequalities above already ensure cos values in [-1, 1]
      // But we need to check they don't equal exactly +/-1 (degenerate case)

      // Check for degenerate angles (0 degrees or 180 degrees)
      if (std::abs(std::abs(cos_alpha) - 1.0) < angle_tolerance) {
         violations.push_back(G6Violation("0 < alpha < pi",
            "Angle alpha = " + std::to_string(std::acos(cos_alpha) * 180.0 / M_PI) +
            " degrees (degenerate, too close to 0 degrees or 180 degrees)"));
      }
      if (std::abs(std::abs(cos_beta) - 1.0) < angle_tolerance) {
         violations.push_back(G6Violation("0 < beta < pi",
            "Angle beta = " + std::to_string(std::acos(cos_beta) * 180.0 / M_PI) +
            " degrees (degenerate, too close to 0 degrees or 180 degrees)"));
      }
      if (std::abs(std::abs(cos_gamma) - 1.0) < angle_tolerance) {
         violations.push_back(G6Violation("0 < gamma < pi",
            "Angle gamma = " + std::to_string(std::acos(cos_gamma) * 180.0 / M_PI) +
            " degrees (degenerate, too close to 0 degrees or 180 degrees)"));
      }

      // Note: The angle sum constraint (0 < alpha+beta+gamma < 2*pi) and other 
      // triangle-like inequalities for angles are automatically enforced by the 
      // metric determinant test below. If det > 0, all angle constraints are satisfied.
      // If det = 0, the lattice vectors are coplanar (e.g., three angles summing to 360 degrees).
      // If det < 0, the metric tensor is not positive definite (impossible geometry).

      // ===================================================================
      // All-negative scalar products constraint
      // When all angles are obtuse (all g4,g5,g6 negative),
      // there's a geometric constraint on how negative they can be
      // ===================================================================
      int nneg = 0;
      if (g4 < -tolerance) nneg++;
      if (g5 < -tolerance) nneg++;
      if (g6 < -tolerance) nneg++;

      if (nneg == 3) {
         const double sum = g4 + g5 + g6 + g1 + g2;
         if (sum > tolerance) {
            violations.push_back(G6Violation("All obtuse constraint",
               "When all angles obtuse (g4,g5,g6 < 0), require g4+g5+g6+g1+g2 <= 0, but sum = " +
               std::to_string(sum)));
         }
      }

      // ===================================================================
      // Volume check: V^2 = det(G) must be positive
      // (Already checked above, but we can verify consistency)
      // ===================================================================
      const double volume_squared = det;
      if (volume_squared <= tolerance) {
         violations.push_back(G6Violation("V^2 > 0",
            "Unit cell volume squared = " + std::to_string(volume_squared) +
            " (lattice has zero or negative volume)"));
      }

      return violations.empty();
   }

   // Simple boolean check without violation details
   static bool IsValid(const G6& v, double tolerance = 1.0e-10) {
      std::vector<G6Violation> violations;
      return Check(v, violations, tolerance);
   }

   // Print violations to output stream
   static void PrintViolations(const std::vector<G6Violation>& violations, std::ostream& os = std::cout) {
      if (violations.empty()) {
         os << "G6 represents a valid lattice" << std::endl;
         return;
      }

      os << "G6 validity violations (" << violations.size() << "):" << std::endl;
      for (size_t i = 0; i < violations.size(); ++i) {
         os << "  " << (i + 1) << ". " << violations[i].condition << std::endl;
         os << "     " << violations[i].description << std::endl;
      }
   }

   // Detailed report including metric tensor determinant and volume
   static void PrintDetailedReport(const G6& v, std::ostream& os = std::cout) {
      const double g1 = v[0];
      const double g2 = v[1];
      const double g3 = v[2];
      const double g4 = v[3];
      const double g5 = v[4];
      const double g6 = v[5];

      os << "G6 Validity Report" << std::endl;
      os << "==================" << std::endl;
      os << "G6: " << v << std::endl;

      // Compute angles
      if (g1 > 0 && g2 > 0 && g3 > 0) {
         const double cos_alpha = g4 / (2.0 * std::sqrt(g2) * std::sqrt(g3));
         const double cos_beta = g5 / (2.0 * std::sqrt(g1) * std::sqrt(g3));
         const double cos_gamma = g6 / (2.0 * std::sqrt(g1) * std::sqrt(g2));

         os << "Cosines: cos(alpha)=" << cos_alpha
            << ", cos(beta)=" << cos_beta
            << ", cos(gamma)=" << cos_gamma << std::endl;

         // Compute determinant and volume
         const double det = g1 * g2 * g3
            + 0.25 * g4 * g5 * g6
            - 0.25 * (g1 * g4 * g4 + g2 * g5 * g5 + g3 * g6 * g6);

         os << "Metric determinant: " << det << std::endl;

         if (det > 0.0) {
            const double volume = std::sqrt(det);
            os << "Unit cell volume: " << volume << std::endl;
         } else if (det < 0.0) {
            os << "Unit cell volume: UNDEFINED (determinant is negative)" << std::endl;
         } else {
            os << "Unit cell volume: 0 (determinant is zero, degenerate lattice)" << std::endl;
         }
      }

      // Check validity
      std::vector<G6Violation> violations;
      bool valid = Check(v, violations);

      os << "\nValidity: " << (valid ? "VALID" : "INVALID") << std::endl;
      if (!valid) {
         os << std::endl;
         PrintViolations(violations, os);
      }
   }

   // Run comprehensive test suite with various test cases
   static void RunTests(std::ostream& os = std::cout) {
      os << "G6 Validity Checker Test Suite" << std::endl;
      os << "===============================" << std::endl << std::endl;

      // Test 1: Valid cubic lattice
      os << "Test 1: Valid cubic lattice" << std::endl;
      os << "----------------------------" << std::endl;
      G6 test1(10.0, 10.0, 10.0, 0.0, 0.0, 0.0);
      PrintDetailedReport(test1, os);
      os << std::endl << std::endl;

      // Test 2: Invalid - negative g1
      os << "Test 2: Invalid - negative g1" << std::endl;
      os << "------------------------------" << std::endl;
      G6 test2(-10.0, 10.0, 10.0, 0.0, 0.0, 0.0);
      PrintDetailedReport(test2, os);
      os << std::endl << std::endl;

      // Test 3: Invalid - Schwarz inequality violated
      os << "Test 3: Invalid - Schwarz inequality violated" << std::endl;
      os << "----------------------------------------------" << std::endl;
      // g2=4, g3=9, so max |g4| = 2*sqrt(4*9) = 12
      // Setting g4=15 violates this
      G6 test3(25.0, 4.0, 9.0, 15.0, 0.0, 0.0);
      PrintDetailedReport(test3, os);
      os << std::endl << std::endl;

      // Test 4: Invalid - zero determinant (degenerate)
      os << "Test 4: Invalid - zero determinant (vectors parallel)" << std::endl;
      os << "------------------------------------------------------" << std::endl;
      // Setting g4 = 2*sqrt(g2*g3) makes cos(alpha) = 1, so b parallel to c
      G6 test4(10.0, 4.0, 9.0, 12.0, 0.0, 0.0);
      PrintDetailedReport(test4, os);
      os << std::endl << std::endl;

      // Test 5: Invalid - 2x2 minor violation
      os << "Test 5: Invalid - 2x2 principal minor negative" << std::endl;
      os << "-----------------------------------------------" << std::endl;
      // Make g1*g2 - g6^2/4 < 0 by setting g6 too large
      // g1=4, g2=4, so g1*g2 = 16, need g6^2/4 > 16, so |g6| > 8
      G6 test5(4.0, 4.0, 16.0, 0.0, 0.0, 9.0);
      PrintDetailedReport(test5, os);
      os << std::endl << std::endl;

      // Test 6: Valid monoclinic lattice (one obtuse angle)
      os << "Test 6: Valid monoclinic lattice (beta = 120 degrees)" << std::endl;
      os << "-----------------------------------------------" << std::endl;
      double a6 = 3.0, b6 = 4.0, c6 = 5.0;
      double beta_cos = -0.5;  // 120 degrees
      double g5_6 = 2.0 * a6 * c6 * beta_cos;
      G6 test6(a6 * a6, b6 * b6, c6 * c6, 0.0, g5_6, 0.0);
      PrintDetailedReport(test6, os);
      os << std::endl << std::endl;

      // Test 7: Invalid - angle sum >= 2*pi
      os << "Test 7: Invalid - angle sum >= 360 degrees (impossible geometry)" << std::endl;
      os << "-----------------------------------------------------------" << std::endl;
      // Three angles all close to 120 degrees would sum to 360 degrees
      // cos(120 degrees) = -0.5
      double a7 = 3.0, b7 = 3.0, c7 = 3.0;
      double g4_7 = -2.0 * b7 * c7 * 0.5;
      double g5_7 = -2.0 * a7 * c7 * 0.5;
      double g6_7 = -2.0 * a7 * b7 * 0.5;
      G6 test7(a7 * a7, b7 * b7, c7 * c7, g4_7, g5_7, g6_7);
      PrintDetailedReport(test7, os);
      os << std::endl << std::endl;

      // Test 8: Valid triclinic with all obtuse angles
      os << "Test 8: Valid triclinic (all obtuse angles)" << std::endl;
      os << "--------------------------------------------" << std::endl;
      // All angles obtuse but sum < 360 degrees and satisfying constraints
      // Use angles: alpha=110 degrees, beta=110 degrees, gamma=110 degrees (sum=330 degrees < 360 degrees)
      double a8 = 4.0, b8 = 5.0, c8 = 6.0;
      double cos_110 = std::cos(110.0 * M_PI / 180.0);  // ~= -0.342
      double g4_8 = 2.0 * b8 * c8 * cos_110;  // 2*5*6*(-0.342) = -20.52
      double g5_8 = 2.0 * a8 * c8 * cos_110;  // 2*4*6*(-0.342) = -16.42
      double g6_8 = 2.0 * a8 * b8 * cos_110;  // 2*4*5*(-0.342) = -13.68
      // Check: g4+g5+g6+g1+g2 = -20.52-16.42-13.68+16+25 = -9.62 < 0 OK
      G6 test8(a8 * a8, b8 * b8, c8 * c8, g4_8, g5_8, g6_8);
      PrintDetailedReport(test8, os);
      os << std::endl << std::endl;

      // Test 9: Invalid - all obtuse constraint violated
      os << "Test 9: Invalid - all obtuse angles constraint violated" << std::endl;
      os << "--------------------------------------------------------" << std::endl;
      // Make all angles obtuse but violate g4+g5+g6+g1+g2 <= 0
      // This requires very obtuse angles but not too large in magnitude
      G6 test9(2.0, 2.0, 10.0, -0.5, -0.5, -0.5);
      PrintDetailedReport(test9, os);
      os << std::endl << std::endl;

      // Test 10: Edge case from KG/AB comparison
      os << "Test 10: Cell from Krivy-Gruber / Andrews-Bernstein test" << std::endl;
      os << "---------------------------------------------------------" << std::endl;
      G6 test10(7.560, 11.560, 11.560, 11.560, 2.890, 5.780);
      PrintDetailedReport(test10, os);
      os << std::endl << std::endl;

      os << "Test suite completed." << std::endl;
   }
};

#endif // G6_VALIDITY_H
