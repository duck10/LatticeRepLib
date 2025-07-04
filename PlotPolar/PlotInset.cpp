#include "PlotInset.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>

PlotInset::PlotInset()
   : m_enabled(false)
   , m_x(0.0)
   , m_y(0.0)
   , m_size(0.0)
   , m_region()
{
}

PlotInset::PlotInset(const double x, const double y, const double size, const InsetRegion& region)
   : m_enabled(true)
   , m_x(x)
   , m_y(y)
   , m_size(size)
   , m_region(region)
{
}

std::string PlotInset::writeSVG(const size_t whichCoordinate, const std::vector<PlottedPolar>& plottedData,
   const ColorRange& colRange) const {
   if (!isEnabled()) {
      return "";
   }

   std::string svg = "<g transform=\"translate(" + LRL_DataToSVG(m_x) + "," + LRL_DataToSVG(m_y) + ")\">\n";

   // Inset background and border
   svg += "<rect x=\"0\" y=\"0\" width=\"" + LRL_DataToSVG(m_size) +
      "\" height=\"" + LRL_DataToSVG(m_size) +
      "\" fill=\"white\" stroke=\"orange\" stroke-width=\"2\"/>\n";

   // Debug: Add size info to SVG
   svg += "<!-- Inset size: " + LRL_DataToSVG(m_size) + "x" + LRL_DataToSVG(m_size) + " -->\n";

   // Calculate and display magnification factor
   const double contentSize = m_size * 0.95;  // Updated to match writeInsetContent
   const double regionWidth = m_region.width();
   const double regionHeight = m_region.height();
   const double maxRegionDim = std::max(regionWidth, regionHeight);

   if (maxRegionDim > 0.0) {
      const double magnification = (contentSize * 0.95) / maxRegionDim;  // Match the scaling
      std::ostringstream magStream;
      magStream << std::fixed << std::setprecision(1) << magnification << "x";

      // Position magnification text in lower right corner
      const double magX = m_size - 5;  // 5 pixels from right edge
      const double magY = m_size - 5;  // 5 pixels from bottom edge

      svg += "<text x=\"" + LRL_DataToSVG(magX) + "\" y=\"" + LRL_DataToSVG(magY) + "\" " +
         "text-anchor=\"end\" font-size=\"10\" fill=\"blue\">" + magStream.str() + "</text>\n";
   }

   // Content
   svg += writeInsetContent(whichCoordinate, plottedData, colRange);

   svg += "</g>\n";
   return svg;
}

std::string PlotInset::writeInsetContent(const size_t whichCoordinate, const std::vector<PlottedPolar>& plottedData,
   const ColorRange& colRange) const {

   std::cout.flush();

   // Use 95% of inset size for content area, centered
   const double contentSize = m_size * 0.95;
   const double centerX = m_size / 2.0;
   const double centerY = m_size / 2.0;

   std::string content = "<g transform=\"translate(" + LRL_DataToSVG(centerX) + "," +
      LRL_DataToSVG(centerY) + ")\">\n";

   // Calculate scaling first to determine proper axis length
   const double regionWidth = m_region.width();
   const double regionHeight = m_region.height();
   const double maxRegionDim = std::max(regionWidth, regionHeight);

   double scale = 1.0;
   if (maxRegionDim > 0.0) {
      scale = (contentSize * 0.8) / maxRegionDim;  // Scale to 1.4x to leave more margin
   }

   // Draw coordinate axes to match the actual data extent - use full scaled region
   const double axisLen = (maxRegionDim * scale) / 2.0;

   // But scale up the axes to fill more of the content area
   const double axisScale = 2.0;  // Make axes twice as long as data extent
   const double displayAxisLen = axisLen * axisScale;

   content += "<line x1=\"" + LRL_DataToSVG(-displayAxisLen) + "\" y1=\"0\" " +
      "x2=\"" + LRL_DataToSVG(displayAxisLen) + "\" y2=\"0\" " +
      "stroke=\"gray\" stroke-width=\"1\"/>\n";
   content += "<line x1=\"0\" y1=\"" + LRL_DataToSVG(-displayAxisLen) + "\" " +
      "x2=\"0\" y2=\"" + LRL_DataToSVG(displayAxisLen) + "\" " +
      "stroke=\"gray\" stroke-width=\"1\"/>\n";

   // Collect points that fall within the zoom region
   std::vector<std::pair<size_t, std::pair<double, double>>> pointsInRegion;

   for (size_t i = 0; i < plottedData.size(); ++i) {
      const PlottedPolar& plotted = plottedData[i];
      const Vector_2& coords = plotted[whichCoordinate - 1];  // Get coordinates for this plot
      const double plotX = coords[0];
      const double plotY = coords[1];

      // Check if this point is within the zoom region bounds
      if (plotX >= m_region.minX && plotX <= m_region.maxX &&
         plotY >= m_region.minY && plotY <= m_region.maxY) {
         pointsInRegion.emplace_back(i, std::make_pair(plotX, plotY));
      }
   }

   std::cout << "; DEBUG: Found " << pointsInRegion.size() << " points in region for coordinate " << whichCoordinate << std::endl;
   std::cout.flush();

   if (pointsInRegion.empty()) {
      std::cout << "; DEBUG: No points found, returning early" << std::endl;
      content += "<text x=\"0\" y=\"0\" text-anchor=\"middle\" font-size=\"10\" fill=\"orange\">No points in zoom region</text>\n";
      content += "</g>\n";
      return content;
   }

   {
      std::cout << "; DEBUG: About to calculate data bounds" << std::endl;
   }
   std::cout.flush();

   // Add Y-flip transform to match main plot coordinate system
   content += "<g transform=\"scale(1,-1)\">\n";

   // Draw the points in the inset
   for (const auto& pointData : pointsInRegion) {
      const size_t originalIndex = pointData.first;
      const double plotX = pointData.second.first;
      const double plotY = pointData.second.second;

      // Transform to inset coordinates: center on actual region midpoint
      const double regionCenterX = (m_region.minX + m_region.maxX) / 2.0;
      const double regionCenterY = (m_region.minY + m_region.maxY) / 2.0;
      const double insetX = (plotX - regionCenterX) * scale;
      const double insetY = (plotY - regionCenterY) * scale;

      // Get color based on original point index
      unsigned long r, g, b;
      const double frac = (plottedData.size() == 1) ? 1.0 : double(originalIndex) / double(plottedData.size() - 1);
      colRange.GetRGBFromRangeFraction(frac, r, g, b);

      // Draw the point (smaller radius for inset)
      content += "<circle r=\"4\" fill=\"rgb(" + LRL_ToString(r) + "," +
         LRL_ToString(g) + "," + LRL_ToString(b) +
         ")\" cx=\"" + LRL_DataToSVG(insetX) +
         "\" cy=\"" + LRL_DataToSVG(insetY) + "\" " +
         "stroke=\"black\" stroke-width=\"0.5\"/>\n";
   }

   content += "</g>\n"; // End Y-flip transform
   content += "</g>\n"; // End inset content group
   return content;
}



