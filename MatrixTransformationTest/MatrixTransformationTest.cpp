// TransformationTest.cpp
#include "MatrixTransformationTest.h"
#include <iostream>
#include <iomanip>
#include <cmath>

#include "LRL_Cell.h"
#include "NCDist.h"
#include "Niggli.h"
#include "Polar.h"
#include "StoreResults.h"

double TransformationTest::ReducedNCDist(const LRL_Cell& cell1, const LRL_Cell& cell2) const {
   // Convert to G6
   G6 g6_1(cell1);
   G6 g6_2(cell2);

   // Reduced versions
   G6 reduced_g6_1;
   G6 reduced_g6_2;

   // Use ReduceWithoutMatrices to get reduced cells
   const double delta = 1.0e-10; // Tolerance parameter
   bool success1 = Niggli::ReduceWithoutMatrices(g6_1, reduced_g6_1, delta);
   bool success2 = Niggli::ReduceWithoutMatrices(g6_2, reduced_g6_2, delta);

   if (!success1 || !success2) {
      std::cerr << "Warning: Cell reduction failed" << std::endl;
      return -1.0; // Indicate failure
   }

   // Now compute NCDist between the reduced cells
   return NCDist(reduced_g6_1.data(), reduced_g6_2.data());
}



// TransformationTest.cpp - Modified with adjusted 3D matrices

TransformationTest::TransformationTest() {
   // Modified 3D matrices to align with G6 results

// R5_Plus: c → c - b
   m_transformations.emplace_back(
      "R5_Plus",
      Matrix_3x3(1, 0, 0, 0, 1, 0, 0, -1, 1),
      MatG6("1 0 0 0 0 0  0 1 0 0 0 0  0 1 1 -1 0 0  0 -2 0 1 0 0  0 0 0 0 1 -1  0 0 0 0 0 1")
   );

   // R5_Minus: c → c + b
   m_transformations.emplace_back(
      "R5_Minus",
      Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 1, 1),
      MatG6("1 0 0 0 0 0  0 1 0 0 0 0  0 1 1 1 0 0  0 2 0 1 0 0  0 0 0 0 1 1  0 0 0 0 0 1")
   );

   // R6_Plus: c → c - a
   m_transformations.emplace_back(
      "R6_Plus",
      Matrix_3x3(1, 0, 0, 0, 1, 0, -1, 0, 1),
      MatG6("1 0 0 0 0 0  0 1 0 0 0 0  1 0 1 0 -1 0  0 0 0 1 0 -1  -2 0 0 0 1 0  0 0 0 0 0 1")
   );

   // R6_Minus: c → c + a
   m_transformations.emplace_back(
      "R6_Minus",
      Matrix_3x3(1, 0, 0, 0, 1, 0, 1, 0, 1),
      MatG6("1 0 0 0 0 0  0 1 0 0 0 0  1 0 1 0 1 0  0 0 0 1 0 1  2 0 0 0 1 0  0 0 0 0 0 1")
   );

   // R7_Plus: b → b - a
   m_transformations.emplace_back(
      "R7_Plus",
      Matrix_3x3(1, 0, 0, -1, 1, 0, 0, 0, 1),
      MatG6("1 0 0 0 0 0  1 1 0 0 0 -1  0 0 1 0 0 0  0 0 0 1 -1 0  0 0 0 0 1 0  -2 0 0 0 0 1")
   );

   // R7_Minus: b → b + a
   m_transformations.emplace_back(
      "R7_Minus",
      Matrix_3x3(1, 0, 0, 1, 1, 0, 0, 0, 1),
      MatG6("1 0 0 0 0 0  1 1 0 0 0 1  0 0 1 0 0 0  0 0 0 1 1 0  0 0 0 0 1 0  2 0 0 0 0 1")
   );

   // R8: c → a + b + c (with determinant exactly 1.0)
   m_transformations.emplace_back(
      "R8_proper",
      Matrix_3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0),  // Det = 1.0
      MatG6("1 0 0 0 0 0  0 1 0 0 0 0  1 1 1 1 1 1  0 2 0 1 0 1  2 0 0 0 1 1  0 0 0 0 0 1")
   );

   // R12: c → -a - b + c (with determinant exactly 1.0)
   m_transformations.emplace_back(
      "R12_proper",
      Matrix_3x3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, -1.0, 1.0),  // Det = 1.0
      MatG6("1 0 0 0 0 0  0 1 0 0 0 0  1 1 1 -1 -1 1  0 -2 0 1 0 -1  -2 0 0 0 1 -1  0 0 0 0 0 1")
   );
}

