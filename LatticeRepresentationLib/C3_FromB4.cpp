// Conversions between C3 and B4.
// Kept in its own translation unit so that programs which never construct
// a C3 from a B4 do not need to compile or link against B4.h.

#include "C3.h"
#include "B4.h"
#include "S6.h"

C3::C3(const B4& del)
   : m_valid(del.GetValid())
{
   C3::m_reductionFunctions = C3::SetReduceFunctions();  (*this) = S6(del);
}

C3& C3::operator= (const B4& v) {
   *this = C3(v);
   return *this;
}
