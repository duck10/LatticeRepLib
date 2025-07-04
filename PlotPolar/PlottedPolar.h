#ifndef PLOTTEDPOLAR_H
#define PLOTTEDPOLAR_H

#include "Vector_2.h"

// Data structure holding coordinates already scaled for plotting
// Eliminates coordinate conversion bugs by working in plot space throughout
class PlottedPolar {
public:
   PlottedPolar() = default;

   PlottedPolar(const Vector_2& coord1, const Vector_2& coord2, const Vector_2& coord3)
      : m_coords{ coord1, coord2, coord3 }
   {
   }

   const Vector_2& operator[](const size_t index) const { return m_coords[index]; }
   Vector_2& operator[](const size_t index) { return m_coords[index]; }

   friend std::ostream& operator<<(std::ostream& os, const PlottedPolar& plotted) {
      os << "PlottedPolar: [" << plotted.m_coords[0][0] << "," << plotted.m_coords[0][1] << "] "
         << "[" << plotted.m_coords[1][0] << "," << plotted.m_coords[1][1] << "] "
         << "[" << plotted.m_coords[2][0] << "," << plotted.m_coords[2][1] << "]";
      return os;
   }

private:
   Vector_2 m_coords[3];  // Pre-scaled coordinates for each plot [0]=plot1, [1]=plot2, [2]=plot3
};

#endif // PLOTTEDPOLAR_H

