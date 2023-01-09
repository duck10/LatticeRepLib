#include "C3Plot.h"
#include "FileOperations.h"
#include "LRL_CreateFileName.h"
#include "LRL_DataToSVG.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "S6.h"

#include <cmath>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>


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

double C3Plot::CellScaleFactor() {
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
   //const std::string defs = "<defs>\n	<circle id = \"circ\"  r=\"5\" stroke = \"black\" stroke-width=\".6\"  />\n</defs>\n";

   const std::string scale = LRL_ToString(1);
   std::string transform = "<g transform = \"translate( "
      + LRL_DataToSVG(0) + ", " + LRL_DataToSVG(0)
      +")\" >\n" +  "\n";
   for (size_t i = 0; i < v.size(); ++i) {
      const std::string x = LRL_DataToSVG_ToQuotedString(v[i][sp.index1]*cellScale);
      const std::string y = LRL_DataToSVG_ToQuotedString(v[i][sp.index2]*cellScale);
      const std::string radius = LRL_DataToSVG_ToQuotedString( 3);

      const std::string s = std::string("<circle fill=\"none\"  r=\"5\" stroke = \"black\" stroke-width=\"2\"" 
         " cx=" + x +
         " cy=" + y + "/>\n" );
      transform += s;


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
{
}

std::string C3Plot::BuildIntro(const std::string& filename) {
   const std::string swx = LRL_DataToSVG_ToQuotedString(m_wx);
   const std::string swy = LRL_DataToSVG_ToQuotedString(m_wy);

   return
   "<svg width=" + swx + " height=" + swy +
      " viewBox=\"1 0  " + LRL_DataToSVG(m_wx * 1.2, " ", m_wy * 1.2) +
      "\"  version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" \nxmlns:xlink=\"http://www.w3.org/1999/xlink\">" +
      "\n<desc>File name  = " + filename + " </desc> """ +
      "\n <g transform=\"scale(0.6)\">\n";
}


std::vector<S6> C3Plot::PrepareCells() {
   std::vector<S6> v;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   for (size_t i = 0; i < inputList.size(); ++i) {
      v.push_back(S6(inputList[i].GetCell()));
   }
   return v;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void C3Plot::SendFrameToFile(const std::string& sFileName, const std::string& data) {
   std::ofstream fileout;
   FileOperations::OpenOutputFile(fileout, sFileName.c_str());

   if (fileout.is_open())
   {
      fileout.seekp(0);
         fileout << data << std::endl;
   }
   else
      std::cout << "Could not open file " << sFileName << " for write in SVGWriter.h" << std::endl;

   fileout.close();
}
