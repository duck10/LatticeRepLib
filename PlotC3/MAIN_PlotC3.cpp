// PlotC3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "C3Plot.h"
#include "ColorTables.h"
#include "GetDate.h"
#include "LRL_DataToSVG.h"
#include "LRL_CreateFileName.h"
#include "LRL_MinMaxTools.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "LRL_Vector3.h"
#include "S6.h"
#include "Vector_2.h"
#include "WebIO.h"

#include <algorithm>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

static std::string PlotC3( const size_t whichPlot, const int wx, const int wy, const std::string& s) {
   const std::string sScaler = std::to_string(whichPlot);
   const std::string sScalerP3 = std::to_string(whichPlot+3);
   //<g transform = "translate(150,975) scale(1 -1)">
   const std::string x1 = LRL_DataToSVG_ToQuotedString(-0);
   const std::string y1 = LRL_DataToSVG_ToQuotedString(0);
   const std::string x2 = LRL_DataToSVG_ToQuotedString(0);
   const std::string y2 = LRL_DataToSVG_ToQuotedString(-std::min(wx, wy));
   const std::string x3 = LRL_DataToSVG_ToQuotedString(-std::min(wx,wy));
   const std::string y3 = LRL_DataToSVG_ToQuotedString(0);

   const std::string line1 = "\n<line x1=" + x1 + " y1 = " + y1 + " x2 =" + x2
      + " y2 = " + y2 + " stroke-width = \"2\" stroke=\"black\" > </line>";
   const std::string line2 = "\n<line x1=" + x1 + " y1 = " + y1 + " x2 =" + x3
      + " y2 = " + y3 + " stroke-width = \"2\" stroke=\"black\" > </line>";

   std::string plotc3 = LRL_DataToSVG("<g transform = \"translate( ", wx, ",", wy, ")\">\n",
      line1 + "\n" +
      line2, "\n\n") + s;

   const std::string strScalar = LRL_DataToSVG(whichPlot);

   plotc3 += "<!--plot labeling-->"
      "\n"
      "<g transform = \"translate(0,-265)\" >\n"
      "<text x = \"-350\" y = \"-200\"  font-size = \"40\" font-family = \"Arial, Helvetica, sans-serif\" >Coordinate c</text>\n"
      "<g transform = \"translate(205,10)\">\n"
      "<text x = \"-325\" y = \"-200\"  font-size = \"30\" font-family = \"Arial, Helvetica, sans-serif\" >" + strScalar + " </text >\n"
      "</g>\n"
      "</g>\n"


      "<g transform = \"translate(0,0)\" >\n"
      "<text   x = \"-480\" y = \"-20\"  font-size = \"40\" font-family = \"Arial, Helvetica, sans-serif\" > -s </text>\n"
      "<g transform = \"translate(30,10)\">\n"
      "<text  x = \"-480\" y = \"-20\"  font-size = \"30\" font-family = \"Arial, Helvetica, sans-serif\" >"+sScaler+"</text >\n"
      "</g>\n"
      "</g>\n"

      "<g transform = \"translate(0,0)\" >\n"
      "<text  x = \"-60\" y = \"-450\" font-size = \"40\" font-family = \"Arial, Helvetica, sans-serif\" > -s </text>\n"
      "<g transform = \"translate(30,10)\">\n"
      "<text x = \"-60\" y = \"-450\"  font-size = \"30\" font-family = \"Arial, Helvetica, sans-serif\" >"+sScalerP3+"</text >\n"
      "</g>\n"
      "</g>\n"
   "</g>\n";

   return plotc3;;
}

static std::string WrapSVG(const std::string& fileName,
   const int wx = 1400, const int wy = 600, const std::string& s = "") {
   const std::string swx = LRL_DataToSVG_ToQuotedString(wx);
   const std::string swy = LRL_DataToSVG_ToQuotedString(wy);


   return    "\n\n" + s + "." + "</g>";
}

