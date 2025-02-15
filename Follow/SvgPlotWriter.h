#ifndef SVGPLOTWRITER_H
#define SVGPLOTWRITER_H

#include <fstream>
#include <vector>
#include <memory>

#include "Distance.h"
#include "FollowControls.h"
#include "GlitchTypes.h"  

struct LineStyle {
   std::string color;
   std::string dashArray;
   std::string markerType;
   static std::vector<LineStyle> getStyles(size_t count);
};

class ColorTables {
public:
   static std::string interpolateColor(size_t index, size_t total) {
      if (total <= 1) return "#1589FF";
      double t = index / (total - 1.0);
      int r = static_cast<int>(255 * (1.0 - t));
      int b = static_cast<int>(255 * t);
      char hexColor[8];
      snprintf(hexColor, sizeof(hexColor), "#%02X00%02X", r, b);
      return hexColor;
   }
};

struct DataPoint {
   double x;
   double y;
   size_t index;
};

class SvgPlotWriter {
public:
   SvgPlotWriter(std::ofstream& svgfile, const FollowControls& controls);

   void setGlitches(const std::vector<Glitch>& glitches) { this->glitches = glitches; }

   void writePlot(const std::vector<std::vector<double>>& allDistances,
      const std::vector<std::unique_ptr<Distance>>& distfuncs,
      int trial, int perturbation,
      int width = 800, int height = 400);

private:
   static constexpr int PLOT_LINE_WIDTH = 3;
   static constexpr int NUM_MARKERS = 8;

   void writeHeader(int width, int height);
   void writeTitle(int width, const std::string& datetime, int trial, int perturbation);
   void writeGridAndAxes(int width, int height, int margin, double maxDist,
      const std::vector<std::vector<double>>& allDistances);

   void writeGrid(int width, int height, int leftMargin, int margin,
      const AxisLimits& xLimits, const AxisLimits& yLimits,
      double xScale, double yScale);


   void writePlotData(int width, int height, int margin, double maxDist,
      const std::vector<std::vector<double>>& allDistances,
      const std::vector<std::unique_ptr<Distance>>& distfuncs);
   void writeXAxis(int width, int height, int leftMargin, int margin,
      const AxisLimits& limits, double xScale);


   void writeYAxis(int leftMargin, int height, int margin,
      const AxisLimits& limits, double yScale);


   void writeLegend(int width, int margin,
      const std::vector<std::vector<double>>& allDistances,
      const std::vector<std::unique_ptr<Distance>>& distfuncs);
   void writeMetadata(int trial, int perturbation, const std::string& datetime);
   std::string reportGlitches(const int n = 2);
   std::string WriteDistanceSummary(const std::vector<std::vector<double>>& alldistances) const;

private:
   struct PlotDimensions {
      double xMin, yMin;
      double xScale, yScale;
      int leftMargin, height, margin;
      double plotWidth, plotHeight;
   };
   PlotDimensions calculatePlotDimensions(int width, int height, int margin,
      const std::vector<std::vector<double>>& allDistances) const;

   void drawPlotLine(const std::vector<double>& values, const PlotDimensions& dims,
      const LineStyle& style);

   void drawGlitches(const PlotDimensions& dims, const std::string& distanceType);

   void drawGlitchLine(double x, int height, int margin);
   void drawGlitchMarker(double x, double y);
   void drawGlitchIndex(double x, double y, int margin, size_t index);
   void writeGlitchComments(const std::vector<std::unique_ptr<Distance>>& distfuncs);

   std::ofstream& svg;
   const FollowControls& controls;
   std::vector<Glitch> glitches;
};

#endif // SVGPLOTWRITER_H



