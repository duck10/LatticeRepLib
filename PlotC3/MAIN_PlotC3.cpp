// PlotC3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "C3Plot.h"
#include "ColorTables.h"
#include "LRL_ToString.h"
#include "LRL_DataToSVG.h"
#include "LRL_CreateFileName.h"
#include "LRL_MinMaxTools.h"
#include "LRL_ReadLatticeData.h"
#include "S6.h"
#include "LRL_Vector3.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

std::string PlotC3( const size_t whichPlot, const int wx, const int wy, const std::string& s) {
   const std::string sScaler = LRL_ToString(whichPlot);
   const std::string sScalerP3 = LRL_ToString(whichPlot+3);
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

std::string WrapSVG(const std::string& fileName,
   const int wx = 1400, const int wy = 600, const std::string& s = "") {
   const std::string swx = LRL_DataToSVG_ToQuotedString(wx);
   const std::string swy = LRL_DataToSVG_ToQuotedString(wy);


   return    "\n\n" + s + "." + "</g>";
}

std::vector<S6> PrepareCells() {
   std::vector<S6> v;
   LRL_ReadLatticeData reader;
   const std::vector<LRL_ReadLatticeData> inputList = reader.ReadLatticeData();
   for (size_t i = 0; i < inputList.size(); ++i) {
      v.push_back(S6(inputList[i].GetCell()));
   }
   for (size_t i = 0; i < std::min(size_t(5), inputList.size()); ++i) {
      std::cout << inputList[i].GetLattice() << "  " << LRL_Cell_Degrees(inputList[i].GetCell()) << std::endl;
   }

   if (inputList.size() > 5) {
      std::cout << " ..." << std::endl;
      for (size_t i = inputList.size() - 3; i < inputList.size(); ++i) {
         std::cout << inputList[i].GetLattice() << "  " << LRL_Cell_Degrees(inputList[i].GetCell()) << std::endl;
      }

   }
   return v;
}

std::string CellPrecision2(const S6& s) {
   std::stringstream os;
   const LRL_Cell_Degrees cell(s);
   os << std::fixed << std::setprecision(2);
   for (size_t i = 0; i < 6; ++i) {
      os << cell[i] << "  ";
   }
   return os.str();
}

std::string PrepareLegend(const double x, const double y, const std::vector<S6>& v) {

   //out += commandLine;
   std::string out;
   const std::string xtext = LRL_ToString(x);
   const std::string ytext = LRL_ToString(y);
   const std::string sourceFile(__FILE__);

   const std::string fileLabel = sourceFile + "\n";

   const std::string count =  "Number of points: " + LRL_ToString(v.size()) + "\n\n";

   const std::string date = LRL_CreateFileName::Create("Created ", "\n");

   out += fileLabel;
   out += date;
   //   out += rect;
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
   const std::string ypos = LRL_ToString(y + 3400);
   const std::string commandLine = LRL_ToString("\n edit SVG file to insert command line text\n");

   std::cout << out << std::endl;
   return "";
}

std::pair<double, double> GetMinMaxS6(const std::vector<S6>& v) {
   double mins6 = DBL_MAX;
   double maxs6 = -mins6;

   for (size_t i = 0; i < v.size(); ++i) {
      const S6& vv = v[i];
      mins6 = minNC(vv[0], vv[1], vv[2], vv[3], vv[4], vv[5]);
      maxs6 = maxNC(vv[0], vv[1], vv[2], vv[3], vv[4], vv[5]);
   }
   return { mins6,maxs6 };
}

int main(int argc, char* argv[])
{
   std::cout << "; PlotC3" << std::endl;
   const std::string filename = LRL_CreateFileName::Create("PLT_", "svg");

   C3Plot c3plot(filename, 1400, 600, 500, 500);

   std::string svgOutput;
   const std::string intro = c3plot.GetIntro(filename);
   svgOutput += intro;

   const std::vector<S6> v = PrepareCells();
   const std::string legend = PrepareLegend(600, 600, v);
   const std::pair<double, double> minmax = GetMinMaxS6(v);
   svgOutput += legend;

   for (size_t whichPlot = 1; whichPlot < 4; ++whichPlot) {

      std::string line;
      std::string cells;

      line += c3plot.CreatePolylineFromPoints(whichPlot, ".5", v);
      cells += c3plot.DrawCells(whichPlot, v);

      std::string plotc3;
      if (whichPlot == 1)
         plotc3 = PlotC3(whichPlot, 500, 500, line + "  " + cells);
      if (whichPlot == 2)
         plotc3 = PlotC3(whichPlot, 1100, 500, line + "  " + cells);
      if (whichPlot == 3)
         plotc3 = PlotC3(whichPlot, 1700, 500, line + "  " + cells);

      svgOutput += plotc3;;
   }

   std::cout << "The S6 data range is " << minmax.first << " to " << minmax.second << std::endl;
   std::cout << "; Graphical output SVG file = " + filename << std::endl;
   c3plot.SendFrameToFile(filename, svgOutput + c3plot.GetFoot());
}