std::string PlotInset::writeZoomBox() const {
   if (!isEnabled()) {
      return "";
   }

   // Add small margin around the detected region
   const double marginX = m_region.width() * 0.05;
   const double marginY = m_region.height() * 0.05;

   const double x = m_region.minX - marginX;
   const double y = m_region.minY - marginY;
   const double width = m_region.width() + 2 * marginX;
   const double height = m_region.height() + 2 * marginY;

   return "<rect x=\"" + LRL_DataToSVG(x) + "\" y=\"" + LRL_DataToSVG(y) +
      "\" width=\"" + LRL_DataToSVG(width) + "\" height=\"" + LRL_DataToSVG(height) +
      "\" fill=\"none\" stroke=\"orange\" stroke-width=\"3\" stroke-dasharray=\"10,5\"/>\n";
}

PlotInset::InsetRegion PlotInset::autoDetectCluster(const size_t whichCoordinate, const std::vector<PlottedPolar>& plottedData) {
   InsetRegion region;
   if (plottedData.size() < 5) {
      return region; // Invalid region
   }

   // Extract coordinates for the specified plot
   std::vector<std::pair<double, double>> points;
   for (const PlottedPolar& plotted : plottedData) {
      const Vector_2& coords = plotted[whichCoordinate - 1];
      points.push_back(std::make_pair(coords[0], coords[1]));
   }

   // Find the geometric center of all points
   double centerX = 0.0, centerY = 0.0;
   for (const auto& point : points) {
      centerX += point.first;
      centerY += point.second;
   }
   centerX /= points.size();
   centerY /= points.size();

   // Calculate distance from center for each point and create index pairs
   std::vector<std::pair<double, size_t>> distances;
   for (size_t i = 0; i < points.size(); ++i) {
      const double dx = points[i].first - centerX;
      const double dy = points[i].second - centerY;
      const double distance = std::sqrt(dx * dx + dy * dy);
      distances.push_back(std::make_pair(distance, i));
   }

   // Sort by distance to find closest points
   std::sort(distances.begin(), distances.end());

   // Target the central 80% of points for the cluster
   const size_t targetPoints = static_cast<size_t>(plottedData.size() * 0.8);

   // Find the actual bounds of the closest targetPoints
   double minX = std::numeric_limits<double>::max();
   double maxX = std::numeric_limits<double>::lowest();
   double minY = std::numeric_limits<double>::max();
   double maxY = std::numeric_limits<double>::lowest();

   for (size_t i = 0; i < targetPoints; ++i) {
      const size_t pointIndex = distances[i].second;
      const double x = points[pointIndex].first;
      const double y = points[pointIndex].second;

      minX = std::min(minX, x);
      maxX = std::max(maxX, x);
      minY = std::min(minY, y);
      maxY = std::max(maxY, y);
   }

   // Create region from the actual cluster bounds
   region = InsetRegion(minX, minY, maxX, maxY);
   region.pointCount = targetPoints;

   // Ensure the region has minimum size for visibility
   const double minSize = 20.0; // Minimum region size in plot coordinates
   if (region.width() < minSize) {
      const double expand = (minSize - region.width()) / 2.0;
      region.minX -= expand;
      region.maxX += expand;
   }
   if (region.height() < minSize) {
      const double expand = (minSize - region.height()) / 2.0;
      region.minY -= expand;
      region.maxY += expand;
   }

   return region;
}



