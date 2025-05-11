#ifndef LATTICE_MATCHER_H
#define LATTICE_MATCHER_H

#include "LRL_Vector3.h"
#include "LatticeCell.h"
#include "B4.h"
#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include "NCDist.h"
#include "S6.h"
#include "Selling.h"
#include "G6.h"

#include <vector>
#include <string>
#include <utility>
#include <limits>
#include <array>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <iomanip>

     // Check if matrix has determinant +1 (valid for lattice transformation)
static  bool isValidTransformationMatrix(const Matrix_3x3& matrix) {
   // Calculate determinant and check if it's +1 (within numerical precision)
   const double det = matrix.Det();
   const double tolerance = 1e-10; // Adjust tolerance as needed
   return std::abs(det - 1.0) < tolerance;
}



// Using a templated class to force all implementations to be included at compile time
template <typename T = void>
class LatticeMatcher {
public:
   struct TransformationResult {
      Matrix_3x3 transformMatrix;
      double distance;
      bool isValid;

      TransformationResult()
         : transformMatrix(1, 0, 0, 0, 1, 0, 0, 0, 1),
         distance(std::numeric_limits<double>::max()),
         isValid(true) {
      }


      TransformationResult(const Matrix_3x3& matrix, const double dist, const bool valid = true)
         : transformMatrix(matrix), distance(dist), isValid(valid) {
      }

      bool operator<(const TransformationResult& other) const {
         return distance < other.distance;
      }
   };

   LatticeMatcher() = default;

   LatticeMatcher(const LatticeCell& sourceCell, const LatticeCell& targetCell)
      : m_sourceCell(sourceCell), m_targetCell(targetCell) {
   }

   void setSourceCell(const LatticeCell& cell) {
      m_sourceCell = cell;
   }

   void setTargetCell(const LatticeCell& cell) {
      m_targetCell = cell;
   }

   const LatticeCell& getSourceCell() const {
      return m_sourceCell;
   }

   const LatticeCell& getTargetCell() const {
      return m_targetCell;
   }

   LatticeCell applyTransformation(const Matrix_3x3& transformMatrix) const {
      const LRL_Cell sourceCell = m_sourceCell.getCell();
      const B4 sourceB4(sourceCell);

      // Use the new matrix multiplication operator
      const B4 transformedB4 = transformMatrix * sourceB4;

      // Return the LatticeCell for display/compatibility
      return LatticeCell(transformedB4);
   }


   B4 applyTransformationToB4(const Matrix_3x3& transformMatrix, const B4& sourceB4) const {
      return transformMatrix * sourceB4;  // much cleaner!
   }

   S6 cellToS6(const LatticeCell& cell) const {
      const LRL_Cell lrlCell = cell.getCell();
      return S6(lrlCell);
   }

   double calculateS6Distance(const LatticeCell& cell1, const LatticeCell& cell2) const {
      // Convert to S6
      S6 s61 = S6(cell1.getCell());
      S6 s62 = S6(cell2.getCell());

      // Apply Selling reduction to both
      S6 reduced1, reduced2;
      Selling::Reduce(s61, reduced1);
      Selling::Reduce(s62, reduced2);

      // Calculate CS6Dist using reduced cells
      return CS6Dist(reduced1.data(), reduced2.data());
   }


   //double calculateCellDistance(const LatticeCell& transformedCell) const {
   //   return calculateS6Distance(transformedCell, m_targetCell);
   //}

   //double calculateCellDistance(const LatticeCell& cell1, const LatticeCell& cell2) const {
   //   return calculateS6Distance(cell1, cell2);
   //}

   bool hasDeterminantOne(const Matrix_3x3& matrix) const {
      const double det = matrix.Det();
      return std::abs(std::abs(det) - 1.0) < 1e-10;
   }

   bool isValidTransformation(const Matrix_3x3& transformMatrix) const {
      return hasDeterminantOne(transformMatrix);
   }

