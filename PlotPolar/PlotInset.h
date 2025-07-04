#ifndef PLOTINSET_H
#define PLOTINSET_H

#include <string>
#include <vector>
#include "ColorTables.h"
#include "PlottedPolar.h"
#include "LRL_ToString.h"
#include "LRL_DataToSVG.h"

class PlotInset {
public:
   struct InsetRegion {
      bool hasCluster;
      double minX;
      double maxX;
      double minY;
      double maxY;
      size_t pointCount;

      InsetRegion()
         : hasCluster(false)
         , minX(19191.0)
         , maxX(19191.0)
         , minY(19191.0)
         , maxY(19191.0)
         , pointCount(0)
      {
      }

      InsetRegion(const double minX_, const double minY_, const double maxX_, const double maxY_)
         : hasCluster(true)
         , minX(minX_)
         , maxX(maxX_)
         , minY(minY_)
         , maxY(maxY_)
         , pointCount(0)
      {
      }

      bool isValid() const {
         return hasCluster && (minX != 19191.0) && (maxX != 19191.0) &&
            (minY != 19191.0) && (maxY != 19191.0) && (maxX > minX) && (maxY > minY);
      }

      double width() const { return maxX - minX; }
      double height() const { return maxY - minY; }
      double centerX() const { return (minX + maxX) / 2.0; }
      double centerY() const { return (minY + maxY) / 2.0; }

      friend std::ostream& operator<<(std::ostream& os, const InsetRegion& region) {
         os << "InsetRegion: hasCluster=" << (region.hasCluster ? "true" : "false")
            << " bounds=(" << region.minX << "," << region.minY
            << ") to (" << region.maxX << "," << region.maxY
            << ") points=" << region.pointCount;
         return os;
      }
   };

   // Default constructor creates zero-size (disabled) inset
   PlotInset();

   // Constructor for active inset
   PlotInset(const double x, const double y, const double size, const InsetRegion& region);

   // Enable/disable the inset
   void setEnabled(const bool enabled) { m_enabled = enabled; }
   void setRegion(const InsetRegion& region) { m_region = region; }
   void setPosition(const double x, const double y) { m_x = x; m_y = y; }
   void setSize(const double size) { m_size = size; }

   // Getters
   bool isEnabled() const { return m_enabled && m_size > 0.0 && m_region.isValid(); }
   double getX() const { return m_x; }
   double getY() const { return m_y; }
   double getSize() const { return m_size; }
   const InsetRegion& getRegion() const { return m_region; }

   // SVG generation - NO MORE cellScale parameter needed!
   std::string writeSVG(const size_t whichCoordinate, const std::vector<PlottedPolar>& plottedData,
      const ColorRange& colRange) const;

   // Create zoom box for the main plot - NO MORE cellScale parameter!
   std::string writeZoomBox() const;

   // Auto-detect cluster region - works in plot coordinates directly
   static InsetRegion autoDetectCluster(const size_t whichCoordinate, const std::vector<PlottedPolar>& plottedData);

   friend std::ostream& operator<<(std::ostream& os, const PlotInset& inset) {
      os << "PlotInset: enabled=" << (inset.m_enabled ? "true" : "false")
         << " pos=(" << inset.m_x << "," << inset.m_y << ") size=" << inset.m_size
         << " region=" << inset.m_region;
      return os;
   }

   void autoDetectInset(const double insetX, const double insetY, const double insetSize);

private:
   bool m_enabled;
   double m_x;         // Position in plot coordinates
   double m_y;
   double m_size;      // Size of inset box
   InsetRegion m_region; // Region to zoom into (in plot coordinates)

   std::string writeInsetContent(const size_t whichCoordinate, const std::vector<PlottedPolar>& plottedData,
      const ColorRange& colRange) const;
};

#endif // PLOTINSET_H
