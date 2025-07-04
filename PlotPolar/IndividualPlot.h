#ifndef INDIVIDUALPLOT_H
#define INDIVIDUALPLOT_H

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "InsetData.h"  // Changed from PlotInset.h
#include "PlotPolarControls.h"
#include "PlottedPolar.h"
#include "ColorTables.h"
#include "LRL_ToString.h"
#include "LRL_DataToSVG.h"

class IndividualPlot {
public:
   // Constructor
   IndividualPlot(const size_t whichCoordinate, const int plotX, const int plotY, const int plotSize,
      const PlotPolarControls& controls);

   // Core functionality - takes already-plotted coordinates
   void setPlottedData(const std::vector<PlottedPolar>& plottedData, const ColorRange& colRange);

   void autoDetectInset(const double insetX = 200.0, const double insetY = -300.0, const double insetSize = 270.0);

   // SVG generation
   std::string writeSVG() const;

   // Getters
   size_t getCoordinate() const { return m_whichCoordinate; }
   int getPlotX() const { return m_plotX; }
   int getPlotY() const { return m_plotY; }
   int getPlotSize() const { return m_plotSize; }
   bool hasInset() const { return !m_insetData.isEmpty(); }

   friend std::ostream& operator<<(std::ostream& os, const IndividualPlot& plot) {
      os << "IndividualPlot: coord=" << plot.m_whichCoordinate
         << " pos=(" << plot.m_plotX << "," << plot.m_plotY << ") size=" << plot.m_plotSize
         << " points=" << plot.m_plottedData.size() << " inset=" << (plot.hasInset() ? "yes" : "no");
      return os;
   }

private:
   const size_t m_whichCoordinate;  // Which coordinate (1, 2, or 3)
   const int m_plotX;               // Position in global SVG space
   const int m_plotY;
   const int m_plotSize;            // Size of plot area
   const PlotPolarControls& m_controls;

   std::vector<PlottedPolar> m_plottedData;  // Already in plot coordinates
   const ColorRange* m_pColRange;            // Pointer to color mapping

   InsetData m_insetData;           // New clean inset data structure

   // Internal methods
   std::string writeAxes() const;
   std::string writePoints() const;
   std::string writeLabels() const;
   std::string writeInsetAndZoomBox() const;
   std::string writeConvexHullConnectorLines(const double insetX, const double insetY, const double insetSize) const;
};

#endif // INDIVIDUALPLOT_H