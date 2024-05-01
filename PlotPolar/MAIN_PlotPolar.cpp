
#include "ColorTables.h"
#include "GetDate.h"
#include "LRL_DataToSVG.h"
#include "LRL_CreateFileName.h"
#include "LRL_MinMaxTools.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "PlotPolar.h"
#include "Polar.h"
#include "LRL_Vector3.h"
#include "Vector_2.h"
#include "WebIO.h"


#include <algorithm>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

static std::string MakePlotPolar(const size_t whichPlot, const int wx, const int wy, const std::string& s) {
   const std::string sScaler = std::to_string(whichPlot);
   const std::string sScalerP3 = std::to_string(whichPlot + 3);
   //<g transform = "translate(150,975) scale(1 -1)">
   const std::string x1 = LRL_DataToSVG_ToQuotedString(std::min(wx, wy));
   const std::string y1 = LRL_DataToSVG_ToQuotedString(0);
   const std::string x2 = LRL_DataToSVG_ToQuotedString(0);
   const std::string y2 = LRL_DataToSVG_ToQuotedString(-std::min(wx, wy));
   const std::string x3 = LRL_DataToSVG_ToQuotedString(-std::min(wx, wy));
   const std::string y3 = LRL_DataToSVG_ToQuotedString(0);

   const std::string line1 = "\n<line x1=" + y1 + " y1 = " + y1 + " x2 =" + x2
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
      "<text x = \"-350\" y = \"-200\"  font-size = \"40\" font-family = \"Arial, Helvetica, sans-serif\" >Coordinate p</text>\n"
      "<g transform = \"translate(205,10)\">\n"
      "<text x = \"-325\" y = \"-200\"  font-size = \"30\" font-family = \"Arial, Helvetica, sans-serif\" >" + strScalar + " </text >\n"
      "</g>\n"
      "</g>\n"


      "<g transform = \"translate(0,0)\" >\n"
      "<text   x = \"-480\" y = \"-20\"  font-size = \"40\" font-family = \"Arial, Helvetica, sans-serif\" > -x </text>\n"
      "<g transform = \"translate(30,10)\">\n"
      "<text  x = \"-480\" y = \"-20\"  font-size = \"30\" font-family = \"Arial, Helvetica, sans-serif\" >" + sScaler + "</text >\n"
      "</g>\n"
      "</g>\n"

      "<g transform = \"translate(0,0)\" >\n"
      "<text  x = \"-60\" y = \"-450\" font-size = \"40\" font-family = \"Arial, Helvetica, sans-serif\" > y </text>\n"
      "<g transform = \"translate(30,10)\">\n"
      "<text x = \"-60\" y = \"-450\"  font-size = \"30\" font-family = \"Arial, Helvetica, sans-serif\" >" + sScaler + "</text >\n"
      "</g>\n"
      "</g>\n"
      "</g>\n"
   "<g transform = \"translate(0,0)\" >\n"
      "<text   x = \"360\" y = \"-20\"  font-size = \"40\" font-family = \"Arial, Helvetica, sans-serif\" > x </text>\n"
      "<g transform = \"translate(30,10)\">\n"
      "<text  x = \"360\" y = \"-20\"  font-size = \"30\" font-family = \"Arial, Helvetica, sans-serif\" >" + sScaler + "</text >\n"
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

std::vector<Polar> ConvertInputToPolar(const std::vector<LRL_ReadLatticeData>& inputList) {
   std::vector<Polar> v;

   for (size_t i = 0; i < inputList.size(); ++i) {
      v.emplace_back(Polar(inputList[i].GetCell()));
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
static std::string PrepareLegend(const double x, const double y, const std::vector<Polar>& v) {

   //out += commandLine;
   std::string out;

   const std::string count = "; Number of points: " + std::to_string(v.size()) + "\n\n";

   out += count;

   if (v.empty()) {
      std::cout << "; PlotPolar requires at least one lattice" << std::endl;
   }

   return out;
}

static std::pair<double, double> GetMinMaxPolar(const std::vector<Polar>& v) {
   double mins6 = DBL_MAX;
   double maxs6 = -DBL_MAX;

   for (const auto& vv : v) {
      mins6 = minNC(mins6, vv[0][0], vv[0][1]);
      maxs6 = maxNC(maxs6, vv[0][0], vv[0][1]);
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
      << "\"  font-size = \"20\" font-family = \"Arial, Helvetica, sans-serif\" >LRL-WEB  PlotPolar   "
      << GetDate()
      << " </text>\n";
   s += os.str();;
   return s;
}

std::string  PrepareColorGuide(const PlotPolar& c3plot, const int xint, const int yint) {
   unsigned long r;
   unsigned long g;
   unsigned long b;
   const size_t nguides = 9;
   std::string out;

   for (size_t i = 0; i <= nguides; ++i)
   {
      const double frac = double(i) / double(nguides - 1);
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
   std::cout << "; PlotPolar" << std::endl;

   WebIO webio(argc, argv, "PlotPolar", 1);
   webio.GetWebBlockSize(argc, argv);
   webio.CreateFilenamesAndLinks(1, "PPL");
   
   const size_t& blockstart = webio.m_blockstart;
   const size_t& blocksize = webio.m_blocksize;

   const std::vector<std::string>& basicfileNameList = webio.m_basicfileNameList;
   const std::vector<std::string>& RawFileNameList = webio.m_FileNameList;
   const std::vector<std::string>& FullfileNameList = webio.m_FullfileNameList;
   const std::string& filename = basicfileNameList[0];
   const std::string& graphicsFileName = RawFileNameList[0];

   //std::cout << "; PlotPolar cell block start " << blockstart << std::endl;
   //std::cout << "; PlotPolar cell block size " << blocksize << std::endl;

   for (size_t i = blockstart; (i < blockstart + blocksize); ++i)
   {
      std::cout << "; PlotPolar graphics file(s) " <<
         i + 1 << "  " << FullfileNameList[i - blockstart] << std::endl;
   }

   LRL_ReadLatticeData reader;
   const std::vector<LRL_ReadLatticeData> inputList = reader.ReadLatticeData();

   PlotPolar plplot(filename, 1250, 750, 500, 500);

   std::string svgOutput;
   const std::string intro = plplot.GetIntro(filename);
   svgOutput += intro;

   std::vector<Polar> vPolar = ConvertInputToPolar(inputList);
   std::pair<double, double> minmaxPolar = GetMinMaxPolar(vPolar);
   if (abs(minmaxPolar.second) < 1.0E-5) minmaxPolar.second = 0.0;
   const std::string dataRange = LRL_ToString("; The Polar data value range is ", minmaxPolar.first, " to ", minmaxPolar.second);
   const std::string legend = AddTextAtBottom(1150, 800, dataRange) +
      PrepareColorGuide(plplot, 1150, 750);

   svgOutput += legend;
   svgOutput += MakePlotPolar(1, 500, 500, "  " + plplot.DrawCells(1, vPolar));
   svgOutput += MakePlotPolar(2, 1550, 500, "  " + plplot.DrawCells(2, vPolar));
   svgOutput += MakePlotPolar(3, 500, 1100, "  " + plplot.DrawCells(3, vPolar));

   std::cout << dataRange << std::endl << std::endl;
   ListInput(inputList);
   plplot.SendFrameToFile(graphicsFileName, svgOutput + plplot.GetFoot());
   std::cout << PrepareLegend(600, 600, vPolar);
}
