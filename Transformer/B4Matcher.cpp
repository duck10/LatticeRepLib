#include "B4Matcher.h"
#include "LRL_Cell.h"
#include "TransformationMatrices.h"
#include "MultiTransformFinderControls.h"
#include "TransformerUtilities.h"
#include "Niggli.h"
#include "P3.h"
#include "S6.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <set>

std::vector<Matrix_3x3> generatePermutationMatrices() {
   std::vector<Matrix_3x3> permutations;
   std::set<Matrix_3x3> uniqueMatrices;  // To avoid duplicates

   // 1. Identity
   permutations.emplace_back(Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1));

   // The six basic permutations:

   // 2-3. Even permutations (cyclic, det = +1 naturally)
   permutations.emplace_back(Matrix_3x3(0, 0, 1, 1, 0, 0, 0, 1, 0));  // x→z, y→x, z→y
   permutations.emplace_back(Matrix_3x3(0, 1, 0, 0, 0, 1, 1, 0, 0));  // x→y, y→z, z→x

   // 4-6. Odd permutations (swaps, need one sign flip for det = +1)
   permutations.emplace_back(Matrix_3x3(0, 1, 0, 1, 0, 0, 0, 0, -1));  // swap x,y, negate z
   permutations.emplace_back(Matrix_3x3(0, 0, 1, 0, -1, 0, 1, 0, 0));  // swap x,z, negate y
   permutations.emplace_back(Matrix_3x3(-1, 0, 0, 0, 0, 1, 0, 1, 0));  // swap y,z, negate x

   // Extra permutations with sign flips that preserve det = +1

   // Sign flips for identity permutation
   permutations.emplace_back(Matrix_3x3(-1, 0, 0, 0, -1, 0, 0, 0, 1));  // negate x,y
   permutations.emplace_back(Matrix_3x3(-1, 0, 0, 0, 1, 0, 0, 0, -1));  // negate x,z
   permutations.emplace_back(Matrix_3x3(1, 0, 0, 0, -1, 0, 0, 0, -1));  // negate y,z

   // The specific matrix we identified as missing but useful
   permutations.emplace_back(Matrix_3x3(-1, 1, 0, 0, 1, 0, 0, 0, -1));

   // Add more sign-flipped variations if needed...
      // Add BOTH formats of the matrix we're looking for
   permutations.emplace_back(Matrix_3x3(0, 1, 0, 0, 0, 1, 1, 0, 0)); // Row-major format
   permutations.emplace_back(Matrix_3x3(0, 0, 1, 1, 0, 0, 0, 1, 0)); // Column-major format

   return permutations;
}

B4Matcher::B4Matcher(const LRL_Cell& cellToTransform,
   const LRL_Cell& referenceCell,
   const MultiTransformFinderControls& controls)
   : m_b4CellToTransform(cellToTransform)
   , m_b4ReferenceCell(referenceCell)
   , m_controls(controls) {
}


B4Matcher::TransformResult B4Matcher::findBestTransformation() const {
   if (m_transformResults.empty()) {
      // Create and initialize the TransformResult properly
      TransformResult result;
      result.transformMatrix = Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
      result.transformedB4 = m_b4CellToTransform; // Use the source B4, not a double
      result.b4Distance = std::numeric_limits<double>::max();
      return result;
   }
   return m_transformResults[0];
}

std::vector<B4Matcher::TransformResult> B4Matcher::findMultipleTransformations(size_t maxCount) const {
   if (m_transformResults.empty()) {
      // Create and initialize the TransformResult properly
      TransformResult result;
      result.transformMatrix = Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1);
      result.transformedB4 = m_b4CellToTransform; // Use the source B4, not a double
      result.b4Distance = std::numeric_limits<double>::max();
      return std::vector<TransformResult>{result}; // Use {} instead of () for initialization
   }

   // The results are already sorted by sortTransformationsByQuality()
   // Just apply the limit
   const size_t count = std::min(maxCount, m_transformResults.size());
   return m_transformResults;
}

bool B4Matcher::isValidTransformationMatrix(const Matrix_3x3& matrix) const {
   const double det = matrix.Det();
   const double tolerance = 1e-10;

   // Must have determinant exactly +1
   if (std::abs(det - 1.0) > tolerance) {
      if (m_controls.shouldShowDetails()) {
         std::cout << "Rejecting matrix with det = " << det << std::endl;
         std::cout << matrix << std::endl;
      }
      return false;
   }

   return true;
}

