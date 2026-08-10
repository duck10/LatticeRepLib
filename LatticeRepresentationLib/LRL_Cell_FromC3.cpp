// Conversions between LRL_Cell and C3.
// Kept in its own translation unit so that programs which never construct
// an LRL_Cell from a C3 do not need to compile or link against C3.h.

#include "LRL_Cell.h"
#include "C3.h"
#include "S6.h"
#include "LRL_MinMaxTools.h"

#include <cmath>

namespace {
   const double pi = 4.0 * atan(1.0);
   const double twopi = 2.0 * pi;
}

LRL_Cell::LRL_Cell(const C3& c3)
{
   *this = S6(c3);
   m_valid = m_valid && c3.GetValid() && GetValid() && m_cell[3] < pi && m_cell[4] < pi && m_cell[5] < pi && (m_cell[3] + m_cell[4] + m_cell[5]) < twopi
      && (m_cell[3] + m_cell[4] + m_cell[5] - 2.0 * maxNC(m_cell[3], m_cell[4], m_cell[5]) >= 0.0);
}

LRL_Cell& LRL_Cell::operator= (const C3& c3) {
   *this = LRL_Cell(c3);
   return *this;
}
