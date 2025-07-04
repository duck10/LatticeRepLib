#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "PlotPolarControls.h"
#include "ColorTables.h"
#include "GetDate.h"
#include "LRL_DataToSVG.h"
#include "LRL_CreateFileName.h"
#include "LRL_MinMaxTools.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "LRL_Vector3.h"
#include "PlotPolar.h"
#include "Plots.h"
#include "Polar.h"
#include "ProgramSetup.h"
#include "Vector_2.h"
#include "WebIO.h"
#include "IndividualPlot.h"

// Keep existing helper functions
static void ListInput(const std::vector<LatticeCell>& inputList) {
   for (size_t i = 0; i < std::min(size_t(5), inputList.size()); ++i) {
      std::cout << inputList[i].getLatticeType() << "  " << LRL_Cell_Degrees(inputList[i].getCell()) << std::endl;
   }

   if (inputList.size() > 5) {
      std::cout << " ..." << std::endl;
      for (size_t i = inputList.size() - 3; i < inputList.size(); ++i) {
         std::cout << inputList[i].getLatticeType() << "  " << LRL_Cell_Degrees(inputList[i].getCell()) << std::endl;
      }
   }
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

static std::string PrepareLegend(const double x, const double y, const size_t nData,
   const std::string& programName) {
   std::string out;
   const std::string count = "; Number of points: " + std::to_string(nData) + "\n\n";
   out += count;

   if (nData == 0) {
      std::cout << "; " + programName + " requires at least one lattice" << std::endl;
   }

   return out;
}

static std::string AddTextAtBottom(const int x, const int y, const std::string& dataRange,
   const std::string& programName) {
   std::string s = "<text x = \"" + LRL_DataToSVG(x) + "\" y = \"" + LRL_DataToSVG(y) + "\""
      " font-size = \"20\" " +
      " font-family = \"Arial, Helvetica, sans-serif \">" + LRL_DataToSVG(dataRange) + "</text>\n";

   s += "<!--#######################################################-->\n"
      "<text x = \"" + LRL_DataToSVG(x) + "\" y = \"" + LRL_DataToSVG(y + 80) + "\""
      " font-size = \"20\" " +
      " font-family = \"Arial, Helvetica, sans-serif \">\n" +
      ""  // INSERT TEXT HERE
      + " \n</text>   "
      "<!-- add a comment such as the command line-->\n"
      "<!--#######################################################-->\n"
      "<!--<text x=\"1150\" y=\"920\" font-size = \"20\"  font-family = \"Arial, Helvetica, sans - serif \"> REPLACEABLE</text>-->\n";

   std::ostringstream os;
   os << "<text x = \""
      << x
      << "\" y = \""
      << y + 40
      << "\"  font-size = \"20\" font-family = \"Arial, Helvetica, sans-serif\" >LRL-WEB  " + programName + "   "
      << GetDate()
      << " </text>\n";
   s += os.str();
   return s;
}

std::string PrepareColorGuide(const ColorRange& colorRange, const int xint, const int yint) {
   unsigned long r;
   unsigned long g;
   unsigned long b;
   const size_t nguides = 9;
   std::string out;

   for (size_t i = 0; i <= nguides; ++i)
   {
      const double frac = double(i) / double(nguides - 1);
      colorRange.GetRGBFromRangeFraction(frac, r, g, b);
      const std::string x = std::to_string(xint + i * 15);
      const std::string y = std::to_string(yint);
      const std::string s = std::string("<circle  r=\"12\" stroke = \"black\" stroke