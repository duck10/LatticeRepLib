#pragma warning(disable: 4101) // Visual Studio 
#pragma warning(disable: 4566) // Visual Studio 

#include "CenteringExperiment.h"

//#include "LRL_Cell.h"
//#include "LRL_Cell_Degrees.h"
//#include "Matrix_3x3.h"
//#include "LatticeCell.h"
//#include "G6.h"
//#include "NCDist.h"
//#include "P3.h"
//#include "TransformerUtilities.h"
//#include "LatticeTransformer.h"
//#include <iostream>
//#include <iomanip>
//
//void verifyCenteringMatrices() {
//   std::cout << "=== Verifying C Centering Matrices ===" << std::endl;
//
//   // Standard C to P matrix
//   const Matrix_3x3 C_to_P_standard(
//      0.5, 0.5, 0,
//      -0.5, 0.5, 0,
//      0, 0, 1
//   );
//
//   // Let's also check the IUCr standard
//   const Matrix_3x3 C_to_P_IUCr(
//      1.0, 0.0, 0.0,
//      0.0, 0.5, 0.5,
//      0.0, -0.5, 0.5
//   );
//
//   // Test cells
//   const LRL_Cell cCell(10, 20, 30, 90, 99, 90);  // C-centered
//   const LRL_Cell pCell(10, 11.18, 30, 94.012, 99, 63.435);  // P primitive
//
//   std::cout << "C-centered cell: " << LRL_Cell_Degrees(cCell) << std::endl;
//   std::cout << "P primitive cell: " << LRL_Cell_Degrees(pCell) << std::endl;
//
//   // Test different C to P matrices
//   std::cout << "\n--- Testing standard C to P matrix ---" << std::endl;
//   std::cout << "Matrix:" << std::endl;
//   std::cout << C_to_P_standard << std::endl;
//
//   const LRL_Cell primFromC_standard = C_to_P_standard * cCell;
//   std::cout << "Primitive from C: " << LRL_Cell_Degrees(primFromC_standard) << std::endl;
//
//   // Check NCDist
//   G6 g6_standard(primFromC_standard);
//   G6 g6_p(pCell);
//   double ncDist_standard = NCDist(g6_standard.data(), g6_p.data());
//   std::cout << "NCDist to P cell: " << ncDist_standard << std::endl;
//
//   std::cout << "\n--- Testing IUCr C to P matrix ---" << std::endl;
//   std::cout << "Matrix:" << std::endl;
//   std::cout << C_to_P_IUCr << std::endl;
//
//   const LRL_Cell primFromC_IUCr = C_to_P_IUCr * cCell;
//   std::cout << "Primitive from C: " << LRL_Cell_Degrees(primFromC_IUCr) << std::endl;
//
//   G6 g6_IUCr(primFromC_IUCr);
//   double ncDist_IUCr = NCDist(g6_IUCr.data(), g6_p.data());
//   std::cout << "NCDist to P cell: " << ncDist_IUCr << std::endl;
//
//   // Check what matrix is actually being used
//   std::cout << "\n--- Testing what LatticeTransformer uses ---" << std::endl;
//
//   LatticeCell latticeCellC(cCell, "C");
//   MultiTransformFinderControls controls;
//   controls.setShowDetails(false);
//
//   LatticeTransformer transformer(controls);
//
//   // Just get the centering matrix
//   const Matrix_3x3 toPrimitive = ToPrimitive("C", cCell);
//   std::cout << "ToPrimitive(\"C\") returns:" << std::endl;
//   std::cout << toPrimitive << std::endl;
//
//   // Check if it matches either standard
//   bool matchesStandard = true;
//   bool matchesIUCr = true;
//
//   for (int i = 0; i < 9; ++i) {
//      if (std::abs(toPrimitive[i] - C_to_P_standard[i]) > 1e-10) {
//         matchesStandard = false;
//      }
//      if (std::abs(toPrimitive[i] - C_to_P_IUCr[i]) > 1e-10) {
//         matchesIUCr = false;
//      }
//   }
//
//   std::cout << "\nMatches standard C to P: " << (matchesStandard ? "YES" : "NO") << std::endl;
//   std::cout << "Matches IUCr C to P: " << (matchesIUCr ? "YES" : "NO") << std::endl;
//
//   // Test the complete transformation with the actual matrix
//   const LRL_Cell primFromC_actual = toPrimitive * cCell;
//   std::cout << "\nPrimitive from C (actual): " << LRL_Cell_Degrees(primFromC_actual) << std::endl;
//
//   G6 g6_actual(primFromC_actual);
//   double ncDist_actual = NCDist(g6_actual.data(), g6_p.data());
//   std::cout << "NCDist to P cell: " << ncDist_actual << std::endl;
//
//   // Find the best transformation between primitive forms
//   std::cout << "\n=== Finding best transformation ===" << std::endl;
//
//   // Test matrix from our earlier analysis
//   const Matrix_3x3 bestMatrix(-1, 1, 0, 0, 1, 0, 0, 0, -1);
//
//   // Apply to each primitive form
//   std::cout << "\nUsing standard C to P:" << std::endl;
//   LRL_Cell transformed_standard = bestMatrix * primFromC_standard;
//   double p3_standard = P3::DistanceBetween(transformed_standard, pCell);
//   std::cout << "P3 distance: " << p3_standard << " Å" << std::endl;
//
//   std::cout << "\nUsing IUCr C to P:" << std::endl;
//   LRL_Cell transformed_IUCr = bestMatrix * primFromC_IUCr;
//   double p3_IUCr = P3::DistanceBetween(transformed_IUCr, pCell);
//   std::cout << "P3 distance: " << p3_IUCr << " Å" << std::endl;
//
//   std::cout << "\nUsing actual C to P:" << std::endl;
//   LRL_Cell transformed_actual = bestMatrix * primFromC_actual;
//   double p3_actual = P3::DistanceBetween(transformed_actual, pCell);
//   std::cout << "P3 distance: " << p3_actual << " Å" << std::endl;
//}
//
//int main() {
//   std::cout << std::fixed << std::setprecision(8);
//   verifyCenteringMatrices();
//   return 0;
//}