static void ListInput(const std::vector<LRL_ReadLatticeData>& inputList) {
   for (size_t i = 0; i < std::min(size_t(5), inputList.size()); ++i) {
      std::cout << inputList[i].GetLattice() << "  " << LRL_Cell_Degrees(inputList[i].GetCell()) << std::endl;
   }

   if (inputList.size() > 5) {
      std::cout << " ..." << std::endl;
      for (size_t i = inputList.size() - 3; i < inputList.size(); ++i) {
         std::cout << inputList[i].GetLattice() << "  " << LRL_Cell_Degrees(inputList[i].GetCell()) << std::endl;
      }

   }
}

static std::vector<S6> ConvertInputToS6(const std::vector<LRL_ReadLatticeData>& inputList) {
   std::vector<S6> v;

   for (size_t i = 0; i < inputList.size(); ++i) {
      v.push_back(S6(inputList[i].GetCell()));
   }
   return v;
}

static std::string CellPrecision2(const S6& s) {
   std::stringstream os;
   const LRL_Cell_Degrees cell(s);
   os << std::fixed << std::setprecision(2);
   for (size_t i = 0; i < 6; ++i) {
      os << cell[i] << "  ";
   }
   return os.str();
}

static std::string PrepareLegend(const double x, const double y, const std::vector<S6>& v) {

   //out += commandLine;
   std::string out;

   const std::string count =  "; Number of points: " + std::to_string(v.size()) + "\n\n";

   out += count;

   if (v.empty()) {
      std::cout << "; PlotC3 requires at least one lattice" << std::endl;
   }
   else if (v.size() < 5) {
      // just list them all
      for (size_t i = 0; i < v.size(); ++i) {
         const std::string cellText =  "P " + CellPrecision2(v[i]) + "\n";
         out += cellText;
      }
   }
   else {
      // just list the first 3 and the last one
      for (size_t i = 0; i < 3; ++i) {
         const std::string cellText1 = "P " + CellPrecision2(v[i]);

            out += cellText1 + "\n";
      }
      std::stringstream os;
      os << CellPrecision2(v[v.size() - 1]);

      const std::string dots = "\n ...\n\n";

      const std::string cellText2 = "  P " + os.str() + "\n";
      out += dots;
      out += cellText2;
   }
   const std::string ypos = std::to_string(y + 3400);
   const std::string commandLine = "\n edit SVG file to insert command line text\n";

   return out;
}

static std::pair<double, double> GetMinMaxS6(const std::vector<S6>& v) {
   double mins6 = DBL_MAX;
   double maxs6 = -DBL_MAX;

   for (size_t i = 0; i < v.size(); ++i) {
      const S6& vv = v[i];
      mins6 = minNC(mins6, vv[0], vv[1], vv[2], vv[3], vv[4], vv[5]);
      maxs6 = maxNC(maxs6, vv[0], vv[1], vv[2], vv[3], vv[4], vv[5]);
   }
   return { mins6,maxs6 };
}

static std::string AddTextAtBottom(const int x, const int y, const std::string& dataRange) {
   std::string s = "<text x = \"" + LRL_DataToSVG(x) + "\" y = \"" + LRL_DataToSVG(y) + "\""
      " font-size = \"20\" " +
      " font-family = \"Arial, Helvetica, sans-serif \">" + LRL_DataToSVG(dataRange) + "</text>\n";
   //
   //
   // the next line has a blank. It is a place to add a comment such as the command line to generate the plot
   //
   //
   s += "<!--#######################################################-->\n"
      "<text x = \"" + LRL_DataToSVG(x) + "\" y = \"" + LRL_DataToSVG(y + 80) + "\""
      " font-size = \"20\" " +
      " font-family = \"Arial, Helvetica, sans-serif \">\n" +
      ""  // INSERT TEXT HERE
      + " \n</text>   "
      "<!-- add a comment such as the command line-->\n"
      "<!--#######################################################-->\n";

   std::ostringstream os;
   os << "<text x = \""
      << x
      << "\" y = \""
      << y + 40
      << "\"  font-size = \"20\" font-family = \"Arial, Helvetica, sans-serif\" >LRL-WEB  PlotC3   "
      << GetDate()
      << " </text>\n";
   s += os.str();;
   return s;
}

