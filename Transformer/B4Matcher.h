#ifndef B4_MATCHER_H
#define B4_MATCHER_H

#include "TransformerUtilities.h"

#include "B4.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LatticeCell.h"
#include "S6.h"
#include "Selling.h"
#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include "P3.h"
#include "TransformerUtilities.h" // Add this include
#include <vector>
#include <algorithm>
#include <limits>
#include <iostream>
#include <iomanip>
#include <sstream>



/**
 * @brief Generate all 24 permutation matrices with determinant +1
 *
 * @return std::vector<Matrix_3x3> List of permutation matrices
 */
std::vector<Matrix_3x3> generatePermutationMatrices();

/**
 * @brief Class for finding transformations between lattice cells using B4 representation
 *
 * The B4Matcher class preserves orientation information throughout the transformation
 * process by working directly with basis vectors rather than cell parameters.
 */
class B4Matcher {
public:
   /**
    * @brief Struct to hold transformation results
    */
   struct TransformResult {
      Matrix_3x3 transformMatrix;            ///< Transformation matrix
      B4 transformedB4;             ///< Transformed B4 after applying matrix
      double b4Distance;            ///< B4 distance between transformed and target
      std::string interpretation;   ///< Human-readable description

      // Default constructor needed for std::vector
      TransformResult()
         : transformMatrix(), transformedB4(), b4Distance(std::numeric_limits<double>::max()),
         interpretation("Identity") {
      }

      TransformResult(const Matrix_3x3& m, const B4& b4, double distance)
         : transformMatrix(m), transformedB4(b4), b4Distance(distance) {
         interpretation = interpretMatrix(m);
      }

      // Sort by B4 distance
      bool operator<(const TransformResult& other) const {
         return b4Distance < other.b4Distance;
      }

      /**
       * @brief Generate human-readable interpretation of a transformation matrix
       *
       * @param matrix The transformation matrix to interpret
       * @return std::string Description of the transformation
       */
      static std::string interpretMatrix(const Matrix_3x3& matrix);
   };

   /**
    * @brief Construct matcher from B4 representations
    *
    * @param sourceB4 Source B4
    * @param targetB4 Target B4
    */
   B4Matcher(const B4& sourceB4, const B4& targetB4)
      : m_sourceB4(sourceB4), m_targetB4(targetB4) {
   }

   /**
    * @brief Construct matcher from LRL_Cell representations
    *
    * @param sourceCell Source cell
    * @param targetCell Target cell
    */
   B4Matcher(const LRL_Cell& sourceCell, const LRL_Cell& targetCell)
      : m_sourceB4(sourceCell), m_targetB4(targetCell) {
   }

   /**
    * @brief Find the best transformation matrix using B4 distance
    *
    * @return TransformResult Best transformation based on B4 distance
    */
   TransformResult findBestTransformation(int complexity = 2) const;

   /**
    * @brief Find multiple transformations sorted by B4 distance
    *
    * @param maxResults Maximum number of results to return
    * @param complexity Complexity level for transformation generation
    * @return std::vector<TransformResult> List of transformations
    */
   std::vector<TransformResult> findMultipleTransformations(const size_t maxResults, int complexity = 2) const;

   /**
    * @brief Find multiple transformations sorted by a hybrid score of B4 distance and S6 angle
    *
    * @param maxResults Maximum number of results to return
    * @param complexity Complexity level for transformation generation
    * @return std::vector<TransformResult> List of transformations
    */
   std::vector<TransformResult> findMultipleHybridTransformations(const size_t maxResults, int complexity = 2) const;

   /**
    * @brief Display detailed information about a transformation result
    *
    * @param result Transformation result to display
    * @param sourceCell Original source cell
    * @param targetCell Target cell to match
    */

   /**
    * @brief Apply a transformation to a B4 vector
    *
    * @param matrix Transformation matrix
    * @param sourceB4 Source B4
    * @return B4 Transformed B4
    */
   static B4 applyTransformation(const Matrix_3x3& matrix, const B4& sourceB4) {
      return matrix * sourceB4;
   }

   // Add to B4Matcher.h
   std::vector<TransformResult> findBestParameterFit(const size_t maxResults, int complexity = 2) const;

   void displayTransformResult(
      const TransformResult& result,
      const LRL_Cell& sourceCell,
      const LRL_Cell& targetCell,
      bool showDetails = true) const;

   // Add to B4Matcher.h in the public section
   static void displayTransformResult(
      const TransformResult& result,
      const LRL_Cell& sourceCell,
      const LRL_Cell& targetCell);


   // Accessors
   const B4& getSourceB4() const { return m_sourceB4; }
   const B4& getTargetB4() const { return m_targetB4; }

private:
   B4 m_sourceB4;    ///< Source B4 representation
   B4 m_targetB4;    ///< Target B4 representation

   /**
    * @brief Check if a matrix is a permutation matrix (possibly with sign flips)
    *
    * @param matrix Matrix to check
    * @return bool True if matrix is a permutation matrix
    */
   static bool isPermutationMatrix(const Matrix_3x3& matrix);

   /**
    * @brief Analyze how a matrix would permute axes and parameters
    *
    * @param matrix Transformation matrix
    * @param sourceParams Source cell parameters
    * @param targetParams Target cell parameters
    */
   static void analyzeAxisPermutation(
      const Matrix_3x3& matrix,
      const LRL_Cell_Degrees& sourceParams,
      const LRL_Cell_Degrees& targetParams);

   /**
    * @brief Compare two double values with tolerance
    *
    * @param a First value
    * @param b Second value
    * @param tolerance Comparison tolerance
    * @return bool True if values are approximately equal
    */
   static bool isApproximately(double a, double b, double tolerance = 1e-10) {
      return std::abs(a - b) < tolerance;
   }
};

/**
 * @brief Test specific transformation matrices with detailed output
 *
 * @param sourceCell Source cell
 * @param targetCell Target cell
 */
void testTransformationMatrices(const LRL_Cell& sourceCell, const LRL_Cell& targetCell);

#endif // B4_MATCHER_H

