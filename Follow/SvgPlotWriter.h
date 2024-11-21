// SvgPlotWriter.h
#pragma once

#include <fstream>
#include <vector>
#include <memory>

#include "ColorTables.h"
#include "ControlVariables.h"
#include "Distance.h"
#include "GlitchDetector.h"

class SvgPlotWriter {
public:
   SvgPlotWriter(std::ofstream& svg, const ControlVariables& controlVars,
       GlitchDetector& detector);

   void writePlot(const std::vector<std::vector<double>>& allDistances,
      const std::vector<std::unique_ptr<Distance>>& distances,
      int trial, int perturbation,
      int width = 800, int height = 400);


private:
   void writeHeader(int width, int height);
   void writeTitle(int width, const std::string& datetime, int trial, int perturbation);
   void writeGridAndAxes(int width, int height, int margin, double maxDist,
      const std::vector<std::vector<double>>& allDistances);
   void writeAxisLabels(int leftMargin, int width, int height, int margin,
      double xMin, double xMax, double xStepSize,
      double yMin, double yMax, double yStepSize,
      double xScale, double yScale, bool useScientific);
   void writePlotData(int width, int height, int margin, double maxDist,
      const std::vector<std::vector<double>>& allDistances);
   void writeLegend(int width, int margin,
      const std::vector<std::vector<double>>& allDistances,
      const std::vector<std::unique_ptr<Distance>>& distances);

   void writeMetadata(int trial, int perturbation, const std::string& datetime);

   std::ofstream& svg;
   const ControlVariables& controlVars;
   GlitchDetector& glitchDetector;
   //const int minColor = 0xFFFF00;  // yellow
   //const int maxColor = 0x1589FF; // nice blue
   ColorRange colorRange = { 0xFFFF00, 0x1589FF };
};