std::string  PrepareColorGuide(const C3Plot& c3plot, const int xint, const int yint) {
   unsigned long r;
   unsigned long g;
   unsigned long b;
   const size_t nguides = 9;
   std::string out;

   for ( size_t i=0; i<=nguides; ++i ) 
   {
      const double frac = double(i) / double(nguides-1);
      c3plot.GetColorRange().GetRGBFromRangeFraction(frac, r, g, b);
      const std::string circle = "";
      const std::string x = std::to_string(xint + i * 15);
      const std::string y = std::to_string(yint);
      const std::string s = std::string("<circle  r=\"12\" stroke = \"black\" stroke-width=\"0.5\""
         " fill= \"rgb(" + std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + ")\""
         " cx=\"" + x + "\""
         " cy=\"" + y + "\"/>\n");
      out += s;
   }
   return out;
}

int main(int argc, char* argv[])
{
   std::cout << "; PlotC3" << std::endl;

   WebIO webio(argc, argv, "PlotC3", 1);
   webio.GetWebBlockSize(argc, argv);
   webio.CreateFilenamesAndLinks(1, "PLT");

   const size_t& blockstart = webio.m_blockstart;
   const size_t& blocksize = webio.m_blocksize;

   const std::vector<std::string>& basicfileNameList = webio.m_basicfileNameList;
   const std::vector<std::string>& RawFileNameList = webio.m_FileNameList;
   const std::vector<std::string>& FullfileNameList = webio.m_FullfileNameList;
   const std::string& filename = basicfileNameList[0];
   const std::string& graphicsFileName = RawFileNameList[0];

   //std::cout << "; PlotC3 cell block start " << blockstart << std::endl;
   //std::cout << "; PlotC3 cell block size " << blocksize << std::endl;

   for (size_t i = blockstart; (i < blockstart + blocksize); ++i)
   {
      std::cout << "; PlotC3 graphics file(s) " <<
         i+1 << "  " << FullfileNameList[i - blockstart] << std::endl;
   }

   LRL_ReadLatticeData reader;
   const std::vector<LRL_ReadLatticeData> inputList = reader.ReadLatticeData();
  
   C3Plot c3plot(filename, 1050, 380, 500, 500);

   std::string svgOutput;
   const std::string intro = c3plot.GetIntro(filename);
   svgOutput += intro;

   const std::vector<S6> vS6 = ConvertInputToS6(inputList);
   std::pair<double, double> minmax = GetMinMaxS6(vS6);
   if (abs(minmax.second) < 1.0E-5) minmax.second = 0.0;
   const std::string dataRange = LRL_ToString("; The S6 data range is ", minmax.first, " to ", minmax.second);
   const std::string legend = AddTextAtBottom(350, 550, dataRange) +
      PrepareColorGuide(c3plot, 850, 550);

   svgOutput += legend;

   if (bool drawConnectingLines = false)
   {
      svgOutput += PlotC3(1, 500, 500, c3plot.CreatePolylineFromPoints(1, ".5", vS6));
      svgOutput += PlotC3(2, 1100, 500, c3plot.CreatePolylineFromPoints(2, ".5", vS6));
      svgOutput += PlotC3(3, 1700, 500, c3plot.CreatePolylineFromPoints(3, ".5", vS6));
   }

   svgOutput += PlotC3(1, 500, 500, "  " + c3plot.DrawCells(1, vS6));
   svgOutput += PlotC3(2, 1100, 500, "  " + c3plot.DrawCells(2, vS6));
   svgOutput += PlotC3(3, 1700, 500, "  " + c3plot.DrawCells(3, vS6));

   std::cout << dataRange << std::endl << std::endl;
   ListInput(inputList);
   c3plot.SendFrameToFile(graphicsFileName, svgOutput + c3plot.GetFoot());
   std::cout << PrepareLegend(600, 600, vS6);
}
