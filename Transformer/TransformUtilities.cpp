#include "TransformerUtilities.h"
#include "TransformationMatrices.h" // For matrix sets

namespace TransformerUtilities {
   void SortByP3Distance(std::vector<TransformResult>& transformations) {
      // Simple sort by P3 distance
      std::sort(transformations.begin(), transformations.end(),
         [](const TransformResult& a, const TransformResult& b) {
            return a.p3DistanceFinal < b.p3DistanceFinal;
         });
   }

   void SortByS6Angle(std::vector<TransformResult>& transformations) {
      // Simple sort by S6 angle
      std::sort(transformations.begin(), transformations.end(),
         [](const TransformResult& a, const TransformResult& b) {
            return a.s6AngleFinal < b.s6AngleFinal;
         });
   }

   std::set<Matrix_3x3> GetComprehensiveTransformationMatrices() {
      std::set<Matrix_3x3> matrices;  // Use set for automatic uniqueness

      // Add matrices from MatrixSets
      for (const auto& matrix : MatrixSets::PERMUTATIONS) {
         matrices.insert(matrix);
      }

      for (const auto& matrix : MatrixSets::SHEARS) {
         matrices.insert(matrix);
      }

      for (const auto& matrix : MatrixSets::COMPLEX) {
         matrices.insert(matrix);
      }

      // Add our special cyclic permutation 
      matrices.insert(Matrix_3x3(0, 1, 0, 0, 0, 1, 1, 0, 0));

      // Also add its negative version
      matrices.insert(Matrix_3x3(0, -1, 0, 0, 0, -1, 1, 0, 0));

      return matrices;
   }


   Matrix_3x3 ComposeCompleteTransformation(
      const Matrix_3x3& refToPrimMatrix,      // M₁R
      const Matrix_3x3& refNiggliMatrix,      // M₂R
      const Matrix_3x3& permutationMatrix,    // [P]
      const Matrix_3x3& mobileNiggliMatrix,   // M₂M
      const Matrix_3x3& mobileToPrimMatrix,   // M₁M
      bool useNiggli
   ) {
      if (useNiggli) {
         // Full chain with Niggli reduction
         // M_1R^-1 * [P] * M_MR * M_2M * M_1M
         Matrix_3x3 result = refToPrimMatrix.Inverse();
         result = result * permutationMatrix;
         result = result * refNiggliMatrix;
         result = result * mobileNiggliMatrix.Inverse();
         result = result * mobileToPrimMatrix;
         return result;
      }
      else {
         // Simplified chain without Niggli reduction
         // M_1R^-1 * [P] * M_1M
         Matrix_3x3 result = refToPrimMatrix.Inverse();
         result = result * permutationMatrix;
         result = result * mobileToPrimMatrix;
         return result;
      }
   }


   void AddStandardTransformations(
      std::vector<TransformResult>& transformations,
      const LRL_Cell& sourceCell,
      const LRL_Cell& referenceCell,
      const Matrix_3x3& refToPrimMatrix,
      const Matrix_3x3& refNiggliMatrix,
      const Matrix_3x3& mobileNiggliMatrix,
      const Matrix_3x3& mobileToPrimMatrix
   ) {
      // Get a comprehensive set of transformation matrices
      std::set<Matrix_3x3> standardMatriceSet = GetComprehensiveTransformationMatrices();

      // Create transformations for each matrix
      for (const auto& primMatrix : standardMatriceSet) {
         // Compose the complete transformation
         Matrix_3x3 completeMatrix = TransformerUtilities::ComposeCompleteTransformation(
            refToPrimMatrix,     // M₁R
            refNiggliMatrix,     // M₂R
            primMatrix,          // [P]
            mobileNiggliMatrix,  // M₂M
            mobileToPrimMatrix   // M₁M
         );

         // Skip transformations with non-positive determinants
         if (completeMatrix.Det() <= 0.0) {
            continue;
         }

         // Create a new transformation result
         TransformResult result;

         // Set the primitive-to-primitive transformation
         result.transformMatrix = primMatrix;
         result.primitiveToPrimitiveMatrix = primMatrix;

         // Set the complete transformation matrix
         result.completeTransformMatrix = completeMatrix;

         // Apply transformation to get transformed B4
         result.transformedB4 = primMatrix * B4(sourceCell);

         // Calculate the transformed complete cell
         result.transformedCompleteCell = completeMatrix * sourceCell;

         // Calculate S6 angle
         S6 transformedS6(result.transformedCompleteCell);
         S6 referenceS6(referenceCell);
         result.s6AngleFinal = S6::AngleBetween(transformedS6, referenceS6);

         // Calculate P3 distance
         result.p3DistanceFinal = P3::DistanceBetween(result.transformedCompleteCell, referenceCell);

         // Add to results list
         transformations.push_back(result);
      }
   }
}

