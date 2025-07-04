#include "IndividualPlot.h"
#include "Vector_2.h"

IndividualPlot::IndividualPlot(const size_t whichCoordinate, const int plotX, const int plotY, const int plotSize,
   const PlotPolarControls& controls)
   : m_whichCoordinate(whichCoordinate)
   , m_plotX(plotX)
   , m_plotY(plotY)
   , m_plotSize(plotSize)
   , m_pColRange(nullptr)
   , m_insetData()  // Default zero-size inset
   , m_controls(controls)
{
}

void IndividualPlot::setPlottedData(const std::vector<PlottedPolar>& plottedData, const ColorRange& colRange) {
   m_plottedData = plottedData;
   m_pColRange = &colRange;
}



std::string IndividualPlot::writeSVG() const {
   // Create the plot group with translation to position
   std::string svg = "<g transform=\"translate(" + LRL_DataToSVG(m_plotX) + "," + LRL_DataToSVG(m_plotY) + ")\">\n";

   // Add components in order
   svg += writeAxes();
   svg += writePoints();
   svg += writeInsetAndZoomBox();
   svg += writeLabels();

   svg += "</g>\n";
   return svg;
}

std::string IndividualPlot::writeAxes() const {
   const std::string axisSize = LRL_DataToSVG_ToQuotedString(m_plotSize);
   const std::string negAxisSize = LRL_DataToSVG_ToQuotedString(-m_plotSize);
   const std::string zero = LRL_DataToSVG_ToQuotedString(0);

   // Vertical axis (Y)
   std::string axes = "<line x1=" + zero + " y1=" + zero + " x2=" + zero +
      " y2=" + negAxisSize + " stroke-width=\"2\" stroke=\"black\"/>\n";

   // Horizontal axis (X)  
   axes += "<line x1=" + axisSize + " y1=" + zero + " x2=" + negAxisSize +
      " y2=" + zero + " stroke-width=\"2\" stroke=\"black\"/>\n";

   return axes;
}



