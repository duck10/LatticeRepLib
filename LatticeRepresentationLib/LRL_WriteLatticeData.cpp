#include "LRL_WriteLatticeData.h"

std::string LRL_WriteLatticeData::Formatter(const LRL_Cell&, const LRL_Cell& cell) const {
   return Formatter(std::string("P"), cell);
}

std::string LRL_WriteLatticeData::WriteLatticeAndCell(const std::string& lattice, const LRL_Cell& cell) const {
   return Formatter(lattice, LRL_Cell_Degrees(cell));
}

std::string LRL_WriteLatticeData::Formatter(const std::string& lattice, const LRL_Cell& cell) const {
   std::stringstream out;
   out << std::fixed << std::showpoint << std::setprecision(m_precision);
   out << lattice + " ";
   for (size_t i = 0; i < cell.size(); ++i) {
      out << cell[i] << " ";
   }

   return out.str() + "\n";
}
