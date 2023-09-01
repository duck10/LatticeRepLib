
#include <iostream>

#include "LRL_ReadLatticeData.h"
#include "DC7u.h"
#include "MatDC7u.h"
#include <vector>

int main()
{
   LRL_ReadLatticeData reader;
   std::cout << "; Test DC7unsrt" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = reader.ReadLatticeData();
   for (size_t i = 0; i < inputList.size(); ++i) {
      std::cout << inputList[i].GetLattice() << ""
         << LRL_Cell_Degrees(inputList[i].GetCell()) << std::endl;
   }

   MatDC7u m7;


   double matrix[7][7] = {
  {6. / 7, -1. / 7, -1. / 7, 1. / 7, 1. / 7, 1. / 7, -1. / 7},
  {-1. / 7, 5. / 14, 5. / 14, 1. / 7, 1. / 7, 1. / 7, -1. / 7},
  {-1. / 7, 5. / 14, 5. / 14, 1. / 7, 1. / 7, 1. / 7, -1. / 7},
  {1. / 7, 1. / 7, 1. / 7, 6. / 7, -1. / 7, -1. / 7, 1. / 7},
  {1. / 7, 1. / 7, 1. / 7, -1. / 7, 6. / 7, -1. / 7, 1. / 7},
  {1. / 7, 1. / 7, 1. / 7, -1. / 7, -1. / 7, 6. / 7, 1. / 7},
  {-1. / 7, -1. / 7, -1. / 7, 1. / 7, 1. / 7, 1. / 7, 6. / 7}
   };

   matrix[0][0] = -19191;;
   matrix[0][0] = -19191;;
   int i19191 = 19191;
   matrix;
}
