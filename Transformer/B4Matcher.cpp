// B4Matcher.cpp
#include "B4Matcher.h"

// Add this function to your source file (same file as your other matrix generators)
std::vector<Matrix_3x3> generatePermutationMatrices() {
   std::vector<Matrix_3x3> permutations;

   // Identity
   permutations.push_back(Matrix_3x3(
      1, 0, 0,
      0, 1, 0,
      0, 0, 1
   ));

   // Cyclic permutations
   permutations.push_back(Matrix_3x3(
      0, 0, 1,
      1, 0, 0,
      0, 1, 0
   ));

   permutations.push_back(Matrix_3x3(
      0, 1, 0,
      0, 0, 1,
      1, 0, 0
   ));

   // Permutations with sign flips are already covered in other generators

   return permutations;
}

std::string B4Matcher::TransformResult::interpretMatrix(const Matrix_3x3& matrix) {
   // Common transformations
   if (B4Matcher::isApproximately(matrix[0], 1.0) &&
      B4Matcher::isApproximately(matrix[4], 1.0) &&
      B4Matcher::isApproximately(matrix[8], 1.0) &&
      B4Matcher::isApproximately(matrix[1], 0.0) &&
      B4Matcher::isApproximately(matrix[2], 0.0) &&
      B4Matcher::isApproximately(matrix[3], 0.0) &&
      B4Matcher::isApproximately(matrix[5], 0.0) &&
      B4Matcher::isApproximately(matrix[6], 0.0) &&
      B4Matcher::isApproximately(matrix[7], 0.0)) {
      return "Identity (a,b,c) -> (a,b,c)";
   }

   if (B4Matcher::isApproximately(matrix[2], 1.0) &&
      B4Matcher::isApproximately(matrix[3], 1.0) &&
      B4Matcher::isApproximately(matrix[7], 1.0) &&
      B4Matcher::isApproximately(matrix[0], 0.0) &&
      B4Matcher::isApproximately(matrix[1], 0.0) &&
      B4Matcher::isApproximately(matrix[4], 0.0) &&
      B4Matcher::isApproximately(matrix[5], 0.0) &&
      B4Matcher::isApproximately(matrix[6], 0.0) &&
      B4Matcher::isApproximately(matrix[8], 0.0)) {
      return "Cyclic Permutation (a,b,c) -> (c,a,b)";
   }

   // Default: Build description from matrix
   std::ostringstream oss;
   bool firstA = true, firstB = true, firstC = true;

   oss << "a' = ";
   if (matrix[0] != 0) {
      oss << (firstA ? "" : (matrix[0] > 0 ? "+ " : "")) << matrix[0] << "*a ";
      firstA = false;
   }
   if (matrix[1] != 0) {
      oss << (firstA ? "" : (matrix[1] > 0 ? "+ " : "")) << matrix[1] << "*b ";
      firstA = false;
   }
   if (matrix[2] != 0) {
      oss << (firstA ? "" : (matrix[2] > 0 ? "+ " : "")) << matrix[2] << "*c";
      firstA = false;
   }

   oss << std::endl << "b' = ";
   if (matrix[3] != 0) {
      oss << (firstB ? "" : (matrix[3] > 0 ? "+ " : "")) << matrix[3] << "*a ";
      firstB = false;
   }
   if (matrix[4] != 0) {
      oss << (firstB ? "" : (matrix[4] > 0 ? "+ " : "")) << matrix[4] << "*b ";
      firstB = false;
   }
   if (matrix[5] != 0) {
      oss << (firstB ? "" : (matrix[5] > 0 ? "+ " : "")) << matrix[5] << "*c";
      firstB = false;
   }

   oss << std::endl << "c' = ";
   if (matrix[6] != 0) {
      oss << (firstC ? "" : (matrix[6] > 0 ? "+ " : "")) << matrix[6] << "*a ";
      firstC = false;
   }
   if (matrix[7] != 0) {
      oss << (firstC ? "" : (matrix[7] > 0 ? "+ " : "")) << matrix[7] << "*b ";
      firstC = false;
   }
   if (matrix[8] != 0) {
      oss << (firstC ? "" : (matrix[8] > 0 ? "+ " : "")) << matrix[8] << "*c";
      firstC = false;
   }

   return oss.str();
}

