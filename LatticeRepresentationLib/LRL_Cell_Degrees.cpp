

#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"

#include <iomanip>
#include <sstream>
#include <string>

LRL_Cell_Degrees::LRL_Cell_Degrees(const LRL_Cell& cell) {
   m_cell = cell.GetVector();
   for (unsigned long i = 3; i < 6; ++i)
      m_cell[i] *= 180.0 / (4.0*atan(1.0));
}


std::ostream& operator<< (std::ostream& o, const LRL_Cell_Degrees& c) {
   std::streamsize oldPrecision = o.precision();
   o << std::fixed << std::setprecision(5);
   for (unsigned long i = 0; i < 6; ++i)
      o << std::setw(9) << c.GetVector()[i] << " ";
   o << std::setprecision(oldPrecision);
   o.unsetf(std::ios::floatfield);
   return o;
}
