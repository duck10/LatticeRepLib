#include "ReduceTransformsTest.h"

#include <iostream>

#include "ReduceTransformsTest.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include <string>
#include <sstream>
#include <cmath>

#include "G6.h"
#include "LRL_Cell.h"
#include "LRL_Vector3.h"
#include "Niggli.h"
#include "PairReporter.h"
#include "Polar.h"
#include "StoreResults.h"

static const double M_PI = 4.0 * atan(1.0);


// Simple test function to verify our ReduceWithTransforms solution
void TestSingleCell() {
   std::cout << "Testing ReduceWithTransforms on a single cell..." << std::endl;

   // Use one of the cells from the failed tests
   G6 inputG6(37.646, 37.699, 2.358, 6.035, 7.434, 16.517);

   MatG6 mG6;
   Matrix_3x3 m3d;
   G6 reducedG6;

   bool success = Niggli::ReduceWithTransforms(inputG6, mG6, m3d, reducedG6, 1.0E-10);

   std::cout << "Success: " << (success ? "Yes" : "No") << std::endl;
   std::cout << "Input G6: " << inputG6 << std::endl;
   std::cout << "Reduced G6: " << reducedG6 << std::endl;
   std::cout << "3D Matrix: " << std::endl << m3d << std::endl;
   std::cout << "Determinant: " << m3d.Det() << std::endl;

   // Verify that the 3D matrix correctly transforms the input cell to the reduced cell
   LRL_Cell inputCell(inputG6);
   double alpha = inputCell[3] * M_PI / 180.0;
   double beta = inputCell[4] * M_PI / 180.0;
   double gamma = inputCell[5] * M_PI / 180.0;

   // Create the input cell vectors in Cartesian coordinates
   Vector_3 a1(inputCell[0], 0.0, 0.0);

   Vector_3 b1(inputCell[1] * cos(gamma),
      inputCell[1] * sin(gamma),
      0.0);

   Vector_3 c1(inputCell[2] * cos(beta),
      inputCell[2] * (cos(alpha) - cos(beta) * cos(gamma)) / sin(gamma),
      inputCell[2] * sqrt(1.0 - cos(beta) * cos(beta) -
         pow((cos(alpha) - cos(beta) * cos(gamma)) / sin(gamma), 2)));

   // Apply the 3D transformation
   Vector_3 a2 = m3d * a1;
   Vector_3 b2 = m3d * b1;
   Vector_3 c2 = m3d * c1;

   // Calculate the G6 from the transformed vectors
   double g1 = a2.SquaredLength();
   double g2 = b2.SquaredLength();
   double g3 = c2.SquaredLength();
   double g4 = 2.0 * b2.Dot(c2);
   double g5 = 2.0 * a2.Dot(c2);
   double g6 = 2.0 * a2.Dot(b2);

   G6 calculatedG6(g1, g2, g3, g4, g5, g6);

   std::cout << "Calculated G6 from 3D transform: " << calculatedG6 << std::endl;

   // Calculate the maximum difference between the expected reduced G6 and the calculated G6
   double maxDiff = 0.0;
   for (int i = 0; i < 6; ++i) {
      maxDiff = std::max(maxDiff, std::abs(calculatedG6[i] - reducedG6[i]));
   }
   std::cout << "Maximum difference: " << maxDiff << std::endl;

   // Check LRL_Cell parameters as well
   LRL_Cell reducedCell(reducedG6);
   LRL_Cell transformedCell(calculatedG6);

   std::cout << "Reduced cell parameters: " << reducedCell << std::endl;
   std::cout << "Transformed cell parameters: " << transformedCell << std::endl;

   // Also test with a monoclinic cell
   std::cout << "\nTesting with a monoclinic cell..." << std::endl;
   G6 monoclinicG6(10.0, 20.0, 10.0, 0.0, -10.0, 0.0);  // a=c, alpha=gamma=90, beta=120

   success = Niggli::ReduceWithTransforms(monoclinicG6, mG6, m3d, reducedG6, 1.0E-10);

   std::cout << "Success: " << (success ? "Yes" : "No") << std::endl;
   std::cout << "Input G6: " << monoclinicG6 << std::endl;
   std::cout << "Reduced G6: " << reducedG6 << std::endl;
   std::cout << "3D Matrix: " << std::endl << m3d << std::endl;
   std::cout << "Determinant: " << m3d.Det() << std::endl;

   // Verify transformation for monoclinic cell
   inputCell = LRL_Cell(monoclinicG6);
   alpha = inputCell[3] * M_PI / 180.0;
   beta = inputCell[4] * M_PI / 180.0;
   gamma = inputCell[5] * M_PI / 180.0;

   // Create the input cell vectors in Cartesian coordinates
   a1 = Vector_3(inputCell[0], 0.0, 0.0);

   b1 = Vector_3(inputCell[1] * cos(gamma),
      inputCell[1] * sin(gamma),
      0.0);

   c1 = Vector_3(inputCell[2] * cos(beta),
      inputCell[2] * (cos(alpha) - cos(beta) * cos(gamma)) / sin(gamma),
      inputCell[2] * sqrt(1.0 - cos(beta) * cos(beta) -
         pow((cos(alpha) - cos(beta) * cos(gamma)) / sin(gamma), 2)));

   // Apply the 3D transformation
   a2 = m3d * a1;
   b2 = m3d * b1;
   c2 = m3d * c1;

   // Calculate the G6 from the transformed vectors
   g1 = a2.SquaredLength();
   g2 = b2.SquaredLength();
   g3 = c2.SquaredLength();
   g4 = 2.0 * b2.Dot(c2);
   g5 = 2.0 * a2.Dot(c2);
   g6 = 2.0 * a2.Dot(b2);

   calculatedG6 = G6(g1, g2, g3, g4, g5, g6);

   std::cout << "Calculated G6 from 3D transform: " << calculatedG6 << std::endl;

   // Calculate the maximum difference
   maxDiff = 0.0;
   for (int i = 0; i < 6; ++i) {
      maxDiff = std::max(maxDiff, std::abs(calculatedG6[i] - reducedG6[i]));
   }
   std::cout << "Maximum difference: " << maxDiff << std::endl;

   // Also test with a rhombohedral cell
   std::cout << "\nTesting with a rhombohedral cell..." << std::endl;
   G6 rhombohedralG6(8.0, 8.0, 8.0, -1.0, -1.0, -1.0);

   success = Niggli::ReduceWithTransforms(rhombohedralG6, mG6, m3d, reducedG6, 1.0E-10);

   std::cout << "Success: " << (success ? "Yes" : "No") << std::endl;
   std::cout << "Input G6: " << rhombohedralG6 << std::endl;
   std::cout << "Reduced G6: " << reducedG6 << std::endl;
   std::cout << "3D Matrix: " << std::endl << m3d << std::endl;
   std::cout << "Determinant: " << m3d.Det() << std::endl;

   // Verify transformation for rhombohedral cell
   inputCell = LRL_Cell(rhombohedralG6);
   alpha = inputCell[3] * M_PI / 180.0;
   beta = inputCell[4] * M_PI / 180.0;
   gamma = inputCell[5] * M_PI / 180.0;

   // Create the input cell vectors in Cartesian coordinates
   a1 = Vector_3(inputCell[0], 0.0, 0.0);

   b1 = Vector_3(inputCell[1] * cos(gamma),
      inputCell[1] * sin(gamma),
      0.0);

   c1 = Vector_3(inputCell[2] * cos(beta),
      inputCell[2] * (cos(alpha) - cos(beta) * cos(gamma)) / sin(gamma),
      inputCell[2] * sqrt(1.0 - cos(beta) * cos(beta) -
         pow((cos(alpha) - cos(beta) * cos(gamma)) / sin(gamma), 2)));

   // Apply the 3D transformation
   a2 = m3d * a1;
   b2 = m3d * b1;
   c2 = m3d * c1;

   // Calculate the G6 from the transformed vectors
   g1 = a2.SquaredLength();
   g2 = b2.SquaredLength();
   g3 = c2.SquaredLength();
   g4 = 2.0 * b2.Dot(c2);
   g5 = 2.0 * a2.Dot(c2);
   g6 = 2.0 * a2.Dot(b2);

   calculatedG6 = G6(g1, g2, g3, g4, g5, g6);

   std::cout << "Calculated G6 from 3D transform: " << calculatedG6 << std::endl;

   // Calculate the maximum difference
   maxDiff = 0.0;
   for (int i = 0; i < 6; ++i) {
      maxDiff = std::max(maxDiff, std::abs(calculatedG6[i] - reducedG6[i]));
   }
   std::cout << "Maximum difference: " << maxDiff << std::endl;
}

