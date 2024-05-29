#ifndef LRL_WRITElATTICEDATA
#define LRL_WRITElATTICEDATA

#include <cmath>
#include <iomanip>
#include <string>
#include <vector>

#include "LRL_Cell_Degrees.h"

class LRL_WriteLatticeData {
public:

   explicit(false) LRL_WriteLatticeData(const int precision = 3)
      : m_precision(precision) {}

   void SetPrecision(const int n) { m_precision = n; }
   int GetPrecision() const { return m_precision; }

   template<typename OUTPUTTYPE>
   std::string Write(const OUTPUTTYPE& type, const LRL_Cell& cell) const {
      return Formatter(type, cell);
   }

   std::string WriteLatticeAndCell(const std::string& lattice, const LRL_Cell& cell) const;

private:
   template<typename OUTPUTTYPE>
   std::string Formatter(const OUTPUTTYPE& type, const LRL_Cell& cell) const {
      std::stringstream out;
      out << std::fixed << std::showpoint << std::setprecision(m_precision);
      out << type.GetName() + " ";

      const OUTPUTTYPE data(cell);
      for (size_t i = 0; i < cell.size(); ++i) {
         out << data[i] << " ";
      }

      return out.str() + "\n";
   }

   std::string Formatter(const LRL_Cell& , const LRL_Cell& cell) const;
   std::string Formatter(const std::string& lattice, const LRL_Cell& cell)const ;

private:
   int m_precision;
};

#endif // LRL_WRITElATTICEDATA
