#include "Delone.h"
#include "G6.h"
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


LatticeConverter::LatticeConverter(const eOutputType type)
   : m_OutputType(type)
{

}

LRL_Cell LatticeConverter::MakePrimitiveCell(const std::string& lattice, const LRL_Cell& cell) {
   if (lattice == "P") return cell;
   const G6 g6 = G6(cell);
   const MatG6 mLattice = LRL_Cell::LatSymMatG6(lattice, cell);
   const G6 returnG6 = mLattice * g6;
   return LRL_Cell(returnG6);
}

void LatticeConverter::SetOutputMaxima(void) { m_OutputType = emaxima; }
void LatticeConverter::SetOutputText(void) { m_OutputType = etext; }

LRL_Cell LatticeConverter::NiggliReduceCell(const std::string& lattice, const LRL_Cell& cell) {
   const G6 g6 = G6(cell);
   const MatG6 mLattice = LRL_Cell::LatSymMatG6(lattice, cell);
   MatG6 m66;
   G6 redVec;
   const bool b = Niggli::Reduce(mLattice * g6, redVec, 0.0);
   if (b) {
      return LRL_Cell(redVec);;
   }
   else {
      return LRL_Cell();
   }
}

LRL_Cell LatticeConverter::NiggliReduceCell(const std::string& lattice, const LRL_Cell& cell, MatG6& mat) {
   const MatG6 mLattice = LRL_Cell::LatSymMatG6(lattice, cell);
   MatG6 m66;
   G6 redVec;
   const bool b = Niggli::Reduce(mLattice * G6(cell), m66, redVec, 0.0);
   if (b) {
      mat = m66 * mLattice;
      return LRL_Cell(redVec);
   }
   else {
      return LRL_Cell();
   }
}

LRL_Cell LatticeConverter::DeloneReduceCell(const std::string& lattice, const LRL_Cell& cell) {
   const G6 g6 = G6(cell);
   const MatG6 mLattice = LRL_Cell::LatSymMatG6(lattice, cell);
   MatS6 m66;
   S6 redVec;
   const bool b = Delone::Reduce(S6(mLattice * g6), redVec);
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

   const bool b = Selling::Reduce(S6(mLattice * G6(cell)), redVec);
   if (b) {
      return LRL_Cell(redVec);;
   }
   else {
      return LRL_Cell();
   }
}

LRL_Cell LatticeConverter::SellingReduceCell(const std::string& lattice, const LRL_Cell& cell, MatS6& mat) {
   const MatG6 mLattice = LRL_Cell::LatSymMatG6(lattice, cell);
   MatS6 m66;
   S6 redVec;

   const bool b = Selling::Reduce(S6(mLattice * G6(cell)), m66, redVec, 0.0);
   if (b) {
      mat = m66 * mLattice;
      return LRL_Cell(redVec);;
   }
   else {
      return LRL_Cell();
   }
}
