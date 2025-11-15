#include <iostream>
#include <vector>
#include <set>
#include "ProgramSetup.h"
#include "TransformationMatrices.h"
#include "MatrixCellOperator.h"
#include "MatS6.h"
#include "P3.h"
#include "Polar.h"
#include "S6.h"
#include "Selling.h"

// THESE ARE BOUNDARY TRANSFORMATIONS IN S6
std::vector<MatS6> vmSelling{
   MatS6("-1 0 0  0 0 0   1  1 0 0  0 0   1 0  0 0 1  0   -1 0 0  1 0 0   1  0 1 0  0 0   1 0  0 0 0  1"),  // g or p or s1
   MatS6(" 1 1 0  0 0 0   0 -1 0 0  0 0   0 1  0 1 0  0    0 1 1  0 0 0   0 -1 0 0  1 0   0 1  0 0 0  1"),  // h or q or s2
   MatS6(" 1 0 1  0 0 0   0  0 1 1  0 0   0 0 -1 0 0  0    0 1 1  0 0 0   0  0 1 0  1 0   0 0 -1 0 0  1"),  // k or r or s3
   MatS6(" 1 0 0 -1 0 0   0  0 1 1  0 0   0 1  0 1 0  0    0 0 0 -1 0 0   0  0 0 1  1 0   0 0  0 1 0  1"),  // l or s or s4
   MatS6(" 0 0 1  0 1 0   0  1 0 0 -1 0   1 0  0 0 1  0    0 0 0  1 1 0   0  0 0 0 -1 0   0 0  0 0 1  1"),  // m or t or s5
   MatS6(" 0 1 0  0 0 1   1  0 0 0  0 1   0 0  1 0 0 -1    0 0 0  1 0 1   0  0 0 0  1 1   0 0  0 0 0 -1")   // n or u or s6
};

static const std::vector<Matrix_3x3> unimodular = TransformationMatrices::generateUnimodularMatrices();

int main() {
   //Selling::Write_S6_E3_MatrixPairs();

   std::cout << "; Find 3x3 matrices corresponding to S6 Selling reduction matrices" << std::endl;
   const S6 ran(-.1, -.2, .3, -100, -110, -120);

   int count = 0;
   for (const auto& vm : vmSelling) {
      ++count;
      const LRL_Cell cellTest = vm * ran;

      bool found = false;
      for (const auto& u : unimodular) {
         const LRL_Cell modCell = u * LRL_Cell(ran);
         const double p3dist = (P3(cellTest) - P3(modCell)).norm();
         if (p3dist < 0.0001) {
            std::cout << "S6 matrix # " << count << std::endl;
            std::cout << " P3 dist = " << p3dist << std::endl;
            std::cout << "S6 matrix:" << std::endl;
            std::cout << vm << std::endl;
            std::cout << "Corresponding 3x3 matrix:" << std::endl;
            std::cout << u << std::endl << std::endl;
            found = true;
            break;
         }
      }

      if (!found) {
         std::cout << "WARNING: No 3x3 matrix found for S6 matrix # " << count << std::endl << std::endl;
      }
   }

   return 0;
}