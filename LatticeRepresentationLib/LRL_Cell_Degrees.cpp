

#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"

#include <iomanip>
#include <sstream>
#include <string>

const double pi = 4.0*atan(1.0);
const double twopi = 2.0*pi;

LRL_Cell_Degrees::LRL_Cell_Degrees(const LRL_Cell& cell) {
   m_cell = cell.GetVector();
   m_valid = cell.GetValid() && m_cell[3] < pi && m_cell[4] < pi && m_cell[5] < pi && (m_cell[3] + m_cell[4] + m_cell[5])< twopi;
   if (m_valid)
      for (size_t i = 3; i < 6; ++i)
         m_cell[i] *= 180.0 / (4.0*atan(1.0));
   else
      *this = LRL_Cell(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
}


std::ostream& operator<< (std::ostream& o, const LRL_Cell_Degrees& c) {
   std::streamsize oldPrecision = o.precision();
   o << std::fixed << std::setprecision(3);
   for (size_t i = 0; i < 6; ++i)
      o << std::setw(9) << c.GetVector()[i] << " ";
   o << std::setprecision(oldPrecision);
   o.unsetf(std::ios::floatfield);
   return o;
}
