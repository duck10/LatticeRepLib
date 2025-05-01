#ifndef CELL_TRANSFORMER_3D_H
#define CELL_TRANSFORMER_3D_H

#include "LRL_Vector3.h"
#include "LatticeCell.h"
#include "CS6Dist.h"  // Include your existing CS6Dist function
#include "NCDist.h"   // Include your existing NCDist function
#include "S6.h"       // Include your S6 class
#include "G6.h"       // Include your G6 class

#include <vector>
#include <string>
#include <utility>
#include <limits>
#include <array>
#include <sstream>

/**
 * @class CellTransformer3D
 * @brief Finds transformations between two unit cells in 3D space
 *
 * This class specializes in finding the transformation matrix between
 * two given unit cells that describe the same or similar crystal lattices.
 * It works directly in 3D space and provides interpretable transformations.
 */
class CellTransformer3D {
public:
   struct TransformationResult {
      Matrix_3x3 transformMatrix;
      double distance;  // Distance measure between transformed and target cell
      bool isValid;     // Whether this transformation preserves the lattice (det = ±1)

      TransformationResult()
         : transformMatrix(1, 0, 0, 0, 1, 0, 0, 0, 1),
         distance(std::numeric_limits<double>::max()),
         isValid(true) {
      }

      TransformationResult(const Matrix_3x3& matrix, double dist, bool valid = true)
         : transformMatrix(matrix), distance(dist), isValid(valid) {
      }

      bool operator<(const TransformationResult& other) const {
         return distance < other.distance;
      }
   };

   /**
    * @brief Default constructor
    */
   CellTransformer3D() = default;

   /**
    * @brief Constructor with source and target cells
    * @param sourceCell The source unit cell
    * @param targetCell The target unit cell
    */
   CellTransformer3D(const LatticeCell& sourceCell, const LatticeCell& targetCell);

   /**
    * @brief Set the source unit cell
    * @param cell The source unit cell
    */
   void setSourceCell(const LatticeCell& cell);

   /**
    * @brief Set the target unit cell
    * @param cell The target unit cell
    */
   void setTargetCell(const LatticeCell& cell);

   /**
    * @brief Get the source unit cell
    * @return The source unit cell
    */
   const LatticeCell& getSourceCell() const;

   /**
    * @brief Get the target unit cell
    * @return The target unit cell
    */
   const LatticeCell& getTargetCell() const;

   /**
    * @brief Find the best transformation from source to target cell
    * @param distanceThreshold Early stopping threshold for minimum distance
    * @return TransformationResult containing the matrix and distance
    */
   TransformationResult findBestTransformation(
      double distanceThreshold = 1.0e-6) const;

   /**
    * @brief Find direct transformation (similar to findBestTransformation)
    * @return TransformationResult containing the matrix and distance
    */
   TransformationResult findDirectTransformation() const;

   /**
    * @brief Find transformation using least squares
    * @return TransformationResult containing the matrix and distance
    */
   TransformationResult findLeastSquaresTransformation() const;

   /**
    * @brief Find integer transformations
    * @param maxCoeff Maximum coefficient value to try in matrix elements
    * @param maxResults Maximum number of results to return
    * @return Vector of TransformationResults sorted by error
    */
   std::vector<TransformationResult> findIntegerTransformations(
      int maxCoeff = 2,
      size_t maxResults = 5) const;

   /**
    * @brief Apply a transformation matrix to the source cell
    * @param transformMatrix The transformation matrix to apply
    * @return The transformed cell
    */
   LatticeCell applyTransformation(const Matrix_3x3& transformMatrix) const;

   /**
    * @brief Measure the distance between a transformed source cell and the target cell
    * @param transformedCell The transformed source cell
    * @return Distance measure (lower is better)
    */
   double calculateCellDistance(const LatticeCell& transformedCell) const;

   /**
    * @brief Verify if the transformation preserves the lattice
    * @param transformMatrix The transformation matrix to verify
    * @return True if the transformation preserves the lattice (det = ±1)
    */
   bool isValidTransformation(const Matrix_3x3& transformMatrix) const;

   /**
    * @brief Express a transformation matrix in terms of lattice vector operations
    * @param matrix The transformation matrix
    * @return String representation of lattice vector operations
    */
   static std::string interpretTransformation(const Matrix_3x3& matrix);

   /**
    * @brief Format the transformation result as a string
    * @param result The transformation result
    * @param precision Number of decimal places
    * @return Formatted string representation
    */
   std::string formatTransformation(
      const TransformationResult& result,
      int precision = 4) const;

private:
   LatticeCell m_sourceCell;
   LatticeCell m_targetCell;

   /**
    * @brief Generate transformation matrices with integer coefficients
    * @param maxCoeff Maximum absolute value for coefficients
    * @return Vector of transformation matrices
    */
   std::vector<Matrix_3x3> generateIntegerMatrices(int maxCoeff = 2) const;

   /**
    * @brief Generate transformation matrices with one fractional component
    * @param maxCoeff Maximum absolute value for integer coefficients
    * @return Vector of transformation matrices
    */
   std::vector<Matrix_3x3> generateFractionalMatrices(int maxCoeff = 2) const;

   /**
    * @brief Check if a matrix has determinant close to ±1
    * @param matrix The matrix to check
    * @return True if determinant is close to ±1
    */
   bool hasDeterminantOne(const Matrix_3x3& matrix) const;

   /**
    * @brief Convert a cell to S6 for distance calculation
    * @param cell The cell to convert
    * @return S6 representation of the cell
    */
   S6 cellToS6(const LatticeCell& cell) const;

   /**
    * @brief Calculate S6 distance between two cells
    * @param cell1 First cell
    * @param cell2 Second cell
    * @return S6 distance
    */
   double calculateS6Distance(const LatticeCell& cell1, const LatticeCell& cell2) const;
};

#endif // CELL_TRANSFORMER_3D_H