LRL_Cell TransformationTest::Apply3DTransformation(const LRL_Cell& cell, const Matrix_3x3& transform) const {
   // Convert LRL_Cell to B4 to get direct access to basis vectors
   B4 cellB4(cell);

   // Extract basis vectors
   Vector_3 a = cellB4[0];
   Vector_3 b = cellB4[1];
   Vector_3 c = cellB4[2];

   // For debugging
   std::cout << "Original basis vectors:" << std::endl;
   std::cout << "a: " << a << std::endl;
   std::cout << "b: " << b << std::endl;
   std::cout << "c: " << c << std::endl;

   // Create a matrix with basis vectors as rows
   Matrix_3x3 basisMatrix(a[0], a[1], a[2],
      b[0], b[1], b[2],
      c[0], c[1], c[2]);

   // Apply transformation (assuming basis vectors are rows)
   Matrix_3x3 transformedBasisMatrix = transform * basisMatrix;

   // Extract transformed basis vectors
   Vector_3 a_new(transformedBasisMatrix[0], transformedBasisMatrix[1], transformedBasisMatrix[2]);
   Vector_3 b_new(transformedBasisMatrix[3], transformedBasisMatrix[4], transformedBasisMatrix[5]);
   Vector_3 c_new(transformedBasisMatrix[6], transformedBasisMatrix[7], transformedBasisMatrix[8]);

   // For debugging
   std::cout << "Transformed basis vectors:" << std::endl;
   std::cout << "a': " << a_new << std::endl;
   std::cout << "b': " << b_new << std::endl;
   std::cout << "c': " << c_new << std::endl;

   // Create new B4 from transformed basis vectors
   B4 transformedB4;
   transformedB4[0] = a_new;
   transformedB4[1] = b_new;
   transformedB4[2] = c_new;
   transformedB4[3] = -(a_new + b_new + c_new);  // Compute d = -(a+b+c)

   // Convert back to LRL_Cell
   LRL_Cell transformedCell(transformedB4);

   // Report volume for comparison
   std::cout << "3D transformation cell volume: " << transformedCell.Volume() << std::endl;

   return transformedCell;
}


LRL_Cell TransformationTest::Apply6DTransformation(const LRL_Cell& cell, const MatG6& transform) const {
   // Convert cell to G6 representation
   G6 g6Cell(cell);

   // Debug output
   std::cout << "Original G6: " << g6Cell << std::endl;
   std::cout << "Transformation Matrix: " << transform << std::endl;

   // Apply the transformation
   G6 transformedG6 = transform * g6Cell;

   // Debug output
   std::cout << "Transformed G6: " << transformedG6 << std::endl;

   // Convert back to LRL_Cell and report volume
   LRL_Cell transformedCell(transformedG6);
   std::cout << "6D transformation cell volume: " << transformedCell.Volume() << std::endl;

   return transformedCell;
}

