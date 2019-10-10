


#include "Delone.h"
#include "D7.h"
#include "G6.h"
#include "B4.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "S6.h"
#include "LatticeConverter.h"
#include "LRL_MaximaTools.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "MatS6.h"
#include "Niggli.h"
#include "Selling.h"

#include <string>


void LatticeConverter::TextOutput(const std::string& label, const std::string& lattice, const LRL_Cell& cell) const {
   std::cout << label << std::endl;
   std::cout << "lattice " << lattice << std::endl;
   std::cout << "LRL_Cell  " << LRL_ToString(cell) << std::endl;
   std::cout << "G6 " << LRL_ToString(G6(cell)) << std::endl;
   std::cout << "D7  " << LRL_ToString(D7(G6(cell))) << std::endl;
   std::cout << "Scalars " << LRL_ToString(S6(cell)) << std::endl;
   std::cout << "C3 " << LRL_ToString(C3(cell)) << std::endl;
}

void LatticeConverter::MaximaOutput(const std::string& label, const std::string& lattice, const LRL_Cell& cell) const {
   std::cout << label << std::endl;
   std::cout << "lattice " << lattice << std::endl;
   std::cout << "LRL_Cell  " << LRL_MaximaTools::MaximaFromString(LRL_ToString(cell)) << std::endl;
   std::cout << "G6 " << LRL_MaximaTools::MaximaFromString(LRL_ToString(G6(cell))) << std::endl;
   std::cout << "D7  " << LRL_MaximaTools::MaximaFromString(LRL_ToString(D7(G6(cell)))) << std::endl;
   std::cout << "Scalars " << LRL_MaximaTools::MaximaFromString(LRL_ToString(S6(cell))) << std::endl;
   std::cout << "C3 " << LRL_MaximaTools::MaximaFromString(LRL_ToString(C3(cell))) << std::endl;
}

LatticeConverter::LatticeConverter(const eOutputType type)
   : m_OutputType(type)
{

}

LRL_Cell LatticeConverter::MakePrimitiveCell(const std::string& lattice, const LRL_Cell& cell) {
   const G6 g6 = G6(cell);
   const MatG6 mLattice = LRL_Cell::LatSymMatG6(lattice, cell);
   MatG6 m66;
   G6 redVec;
   return LRL_Cell(mLattice*g6);
}

void LatticeConverter::SetOutputMaxima(void) { m_OutputType = emaxima; }
void LatticeConverter::SetOutputText  (void) { m_OutputType = etext; }

void LatticeConverter::Output(const std::string& label, const std::string& lattice, const LRL_Cell& cell) const {
   (m_OutputType == etext) ? TextOutput(label, lattice, LRL_Cell(cell)) : MaximaOutput(label, lattice, LRL_Cell(cell));
}

LRL_Cell LatticeConverter::NiggliReduceCell(const std::string& lattice, const LRL_Cell& cell) {
   const G6 g6 = G6(cell);
   const MatG6 mLattice = LRL_Cell::LatSymMatG6(lattice, cell);
   MatG6 m66;
   G6 redVec;
   const bool b = Niggli::Reduce(mLattice*g6, redVec, 0.0);
   if (b) {
      return LRL_Cell(redVec);;
   }
   else {
      return LRL_Cell();
   }
}

LRL_Cell LatticeConverter::NiggliReduceCell( const std::string& lattice, const LRL_Cell& cell, MatG6& mat ) {
   const MatG6 mLattice = LRL_Cell::LatSymMatG6( lattice, cell );
   MatG6 m66;
   G6 redVec;
   const bool b = Niggli::Reduce( mLattice * G6( cell ), m66, redVec, 0.0);
   if (b) {
      mat = m66 * mLattice;
      return LRL_Cell( redVec );
   } else {
      return LRL_Cell( );
   }
}


void LatticeConverter::NiggliReducedOutput(const std::string& label, const std::string& lattice, const LRL_Cell& cell) {
   const LRL_Cell reducedCell = NiggliReduceCell(lattice, cell);
   Output(label, "P", reducedCell);
}

LRL_Cell LatticeConverter::DeloneReduceCell(const std::string& lattice, const LRL_Cell& cell) {
   const G6 g6 = G6(cell);
   const MatG6 mLattice = LRL_Cell::LatSymMatG6(lattice, cell);
   MatS6 m66;
   S6 redVec;
   //const bool b = Delone::Reduce(S6(mLattice*g6), m66, redVec, 0.00000001);
   const bool b = Delone::Reduce(S6(mLattice*g6), redVec);
   if (b) {
      return LRL_Cell(redVec);;
   }
   else {
      return LRL_Cell();
   }
}

LRL_Cell LatticeConverter::SellingReduceCell(const std::string& lattice, const LRL_Cell& cell) {
   const MatG6 mLattice = LRL_Cell::LatSymMatG6(lattice, cell);
   MatS6 m66;
   S6 redVec;

   const bool b = Selling::Reduce(S6(mLattice*G6( cell ) ), redVec);
   if (b) {
      return LRL_Cell(redVec);;
   }
   else {
      return LRL_Cell();
   }
}

LRL_Cell LatticeConverter::SellingReduceCell( const std::string& lattice, const LRL_Cell& cell, MatS6& mat ) {
   const MatG6 mLattice = LRL_Cell::LatSymMatG6( lattice, cell );
   MatS6 m66;
   S6 redVec;

   const bool b = Selling::Reduce( S6( mLattice*G6( cell ) ), m66, redVec, 0.0 );
   if (b) {
      mat = m66 * mLattice;
      return LRL_Cell( redVec );;
   } else {
      return LRL_Cell( );
   }
}

void LatticeConverter::DeloneReducedOutput(const std::string& label, const std::string& lattice, const LRL_Cell& cell) {
   const LRL_Cell reducedCell = DeloneReduceCell(lattice, cell);
   Output(label, "P", reducedCell);
}
