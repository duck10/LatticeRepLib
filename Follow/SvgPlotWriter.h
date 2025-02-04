#ifndef SVGPLOTWRITER_H
#define SVGPLOTWRITER_H

#include <fstream>
#include <vector>
#include <memory>

#include "Distance.h"
#include "FollowControls.h"
#include "GlitchTypes.h"  

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
   void writeHeader(int width, int height);
   void writeTitle(int width, const std::string& datetime, int trial, int perturbation);
   void writeGridAndAxes(int width, int height, int margin, double maxDist,
      const std::vector<std::vector<double>>& allDistances);
   void writeAxisLabels(int leftMargin, int width, int height, int margin,
      double xMin, double xMax, double xStepSize,
      double yMin, double yMax, double yStepSize,
      double xScale, double yScale, bool useScientific);
   void writePlotData(int width, int height, int margin, double maxDist,
      const std::vector<std::vector<double>>& allDistances,
      const std::vector<std::unique_ptr<Distance>>& distfuncs);

      void writeLegend(int width, int margin,
      const std::vector<std::vector<double>>& allDistances,
      const std::vector<std::unique_ptr<Distance>>& distfuncs);
   void writeMetadata(int trial, int perturbation, const std::string& datetime);
   std::string reportGlitches(const int n = 2);
   std::string WriteDistanceSummary(const std::vector<std::vector<double>>& alldistances) const;

   std::ofstream& svg;
   const FollowControls& controls;
   std::vector<Glitch> glitches;
};

#endif // SVGPLOTWRITER_H