bool B4Matcher::isPermutationMatrix(const Matrix_3x3& matrix) {
   // Check if each row has exactly one non-zero element
   for (int row = 0; row < 3; ++row) {
      int nonZeroCount = 0;
      for (int col = 0; col < 3; ++col) {
         if (std::abs(matrix[row * 3 + col]) > 0.9) {
            nonZeroCount++;
         }
      }
      if (nonZeroCount != 1) return false;
   }

   // Check if each column has exactly one non-zero element
   for (int col = 0; col < 3; ++col) {
      int nonZeroCount = 0;
      for (int row = 0; row < 3; ++row) {
         if (std::abs(matrix[row * 3 + col]) > 0.9) {
            nonZeroCount++;
         }
      }
      if (nonZeroCount != 1) return false;
   }

   return true;
}

void B4Matcher::analyzeAxisPermutation(
   const Matrix_3x3& matrix,
   const LRL_Cell_Degrees& sourceDeg,
   const LRL_Cell_Degrees& targetDeg) {

   // Extract transformation pattern from matrix
   int mapping[3] = { -1, -1, -1 };

   // For each row, find which column has a non-zero value
   for (int row = 0; row < 3; ++row) {
      for (int col = 0; col < 3; ++col) {
         if (std::abs(matrix[row * 3 + col]) > 0.9) {
            mapping[row] = col;
         }
      }
   }

   // Display the axes mapping
   std::cout << "      Axes mapping based on matrix: ";
   char axes[3] = { 'a', 'b', 'c' };
   for (int i = 0; i < 3; ++i) {
      if (mapping[i] >= 0) {
         std::cout << axes[mapping[i]] << " -> " << axes[i] << "  ";
      }
   }
   std::cout << std::endl;

   // Extract source and target values
   double sourceValues[3] = { sourceDeg[0], sourceDeg[1], sourceDeg[2] };
   double targetValues[3] = { targetDeg[0], targetDeg[1], targetDeg[2] };

   // Check if this permutation would map source values to target values
   double mappedValues[3] = { 0, 0, 0 };
   for (int i = 0; i < 3; ++i) {
      if (mapping[i] >= 0) {
         mappedValues[i] = sourceValues[mapping[i]];
      }
   }

   // Show how source values would map to target if this transformation were applied
   std::cout << "      If this matrix were correctly applied:" << std::endl;
   std::cout << "      Source: " << sourceValues[0] << ", " << sourceValues[1] << ", " << sourceValues[2] << std::endl;
   std::cout << "      Would map to: " << mappedValues[0] << ", " << mappedValues[1] << ", " << mappedValues[2] << std::endl;
   std::cout << "      Target: " << targetValues[0] << ", " << targetValues[1] << ", " << targetValues[2] << std::endl;

   // Check if the mapping would match the target
   bool isGoodMatch = true;
   for (int i = 0; i < 3; ++i) {
      if (std::abs(mappedValues[i] - targetValues[i]) > 0.5) {
         isGoodMatch = false;
         break;
      }
   }

   if (isGoodMatch) {
      std::cout << "      This transformation would correctly map source to target parameters!" << std::endl;
   }
   else {
      std::cout << "      This transformation would NOT correctly map source to target parameters." << std::endl;
   }
}

B4Matcher::TransformResult B4Matcher::findBestTransformation(int complexity) const {

   // Use the advanced matrix generation instead of basic permutations
   // The default complexity of 2 should be sufficient for most cases
   std::vector<Matrix_3x3> matrices = generateAdvancedTransformationMatrices(complexity);
   std::vector<TransformResult> results;

   for (const auto& matrix : matrices) {
      // Apply transformation directly to B4
      B4 transformedB4 = matrix * m_sourceB4;

      // Calculate B4 distance
      double distance = B4::DistanceBetween(transformedB4, m_targetB4);

      results.emplace_back(matrix, transformedB4, distance);
   }

   // Sort by B4 distance
   std::sort(results.begin(), results.end());

   // Return the best match
   if (results.empty()) {
      // In case no valid transformation was found (shouldn't happen)
      return TransformResult();
   }
   return results.front();
}