#include "MultiTransformFinderControls.h"
#include "LatticeCell.h"
#include "B4Matcher.h"
#include "TransformerDisplay.h"
#include "LatticeTransformer.h"
#include "TransformationMatrices.h"
#include "TransformerUtilities.h"
#include "InputHandler.h"
#include "ProgramSetup.h"
#include "LRL_Cell.h"
#include "Niggli.h"
#include "G6.h"
#include "StoreResults.h"

#include <iomanip>  // for std::setprecision
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>


void debugTransformResult(const CompleteTransformationResult& result, const std::string& label) {
   std::cout << "DEBUG " << label << ":\n";
   if (result.primitiveResult.isValid && !result.primitiveResult.transformations.empty()) {
      const auto& transform = result.primitiveResult.transformations[0];
      std::cout << "  p3DistanceFinal: " << transform.p3DistanceFinal << "\n";
      std::cout << "  b4Distance: " << transform.b4Distance << "\n";
      std::cout << "  cs6DistanceFinal: " << transform.cs6DistanceFinal << "\n";
      std::cout << "  s6AngleFinal: " << transform.s6AngleFinal << "\n";
   }
   std::cout << "\n";
}


void outputCsvRow(const CompleteTransformationResult& standardResult,
   const CompleteTransformationResult& niggliResult,
   size_t cellIndex,
   const LatticeCell& mobileCell,
   const LatticeCell& referenceCell) {

   double standardP3 = 0.0, niggliP3 = 0.0;
   double ncdist = 0.0;

   // Calculate NCDist once (it's the same regardless of transformation method)
   const LRL_Cell reducedReference = NiggliReduce(referenceCell);
   const LRL_Cell reducedMobile = NiggliReduce(mobileCell);
   ncdist = CalculateNCDist(reducedMobile, reducedReference);

   // Extract P3 data from standard result
   if (standardResult.primitiveResult.isValid &&
      !standardResult.primitiveResult.transformations.empty()) {
      const auto& transform = standardResult.primitiveResult.transformations[0];
      standardP3 = transform.p3DistanceFinal;
   }

   // Extract P3 data from Niggli result  
   if (niggliResult.primitiveResult.isValid &&
      !niggliResult.primitiveResult.transformations.empty()) {
      const auto& transform = niggliResult.primitiveResult.transformations[0];
      niggliP3 = transform.p3DistanceFinal;
   }

   // Output CSV row
   std::cout << cellIndex << ","
      << standardP3 << ","
      << niggliP3 << ","
      << ncdist << "\n";
}

