// Conversions between G6 and B4.
// Kept in its own translation unit so that programs which never construct
// a G6 from a B4 do not need to compile or link against B4.h.

#include "G6.h"
#include "B4.h"
#include "S6.h"

G6::G6(const B4& dt) {
   m_vec.resize(6);
   m_dim = 6;
   (*this) = S6(dt);
   m_valid = dt.GetValid();
}

G6& G6::operator= (const B4& v)
{
   (*this) = G6(v);
   return *this;
}