// Function to test the correspondence between a G6 transformation matrix and its 3D counterpart
void TestMatrixCorrespondence(const MatG6& matG6, const Matrix_3x3& mat3D, const std::string& matrixName, const G6& testCell = G6()) {
   std::cout << "=== Testing " << matrixName << " ===" << std::endl;

   // Use provided test cell or generate a random one
   G6 randomCell;
   if (testCell.IsValid()) {
      randomCell = testCell;
      std::cout << "Using provided test cell: " << randomCell << std::endl;
   }
   else {
      // Generate a proper random cell using either G6::rand() or Polar::rand()
      // Option 1: Using G6::rand() if available
      // randomCell = G6::rand();

      // Option 2: Using Polar::rand() which returns an LRL_Cell
      LRL_Cell polarCell = Polar::rand();
      randomCell = G6(polarCell);

      std::cout << "Generated random cell: " << randomCell << std::endl;
      std::cout << "Cell parameters: " << LRL_Cell(randomCell) << std::endl;
   }

   // Apply the G6 transformation
   G6 transformedG6 = matG6 * randomCell;
   std::cout << "G6-transformed cell: " << transformedG6 << std::endl;

   // Convert the input cell to real space basis vectors
   LRL_Cell inputCell(randomCell);
   double alpha = inputCell[3] * M_PI / 180.0;  // Convert to radians
   double beta = inputCell[4] * M_PI / 180.0;
   double gamma = inputCell[5] * M_PI / 180.0;

   // Create input cell basis vectors in Cartesian coordinates
   Vector_3 a1(inputCell[0], 0.0, 0.0);

   Vector_3 b1(inputCell[1] * cos(gamma),
      inputCell[1] * sin(gamma),
      0.0);

   double cx1 = inputCell[2] * cos(beta);
   double cy1 = inputCell[2] * (cos(alpha) - cos(beta) * cos(gamma)) / sin(gamma);
   double cz1 = inputCell[2] * sqrt(1.0 - cos(beta) * cos(beta) -
      pow((cos(alpha) - cos(beta) * cos(gamma)) / sin(gamma), 2));

   Vector_3 c1(cx1, cy1, cz1);

   // Apply the 3D transformation
   Vector_3 a2 = mat3D * a1;
   Vector_3 b2 = mat3D * b1;
   Vector_3 c2 = mat3D * c1;

   // Calculate the G6 parameters from the transformed basis vectors
   double g1 = a2.SquaredLength();
   double g2 = b2.SquaredLength();
   double g3 = c2.SquaredLength();
   double g4 = 2.0 * b2.Dot(c2);
   double g5 = 2.0 * a2.Dot(c2);
   double g6 = 2.0 * a2.Dot(b2);

   G6 calculatedG6(g1, g2, g3, g4, g5, g6);
   std::cout << "3D-transformed cell (calculated G6): " << calculatedG6 << std::endl;

   // Calculate the difference between the G6-transformed and 3D-transformed results
   double maxDiff = 0.0;
   double relMaxDiff = 0.0;
   for (int i = 0; i < 6; ++i) {
      double diff = std::abs(transformedG6[i] - calculatedG6[i]);
      double relDiff = diff / (std::abs(transformedG6[i]) + 1e-10);  // Avoid division by zero
      maxDiff = std::max(maxDiff, diff);
      relMaxDiff = std::max(relMaxDiff, relDiff);
   }

   std::cout << "Absolute maximum difference: " << maxDiff << std::endl;
   std::cout << "Relative maximum difference: " << relMaxDiff << std::endl;

   // Check individual components
   for (int i = 0; i < 6; ++i) {
      double diff = transformedG6[i] - calculatedG6[i];
      double relDiff = diff / (std::abs(transformedG6[i]) + 1e-10);
      std::cout << "G6[" << i << "]: " << transformedG6[i] << " vs " << calculatedG6[i]
         << " (diff: " << diff << ", rel: " << relDiff << ")" << std::endl;
   }

   // Convert both to LRL_Cell for a more intuitive comparison
   LRL_Cell transformedCell(transformedG6);
   LRL_Cell calculatedCell(calculatedG6);

   std::cout << "G6-transformed cell parameters: " << transformedCell << std::endl;
   std::cout << "3D-transformed cell parameters: " << calculatedCell << std::endl;

   // Print the 3D matrix information
   std::cout << "3D Matrix:" << std::endl << mat3D << std::endl;
   std::cout << "Determinant: " << mat3D.Det() << std::endl;

   // Check if the matrix has integer values
   bool isInteger = true;
   for (int i = 0; i < 9; ++i) {
      double val = mat3D[i];
      double roundedVal = round(val);
      if (std::abs(val - roundedVal) > 1e-6) {
         isInteger = false;
         break;
      }
   }

   std::cout << "Matrix has integer values: " << (isInteger ? "Yes" : "No") << std::endl;

   // Print the basis vectors before and after transformation
   std::cout << "Original basis vectors:" << std::endl;
   std::cout << "a1: " << a1 << std::endl;
   std::cout << "b1: " << b1 << std::endl;
   std::cout << "c1: " << c1 << std::endl;

   std::cout << "Transformed basis vectors:" << std::endl;
   std::cout << "a2: " << a2 << std::endl;
   std::cout << "b2: " << b2 << std::endl;
   std::cout << "c2: " << c2 << std::endl;

   std::cout << std::endl;
}


// Function to test all standard matrix pairs
void TestAllMatrixPairs() {
   // Define the 3D matrices corresponding to G6 transformations
   // Standard presentation matrices
   const Matrix_3x3 spnull_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0); // Identity

   const Matrix_3x3 sp1_3D(0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
   const Matrix_3x3 sp2_3D(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);

   const Matrix_3x3 sp34a_3D(1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, -1.0);
   const Matrix_3x3 sp34b_3D(-1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0);
   const Matrix_3x3 sp34c_3D(-1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 1.0);

   // Reduction operation matrices - these need verification
   const Matrix_3x3 R5_Plus_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, -1.0, 1.0);
   const Matrix_3x3 R5_Minus_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0);

   const Matrix_3x3 R6_Plus_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, 0.0, 1.0);
   const Matrix_3x3 R6_Minus_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);

   const Matrix_3x3 R7_Plus_3D(1.0, 0.0, 0.0, -1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
   const Matrix_3x3 R7_Minus_3D(1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0);

   const Matrix_3x3 R8_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0);
   const Matrix_3x3 R12_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, -1.0, -1.0);

   // G6 matrices used in standard Niggli reduction
   const static MatG6 spnull("1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 1");
   const static MatG6 sp1("0 1 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0 0 0 1");
   const static MatG6 sp2("1 0 0 0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 1 0");
   const static MatG6 sp34a("1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 -1 0 0 0 0 0 0 -1 0 0 0 0 0 0 1");
   const static MatG6 sp34b("1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 -1 0 0 0 0 0 0 1 0 0 0 0 0 0 -1");
   const static MatG6 sp34c("1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 -1 0 0 0 0 0 0 -1");

   const static MatG6 R5_Plus(" 1 0 0 0 0 0  0 1 0 0 0 0  0 1 1 -1 0 0  0 -2 0 1 0 0  0 0 0 0 1 -1  0 0 0 0 0 1");
   const static MatG6 R5_Minus(" 1 0 0 0 0 0  0 1 0 0 0 0  0 1 1 +1 0 0  0 +2 0 1 0 0  0 0 0 0 1 +1  0 0 0 0 0 1");
   const static MatG6 R6_Plus(" 1 0 0 0 0 0  0 1 0 0 0 0  1 0 1 0 -1 0  0 0 0 1 0 -1  -2 0 0 0 1 0  0 0 0 0 0 1");
   const static MatG6 R6_Minus(" 1 0 0 0 0 0  0 1 0 0 0 0  1 0 1 0 +1 0  0 0 0 1 0 +1  +2 0 0 0 1 0  0 0 0 0 0 1");
   const static MatG6 R7_Plus(" 1 0 0 0 0 0  1 1 0 0 0 -1  0 0 1 0 0 0  0 0 0 1 -1 0  0 0 0 0 1 0  -2 0 0 0 0 1");
   const static MatG6 R7_Minus(" 1 0 0 0 0 0  1 1 0 0 0 +1  0 0 1 0 0 0  0 0 0 1 +1 0  0 0 0 0 1 0  +2 0 0 0 0 1");
   const static MatG6 R8(" 1 0 0 0 0 0  0 1 0 0 0 0  1 1 1 1 1 1  0 2 0 1 0 1  2 0 0 0 1 1  0 0 0 0 0 1");
   const static MatG6 R12("1 0 0 0 0 0  0 1 0 0 0 0  1 1 1 1 1 1  0 -2 0 -1 0 -1  -2 0 0 0 -1 -1  0 0 0 0 0 1");

   // Create a standard test cell that will be used for all tests for consistency
   G6 testCell(10.0, 15.0, 20.0, 5.0, 8.0, 12.0);  // A cell with all positive components

   // Test each matrix pair
   TestMatrixCorrespondence(spnull, spnull_3D, "spnull (Identity)", testCell);
   TestMatrixCorrespondence(sp1, sp1_3D, "sp1 (Swap a,b)", testCell);
   TestMatrixCorrespondence(sp2, sp2_3D, "sp2 (Swap b,c)", testCell);
   TestMatrixCorrespondence(sp34a, sp34a_3D, "sp34a (Flip sign of g4, g5)", testCell);
   TestMatrixCorrespondence(sp34b, sp34b_3D, "sp34b (Flip sign of g4, g6)", testCell);
   TestMatrixCorrespondence(sp34c, sp34c_3D, "sp34c (Flip sign of g5, g6)", testCell);

   // Test the reduction matrices
   TestMatrixCorrespondence(R5_Plus, R5_Plus_3D, "R5_Plus", testCell);
   TestMatrixCorrespondence(R5_Minus, R5_Minus_3D, "R5_Minus", testCell);
   TestMatrixCorrespondence(R6_Plus, R6_Plus_3D, "R6_Plus", testCell);
   TestMatrixCorrespondence(R6_Minus, R6_Minus_3D, "R6_Minus", testCell);
   TestMatrixCorrespondence(R7_Plus, R7_Plus_3D, "R7_Plus", testCell);
   TestMatrixCorrespondence(R7_Minus, R7_Minus_3D, "R7_Minus", testCell);
   TestMatrixCorrespondence(R8, R8_3D, "R8", testCell);
   TestMatrixCorrespondence(R12, R12_3D, "R12", testCell);

   // Also test with a few specific cells to check for issues
   G6 monoclinicCell(10.0, 20.0, 10.0, 0.0, -10.0, 0.0);  // a=c, alpha=gamma=90, beta=120
   TestMatrixCorrespondence(R5_Plus, R5_Plus_3D, "R5_Plus with monoclinic cell", monoclinicCell);

   G6 rhombohedralCell(8.0, 8.0, 8.0, -1.0, -1.0, -1.0);
   TestMatrixCorrespondence(R6_Plus, R6_Plus_3D, "R6_Plus with rhombohedral cell", rhombohedralCell);
}