// TransformUtilities.cpp - Add these implementations

// Debug utility for matrix representation
void debugMatrixRepresentation(const Matrix_3x3& matrix, const std::string& label) {
   std::cout << "\nMatrix Representation Debug: " << label << std::endl;

   // Display raw values
   std::cout << "Raw elements in memory:" << std::endl;
   for (int i = 0; i < 9; i++) {
      std::cout << matrix[i] << " ";
      if (i % 3 == 2) std::cout << std::endl;
   }

   // Display as 3x3 matrix
   std::cout << "\nAs 3x3 matrix:" << std::endl;
   for (int i = 0; i < 3; i++) {
      std::cout << "  ";
      for (int j = 0; j < 3; j++) {
         std::cout << matrix[i * 3 + j] << " ";
      }
      std::cout << std::endl;
   }

   std::cout << "Determinant: " << matrix.Det() << std::endl;
}


// Function to show usage information
void showUsageInformation(const MultiTransformFinderControls& controls) {
   std::cout << "Error: Need at least two cells for transformation." << std::endl;
   std::cout << "Usage: Transformer [options] [reference_cell] [mobile_cell1] [mobile_cell2] ..." << std::endl;
   std::cout << "Options:" << std::endl;
   std::cout << "  SHOWDETAILS or DETAILS [true/false]  - Show detailed output" << std::endl;
   std::cout << "  SHOWTRANSFORMINFO [true/false]       - Show extra transformation details" << std::endl;
   std::cout << "  MATRIXSEARCHDEPTH [number]           - Set maximum search depth for matrices" << std::endl;
   std::cout << "  MAXTRANSFORMS [number]               - Set maximum transformations to display" << std::endl;
   std::cout << "  MAXDISTANCE [number]                 - Set maximum allowed transformation distance" << std::endl;
   std::cout << "  DISTANCETHRESHOLD [number]           - Set distance threshold for transformations" << std::endl;
   std::cout << "  INCLUDEIDENTITY [true/false]         - Include identity matrix in results" << std::endl;
   std::cout << "  SHOWCONTROLS or SHOW [true/false]    - Show control settings" << std::endl;
}

// Test specific matrices with B4
void testSpecificMatricesWithB4(const LRL_Cell& sourceCell, const LRL_Cell& targetCell) {
   std::cout << "Testing specific transformation matrices using B4..." << std::endl;

   // Define matrices to test
   std::vector<Matrix_3x3> matricesToTest;

   // 1. Identity
   matricesToTest.push_back(Matrix_3x3(
      1, 0, 0,
      0, 1, 0,
      0, 0, 1
   ));

   // 2. Permutation (swap a and c)
   matricesToTest.push_back(Matrix_3x3(
      0, 0, 1,
      1, 0, 0,
      0, 1, 0
   ));

   // 3. Complex matrix from previous tests
   matricesToTest.push_back(Matrix_3x3(
      3, -3, -1,
      1, 2, 2,
      -3, -1, -2
   ));

   std::cout << "Testing " << matricesToTest.size() << " specific transformation matrices..." << std::endl;

   std::cout << "Source Cell:\n        " << LRL_Cell_Degrees(sourceCell) << std::endl;
   std::cout << "Target Cell:\n        " << LRL_Cell_Degrees(targetCell) << std::endl;

   std::cout << "\nTesting each transformation matrix:\n" << std::endl;

   // Create B4 representations
   const B4 sourceB4(sourceCell);
   const B4 targetB4(targetCell);

   // Test each matrix
   int count = 0;
   for (const auto& matrix : matricesToTest) {
      count++;
      std::cout << "Matrix " << count << ":" << std::endl;
      std::cout << matrix << std::endl;

      // Calculate determinant
      double det = matrix.Det();
      std::cout << "Determinant: " << det << std::endl;

      // Apply transformation directly to B4
      std::cout << sourceB4 << std::endl;
      const B4 transformedB4 = matrix * sourceB4;
      std::cout << "transformedB4 " << transformedB4 << std::endl;

      // Construct the transformed cell (for display only)
      const LatticeCell transformedCell = LatticeCell(transformedB4);

      std::cout << "Transformed Cell (B):\n      " << (transformedCell.getCell()) << std::endl;
      std::cout << "Transformed Cell (B):\n        " << LRL_Cell_Degrees(transformedCell.getCell()) << std::endl;

      // Calculate distances
      S6 s1 = S6(transformedCell.getCell());
      S6 s2 = S6(targetCell);
      S6 reduced1, reduced2;
      Selling::Reduce(s1, reduced1);
      Selling::Reduce(s2, reduced2);
      const double cs6Dist = CS6Dist(reduced1.data(), reduced2.data());

      // Calculate B4 distance
      const double b4Distance = B4::DistanceBetween(transformedB4, targetB4);

      // Calculate Euclidean distance (for reference)
      double euclideanDist = 0.0;
      for (int i = 0; i < 6; i++) {
         double diff = s1[i] - s2[i];
         euclideanDist += diff * diff;
      }
      euclideanDist = std::sqrt(euclideanDist);

      std::cout << "B4 Distance = " << b4Distance << std::endl;
      std::cout << "CS6Dist = " << cs6Dist << ", Euclidean Dist = " << euclideanDist << std::endl;

      std::cout << "--------------------------" << std::endl;
   }
}

