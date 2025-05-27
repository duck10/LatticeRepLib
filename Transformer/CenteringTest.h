#ifndef CENTERINGTEST_H
#define CENTERINGTEST_H

#include <iostream>

#include "LRL_Cell.h"
#include "LRL_Vector3.h"
#include "NCDist.h"
#include "Niggli.h"
#include "TransformerUtilities.h"



// Define M_PI if it's not already defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void testCenteringMatrix(const std::string& name, const Matrix_3x3& m, const LRL_Cell& cell) {
   const G6 g6cell(cell);
   G6 reduced;
   G6 reducedprimitive;


   const LRL_Cell primitive = m * cell;
   const G6 g6primitive(primitive);

   const bool b1 = Niggli::Reduce(g6cell, reduced);
   const bool b2 = Niggli::Reduce(g6primitive, reducedprimitive);
   const double dist = NCDist(reduced.data(), reducedprimitive.data());
   if (dist > 1.0E-4) {
      std::cout << "ERROR center removed lattice not the same lattice\n";
   }
}


// Function to test all centering transformation matrices
void TestAllCenteringTransformations() {
   std::cout << "===== Testing All Centering Transformation Matrices =====" << std::endl;

   // Define all centering matrices
   const Matrix_3x3 A_to_P(1.0, 0.0, 0.0, 0.0, 0.5, -0.5, 0.0, 0.5, 0.5);
   const Matrix_3x3 B_to_P(0.5, 0.0, -0.5, 0.0, 1.0, 0.0, 0.5, 0.0, 0.5);
   const Matrix_3x3 C_to_P(1.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.0, -0.5, 0.5);
   const Matrix_3x3 I_to_P(0.5, 0.5, -0.5, -0.5, 0.5, 0.5, 0.5, -0.5, 0.5);
   const Matrix_3x3 F_to_P(0.5, 0.0, 0.5, 0.5, 0.5, 0.0, 0.0, 0.5, 0.5);
   const Matrix_3x3 R_to_P(2.0 / 3.0, -1.0 / 3.0, -1.0 / 3.0,
      1.0 / 3.0, 1.0 / 3.0, -2.0 / 3.0,
      1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0);

   // Test cells
   LRL_Cell orthoCell(10.0, 15.0, 20.0, 90.0, 90.0, 90.0); // Orthorhombic
   LRL_Cell monoCell(10.0, 15.0, 20.0, 90.0, 100.0, 90.0); // Monoclinic
   LRL_Cell hexCell(10.0, 10.0, 15.0, 90.0, 90.0, 120.0);  // Hexagonal

   // Test each centering type
   std::cout << "\nTesting A-centered to Primitive:" << std::endl;
   testCenteringMatrix("A", A_to_P, monoCell);

   std::cout << "\nTesting B-centered to Primitive:" << std::endl;
   testCenteringMatrix("B", B_to_P, monoCell);

   std::cout << "\nTesting C-centered to Primitive:" << std::endl;
   testCenteringMatrix("C", C_to_P, monoCell);

   std::cout << "\nTesting I-centered to Primitive:" << std::endl;
   testCenteringMatrix("I", I_to_P, orthoCell);

   std::cout << "\nTesting F-centered to Primitive:" << std::endl;
   testCenteringMatrix("F", F_to_P, orthoCell);

   std::cout << "\nTesting R (rhombohedral in hex) to Primitive:" << std::endl;
   testCenteringMatrix("R", R_to_P, hexCell);

   // Test inverse transformations
   std::cout << "\n\nTesting Inverse Transformations (P to Centered):" << std::endl;
   testInverseTransformations();
}

// Test the inverse transformations
void testInverseTransformations() {
   // Test that X_to_P * P_to_X = Identity
   const Matrix_3x3 A_to_P(1.0, 0.0, 0.0, 0.0, 0.5, -0.5, 0.0, 0.5, 0.5);
   const Matrix_3x3 P_to_A = A_to_P.Inver();

   Matrix_3x3 identity_test = A_to_P * P_to_A;
   std::cout << "A_to_P * P_to_A should be identity:" << std::endl;
   std::cout << identity_test << std::endl;

   // Check how close to identity
   double max_diff = 0.0;
   for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
         double expected = (i == j) ? 1.0 : 0.0;
         double actual = identity_test[i * 3 + j];
         max_diff = std::max(max_diff, std::abs(actual - expected));
      }
   }
   std::cout << "Maximum deviation from identity: " << max_diff << std::endl;
}

// Special test for rhombohedral cells
void testRhombohedralTransformation() {
   std::cout << "\n===== Testing Rhombohedral Transformations =====" << std::endl;

   // Create a rhombohedral cell in hexagonal setting
   // a = b ≠ c, α = β = 90°, γ = 120°
   LRL_Cell hexRhombCell(10.0, 10.0, 15.0, 90.0, 90.0, 120.0);

   const Matrix_3x3 R_to_P(2.0 / 3.0, -1.0 / 3.0, -1.0 / 3.0,
      1.0 / 3.0, 1.0 / 3.0, -2.0 / 3.0,
      1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0);

   // Apply transformation
   double alpha = hexRhombCell[3] * M_PI / 180.0;
   double beta = hexRhombCell[4] * M_PI / 180.0;
   double gamma = hexRhombCell[5] * M_PI / 180.0;

   Vector_3 a_hex(hexRhombCell[0], 0.0, 0.0);
   Vector_3 b_hex(hexRhombCell[1] * cos(gamma), hexRhombCell[1] * sin(gamma), 0.0);
   Vector_3 c_hex(hexRhombCell[2] * cos(beta),
      hexRhombCell[2] * (cos(alpha) - cos(beta) * cos(gamma)) / sin(gamma),
      hexRhombCell[2] * sqrt(1.0 - cos(beta) * cos(beta) -
         pow((cos(alpha) - cos(beta) * cos(gamma)) / sin(gamma), 2)));

   // Transform to rhombohedral primitive
   Vector_3 a_rhomb = R_to_P * a_hex;
   Vector_3 b_rhomb = R_to_P * b_hex;
   Vector_3 c_rhomb = R_to_P * c_hex;

   // Calculate the rhombohedral cell parameters
   double a_r = a_rhomb.Norm();
   double b_r = b_rhomb.Norm();
   double c_r = c_rhomb.Norm();

   double alpha_r = acos(b_rhomb.Dot(c_rhomb) / (b_r * c_r)) * 180.0 / M_PI;
   double beta_r = acos(a_rhomb.Dot(c_rhomb) / (a_r * c_r)) * 180.0 / M_PI;
   double gamma_r = acos(a_rhomb.Dot(b_rhomb) / (a_r * b_r)) * 180.0 / M_PI;

   std::cout << "Hexagonal setting: " << hexRhombCell << std::endl;
   std::cout << "Rhombohedral primitive: "
      << a_r << " " << b_r << " " << c_r << " "
      << alpha_r << " " << beta_r << " " << gamma_r << std::endl;

   // For a true rhombohedral cell, we should have a = b = c and α = β = γ
   std::cout << "Rhombohedral check (should have equal axes and angles):" << std::endl;
   std::cout << "  Axes: a=" << a_r << ", b=" << b_r << ", c=" << c_r << std::endl;
   std::cout << "  Angles: α=" << alpha_r << ", β=" << beta_r << ", γ=" << gamma_r << std::endl;
}

#endif // CENTERINGTEST_H