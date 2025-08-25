#ifndef PURE_BOUNDARY_GENERATOR_H
#define PURE_BOUNDARY_GENERATOR_H

#include "P3.h"
#include "P3_Reduce.h"
#include "LRL_Cell.h"
#include "EnhancedBoundaryDetector.h"
#include "Polar.h"
#include <vector>
#include <string>
#include <cmath>
#include <iostream>

class PureBoundaryGenerator {
private:
   const double EPSILON = 1e-6;

   // Helper to check if a P3 exhibits the expected boundary condition
   static bool VerifyBoundaryCondition(const P3& p3, const std::string& boundaryType) {
      EnhancedBoundaryDetector detector;

      // Create a small perturbation to test boundary crossing
      P3 perturbed = PerturbP3(p3, 0.001);
      auto [reducedOrig, _] = P3_Reduce::ReduceCellWith3480(p3);
      auto [reducedPert, __] = P3_Reduce::ReduceCellWith3480(perturbed);

      double percentDiff = 100.0 * (P3(reducedOrig) - P3(reducedPert)).norm() /
         ((P3(reducedOrig).norm() + P3(reducedPert).norm()) / 2.0);

      std::string detectedType = detector.classifyBoundaryType(percentDiff, P3(reducedOrig), P3(reducedPert));
      return detectedType.find(boundaryType) != std::string::npos;
   }

public:
   struct BoundaryExample {
      P3 p3;
      std::string boundaryType;
      bool isValidAfterReduction;
      double constraintValue;  // Actual value of the constraint (e.g., dot product for orthogonality)

      BoundaryExample(const P3& p, const std::string& type)
         : p3(p), boundaryType(type), isValidAfterReduction(false), constraintValue(0.0) {
      }
   };

   // 1. ORTHOGONALITY BOUNDARIES

   // Generate p1 ⊥ p2 boundary example
   static BoundaryExample GenerateP1OrthP2Boundary() {
      // Strategy: Create vectors where p1 ⊥ p2, then adjust to ensure reduction stability

      // Start with orthogonal vectors in 2D
      P3 candidate(
         1.0, 0.0,     // p1 = (1, 0)
         0.0, 1.0,     // p2 = (0, 1) - orthogonal to p1
         1.5, 1.2      // p3 arbitrary but reasonable
      );

      // Try multiple variations until we get a stable reduced form
      for (int attempt = 0; attempt < 100; ++attempt) {
         // Add small variations to avoid exact degeneracies
         double scale = 1.0 + 0.1 * attempt;
         P3 test(
            scale, 0.0,
            0.0, scale * 1.1,
            scale * 1.5, scale * 1.2
         );

         auto [reduced, transform] = P3_Reduce::ReduceCellWith3480(test);
         P3 reducedP3(reduced);

         // Check if p1 ⊥ p2 in reduced form
         double dot = reducedP3[0].first * reducedP3[1].first +
            reducedP3[0].second * reducedP3[1].second;

         if (std::abs(dot) < 0.1) {  // Nearly orthogonal
            BoundaryExample result(reducedP3, "angle_p1_p2_90deg");
            result.constraintValue = dot;
            result.isValidAfterReduction = true;
            return result;
         }
      }

      return BoundaryExample(candidate, "angle_p1_p2_90deg");
   }

   // Generate p1 ⊥ p3 boundary example
   static BoundaryExample GenerateP1OrthP3Boundary() {
      P3 candidate(
         1.0, 0.0,     // p1 = (1, 0)
         1.2, 0.8,     // p2 arbitrary
         0.0, 1.0      // p3 = (0, 1) - orthogonal to p1
      );

      for (int attempt = 0; attempt < 100; ++attempt) {
         double scale = 1.0 + 0.1 * attempt;
         P3 test(
            scale, 0.0,
            scale * 1.2, scale * 0.8,
            0.0, scale * 1.0
         );

         auto [reduced, transform] = P3_Reduce::ReduceCellWith3480(test);
         P3 reducedP3(reduced);

         double dot = reducedP3[0].first * reducedP3[2].first +
            reducedP3[0].second * reducedP3[2].second;

         if (std::abs(dot) < 0.1) {
            BoundaryExample result(reducedP3, "angle_p1_p3_90deg");
            result.constraintValue = dot;
            result.isValidAfterReduction = true;
            return result;
         }
      }

      return BoundaryExample(candidate, "angle_p1_p3_90deg");
   }