Matrix_3x3 ComposeCompleteTransformation(
   const Matrix_3x3& refToPrimMatrix,      // M₁R
   const Matrix_3x3& refNiggliMatrix,      // M₂R
   const Matrix_3x3& permutationMatrix,    // [P]
   const Matrix_3x3& mobileNiggliMatrix,   // M₂M
   const Matrix_3x3& mobileToPrimMatrix,   // M₁M
   bool useNiggli
) {
   if (useNiggli) {
      // Full chain with Niggli reduction
      Matrix_3x3 result = refToPrimMatrix.Inverse();
      result = result * permutationMatrix;
      result = result * refNiggliMatrix;
      result = result * mobileNiggliMatrix.Inverse();
      result = result * mobileToPrimMatrix;
      return result;
   }
   else {
      // Simplified chain without Niggli reduction
      Matrix_3x3 result = refToPrimMatrix.Inverse();
      result = result * permutationMatrix;
      result = result * mobileToPrimMatrix;
      return result;
   }
}


Matrix_3x3 CalculateNiggliReductionMatrix(const LRL_Cell& cell) {
   // Convert cell to G6 form
   G6 g6Cell = cell;

   // Get the Niggli reduced cell
   G6 niggliReducedG6;
   Matrix_3x3 m3d;
   MatG6 mg6;
   Niggli::ReduceWithTransforms(g6Cell, mg6, m3d, niggliReducedG6, 0.001);
   return m3d;
}

// Add this function to TransformerUtilities.cpp

Matrix_3x3 ComposeTransformationMatrix(
   const Matrix_3x3& refToPrimMatrix,
   const Matrix_3x3& permutationMatrix,
   const Matrix_3x3& mobileToPrimMatrix,
   bool useNiggli,
   const Matrix_3x3& refNiggliMatrix,
   const Matrix_3x3& mobileNiggliMatrix
) {
   // Use the existing Inver() method
   Matrix_3x3 refToPrimInverse = refToPrimMatrix.Inver();

   if (!useNiggli) {
      // Simplified chain for non-Niggli case
      return refToPrimInverse * permutationMatrix * mobileToPrimMatrix;
   }
   else {
      // Full chain for Niggli case
      Matrix_3x3 mobileNiggliInverse = mobileNiggliMatrix.Inver();
      return refToPrimInverse * permutationMatrix * refNiggliMatrix *
         mobileNiggliInverse * mobileToPrimMatrix;
   }
}




std::vector<Matrix_3x3> generateBasicPermutationMatrices() {
   std::vector<Matrix_3x3> matrices;

   // Add identity matrix
   matrices.push_back(Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1));

   // Add basic permutation matrices
   matrices.push_back(Matrix_3x3(0, 1, 0, 1, 0, 0, 0, 0, 1));  // Swap x,y
   matrices.push_back(Matrix_3x3(0, 0, 1, 0, 1, 0, 1, 0, 0));  // Swap y,z
   matrices.push_back(Matrix_3x3(1, 0, 0, 0, 0, 1, 0, 1, 0));  // Swap x,z

   // Add more permutation matrices as needed

   return matrices;
}

