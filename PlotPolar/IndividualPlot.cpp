#include "IndividualPlot.h"
#include "Vector_2.h"
#include "ZoomInset.h"
#include "LinearAxis.h"
#include <sstream>
#include <iomanip>

IndividualPlot::IndividualPlot(const size_t whichCoordinate, const int plotX, const int plotY, const int plotSize,
   const PlotPolarControls& controls, const double scaleFactor)
   : m_whichCoordinate(whichCoordinate)
   , m_plotX(plotX)
   , m_plotY(plotY)
   , m_plotSize(plotSize)
   , m_pColRange(nullptr)
   , m_zoomInset()
   , m_controls(controls)
   , m_scaleFactor(scaleFactor)  // Initialize scale factor
{
}

void IndividualPlot::setPlottedData(const std::vector<PlottedPolar>& plottedData, const ColorRange& colRange) {
   m_plottedData = plottedData;
   m_pColRange = &colRange;
}

std::pair<double, std::string> calculateDataTickValue(const std::vector<PlottedPolar>& plottedData,
   size_t coordinate, double scaleFactor) {
   if (plottedData.empty() || scaleFactor == 0.0) {
      return { 100.0, "1.0" };  // Fallback
   }

   // Reverse engineer the original data range from the scaled pixel data
   double maxPixelVal = 0.0;
   for (const PlottedPolar& plotted : plottedData) {
      const Vector_2& coords = plotted[coordinate - 1];
      maxPixelVal = std::max(maxPixelVal, std::max(std::abs(coords[0]), std::abs(coords[1])));
   }

   // Convert back to data coordinates
   double maxDataVal = maxPixelVal / scaleFactor;

   // Use LinearAxis to find nice tick spacing in data coordinates
   LinearAxis axis;
   axis.SetStepSizeList();
   AxisLimits limits = axis.LinearAxisLimits(0.0, maxDataVal);

   double stepSize = limits.GetStepSize();
   double dataTickValue = stepSize * std::floor(maxDataVal / stepSize);

   if (dataTickValue > maxDataVal * 0.8) {
      dataTickValue -= stepSize;
   }

   if (dataTickValue <= 0) {
      dataTickValue = stepSize;
   }

   // Format the data value for the label
   std::ostringstream oss;
   if (dataTickValue >= 10.0) {
      oss << std::fixed << std::setprecision(0) << dataTickValue;
   }
   else if (dataTickValue >= 1.0) {
      oss << std::fixed << std::setprecision(1) << dataTickValue;
   }
   else {
      oss << std::fixed << std::setprecision(2) << dataTickValue;
   }

   // Return the pixel position and data label
   double pixelPosition = dataTickValue * scaleFactor;
   return std::make_pair(pixelPosition, oss.str());
}


std::string IndividualPlot::writeSVG() const {
   // Create the plot group with translation to position
   std::string svg = "<!-- ===== PLOT " + std::to_string(m_whichCoordinate) + " ===== -->\n";
   svg += "<g transform=\"translate(" + LRL_DataToSVG(m_plotX) + "," + LRL_DataToSVG(m_plotY) + ")\">\n";

   // Add components in order with labels
   svg += "<!-- Plot " + std::to_string(m_whichCoordinate) + " axes -->\n";
   svg += writeAxes();

   svg += "<!-- Plot " + std::to_string(m_whichCoordinate) + " data points -->\n";
   svg += writePoints();

   svg += "<!-- Plot " + std::to_string(m_whichCoordinate) + " zoom inset and zoom box -->\n";
   svg += writeInsetAndZoomBox();

   svg += "<!-- Plot " + std::to_string(m_whichCoordinate) + " labels -->\n";
   svg += writeLabels();

   svg += "</g>\n";
   svg += "<!-- End plot " + std::to_string(m_whichCoordinate) + " -->\n\n";
   return svg;
}