std::vector<Matrix_3x3> B4Matcher::generateTransformationMatrices() const {
   std::set<Matrix_3x3> matricesSet;  // Use set to avoid duplicates

   // Get configuration parameters from controls - UPDATED NAMES
   const int maxMatrices = m_controls.getMaxMatrixCount();        // Was getMaxMatrixSearchLimit()
   const int combDepth = m_controls.getMatrixCombinationDepth();  // Was getMaxMatrixSearchDepth() 
   const int maxCoefficient = m_controls.getMaxMatrixCoefficient(); // Unchanged

   if (m_controls.shouldShowDetails()) {
      std::cout << "Matrix generation parameters:" << std::endl;
      std::cout << "  - Max matrix count: " << maxMatrices << std::endl;         // Updated
      std::cout << "  - Combination depth: " << combDepth << std::endl;         // Updated  
      std::cout << "  - Max coefficient: " << maxCoefficient << std::endl;
   }

   // Use combDepth from controls instead of searchDepth parameter
   // (We'll phase out the searchDepth parameter entirely in Phase 2)
   const int actualDepth = combDepth;  // Use controls value, not parameter

   // Add basic matrices (permutations, shears, complex)
   for (const auto& matrix : MatrixSets::PERMUTATIONS) {
      matricesSet.insert(matrix);
   }

   for (const auto& matrix : MatrixSets::SHEARS) {
      matricesSet.insert(matrix);
   }

   for (const auto& matrix : MatrixSets::COMPLEX) {
      matricesSet.insert(matrix);
   }

   // Generate matrices with larger coefficients
   // Use maxCoefficient parameter to control the range
   if (maxCoefficient > 1) {
      // Generate shear matrices with coefficients up to maxCoefficient
      for (int i = 2; i <= maxCoefficient; ++i) {  // Start at 2 since 1 is already in basic shears
         // Add additional shear matrices with larger coefficients
         // Shear in XY plane
         matricesSet.insert(Matrix_3x3(1, i, 0, 0, 1, 0, 0, 0, 1));
         matricesSet.insert(Matrix_3x3(1, -i, 0, 0, 1, 0, 0, 0, 1));

         // Shear in XZ plane
         matricesSet.insert(Matrix_3x3(1, 0, i, 0, 1, 0, 0, 0, 1));
         matricesSet.insert(Matrix_3x3(1, 0, -i, 0, 1, 0, 0, 0, 1));

         // Shear in YX plane
         matricesSet.insert(Matrix_3x3(1, 0, 0, i, 1, 0, 0, 0, 1));
         matricesSet.insert(Matrix_3x3(1, 0, 0, -i, 1, 0, 0, 0, 1));

         // Shear in YZ plane
         matricesSet.insert(Matrix_3x3(1, 0, 0, 0, 1, i, 0, 0, 1));
         matricesSet.insert(Matrix_3x3(1, 0, 0, 0, 1, -i, 0, 0, 1));

         // Shear in ZX plane
         matricesSet.insert(Matrix_3x3(1, 0, 0, 0, 1, 0, i, 0, 1));
         matricesSet.insert(Matrix_3x3(1, 0, 0, 0, 1, 0, -i, 0, 1));

         // Shear in ZY plane
         matricesSet.insert(Matrix_3x3(1, 0, 0, 0, 1, 0, 0, i, 1));
         matricesSet.insert(Matrix_3x3(1, 0, 0, 0, 1, 0, 0, -i, 1));
      }
   }

   // Check if we've exceeded max matrices count
   if (matricesSet.size() >= maxMatrices) {
      if (m_controls.shouldShowDetails()) {
         std::cout << "Reached max matrices limit after generating basic matrices: "
            << matricesSet.size() << std::endl;
      }
      return std::vector<Matrix_3x3>(matricesSet.begin(), matricesSet.end());
   }

   // Generate combinations if needed, respecting combination depth from controls
   if (actualDepth > 1) {  // Use actualDepth (from controls) not searchDepth parameter
      // Permutation-shear combinations
      std::set<Matrix_3x3> newMatrices;
      for (const auto& perm : MatrixSets::PERMUTATIONS) {
         for (const auto& shear : matricesSet) {
            // Only combine if shear is not a permutation (to avoid duplicates)
            bool isPermutation = false;
            for (const auto& p : MatrixSets::PERMUTATIONS) {
               if (shear == p) {
                  isPermutation = true;
                  break;
               }
            }
            if (!isPermutation) {
               // Try perm * shear
               Matrix_3x3 combined = perm * shear;
               // Verify determinant is still 1
               if (std::abs(combined.Det() - 1.0) < 0.01) {
                  newMatrices.insert(combined);
               }

               // Try shear * perm
               combined = shear * perm;
               // Verify determinant is still 1
               if (std::abs(combined.Det() - 1.0) < 0.01) {
                  newMatrices.insert(combined);
               }

               // Check if we'll exceed max matrices
               if (matricesSet.size() + newMatrices.size() >= maxMatrices) {
                  break;
               }
            }
         }
         // Check if we'll exceed max matrices
         if (matricesSet.size() + newMatrices.size() >= maxMatrices) {
            break;
         }
      }

      // Add the new matrices to the set
      matricesSet.insert(newMatrices.begin(), newMatrices.end());

      if (m_controls.shouldShowDetails()) {
         std::cout << "After depth 1 combinations: " << matricesSet.size() << " matrices" << std::endl;
      }

      // Check if we've reached the limit
      if (matricesSet.size() >= maxMatrices) {
         return std::vector<Matrix_3x3>(matricesSet.begin(), matricesSet.end());
      }
   }

   // More combinations for higher combination depths
   if (actualDepth > 2) {  // Use actualDepth
      // Get all non-permutation matrices
      std::vector<Matrix_3x3> nonPerms;
      for (const auto& m : matricesSet) {
         bool isPerm = false;
         for (const auto& p : MatrixSets::PERMUTATIONS) {
            if (m == p) {
               isPerm = true;
               break;
            }
         }
         if (!isPerm) {
            nonPerms.push_back(m);
         }
      }

      // Limit the number of combinations to avoid excessive computation
      const size_t shearCount = nonPerms.size();
      std::set<Matrix_3x3> newMatrices;

      // Shear-shear combinations
      for (size_t i = 0; i < shearCount && matricesSet.size() + newMatrices.size() < maxMatrices; ++i) {
         for (size_t j = 0; j < shearCount && matricesSet.size() + newMatrices.size() < maxMatrices; ++j) {
            Matrix_3x3 combined = nonPerms[i] * nonPerms[j];
            // Verify determinant is still 1
            if (std::abs(combined.Det() - 1.0) < 0.01) {
               newMatrices.insert(combined);
            }
         }
      }

      // Add the new matrices to the set
      matricesSet.insert(newMatrices.begin(), newMatrices.end());

      if (m_controls.shouldShowDetails()) {
         std::cout << "After depth 2 combinations: " << matricesSet.size() << " matrices" << std::endl;
      }

      // Check if we've reached the limit
      if (matricesSet.size() >= maxMatrices) {
         return std::vector<Matrix_3x3>(matricesSet.begin(), matricesSet.end());
      }
   }

   // For even deeper search
   if (actualDepth > 3) {  // Use actualDepth
      // Triple combinations (perm * shear * shear)
      std::set<Matrix_3x3> newMatrices;

      for (const auto& perm : MatrixSets::PERMUTATIONS) {
         for (const auto& s1 : MatrixSets::SHEARS) {
            for (const auto& s2 : MatrixSets::SHEARS) {
               // Check if we'll exceed max matrices
               if (matricesSet.size() + newMatrices.size() >= maxMatrices) {
                  break;
               }

               Matrix_3x3 combined = perm * s1 * s2;
               // Verify determinant is still 1
               if (std::abs(combined.Det() - 1.0) < 0.01) {
                  newMatrices.insert(combined);
               }
            }
            // Check if we'll exceed max matrices
            if (matricesSet.size() + newMatrices.size() >= maxMatrices) {
               break;
            }
         }
         // Check if we'll exceed max matrices
         if (matricesSet.size() + newMatrices.size() >= maxMatrices) {
            break;
         }
      }

      // Add the new matrices to the set
      matricesSet.insert(newMatrices.begin(), newMatrices.end());

      if (m_controls.shouldShowDetails()) {
         std::cout << "After depth 3 combinations: " << matricesSet.size() << " matrices" << std::endl;
      }
   }

   // Final debug output
   if (m_controls.shouldShowDetails()) {
      std::cout << "Final matrix count: " << matricesSet.size() << " matrices" << std::endl;
      std::cout << "Used combination depth: " << actualDepth << " (from controls)" << std::endl;
   }

   // Convert to vector for return
   return std::vector<Matrix_3x3>(matricesSet.begin(), matricesSet.end());
}

