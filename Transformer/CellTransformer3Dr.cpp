#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <limits>

#include <B4.h>
#include "CellTransformer3D.h"
#include <CS6Dist.h>
#include <CS6Dist.cpp>
#include <S6.h>


// Find the best transformation from source to target
CellTransformer3D::TransformationResult CellTransformer3D::findBestTransformation(
   double distanceThreshold) const {

   TransformationResult bestResult;

   // Phase 1: Try integer matrices
   std::vector<Matrix_3x3> integerMatrices = generateIntegerMatrices(2);

   for (const auto& matrix : integerMatrices) {
      // Apply transformation
      LatticeCell transformedCell = applyTransformation(matrix);

      // Calculate distance
      double distance = calculateCellDistance(transformedCell);

      // Update best result if better
      if (distance < bestResult.distance) {
         bestResult = TransformationResult(matrix, distance, true);

         // Early stopping if distance is below threshold
         if (distance < distanceThreshold) {
            return bestResult;
         }
      }
   }

   // Phase 2: Try matrices with one fractional component if needed
   if (bestResult.distance > distanceThreshold) {
      std::vector<Matrix_3x3> fractionalMatrices = generateFractionalMatrices(1);

      for (const auto& matrix : fractionalMatrices) {
         // Apply transformation
         LatticeCell transformedCell = applyTransformation(matrix);

         // Calculate distance
         double distance = calculateCellDistance(transformedCell);

         // Update best result if better
         if (distance < bestResult.distance) {
            bestResult = TransformationResult(matrix, distance, true);

            // Early stopping if distance is below threshold
            if (distance < distanceThreshold) {
               break;
            }
         }
      }
   }

   return bestResult;
}

// Find direct transformation (alias for findBestTransformation)
CellTransformer3D::TransformationResult CellTransformer3D::findDirectTransformation() const {
   // This is essentially the same as findBestTransformation
   return findBestTransformation();
}

// Find transformation using least squares
CellTransformer3D::TransformationResult CellTransformer3D::findLeastSquaresTransformation() const {
   // Extract basis vectors from source and target cells
   const LRL_Cell sourceCell = m_sourceCell.getCell();
   const LRL_Cell targetCell = m_targetCell.getCell();

   const B4 sourceB4(sourceCell);
   const B4 targetB4(targetCell);

   // Extract basis vectors
   const Vector_3 sourceV1 = sourceB4[0];
   const Vector_3 sourceV2 = sourceB4[1];
   const Vector_3 sourceV3 = sourceB4[2];

   const Vector_3 targetV1 = targetB4[0];
   const Vector_3 targetV2 = targetB4[1];
   const Vector_3 targetV3 = targetB4[2];

   // Create matrices from basis vectors (each column is a basis vector)
   const Matrix_3x3 sourceMatrix(
      sourceV1[0], sourceV2[0], sourceV3[0],
      sourceV1[1], sourceV2[1], sourceV3[1],
      sourceV1[2], sourceV2[2], sourceV3[2]
   );

   const Matrix_3x3 targetMatrix(
      targetV1[0], targetV2[0], targetV3[0],
      targetV1[1], targetV2[1], targetV3[1],
      targetV1[2], targetV2[2], targetV3[2]
   );

   // Calculate transformation matrix using least squares: M = T * S^(-1)
   const double det = sourceMatrix.Det();
   if (std::abs(det) < 1e-10) {
      // Source matrix is singular, return identity matrix
      return TransformationResult(Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1),
         std::numeric_limits<double>::max(), false);
   }

   const Matrix_3x3 transformMatrix = targetMatrix * sourceMatrix.Inver();

   // Apply transformation to verify and calculate error
   LatticeCell transformedCell = applyTransformation(transformMatrix);
   double distance = calculateCellDistance(transformedCell);

   // Check if valid lattice transformation (det = ±1)
   bool isValid = hasDeterminantOne(transformMatrix);

   return TransformationResult(transformMatrix, distance, isValid);
}

// Find integer transformations
std::vector<CellTransformer3D::TransformationResult> CellTransformer3D::findIntegerTransformations(
   int maxCoeff,
   size_t maxResults) const {

   std::vector<TransformationResult> results;

   // Generate integer matrices with determinant = ±1
   std::vector<Matrix_3x3> matrices = generateIntegerMatrices(maxCoeff);

   // Evaluate each matrix
   for (const auto& matrix : matrices) {
      // Apply transformation
      LatticeCell transformedCell = applyTransformation(matrix);

      // Calculate distance
      double distance = calculateCellDistance(transformedCell);

      // Store result
      results.push_back(TransformationResult(matrix, distance, true));
   }

   // Sort results by distance (lowest first)
   std::sort(results.begin(), results.end());

   // Limit number of results
   if (results.size() > maxResults) {
      results.resize(maxResults);
   }

   return results;
}