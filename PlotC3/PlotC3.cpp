// PlotC3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "C3Plot.h"
#include "LRL_ToString.h"
#include "LRL_DataToSVG.h"
#include "LRL_CreateFileName.h"
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

size_t scalar = 19191;

std::vector<std::string> allowed{ "seq","C3","RI" };


std::string PlotC3( const size_t scalar, const int wx, const int wy, const std::string& s) {
   const std::string sScaler = LRL_ToString(scalar);
   const std::string sScalerP3 = LRL_ToString(scalar+3);
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

   const std::string strScalar = LRL_DataToSVG(scalar);

   plotc3 += "<!--plot labeling-->"
      "\n"
      "<g transform = \"translate(0,-265)\" >\n"
      "<text x = \"-350\" y = \"-200\"  font-size = \"40\" font-family = \"Arial, Helvetica, sans-serif\" >Coordinate c</text>\n"
      "<g transform = \"translate(205,10)\">\n"
      "<text x = \"-328\" y = \"-200\"  font-size = \"30\" font-family = \"Arial, Helvetica, sans-serif\" >" + strScalar + " </text >\n"
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
   const int wx = 2000, const int wy = 2000, const std::string& s = "") {
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
   std::string out;
   const std::string xtext = LRL_ToString(x);
   const std::string ytext = LRL_ToString(y);
   const std::string sourceFile(__FILE__);
   const std::string sxlabel = LRL_ToString(x + 20);
   const std::string sylabel = LRL_ToString(y + 50);

   const std::string sydate = LRL_ToString(y+100);
   const std::string sycount = LRL_ToString(y + 150);

   const std::string fileLabel = "<text x=\"" + sxlabel + "\" y=\""+ sylabel+"\" >"
      + sourceFile + " </text>\n";

   const std::string count = "<text x=\"" + sxlabel + "\" y=\""
      + sycount + "\" > " + "Number of points: " + LRL_ToString(v.size()) + "</text>\n";

   const std::string rect = 
      "<rect x=\""+xtext+"\" y= \"" + ytext + "\" width = \"500\"" +" height=\"500\" fill=\"none\" "
      "stroke=\"black\" stroke-width=\"2\" />\n";

   const std::string dateTime = LRL_CreateFileName::Create("Created ", "");
   const std::string datePos = LRL_ToString(y + 460);
   const std::string date = "<text x=\"" + sxlabel + "\" y=\"" + datePos + "\">" + dateTime + "</text>\n";

   out += fileLabel;
   out += date;
   out += rect;
   out += count;

   if (v.empty()) {
      std::cout << "; PlotC3 requires at least one lattice" << std::endl;
   }
   else if (v.size() < 5) {
      // just list them all
      for (size_t i = 0; i < v.size(); ++i) {
         const std::string ypos = LRL_ToString(y + 200 + i * 20);
         const std::string cellText =
            "<text x=\"" + sxlabel + "\" y=\""
            + ypos + "\" > " + "P " + CellPrecision2(v[i]) + "</text>\n";
         out += cellText;
      }
   }
   else {
      // just list the first 3 and the last one
      for (size_t i = 0; i < 3; ++i) {
         const std::string ypos = LRL_ToString(y + 200 + i * 20);
         const std::string cellText1 =
            "<text x=\"" + sxlabel + "\" y=\""
            + ypos + "\" > " + "P " + CellPrecision2(v[i]) + "</text>\n";

         out += cellText1;
         out += "...\n";
      }
      const std::string ypos = LRL_ToString(y + 280);
      std::stringstream os;
      os << CellPrecision2(v[v.size() - 1]);

      const std::string cellText2 =
         "<text x=\"" + sxlabel + "\" y=\""
         + ypos + "\" > " + "P " + os.str() + "</text>\n";
      const std::string dots =
         "<text x=\"" + sxlabel + "\" y=\""
         + LRL_ToString(y + 255) + "\" > " + "\n ...\n</text>\n";
      out += dots;
      out += cellText2;

   }
   const std::string ypos = LRL_ToString(y + 3400);
   const std::string commandLine =
      "<text x=\"" + sxlabel + "\" y=\""
      + LRL_ToString(y + 355) + "\" > " + "\n edit SVG file to insert command line text</text>\n";

   //out += commandLine;

   return out;
}

int main(int argc, char* argv[])
{
   const std::string filename = LRL_CreateFileName::Create("PLT_", "svg");

   C3Plot c3plot(filename, 2000, 2000, 500, 500);

   std::string svgOutput;
   const std::string intro = c3plot.GetIntro(filename);
   svgOutput += intro;

   const std::vector<S6> v = PrepareCells();
   const std::string legend = PrepareLegend(600, 600, v);
   svgOutput += legend;

   for (size_t scalar = 1; scalar < 4; ++scalar) {

      std::string line;
      std::string cells;

      line += c3plot.CreatePolylineFromPoints(scalar, ".5", v);
      cells += c3plot.DrawCells(scalar, v);

      std::string plotc3;
      if (scalar == 1)
         plotc3 = PlotC3(scalar, 500, 500, line + "  " + cells);
      if (scalar == 2)
         plotc3 = PlotC3(scalar, 1100, 500, line + "  " + cells);
      if (scalar == 3)
         plotc3 = PlotC3(scalar, 500, 1100, line + "  " + cells);

      svgOutput += plotc3;;
   }

   std::cout << "; Graphical output SVG file =" + filename << std::endl;
   c3plot.SendFrameToFile(filename, svgOutput + c3plot.GetFoot());
}
