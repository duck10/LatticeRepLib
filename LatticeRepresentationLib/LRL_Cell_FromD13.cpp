// Conversion from D13 to LRL_Cell.
// Not yet implemented -- kept in its own translation unit so that programs
// which never construct an LRL_Cell from a D13 do not pull in D13's
// definition (or anything it depends on) at all.

#include "LRL_Cell.h"
#include <iostream>

LRL_Cell::LRL_Cell(const D13& d13) {
   std::cout << "; not implemented" << std::endl;
   m_valid = false;
}