std::string IndividualPlot::writeAxes() const {
   const std::string axisSize = LRL_DataToSVG_ToQuotedString(m_plotSize);
   const std::string negAxisSize = LRL_DataToSVG_ToQuotedString(-m_plotSize);
   const std::string zero = LRL_DataToSVG_ToQuotedString(0);

   // Use the stored scale factor
   auto [pixelTickValue, dataLabel] = calculateDataTickValue(m_plottedData, m_whichCoordinate, m_scaleFactor);

   std::string axes;

   // Y axis
   axes += "<!-- Y axis -->\n";
   axes += "<line x1=" + zero + " y1=" + zero + " x2=" + zero +
      " y2=" + negAxisSize + " stroke-width=\"2\" stroke=\"black\"/>\n";

   // Y axis tick
   axes += "<line x1=\"-5\" y1=\"" + LRL_DataToSVG(-pixelTickValue) + "\" " +
      "x2=\"5\" y2=\"" + LRL_DataToSVG(-pixelTickValue) + "\" " +
      "stroke-width=\"2\" stroke=\"black\"/>\n";
   axes += "<text x=\"-15\" y=\"" + LRL_DataToSVG(-pixelTickValue + 5) + "\" " +
      "text-anchor=\"end\" font-size=\"16\" font-family=\"Arial, Helvetica, sans-serif\">" +
      dataLabel + "</text>\n";

   // X axis
   axes += "<!-- X axis -->\n";
   axes += "<line x1=" + axisSize + " y1=" + zero + " x2=" + negAxisSize +
      " y2=" + zero + " stroke-width=\"2\" stroke=\"black\"/>\n";

   // Positive X tick
   axes += "<line x1=\"" + LRL_DataToSVG(pixelTickValue) + "\" y1=\"-5\" " +
      "x2=\"" + LRL_DataToSVG(pixelTickValue) + "\" y2=\"5\" " +
      "stroke-width=\"2\" stroke=\"black\"/>\n";
   axes += "<text x=\"" + LRL_DataToSVG(pixelTickValue) + "\" y=\"20\" " +
      "text-anchor=\"middle\" font-size=\"16\" font-family=\"Arial, Helvetica, sans-serif\">" +
      dataLabel + "</text>\n";

   // Negative X tick
   axes += "<line x1=\"" + LRL_DataToSVG(-pixelTickValue) + "\" y1=\"-5\" " +
      "x2=\"" + LRL_DataToSVG(-pixelTickValue) + "\" y2=\"5\" " +
      "stroke-width=\"2\" stroke=\"black\"/>\n";
   axes += "<text x=\"" + LRL_DataToSVG(-pixelTickValue) + "\" y=\"20\" " +
      "text-anchor=\"middle\" font-size=\"16\" font-family=\"Arial, Helvetica, sans-serif\">-" +
      dataLabel + "</text>\n";

   return axes;
}



void IndividualPlot::autoDetectInset(const double insetX, const double insetY, const double insetSize) {
   if (m_controls.shouldShowDetails())
   {
      std::cout << "; DEBUG: autoDetectInset called with size=" << insetSize << " for coordinate " << m_whichCoordinate << std::endl;
   }

   if (m_plottedData.size() < 5 || m_pColRange == nullptr) {
      m_zoomInset = ZoomInset(); // Empty zoom inset
      return;
   }

   // Create zoom inset - it handles everything internally
   m_zoomInset = ZoomInset(m_plottedData, m_whichCoordinate, insetX, insetY, insetSize, m_controls.getClusterPercent());
   if (m_controls.shouldShowDetails())
   {
      if (m_zoomInset.isEnabled()) {
         std::cout << "; Plot " << m_whichCoordinate << ": Zoom inset enabled" << std::endl;
      }
      else {
         std::cout << "; Plot " << m_whichCoordinate << ": Zoom inset disabled" << std::endl;
      }
   }
}

