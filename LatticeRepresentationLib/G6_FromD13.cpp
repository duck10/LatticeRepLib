// Conversion from D13 to G6.
// Not yet implemented -- kept in its own translation unit so that programs
// which never construct a G6 from a D13 do not pull in D13's definition
// (or anything it depends on) at all.

#include "G6.h"
#include <iostream>

G6::G6(const D13& d13) {
   std::cout << "; not implemented" << std::endl;
   m_valid = false;
}
