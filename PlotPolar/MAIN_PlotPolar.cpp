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
#include "PlottedPolar.h"

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
      const std::string s = std::string("<circle  r=\"12\" stroke = \"black\" stroke-width=\"0.5\""
         " fill= \"rgb(" + std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + ")\""
         " cx=\"" + x + "\""
         " cy=\"" + y + "\"/>\n");
      out += s;
   }
   return out;
}

// Convert Polar data to PlottedPolar (scaled for display)
std::vector<PlottedPolar> convertToPlottedPolar(const std::vector<Polar>& polarData, const double scale) {
   std::vector<PlottedPolar> plottedData;

   for (const Polar& polar : polarData) {
      // Extract coordinates for each plot and scale them
      Vector_2 coord1(polar[0][0] * scale, polar[0][1] * scale);
      Vector_2 coord2(polar[1][0] * scale, polar[1][1] * scale);
      Vector_2 coord3(polar[2][0] * scale, polar[2][1] * scale);

      plottedData.emplace_back(PlottedPolar(coord1, coord2, coord3));
   }

   return plottedData;
}

// Helper function to create and configure a plot
IndividualPlot createPlot(size_t coordinate, int x, int y, int size,
   const PlotPolarControls& controls,
   const std::vector<PlottedPolar>& plottedData,
   const ColorRange& colRange) {
   IndividualPlot plot(coordinate, x, y, size, controls);
   plot.setPlottedData(plottedData, colRange);

   if (controls.getEnableZoom()) {
      const double insetSize = double(controls.getInsetSize());
      plot.autoDetectInset(200.0, -300.0, insetSize);

      if (controls.shouldShowDetails()) {
         if (plot.hasInset()) {
            std::cout << "; Plot " << coordinate << ": Zoom inset enabled" << std::endl;
         }
         else {
            std::cout << "; Plot " << coordinate << ": No suitable cluster found for zoom" << std::endl;
         }
      }
   }

   return plot;
}




int main(int argc, char* argv[]) {
   const std::string programName("PlotPolar");
   const std::string dataName("Polar");
   const ColorRange colorRange(0xFFFF00, 0x1589FF);

   std::cout << "; " + programName << std::endl;

   try {
      WebIO webio(argc, argv, programName, 1);

      PlotPolarControls controls;
      const BasicProgramInput<PlotPolarControls> dc_setup("CmdPlotPolar", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      webio.CreateFilenamesAndLinks(1, controls.getPrefix());
      const std::string& graphicsFileName = webio.m_FileNameList[0];
      std::cout << "; " + programName + " graphics file " << webio.m_FullfileNameList[0] << std::endl;

      // Create PlotPolar for scaling calculations (keeping compatibility)
      PlotPolar thePlot(graphicsFileName, 1250, 750, 500, 500);

      std::string svgOutput;
      const std::string intro = thePlot.GetIntro(webio.m_basicfileNameList[0]);
      svgOutput += intro;

      const std::vector<Polar> vData = ConvertInput<Polar>(dc_setup.getInputList());
      if (controls.shouldShowDetails()) {
         std::cout << "; Converted " << vData.size() << " lattice cells to Polar coordinates" << std::endl;
      }

      // Calculate scaling using existing PlotPolar class
      const double cellScale = thePlot.CellScale(vData);
      const double scaleFactor = thePlot.CellScaleFactor();
      if (controls.shouldShowDetails()) {
         std::cout << "; Scale factor: " << scaleFactor << std::endl;
      }

      // Convert to PlottedPolar using the calculated scale
      const std::vector<PlottedPolar> plottedData = convertToPlottedPolar(vData, scaleFactor);

      std::pair<double, double> minmax = GetMinMaxPlot(vData);
      if (abs(minmax.second) < 1.0E-5) minmax.second = 0.0;
      const std::string dataRange =
         LRL_ToString("; The " + dataName + " data value range is ", minmax.first, " to ", minmax.second);
      const std::string legend = AddTextAtBottom(1150, 800, dataRange, programName) +
         PrepareColorGuide(colorRange, 1150, 750);

      svgOutput += legend;

      // Create individual plots using the new architecture - REFACTORED
      const ColorRange colRange(0xFFFF00, 0x1589FF);

      // Plot configurations: {coordinate, x, y, size}
      const std::vector<std::tuple<size_t, int, int, int>> plotConfigs = {
         {1, 500, 500, 500},    // Plot 1
         {2, 1550, 500, 500},   // Plot 2  
         {3, 500, 1100, 500}    // Plot 3
      };

      // Create and render all plots
      for (const auto& config : plotConfigs) {
         auto [coordinate, x, y, size] = config;
         IndividualPlot plot = createPlot(coordinate, x, y, size, controls, plottedData, colRange);
         svgOutput += plot.writeSVG();
      }

      std::cout << dataRange << std::endl << std::endl;
      ListInput(dc_setup.getInputList());
      thePlot.SendFrameToFile(graphicsFileName, svgOutput + thePlot.GetFoot());
      std::cout << PrepareLegend(600, 600, vData.size(), programName);

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}

