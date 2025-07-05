#include "InsetData.h"
#include <iomanip>

// Clean cluster detection - returns InsetData directly
InsetData autoDetectCluster(const size_t whichCoordinate, 
   const std::vector<PlottedPolar>& plottedData, double clusterPercent) {

   if (plottedData.size() < 5) {
      return InsetData(); // Empty inset
   }

   // Extract coordinates for the specified plot
   std::vector<Vector_2> points;
   for (const PlottedPolar& plotted : plottedData) {
      const Vector_2& coords = plotted[whichCoordinate - 1];
      points.push_back(coords);
   }

   // Find geometric center of all points
   Vector_2 overallCenter(0.0, 0.0);
   for (const Vector_2& point : points) {
      overallCenter[0] += point[0];
      overallCenter[1] += point[1];
   }
   overallCenter[0] /= points.size();
   overallCenter[1] /= points.size();

   // Find closest 60% of points to center
   std::vector<std::pair<double, size_t>> distances;
   for (size_t i = 0; i < points.size(); ++i) {
      const double dx = points[i][0] - overallCenter[0];
      const double dy = points[i][1] - overallCenter[1];
      const double distance = std::sqrt(dx * dx + dy * dy);
      distances.push_back(std::make_pair(distance, i));
   }

   std::sort(distances.begin(), distances.end());
   const size_t targetPoints = static_cast<size_t>(points.size() * clusterPercent);

   // Calculate actual center and extents of selected points
   Vector_2 clusterCenter(0.0, 0.0);
   std::vector<size_t> selectedIndices;

   for (size_t i = 0; i < targetPoints; ++i) {
      const size_t pointIndex = distances[i].second;
      selectedIndices.push_back(pointIndex);
      clusterCenter[0] += points[pointIndex][0];
      clusterCenter[1] += points[pointIndex][1];
   }

   clusterCenter[0] /= targetPoints;
   clusterCenter[1] /= targetPoints;

   // Calculate extents needed to contain all selected points
   double maxExtentX = 0.0, maxExtentY = 0.0;
   for (size_t idx : selectedIndices) {
      const double dx = std::abs(points[idx][0] - clusterCenter[0]);
      const double dy = std::abs(points[idx][1] - clusterCenter[1]);
      maxExtentX = std::max(maxExtentX, dx);
      maxExtentY = std::max(maxExtentY, dy);
   }

   // Add minimum size constraint - but much smaller since we're in plot coordinates
   const double minExtent = 2.0;  // Reduced from 10.0
   maxExtentX = std::max(maxExtentX, minExtent);
   maxExtentY = std::max(maxExtentY, minExtent);

   Vector_2 extents(maxExtentX, maxExtentY);
   return InsetData(clusterCenter, extents, selectedIndices);
}

// Clean inset rendering - everything is simple and contained
std::string writeInsetContent(const InsetData& insetData, const size_t whichCoordinate,
   const std::vector<PlottedPolar>& plottedData, const ColorRange& colRange,
   const double insetSize) {

   if (insetData.isEmpty()) {
      return "";
   }

   const double contentSize = insetSize * 0.95;
   const double centerX = insetSize / 2.0;
   const double centerY = insetSize / 2.0;

   std::string content = "<g transform=\"translate(" + LRL_DataToSVG(centerX) + "," +
      LRL_DataToSVG(centerY) + ")\">\n";

   // Calculate scale to fit the actual data extents properly - simple and consistent
   const double maxExtent = std::max(insetData.extents[0], insetData.extents[1]);
   const double scale = (contentSize * 0.4) / maxExtent;  // Always use 80% of content radius

   // Y-flip transform to match main plot
   content += "<g transform=\"scale(1,-1)\">\n";

   // Draw only the selected points, centered properly
   for (size_t idx : insetData.pointIndices) {
      const PlottedPolar& plotted = plottedData[idx];
      const Vector_2& coords = plotted[whichCoordinate - 1];

      // Simple centering: subtract cluster center and scale
      const double insetX = (coords[0] - insetData.center[0]) * scale;
      const double insetY = (coords[1] - insetData.center[1]) * scale;

      // Get color
      unsigned long r, g, b;
      const double frac = (plottedData.size() == 1) ? 1.0 : double(idx) / double(plottedData.size() - 1);
      colRange.GetRGBFromRangeFraction(frac, r, g, b);

      // Draw the point (smaller radius for inset) - use higher precision
      std::ostringstream xStream, yStream;
      xStream << std::fixed << std::setprecision(2) << insetX;
      yStream << std::fixed << std::setprecision(2) << insetY;

      content += "<circle r=\"4\" fill=\"rgb(" + LRL_ToString(r) + "," +
         LRL_ToString(g) + "," + LRL_ToString(b) +
         ")\" cx=\"" + xStream.str() +
         "\" cy=\"" + yStream.str() + "\" " +
         "stroke=\"black\" stroke-width=\"0.5\"/>\n";
   }

   content += "</g>\n"; // End Y-flip
   content += "</g>\n"; // End content group
   return content;
}

// Clean zoom box rendering
std::string writeZoomBox(const InsetData& insetData) {
   if (insetData.isEmpty()) {
      return "";
   }

   const double margin = 0.05; // 5% margin
   const double marginX = insetData.extents[0] * margin;
   const double marginY = insetData.extents[1] * margin;

   const double x = insetData.minX() - marginX;
   const double y = insetData.minY() - marginY;
   const double width = insetData.width() + 2 * marginX;
   const double height = insetData.height() + 2 * marginY;
   
   // draw the zoom box
   return "<rect x=\"" + LRL_DataToSVG(x) + "\" y=\"" + LRL_DataToSVG(y) +
      "\" width=\"" + LRL_DataToSVG(width) + "\" height=\"" + LRL_DataToSVG(height) +
      "\" fill=\"none\" stroke=\"orange\" stroke-width=\"3\" stroke-dasharray=\"10,5\"/>\n";
}