void testDirectTransformation() {
   // Input cell and reference cell
   const double DEG_TO_RAD = 1.0;
   LRL_Cell inputCell(30.0, 1.0, 2.0, DEG_TO_RAD * 55.0, DEG_TO_RAD * 33.0, DEG_TO_RAD * 44.0);
   LRL_Cell referenceCell(1.0, 2.0, 30.0, DEG_TO_RAD * 33.0, DEG_TO_RAD * 44.0, DEG_TO_RAD * 55.0);

   std::cout << "\n===== TESTING ALL PERMUTATION MATRICES =====\n" << std::endl;

   // Get all permutation matrices from MatrixSets
   std::vector<Matrix_3x3> allPermutations = MatrixSets::getAllPermutationMatrices();

   std::cout << "Input cell: " << LRL_Cell_Degrees(inputCell) << std::endl;
   std::cout << "Reference cell: " << LRL_Cell_Degrees(referenceCell) << std::endl;
   std::cout << "\nTesting " << allPermutations.size() << " permutation matrices:" << std::endl;

   // Track best transformation
   double bestDistance = std::numeric_limits<double>::max();
   Matrix_3x3 bestMatrix;
   LRL_Cell bestTransformed;
   std::string bestMatrixName;

   // Test each matrix
   for (size_t i = 0; i < allPermutations.size(); ++i) {
      const Matrix_3x3& matrix = allPermutations[i];

      // Get matrix name if available
      std::string matrixName = MatrixSets::getPermutationName(matrix);
      if (matrixName == "Unknown permutation") {
         matrixName = "Matrix #" + std::to_string(i);
      }

      // Apply transformation
      LRL_Cell transformedCell = matrix * inputCell;

      // Calculate distance metric
      double distance = P3::DistanceBetween(transformedCell, referenceCell);

      // Output results
      std::cout << "\n" << matrixName << std::endl;
      std::cout << "Matrix: " << matrix << std::endl;
      std::cout << "Det: " << matrix.Det() << std::endl;
      std::cout << "Transformed cell: " << LRL_Cell_Degrees(transformedCell) << std::endl;
      std::cout << "P3 Distance: " << distance << std::endl;

      // Update best if this is better
      if (distance < bestDistance) {
         bestDistance = distance;
         bestMatrix = matrix;
         bestTransformed = transformedCell;
         bestMatrixName = matrixName;
      }
   }

   // Report the best transformation
   std::cout << "\n===== BEST TRANSFORMATION =====\n" << std::endl;
   std::cout << "Best matrix: " << bestMatrixName << std::endl;
   std::cout << "Matrix: " << bestMatrix << std::endl;
   std::cout << "Transformed cell: " << LRL_Cell_Degrees(bestTransformed) << std::endl;
   std::cout << "P3 Distance: " << bestDistance << std::endl;

   // Test original cyclic permutation for comparison
   std::cout << "\n===== ORIGINAL TEST FOR COMPARISON =====\n" << std::endl;

   // Create the cyclic permutation matrix
   Matrix_3x3 cyclicPerm(0.0, 1.0, 0.0,
      0.0, 0.0, 1.0,
      1.0, 0.0, 0.0);

   // Apply transformation and verify
   LRL_Cell transformedCell = cyclicPerm * inputCell;
   std::cout << "Cyclic permutation matrix (xyz->yzx):" << std::endl;
   std::cout << "Matrix: " << cyclicPerm << std::endl;
   std::cout << "Det: " << cyclicPerm.Det() << std::endl;
   std::cout << "Transformed cell: " << LRL_Cell_Degrees(transformedCell) << std::endl;
   std::cout << "P3 Distance: " << P3::DistanceBetween(transformedCell, referenceCell) << std::endl;

   // Test shear matrices
   std::cout << "\n===== TESTING SHEAR MATRICES =====\n" << std::endl;
   std::vector<Matrix_3x3> allShears = MatrixSets::SHEARS;

   std::cout << "Testing " << allShears.size() << " shear matrices:\n" << std::endl;

   // Track best shear transformation
   double bestShearDistance = std::numeric_limits<double>::max();
   Matrix_3x3 bestShearMatrix;
   LRL_Cell bestShearTransformed;
   int bestShearIndex = -1;

   // Test each shear matrix
   for (size_t i = 0; i < allShears.size(); ++i) {
      const Matrix_3x3& matrix = allShears[i];

      // Apply transformation
      LRL_Cell transformedCell = matrix * inputCell;

      // Calculate distance metric
      double distance = P3::DistanceBetween(transformedCell, referenceCell);

      // Output results
      std::cout << "Shear Matrix #" << i << std::endl;
      std::cout << "Matrix: " << matrix << std::endl;
      std::cout << "Det: " << matrix.Det() << std::endl;
      std::cout << "Transformed cell: " << LRL_Cell_Degrees(transformedCell) << std::endl;
      std::cout << "P3 Distance: " << distance << std::endl << std::endl;

      // Update best if this is better
      if (distance < bestShearDistance) {
         bestShearDistance = distance;
         bestShearMatrix = matrix;
         bestShearTransformed = transformedCell;
         bestShearIndex = int(i);
      }
   }

   // Report the best shear transformation
   std::cout << "Best shear matrix: #" << bestShearIndex << std::endl;
   std::cout << "Matrix: " << bestShearMatrix << std::endl;
   std::cout << "Transformed cell: " << LRL_Cell_Degrees(bestShearTransformed) << std::endl;
   std::cout << "P3 Distance: " << bestShearDistance << std::endl;


   if (bestDistance < bestShearDistance) {
      std::cout << "Permutation matrices perform better for this test case." << std::endl;
   }
   else {
      std::cout << "Shear matrices perform better for this test case." << std::endl;
   }
}


