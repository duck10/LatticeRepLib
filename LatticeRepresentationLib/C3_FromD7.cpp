// Conversions between C3 and D7.
// Kept in its own translation unit so that programs which never construct
// a C3 from a D7 do not need to compile or link against D7.h (and
// whatever D7.h pulls in transitively).

#include "C3.h"
#include "D7.h"
#include "G6.h"

C3::C3(const D7& v7)
   : m_valid(v7.GetValid())
{
   C3::m_reductionFunctions = C3::SetReduceFunctions();   (*this) = G6(v7);
}

C3& C3::operator= (const D7& v) {
   *this = C3(v);
   return *this;
}
