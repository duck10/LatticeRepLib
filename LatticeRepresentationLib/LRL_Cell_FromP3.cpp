// Conversion from P3 to LRL_Cell.
// Kept in its own translation unit so that programs which never construct
// an LRL_Cell from a P3 do not need to compile or link against P3.h.

#include "LRL_Cell.h"
#include "P3.h"

#include <cmath>

LRL_Cell::LRL_Cell(const P3& p) {

   for (size_t i = 0; i < 3; ++i) {
      const double x = p[i].first;
      const double y = p[i].second;
      const double length = std::sqrt(x * x + y * y);
      const double angle = std::atan2(y, x);  // radians

      m_cell[i] = length;
      m_cell[i + 3] = angle;
   }

   m_valid = (*this).CheckValid();
}