void testMatrixDeterminants() {
   std::cout << "\n===== CHECKING DETERMINANTS OF ALL MATRICES =====\n" << std::endl;

   const double epsilon = 1e-10;
   bool allValid = true;

   // Check individual permutation matrices
   std::cout << "PERMUTATION MATRICES:\n" << std::endl;

   // Check Identity
   double det = MatrixSets::IDENTITY.Det();
   bool valid = std::abs(det - 1.0) < epsilon;
   std::cout << "IDENTITY: det = " << det << (valid ? " ✓" : " ✗") << std::endl;
   allValid &= valid;

   // Check permutation matrices
   det = MatrixSets::PERM_CYCLIC_1.Det();
   valid = std::abs(det - 1.0) < epsilon;
   std::cout << "PERM_CYCLIC_1: det = " << det << (valid ? " ✓" : " ✗") << std::endl;
   allValid &= valid;

   det = MatrixSets::PERM_CYCLIC_2.Det();
   valid = std::abs(det - 1.0) < epsilon;
   std::cout << "PERM_CYCLIC_2: det = " << det << (valid ? " ✓" : " ✗") << std::endl;
   allValid &= valid;

   // Check odd permutations
   det = MatrixSets::PERM_SWAP_XY.Det();
   valid = std::abs(det - 1.0) < epsilon;
   std::cout << "PERM_SWAP_XY: det = " << det << (valid ? " ✓" : " ✗") << std::endl;
   allValid &= valid;

   det = MatrixSets::PERM_SWAP_YZ.Det();
   valid = std::abs(det - 1.0) < epsilon;
   std::cout << "PERM_SWAP_YZ: det = " << det << (valid ? " ✓" : " ✗") << std::endl;
   allValid &= valid;

   det = MatrixSets::PERM_SWAP_XZ.Det();
   valid = std::abs(det - 1.0) < epsilon;
   std::cout << "PERM_SWAP_XZ: det = " << det << (valid ? " ✓" : " ✗") << std::endl;
   allValid &= valid;

   // Check sign flips
   det = MatrixSets::FLIP_XY.Det();
   valid = std::abs(det - 1.0) < epsilon;
   std::cout << "FLIP_XY: det = " << det << (valid ? " ✓" : " ✗") << std::endl;
   allValid &= valid;

   det = MatrixSets::FLIP_XZ.Det();
   valid = std::abs(det - 1.0) < epsilon;
   std::cout << "FLIP_XZ: det = " << det << (valid ? " ✓" : " ✗") << std::endl;
   allValid &= valid;

   det = MatrixSets::FLIP_YZ.Det();
   valid = std::abs(det - 1.0) < epsilon;
   std::cout << "FLIP_YZ: det = " << det << (valid ? " ✓" : " ✗") << std::endl;
   allValid &= valid;

   // Check complex matrices
   det = MatrixSets::COMPLEX_1.Det();
   valid = std::abs(det - 1.0) < epsilon;
   std::cout << "COMPLEX_1: det = " << det << (valid ? " ✓" : " ✗") << std::endl;
   allValid &= valid;

   det = MatrixSets::COMPLEX_2.Det();
   valid = std::abs(det - 1.0) < epsilon;
   std::cout << "COMPLEX_2: det = " << det << (valid ? " ✓" : " ✗") << std::endl;
   allValid &= valid;

   // Check shear matrices
   std::cout << "\nSHEAR MATRICES:\n" << std::endl;

   det = MatrixSets::SHEAR_XY.Det();
   valid = std::abs(det - 1.0) < epsilon;
   std::cout << "SHEAR_XY: det = " << det << (valid ? " ✓" : " ✗") << std::endl;
   allValid &= valid;

   det = MatrixSets::SHEAR_XZ.Det();
   valid = std::abs(det - 1.0) < epsilon;
   std::cout << "SHEAR_XZ: det = " << det << (valid ? " ✓" : " ✗") << std::endl;
   allValid &= valid;

   det = MatrixSets::SHEAR_YX.Det();
   valid = std::abs(det - 1.0) < epsilon;
   std::cout << "SHEAR_YX: det = " << det << (valid ? " ✓" : " ✗") << std::endl;
   allValid &= valid;

   det = MatrixSets::SHEAR_YZ.Det();
   valid = std::abs(det - 1.0) < epsilon;
   std::cout << "SHEAR_YZ: det = " << det << (valid ? " ✓" : " ✗") << std::endl;
   allValid &= valid;

   det = MatrixSets::SHEAR_ZX.Det();
   valid = std::abs(det - 1.0) < epsilon;
   std::cout << "SHEAR_ZX: det = " << det << (valid ? " ✓" : " ✗") << std::endl;
   allValid &= valid;

   det = MatrixSets::SHEAR_ZY.Det();
   valid = std::abs(det - 1.0) < epsilon;
   std::cout << "SHEAR_ZY: det = " << det << (valid ? " ✓" : " ✗") << std::endl;
   allValid &= valid;

   det = MatrixSets::SHEAR_XY_NEG.Det();
   valid = std::abs(det - 1.0) < epsilon;
   std::cout << "SHEAR_XY_NEG: det = " << det << (valid ? " ✓" : " ✗") << std::endl;
   allValid &= valid;

   det = MatrixSets::SHEAR_XZ_NEG.Det();
   valid = std::abs(det - 1.0) < epsilon;
   std::cout << "SHEAR_XZ_NEG: det = " << det << (valid ? " ✓" : " ✗") << std::endl;
   allValid &= valid;

   det = MatrixSets::SHEAR_YX_NEG.Det();
   valid = std::abs(det - 1.0) < epsilon;
   std::cout << "SHEAR_YX_NEG: det = " << det << (valid ? " ✓" : " ✗") << std::endl;
   allValid &= valid;

   det = MatrixSets::SHEAR_YZ_NEG.Det();
   valid = std::abs(det - 1.0) < epsilon;
   std::cout << "SHEAR_YZ_NEG: det = " << det << (valid ? " ✓" : " ✗") << std::endl;
   allValid &= valid;

   det = MatrixSets::SHEAR_ZX_NEG.Det();
   valid = std::abs(det - 1.0) < epsilon;
   std::cout << "SHEAR_ZX_NEG: det = " << det << (valid ? " ✓" : " ✗") << std::endl;
   allValid &= valid;

   det = MatrixSets::SHEAR_ZY_NEG.Det();
   valid = std::abs(det - 1.0) < epsilon;
   std::cout << "SHEAR_ZY_NEG: det = " << det << (valid ? " ✓" : " ✗") << std::endl;
   allValid &= valid;

   // Check all matrices in arrays
   std::cout << "\nCHECKING ALL MATRICES IN ARRAYS:\n" << std::endl;

   // Check PERMUTATIONS array
   std::cout << "PERMUTATIONS array (" << sizeof(MatrixSets::PERMUTATIONS) / sizeof(MatrixSets::PERMUTATIONS[0]) << " matrices):" << std::endl;
   int i = 0;
   for (const auto& matrix : MatrixSets::PERMUTATIONS) {
      ++i;
      det = matrix.Det();
      valid = std::abs(det - 1.0) < epsilon;
      std::cout << "  Matrix #" << i << ": det = " << det << (valid ? " ✓" : " ✗") << std::endl;
      allValid &= valid;
   }

   // Check SHEARS array
   std::cout << "\nSHEARS array (" << sizeof(MatrixSets::SHEARS) / sizeof(MatrixSets::SHEARS[0]) << " matrices):" << std::endl;
   for (int i = 0; i < sizeof(MatrixSets::SHEARS) / sizeof(MatrixSets::SHEARS[0]); ++i) {
      det = MatrixSets::SHEARS[i].Det();
      valid = std::abs(det - 1.0) < epsilon;
      std::cout << "  Matrix #" << i << ": det = " << det << (valid ? " ✓" : " ✗") << std::endl;
      allValid &= valid;
   }

   // Check COMPLEX array
   std::cout << "\nCOMPLEX array (" << sizeof(MatrixSets::COMPLEX) / sizeof(MatrixSets::COMPLEX[0]) << " matrices):" << std::endl;
   for (int i = 0; i < sizeof(MatrixSets::COMPLEX) / sizeof(MatrixSets::COMPLEX[0]); ++i) {
      det = MatrixSets::COMPLEX[i].Det();
      valid = std::abs(det - 1.0) < epsilon;
      std::cout << "  Matrix #" << i << ": det = " << det << (valid ? " ✓" : " ✗") << std::endl;
      allValid &= valid;
   }

   // Check matrices returned by utility functions
   std::cout << "\nCHECKING MATRICES FROM UTILITY FUNCTIONS:\n" << std::endl;


   // Check getAllPermutationMatrices
   std::vector<Matrix_3x3> allPerms = MatrixSets::getAllPermutationMatrices();
   std::cout << "\ngetAllPermutationMatrices (" << allPerms.size() << " matrices):" << std::endl;
   for (size_t i = 0; i < allPerms.size(); ++i) {
      det = allPerms[i].Det();
      valid = std::abs(det - 1.0) < epsilon;
      std::cout << "  Matrix #" << i << ": det = " << det << (valid ? " ✓" : " ✗") << std::endl;
      allValid &= valid;
   }

   // Final summary
   std::cout << "\n===== DETERMINANT CHECK SUMMARY =====\n" << std::endl;
   if (allValid) {
      std::cout << "ALL MATRICES HAVE DETERMINANT = +1 ✓" << std::endl;
   }
   else {
      std::cout << "WARNING: SOME MATRICES DO NOT HAVE DETERMINANT = +1 ✗" << std::endl;
      std::cout << "Check the details above to identify problematic matrices." << std::endl;
   }
}