void TransformationTest::TestTransformations(const LRL_Cell& testCell) const {
   std::cout << "Testing determinants of 3D matrices:" << std::endl;
   std::cout << " Operation    Determinant" << std::endl;
   std::cout << "-------------------------" << std::endl;

   for (const auto& t : m_transformations) {
      double det = t.m_3d_matrix.Det();
      std::cout << std::setw(10) << t.m_name << std::setw(14) << det;
      if (std::abs(det - 1.0) > 1e-10) {
         std::cout << " - WARNING: Not +1!";
      }
      std::cout << std::endl;
   }

   std::cout << std::endl << "Testing transformations on unit cell:" << std::endl;
   std::cout << "Original cell: " << testCell << std::endl << std::endl;

   for (const auto& t : m_transformations) {
      std::cout << "Applying " << t.m_name << " transformation:" << std::endl;

      // Apply 3D transformation
      std::cout << "3D Matrix:" << std::endl << t.m_3d_matrix << std::endl;
      LRL_Cell transformed3DCell = Apply3DTransformation(testCell, t.m_3d_matrix);

      // Apply 6D transformation
      std::cout << "6D Matrix:" << std::endl << t.m_6d_matrix << std::endl;
      LRL_Cell transformed6DCell = Apply6DTransformation(testCell, t.m_6d_matrix);

      // Display cell parameters for both transformations
      std::cout << "Transformed cell (3D): " << transformed3DCell << std::endl;
      std::cout << "Transformed cell (6D): " << transformed6DCell << std::endl;

      // Compare G6 representations
      G6 g6_original(testCell);
      G6 g6_3d(transformed3DCell);
      G6 g6_6d(transformed6DCell);
      G6 diff = g6_3d - g6_6d;

      std::cout << "Difference (G6): " << diff << std::endl;
      std::cout << "Magnitude of difference: " << diff.norm() << std::endl;

      // Verify that both cells represent the same lattice using NCDist with reduction
      double reducedDistance_3d_6d = ReducedNCDist(transformed3DCell, transformed6DCell);
      std::cout << "Reduced NCDist between 3D and 6D cells: " << reducedDistance_3d_6d;
      if (reducedDistance_3d_6d < 1e-10) {
         std::cout << " - Same lattice";
      }
      else if (reducedDistance_3d_6d < 1e-6) {
         std::cout << " - Practically same lattice (numerical precision)";
      }
      else {
         std::cout << " - WARNING: Different lattices!";
      }
      std::cout << std::endl;

      // Verify that transformed cells represent the same lattice as the original cell
      double reducedDistance_original_3d = ReducedNCDist(testCell, transformed3DCell);
      std::cout << "Reduced NCDist between original and 3D cell: " << reducedDistance_original_3d;
      if (reducedDistance_original_3d < 1e-10) {
         std::cout << " - Same lattice";
      }
      else if (reducedDistance_original_3d < 1e-6) {
         std::cout << " - Practically same lattice (numerical precision)";
      }
      else {
         std::cout << " - WARNING: Different lattices!";
      }
      std::cout << std::endl;

      double reducedDistance_original_6d = ReducedNCDist(testCell, transformed6DCell);
      std::cout << "Reduced NCDist between original and 6D cell: " << reducedDistance_original_6d;
      if (reducedDistance_original_6d < 1e-10) {
         std::cout << " - Same lattice";
      }
      else if (reducedDistance_original_6d < 1e-6) {
         std::cout << " - Practically same lattice (numerical precision)";
      }
      else {
         std::cout << " - WARNING: Different lattices!";
      }
      std::cout << std::endl << std::endl;
   }
}
// main.cpp



