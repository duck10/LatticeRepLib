#include <cmath>
#include <iostream>
#include <string>

#include "BasisBase.h"
#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "LRL_ReadLatticeData.h"
#include "Niggli.h"

class V7 /*: private BasisBase<G6> */{
public:
   friend std::ostream& operator<< (std::ostream&, const V7&);
   V7() {};
   V7(const std::string& lattice, const LRL_Cell& cell);
private:
   std::vector<double> m_vec;
   bool m_valid;
};

std::ostream& operator<< (std::ostream& o, const V7& v) {
   for (size_t i = 0; i < 7; ++i) {
      o << v.m_vec[i] << " ";
   }
   std::cout << std::endl;
   return o;
}


V7::V7(const std::string& lattice, const LRL_Cell& cell)
   : m_valid(true)
{
   m_vec.resize(7);
   const LRL_Cell pCell = LatticeConverter::MakePrimitiveCell(lattice, cell);
   G6 red;
   G6 invRed;
   const bool b1 = Niggli::Reduce(cell, red);
   const bool b2 = Niggli::Reduce(cell.Inverse(), invRed);
   const double volume = cell.Volume();
   m_vec[0] = sqrt(red[0]);
   m_vec[1] = sqrt(red[1]);
   m_vec[2] = sqrt(red[2]);
   m_vec[3] = 1.0 / sqrt(invRed[0]);
   m_vec[4] = 1.0 / sqrt(invRed[1]);
   m_vec[5] = 1.0 / sqrt(invRed[2]);
   m_vec[6] = pow(volume, 1.0 / 3.0);
}

int main()
{
   std::cout << "; To V7" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();

   for (size_t i = 0; i < inputList.size(); ++i) {
      const LRL_Cell pCell = LatticeConverter::MakePrimitiveCell(inputList[i].GetLattice(), inputList[i].GetCell());
      std::cout << "V " << V7("P ", pCell) << std::endl;
   }
}