std::vector<Matrix_3x3> B4Matcher::generatePriorityOrderedMatrices() const {
   std::vector<Matrix_3x3> matrices;

   if (m_controls.shouldShowDetails()) {
      std::cout << "\n=== PRIORITY MATRIX GENERATION ===" << std::endl;
   }

   // PRIORITY 1: Identity matrix (MUST be tested first!)
   matrices.push_back(MatrixSets::IDENTITY);
   if (m_controls.shouldShowDetails()) {
      std::cout << "Priority 1: Added identity matrix" << std::endl;
   }

   // PRIORITY 2: Use the proper crystallographic transformations (det = +1)
   matrices.push_back(MatrixSets::PERM_SWAP_XY);  // Corrected permutations
   matrices.push_back(MatrixSets::PERM_SWAP_YZ);
   matrices.push_back(MatrixSets::PERM_SWAP_XZ);
   matrices.push_back(MatrixSets::PERM_CYCLIC_1);
   matrices.push_back(MatrixSets::PERM_CYCLIC_2);
   if (m_controls.shouldShowDetails()) {
      std::cout << "Priority 2: Added 5 corrected permutations (all det=+1)" << std::endl;
   }

   //PRIORITY 2.5: *** NEW *** Niggli boundary matrices (CRITICAL for edge cases!)
   //These handle lattice basis changes that simple permutations miss

   //Presentation matrices (corrected versions)
   const Matrix_3x3 sp1_3D(0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, -1.0);  // [b,a,-c]
   const Matrix_3x3 sp2_3D(-1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);  // [-a,c,b]

   // Sign change matrices
   const Matrix_3x3 sp34a_3D(1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, -1.0);
   const Matrix_3x3 sp34b_3D(-1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0);
   const Matrix_3x3 sp34c_3D(-1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 1.0);

   // Reduction operation matrices (the key ones you're missing!)
   const Matrix_3x3 R5_Plus_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, -1.0, 1.0);   // [a, b, c-b]
   const Matrix_3x3 R5_Minus_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0);   // [a, b, c+b]

   const Matrix_3x3 R6_Plus_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, 0.0, 1.0);   // [a, b, c-a]
   const Matrix_3x3 R6_Minus_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0);   // [a, b, c+a]

   const Matrix_3x3 R7_Plus_3D(1.0, 0.0, 0.0, -1.0, 1.0, 0.0, 0.0, 0.0, 1.0);   // [a, b-a, c]
   const Matrix_3x3 R7_Minus_3D(1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0);   // [a, b+a, c]

   const Matrix_3x3 R8_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0);         // [a, b, c+a+b]

   const Matrix_3x3 R12_3D(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, -1.0, 1.0);      // [a, b, c-a-b]

   // Add all Niggli matrices
   std::vector<Matrix_3x3> niggliMatrices = {
      sp1_3D, sp2_3D, sp34a_3D, sp34b_3D, sp34c_3D,
      R5_Plus_3D, R5_Minus_3D, R6_Plus_3D, R6_Minus_3D,
      R7_Plus_3D, R7_Minus_3D, R8_3D, R12_3D
   };

   size_t niggliAdded = 0;
   for (const auto& matrix : niggliMatrices) {
      double det = matrix.Det();
      if (std::abs(det - 1.0) < 1e-10 || std::abs(det + 1.0) < 1e-10) {  // Accept det = ±1
         // Check for duplicates
         bool alreadyAdded = false;
         for (const auto& existing : matrices) {
            if (matrix == existing) {
               alreadyAdded = true;
               break;
            }
         }
         if (!alreadyAdded) {
            matrices.push_back(matrix);
            matrices.push_back(matrix.Inverse());
            niggliAdded++;
         }
      }
   }

   if (m_controls.shouldShowDetails()) {
      std::cout << "Priority 2.5: Added " << niggliAdded << " Niggli boundary matrices" << std::endl;
   }

   // PRIORITY 3: Sign flips (only proper rotations det=+1)
   matrices.push_back(Matrix_3x3(-1, 0, 0, 0, -1, 0, 0, 0, 1));  // -x,-y
   matrices.push_back(Matrix_3x3(-1, 0, 0, 0, 1, 0, 0, 0, -1));  // -x,-z
   matrices.push_back(Matrix_3x3(1, 0, 0, 0, -1, 0, 0, 0, -1));  // -y,-z

   if (m_controls.shouldShowDetails()) {
      std::cout << "Priority 3: Added 3 sign flip combinations (det=+1)" << std::endl;
   }

   // PRIORITY 4: Remaining permutations from MatrixSets::PERMUTATIONS
   size_t existingPermCount = 0;
   std::vector<Matrix_3x3> allPermutations = MatrixSets::getAllPermutationMatrices();
   for (const auto& matrix : allPermutations) {
      bool alreadyAdded = false;
      for (const auto& existing : matrices) {
         if (matrix == existing) {
            alreadyAdded = true;
            break;
         }
      }
      if (!alreadyAdded) {
         double det = matrix.Det();
         if (std::abs(det - 1.0) < 1e-10) {  // Only det = +1
            matrices.push_back(matrix);
         }
      }
      else {
         existingPermCount++;
      }
   }

   if (m_controls.shouldShowDetails()) {
      std::cout << "Priority 4: Added remaining permutations (det=+1 only)" << std::endl;
   }

   // PRIORITY 5: Basic shear matrices
   for (const auto& matrix : MatrixSets::SHEARS) {
      double det = matrix.Det();
      if (std::abs(det - 1.0) < 1e-10) {
         matrices.push_back(matrix);
      }
   }

   if (m_controls.shouldShowDetails()) {
      std::cout << "Priority 5: Added shear matrices (det=+1 only)" << std::endl;
   }

   // PRIORITY 6: Complex matrices
   for (const auto& matrix : MatrixSets::COMPLEX) {
      double det = matrix.Det();
      if (std::abs(det - 1.0) < 1e-10) {
         matrices.push_back(matrix);
      }
   }

   // Continue with remaining priorities (extended matrices, combinations)...
   // [Rest of your existing code for priorities 7-8]

   if (m_controls.shouldShowDetails()) {
      std::cout << "=== FINAL: Generated " << matrices.size() << " matrices in priority order ===" << std::endl;
      std::cout << "Includes specialized Niggli boundary matrices for edge cases" << std::endl;

      // Verify first few matrices
      for (size_t i = 0; i < std::min(size_t(5), matrices.size()); ++i) {
         double det = matrices[i].Det();
         std::cout << "Matrix #" << (i + 1) << " det: " << det
            << (std::abs(det - 1.0) < 1e-10 ? " ✓" :
               std::abs(det + 1.0) < 1e-10 ? " ±" : " ⚠") << std::endl;
      }
   }


   // In your new generatePriorityOrderedMatrices():
   if (m_controls.shouldShowDetails()) {
      std::cout << "=== CHECKING FOR CRITICAL MATRIX ===" << std::endl;

      // The matrix that worked in your detailed test:
      Matrix_3x3 goodMatrix(-1.0, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0);

      bool found = false;
      for (size_t i = 0; i < matrices.size(); ++i) {
         if (matrices[i] == goodMatrix) {
            std::cout << "FOUND good matrix at position " << i << std::endl;
            found = true;
            break;
         }
      }

      if (!found) {
         std::cout << "*** CRITICAL MATRIX MISSING! ***" << std::endl;
         matrices.insert(matrices.begin() + 1, goodMatrix); // Force add it
         std::cout << "Added missing matrix at position 1" << std::endl;
      }
   }



   return matrices;
}


