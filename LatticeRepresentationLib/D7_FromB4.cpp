// Conversions between D7 and B4.
// Kept in its own translation unit so that programs which never construct
// a D7 from a B4 do not need to compile or link against B4.h.

#include "D7.h"
#include "B4.h"
#include "G6.h"

D7::D7(const B4& dt) {
   m_vec.resize(7);
   m_dim = 7;
   (*this) = G6(dt);
   m_valid = dt.GetValid();
}

D7& D7::operator= (const B4& v) {
   (*this) = G6(v);
   return *this;
}
