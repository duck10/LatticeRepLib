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
         double val1 = abs(v[i][kk][0]);
         double val2 = abs(v[i][kk][1]);
         double currentMax = fmax(val1, val2);
         maxs = fmax(maxs, currentMax);
      }
   }

   m_maxScalar = maxs;
   return maxs;
}

double PlotPolar::CellScaleFactor() const {
   return 0.9 * m_gx / m_maxScalar;
}

PlotPolar::PlotPolar(const std::string& filename, const int wx, const int wy, const int gx, const int gy)
   : m_wx(wx)
   , m_wy(wy)
   , m_gx(gx)
   , m_gy(gy)
   , m_maxScalar(-DBL_MAX)  // Initialize to minimum value for max-finding
   , m_svgIntro(BuildIntro(filename))
   , m_svgFoot("\n</g> <!--end of scale=0.6-->\n</svg>") {
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

