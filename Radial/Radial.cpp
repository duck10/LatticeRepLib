
#include <iostream>

#include "C3.h"
#include "G6.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_ReadLatticeData.h"
#include "NCDist.h"
#include "Niggli.h"

#include <complex>
#include <vector>

double RadialDist(const LRL_Cell& a, const LRL_Cell&b ) {
   //const std::complex<double> a1(a[0] * (sin(a[3]) + cos(a[3])));
   //const std::complex<double> a2(a[1] * (sin(a[4]) + cos(a[4])));
   //const std::complex<double> a3(a[2] * (sin(a[5]) + cos(a[5])));

   //const std::complex<double> b1(a[0] * (sin(b[3]) + cos(b[3])));
   //const std::complex<double> b2(a[1] * (sin(b[4]) + cos(b[4])));
   //const std::complex<double> b3(a[2] * (sin(b[5]) + cos(b[5])));

   const double a1r = a[0] * sin(a[3]);
   const double a2r = a[1] * sin(a[4]);
   const double a3r = a[2] * sin(a[5]);

   const double a1i = a[0] * cos(a[3]);
   const double a2i = a[1] * cos(a[4]);
   const double a3i = a[2] * cos(a[5]);

   const double b1r = b[0] * sin(b[3]);
   const double b2r = b[1] * sin(b[4]);
   const double b3r = b[2] * sin(b[5]);

   const double b1i = b[0] * cos(b[3]);
   const double b2i = b[1] * cos(b[4]);
   const double b3i = b[2] * cos(b[5]);



   const double dsq =
      (a1r - b1r) * (a1r - b1r) +
      (a2r - b2r) * (a2r - b2r) +
      (a3r - b3r) * (a3r - b3r) +
      (a1i - b1i) * (a1i - b1i) +
      (a2i - b2i) * (a2i - b2i) +
      (a3i - b3i) * (a3i - b3i);
      return sqrt(dsq);
}

G6 Reduce(const LRL_Cell& in) {
   G6 out;
   const G6 gin(in);
   Niggli::Reduce(gin, out);
   return out;
}

int main()
{
   std::cout << "; Radial distances from first cell, radial dist (rd), ncdist (nc)" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   const LRL_Cell& baseCell = inputList[0].GetCell();
   const C3 baseC3(baseCell);
   const G6 baseG6 = Reduce(baseCell);
   std::cout << "; first input cell  " << LRL_Cell_Degrees(baseCell) << std::endl;
   for (size_t i = 0; i < inputList.size(); ++i) {
      const LRL_Cell currentCell = inputList[i].GetCell();
      const C3 currentC3(currentCell);
      const G6 currentG6(Reduce(currentCell).data());
      std::cout << "rd " << RadialDist(baseCell, currentCell) << "  nc " <<
         NCDist(baseG6.data(), currentG6.data()) << "  --  cell " <<
         LRL_Cell_Degrees(currentCell) << std::endl;
   }
}