void printTransformationSeparator(size_t currentIndex, size_t totalCount) {
   std::cout << "\n\n=#==#==#==#==#==#==#==#==#==#==#==#==#==#==#===#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==\n";
   std::cout << "Transformation (Cell to Match to reference) " << currentIndex << " of " << totalCount << "\n";
   std::cout << "=#==#==#==#==#==#==#==#==#==#==#==#==#==#==#===#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==\n\n";
}

void compareP3Results(const CompleteTransformationResult& standardResult,
   const CompleteTransformationResult& niggliResult,
   size_t cellIndex) {
   std::cout << "Cell " << cellIndex << " P3 Comparison:\n";

   // Access the primitive results which contain the actual transformations
   const auto& standardPrimitive = standardResult.primitiveResult;
   const auto& niggliPrimitive = niggliResult.primitiveResult;

   // Check if both have valid transformations
   if (standardPrimitive.isValid && niggliPrimitive.isValid &&
      !standardPrimitive.transformations.empty() && !niggliPrimitive.transformations.empty()) {

      // Get the best (first) transformation from each result
      const auto& standardBest = standardPrimitive.transformations[0];
      const auto& niggliBest = niggliPrimitive.transformations[0];

      // Get P3 distances from the transformations - use p3DistanceFinal
      double standardP3 = standardBest.p3DistanceFinal;
      double niggliP3 = niggliBest.p3DistanceFinal;

      std::cout << "  Standard P3:  " << standardP3 << "\n";
      std::cout << "  Niggli P3:    " << niggliP3 << "\n";

      // Calculate difference
      double difference = std::abs(standardP3 - niggliP3);
      std::cout << "  Difference:   " << difference << "\n";

      if (difference < 0.001) {
         std::cout << "  Status:       SAME (within tolerance)\n";
      }
      else {
         std::cout << "  Status:       DIFFERENT\n";
      }

      // Also show number of transformations found
      std::cout << "  Standard transformations: " << standardPrimitive.transformations.size() << "\n";
      std::cout << "  Niggli transformations:   " << niggliPrimitive.transformations.size() << "\n";
   }
   else {
      std::cout << "  Standard valid: " << (standardPrimitive.isValid ? "Yes" : "No") << "\n";
      std::cout << "  Niggli valid:   " << (niggliPrimitive.isValid ? "Yes" : "No") << "\n";
      if (standardPrimitive.isValid && standardPrimitive.transformations.empty()) {
         std::cout << "  Standard transformations: 0\n";
      }
      if (niggliPrimitive.isValid && niggliPrimitive.transformations.empty()) {
         std::cout << "  Niggli transformations: 0\n";
      }
      std::cout << "  Status:         No valid transformations for comparison\n";
   }

   std::cout << "\n";
}


