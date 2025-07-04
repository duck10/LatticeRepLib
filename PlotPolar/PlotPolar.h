#ifndef PLOTPOLAR_H
#define PLOTPOLAR_H

#include <string>
#include <vector>

#include "ColorTables.h"
#include "Polar.h"
#include "LRL_Vector3.h"

class PlotPolar {
public:
   struct ZoomRegion {
      bool hasCluster;
      double minX;
      double maxX;
      double minY;
      double maxY;
      size_t pointCount;

      ZoomRegion()
         : hasCluster(false)
         , minX(19191.0)
         , maxX(19191.0)
         , minY(19191.0)
         , maxY(19191.0)
         , pointCount(0)
      {
      }

      friend std::ostream& operator<<(std::ostream& os, const ZoomRegion& region) {
         os << "ZoomRegion: hasCluster=" << (region.hasCluster ? "true" : "false")
            << " bounds=(" << region.minX << "," << region.minY
            << ") to (" << region.maxX << "," << region.maxY
            << ") points=" << region.pointCount;
         return os;
      }
   };

   PlotPolar(const std::string& filename, const int wx, const int wy, const int gx, const int gy);

   std::string GetIntro(const std::string& filename) const { return m_svgIntro; }
   std::string GetFoot() const { return m_svgFoot; }
   double CellScale(const std::vector<Polar>& v);
   double CellScaleFactor() const;
   std::string DrawCells(const size_t scalar, const std::vector<Polar>& v, const ColorRange& colRange);
   ZoomRegion AutoDetectClusterRegion(const size_t whichPlot, const std::vector<Polar>& vData) const;
   double GetWx() const { return m_wx; }
   double GetWy() const { return m_wy; }
   double GetGx() const { return m_gx; }
   double GetGy() const { return m_gy; }
   std::vector<S6> PrepareCells() const;
   void SendFrameToFile(const std::string& sFileName, const std::string& data) const;

private:
   const int m_wx;
   const int m_wy;
   const int m_gx;
   const int m_gy;

   int m_color;
   int m_deltaColor;

   std::string m_svgIntro;
   std::string m_svgFoot;

   double m_maxScalar;

   std::string BuildIntro(const std::string& filename) const;
};

class ScalarProperties {

public:
   size_t index1;
   size_t index2;
   int moveX;
   int moveY;

   explicit ScalarProperties(const size_t scalar) {
      if (scalar == 1) {
         index1 = 0;
         index2 = 1;
         moveX = 0;
         moveY = 0;
      }
      else if (scalar == 2) {
         index1 = 1;
         index2 = 4;
         moveX = 550;
         moveY = 0;
      }
      else {
         index1 = 0;
         index2 = 1;
         moveX = 550;
         moveY = 0;
      }
   }

};

#endif  // PLOTPOLAR_H

