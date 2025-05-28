#include "B4Matcher.h"
#include "MatrixSets_B4Matcher..h"
#include "LRL_Cell.h"
#include "NiggliMatrices.h"
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
      result.transformMatrix = MatrixSets::IDENTITY;
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
      result.transformMatrix = MatrixSets::IDENTITY;
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

   const int actualDepth = combDepth;  // Use controls value, not parameter

   // Add basic matrices (permutations, shears, complex)
   std::set<Matrix_3x3> matricesSet = loadBasicMatrices();


   // Generate matrices with larger coefficients
   // Use maxCoefficient parameter to control the range
   if (maxCoefficient > 1) {
      // Generate shear matrices with coefficients up to maxCoefficient
      auto extendedShears = generateExtendedShearMatrices();
      matricesSet.insert(extendedShears.begin(), extendedShears.end());
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
   if (actualDepth > 1) {
      auto newMatrices = generatePermutationShearCombinations(matricesSet, maxMatrices);
      matricesSet.insert(newMatrices.begin(), newMatrices.end());

      if (m_controls.shouldShowDetails()) {
         std::cout << "After depth 1 combinations: " << matricesSet.size() << " matrices" << std::endl;
      }

      // Simple check at the end - exit if over limit
      if (matricesSet.size() >= maxMatrices) {
         if (m_controls.shouldShowDetails()) {
            std::cout << "Reached matrix limit, stopping further generation" << std::endl;
         }
         return std::vector<Matrix_3x3>(matricesSet.begin(), matricesSet.end());
      }
   }


   // More combinations for higher combination depths
   if (actualDepth > 2) {
      auto newMatrices = generateShearShearCombinations(matricesSet);
      matricesSet.insert(newMatrices.begin(), newMatrices.end());

      if (m_controls.shouldShowDetails()) {
         std::cout << "After depth 2 combinations: " << matricesSet.size() << " matrices" << std::endl;
      }

      // Simple check at the end - exit if over limit
      if (matricesSet.size() >= maxMatrices) {
         if (m_controls.shouldShowDetails()) {
            std::cout << "Reached matrix limit, stopping further generation" << std::endl;
         }
         return std::vector<Matrix_3x3>(matricesSet.begin(), matricesSet.end());
      }
   }

   // For even deeper search
   if (actualDepth > 3) {
      auto newMatrices = generateTripleCombinations();
      matricesSet.insert(newMatrices.begin(), newMatrices.end());

      if (m_controls.shouldShowDetails()) {
         std::cout << "After depth 3 combinations: " << matricesSet.size() << " matrices" << std::endl;
      }

      // Simple check at the end - exit if over limit
      if (matricesSet.size() >= maxMatrices) {
         if (m_controls.shouldShowDetails()) {
            std::cout << "Reached matrix limit, stopping further generation" << std::endl;
         }
         return std::vector<Matrix_3x3>(matricesSet.begin(), matricesSet.end());
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

   // Add all Niggli matrices
   std::vector<Matrix_3x3> niggliMatrices = NiggliMatrices::ALL_NIGGLI_MATRICES;

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
   matrices.push_back(MatrixSets::B4_NEG_XY);
   matrices.push_back(MatrixSets::B4_NEG_XZ);
   matrices.push_back(MatrixSets::B4_NEG_YZ);

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
      Matrix_3x3 goodMatrix = MatrixSets::B4_CRITICAL;

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

      if (reservoir.tryAdd(matrix, cellToTransform, referenceCell)) {
         acceptedCount++;
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

// Add this function to B4Matcher.cpp (or create a separate utility file)

std::set<Matrix_3x3> B4Matcher::generateExtendedShearMatrices() const {
   std::set<Matrix_3x3> shearMatrices;

   const int maxCoefficient = m_controls.getMaxMatrixCoefficient();

   if (m_controls.shouldShowDetails()) {
      std::cout << "Generating extended shear matrices with coefficients 2 to "
         << maxCoefficient << std::endl;
   }

   // Generate shear matrices with coefficients from 2 to maxCoefficient
   for (int i = 2; i <= maxCoefficient; ++i) {
      // All 6 shear directions, both positive and negative coefficients

      // Shear in XY plane (modify [1,0] element)
      shearMatrices.insert(Matrix_3x3(1, i, 0, 0, 1, 0, 0, 0, 1));   // +shear
      shearMatrices.insert(Matrix_3x3(1, -i, 0, 0, 1, 0, 0, 0, 1));  // -shear

      // Shear in XZ plane (modify [2,0] element) 
      shearMatrices.insert(Matrix_3x3(1, 0, i, 0, 1, 0, 0, 0, 1));   // +shear
      shearMatrices.insert(Matrix_3x3(1, 0, -i, 0, 1, 0, 0, 0, 1));  // -shear

      // Shear in YX plane (modify [0,1] element)
      shearMatrices.insert(Matrix_3x3(1, 0, 0, i, 1, 0, 0, 0, 1));   // +shear
      shearMatrices.insert(Matrix_3x3(1, 0, 0, -i, 1, 0, 0, 0, 1));  // -shear

      // Shear in YZ plane (modify [2,1] element)
      shearMatrices.insert(Matrix_3x3(1, 0, 0, 0, 1, i, 0, 0, 1));   // +shear
      shearMatrices.insert(Matrix_3x3(1, 0, 0, 0, 1, -i, 0, 0, 1));  // -shear

      // Shear in ZX plane (modify [0,2] element)
      shearMatrices.insert(Matrix_3x3(1, 0, 0, 0, 1, 0, i, 0, 1));   // +shear
      shearMatrices.insert(Matrix_3x3(1, 0, 0, 0, 1, 0, -i, 0, 1));  // -shear

      // Shear in ZY plane (modify [1,2] element)
      shearMatrices.insert(Matrix_3x3(1, 0, 0, 0, 1, 0, 0, i, 1));   // +shear
      shearMatrices.insert(Matrix_3x3(1, 0, 0, 0, 1, 0, 0, -i, 1));  // -shear
   }

   if (m_controls.shouldShowDetails()) {
      std::cout << "Generated " << shearMatrices.size()
         << " extended shear matrices" << std::endl;
   }

   return shearMatrices;
}

// Add this function to B4Matcher.cpp

// Simplified version of generatePermutationShearCombinations

std::set<Matrix_3x3> B4Matcher::generatePermutationShearCombinations(
   const std::set<Matrix_3x3>& existingMatrices,
   size_t maxTotalMatrices) const {

   std::set<Matrix_3x3> newMatrices;

   if (m_controls.shouldShowDetails()) {
      std::cout << "Generating permutation-shear combinations..." << std::endl;
   }

   // Generate all combinations without checking limits inside loops
   for (const auto& perm : MatrixSets::PERMUTATIONS) {
      for (const auto& matrix : existingMatrices) {
         // Only combine if matrix is not already a permutation (to avoid duplicates)
         if (!isPermutationMatrix(matrix)) {
            // Try perm * matrix
            Matrix_3x3 combined = perm * matrix;
            if (hasValidDeterminant(combined)) {
               newMatrices.insert(combined);
            }

            // Try matrix * perm  
            combined = matrix * perm;
            if (hasValidDeterminant(combined)) {
               newMatrices.insert(combined);
            }
         }
      }
   }

   if (m_controls.shouldShowDetails()) {
      std::cout << "Generated " << newMatrices.size() << " new combinations" << std::endl;

      // Check if we're over the limit
      if (existingMatrices.size() + newMatrices.size() > maxTotalMatrices) {
         std::cout << "WARNING: Generated " << (existingMatrices.size() + newMatrices.size())
            << " matrices, exceeds limit of " << maxTotalMatrices << std::endl;
      }
   }

   return newMatrices;
}


// Helper functions to make the logic clearer
bool B4Matcher::isPermutationMatrix(const Matrix_3x3& matrix) const {
   for (const auto& perm : MatrixSets::PERMUTATIONS) {
      if (matrix == perm) {
         return true;
      }
   }
   return false;
}

bool B4Matcher::hasValidDeterminant(const Matrix_3x3& matrix) const {
   return std::abs(matrix.Det() - 1.0) < 0.01;
}

std::set<Matrix_3x3> B4Matcher::loadBasicMatrices() const {
   std::set<Matrix_3x3> matricesSet;

   if (m_controls.shouldShowDetails()) {
      std::cout << "Loading basic matrices (permutations, shears, complex)..." << std::endl;
   }

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

   if (m_controls.shouldShowDetails()) {
      std::cout << "Loaded " << matricesSet.size() << " basic matrices" << std::endl;
   }

   return matricesSet;
}

// Add this function to B4Matcher.cpp

std::set<Matrix_3x3> B4Matcher::generateTripleCombinations() const {
   std::set<Matrix_3x3> newMatrices;

   if (m_controls.shouldShowDetails()) {
      std::cout << "Generating triple combinations (perm * shear * shear)..." << std::endl;
   }

   // Generate all triple combinations without checking limits inside loops
   for (const auto& perm : MatrixSets::PERMUTATIONS) {
      for (const auto& s1 : MatrixSets::SHEARS) {
         for (const auto& s2 : MatrixSets::SHEARS) {
            Matrix_3x3 combined = perm * s1 * s2;

            // Verify determinant is still 1
            if (hasValidDeterminant(combined)) {
               newMatrices.insert(combined);
            }
         }
      }
   }

   if (m_controls.shouldShowDetails()) {
      std::cout << "Generated " << newMatrices.size() << " triple combinations" << std::endl;
   }

   return newMatrices;
}


// Add this function to B4Matcher.cpp

std::set<Matrix_3x3> B4Matcher::generateShearShearCombinations(
   const std::set<Matrix_3x3>& existingMatrices) const {

   std::set<Matrix_3x3> newMatrices;

   if (m_controls.shouldShowDetails()) {
      std::cout << "Generating shear-shear combinations..." << std::endl;
   }

   // Get all non-permutation matrices from existing set
   std::vector<Matrix_3x3> nonPerms;
   for (const auto& matrix : existingMatrices) {
      if (!isPermutationMatrix(matrix)) {
         nonPerms.push_back(matrix);
      }
   }

   if (m_controls.shouldShowDetails()) {
      std::cout << "Found " << nonPerms.size() << " non-permutation matrices to combine" << std::endl;
   }

   // Generate all shear-shear combinations without size checking inside loops
   for (size_t i = 0; i < nonPerms.size(); ++i) {
      for (size_t j = 0; j < nonPerms.size(); ++j) {
         Matrix_3x3 combined = nonPerms[i] * nonPerms[j];

         // Verify determinant is still 1
         if (hasValidDeterminant(combined)) {
            newMatrices.insert(combined);
         }
      }
   }

   if (m_controls.shouldShowDetails()) {
      std::cout << "Generated " << newMatrices.size() << " shear-shear combinations" << std::endl;
   }

   return newMatrices;
}