// MODE 1: Comparison Mode - Always runs both Niggli false and true, compares P3
// Add this structure for storing comparison data
struct ComparisonData {
   size_t cellIndex;
   double standardP3;
   double niggliP3;
   double difference;  // signed difference (standard - niggli)
   double ncdist;
   Matrix_3x3 standardMatrix;
   LRL_Cell mobileCell;
   LRL_Cell referenceCell;
   LRL_Cell standardTransformedCell;
   LRL_Cell niggliTransformedCell;

   // Constructor
   ComparisonData(int index, double std_p3, double nig_p3, double nc_dist,
      const Matrix_3x3& std_matrix,
      const LRL_Cell& mobile, const LRL_Cell& reference,
      const LRL_Cell& std_transformed, const LRL_Cell& nig_transformed)
      : cellIndex(index), standardP3(std_p3), niggliP3(nig_p3),
      difference(std_p3 - nig_p3), ncdist(nc_dist),
      standardMatrix(std_matrix), mobileCell(mobile), referenceCell(reference),
      standardTransformedCell(std_transformed), niggliTransformedCell(nig_transformed) {
   }
};

// Add operator<< for ComparisonData to work with StoreResults
std::ostream& operator<<(std::ostream& os, const ComparisonData& data) {
   os << "Cell " << data.cellIndex << ": Standard P3=" << std::fixed << std::setprecision(6)
      << data.standardP3 << ", Niggli P3=" << data.niggliP3
      << ", Difference=" << std::showpos << data.difference << std::noshowpos
      << ", NCDist=" << data.ncdist << "\n"
      << "Standard Matrix:\n" << data.standardMatrix << "\n"
      << "Standard Best Match:  " << LRL_Cell_Degrees(data.standardTransformedCell) << "\n"
      << "Niggli Best Match:    " << LRL_Cell_Degrees(data.niggliTransformedCell) << "\n";
   return os;
}

