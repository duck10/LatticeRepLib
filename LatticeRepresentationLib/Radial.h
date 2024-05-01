#ifndef RADIAL_H
#define RADIAL_H

#include "G6.h"
#include "LRL_Cell.h"
#include "LRL_StringTools.h"


class Radial {
   enum class Yaxis { NCDist, CS6Dist };
   enum class Xaxis { Polar, G6 };
   enum class XaxisReduced { reduced, notreduced };
public:
   Radial();

   void SetYAxisCS6Dist() { yAxisType = Yaxis::CS6Dist; }
   void SetXAxisReduced() { xAxisReduced = XaxisReduced::reduced; }
   void SetXAxisG6() { xAxisType = Xaxis::G6; }
   void SetBaseCell(const std::string& lattice, const LRL_Cell& cell);
   void SetParameters(int argc, char* argv[]) {

      if (argc == 1) {
         return;
      }
      else if (argc == 2) {
         const std::string strtest(argv[1]);

         /*if (LRL_StringTools::strToupper(strtest).find('R') != std::string::npos)*/ SetXAxisReduced();
         if (LRL_StringTools::strToupper(strtest).find('C') != std::string::npos) SetYAxisCS6Dist();
         /*if (LRL_StringTools::strToupper(strtest).find('G') != std::string::npos)*/ SetXAxisG6();
      }
   }

   LRL_Cell GetBaseCell() const { return m_baseCell; }
   bool IsReducedXaxis() const { return xAxisReduced == XaxisReduced::reduced; }
   std::string GetRadialParameters() const;

   double XAxisDist(const std::string& latticeb, const LRL_Cell& b) const;
   double YAxisDist(const std::string& latticeb, const LRL_Cell& b) const;
   static double RadialDist(const LRL_Cell& a, const LRL_Cell& b);
   //std::vector<std::complex<double> > ComputePolarCoordinate(const LRL_Cell& cell) const;

private:
   Xaxis xAxisType;
   Yaxis yAxisType;
   XaxisReduced xAxisReduced;
   LRL_Cell m_baseCell;
   G6 m_baseG6;
   std::string m_lattice;
};


#endif //  RADIAL_H