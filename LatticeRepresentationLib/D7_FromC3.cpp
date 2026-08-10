// Conversions between D7 and C3.
// Kept in its own translation unit so that programs which never construct
// a D7 from a C3 do not need to compile or link against C3.h.

#include "D7.h"
#include "C3.h"
#include "S6.h"

D7::D7(const C3& c3) {
   m_dim = 7;
   (*this) = S6(c3);
   m_valid = c3.GetValid();
}

D7& D7::operator= (const C3& c3)
{
   (*this) = S6(c3);
   return *this;
}