std::string IndividualPlot::writePoints() const {
   if (m_pColRange == nullptr) {
      return "<!-- No color range set -->\n";
   }

   std::string points = "<g transform=\"translate(0,0) scale(1,-1)\">\n";  // Flip Y coordinate

   for (size_t i = 0; i < m_plottedData.size(); ++i) {
      const PlottedPolar& plotted = m_plottedData[i];
      const Vector_2& coords = plotted[m_whichCoordinate - 1];
      const double plotX = coords[0];
      const double plotY = coords[1];

      unsigned long r, g, b;
      const double frac = (m_plottedData.size() == 1) ? 1.0 : double(i) / double(m_plottedData.size() - 1);
      m_pColRange->GetRGBFromRangeFraction(frac, r, g, b);

      points += std::string("<circle r=\"10\" stroke=\"black\" stroke-width=\"0.5\" ") +
         "fill=\"rgb(" + LRL_ToString(r) + "," + LRL_ToString(g) + "," + LRL_ToString(b) + ")\" " +
         "cx=\"" + LRL_DataToSVG(plotX) + "\" cy=\"" + LRL_DataToSVG(plotY) + "\"/>\n";
   }

   // ZoomInset now handles its own zoom box rendering
   if (m_zoomInset.isEnabled()) {
      points += "<!-- Zoom selection box for plot " + std::to_string(m_whichCoordinate) + " -->\n";
      points += m_zoomInset.writeZoomBox();  // Just the zoom box part
   }

   points += "</g>\n";
   return points;
}



std::string IndividualPlot::writeLabels() const {
   const std::string coordStr = std::to_string(m_whichCoordinate);

   std::string labels;
   labels += "<!--plot labeling-->\n";

   // Main coordinate label
   labels += "<g transform=\"translate(0,-265)\">\n";
   labels += "<text x=\"-350\" y=\"-200\" font-size=\"40\" font-family=\"Arial, Helvetica, sans-serif\">Coordinate p</text>\n";
   labels += "<g transform=\"translate(205,10)\">\n";
   labels += "<text x=\"-325\" y=\"-200\" font-size=\"30\" font-family=\"Arial, Helvetica, sans-serif\">";
   labels += coordStr;
   labels += "</text>\n";
   labels += "</g>\n</g>\n";

   // -X label
   labels += "<g transform=\"translate(0,0)\">\n";
   labels += "<text x=\"-480\" y=\"-20\" font-size=\"40\" font-family=\"Arial, Helvetica, sans-serif\">-x</text>\n";
   labels += "<g transform=\"translate(30,10)\">\n";
   labels += "<text x=\"-480\" y=\"-20\" font-size=\"30\" font-family=\"Arial, Helvetica, sans-serif\">";
   labels += coordStr;
   labels += "</text>\n";
   labels += "</g>\n</g>\n";

   // Y label
   labels += "<g transform=\"translate(0,0)\">\n";
   labels += "<text x=\"-60\" y=\"-450\" font-size=\"40\" font-family=\"Arial, Helvetica, sans-serif\">y</text>\n";
   labels += "<g transform=\"translate(30,10)\">\n";
   labels += "<text x=\"-60\" y=\"-450\" font-size=\"30\" font-family=\"Arial, Helvetica, sans-serif\">";
   labels += coordStr;
   labels += "</text>\n";
   labels += "</g>\n</g>\n";

   // X label  
   labels += "<g transform=\"translate(0,0)\">\n";
   labels += "<text x=\"360\" y=\"-20\" font-size=\"40\" font-family=\"Arial, Helvetica, sans-serif\">x</text>\n";
   labels += "<g transform=\"translate(30,10)\">\n";
   labels += "<text x=\"360\" y=\"-20\" font-size=\"30\" font-family=\"Arial, Helvetica, sans-serif\">";
   labels += coordStr;
   labels += "</text>\n";
   labels += "</g>\n</g>\n";

   return labels;
}

