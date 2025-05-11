// TransformationTest.h
#ifndef TRANSFORMATION_TEST_H
#define TRANSFORMATION_TEST_H

#include <vector>
#include <string>
#include "Matrix_3x3.h"
#include "MatG6.h"
#include "LRL_Cell.h"
#include "Niggli.h"
#include "G6.h"
#include "B4.h"

static constexpr double M_PI = 3.14159265358979323846;


class TransformationTest {
public:
   // Struct to hold transformation data
   struct Transformation {
      std::string m_name;
      Matrix_3x3 m_3d_matrix;
      MatG6 m_6d_matrix;

      Transformation(const std::string& name, const Matrix_3x3& matrix3d, const MatG6& matrix6d)
         : m_name(name), m_3d_matrix(matrix3d), m_6d_matrix(matrix6d) {
      }
   };

   // Constructor - initializes the transformations
   TransformationTest();

   // Apply a 3D transformation to a cell
   LRL_Cell Apply3DTransformation(const LRL_Cell& cell, const Matrix_3x3& transform) const;

   // Apply a 6D transformation to a cell
   LRL_Cell Apply6DTransformation(const LRL_Cell& cell, const MatG6& transform) const;

   // Test all transformations on a given cell
   void TestTransformations(const LRL_Cell& testCell) const;

   double ReducedNCDist(const LRL_Cell& cell1, const LRL_Cell& cell2) const;


private:
   std::vector<Transformation> m_transformations;
};

#endif // TRANSFORMATION_TEST_H