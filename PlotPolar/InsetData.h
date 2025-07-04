#ifndef INSETDATA_H
#define INSETDATA_H

#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

#include "PlotPolar.h"
#include "Vector_2.h"
#include "PlottedPolar.h"
#include "ColorTables.h"
#include "LRL_ToString.h"
#include "LRL_DataToSVG.h"

// Clean inset data structure - each inset knows its own data and bounds
struct InsetData {
   Vector_2 center;        // Center point of the cluster
   Vector_2 extents;       // Half-width and half-height from center
   std::vector<size_t> pointIndices;  // Which points from the main data to show

   InsetData() : center(0.0, 0.0), extents(0.0, 0.0) {}

   InsetData(const Vector_2& center_, const Vector_2& extents_, const std::vector<size_t>& indices_)
      : center(center_), extents(extents_), pointIndices(indices_) {
   }

   // Simple bounds calculation
   double minX() const { return center[0] - extents[0]; }
   double maxX() const { return center[0] + extents[0]; }
   double minY() const { return center[1] - extents[1]; }
   double maxY() const { return center[1] + extents[1]; }
   double width() const { return 2.0 * extents[0]; }
   double height() const { return 2.0 * extents[1]; }

   bool contains(const Vector_2& point) const {
      return point[0] >= minX() && point[0] <= maxX() &&
         point[1] >= minY() && point[1] <= maxY();
   }

   bool isEmpty() const {
      return pointIndices.empty() || (extents[0] == 0.0 && extents[1] == 0.0);
   }
};

// Clean cluster detection - returns InsetData directly
InsetData autoDetectCluster(const size_t whichCoordinate, const std::vector<PlottedPolar>& plottedData, double clusterPercent);

// Clean inset rendering - everything is simple and contained
std::string writeInsetContent(const InsetData& insetData, const size_t whichCoordinate,
   const std::vector<PlottedPolar>& plottedData, const ColorRange& colRange,
   const double insetSize);

// Clean zoom box rendering
std::string writeZoomBox(const InsetData& insetData);

#endif // INSETDATA_H