// MODE 1: Comparison Mode - Always runs both Niggli false and true, compares P3
void runComparisonMode(const std::vector<LatticeCell>& inputList,
   const LatticeCell& referenceCell,
   MultiTransformFinderControls& controls) {

   // Initialize StoreResults for three categories
   StoreResults<std::string, ComparisonData> resultStore(5);  // Keep 5 examples of each
   resultStore.SetTitle("P3 Distance Comparison: Standard vs Niggli Reduction");
   resultStore.SetKeyLabel("Category");
   resultStore.SetItemSeparator("----------------------------------------");
   resultStore.SetValueSeparator("");

   // CSV Header
   if (controls.shouldOutputCsv()) {
      std::cout << "CellIndex,StandardP3,NiggliP3,NCDist\n";
   }

   // STEP 1: COMPUTATION - Run both Niggli false and true
   std::vector<CompleteTransformationResult> standardResults;
   std::vector<CompleteTransformationResult> niggliResults;
   std::vector<LatticeCell> mobileCells;

   for (size_t i = 1; i < inputList.size(); ++i) {
      const LatticeCell& mobileCell = inputList[i];
      mobileCells.push_back(mobileCell);

      // Run with Niggli = false
      controls.setUseNiggliReduction(false);
      LatticeTransformer standardTransformer(controls);
      CompleteTransformationResult standardResult = standardTransformer.transformLattice(
         mobileCell, referenceCell);
      standardResults.push_back(standardResult);

      // Run with Niggli = true
      controls.setUseNiggliReduction(true);
      LatticeTransformer niggliTransformer(controls);
      CompleteTransformationResult niggliResult = niggliTransformer.transformLattice(
         mobileCell, referenceCell);
      niggliResults.push_back(niggliResult);
   }

   // STEP 2: CATEGORIZE AND STORE RESULTS
   const double tolerance = 0.001;  // Tolerance for "nearly equal"

   for (size_t i = 0; i < standardResults.size(); ++i) {
      double standardP3 = 0.0, niggliP3 = 0.0;
      double ncdist = 0.0;
      Matrix_3x3 standardMatrix, niggliMatrix;

      // Calculate NCDist once (it's the same regardless of transformation method)
      const LRL_Cell reducedReference = NiggliReduce(referenceCell);
      const LRL_Cell reducedMobile = NiggliReduce(mobileCells[i]);
      ncdist = CalculateNCDist(reducedMobile, reducedReference);

      // Extract P3 data, matrix, and transformed cells from both results
      LRL_Cell standardTransformed, niggliTransformed;

      if (standardResults[i].primitiveResult.isValid &&
         !standardResults[i].primitiveResult.transformations.empty()) {
         const auto& transform = standardResults[i].primitiveResult.transformations[0];
         standardP3 = transform.p3DistanceFinal;
         standardMatrix = transform.completeTransformMatrix;
         standardTransformed = transform.transformedCompleteCell;
      }

      if (niggliResults[i].primitiveResult.isValid &&
         !niggliResults[i].primitiveResult.transformations.empty()) {
         const auto& transform = niggliResults[i].primitiveResult.transformations[0];
         niggliP3 = transform.p3DistanceFinal;
         niggliTransformed = transform.transformedCompleteCell;
      }

      // Create comparison data with transformed cells
      ComparisonData compData(static_cast<int>(i + 1), standardP3, niggliP3, ncdist,
         standardMatrix, mobileCells[i].getCell(), referenceCell.getCell(),
         standardTransformed, niggliTransformed);

      // Categorize and store using signed difference
      double difference = standardP3 - niggliP3;

      if (std::abs(difference) <= tolerance) {
         resultStore.Store("Nearly Equal", compData);
      }
      else if (difference > tolerance) {
         resultStore.Store("Standard > Niggli", compData);
      }
      else {
         resultStore.Store("Standard < Niggli", compData);
      }

      // Output CSV if requested
      if (controls.shouldOutputCsv()) {
         outputCsvRow(standardResults[i], niggliResults[i], static_cast<int>(i + 1),
            mobileCells[i], referenceCell);
      }
   }

   // STEP 3: OUTPUT RESULTS
   if (!controls.shouldOutputCsv()) {
      // Only show basic input information, skip detailed transformations
      std::cout << "\nProcessed " << mobileCells.size() << " mobile cells against reference cell.\n";
      std::cout << "Reference cell: " << LRL_Cell_Degrees(referenceCell.getCell()) << "\n\n";
   }

   // STEP 4: SHOW STORERESULTS SUMMARY
   std::cout << "\n\n";
   std::cout << "================================================================================\n";
   std::cout << "                           COMPARISON SUMMARY\n";
   std::cout << "================================================================================\n";

   // Display the categorized results
   resultStore.ShowResults();

   // Additional summary statistics
   std::cout << "\n=== SUMMARY STATISTICS ===\n";
   std::cout << "Nearly Equal (|diff| <= " << tolerance << "): "
      << resultStore.GetItemCount("Nearly Equal") << " cases\n";
   std::cout << "Standard > Niggli (diff > " << tolerance << "): "
      << resultStore.GetItemCount("Standard > Niggli") << " cases\n";
   std::cout << "Standard < Niggli (diff < -" << tolerance << "): "
      << resultStore.GetItemCount("Standard < Niggli") << " cases\n";
   std::cout << "Total cases processed: " << resultStore.GetTotalSeen() << "\n";
}


// MODE 2: Standard Mode - Uses user's Niggli setting, shows standard output
void runStandardMode(const std::vector<LatticeCell>& inputList,
   const LatticeCell& referenceCell,
   const MultiTransformFinderControls& controls,
   LatticeTransformer& transformer) {

   // STEP 1: COMPUTATION - Run with user's settings
   std::vector<CompleteTransformationResult> transformationResults;
   std::vector<LatticeCell> mobileCells;

   for (size_t i = 1; i < inputList.size(); ++i) {
      const LatticeCell& mobileCell = inputList[i];
      mobileCells.push_back(mobileCell);

      CompleteTransformationResult result = transformer.transformLattice(
         mobileCell, referenceCell);
      transformationResults.push_back(result);
   }

   // STEP 2: OUTPUT - Display standard results
   TransformerDisplay display(controls);

   for (size_t i = 0; i < transformationResults.size(); ++i) {
      const LatticeCell& mobileCell = mobileCells[i];
      const CompleteTransformationResult& result = transformationResults[i];

      printTransformationSeparator(static_cast<int>(i + 1), static_cast<int>(transformationResults.size()));

      // Show input cells
      if (controls.shouldShowExtraTransformInfo()) {
         showTransformationInfo(mobileCell, referenceCell);
      }
      else {
         display.showInputCells(mobileCell, referenceCell);
      }

      // Display results
      display.showCompleteTransformations(result, mobileCell, referenceCell);
   }
}



