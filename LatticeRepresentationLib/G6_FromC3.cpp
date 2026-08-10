// Conversions between G6 and C3.
// Kept in its own translation unit so that programs which never construct
// a G6 from a C3 do not need to compile or link against C3.h.

#include "G6.h"
#include "C3.h"
#include "S6.h"

G6::G6(const C3& c3) {
   m_dim = 6;
   m_vec.resize(6);
   *this = S6(c3);
}

G6& G6::operator= (const C3& c3)
{
   (*this) = S6(c3);
   m_vec.SetValid(c3.GetValid());
   return *this;
}