   // Generate p2 ⊥ p3 boundary example  
   static BoundaryExample GenerateP2OrthP3Boundary() {
      P3 candidate(
         1.5, 0.5,     // p1 arbitrary
         1.0, 0.0,     // p2 = (1, 0)
         0.0, 1.0      // p3 = (0, 1) - orthogonal to p2
      );

      for (int attempt = 0; attempt < 100; ++attempt) {
         double scale = 1.0 + 0.1 * attempt;
         P3 test(
            scale * 1.5, scale * 0.5,
            scale, 0.0,
            0.0, scale
         );

         auto [reduced, transform] = P3_Reduce::ReduceCellWith3480(test);
         P3 reducedP3(reduced);

         double dot = reducedP3[1].first * reducedP3[2].first +
            reducedP3[1].second * reducedP3[2].second;

         if (std::abs(dot) < 0.1) {
            BoundaryExample result(reducedP3, "angle_p2_p3_90deg");
            result.constraintValue = dot;
            result.isValidAfterReduction = true;
            return result;
         }
      }

      return BoundaryExample(candidate, "angle_p2_p3_90deg");
   }

   // 2. MAGNITUDE EQUALITY BOUNDARIES

   // Generate |p1| = |p2| boundary example
   static BoundaryExample GenerateMagP1EqP2Boundary() {
      for (int attempt = 0; attempt < 200; ++attempt) {
         // Start with vectors of equal magnitude
         double mag = 1.0 + 0.1 * attempt;
         double angle1 = attempt * 0.1;
         double angle2 = angle1 + 1.0;  // Different angles but same magnitude

         P3 test(
            mag * cos(angle1), mag * sin(angle1),  // |p1| = mag
            mag * cos(angle2), mag * sin(angle2),  // |p2| = mag
            1.3, 0.7                               // p3 different magnitude
         );

         auto [reduced, transform] = P3_Reduce::ReduceCellWith3480(test);
         P3 reducedP3(reduced);

         double mag1 = sqrt(reducedP3[0].first * reducedP3[0].first +
            reducedP3[0].second * reducedP3[0].second);
         double mag2 = sqrt(reducedP3[1].first * reducedP3[1].first +
            reducedP3[1].second * reducedP3[1].second);

         if (std::abs(mag1 - mag2) < 0.1) {
            BoundaryExample result(reducedP3, "mag_p1_eq_p2");
            result.constraintValue = std::abs(mag1 - mag2);
            result.isValidAfterReduction = true;
            return result;
         }
      }

      return BoundaryExample(P3(1, 0, 1, 0, 1.3, 0.7), "mag_p1_eq_p2");
   }

   // Generate |p1| = |p3| boundary example
   static BoundaryExample GenerateMagP1EqP3Boundary() {
      for (int attempt = 0; attempt < 200; ++attempt) {
         double mag = 1.0 + 0.1 * attempt;
         double angle1 = attempt * 0.1;
         double angle3 = angle1 + 1.5;

         P3 test(
            mag * cos(angle1), mag * sin(angle1),  // |p1| = mag
            1.2, 0.9,                              // p2 different magnitude
            mag * cos(angle3), mag * sin(angle3)   // |p3| = mag
         );

         auto [reduced, transform] = P3_Reduce::ReduceCellWith3480(test);
         P3 reducedP3(reduced);

         double mag1 = sqrt(reducedP3[0].first * reducedP3[0].first +
            reducedP3[0].second * reducedP3[0].second);
         double mag3 = sqrt(reducedP3[2].first * reducedP3[2].first +
            reducedP3[2].second * reducedP3[2].second);

         if (std::abs(mag1 - mag3) < 0.1) {
            BoundaryExample result(reducedP3, "mag_p1_eq_p3");
            result.constraintValue = std::abs(mag1 - mag3);
            result.isValidAfterReduction = true;
            return result;
         }
      }

      return BoundaryExample(P3(1, 0, 1.2, 0.9, 1, 0), "mag_p1_eq_p3");
   }

