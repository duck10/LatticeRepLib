#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "LRL_ReadLatticeData.h"


int main()
{
   std::cout << "; ToInverse (to reciprocal cell)" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();

   for (size_t i = 0; i < inputList.size(); ++i) {
      const G6 input = G6(inputList[i].GetCell());
      const LRL_Cell pCell = LatticeConverter::MakePrimitiveCell(inputList[i].GetLattice(), inputList[i].GetCell());

      const G6 inv = G6(pCell.Inverse());
      std::streamsize oldPrecision = std::cout.precision();
      std::cout << ";  G6 " << std::setw(9) << input << " input G6" << std::endl;
      std::cout << std::fixed;
      std::cout<< "G6 " << std::setprecision(9) << std::setw(7)
         << inv[0] << " " << inv[1] << " "
         << inv[2] << " " << inv[3] << " "
         << inv[4] << " " << inv[5] << " "
         << " inverse from LRL_Cell.Inverse" << std::endl;
      std::cout << std::setprecision(oldPrecision);
      std::cout.unsetf(std::ios::floatfield);
   }
}
//std::streamsize oldPrecision = o.precision();
//o << std::fixed << std::setprecision(3);
//for (size_t i = 0; i < v.size(); ++i)
//   o << std::setw(9) << v[i] << " ";
//o << std::setprecision(oldPrecision);
//o.unsetf(std::ios::floatfield);
