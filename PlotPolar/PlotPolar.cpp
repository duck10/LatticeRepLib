#include "PlotPolar.h"
#include "ColorTables.h"
#include "FileOperations.h"
#include "LRL_CreateFileName.h"
#include "LRL_DataToSVG.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_ToString.h"
#include "S6.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

static std::string NumberToHexString(const int n) {
   std::stringstream ostr;
   ostr << std::hex << std::setfill('0') << std::setw(6) << n;
   return ostr.str();
}

double PlotPolar::CellScale(const std::vector<Polar>& v) {
   double maxs = -DBL_MAX;
   for (size_t i = 0; i < v.size(); ++i) {
      for (size_t kk = 0; kk < 3; ++kk) {
         maxs = fmax(maxs, fmax(abs(v[i][kk][0]), abs(v[i][kk][1])));
      }
   }
   m_maxScalar = maxs;
   return maxs;
}

double PlotPolar::CellScaleFactor() const {
   return 0.9 * m_gx / m_maxScalar;
}

std::string PlotPolar::DrawCells(const size_t scalar, const std::vector<Polar>& vPolar, const ColorRange& colRange) {
   CellScale(vPolar);
   const double cellScale = CellScaleFactor();
   const ScalarProperties sp(scalar);

   const std::string scale = LRL_ToString(1);
   std::string transform = "<g transform = \"translate( "
      + LRL_DataToSVG(0) + ", " + LRL_DataToSVG(0)
      + ") scale(1 -1)\" >\n" + "\n";
   for (size_t i = 0; i < vPolar.size(); ++i) {
      const Polar pi = vPolar[i];
      const Vector_2 si = pi[scalar - 1];
      const double& xcoord = si[0];
      const double& ycoord = si[1];
      const std::string x = LRL_DataToSVG_ToQuotedString(xcoord * cellScale);
      const std::string y = LRL_DataToSVG_ToQuotedString(ycoord * cellScale);
      const std::string radius = LRL_DataToSVG_ToQuotedString(3);

      unsigned long r;
      unsigned long g;
      unsigned long b;
      const double frac = (vPolar.size() == 1) ? 1.0 : double(i) / double(vPolar.size() - 1);
      colRange.GetRGBFromRangeFraction(frac, r, g, b);

      const std::string s = std::string("<circle  r=\"10\" stroke = \"black\" stroke-width=\"0.5\""
         " fill= \"rgb(" + LRL_ToString(r) + ", " + LRL_ToString(g) + ", " + LRL_ToString(b) + ")\""
         " cx=" + x +
         " cy=" + y + "/>\n");
      transform += s;

      m_color += m_deltaColor % 256;
   }
   transform += "</g> <!--End of transform -->\n\n";
   return transform;
}

PlotPolar::ZoomRegion PlotPolar::AutoDetectClusterRegion(const size_t whichPlot, const std::vector<Polar>& vData) const {
   ZoomRegion region;

   if (vData.size() < 5) {
      region.hasCluster = false;
      return region;
   }

   // Extract coordinates for this plot
   std::vector<std::pair<double, double>> points;
   for (const Polar& polar : vData) {
      const Vector_2& coords = polar[whichPlot - 1];
      points.emplace_back(coords[0], coords[1]);
   }

   // Find the densest region using a sliding window approach
   const size_t minPointsForCluster = std::max(size_t(5), vData.size() / 4);

   double bestDensity = 0.0;
   double bestMinX = 19191.0;
   double bestMaxX = 19191.0;
   double bestMinY = 19191.0;
   double bestMaxY = 19191.0;
   size_t bestCount = 0;

   // Get data bounds
   double dataMinX = points[0].first;
   double dataMaxX = points[0].first;
   double dataMinY = points[0].second;
   double dataMaxY = points[0].second;

   for (const std::pair<double, double>& point : points) {
      dataMinX = std::min(dataMinX, point.first);
      dataMaxX = std::max(dataMaxX, point.first);
      dataMinY = std::min(dataMinY, point.second);
      dataMaxY = std::max(dataMaxY, point.second);
   }

   // Try different window sizes
   const double dataWidth = dataMaxX - dataMinX;
   const double dataHeight = dataMaxY - dataMinY;

   for (double windowSize = 0.2; windowSize <= 0.8; windowSize += 0.1) {
      const double windowWidth = dataWidth * windowSize;
      const double windowHeight = dataHeight * windowSize;

      // Slide window across data space
      for (double x = dataMinX; x <= dataMaxX - windowWidth; x += windowWidth * 0.1) {
         for (double y = dataMinY; y <= dataMaxY - windowHeight; y += windowHeight * 0.1) {
            size_t pointsInWindow = 0;

            for (const std::pair<double, double>& point : points) {
               if (point.first >= x && point.first <= x + windowWidth &&
                  point.second >= y && point.second <= y + windowHeight) {
                  pointsInWindow++;
               }
            }

            const double density = double(pointsInWindow) / (windowWidth * windowHeight);

            if (pointsInWindow >= minPointsForCluster && density > bestDensity) {
               bestDensity = density;
               bestMinX = x;
               bestMaxX = x + windowWidth;
               bestMinY = y;
               bestMaxY = y + windowHeight;
               bestCount = pointsInWindow;
            }
         }
      }
   }

   if (bestCount >= minPointsForCluster) {
      region.hasCluster = true;
      region.minX = bestMinX;
      region.maxX = bestMaxX;
      region.minY = bestMinY;
      region.maxY = bestMaxY;
      region.pointCount = bestCount;
   }

   return region;
}

PlotPolar::PlotPolar(const std::string& filename, const int wx, const int wy, const int gx, const int gy)
   : m_wx(wx)
   , m_wy(wy)
   , m_gx(gx)
   , m_gy(gy)
   , m_svgIntro(BuildIntro(filename))
   , m_svgFoot("\n</g> <!--end of scale=0.6-->\n</svg>")
{
}

std::string PlotPolar::BuildIntro(const std::string& filename) const {
   const std::string swx = LRL_DataToSVG_ToQuotedString(m_wx);
   const std::string swy = LRL_DataToSVG_ToQuotedString(m_wy);

   return
      "<svg width=" + swx + " height=" + swy +
      " viewBox=\"1 0  " + LRL_DataToSVG(m_wx, " ", m_wy) +
      "\"  version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" \nxmlns:xlink=\"http://www.w3.org/1999/xlink\">" +
      "\n<desc>File name  = " + filename + " </desc> """ +
      "\n <g transform=\"scale(0.6)\">\n";
}

std::vector<S6> PlotPolar::PrepareCells() const {
   std::vector<S6> v;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   for (size_t i = 0; i < inputList.size(); ++i) {
      v.emplace_back(inputList[i].GetCell());
   }
   return v;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void PlotPolar::SendFrameToFile(const std::string& sFileName, const std::string& data) const {
   std::ofstream fileout;
   FileOperations::OpenOutputFile(fileout, sFileName.c_str());

   if (fileout.is_open())
   {
      fileout.seekp(0);
      fileout << data << std::endl;
   }
   else
      std::cout << "; Could not open file " << sFileName << " for write in SVGWriter.h" << std::endl;

   fileout.close();
}