   std::vector<Matrix_3x3> generateIntegerMatrices(const int maxCoeff = 2) const {
      std::vector<Matrix_3x3> matrices;

      std::vector<int> coeffs;
      for (int i = -maxCoeff; i <= maxCoeff; ++i) {
         coeffs.push_back(i);
      }

      for (int a : coeffs) {
         if (a == 0) continue;
         for (int b : coeffs) {
            for (int c : coeffs) {
               for (int d : coeffs) {
                  if (d == 0) continue;
                  for (int e : coeffs) {
                     for (int f : coeffs) {
                        for (int g : coeffs) {
                           if (g == 0) continue;
                           for (int h : coeffs) {
                              for (int i : coeffs) {
                                 Matrix_3x3 matrix(a, b, c, d, e, f, g, h, i);
                                 if (hasDeterminantOne(matrix)) {
                                    matrices.push_back(matrix);

                                    if (matrices.size() >= 10000) {
                                       return matrices;
                                    }
                                 }
                              }
                           }
                        }
                     }
                  }
               }
            }
         }
      }

      return matrices;
   }

   std::vector<Matrix_3x3> generateFractionalMatrices(const int maxCoeff = 2) const {
      std::vector<Matrix_3x3> matrices;

      std::vector<int> coeffs;
      for (int i = -maxCoeff; i <= maxCoeff; ++i) {
         coeffs.push_back(i);
      }

      std::vector<double> fractions = { 0.5, -0.5 };

      for (int pos = 0; pos < 9; ++pos) {
         for (double frac : fractions) {
            std::vector<Matrix_3x3> intMatrices = generateIntegerMatrices(1);

            for (Matrix_3x3& matrix : intMatrices) {
               const double original = matrix[pos];
               matrix[pos] = frac;

               if (hasDeterminantOne(matrix)) {
                  matrices.push_back(matrix);
               }

               matrix[pos] = original;

               if (matrices.size() >= 5000) {
                  return matrices;
               }
            }
         }
      }

      return matrices;
   }


   TransformationResult findLeastSquaresTransformation() const {
      const LRL_Cell sourceCell = m_sourceCell.getCell();
      const LRL_Cell targetCell = m_targetCell.getCell();

      const B4 sourceB4(sourceCell);
      const B4 targetB4(targetCell);

      const Vector_3 sourceV1 = sourceB4[0];
      const Vector_3 sourceV2 = sourceB4[1];
      const Vector_3 sourceV3 = sourceB4[2];

      const Vector_3 targetV1 = targetB4[0];
      const Vector_3 targetV2 = targetB4[1];
      const Vector_3 targetV3 = targetB4[2];

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

      const double det = sourceMatrix.Det();
      if (std::abs(det) < 1e-10) {
         return TransformationResult(Matrix_3x3(1, 0, 0, 0, 1, 0, 0, 0, 1),
            std::numeric_limits<double>::max(), false);
      }

      const Matrix_3x3 transformMatrix = targetMatrix * sourceMatrix.Inver();
      const LatticeCell transformedCell = applyTransformation(transformMatrix);
      const double distance = calculateCellDistance(transformedCell);
      const bool isValid = hasDeterminantOne(transformMatrix);

      return TransformationResult(transformMatrix, distance, isValid);
   }

   std::vector<TransformationResult> findIntegerTransformations(
      const int maxCoeff = 2,
      const size_t maxResults = 5) const {

      std::vector<TransformationResult> results;
      const std::vector<Matrix_3x3> matrices = generateIntegerMatrices(maxCoeff);

      for (const auto& matrix : matrices) {
         const LatticeCell transformedCell = applyTransformation(matrix);
         const double distance = calculateCellDistance(transformedCell);
         results.push_back(TransformationResult(matrix, distance, true));
      }

      std::sort(results.begin(), results.end());

      if (results.size() > maxResults) {
         results.resize(maxResults);
      }

      return results;
   }

