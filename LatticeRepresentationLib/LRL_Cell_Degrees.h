#ifndef LRL_CELL_DEGREES_H
#define LRL_CELL_DEGREES_H

#include "LRL_Cell.h"

class LRL_Cell_Degrees : public LRL_Cell {
public:
   friend std::ostream& operator<< (std::ostream& o, const LRL_Cell_Degrees& c);

   LRL_Cell_Degrees(const LRL_Cell& cell);
   LRL_Cell_Degrees(void) {}

   LRL_Cell_Degrees& operator= (const LRL_Cell& c) {
      m_cell = c.GetVector();
      m_valid = c.GetValid();
      return *this;
   }

   std::vector<double> GetVector(void) const { return m_cell; }
   std::string GetName(void) const { return "LRL_Cell_Degrees, unit cell"; }

};


#endif  // LRL_CELL_DEGREES_H