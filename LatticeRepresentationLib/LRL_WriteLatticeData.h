#ifndef LRL_WRITElATTICEDATA
#define LRL_WRITElATTICEDATA

#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "LRL_Cell_Degrees.h"
#include "LRL_ReadLatticeData.h"

class LRL_WriteLatticeData {
public:

   void SetPrecision(const int n) { m_precision = n; }
   int GetPrecision() const { return m_precision; }

   template<typename OUTPUTTYPE>
   std::string Write(const OUTPUTTYPE& type,
      const LRL_Cell& cell) const {
      std::string out;
      out += Formatter(type, cell);
      return out;
   }

   std::string Write(const LRL_Cell& type,
      const LRL_Cell& cell) const {
      std::string out;
      out += Formatter(cell);
      return out;
   }

private:
   template<typename OUTPUTTYPE>
   std::string Formatter(const OUTPUTTYPE& type, const LRL_Cell& cell) const {
      std::stringstream out;
      std::streamsize oldPrecision = std::cout.precision();
      out << std::setprecision(m_precision);
      out << "P ";
      for (size_t i = 0; i < cell.size(); ++i) {
         out << std::to_string(cell[i]) << " ";
      }

      out << "\n";
      out << std::setprecision(oldPrecision);
      out.unsetf(std::ios::floatfield);
      return out.str();
   }

   std::string Formatter(const LRL_Cell& cell) const {
      std::stringstream out;
      std::streamsize oldPrecision = std::cout.precision();
      out << std::setprecision(m_precision);
      out << "P ";
      for (size_t i = 0; i < cell.size(); ++i) {
         out << std::to_string(cell[i]) << " ";
      }

      out << "\n";
      out << std::setprecision(oldPrecision);
      out.unsetf(std::ios::floatfield);
      return out.str();
   }

private:
   int m_precision = 3;

};


#endif // LRL_WRITElATTICEDATA