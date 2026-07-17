//#ifndef LRL_CELL_DEGREES_H
//#define LRL_CELL_DEGREES_H
//
//#include "LRL_Cell.h"
//
//class LRL_Cell_Degrees : public LRL_Cell {
//public:
//   friend std::ostream& operator<< (std::ostream& o, coaznst LRL_Cell_Degrees& c);
//
//   LRL_Cell_Degrees(const LRL_Cell& cell);
//   LRL_Cell_Degrees(void) {}
//
//   LRL_Cell_Degrees& operator= (const LRL_Cell& c) {
//      SetVector(c.GetVector());
//      m_valid = c.GetValid();
//      return *this;
//   }
//
//   std::vector<double> GetVector(void) const { return std::vector<double>(m_cell.begin(), m_cell.end()); }
//   static std::string GetName(void) { return "LRL_Cell_Degrees, unit cell"; }
//
//};
//
//
//#endif  // LRL_CELL_DEGREES_H