void IndividualPlot::autoDetectInset(const double insetX, const double insetY, const double insetSize) {
if ( m_controls.shouldShowDetails())
   {
      std::cout << "; DEBUG: autoDetectInset called with size=" << insetSize << " for coordinate " << m_whichCoordinate << std::endl;
   }

   if (m_plottedData.size() < 5 || m_pColRange == nullptr) {
      m_insetData = InsetData(); // Empty inset
      return;
   }

   // Auto-detect cluster using the new clean approach
   m_insetData = autoDetectCluster(m_whichCoordinate, m_plottedData, m_controls.getClusterPercent());
   if (!m_insetData.isEmpty() && m_controls.shouldShowDetails()) {
      std::cout << "; DEBUG: Creating inset with size=" << insetSize
         << " center=(" << m_insetData.center[0] << "," << m_insetData.center[1] << ")"
         << " extents=(" << m_insetData.extents[0] << "," << m_insetData.extents[1] << ")"
         << " points=" << m_insetData.pointIndices.size() << std::endl;
   }
   else {
      if ( m_controls.shouldShowDetails())
      {
         std::cout << "; DEBUG: No valid cluster detected for coordinate " << m_whichCoordinate << std::endl;
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

   // Add zoom box INSIDE the Y-flip transform using the clean approach
   if (!m_insetData.isEmpty()) {
      points += writeZoomBox(m_insetData);
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
   if (m_insetData.isEmpty() || m_pColRange == nullptr) {
      return "";
   }

   std::string result;

   // Add the inset content
   const double insetX = 200.0;  // Default inset position
   const double insetY = -300.0;
   const double insetSize = 270.0;

   // Create the inset group with positioning
   result += "<g transform=\"translate(" + LRL_DataToSVG(insetX) + "," + LRL_DataToSVG(insetY) + ")\">\n";

   // Inset background and border
   result += "<rect x=\"0\" y=\"0\" width=\"" + LRL_DataToSVG(insetSize) +
      "\" height=\"" + LRL_DataToSVG(insetSize) +
      "\" fill=\"white\" stroke=\"orange\" stroke-width=\"2\"/>\n";

   // Title and build timestamp
   result += "<text x=\"" + LRL_DataToSVG(insetSize / 2.0) + "\" y=\"15\" " +
      "text-anchor=\"middle\" font-size=\"15\" fill=\"orange\">Zoom View</text>\n";

   // Calculate and display magnification factor
   const double maxExtent = std::max(m_insetData.extents[0], m_insetData.extents[1]);
   const double magnification = (insetSize * 0.8 * 0.8) / (2.0 * maxExtent);

   std::ostringstream magStream;
   magStream << std::fixed << std::setprecision(1) << magnification << "x";
   const double magX = insetSize - 5;
   const double magY = insetSize - 5;
   result += "<text x=\"" + LRL_DataToSVG(magX) + "\" y=\"" + LRL_DataToSVG(magY) + "\" " +
      "text-anchor=\"end\" font-size=\"20\" fill=\"blue\">" + magStream.str() + "</text>\n";

   // Add the actual inset content using the clean new approach
   result += writeInsetContent(m_insetData, m_whichCoordinate, m_plottedData, *m_pColRange, insetSize);

   result += "</g>\n"; // End inset group

   // Add connector lines using simple approach
   result += writeConvexHullConnectorLines(insetX, insetY, insetSize);

   return result;
}


std::string IndividualPlot::writeConvexHullConnectorLines(const double insetX, const double insetY, const double insetSize) const {
   if (m_insetData.isEmpty()) {
      return "";
   }

   std::string result;

   // All points (zoom + inset corners) - using the ORIGINAL WORKING logic
   struct Point {
      double x, y;
      bool isZoom;
      Point(double x_, double y_, bool zoom) : x(x_), y(y_), isZoom(zoom) {}
   };

   std::vector<Point> points;

   // Add zoom corners using InsetData bounds - EXACT original coordinates
   points.emplace_back(m_insetData.minX(), -m_insetData.minY(), true);  // NW zoom
   points.emplace_back(m_insetData.maxX(), -m_insetData.minY(), true);  // NE zoom  
   points.emplace_back(m_insetData.maxX(), -m_insetData.maxY(), true);  // SE zoom
   points.emplace_back(m_insetData.minX(), -m_insetData.maxY(), true);  // SW zoom

   // Add inset corners - EXACT original coordinates
   points.emplace_back(insetX, insetY, false);                          // NW inset
   points.emplace_back(insetX + insetSize, insetY, false);              // NE inset
   points.emplace_back(insetX + insetSize, insetY + insetSize, false);  // SE inset
   points.emplace_back(insetX, insetY + insetSize, false);              // SW inset

   // Convex hull using Graham scan - EXACT ORIGINAL ALGORITHM
   auto cross = [](const Point& o, const Point& a, const Point& b) {
      return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
      };

   // Find bottom-most point (or left-most if tie)
   size_t minIdx = 0;
   for (size_t i = 1; i < points.size(); i++) {
      if (points[i].y < points[minIdx].y ||
         (points[i].y == points[minIdx].y && points[i].x < points[minIdx].x)) {
         minIdx = i;
      }
   }
   std::swap(points[0], points[minIdx]);

   // Sort points by polar angle with respect to points[0]
   Point pivot = points[0];
   std::sort(points.begin() + 1, points.end(), [&](const Point& a, const Point& b) {
      double crossProd = cross(pivot, a, b);
      if (crossProd == 0) {
         // Collinear points - sort by distance
         double distA = (a.x - pivot.x) * (a.x - pivot.x) + (a.y - pivot.y) * (a.y - pivot.y);
         double distB = (b.x - pivot.x) * (b.x - pivot.x) + (b.y - pivot.y) * (b.y - pivot.y);
         return distA < distB;
      }
      return crossProd > 0;
      });

   // Build convex hull
   std::vector<Point> hull;
   for (const Point& p : points) {
      while (hull.size() >= 2 && cross(hull[hull.size() - 2], hull[hull.size() - 1], p) <= 0) {
         hull.pop_back();
      }
      hull.push_back(p);
   }

   // Find connector lines (hull edges connecting zoom to inset points)
   for (size_t i = 0; i < hull.size(); i++) {
      size_t next = (i + 1) % hull.size();

      // Check if this hull edge connects a zoom point to an inset point
      if (hull[i].isZoom != hull[next].isZoom) {
         result += "<line x1=\"" + LRL_DataToSVG(hull[i].x) + "\" y1=\"" + LRL_DataToSVG(hull[i].y) +
            "\" x2=\"" + LRL_DataToSVG(hull[next].x) + "\" y2=\"" + LRL_DataToSVG(hull[next].y) +
            "\" stroke=\"orange\" stroke-width=\"1\" stroke-dasharray=\"5,5\"/>\n";
      }
   }

   return result;
}