   // Generate |p2| = |p3| boundary example
   static BoundaryExample GenerateMagP2EqP3Boundary() {
      for (int attempt = 0; attempt < 200; ++attempt) {
         double mag = 1.0 + 0.1 * attempt;
         double angle2 = attempt * 0.1;
         double angle3 = angle2 + 1.2;

         P3 test(
            1.4, 0.6,                              // p1 different magnitude
            mag * cos(angle2), mag * sin(angle2),  // |p2| = mag
            mag * cos(angle3), mag * sin(angle3)   // |p3| = mag
         );

         auto [reduced, transform] = P3_Reduce::ReduceCellWith3480(test);
         P3 reducedP3(reduced);

         double mag2 = sqrt(reducedP3[1].first * reducedP3[1].first +
            reducedP3[1].second * reducedP3[1].second);
         double mag3 = sqrt(reducedP3[2].first * reducedP3[2].first +
            reducedP3[2].second * reducedP3[2].second);

         if (std::abs(mag2 - mag3) < 0.1) {
            BoundaryExample result(reducedP3, "mag_p2_eq_p3");
            result.constraintValue = std::abs(mag2 - mag3);
            result.isValidAfterReduction = true;
            return result;
         }
      }

      return BoundaryExample(P3(1.4, 0.6, 1, 0, 1, 0), "mag_p2_eq_p3");
   }

   // 3. COORDINATE RELATIONSHIP BOUNDARIES

   // Generate p1.x = -p1.y boundary example
   static BoundaryExample GenerateP1XEqNegYBoundary() {
      for (int attempt = 0; attempt < 100; ++attempt) {
         double val = 1.0 + 0.1 * attempt;
         P3 test(
            val, -val,    // p1.x = -p1.y
            1.2, 0.8,     // p2 arbitrary
            0.9, 1.3      // p3 arbitrary
         );

         auto [reduced, transform] = P3_Reduce::ReduceCellWith3480(test);
         P3 reducedP3(reduced);

         if (std::abs(reducedP3[0].first + reducedP3[0].second) < 0.1) {
            BoundaryExample result(reducedP3, "p1_x_eq_neg_y");
            result.constraintValue = std::abs(reducedP3[0].first + reducedP3[0].second);
            result.isValidAfterReduction = true;
            return result;
         }
      }

      return BoundaryExample(P3(1, -1, 1.2, 0.8, 0.9, 1.3), "p1_x_eq_neg_y");
   }

   // Generate p2.x = -p2.y boundary example
   static BoundaryExample GenerateP2XEqNegYBoundary() {
      for (int attempt = 0; attempt < 100; ++attempt) {
         double val = 1.0 + 0.1 * attempt;
         P3 test(
            1.1, 0.7,     // p1 arbitrary
            val, -val,    // p2.x = -p2.y
            1.3, 0.9      // p3 arbitrary
         );

         auto [reduced, transform] = P3_Reduce::ReduceCellWith3480(test);
         P3 reducedP3(reduced);

         if (std::abs(reducedP3[1].first + reducedP3[1].second) < 0.1) {
            BoundaryExample result(reducedP3, "p2_x_eq_neg_y");
            result.constraintValue = std::abs(reducedP3[1].first + reducedP3[1].second);
            result.isValidAfterReduction = true;
            return result;
         }
      }

      return BoundaryExample(P3(1.1, 0.7, 1, -1, 1.3, 0.9), "p2_x_eq_neg_y");
   }

   // Generate p3.x = -p3.y boundary example
   static BoundaryExample GenerateP3XEqNegYBoundary() {
      for (int attempt = 0; attempt < 100; ++attempt) {
         double val = 1.0 + 0.1 * attempt;
         P3 test(
            1.0, 0.8,     // p1 arbitrary
            1.4, 1.1,     // p2 arbitrary
            val, -val     // p3.x = -p3.y
         );

         auto [reduced, transform] = P3_Reduce::ReduceCellWith3480(test);
         P3 reducedP3(reduced);

         if (std::abs(reducedP3[2].first + reducedP3[2].second) < 0.1) {
            BoundaryExample result(reducedP3, "p3_x_eq_neg_y");
            result.constraintValue = std::abs(reducedP3[2].first + reducedP3[2].second);
            result.isValidAfterReduction = true;
            return result;
         }
      }

      return BoundaryExample(P3(1.0, 0.8, 1.4, 1.1, 1, -1), "p3_x_eq_neg_y");
   }

   // 4. SIGN-CHANGE BOUNDARIES