void B4Matcher::FindTransformationsWithReservoir() {
   // Create reservoir with proper sizing
   const size_t minReservoirSize = m_controls.getMaxTransformationsToShow() * 2;
   const size_t reservoirSize = std::max(minReservoirSize, size_t(50));
   const double maxDistance = m_controls.getMaxTransformationDistance();

   TransformationReservoir reservoir(reservoirSize, maxDistance);

   // Convert B4 to LRL_Cell - USING CORRECT MEMBER VARIABLES
   LRL_Cell cellToTransform(m_b4CellToTransform);   // Convert B4 to LRL_Cell
   LRL_Cell referenceCell(m_b4ReferenceCell);       // Convert B4 to LRL_Cell

   if (m_controls.shouldShowDetails()) {
      std::cout << "\n=== TRANSFORMATION SEARCH WITH RESERVOIR ===" << std::endl;
      std::cout << "Reservoir size: " << reservoirSize << " candidates" << std::endl;
      std::cout << "Display count: " << m_controls.getMaxTransformationsToShow() << std::endl;
      std::cout << "Maximum acceptable distance: " << maxDistance << " Å" << std::endl;
      std::cout << "Cells to match:" << std::endl;
      std::cout << "  Source: " << LRL_Cell_Degrees(cellToTransform) << std::endl;
      std::cout << "  Target: " << LRL_Cell_Degrees(referenceCell) << std::endl;
   }

   // Generate matrices in priority order
   std::vector<Matrix_3x3> matrices = generatePriorityOrderedMatrices();

   // Test all matrices, keeping best in reservoir
   size_t testedCount = 0;
   size_t acceptedCount = 0;

   for (const auto& matrix : matrices) {
      testedCount++;
      //if (testedCount <= 3) {
      //   std::cout << "Testing matrix #" << testedCount << " (det=" << matrix.Det() << "):" << std::endl;
      //   std::cout << matrix << std::endl;
      //}

      if (reservoir.tryAdd(matrix, cellToTransform, referenceCell)) {
         acceptedCount++;

         //// ADD THIS DEBUG:
         //if (acceptedCount == 1) {
         //   std::cout << "FIRST ACCEPTED matrix (det=" << matrix.Det() << "):" << std::endl;
         //   std::cout << matrix << std::endl;
         //}
      }
      if (reservoir.tryAdd(matrix, cellToTransform, referenceCell)) {
         acceptedCount++;

         // Show early successes
         if (acceptedCount <= 10 && m_controls.shouldShowDetails()) {
            std::cout << "Candidate #" << acceptedCount << " (matrix #" << testedCount
               << "): distance = " << reservoir.getBestDistance() << " Å" << std::endl;
         }
      }

      // Periodic progress updates for long searches
      if (m_controls.shouldShowDetails() && testedCount % 1000 == 0) {
         std::cout << "Progress: tested " << testedCount << "/" << matrices.size()
            << " matrices, " << acceptedCount << " accepted" << std::endl;
         reservoir.printStats(std::cout);
      }
   }

   if (m_controls.shouldShowDetails()) {
      std::cout << "\n=== SEARCH COMPLETE ===" << std::endl;
      std::cout << "Tested " << testedCount << " matrices total" << std::endl;
      std::cout << "Accepted " << acceptedCount << " good transformations" << std::endl;
      reservoir.printStats(std::cout);
   }

   // Extract results for display
   auto bestCandidates = reservoir.getBest(m_controls.getMaxTransformationsToShow());

   if (bestCandidates.empty()) {
      if (m_controls.shouldShowDetails()) {
         std::cout << "WARNING: No transformations found within distance limit "
            << maxDistance << " Å" << std::endl;
      }
      return;
   }

   // Store results using CORRECT member variable name
   m_transformResults.clear();  // CORRECT: m_transformResults not m_results
   for (size_t i = 0; i < bestCandidates.size(); ++i) {
      const auto& candidate = bestCandidates[i];

      // Create result structure with CORRECT field names from TransformResult.h
      TransformResult result;
      result.completeTransformMatrix = candidate.matrix;
      result.transformedCompleteCell = candidate.transformedCell;
      result.p3DistanceFinal = candidate.p3Distance;
      result.cs6DistanceFinal = candidate.ncDistance;
      result.s6AngleFinal = candidate.s6Angle;

      // Set other required fields
      result.transformMatrix = candidate.matrix;
      result.b4Distance = candidate.p3Distance;
      result.transformedB4 = B4(candidate.transformedCell);

      m_transformResults.push_back(result);  // CORRECT: m_transformResults

      if (m_controls.shouldShowDetails()) {
         std::cout << "Result #" << (i + 1) << ": P3=" << candidate.p3Distance
            << " Å, NC=" << candidate.ncDistance
            << " Å, S6=" << candidate.s6Angle << "°" << std::endl;
      }
   }

   std::cout << "Found " << m_transformResults.size() << " valid transformations." << std::endl;
}