// Testing function with systematic sign variations
void TestSignVariations() {
   // Define a test cell
   G6 testCell(10.0, 15.0, 20.0, 5.0, 8.0, 12.0);

   // Define the base R5_Plus matrix from G6 and 3D
   const static MatG6 R5_Plus(" 1 0 0 0 0 0  0 1 0 0 0 0  0 1 1 -1 0 0  0 -2 0 1 0 0  0 0 0 0 1 -1  0 0 0 0 0 1");

   // Test different sign combinations for the 3D matrix
   Matrix_3x3 R5_Plus_3D_v1(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, -1.0, 1.0);   // Original
   Matrix_3x3 R5_Plus_3D_v2(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, -1.0);   // Flipped c signs
   Matrix_3x3 R5_Plus_3D_v3(1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 1.0, 1.0);   // Flipped b sign
   Matrix_3x3 R5_Plus_3D_v4(1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, -1.0); // Flipped b,c signs

   // Test each variation
   TestMatrixCorrespondence(R5_Plus, R5_Plus_3D_v1, "R5_Plus variant 1 (original)", testCell);
   TestMatrixCorrespondence(R5_Plus, R5_Plus_3D_v2, "R5_Plus variant 2 (flipped c signs)", testCell);
   TestMatrixCorrespondence(R5_Plus, R5_Plus_3D_v3, "R5_Plus variant 3 (flipped b sign)", testCell);
   TestMatrixCorrespondence(R5_Plus, R5_Plus_3D_v4, "R5_Plus variant 4 (flipped b,c signs)", testCell);

   // Similarly, test R6_Plus with sign variations
   const static MatG6 R6_Plus(" 1 0 0 0 0 0  0 1 0 0 0 0  1 0 1 0 -1 0  0 0 0 1 0 -1  -2 0 0 0 1 0  0 0 0 0 0 1");

   Matrix_3x3 R6_Plus_3D_v1(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, 0.0, 1.0);   // Original
   Matrix_3x3 R6_Plus_3D_v2(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, -1.0);   // Flipped c signs
   Matrix_3x3 R6_Plus_3D_v3(-1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);   // Flipped a sign
   Matrix_3x3 R6_Plus_3D_v4(-1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, 0.0, -1.0); // Flipped a,c signs

   TestMatrixCorrespondence(R6_Plus, R6_Plus_3D_v1, "R6_Plus variant 1 (original)", testCell);
   TestMatrixCorrespondence(R6_Plus, R6_Plus_3D_v2, "R6_Plus variant 2 (flipped c signs)", testCell);
   TestMatrixCorrespondence(R6_Plus, R6_Plus_3D_v3, "R6_Plus variant 3 (flipped a sign)", testCell);
   TestMatrixCorrespondence(R6_Plus, R6_Plus_3D_v4, "R6_Plus variant 4 (flipped a,c signs)", testCell);
}

// Function to explore all 8 possible sign combinations for a 3D matrix
void ExploreMatrixSigns(const MatG6& matG6, const Matrix_3x3& baseMatrix, const std::string& matrixName, const G6& testCell) {
   std::cout << "======= Exploring all sign combinations for " << matrixName << " =======" << std::endl;

   // Extract the base matrix elements with their absolute values
   double elements[9];
   for (int i = 0; i < 9; ++i) {
      elements[i] = std::abs(baseMatrix[i]);
   }

   // Get the original signs to determine non-zero elements
   bool nonZero[9];
   int originalSign[9];
   for (int i = 0; i < 9; ++i) {
      nonZero[i] = std::abs(baseMatrix[i]) > 1e-10;
      originalSign[i] = baseMatrix[i] > 0 ? 1 : (baseMatrix[i] < 0 ? -1 : 0);
   }

   // Find best matrix and its error
   double bestError = DBL_MAX;
   Matrix_3x3 bestMatrix = baseMatrix;

   // Test all 256 possible sign combinations (this is exhaustive but includes many invalid matrices)
   for (int signCombo = 0; signCombo < 256; ++signCombo) {
      // Skip combinations that flip signs of zero elements
      bool skipCombo = false;
      for (int i = 0; i < 9; ++i) {
         if (!nonZero[i] && ((signCombo >> i) & 1)) {
            skipCombo = true;
            break;
         }
      }
      if (skipCombo) continue;

      // Generate matrix with this sign combination
      Matrix_3x3 testMatrix;
      for (int i = 0; i < 9; ++i) {
         if (nonZero[i]) {
            int sign = ((signCombo >> i) & 1) ? -1 : 1;
            testMatrix[i] = sign * elements[i];
         }
         else {
            testMatrix[i] = 0.0;
         }
      }

      // Skip if determinant is 0 (singular matrix)
      double det = testMatrix.Det();
      if (std::abs(det) < 1e-10) continue;

      // Apply the G6 transformation
      G6 transformedG6 = matG6 * testCell;

      // Convert the test cell to real space basis vectors
      LRL_Cell inputCell(testCell);
      double alpha = inputCell[3] * M_PI / 180.0;
      double beta = inputCell[4] * M_PI / 180.0;
      double gamma = inputCell[5] * M_PI / 180.0;

      // Create input cell basis vectors in Cartesian coordinates
      Vector_3 a1(inputCell[0], 0.0, 0.0);

      Vector_3 b1(inputCell[1] * cos(gamma),
         inputCell[1] * sin(gamma),
         0.0);

      double cx1 = inputCell[2] * cos(beta);
      double cy1 = inputCell[2] * (cos(alpha) - cos(beta) * cos(gamma)) / sin(gamma);
      double cz1 = inputCell[2] * sqrt(1.0 - cos(beta) * cos(beta) -
         pow((cos(alpha) - cos(beta) * cos(gamma)) / sin(gamma), 2));

      Vector_3 c1(cx1, cy1, cz1);

      // Apply the 3D transformation
      Vector_3 a2 = testMatrix * a1;
      Vector_3 b2 = testMatrix * b1;
      Vector_3 c2 = testMatrix * c1;

      // Calculate the G6 parameters from the transformed basis
      double g1 = a2.SquaredLength();
      double g2 = b2.SquaredLength();
      double g3 = c2.SquaredLength();
      double g4 = 2.0 * b2.Dot(c2);
      double g5 = 2.0 * a2.Dot(c2);
      double g6 = 2.0 * a2.Dot(b2);

      G6 calculatedG6(g1, g2, g3, g4, g5, g6);

      // Calculate the error
      double error = 0.0;
      for (int i = 0; i < 6; ++i) {
         error += std::abs(transformedG6[i] - calculatedG6[i]);
      }

      // If this is better than our current best, update
      if (error < bestError) {
         bestError = error;
         bestMatrix = testMatrix;

         // If error is very small, we found an excellent match
         if (error < 1e-6) {
            break;  // Stop search, we found an excellent match
         }
      }
   }

   // Test the best matrix
   std::cout << "Best matrix found with error: " << bestError << std::endl;
   std::cout << "Best matrix:" << std::endl << bestMatrix << std::endl;

   // Now test this matrix in detail
   TestMatrixCorrespondence(matG6, bestMatrix, matrixName + " (Best Match)", testCell);
}