std::vector<B4Matcher::TransformResult> B4Matcher::findMultipleTransformations(
   const size_t maxResults, int complexity) const {

   std::vector<Matrix_3x3> matrices = generateAdvancedTransformationMatrices(complexity);
   std::vector<TransformResult> results;

   for (const auto& matrix : matrices) {
      // Apply transformation directly to B4
      B4 transformedB4 = matrix * m_sourceB4;

      // Calculate B4 distance
      double distance = B4::DistanceBetween(transformedB4, m_targetB4);

      results.emplace_back(matrix, transformedB4, distance);
   }

   // Sort by B4 distance
   std::sort(results.begin(), results.end());

   // Limit results
   if (results.size() > maxResults) {
      results.resize(maxResults);
   }

   return results;
}

std::vector<B4Matcher::TransformResult> B4Matcher::findMultipleHybridTransformations(
   const size_t maxResults, int complexity) const {

   // Use a set of matrices to automatically eliminate duplicates
   std::set<Matrix_3x3> matrixSet;

   // Add advanced transformation matrices
   std::vector<Matrix_3x3> advancedMatrices = generateAdvancedTransformationMatrices(complexity);
   for (const auto& m : advancedMatrices) {
      matrixSet.insert(m);
   }

   // Add body diagonal transformations
   std::vector<Matrix_3x3> diagonalMatrices = generateBodyDiagonalTransformations(complexity);
   for (const auto& m : diagonalMatrices) {
      matrixSet.insert(m);
   }

   // Calculate both B4 distance and S6 angle for each matrix
   std::vector<std::pair<TransformResult, double>> results;

   for (const auto& matrix : matrixSet) {
      // Apply transformation to B4
      B4 transformedB4 = matrix * m_sourceB4;

      // Calculate B4 distance
      double b4Distance = B4::DistanceBetween(transformedB4, m_targetB4);

      // Create TransformResult
      TransformResult result(matrix, transformedB4, b4Distance);

      // Calculate S6 angle
      S6 transformedS6 = S6(LatticeCell(transformedB4).getCell());
      S6 targetS6 = S6(LatticeCell(m_targetB4).getCell());
      double s6Angle = angleS6(transformedS6, targetS6);

      // Store both the result and the S6 angle
      results.emplace_back(result, s6Angle);
   }

   // Sort by a composite score (weighted B4 distance and S6 angle)
   std::sort(results.begin(), results.end(),
      [](const auto& a, const auto& b) {
         const double weightB4 = 0.3;
         const double weightS6 = 0.7;

         double scoreA = weightB4 * a.first.b4Distance + weightS6 * a.second;
         double scoreB = weightB4 * b.first.b4Distance + weightS6 * b.second;

         return scoreA < scoreB;
      });

   // Extract the TransformResults
   std::vector<TransformResult> finalResults;
   for (size_t i = 0; i < std::min(maxResults, results.size()); ++i) {
      finalResults.push_back(results[i].first);
   }

   return finalResults;
}