   // Generate p1.x sign flip boundary example
   static BoundaryExample GenerateP1XSignFlipBoundary() {
      // Create a P3 where small perturbations cause p1.x to change sign
      for (int attempt = 0; attempt < 100; ++attempt) {
         double small_x = 0.01 * (attempt - 50);  // Range from -0.5 to +0.5
         P3 test(
            small_x, 1.0,  // p1.x near zero
            1.2, 0.8,      // p2 arbitrary
            0.9, 1.3       // p3 arbitrary
         );

         auto [reduced, transform] = P3_Reduce::ReduceCellWith3480(test);
         P3 reducedP3(reduced);

         if (std::abs(reducedP3[0].first) < 0.1 && std::abs(reducedP3[0].first) > 0.001) {
            BoundaryExample result(reducedP3, "p1_x_sign_flip");
            result.constraintValue = reducedP3[0].first;
            result.isValidAfterReduction = true;
            return result;
         }
      }

      return BoundaryExample(P3(0.01, 1.0, 1.2, 0.8, 0.9, 1.3), "p1_x_sign_flip");
   }

   // Generate p2.x sign flip boundary example
   static BoundaryExample GenerateP2XSignFlipBoundary() {
      for (int attempt = 0; attempt < 100; ++attempt) {
         double small_x = 0.01 * (attempt - 50);
         P3 test(
            1.1, 0.9,      // p1 arbitrary
            small_x, 1.0,  // p2.x near zero
            1.3, 0.7       // p3 arbitrary
         );

         auto [reduced, transform] = P3_Reduce::ReduceCellWith3480(test);
         P3 reducedP3(reduced);

         if (std::abs(reducedP3[1].first) < 0.1 && std::abs(reducedP3[1].first) > 0.001) {
            BoundaryExample result(reducedP3, "p2_x_sign_flip");
            result.constraintValue = reducedP3[1].first;
            result.isValidAfterReduction = true;
            return result;
         }
      }

      return BoundaryExample(P3(1.1, 0.9, 0.01, 1.0, 1.3, 0.7), "p2_x_sign_flip");
   }

   // Generate p3.x sign flip boundary example
   static BoundaryExample GenerateP3XSignFlipBoundary() {
      for (int attempt = 0; attempt < 100; ++attempt) {
         double small_x = 0.01 * (attempt - 50);
         P3 test(
            1.0, 0.8,      // p1 arbitrary
            1.2, 1.1,      // p2 arbitrary
            small_x, 1.0   // p3.x near zero
         );

         auto [reduced, transform] = P3_Reduce::ReduceCellWith3480(test);
         P3 reducedP3(reduced);

         if (std::abs(reducedP3[2].first) < 0.1 && std::abs(reducedP3[2].first) > 0.001) {
            BoundaryExample result(reducedP3, "p3_x_sign_flip");
            result.constraintValue = reducedP3[2].first;
            result.isValidAfterReduction = true;
            return result;
         }
      }

      return BoundaryExample(P3(1.0, 0.8, 1.2, 1.1, 0.01, 1.0), "p3_x_sign_flip");
   }

   // MAIN GENERATION FUNCTION
   static std::vector<BoundaryExample> GenerateAllPureBoundaryExamples() {
      std::vector<BoundaryExample> examples;

      std::cout << "Generating Pure Boundary Examples..." << std::endl;

      // Orthogonality boundaries
      examples.push_back(GenerateP1OrthP2Boundary());
      examples.push_back(GenerateP1OrthP3Boundary());
      examples.push_back(GenerateP2OrthP3Boundary());

      // Magnitude equality boundaries
      examples.push_back(GenerateMagP1EqP2Boundary());
      examples.push_back(GenerateMagP1EqP3Boundary());
      examples.push_back(GenerateMagP2EqP3Boundary());

      // Coordinate relationship boundaries
      examples.push_back(GenerateP1XEqNegYBoundary());
      examples.push_back(GenerateP2XEqNegYBoundary());
      examples.push_back(GenerateP3XEqNegYBoundary());

      // Sign change boundaries
      examples.push_back(GenerateP1XSignFlipBoundary());
      examples.push_back(GenerateP2XSignFlipBoundary());
      examples.push_back(GenerateP3XSignFlipBoundary());

      // Report results
      int successCount = 0;
      for (const auto& example : examples) {
         std::cout << "Boundary: " << example.boundaryType
            << " | Valid: " << (example.isValidAfterReduction ? "Yes" : "No")
            << " | Constraint Value: " << example.constraintValue << std::endl;
         if (example.isValidAfterReduction) successCount++;
      }

      std::cout << "Successfully generated " << successCount << " out of "
         << examples.size() << " pure boundary examples." << std::endl;

      return examples;
   }
};

#endif // PURE_BOUNDARY_GENERATOR_H

