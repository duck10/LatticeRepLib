#include "C3Plot.h"
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

double C3Plot::CellScale(const std::vector<S6>& v) {
   double maxs = -DBL_MAX;
   for (size_t i = 0; i < v.size(); ++i) {
      for (size_t kk = 0; kk < 6; ++kk) {
         maxs = fmax(maxs, -v[i][kk]);
      }
   }
   m_maxScalar = maxs;
   return maxs;
}

double C3Plot::CellScaleFactor() const {
   return 0.9 * m_gx / m_maxScalar;
}

std::string C3Plot::CreatePolylineFromPoints(const size_t scalar, const std::string& width, const std::vector<S6>& v) {
   CellScale(v);
   const double cellScale = CellScaleFactor();
   const ScalarProperties sp(scalar);
   std::string transform = "<g transform = \"translate( "
      + LRL_DataToSVG(0) + ", " + LRL_DataToSVG(0)
      + ")\" >\n";

   const std::string dashMode("");
   const std::string strokeWidth = "\" stroke-width=\"" + width + "\"";
   std::string svg = transform + LRL_DataToSVG("   <polyline fill=\"none\" stroke=\"", "#A3A3A3", strokeWidth, "  points=\" ");
   for (size_t i = 0; i < v.size(); ++i) {
      const double& s1 = v[i][sp.index1];
      const double& s2 = v[i][sp.index2];
      svg += LRL_DataToSVG(s1 * cellScale) + " " + LRL_DataToSVG(s2 * cellScale) + ",  ";
   }

   svg += "\"/> \n\n";
   svg += "</g> <!--End of transform -->\n\n";
   return svg;
}

std::string C3Plot::DrawCells(const size_t scalar, const std::vector<S6>& v) {
   CellScale(v);
   const double cellScale = CellScaleFactor();
   const ScalarProperties sp(scalar);
   // colors suggested by HJB
   //   yellow to blue(#FFFF00 to #0000FF)
   //   orange to dark cyan(#FFA500 to #008B8B)
   //   chartreuse to deep sky blue(#7FFF00 to ##00BFFF)   const ColorRange colRange(0xFFFF00, 0x0000FF); //#FFFF00 to #0000FF

   //   #f0f9e8: Alice blue
   //   #bae4bc : Mint green
   //   #7bccc4: Sea green
   //   #43a2ca: Medium turquoise
   //   #0868ac: Denim
   //   #1F45FC Blue Orchid
   //   #1589FF Neon Blue

   //const ColorRange colRange(0xFFA500, 0x008B8B); // not so nice
   //const ColorRange colRange(0xFFFF00, 0x0000FF); // nice yellow to blue
   //const ColorRange colRange(0x7FFF00, 0xFFA500); // should be yellow to orange
   //const ColorRange colRange(0xFFFF00, 0x0000FF); // should be yellow to orange
   //const ColorRange colRange(0xFFFF00, 0x00BFFF); // should be yellow to deep sky blue
   //const ColorRange colRange(0xFFFF00, 0x1589FF); // should be yellow to Neon Blue
   //const ColorRange colRange(0xFFFF00, 0x1589FF); // should be yellow to Neon Blue

   const std::string scale = LRL_ToString(1);
   std::string transform = "<g transform = \"translate( "
      + LRL_DataToSVG(0) + ", " + LRL_DataToSVG(0)
      + ")\" >\n" + "\n";
   for (size_t i = 0; i < v.size(); ++i) {
      const std::string x = LRL_DataToSVG_ToQuotedString(v[i][sp.index1] * cellScale);
      const std::string y = LRL_DataToSVG_ToQuotedString(v[i][sp.index2] * cellScale);
      const std::string radius = LRL_DataToSVG_ToQuotedString(3);

      //const int color = colRange.GetColorFromRangeFraction(double(i) / double(v.size()));
      unsigned long r;
      unsigned long g;
      unsigned long b;
      const double frac = double(i) / double(v.size());
      m_colRange.GetRGBFromRangeFraction(frac, r, g, b);

      //// fill="rgb(255, 0, 0)"
      //const std::string hexColor = NumberToHexString(color);

      //// Convert hex digits to integers
      //int red, green, blue;
      //if (hexColor.length() == 6) {
      //   red = std::stoi(hexColor.substr(0, 2), nullptr, 16);
      //   green = std::stoi(hexColor.substr(2, 2), nullptr, 16);
      //   blue = std::stoi(hexColor.substr(4, 2), nullptr, 16);
      //}
      //else {
      //   red = std::stoi(hexColor.substr(0, 1), nullptr, 16) * 16 + std::stoi(hexColor.substr(1, 1), nullptr, 16);
      //   green = std::stoi(hexColor.substr(1, 1), nullptr, 16) * 16 + std::stoi(hexColor.substr(2, 1), nullptr, 16);
      //   blue = std::stoi(hexColor.substr(2, 1), nullptr, 16) * 16 + std::stoi(hexColor.substr(2, 1), nullptr, 16);
      //   blue /= 2; // Divide blue by 2 to handle shorthand hex format
      //}
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


C3Plot::C3Plot(const std::string& filename, const int wx, const int wy, const int gx, const int gy)
   : m_wx(wx)
   , m_wy(wy)
   , m_gx(gx)
   , m_gy(gy)
   , m_svgIntro(BuildIntro(filename))
   , m_svgFoot("\n</g> <!--end of scale=0.6-->\n</svg>")
   , m_colRange(0xFFFF00, 0x1589FF) // should be yellow to Neon Blue
{
}

std::string C3Plot::BuildIntro(const std::string& filename) const {
   const std::string swx = LRL_DataToSVG_ToQuotedString(m_wx);
   const std::string swy = LRL_DataToSVG_ToQuotedString(m_wy);

   return
   "<svg width=" + swx + " height=" + swy +
      " viewBox=\"1 0  " + LRL_DataToSVG(m_wx, " ", m_wy) +
      "\"  version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" \nxmlns:xlink=\"http://www.w3.org/1999/xlink\">" +
      "\n<desc>File name  = " + filename + " </desc> """ +
      "\n <g transform=\"scale(0.6)\">\n";
}


std::vector<S6> C3Plot::PrepareCells() const {
   std::vector<S6> v;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   for (size_t i = 0; i < inputList.size(); ++i) {
      v.emplace_back(S6(inputList[i].GetCell()));
   }
   return v;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void C3Plot::SendFrameToFile(const std::string& sFileName, const std::string& data) const {
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