// Function to explore key matrices with intelligent sign permutations
void ExploreKeyMatrices() {
   // Define the G6 matrices we want to test
   const static MatG6 R5_Plus(" 1 0 0 0 0 0  0 1 0 0 0 0  0 1 1 -1 0 0  0 -2 0 1 0 0  0 0 0 0 1 -1  0 0 0 0 0 1");
   const static MatG6 R5_Minus(" 1 0 0 0 0 0  0 1 0 0 0 0  0 1 1 +1 0 0  0 +2 0 1 0 0  0 0 0 0 1 +1  0 0 0 0 0 1");
   const static MatG6 R6_Plus(" 1 0 0 0 0 0  0 1 0 0 0 0  1 0 1 0 -1 0  0 0 0 1 0 -1  -2 0 0 0 1 0  0 0 0 0 0 1");
   const static MatG6 R6_Minus(" 1 0 0 0 0 0  0 1 0 0 0 0  1 0 1 0 +1 0  0 0 0 1 0 +1  +2 0 0 0 1 0  0 0 0 0 0 1");
   const static MatG6 R7_Plus(" 1 0 0 0 0 0  1 1 0 0 0 -1  0 0 1 0 0 0  0 0 0 1 -1 0  0 0 0 0 1 0  -2 0 0 0 0 1");
   const static MatG6 R7_Minus(" 1 0 0 0 0 0  1 1 0 0 0 +1  0 0 1 0 0 0  0 0 0 1 +1 0  0 0 0 0 1 0  +2 0 0 0 0 1");
   const static MatG6 R8(" 1 0 0 0 0 0  0 1 0 0 0 0  1 1 1 1 1 1  0 2 0 1 0 1  2 0 0 0 1 1  0 0 0 0 0 1");
   const static MatG6 R12("1 0 0 0 0 0  0 1 0 0 0 0  1 1 1 1 1 1  0 -2 0 -1 0 -1  -2 0 0 0 -1 -1  0 0 0 0 0 1");

   // Define our base 3D matrices to explore
   const Matrix_3x3 R5_Plus_3D_base(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, -1.0, 1.0);
   const Matrix_3x3 R5_Minus_3D_base(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0);
   const Matrix_3x3 R6_Plus_3D_base(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, 0.0, 1.0);
   const Matrix_3x3 R6_Minus_3D_base(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
   const Matrix_3x3 R7_Plus_3D_base(1.0, 0.0, 0.0, -1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
   const Matrix_3x3 R7_Minus_3D_base(1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
   const Matrix_3x3 R8_3D_base(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0);
   const Matrix_3x3 R12_3D_base(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, -1.0, -1.0);

   // Use a well-behaved test cell for consistent results
   G6 testCell(10.0, 15.0, 20.0, 5.0, 8.0, 12.0);  // A cell with all positive components

   // Explore each matrix
   ExploreMatrixSigns(R5_Plus, R5_Plus_3D_base, "R5_Plus", testCell);
   ExploreMatrixSigns(R5_Minus, R5_Minus_3D_base, "R5_Minus", testCell);
   ExploreMatrixSigns(R6_Plus, R6_Plus_3D_base, "R6_Plus", testCell);
   ExploreMatrixSigns(R6_Minus, R6_Minus_3D_base, "R6_Minus", testCell);
   ExploreMatrixSigns(R7_Plus, R7_Plus_3D_base, "R7_Plus", testCell);
   ExploreMatrixSigns(R7_Minus, R7_Minus_3D_base, "R7_Minus", testCell);
   ExploreMatrixSigns(R8, R8_3D_base, "R8", testCell);
   ExploreMatrixSigns(R12, R12_3D_base, "R12", testCell);

   // Also test with a few other cell types
   G6 monoclinicCell(10.0, 20.0, 10.0, 0.0, -10.0, 0.0);  // Monoclinic cell
   G6 rhombohedralCell(8.0, 8.0, 8.0, -1.0, -1.0, -1.0);  // Rhombohedral cell

   // Test a few key matrices with these different cell types
   ExploreMatrixSigns(R5_Plus, R5_Plus_3D_base, "R5_Plus with monoclinic cell", monoclinicCell);
   ExploreMatrixSigns(R6_Plus, R6_Plus_3D_base, "R6_Plus with rhombohedral cell", rhombohedralCell);
}

// Function to derive the correct 3D matrices directly from G6 operations
void DeriveCorrect3DMatrices() {
   std::cout << "============ Deriving correct 3D matrices ============" << std::endl;

   // Create a test cell and convert to basis vectors
   G6 testCell(10.0, 15.0, 20.0, 5.0, 8.0, 12.0);
   LRL_Cell inputCell(testCell);

   double alpha = inputCell[3] * M_PI / 180.0;
   double beta = inputCell[4] * M_PI / 180.0;
   double gamma = inputCell[5] * M_PI / 180.0;

   // Create basis vectors for the input cell
   Vector_3 a1(inputCell[0], 0.0, 0.0);

   Vector_3 b1(inputCell[1] * cos(gamma),
      inputCell[1] * sin(gamma),
      0.0);

   double cx1 = inputCell[2] * cos(beta);
   double cy1 = inputCell[2] * (cos(alpha) - cos(beta) * cos(gamma)) / sin(gamma);
   double cz1 = inputCell[2] * sqrt(1.0 - cos(beta) * cos(beta) -
      pow((cos(alpha) - cos(beta) * cos(gamma)) / sin(gamma), 2));

   Vector_3 c1(cx1, cy1, cz1);

   // Define the operations we want to derive matrices for
   struct Operation {
      std::string name;
      MatG6 matG6;
   };

   std::vector<Operation> operations = {
       {"R5_Plus", MatG6(" 1 0 0 0 0 0  0 1 0 0 0 0  0 1 1 -1 0 0  0 -2 0 1 0 0  0 0 0 0 1 -1  0 0 0 0 0 1")},
       {"R5_Minus", MatG6(" 1 0 0 0 0 0  0 1 0 0 0 0  0 1 1 +1 0 0  0 +2 0 1 0 0  0 0 0 0 1 +1  0 0 0 0 0 1")},
       {"R6_Plus", MatG6(" 1 0 0 0 0 0  0 1 0 0 0 0  1 0 1 0 -1 0  0 0 0 1 0 -1  -2 0 0 0 1 0  0 0 0 0 0 1")},
       {"R6_Minus", MatG6(" 1 0 0 0 0 0  0 1 0 0 0 0  1 0 1 0 +1 0  0 0 0 1 0 +1  +2 0 0 0 1 0  0 0 0 0 0 1")},
       {"R7_Plus", MatG6(" 1 0 0 0 0 0  1 1 0 0 0 -1  0 0 1 0 0 0  0 0 0 1 -1 0  0 0 0 0 1 0  -2 0 0 0 0 1")},
       {"R7_Minus", MatG6(" 1 0 0 0 0 0  1 1 0 0 0 +1  0 0 1 0 0 0  0 0 0 1 +1 0  0 0 0 0 1 0  +2 0 0 0 0 1")},
       {"R8", MatG6(" 1 0 0 0 0 0  0 1 0 0 0 0  1 1 1 1 1 1  0 2 0 1 0 1  2 0 0 0 1 1  0 0 0 0 0 1")},
       {"R12", MatG6("1 0 0 0 0 0  0 1 0 0 0 0  1 1 1 1 1 1  0 -2 0 -1 0 -1  -2 0 0 0 -1 -1  0 0 0 0 0 1")}
   };

   // For each operation
   for (const auto& op : operations) {
      std::cout << "Deriving 3D matrix for " << op.name << std::endl;

      // Apply the G6 transformation
      G6 transformedG6 = op.matG6 * testCell;
      LRL_Cell transformedCell(transformedG6);

      std::cout << "  Input G6: " << testCell << std::endl;
      std::cout << "  Transformed G6: " << transformedG6 << std::endl;

      // Create basis vectors for the transformed cell
      double alphaTrans = transformedCell[3] * M_PI / 180.0;
      double betaTrans = transformedCell[4] * M_PI / 180.0;
      double gammaTrans = transformedCell[5] * M_PI / 180.0;

      Vector_3 a2(transformedCell[0], 0.0, 0.0);

      Vector_3 b2(transformedCell[1] * cos(gammaTrans),
         transformedCell[1] * sin(gammaTrans),
         0.0);

      double cx2 = transformedCell[2] * cos(betaTrans);
      double cy2 = transformedCell[2] * (cos(alphaTrans) - cos(betaTrans) * cos(gammaTrans)) / sin(gammaTrans);
      double cz2 = transformedCell[2] * sqrt(1.0 - cos(betaTrans) * cos(betaTrans) -
         pow((cos(alphaTrans) - cos(betaTrans) * cos(gammaTrans)) / sin(gammaTrans), 2));

      Vector_3 c2(cx2, cy2, cz2);

      // Now the challenge is to find a 3D matrix M such that:
      // M * a1 = a2
      // M * b1 = b2
      // M * c1 = c2

      // This is a complex problem, but for simple operations, we can try to
      // identify integer linear combinations that might work
      std::cout << "  Original basis vectors:" << std::endl;
      std::cout << "    a1: " << a1 << std::endl;
      std::cout << "    b1: " << b1 << std::endl;
      std::cout << "    c1: " << c1 << std::endl;

      std::cout << "  Transformed basis vectors:" << std::endl;
      std::cout << "    a2: " << a2 << std::endl;
      std::cout << "    b2: " << b2 << std::endl;
      std::cout << "    c2: " << c2 << std::endl;

      // Try to identify simple integer transformations
      // For example, check if a2 = a1, b2 = b1, c2 = c1 + b1
      // This would be R5_Plus with matrix [1 0 0; 0 1 0; 0 1 1]

      // Let's try to determine the 3D matrix directly
      // We'll set up a system of equations using the known vectors

      // Create matrices from the basis vectors
      Matrix_3x3 A1(
         a1[0], b1[0], c1[0],
         a1[1], b1[1], c1[1],
         a1[2], b1[2], c1[2]
      );

      Matrix_3x3 A2(
         a2[0], b2[0], c2[0],
         a2[1], b2[1], c2[1],
         a2[2], b2[2], c2[2]
      );

      // The transformation matrix M should satisfy M * A1 = A2
      // So M = A2 * A1^(-1)

      try {
         Matrix_3x3 A1_inv = A1.Inver();
         Matrix_3x3 M = A2 * A1_inv;

         // Now round the matrix elements to the nearest integer
         Matrix_3x3 M_rounded;
         for (int i = 0; i < 9; ++i) {
            M_rounded[i] = round(M[i]);
         }

         // Check if the rounded matrix is close to the original
         double maxDiff = 0.0;
         for (int i = 0; i < 9; ++i) {
            maxDiff = std::max(maxDiff, std::abs(M[i] - M_rounded[i]));
         }

         std::cout << "  Derived 3D matrix:" << std::endl << M << std::endl;
         std::cout << "  Rounded 3D matrix:" << std::endl << M_rounded << std::endl;
         std::cout << "  Max difference between original and rounded: " << maxDiff << std::endl;

         // Verify the rounded matrix
         Vector_3 a2_test = M_rounded * a1;
         Vector_3 b2_test = M_rounded * b1;
         Vector_3 c2_test = M_rounded * c1;

         double g1_test = a2_test.SquaredLength();
         double g2_test = b2_test.SquaredLength();
         double g3_test = c2_test.SquaredLength();
         double g4_test = 2.0 * b2_test.Dot(c2_test);
         double g5_test = 2.0 * a2_test.Dot(c2_test);
         double g6_test = 2.0 * a2_test.Dot(b2_test);

         G6 g6_testv(g1_test, g2_test, g3_test, g4_test, g5_test, g6_test);

         std::cout << "  G6 from rounded matrix: " << g6_testv << std::endl;

         // Calculate the error
         double error = 0.0;
         for (int i = 0; i < 6; ++i) {
            error += std::abs(transformedG6[i] - g6_testv[i]);
         }

         std::cout << "  Error with rounded matrix: " << error << std::endl;

         // If error is large, try sign flips
         if (error > 1e-6) {
            std::cout << "  Trying sign variants..." << std::endl;
            ExploreMatrixSigns(op.matG6, M_rounded, op.name, testCell);
         }
      }
      catch (...) {
         std::cout << "  Failed to compute inverse matrix." << std::endl;
      }

      std::cout << std::endl;
   }
}

// Test the ReduceWithTransforms method with many random cells
int TestReduceWithTransformsRandom(const size_t numTests) {
   std::cout << "Testing Niggli::ReduceWithTransforms with " << numTests << " random cells..." << std::endl;

   // Statistics
   size_t successCount = 0;
   size_t failCount = 0;
   size_t matrixMismatchCount = 0;
   std::vector<G6> failedCells;

   // Store detailed statistics using StoreResults
   StoreResults<double, std::string> transformStats(100);
   transformStats.SetTitle("ReduceWithTransforms Statistics");

   // Loop through test cases
   for (size_t test = 0; test < numTests; ++test) {
      // Generate a random cell
      LRL_Cell cell = Polar::rand();
      G6 inputG6(cell);

      // Perform standard Niggli reduction
      MatG6 mG6;
      G6 reducedG6;
      bool success = Niggli::Reduce(inputG6, mG6, reducedG6, 1.0E-10);

      if (!success) {
         std::cout << "Standard Niggli reduction failed for test " << test << std::endl;
         failCount++;
         transformStats.Store(0.0, "StandardReductionFailed");
         failedCells.push_back(inputG6);
         continue;
      }

      // Perform Niggli reduction with 3D transform
      MatG6 mG6_transformed;
      Matrix_3x3 m3d;
      G6 reducedG6_transformed;
      bool success_transformed = Niggli::ReduceWithTransforms(inputG6, mG6_transformed, m3d, reducedG6_transformed, 1.0E-10);

      if (!success_transformed) {
         std::cout << "ReduceWithTransforms failed for test " << test << std::endl;
         failCount++;
         transformStats.Store(0.0, "ReduceWithTransformsFailed");
         failedCells.push_back(inputG6);
         continue;
      }

      // Compare the reduced G6 vectors from both methods
      double g6Diff = 0.0;
      for (int i = 0; i < 6; ++i) {
         g6Diff = std::max(g6Diff, std::abs(reducedG6[i] - reducedG6_transformed[i]));
      }

      if (g6Diff > 1.0E-8) {
         std::cout << "Reduced G6 vectors don't match for test " << test << " (diff = " << g6Diff << ")" << std::endl;
         failCount++;
         transformStats.Store(0.0, "ReducedG6Mismatch");
         failedCells.push_back(inputG6);
         continue;
      }

      // Now test if the 3D matrix actually transforms the cell correctly
      // Get the basis vectors from the input cell
      LRL_Cell inputCell(inputG6);
      double alpha = inputCell[3] * M_PI / 180.0;
      double beta = inputCell[4] * M_PI / 180.0;
      double gamma = inputCell[5] * M_PI / 180.0;

      // Create the input cell vectors in Cartesian coordinates
      Vector_3 a1(inputCell[0], 0.0, 0.0);

      Vector_3 b1(inputCell[1] * cos(gamma),
         inputCell[1] * sin(gamma),
         0.0);

      Vector_3 c1(inputCell[2] * cos(beta),
         inputCell[2] * (cos(alpha) - cos(beta) * cos(gamma)) / sin(gamma),
         inputCell[2] * sqrt(1.0 - cos(beta) * cos(beta) -
            pow((cos(alpha) - cos(beta) * cos(gamma)) / sin(gamma), 2)));

      // Apply the 3D transformation
      Vector_3 a2 = m3d * a1;
      Vector_3 b2 = m3d * b1;
      Vector_3 c2 = m3d * c1;

      // Calculate the G6 from the transformed vectors
      double g1 = a2.SquaredLength();
      double g2 = b2.SquaredLength();
      double g3 = c2.SquaredLength();
      double g4 = 2.0 * b2.Dot(c2);
      double g5 = 2.0 * a2.Dot(c2);
      double g6 = 2.0 * a2.Dot(b2);

      G6 calculatedG6(g1, g2, g3, g4, g5, g6);

      // Compare with the expected reduced G6
      double calcDiff = 0.0;
      for (int i = 0; i < 6; ++i) {
         calcDiff = std::max(calcDiff, std::abs(calculatedG6[i] - reducedG6_transformed[i]));
      }

      // Allow a larger tolerance for the 3D transformation due to potential numerical issues
      const double tolerance = 1.0E-6;
      if (calcDiff > tolerance) {
         std::stringstream ss;
         ss << "Matrix3DMismatch_Diff:" << std::fixed << std::setprecision(8) << calcDiff;
         transformStats.Store(0.0, ss.str());

         matrixMismatchCount++;
         failCount++;
         failedCells.push_back(inputG6);

         if (matrixMismatchCount < 5) {
            // Report detailed info for the first few mismatches
            std::cout << "3D transformation matrix doesn't properly transform the cell for test " << test
               << " (diff = " << calcDiff << ")" << std::endl;
            std::cout << "  Input G6: " << inputG6 << std::endl;
            std::cout << "  Expected G6: " << reducedG6_transformed << std::endl;
            std::cout << "  Calculated G6: " << calculatedG6 << std::endl;
            std::cout << "  3D Matrix: " << std::endl << m3d << std::endl;
            std::cout << "  Determinant: " << m3d.Det() << std::endl;
         }

         continue;
      }

      // Check that the determinant is reasonable (should be near ±1)
      double det = m3d.Det();
      if (std::abs(std::abs(det) - 1.0) > 0.1) {
         std::stringstream ss;
         ss << "BadDeterminant_" << std::fixed << std::setprecision(3) << det;
         transformStats.Store(0.0, ss.str());

         failCount++;
         failedCells.push_back(inputG6);

         std::cout << "3D matrix has a bad determinant for test " << test << " (det = " << det << ")" << std::endl;
         continue;
      }

      // Test passed
      successCount++;
      transformStats.Store(1.0, "Success");

      // Categorize the transformation by its properties
      if (std::abs(det - 1.0) < 0.1) {
         transformStats.Store(1.0, "Determinant_+1");
      }
      else if (std::abs(det + 1.0) < 0.1) {
         transformStats.Store(1.0, "Determinant_-1");
      }

      // Print progress
      if ((test + 1) % 10 == 0) {
         std::cout << "Completed " << (test + 1) << " tests. Success rate: "
            << (successCount * 100.0 / (test + 1)) << "%" << std::endl;
      }
   }

   // Print final statistics
   std::cout << "\nTest Summary:" << std::endl;
   std::cout << "Total tests: " << numTests << std::endl;
   std::cout << "Successful tests: " << successCount << " ("
      << (successCount * 100.0 / numTests) << "%)" << std::endl;
   std::cout << "Failed tests: " << failCount << " ("
      << (failCount * 100.0 / numTests) << "%)" << std::endl;
   std::cout << "Matrix mismatch count: " << matrixMismatchCount << std::endl;

   // Print the first few failed cells
   const int maxFailToPrint = 3;
   if (!failedCells.empty()) {
      std::cout << "\nFailed Cells (up to " << maxFailToPrint << "):" << std::endl;
      for (int i = 0; i < std::min(maxFailToPrint, static_cast<int>(failedCells.size())); ++i) {
         std::cout << "Cell " << (i + 1) << ": " << failedCells[i] << std::endl;
      }
   }

   // Show detailed statistics
   std::cout << "\nDetailed Statistics:" << std::endl;
   transformStats.ShowResults();

   return (successCount > 0.95 * numTests) ? 0 : 1; // Return success if at least 95% of tests pass
}

// Test specific cases that are known to be challenging
void TestSpecificCases() {
   std::cout << "\nTesting specific test cases for ReduceWithTransforms..." << std::endl;

   // Test case 1: A cell that requires R5_Plus transformation
   G6 cell1(10.0, 5.0, 2.0, 7.0, 1.0, 0.5);  // g4 > g2

   MatG6 mG6;
   Matrix_3x3 m3d;
   G6 reducedG6;

   bool success = Niggli::ReduceWithTransforms(cell1, mG6, m3d, reducedG6, 1.0E-10);

   std::cout << "Test Case 1 (R5_Plus):" << std::endl;
   std::cout << "  Success: " << (success ? "Yes" : "No") << std::endl;
   std::cout << "  Input G6: " << cell1 << std::endl;
   std::cout << "  Reduced G6: " << reducedG6 << std::endl;
   std::cout << "  3D Matrix: " << std::endl << m3d << std::endl;
   std::cout << "  Determinant: " << m3d.Det() << std::endl;

   // Test the transformation
   LRL_Cell inputCell(cell1);
   double alpha = inputCell[3] * M_PI / 180.0;
   double beta = inputCell[4] * M_PI / 180.0;
   double gamma = inputCell[5] * M_PI / 180.0;

   Vector_3 a1(inputCell[0], 0.0, 0.0);
   Vector_3 b1(inputCell[1] * cos(gamma), inputCell[1] * sin(gamma), 0.0);
   Vector_3 c1(inputCell[2] * cos(beta),
      inputCell[2] * (cos(alpha) - cos(beta) * cos(gamma)) / sin(gamma),
      inputCell[2] * sqrt(1.0 - cos(beta) * cos(beta) -
         pow((cos(alpha) - cos(beta) * cos(gamma)) / sin(gamma), 2)));

   Vector_3 a2 = m3d * a1;
   Vector_3 b2 = m3d * b1;
   Vector_3 c2 = m3d * c1;

   double g1 = a2.SquaredLength();
   double g2 = b2.SquaredLength();
   double g3 = c2.SquaredLength();
   double g4 = 2.0 * b2.Dot(c2);
   double g5 = 2.0 * a2.Dot(c2);
   double g6 = 2.0 * a2.Dot(b2);

   G6 calculatedG6(g1, g2, g3, g4, g5, g6);

   std::cout << "  Calculated G6 from 3D transform: " << calculatedG6 << std::endl;
   double maxDiff = 0.0;
   for (int i = 0; i < 6; ++i) {
      maxDiff = std::max(maxDiff, std::abs(calculatedG6[i] - reducedG6[i]));
   }
   std::cout << "  Maximum difference: " << maxDiff << std::endl;

   // Test Case 2: A cell that requires multiple transformations
   G6 cell2(8.0, 8.0, 8.0, -1.0, -1.0, -1.0);  // Rhombohedral cell

   success = Niggli::ReduceWithTransforms(cell2, mG6, m3d, reducedG6, 1.0E-10);

   std::cout << "\nTest Case 2 (Rhombohedral):" << std::endl;
   std::cout << "  Success: " << (success ? "Yes" : "No") << std::endl;
   std::cout << "  Input G6: " << cell2 << std::endl;
   std::cout << "  Reduced G6: " << reducedG6 << std::endl;
   std::cout << "  3D Matrix: " << std::endl << m3d << std::endl;
   std::cout << "  Determinant: " << m3d.Det() << std::endl;

   // Test the transformation for this case too
   inputCell = LRL_Cell(cell2);
   alpha = inputCell[3] * M_PI / 180.0;
   beta = inputCell[4] * M_PI / 180.0;
   gamma = inputCell[5] * M_PI / 180.0;

   a1 = Vector_3(inputCell[0], 0.0, 0.0);
   b1 = Vector_3(inputCell[1] * cos(gamma), inputCell[1] * sin(gamma), 0.0);
   c1 = Vector_3(inputCell[2] * cos(beta),
      inputCell[2] * (cos(alpha) - cos(beta) * cos(gamma)) / sin(gamma),
      inputCell[2] * sqrt(1.0 - cos(beta) * cos(beta) -
         pow((cos(alpha) - cos(beta) * cos(gamma)) / sin(gamma), 2)));

   a2 = m3d * a1;
   b2 = m3d * b1;
   c2 = m3d * c1;

   g1 = a2.SquaredLength();
   g2 = b2.SquaredLength();
   g3 = c2.SquaredLength();
   g4 = 2.0 * b2.Dot(c2);
   g5 = 2.0 * a2.Dot(c2);
   g6 = 2.0 * a2.Dot(b2);

   calculatedG6 = G6(g1, g2, g3, g4, g5, g6);

   std::cout << "  Calculated G6 from 3D transform: " << calculatedG6 << std::endl;
   maxDiff = 0.0;
   for (int i = 0; i < 6; ++i) {
      maxDiff = std::max(maxDiff, std::abs(calculatedG6[i] - reducedG6[i]));
   }
   std::cout << "  Maximum difference: " << maxDiff << std::endl;

   // Test Case 3: A cell with nearly identical axes lengths but different angles
   G6 cell3(10.0, 10.01, 10.02, 2.0, -3.0, 4.0);

   success = Niggli::ReduceWithTransforms(cell3, mG6, m3d, reducedG6, 1.0E-10);

   std::cout << "\nTest Case 3 (Nearly identical axes):" << std::endl;
   std::cout << "  Success: " << (success ? "Yes" : "No") << std::endl;
   std::cout << "  Input G6: " << cell3 << std::endl;
   std::cout << "  Reduced G6: " << reducedG6 << std::endl;
   std::cout << "  3D Matrix: " << std::endl << m3d << std::endl;
   std::cout << "  Determinant: " << m3d.Det() << std::endl;

   // Test the transformation
   inputCell = LRL_Cell(cell3);
   alpha = inputCell[3] * M_PI / 180.0;
   beta = inputCell[4] * M_PI / 180.0;
   gamma = inputCell[5] * M_PI / 180.0;

   a1 = Vector_3(inputCell[0], 0.0, 0.0);
   b1 = Vector_3(inputCell[1] * cos(gamma), inputCell[1] * sin(gamma), 0.0);
   c1 = Vector_3(inputCell[2] * cos(beta),
      inputCell[2] * (cos(alpha) - cos(beta) * cos(gamma)) / sin(gamma),
      inputCell[2] * sqrt(1.0 - cos(beta) * cos(beta) -
         pow((cos(alpha) - cos(beta) * cos(gamma)) / sin(gamma), 2)));

   a2 = m3d * a1;
   b2 = m3d * b1;
   c2 = m3d * c1;

   g1 = a2.SquaredLength();
   g2 = b2.SquaredLength();
   g3 = c2.SquaredLength();
   g4 = 2.0 * b2.Dot(c2);
   g5 = 2.0 * a2.Dot(c2);
   g6 = 2.0 * a2.Dot(b2);

   calculatedG6 = G6(g1, g2, g3, g4, g5, g6);

   std::cout << "  Calculated G6 from 3D transform: " << calculatedG6 << std::endl;
   maxDiff = 0.0;
   for (int i = 0; i < 6; ++i) {
      maxDiff = std::max(maxDiff, std::abs(calculatedG6[i] - reducedG6[i]));
   }
   std::cout << "  Maximum difference: " << maxDiff << std::endl;

   // Test Case 4: A cell with one very short axis
   G6 cell4(50.0, 50.0, 2.0, 0.0, 0.0, 0.0);  // Orthorhombic with one short axis

   success = Niggli::ReduceWithTransforms(cell4, mG6, m3d, reducedG6, 1.0E-10);

   std::cout << "\nTest Case 4 (One short axis):" << std::endl;
   std::cout << "  Success: " << (success ? "Yes" : "No") << std::endl;
   std::cout << "  Input G6: " << cell4 << std::endl;
   std::cout << "  Reduced G6: " << reducedG6 << std::endl;
   std::cout << "  3D Matrix: " << std::endl << m3d << std::endl;
   std::cout << "  Determinant: " << m3d.Det() << std::endl;

   // Test the transformation
   inputCell = LRL_Cell(cell4);
   alpha = inputCell[3] * M_PI / 180.0;
   beta = inputCell[4] * M_PI / 180.0;
   gamma = inputCell[5] * M_PI / 180.0;

   a1 = Vector_3(inputCell[0], 0.0, 0.0);
   b1 = Vector_3(inputCell[1] * cos(gamma), inputCell[1] * sin(gamma), 0.0);
   c1 = Vector_3(inputCell[2] * cos(beta),
      inputCell[2] * (cos(alpha) - cos(beta) * cos(gamma)) / sin(gamma),
      inputCell[2] * sqrt(1.0 - cos(beta) * cos(beta) -
         pow((cos(alpha) - cos(beta) * cos(gamma)) / sin(gamma), 2)));

   a2 = m3d * a1;
   b2 = m3d * b1;
   c2 = m3d * c1;

   g1 = a2.SquaredLength();
   g2 = b2.SquaredLength();
   g3 = c2.SquaredLength();
   g4 = 2.0 * b2.Dot(c2);
   g5 = 2.0 * a2.Dot(c2);
   g6 = 2.0 * a2.Dot(b2);

   calculatedG6 = G6(g1, g2, g3, g4, g5, g6);

   std::cout << "  Calculated G6 from 3D transform: " << calculatedG6 << std::endl;
   maxDiff = 0.0;
   for (int i = 0; i < 6; ++i) {
      maxDiff = std::max(maxDiff, std::abs(calculatedG6[i] - reducedG6[i]));
   }
   std::cout << "  Maximum difference: " << maxDiff << std::endl;
}

// Generate and test cells with specific challenging properties
void TestChallengingCells() {
   std::cout << "\nTesting cells with challenging properties..." << std::endl;

   // Create a StoreResults object to track statistics
   StoreResults<double, std::string> challengingStats(100);
   challengingStats.SetTitle("Challenging Cell Tests");

   // Test high aspect ratio cells
   for (int i = 0; i < 10; ++i) {
      // Create a cell with high aspect ratio
      double a = 1.0;
      double b = 1.0;
      double c = 10.0 + i * 5.0; // Increase c from 10 to 55
      double alpha = 90.0;
      double beta = 90.0;
      double gamma = 90.0;

      LRL_Cell cell(a, b, c, alpha, beta, gamma);
      G6 inputG6(cell);

      MatG6 mG6;
      Matrix_3x3 m3d;
      G6 reducedG6;

      bool success = Niggli::ReduceWithTransforms(inputG6, mG6, m3d, reducedG6, 1.0E-10);

      // Test the transformation
      double maxDiff = 0.0;
      if (success) {
         // Get the basis vectors from the input cell
         double alphaRad = alpha * M_PI / 180.0;
         double betaRad = beta * M_PI / 180.0;
         double gammaRad = gamma * M_PI / 180.0;

         Vector_3 a1(a, 0.0, 0.0);
         Vector_3 b1(b * cos(gammaRad), b * sin(gammaRad), 0.0);
         Vector_3 c1(c * cos(betaRad),
            c * (cos(alphaRad) - cos(betaRad) * cos(gammaRad)) / sin(gammaRad),
            c * sqrt(1.0 - cos(betaRad) * cos(betaRad) -
               pow((cos(alphaRad) - cos(betaRad) * cos(gammaRad)) / sin(gammaRad), 2)));

         Vector_3 a2 = m3d * a1;
         Vector_3 b2 = m3d * b1;
         Vector_3 c2 = m3d * c1;

         double g1 = a2.SquaredLength();
         double g2 = b2.SquaredLength();
         double g3 = c2.SquaredLength();
         double g4 = 2.0 * b2.Dot(c2);
         double g5 = 2.0 * a2.Dot(c2);
         double g6 = 2.0 * a2.Dot(b2);

         G6 calculatedG6(g1, g2, g3, g4, g5, g6);

         for (int j = 0; j < 6; ++j) {
            maxDiff = std::max(maxDiff, std::abs(calculatedG6[j] - reducedG6[j]));
         }
      }

      std::stringstream ss;
      ss << "AspectRatio_" << c << (success ? "_Success" : "_Failure") << "_Diff_" << maxDiff;
      challengingStats.Store(success ? 1.0 : 0.0, ss.str());

      if (i == 0 || i == 9) {
         // Print details for the first and last test
         std::cout << "High Aspect Ratio Test (c = " << c << "):" << std::endl;
         std::cout << "  Success: " << (success ? "Yes" : "No") << std::endl;
         std::cout << "  Input G6: " << inputG6 << std::endl;
         if (success) {
            std::cout << "  Reduced G6: " << reducedG6 << std::endl;
            std::cout << "  3D Matrix: " << std::endl << m3d << std::endl;
            std::cout << "  Determinant: " << m3d.Det() << std::endl;
            std::cout << "  Maximum difference: " << maxDiff << std::endl;
         }
         std::cout << std::endl;
      }
   }

   // Test cells with extreme angles
   for (int i = 0; i < 10; ++i) {
      // Create a cell with extreme angles
      double a = 10.0;
      double b = 10.0;
      double c = 10.0;
      double alpha = 20.0 + i * 5.0; // Increase alpha from 20 to 65
      double beta = 90.0;
      double gamma = 90.0;

      LRL_Cell cell(a, b, c, alpha, beta, gamma);
      G6 inputG6(cell);

      MatG6 mG6;
      Matrix_3x3 m3d;
      G6 reducedG6;

      bool success = Niggli::ReduceWithTransforms(inputG6, mG6, m3d, reducedG6, 1.0E-10);

      // Test the transformation
      double maxDiff = 0.0;
      if (success) {
         // Get the basis vectors from the input cell
         double alphaRad = alpha * M_PI / 180.0;
         double betaRad = beta * M_PI / 180.0;
         double gammaRad = gamma * M_PI / 180.0;

         Vector_3 a1(a, 0.0, 0.0);
         Vector_3 b1(b * cos(gammaRad), b * sin(gammaRad), 0.0);
         Vector_3 c1(c * cos(betaRad),
            c * (cos(alphaRad) - cos(betaRad) * cos(gammaRad)) / sin(gammaRad),
            c * sqrt(1.0 - cos(betaRad) * cos(betaRad) -
               pow((cos(alphaRad) - cos(betaRad) * cos(gammaRad)) / sin(gammaRad), 2)));

         Vector_3 a2 = m3d * a1;
         Vector_3 b2 = m3d * b1;
         Vector_3 c2 = m3d * c1;

         double g1 = a2.SquaredLength();
         double g2 = b2.SquaredLength();
         double g3 = c2.SquaredLength();
         double g4 = 2.0 * b2.Dot(c2);
         double g5 = 2.0 * a2.Dot(c2);
         double g6 = 2.0 * a2.Dot(b2);

         G6 calculatedG6(g1, g2, g3, g4, g5, g6);

         for (int j = 0; j < 6; ++j) {
            maxDiff = std::max(maxDiff, std::abs(calculatedG6[j] - reducedG6[j]));
         }
      }

      std::stringstream ss;
      ss << "ExtremeAngle_" << alpha << (success ? "_Success" : "_Failure") << "_Diff_" << maxDiff;
      challengingStats.Store(success ? 1.0 : 0.0, ss.str());

      if (i == 0 || i == 9) {
         // Print details for the first and last test
         std::cout << "Extreme Angle Test (alpha = " << alpha << "):" << std::endl;
         std::cout << "  Success: " << (success ? "Yes" : "No") << std::endl;
         std::cout << "  Input G6: " << inputG6 << std::endl;
         if (success) {
            std::cout << "  Reduced G6: " << reducedG6 << std::endl;
            std::cout << "  3D Matrix: " << std::endl << m3d << std::endl;
            std::cout << "  Determinant: " << m3d.Det() << std::endl;
            std::cout << "  Maximum difference: " << maxDiff << std::endl;
         }
         std::cout << std::endl;
      }
   }

   // Print statistics
   std::cout << "\nChallenging Cell Test Statistics:" << std::endl;
   challengingStats.ShowResults();
}

// Main test function that runs all the tests
int TestReduceTransforms(bool runRandomTests) {
   std::cout << "------- Starting ReduceWithTransforms Tests -------" << std::endl;

   // Test specific cases first
   TestSpecificCases();

   // Test challenging cells
   TestChallengingCells();

   // Test random cells if requested
   int randomTestResult = 0;
   if (runRandomTests) {
      randomTestResult = TestReduceWithTransformsRandom(100);
   }

   std::cout << "------- Completed ReduceWithTransforms Tests -------" << std::endl;

   return randomTestResult;
}

// Main test runner
int main() {
   std::cout << "===== 3D Matrix Transformation Verification Tool =====" << std::endl;
   std::cout << "This tool helps identify the correct 3D matrices for Niggli reduction" << std::endl << std::endl;

   int choice = 0;
   bool exitProgram = false;

   while (!exitProgram) {
      std::cout << "\nPlease select a test to run:" << std::endl;
      std::cout << "1. Basic test of the current 3D matrix definitions" << std::endl;
      std::cout << "2. Test all sign variations for key matrices" << std::endl;
      std::cout << "3. Explore all possible sign combinations for matrices" << std::endl;
      std::cout << "4. Derive 3D matrices directly from transformations" << std::endl;
      std::cout << "5. Test ReduceWithTransforms on specific cells" << std::endl;
      std::cout << "0. Exit" << std::endl;

      std::cout << "Enter your choice: ";
      std::cin >> choice;

      switch (choice) {
      case 0:
         exitProgram = true;
         std::cout << "Exiting..." << std::endl;
         break;

      case 1:
         std::cout << "\nRunning basic test of current 3D matrix definitions..." << std::endl;
         TestAllMatrixPairs();
         break;

      case 2:
         std::cout << "\nTesting sign variations for key matrices..." << std::endl;
         TestSignVariations();
         break;

      case 3:
         std::cout << "\nExploring all possible sign combinations..." << std::endl;
         ExploreKeyMatrices();
         break;

      case 4:
         std::cout << "\nDeriving 3D matrices directly from transformations..." << std::endl;
         DeriveCorrect3DMatrices();
         break;

      case 5:
         std::cout << "\nTesting ReduceWithTransforms on specific cells..." << std::endl;
         TestSingleCell();
         break;

      default:
         std::cout << "Invalid choice. Please try again." << std::endl;
      }
   }

   return 0;
}

// Function to generate a summary report of correct 3D matrices
void GenerateSummaryReport() {
   std::cout << "\n===== Summary of Correct 3D Matrices for Niggli Reduction =====" << std::endl;

   // Define the G6 matrices
   const static MatG6 spnull("1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 1");
   const static MatG6 sp1("0 1 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0 0 0 1");
   const static MatG6 sp2("1 0 0 0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 0 1 0");
   const static MatG6 sp34a("1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 -1 0 0 0 0 0 0 -1 0 0 0 0 0 0 1");
   const static MatG6 sp34b("1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 -1 0 0 0 0 0 0 1 0 0 0 0 0 0 -1");
   const static MatG6 sp34c("1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 -1 0 0 0 0 0 0 -1");
   const static MatG6 R5_Plus(" 1 0 0 0 0 0  0 1 0 0 0 0  0 1 1 -1 0 0  0 -2 0 1 0 0  0 0 0 0 1 -1  0 0 0 0 0 1");
   const static MatG6 R5_Minus(" 1 0 0 0 0 0  0 1 0 0 0 0  0 1 1 +1 0 0  0 +2 0 1 0 0  0 0 0 0 1 +1  0 0 0 0 0 1");
   const static MatG6 R6_Plus(" 1 0 0 0 0 0  0 1 0 0 0 0  1 0 1 0 -1 0  0 0 0 1 0 -1  -2 0 0 0 1 0  0 0 0 0 0 1");
   const static MatG6 R6_Minus(" 1 0 0 0 0 0  0 1 0 0 0 0  1 0 1 0 +1 0  0 0 0 1 0 +1  +2 0 0 0 1 0  0 0 0 0 0 1");
   const static MatG6 R7_Plus(" 1 0 0 0 0 0  1 1 0 0 0 -1  0 0 1 0 0 0  0 0 0 1 -1 0  0 0 0 0 1 0  -2 0 0 0 0 1");
   const static MatG6 R7_Minus(" 1 0 0 0 0 0  1 1 0 0 0 +1  0 0 1 0 0 0  0 0 0 1 +1 0  0 0 0 0 1 0  +2 0 0 0 0 1");
   const static MatG6 R8(" 1 0 0 0 0 0  0 1 0 0 0 0  1 1 1 1 1 1  0 2 0 1 0 1  2 0 0 0 1 1  0 0 0 0 0 1");
   const static MatG6 R12("1 0 0 0 0 0  0 1 0 0 0 0  1 1 1 1 1 1  0 -2 0 -1 0 -1  -2 0 0 0 -1 -1  0 0 0 0 0 1");

   // Define a struct for the results
   struct MatrixResult {
      std::string name;
      MatG6 matG6;
      Matrix_3x3 mat3D;
      std::string description;
      bool isVerified;
   };

   // Create a list of matrices with their correct 3D counterparts
   std::vector<MatrixResult> matrixResults = {
       {"spnull (Identity)", spnull, Matrix_3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0),
        "Identity matrix", true},
       {"sp1 (Swap a,b)", sp1, Matrix_3x3(0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0),
        "Swaps a and b axes", true},
       {"sp2 (Swap b,c)", sp2, Matrix_3x3(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0),
        "Swaps b and c axes", true},
       {"sp34a", sp34a, Matrix_3x3(1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, -1.0),
        "Flips signs of b and c", true},
       {"sp34b", sp34b, Matrix_3x3(-1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0),
        "Flips signs of a and c", true},
       {"sp34c", sp34c, Matrix_3x3(-1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 1.0),
        "Flips signs of a and b", true},

        // Note: These are placeholder values - they need verification from the tests
        {"R5_Plus", R5_Plus, Matrix_3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, -1.0, 1.0),
         "c' = c - b", false},
        {"R5_Minus", R5_Minus, Matrix_3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0),
         "c' = c + b", false},
        {"R6_Plus", R6_Plus, Matrix_3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, 0.0, 1.0),
         "c' = c - a", false},
        {"R6_Minus", R6_Minus, Matrix_3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0),
         "c' = c + a", false},
        {"R7_Plus", R7_Plus, Matrix_3x3(1.0, 0.0, 0.0, -1.0, 1.0, 0.0, 0.0, 0.0, 1.0),
         "b' = b - a", false},
        {"R7_Minus", R7_Minus, Matrix_3x3(1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0),
         "b' = b + a", false},
        {"R8", R8, Matrix_3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0),
         "c' = a + b + c", false},
        {"R12", R12, Matrix_3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, -1.0, -1.0),
         "c' = -a - b - c", false}
   };

   // Print out the table
   std::cout << std::left << std::setw(15) << "Matrix Name"
      << std::setw(20) << "Description"
      << std::setw(50) << "3D Matrix"
      << std::setw(10) << "Verified" << std::endl;
   std::cout << std::string(95, '-') << std::endl;

   for (const auto& result : matrixResults) {
      std::ostringstream matrixStr;
      matrixStr << "[[" << std::setw(3) << result.mat3D[0] << ", " << std::setw(3) << result.mat3D[1] << ", " << std::setw(3) << result.mat3D[2] << "], ";
      matrixStr << "[" << std::setw(3) << result.mat3D[3] << ", " << std::setw(3) << result.mat3D[4] << ", " << std::setw(3) << result.mat3D[5] << "], ";
      matrixStr << "[" << std::setw(3) << result.mat3D[6] << ", " << std::setw(3) << result.mat3D[7] << ", " << std::setw(3) << result.mat3D[8] << "]]";

      std::cout << std::left << std::setw(15) << result.name
         << std::setw(20) << result.description
         << std::setw(50) << matrixStr.str()
         << std::setw(10) << (result.isVerified ? "Yes" : "No") << std::endl;
   }

   std::cout << "\nNote: Matrices marked as 'Not Verified' need further testing." << std::endl;
   std::cout << "Run the appropriate tests to determine the correct 3D matrices for these operations." << std::endl;
}

