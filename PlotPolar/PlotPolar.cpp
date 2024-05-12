#include "PlotPolar.h"
#include "ColorTables.h"
#include "FileOperations.h"
#include "LRL_CreateFileName.h"
#include "LRL_DataToSVG.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "S6.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

static std::string NumberToHexString(const int n) {
   std::stringstream ostr;
   ostr << std::hex << std::setfill('0') << std::setw(6) << n;
   return ostr.str();
}

double PlotPolar::CellScale(const std::vector<Polar>& v) {
   double maxs = -DBL_MAX;
   for (size_t i = 0; i < v.size(); ++i) {
      for (size_t kk = 0; kk < 3; ++kk) {
         maxs = fmax(maxs, fmax(abs(v[i][kk][0]), abs(v[i][kk][1])));
      }
   }
   m_maxScalar = maxs;
   return maxs;
}

double PlotPolar::CellScaleFactor() const {
   return 0.9 * m_gx / m_maxScalar;
}

std::string PlotPolar::DrawCells(const size_t scalar, const std::vector<Polar>& vPolar, const ColorRange& colRange) {
   CellScale(vPolar);
   const double cellScale = CellScaleFactor();
   const ScalarProperties sp(scalar);

   const std::string scale = LRL_ToString(1);
   std::string transform = "<g transform = \"translate( "
      + LRL_DataToSVG(0) + ", " + LRL_DataToSVG(0)
      + ") scale(1 -1)\" >\n" + "\n";
   for (size_t i = 0; i < vPolar.size(); ++i) {
      const Polar pi = vPolar[i];
      const Vector_2 si = pi[scalar - 1];
      const double& xcoord = si[0];
      const double& ycoord = si[1];
      const std::string x = LRL_DataToSVG_ToQuotedString(xcoord * cellScale);
      const std::string y = LRL_DataToSVG_ToQuotedString(ycoord * cellScale);
      const std::string radius = LRL_DataToSVG_ToQuotedString(3);

      unsigned long r;
      unsigned long g;
      unsigned long b;
      const double frac = (vPolar.size()==1) ? 1.0 : double(i) / double(vPolar.size() - 1);
      colRange.GetRGBFromRangeFraction(frac, r, g, b);

      const std::string s = std::string("<circle  r=\"10\" stroke = \"black\" stroke-width=\"0.5\""
         " fill= \"rgb(" + LRL_ToString(r) + ", " + LRL_ToString(g) + ", " + LRL_ToString(b) + ")\""
         " cx=" + x +
         " cy=" + y + "/>\n");
      transform += s;

      m_color += m_deltaColor % 256;
   }
   transform += "</g> <!--End of transform -->\n\n";
   return transform;
}


PlotPolar::PlotPolar(const std::string& filename, const int wx, const int wy, const int gx, const int gy)
   : m_wx(wx)
   , m_wy(wy)
   , m_gx(gx)
   , m_gy(gy)
   , m_svgIntro(BuildIntro(filename))
   , m_svgFoot("\n</g> <!--end of scale=0.6-->\n</svg>")
{
}

std::string PlotPolar::BuildIntro(const std::string& filename) const {
   const std::string swx = LRL_DataToSVG_ToQuotedString(m_wx);
   const std::string swy = LRL_DataToSVG_ToQuotedString(m_wy);

   return
      "<svg width=" + swx + " height=" + swy +
      " viewBox=\"1 0  " + LRL_DataToSVG(m_wx, " ", m_wy) +
      "\"  version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" \nxmlns:xlink=\"http://www.w3.org/1999/xlink\">" +
      "\n<desc>File name  = " + filename + " </desc> """ +
      "\n <g transform=\"scale(0.6)\">\n";
}

std::vector<S6> PlotPolar::PrepareCells() const {
   std::vector<S6> v;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   for (size_t i = 0; i < inputList.size(); ++i) {
      v.emplace_back(inputList[i].GetCell());
   }
   return v;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void PlotPolar::SendFrameToFile(const std::string& sFileName, const std::string& data) const {
   std::ofstream fileout;
   FileOperations::OpenOutputFile(fileout, sFileName.c_str());

   if (fileout.is_open())
   {
      fileout.seekp(0);
      fileout << data << std::endl;
   }
   else
      std::cout << "; Could not open file " << sFileName << " for write in SVGWriter.h" << std::endl;

   fileout.close();
}