std::string IndividualPlot::writeInsetAndZoomBox() const {
   if (m_pColRange == nullptr) {
      return "";
   }

   // ZoomInset handles everything: inset content + connector lines
   return m_zoomInset.writeSVG(m_plottedData, m_whichCoordinate, *m_pColRange);
}

//std::string IndividualPlot::writeConvexHullConnectorLines(const double insetX, const double insetY, const double insetSize) const {
//   if (m_insetData.isEmpty()) {
//      return "";
//   }
//
//   std::string result;
//
//   // All points (zoom + inset corners) - using the ORIGINAL WORKING logic
//   struct Point {
//      double x, y;
//      bool isZoom;
//      Point(double x_, double y_, bool zoom) : x(x_), y(y_), isZoom(zoom) {}
//   };
//
//   std::vector<Point> points;
//
//   // Add zoom corners using InsetData bounds - EXACT original coordinates
//   points.emplace_back(m_insetData.minX(), -m_insetData.minY(), true);  // NW zoom
//   points.emplace_back(m_insetData.maxX(), -m_insetData.minY(), true);  // NE zoom  
//   points.emplace_back(m_insetData.maxX(), -m_insetData.maxY(), true);  // SE zoom
//   points.emplace_back(m_insetData.minX(), -m_insetData.maxY(), true);  // SW zoom
//
//   // Add inset corners - EXACT original coordinates
//   points.emplace_back(insetX, insetY, false);                          // NW inset
//   points.emplace_back(insetX + insetSize, insetY, false);              // NE inset
//   points.emplace_back(insetX + insetSize, insetY + insetSize, false);  // SE inset
//   points.emplace_back(insetX, insetY + insetSize, false);              // SW inset
//
//   // Convex hull using Graham scan - EXACT ORIGINAL ALGORITHM
//   auto cross = [](const Point& o, const Point& a, const Point& b) {
//      return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
//      };
//
//   // Find bottom-most point (or left-most if tie)
//   size_t minIdx = 0;
//   for (size_t i = 1; i < points.size(); i++) {
//      if (points[i].y < points[minIdx].y ||
//         (points[i].y == points[minIdx].y && points[i].x < points[minIdx].x)) {
//         minIdx = i;
//      }
//   }
//   std::swap(points[0], points[minIdx]);
//
//   // Sort points by polar angle with respect to points[0]
//   Point pivot = points[0];
//   std::sort(points.begin() + 1, points.end(), [&](const Point& a, const Point& b) {
//      double crossProd = cross(pivot, a, b);
//      if (crossProd == 0) {
//         // Collinear points - sort by distance
//         double distA = (a.x - pivot.x) * (a.x - pivot.x) + (a.y - pivot.y) * (a.y - pivot.y);
//         double distB = (b.x - pivot.x) * (b.x - pivot.x) + (b.y - pivot.y) * (b.y - pivot.y);
//         return distA < distB;
//      }
//      return crossProd > 0;
//      });
//
//   // Build convex hull
//   std::vector<Point> hull;
//   for (const Point& p : points) {
//      while (hull.size() >= 2 && cross(hull[hull.size() - 2], hull[hull.size() - 1], p) <= 0) {
//         hull.pop_back();
//      }
//      hull.push_back(p);
//   }
//
//   // Find connector lines (hull edges connecting zoom to inset points)
//   for (size_t i = 0; i < hull.size(); i++) {
//      size_t next = (i + 1) % hull.size();
//
//      // Check if this hull edge connects a zoom point to an inset point
//      if (hull[i].isZoom != hull[next].isZoom) {
//         result += "<line x1=\"" + LRL_DataToSVG(hull[i].x) + "\" y1=\"" + LRL_DataToSVG(hull[i].y) +
//            "\" x2=\"" + LRL_DataToSVG(hull[next].x) + "\" y2=\"" + LRL_DataToSVG(hull[next].y) +
//            "\" stroke=\"orange\" stroke-width=\"1\" stroke-dasharray=\"5,5\"/>\n";
//      }
//   }
//
//   return result;
//}

