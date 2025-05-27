#pragma warning(disable: 4101) // Visual Studio 
#pragma warning(disable: 4566) // Visual Studio 


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

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>


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
         bestShearIndex = i;
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
      det =matrix.Det();
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

int main(int argc, char* argv[]) {

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

      if ( controls.shouldShowDetails())
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

      // Process each mobile cell (all cells after the first one)
      for (size_t i = 1; i < inputList.size(); ++i) {
         const LatticeCell& mobileCell = inputList[i];

         // Add a separator between multiple transformations
         if (i >= 1) {
            std::cout << "\n\n=#==#==#==#==#==#==#==#==#==#==#==#==#==#==#===#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==\n";
            std::cout << "Transformation (Cell to Match to reference) " << i << " of " << (inputList.size() - 1) << "\n";
            std::cout << "=#==#==#==#==#==#==#==#==#==#==#==#==#==#==#===#==#==#==#==#==#==#==#==#==#==#==#==#==#==#==\n\n";
         }

         // Create display handler
         TransformerDisplay display(controls);
         // Show input cells for this comparison
         display.showInputCells(mobileCell, referenceCell);

         // If show extra transform info is enabled, show additional information
         if (controls.shouldShowExtraTransformInfo()) {
            // Use the utility function to show transformation info
            showTransformationInfo(mobileCell, referenceCell);
         }

         // Transform lattice with complete centering handling
         CompleteTransformationResult result = transformer.transformLattice(
            mobileCell, referenceCell);

         // Display the results
         display.showCompleteTransformations(result, mobileCell, referenceCell);
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

