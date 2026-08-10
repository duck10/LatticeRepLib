// Conversions between LRL_Cell and D7.
// Kept in its own translation unit so that programs which never construct
// an LRL_Cell from a D7 do not need to compile or link against D7.h (and
// whatever D7.h pulls in transitively, e.g. DC7u.h/Niggli.h).

#include "LRL_Cell.h"
#include "D7.h"
#include "G6.h"

LRL_Cell::LRL_Cell(const D7& v7)
   : m_valid(v7.GetValid())
{
   (*this) = G6(v7);
}

LRL_Cell& LRL_Cell::operator= (const D7& v) {
   *this = LRL_Cell(v);
   return *this;
}