int main(int argc, char* argv[]) {

   //p  10.000    11.180    30.000    94.012    99.000    63.435
   //   p  11.180    11.180    30.000    85.988    94.012    53.130
   // one step
   //const LRL_Cell cell1("10.000    11.180    30.000    94.012    99.000    63.435");
   //const LRL_Cell cell2("11.180    11.180    30.000    85.988    94.012    53.130");


   //10.000    11.000  1943.569    89.965    89.888    81.000
   //   10.000    11.000  2000.000    77.000    88.000    99.000
   // // 54 steps
   //const LRL_Cell cell1("10.000    11.000  2000.000    77.000    88.000    99.000");
   //const LRL_Cell cell2("10.000    11.000  1943.569    89.965    89.888    81.000");



   //Cell to Transform(Degrees) : P     4.181     4.529    10.000    91.444    96.000    97.470
   //   Reference Cell(Degrees) : P    10.000    20.000    30.000    10.000    11.000    12.000
   // 6 steps
   const LRL_Cell cell1("10.000    20.000    30.000    10.000    11.000    12.000");
   const LRL_Cell cell2("4.181     4.529    10.000    91.444    96.000    97.470");


   //testCenteringExperiment(cell1);
   testMatchingExperiment(cell1, cell2);
   //Matrix_3x3 m1(-1, 1, -1,      0, 1, -1,      - 1, 0, -1);
   //Matrix_3x3 miInverse = m1.Inverse();
   //std::cout << " m1\n" << m1 << std::endl;
   //std::cout << " miInverse\n" << miInverse << std::endl;

   // In main or another test function
   std::vector<Matrix_3x3> completeGroup = MatrixSets::getAllPermutationMatrices();

   //const Matrix_3x3 m33 = { 1,-1,0, 0,1,0, 0,-1,1 };
   //const LRL_Cell c3 = { 1,2,1,90,90, 90 };
   //const LRL_Cell out = m33 * c3;
   //std::cout << m33 << std::endl << std::endl;
   //std::cout << "P " << LRL_Cell_Degrees(c3) << std::endl;
   //std::cout << "P " << LRL_Cell_Degrees(out) << std::endl;
   //exit(0);

   //const LRL_Cell cell(LRL_Cell::rand());
   //const Matrix_3x3 toC = cCentered;
   //const Matrix_3x3 toF = faceCentered;
   //const LRL_Cell_Degrees outC = toC.Inver() * cell;
   //const LRL_Cell_Degrees outF = toF.Inver() * cell;
   //std::cout << "C " << outC << std::endl;
   //std::cout << "F " << outF << std::endl;
   //exit(0);

   //G6 junk;
   //MatG6 mjunk;
   //Niggli::Reduce(junk, mjunk, junk, 19191.);

   try {
      // initialize controls
      MultiTransformFinderControls controls;

      // Initialize program using  BasicProgramInput system
      BasicProgramInput<MultiTransformFinderControls> program_setup("Transformer", controls);
      std::vector<LatticeCell>& inputList = program_setup.getInputList();

      if (controls.shouldRunTests()) {
         //const std::vector<LatticeCell> testCells = RunTests(controls, 1);
         //inputList.insert(inputList.end(),testCells.begin(), testCells.end());
      }

      if (controls.shouldShowDetails())
      {
         testDirectTransformation();
         testMatrixDeterminants();
         testPermutationGroupClosure();
         Niggli::CheckAllNiggliMatrixDeterminants();
         // Save the complete group for future use by updating your TransformationMatrices.h
         // You could output them to a file for manual inclusion
         std::cout << "\nComplete group matrices:" << std::endl;
         for (size_t i = 0; i < completeGroup.size(); ++i) {
            //std::cout << "Matrix #" << i << ":" << std::endl;
            //std::cout << completeGroup[i] << std::endl;
         }
      }


      // Show control settings if requested
      if (controls.getShowControls()) {
         std::cout << controls << std::endl;
      }

      // Need at least two cells (reference and at least one mobile)
      if (inputList.size() < 2) {
         showUsageInformation(controls);
         return 1;
      }

      // Get the reference cell (first one in the list)
      const LatticeCell& referenceCell = inputList[0];

      // Create lattice transformer
      LatticeTransformer transformer(controls);

      if (controls.shouldRunComparisonMode()) {
         if (inputList.size() < 3) {
            std::cout << "ERROR: Comparison mode requires at least 2 mobile cells (3 total cells).\n";
            std::cout << "       You provided " << inputList.size() << " cells.\n";
            std::cout << "       Switching to standard mode.\n\n";
            runStandardMode(inputList, referenceCell, controls, transformer);
         }
         else {
            std::cout << "Running in COMPARISON MODE - comparing Niggli false vs true\n\n";
            runComparisonMode(inputList, referenceCell, controls);
         }
      }
      else {
         std::cout << "Running in STANDARD MODE - using current Niggli setting ("
            << (controls.shouldUseNiggliReduction() ? "true" : "false") << ")\n\n";
         runStandardMode(inputList, referenceCell, controls, transformer);
      }


      return 0;
   }
   catch (const std::exception& ex) {
      std::cerr << "Error: " << ex.what() << std::endl;
      return 1;
   }
   catch (...) {
      std::cerr << "Unknown error occurred" << std::endl;
      return 1;
   }
}