void B4Matcher::displayTransformResult(
   const TransformResult& result,
   const LRL_Cell& sourceCell,
   const LRL_Cell& targetCell) {

   // Basic output
   std::cout << "Matrix:" << std::endl;
   std::cout << result.transformMatrix << std::endl;

   // Convert to cell parameters for display
   LatticeCell transformedCell(result.transformedB4);
   LRL_Cell transformedRawCell = transformedCell.getCell();
   LRL_Cell_Degrees transformedCellDeg(transformedRawCell);
   LRL_Cell_Degrees targetCellDeg(targetCell);

   std::cout << "Transformed Cell:    " << transformedCellDeg << std::endl;
   std::cout << "Target Cell:         " << targetCellDeg << std::endl;

   // Calculate metrics
   S6 transformedS6 = S6(transformedRawCell);
   S6 targetS6 = S6(targetCell);
   double s6Angle = angleS6(transformedS6, targetS6);

   P3 transformedP3(transformedRawCell);
   P3 targetP3(targetCell);
   double p3Distance = P3::DistanceBetween(transformedP3, targetP3);

   // Calculate parameter differences
   double paramDiffs[6] = {
       std::abs(transformedCellDeg[0] - targetCellDeg[0]),
       std::abs(transformedCellDeg[1] - targetCellDeg[1]),
       std::abs(transformedCellDeg[2] - targetCellDeg[2]),
       std::abs(transformedCellDeg[3] - targetCellDeg[3]),
       std::abs(transformedCellDeg[4] - targetCellDeg[4]),
       std::abs(transformedCellDeg[5] - targetCellDeg[5])
   };

   double totalParamDiff =
      paramDiffs[0] + paramDiffs[1] + paramDiffs[2] +
      paramDiffs[3] / 10.0 + paramDiffs[4] / 10.0 + paramDiffs[5] / 10.0;

   // Display metrics
   std::cout << "\nMetrics:" << std::endl;
   std::cout << "  B4 Distance:      " << result.b4Distance << std::endl;
   std::cout << "  S6 Angle:         " << s6Angle << " degrees" << std::endl;
   std::cout << "  P3 Distance:      " << p3Distance << std::endl;
   std::cout << "  Param Difference: " << totalParamDiff << std::endl;

   // Parameter comparison
   std::cout << "\nParameter matches:" << std::endl;
   std::cout << "  a: " << transformedCellDeg[0] << " vs " << targetCellDeg[0]
      << " (diff: " << paramDiffs[0] << ")" << std::endl;
   std::cout << "  b: " << transformedCellDeg[1] << " vs " << targetCellDeg[1]
      << " (diff: " << paramDiffs[1] << ")" << std::endl;
   std::cout << "  c: " << transformedCellDeg[2] << " vs " << targetCellDeg[2]
      << " (diff: " << paramDiffs[2] << ")" << std::endl;
   std::cout << "  alpha: " << transformedCellDeg[3] << " vs " << targetCellDeg[3]
      << " (diff: " << paramDiffs[3] << ")" << std::endl;
   std::cout << "  beta: " << transformedCellDeg[4] << " vs " << targetCellDeg[4]
      << " (diff: " << paramDiffs[4] << ")" << std::endl;
   std::cout << "  gamma: " << transformedCellDeg[5] << " vs " << targetCellDeg[5]
      << " (diff: " << paramDiffs[5] << ")" << std::endl;
}



// Modify this function in B4Matcher.cpp
std::vector<B4Matcher::TransformResult> B4Matcher::findBestParameterFit(
   const size_t maxResults, int complexity) const {

   // Generate matrices
   std::vector<Matrix_3x3> matrices = generateAdvancedTransformationMatrices(complexity);
   std::vector<Matrix_3x3> permMatrices = generatePermutationMatrices();
   matrices.insert(matrices.end(), permMatrices.begin(), permMatrices.end());

   // Process matrices and calculate parameter differences
   std::vector<std::pair<TransformResult, double>> results;
   for (const auto& matrix : matrices) {
      // Apply transformation
      B4 transformedB4 = matrix * m_sourceB4;
      double b4Distance = B4::DistanceBetween(transformedB4, m_targetB4);

      // Calculate parameter difference
      LatticeCell transformedCell(transformedB4);
      LRL_Cell_Degrees transformedCellDeg(transformedCell.getCell());
      LRL_Cell_Degrees targetCellDeg(LatticeCell(m_targetB4).getCell());

      double paramDiff =
         std::abs(transformedCellDeg[0] - targetCellDeg[0]) +
         std::abs(transformedCellDeg[1] - targetCellDeg[1]) +
         std::abs(transformedCellDeg[2] - targetCellDeg[2]) +
         std::abs(transformedCellDeg[3] - targetCellDeg[3]) / 10.0 +
         std::abs(transformedCellDeg[4] - targetCellDeg[4]) / 10.0 +
         std::abs(transformedCellDeg[5] - targetCellDeg[5]) / 10.0;

      TransformResult result(matrix, transformedB4, b4Distance);
      results.emplace_back(result, paramDiff);
   }

   // Sort by parameter difference
   std::sort(results.begin(), results.end(),
      [](const auto& a, const auto& b) {
         return a.second < b.second;
      });

   // Deduplicate using a set of matrices
   std::vector<TransformResult> finalResults;
   std::set<Matrix_3x3> uniqueMatrices;

   for (const auto& pair : results) {
      const auto& result = pair.first;

      // Only add if we haven't seen this matrix before
      if (uniqueMatrices.find(result.transformMatrix) == uniqueMatrices.end()) {
         finalResults.push_back(result);
         uniqueMatrices.insert(result.transformMatrix);

         // Stop if we have enough results
         if (finalResults.size() >= maxResults) break;
      }
   }

   // Remove this output line - it will be handled by the display class
   // std::cout << "Found " << results.size()
   //    << " transformations, removed "
   //    << (results.size() - uniqueMatrices.size())
   //    << " duplicates." << std::endl;

   return finalResults;
}

