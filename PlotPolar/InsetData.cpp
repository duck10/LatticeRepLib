#include "InsetData.h"
#include <iomanip>

// Clean cluster detection - returns InsetData directly
// Replace the autoDetectCluster function in InsetData.cpp with this:

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

   // Find data bounds
   double minX = points[0][0], maxX = points[0][0];
   double minY = points[0][1], maxY = points[0][1];

   for (const Vector_2& point : points) {
      minX = std::min(minX, point[0]);
      maxX = std::max(maxX, point[0]);
      minY = std::min(minY, point[1]);
      maxY = std::max(maxY, point[1]);
   }

   // Find the center of data
   double centerX = (minX + maxX) / 2.0;
   double centerY = (minY + maxY) / 2.0;

   // Start with a small region and expand until we get roughly the target percentage
   double regionWidth = (maxX - minX) * 0.2;   // Start with 20% of data range
   double regionHeight = (maxY - minY) * 0.2;

   const size_t targetPoints = static_cast<size_t>(plottedData.size() * clusterPercent);

   // Expand region until we capture enough points
   for (int iterations = 0; iterations < 20; ++iterations) {
      // Count points in current region
      std::vector<size_t> pointsInRegion;

      double regMinX = centerX - regionWidth / 2.0;
      double regMaxX = centerX + regionWidth / 2.0;
      double regMinY = centerY - regionHeight / 2.0;
      double regMaxY = centerY + regionHeight / 2.0;

      for (size_t i = 0; i < points.size(); ++i) {
         if (points[i][0] >= regMinX && points[i][0] <= regMaxX &&
            points[i][1] >= regMinY && points[i][1] <= regMaxY) {
            pointsInRegion.push_back(i);
         }
      }

      // If we have enough points, we're done
      if (pointsInRegion.size() >= targetPoints) {
         Vector_2 center(centerX, centerY);
         Vector_2 extents(regionWidth / 2.0, regionHeight / 2.0);
         return InsetData(center, extents, pointsInRegion);
      }

      // Expand region for next iteration
      regionWidth *= 1.2;
      regionHeight *= 1.2;
   }

   // Fallback - return empty if we couldn't find a good region
   return InsetData();
}


// Clean inset rendering - everything is simple and contained
std::string writeInsetContent(const InsetData& insetData, const size_t whichCoordinate,
   const std::vector<PlottedPolar>& plottedData, const ColorRange& colRange,
   const double insetSize) {

   if (insetData.isEmpty()) {
      return "";
   }

   const double centerX = insetSize / 2.0;
   const double centerY = insetSize / 2.0;

   std::string content = "<g transform=\"translate(" + LRL_DataToSVG(centerX) + "," +
      LRL_DataToSVG(centerY) + ")\">\n";

   // Calculate scale to fit the actual data extents properly - simple and consistent
   const double maxExtent = std::max(insetData.extents[0], insetData.extents[1]);
   const double scale = (insetSize * .45) / maxExtent;  // scale for "radius" 1.0 would fill inset box

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