int main() {

   std::cout << "========== Crystallographic Transformation Test ==========" << std::endl;

   // Create a test instance
   TransformationTest transformTest;

   // Create several test cells with different properties

   // 1. Simple cubic cell
   LRL_Cell cubicCell(10.0, 10.0, 10.0, 90.0, 90.0, 90.0);

   // 2. Orthorhombic cell
   LRL_Cell orthoCell(8.0, 12.0, 15.0, 90.0, 90.0, 90.0);

   // 3. Monoclinic cell
   LRL_Cell monoclinicCell(10.0, 10.0, 10.0, 90.0, 120.0, 90.0);

   // 4. Triclinic cell
   LRL_Cell triclinicCell(8.0, 10.0, 12.0, 80.0, 85.0, 75.0);

   // 5. Rhombohedral cell
   LRL_Cell rhombohedralCell(10.0, 10.0, 10.0, 80.0, 80.0, 80.0);

   // Choose which cell to test
   int choice = 0;
   std::cout << "Select a cell type to test:" << std::endl;
   std::cout << "1. Cubic" << std::endl;
   std::cout << "2. Orthorhombic" << std::endl;
   std::cout << "3. Monoclinic" << std::endl;
   std::cout << "4. Triclinic" << std::endl;
   std::cout << "5. Rhombohedral" << std::endl;
   std::cout << "Enter your choice (1-5): ";
   std::cin >> choice;

   LRL_Cell testCell;
   switch (choice) {
   case 1:
      testCell = cubicCell;
      std::cout << "Testing cubic cell" << std::endl;
      break;
   case 2:
      testCell = orthoCell;
      std::cout << "Testing orthorhombic cell" << std::endl;
      break;
   case 3:
      testCell = monoclinicCell;
      std::cout << "Testing monoclinic cell" << std::endl;
      break;
   case 4:
      testCell = triclinicCell;
      std::cout << "Testing triclinic cell" << std::endl;
      break;
   case 5:
      testCell = rhombohedralCell;
      std::cout << "Testing rhombohedral cell" << std::endl;
      break;
   default:
      testCell = cubicCell;
      std::cout << "Invalid choice. Testing cubic cell by default." << std::endl;
   }

   // Display the test cell
   std::cout << "\nTest cell parameters: " << testCell << std::endl;

   // Run the tests
   transformTest.TestTransformations(testCell);

   // Option to continue testing
   char continueChoice;
   do {
      std::cout << "\nDo you want to test another cell? (y/n): ";
      std::cin >> continueChoice;

      if (continueChoice == 'y' || continueChoice == 'Y') {
         std::cout << "Select a cell type to test:" << std::endl;
         std::cout << "1. Cubic" << std::endl;
         std::cout << "2. Orthorhombic" << std::endl;
         std::cout << "3. Monoclinic" << std::endl;
         std::cout << "4. Triclinic" << std::endl;
         std::cout << "5. Rhombohedral" << std::endl;
         std::cout << "6. Custom cell (input parameters)" << std::endl;
         std::cout << "Enter your choice (1-6): ";
         std::cin >> choice;

         if (choice == 6) {
            // Allow the user to input custom cell parameters
            double a, b, c, alpha, beta, gamma;
            std::cout << "Enter cell parameters:" << std::endl;
            std::cout << "a: ";
            std::cin >> a;
            std::cout << "b: ";
            std::cin >> b;
            std::cout << "c: ";
            std::cin >> c;
            std::cout << "alpha (degrees): ";
            std::cin >> alpha;
            std::cout << "beta (degrees): ";
            std::cin >> beta;
            std::cout << "gamma (degrees): ";
            std::cin >> gamma;

            testCell = LRL_Cell(a, b, c, alpha, beta, gamma);
            std::cout << "Testing custom cell" << std::endl;
         }
         else {
            switch (choice) {
            case 1:
               testCell = cubicCell;
               std::cout << "Testing cubic cell" << std::endl;
               break;
            case 2:
               testCell = orthoCell;
               std::cout << "Testing orthorhombic cell" << std::endl;
               break;
            case 3:
               testCell = monoclinicCell;
               std::cout << "Testing monoclinic cell" << std::endl;
               break;
            case 4:
               testCell = triclinicCell;
               std::cout << "Testing triclinic cell" << std::endl;
               break;
            case 5:
               testCell = rhombohedralCell;
               std::cout << "Testing rhombohedral cell" << std::endl;
               break;
            default:
               testCell = cubicCell;
               std::cout << "Invalid choice. Testing cubic cell by default." << std::endl;
            }
         }

         // Display the test cell
         std::cout << "\nTest cell parameters: " << testCell << std::endl;

         // Run the tests
         transformTest.TestTransformations(testCell);
      }
   } while (continueChoice == 'y' || continueChoice == 'Y');

   std::cout << "Tests completed. Exiting program." << std::endl;

   return 0;
}