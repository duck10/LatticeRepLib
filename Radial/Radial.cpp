
#include <iostream>

#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include "G6.h"
#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_ReadLatticeData.h"
#include "NCDist.h"
#include "Niggli.h"
#include "Selling.h"
#include "LRL_StringTools.h"

#include <algorithm>
#include <complex>
#include <string>
#include <vector>

G6 G6Reduce(const std::string& lattice, const LRL_Cell& in) {
   G6 out;
   const G6 gin(in);
   Niggli::Reduce(gin, out);
   return LatticeConverter::NiggliReduceCell("P", in);
}


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

         if (LRL_StringTools::strToupper(strtest).find('R') != std::string::npos) SetXAxisReduced();
         if (LRL_StringTools::strToupper(strtest).find('C') != std::string::npos) SetYAxisCS6Dist();
         if (LRL_StringTools::strToupper(strtest).find('G') != std::string::npos) SetXAxisG6();
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

std::string Radial::GetRadialParameters() const {
   std::string out;
   const std::string xax = xAxisType == Xaxis::Polar ? "Polar" : "G6-Euc";
   const std::string yax = yAxisType == Yaxis::NCDist ? "NCDist" : "CS6Dist";
   const std::string isReduced = IsReducedXaxis() ? "reduced" : "not_reduced";
   out += "xaxis_" + xax + "_yaxis_" + yax + "_xaxis_is_" + isReduced;
   return out;
}


Radial::Radial() {
   xAxisType = Xaxis::Polar;
   xAxisReduced = XaxisReduced::notreduced;
   yAxisType = Yaxis::NCDist;
}

//std::vector<std::complex<double> > Radial::ComputePolarCoordinate(const LRL_Cell& cell) const {
//   std::vector<std::complex<double> > out(3);
//   for (size_t i = 0; i < 3; ++i) {
//      out[i] = cell[i] * std::complex<double>(cos(cell[i + 3]), sin(cell[i + 3]));
//   }
//   return out;
//}

double Radial::XAxisDist(const std::string& latticeb, const LRL_Cell& b) const {
   const LRL_Cell a = m_baseCell;
   if (xAxisType == Xaxis::Polar) {
      if (xAxisReduced == XaxisReduced::reduced) {
         return RadialDist(a, LatticeConverter::NiggliReduceCell(latticeb, b));
      }
      return RadialDist(a, b);
   }
   else if (xAxisType == Xaxis::G6) {
      return (G6(a) - G6(LatticeConverter::NiggliReduceCell(latticeb, b))).norm();
   }
   else
   {
      throw (R"(undocumented type)");
   }
   return double();
}

void Radial::SetBaseCell(const std::string& lattice, const LRL_Cell& cell) {
   if (xAxisReduced == XaxisReduced::notreduced) {
      m_baseCell = cell;
      m_baseG6 = cell;
      m_lattice = lattice;
   }
   else
   {
      // need to reduce the cell
      const LRL_Cell red = LatticeConverter::NiggliReduceCell(lattice, cell);
      m_baseCell = red;
      m_baseG6 = red;
      m_lattice = "P";
   }
}

double Radial::YAxisDist(const std::string& latticeb, const LRL_Cell& b) const {
   const LRL_Cell& a = m_baseCell;
   const std::string& latticea = m_lattice;
   if (yAxisType == Yaxis::NCDist) {
      return NCDist(G6(LatticeConverter::NiggliReduceCell(latticea, a)).data(),
         G6(LatticeConverter::NiggliReduceCell(latticeb, b)).data());
   }
   else if (yAxisType == Yaxis::CS6Dist) {
      return CS6Dist(LatticeConverter::DeloneReduceCell(latticea, a).data(),
         LatticeConverter::DeloneReduceCell(latticeb, b).data());
   }
   else {
      throw (R"(not an allowed type)");
   }
}

double Radial::RadialDist(const LRL_Cell& a, const LRL_Cell& b) {

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

int main(int argc, char* argv[])
{
   Radial rad;
   rad.SetParameters(argc, argv);
   std::cout << "; Radial distances from first cell, radial dist (rd), ncdist (nc)" << std::endl;
   std::cout << "; Parameters: " << rad.GetRadialParameters() << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   rad.SetBaseCell(inputList[0].GetLattice(), inputList[0].GetCell());

   std::cout << "; first input cell  " << LRL_Cell_Degrees(rad.GetBaseCell()) << std::endl;

   for (size_t i = 0; i < inputList.size(); ++i) {
      const LRL_Cell currentCell = inputList[i].GetCell();
      const double xdist = rad.XAxisDist(inputList[i].GetLattice(), currentCell);
      const double ydist = rad.YAxisDist(inputList[i].GetLattice(), currentCell);

      std::cout << "rd " << xdist << "  nc " <<
         ydist << "  --  cell " <<
         LRL_Cell_Degrees(currentCell) << std::endl;
   }
}
