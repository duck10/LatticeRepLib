
#include <iostream>
#include <utility>
#include <vector>

#include "C3.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_Vector3.h"
#include "LRL_MaximaTools.h"

std::vector<Matrix_3x3> Create3x3Matrices() {
   std::vector<Matrix_3x3> vm;
   vm.push_back(Matrix_3x3( 1, 0, 0,   0, 1, 0,   0, 0, 1));  // identity
   vm.push_back(Matrix_3x3( -1, 0, 0,   0, 0, 1,   0, 1, 0));  // exchange b/c
   vm.push_back(Matrix_3x3( 0, 1, 0,   1, 0, 0,   0, 0, -1));  // exchange a/b
   vm.push_back(Matrix_3x3( 0, 0, 1,   1, 0, 0,   0, 1, 0));  // rotate a/b/c => c'/a'/b/
   vm.push_back(Matrix_3x3( 0, 1, 0,   0, 0, 1,   1, 0, 0));  // rotate a/b/c -> b'/c'/a'
   vm.push_back(Matrix_3x3( 0, 0, 1,   0, -1, 0,   1, 0, 0));  // exchange a/c
   return vm;
}

bool HasTwoRightAngles(const LRL_Cell& cell) {
   int count = 0;
   const double rad = 90.0 / 180.0 * (4.0 * atan(1.0));
   count += (abs(cell[3] - rad) < 1.0E-6) ? 1 : 0;
   count += (abs(cell[4] - rad) < 1.0E-6) ? 1 : 0;
   count += (abs(cell[5] - rad) < 1.0E-6) ? 1 : 0;
   return count == 2;
}

int main()
{
   const std::vector<Matrix_3x3> vm3x3 = Create3x3Matrices();

   // read the input
   std::cout << "; To B Unique" << std::endl;
   std::vector<std::pair<LRL_Cell,Matrix_3x3> > cells;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   for (size_t i = 0; i < inputList.size(); ++i) {
      const std::string lattice = inputList[i].GetLattice();
      const S6 s1 = inputList[i].GetCell();

      // if the cell is has 2 right angles, make sure beta is unique
      if (!HasTwoRightAngles(inputList[i].GetCell())) {
         std::cout << "input cell does not have two 90 degree angles" << std::endl;
      }
      else if (s1[1] != 0.0) { //beta already unique
         cells.push_back(std::make_pair(inputList[i].GetCell(), vm3x3[0]));
      }
      else if (s1[0] == 0.0) { // gamma is unique
         C3 c(s1); 
         std::swap(c[1], c[2]);
         cells.push_back(std::make_pair(c, vm3x3[3]));
      }
      else if (s1[2] == 0.0) { // alpha is unique
         C3 c(s1);
         std::swap(c[0], c[1]);
         cells.push_back(std::make_pair(c, vm3x3[4]));
      }
   }

   // if necessary, swap a and c
   for (size_t i = 0; i < cells.size(); ++i) {
      if (cells[i].first[0] > cells[i].first[2]) {
         const LRL_Cell& thisCell = cells[i].first;
         const Matrix_3x3& m3 = vm3x3[5] * cells[i].second;
         C3 c(thisCell);
         std::swap(c[0], c[2]);
         cells[i] = std::make_pair(c, m3);
      }
   }

   // output the modified cells
   for (size_t i = 0; i < cells.size(); ++i) {
      std::cout << LRL_Cell_Degrees(cells[i].first) << "  "
         << LRL_MaximaTools::MaximaFromMat(cells[i].second) << std::endl;
      std::cout << cells[i].second << std::endl << std::endl;
   }
}