   static std::string interpretTransformation(const Matrix_3x3& matrix) {
      std::ostringstream oss;

      auto valueToString = [](double val) -> std::string {
         if (std::abs(val - std::round(val)) < 1e-10) {
            int intVal = static_cast<int>(std::round(val));
            if (intVal == 0) return "0";
            if (intVal == 1) return "";
            if (intVal == -1) return "-";
            return std::to_string(intVal);
         }

         if (std::abs(val - 0.5) < 1e-10) return "1/2";
         if (std::abs(val + 0.5) < 1e-10) return "-1/2";
         if (std::abs(val - 1.0 / 3.0) < 1e-10) return "1/3";
         if (std::abs(val + 1.0 / 3.0) < 1e-10) return "-1/3";
         if (std::abs(val - 2.0 / 3.0) < 1e-10) return "2/3";
         if (std::abs(val + 2.0 / 3.0) < 1e-10) return "-2/3";

         std::ostringstream valStr;
         valStr << std::fixed << std::setprecision(4) << val;
         return valStr.str();
         };

      auto buildTerm = [&valueToString](double coeff, char var) -> std::string {
         if (std::abs(coeff) < 1e-10) return "";

         std::string term = valueToString(coeff) + var;
         return term;
         };

      oss << "a' = ";
      if (std::abs(matrix[0]) > 1e-10) oss << buildTerm(matrix[0], 'a');
      if (std::abs(matrix[1]) > 1e-10) {
         if (oss.str().length() > 4 && matrix[1] > 0) oss << " + ";
         oss << buildTerm(matrix[1], 'b');
      }
      if (std::abs(matrix[2]) > 1e-10) {
         if (oss.str().length() > 4 && matrix[2] > 0) oss << " + ";
         oss << buildTerm(matrix[2], 'c');
      }
      if (oss.str() == "a' = ") oss << "0";

      oss << std::endl << "b' = ";
      if (std::abs(matrix[3]) > 1e-10) oss << buildTerm(matrix[3], 'a');
      if (std::abs(matrix[4]) > 1e-10) {
         if (oss.str().length() > 4 && matrix[4] > 0) oss << " + ";
         oss << buildTerm(matrix[4], 'b');
      }
      if (std::abs(matrix[5]) > 1e-10) {
         if (oss.str().length() > 4 && matrix[5] > 0) oss << " + ";
         oss << buildTerm(matrix[5], 'c');
      }
      if (oss.str().substr(oss.str().find_last_of('\n') + 1) == "b' = ") oss << "0";

      oss << std::endl << "c' = ";
      if (std::abs(matrix[6]) > 1e-10) oss << buildTerm(matrix[6], 'a');
      if (std::abs(matrix[7]) > 1e-10) {
         if (oss.str().length() > oss.str().find_last_of('\n') + 4 && matrix[7] > 0) oss << " + ";
         oss << buildTerm(matrix[7], 'b');
      }
      if (std::abs(matrix[8]) > 1e-10) {
         if (oss.str().length() > oss.str().find_last_of('\n') + 4 && matrix[8] > 0) oss << " + ";
         oss << buildTerm(matrix[8], 'c');
      }
      if (oss.str().substr(oss.str().find_last_of('\n') + 1) == "c' = ") oss << "0";

      return oss.str();
   }


   // Stream output operator for LatticeMatcher
   friend std::ostream& operator<<(std::ostream& os, const LatticeMatcher& matcher) {
      os << "LatticeMatcher:" << std::endl;
      os << "  Source cell: " << std::endl;
      os << matcher.m_sourceCell.getCell() << std::endl;
      os << "  Target cell: " << std::endl;
      os << matcher.m_targetCell.getCell() << std::endl;
      return os;
   }

private:
   LatticeCell m_sourceCell;
   LatticeCell m_targetCell;
};

// Create a type alias for convenience
using LatticeMatcherDefault = LatticeMatcher<>;

#endif // LATTICE_MATCHER_H

