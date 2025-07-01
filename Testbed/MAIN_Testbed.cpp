#include <algorithm>
#include <vector>
#include <iomanip>
#include <iostream>
#include <cmath>
#include <set>

#include "MultiTransformFinderControls.h"
#include "Niggli.h"
#include "G6.h"
#include "P3.h"
#include "LRL_Vector3.h"
#include "B4.h"
#include "MatG6.h"

#include "InputHandler.h"
#include "ProgramSetup.h"



static std::vector<Matrix_3x3> XgenerateUnimodularMatrices() {
   /*
   this function generates a subset of the unimodular matrices with determinant +1
   and elements 0/+1/-1. It is a restricted subset of group GL(n,Z).
   */
   std::vector<Matrix_3x3> result;

   // Allowed values: -1, 0, 1
   static const std::vector<double>values  = { -1, 0, 1 };
   const size_t n = values.size();

   // Generate all possible 3x3 matrices with elements from {-1, 0, 1}
   for (int a00 = 0; a00 < n; a00++) {
      for (int a01 = 0; a01 < n; a01++) {
         for (int a02 = 0; a02 < n; a02++) {
            for (int a10 = 0; a10 < n; a10++) {
               for (int a11 = 0; a11 < n; a11++) {
                  for (int a12 = 0; a12 < n; a12++) {
                     for (int a20 = 0; a20 < n; a20++) {
                        for (int a21 = 0; a21 < n; a21++) {
                           for (int a22 = 0; a22 < n; a22++) {
                              Matrix_3x3 mat = {
                                  values[a00], values[a01], values[a02],
                                  values[a10], values[a11], values[a12],
                                  values[a20], values[a21], values[a22]
                              };

                              if (mat.Det() == 1) {
                                 result.emplace_back(mat);
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

   return result;
}




static std::vector<Matrix_3x3> unimodular_matrices = XgenerateUnimodularMatrices();

// Operator to transform a cell using a matrix
inline LRL_Cell operator*(const Matrix_3x3& matrix, const LRL_Cell& cell) {
   // Convert cell to B4 representation
   B4 b4Cell(cell);

   // Apply transformation in B4 space
   B4 transformedB4 = matrix * b4Cell;

   // Convert back to LRL_Cell
   return LRL_Cell(transformedB4);
}

std::pair<LRL_Cell, LRL_Cell> ReducePair(const LRL_Cell& reference, const LRL_Cell& mobile) {
   G6 red1;
   G6 red2;
   MatG6 mg6;
   Matrix_3x3 m3d;
   double minP3 = DBL_MAX;
   Niggli::ReduceWithTransforms(G6(reference), mg6, m3d, red1);
   Niggli::ReduceWithTransforms(G6(mobile), mg6, m3d, red2);
   return { red1,red2 };
}

//unimodular_matrices
void FindBestFit(const LRL_Cell& reference, const LRL_Cell& mobile) {
   double minP3 = DBL_MAX;
   for (const auto& m : unimodular_matrices) {
      const P3 p1(reference);
      const P3 p2(m*mobile);
      minP3 = std::min(minP3, (p1 - p2).norm());
   }
   std::cout << "smallest P3 distance " << minP3 << std::endl;
}

void VariousFits(const LRL_Cell& reference, const LRL_Cell& mobile) {
   std::pair<LRL_Cell,LRL_Cell> result = ReducePair(reference, mobile);
   auto [red1, red2] = result;

   FindBestFit(reference, mobile);
   FindBestFit(red1, mobile);
   FindBestFit(reference, red2);
   FindBestFit(red1, red2);
   std::cout << "---------------------------" << std::endl;
}

inline  double angleS6(const S6& s1, const S6& s2) {
   return acos(s1.Dot(s2) / s1.norm() / s2.norm()) * 180 / 4.0 / atan(1.0);
}

// Example usage
int main() {

   for (size_t i = 0; i < 2000; ++i) {
      const LRL_Cell cell1 = Polar::rand();
      const LRL_Cell cell2 = Polar::rand();
      const double p3diff = (P3(cell1) - P3(cell2)).norm();
      const double s6angle = angleS6(cell1, cell2);
      const double g6diff = (G6(cell1) - G6(cell2)).norm();
      std::cout << p3diff << "  " << g6diff << std::endl;

   }

   exit(0);
   bool found = false;
   for (size_t i = 0; i < unimodular_matrices.size(); ++i) {
      const Matrix_3x3& current = unimodular_matrices[i];

      // Debug: check a few specific indices
      if (i < 5 || i > unimodular_matrices.size() - 5) {
         std::cout << "Matrix " << i << ":" << std::endl << current << std::endl;
      }

      MultiTransformFinderControls controls;
      const BasicProgramInput<MultiTransformFinderControls> program_setup("UnifiedLatticeMatcher", controls);


      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      const std::vector<LatticeCell>& inputList = program_setup.getInputList();
      if (inputList.size() < 2) {
         std::cout << "at least 2 input cells are required" << std::endl;
         return (-1);
      }

      G6 red1;
      MatG6 mg6;
      Matrix_3x3 m3d;
      Niggli::ReduceWithTransforms(G6(inputList[0]), mg6, m3d, red1);

      for (size_t kk = 1; kk < inputList.size(); ++kk)
      {
         VariousFits((inputList[0].getCell()), inputList[kk].getCell());
         //FindBestFit(red1, inputList[kk].getCell());
      }
      return 0;
   }
}