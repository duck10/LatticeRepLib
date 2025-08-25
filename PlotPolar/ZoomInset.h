#ifndef ZOOM_INSET_H
#define ZOOM_INSET_H

#include "InsetData.h"
#include "LRL_DataToSVG.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

class ZoomInset {
private:
   InsetData m_data;
   double m_insetX, m_insetY, m_insetSize;
   double m_magnification;
   bool m_enabled;

   std::string writeInsetContent(const std::vector<PlottedPolar>& plottedData,
      size_t coordinate, const ColorRange& colRange) const {
      return ::writeInsetContent(m_data, coordinate, plottedData, colRange, m_insetSize);
   }

   std::string writeConnectorLines() const {
      if (!m_enabled || m_data.isEmpty()) {
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
      const double margin = 0.05;
      const double marginX = m_data.extents[0] * margin;
      const double marginY = m_data.extents[1] * margin;

      points.emplace_back(m_data.minX() - marginX, -m_data.minY() + marginY, true);   // NW
      points.emplace_back(m_data.maxX() + marginX, -m_data.minY() + marginY, true);   // NE  
      points.emplace_back(m_data.maxX() + marginX, -m_data.maxY() - marginY, true);   // SE
      points.emplace_back(m_data.minX() - marginX, -m_data.maxY() - marginY, true);   // SW

      // Add inset corners - EXACT original coordinates
      points.emplace_back(m_insetX, m_insetY, false);                          // NW inset
      points.emplace_back(m_insetX + m_insetSize, m_insetY, false);            // NE inset
      points.emplace_back(m_insetX + m_insetSize, m_insetY + m_insetSize, false); // SE inset
      points.emplace_back(m_insetX, m_insetY + m_insetSize, false);            // SW inset

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
            // draw connector lines
            result += "<line x1=\"" + LRL_DataToSVG(hull[i].x) + "\" y1=\"" + LRL_DataToSVG(hull[i].y) +
               "\" x2=\"" + LRL_DataToSVG(hull[next].x) + "\" y2=\"" + LRL_DataToSVG(hull[next].y) +
               "\" stroke=\"orange\" stroke-width=\"1\" stroke-dasharray=\"15, 3, 5\"/>\n";
         }
      }

      return result;
   }

public:
   ZoomInset() : m_insetX(0), m_insetY(0), m_insetSize(0), m_magnification(0), m_enabled(false) {}

   ZoomInset(const std::vector<PlottedPolar>& plottedData, size_t coordinate,
      double insetX, double insetY, double insetSize, double clusterPercent) {
      m_data = autoDetectCluster(coordinate, plottedData, clusterPercent);
      m_insetX = insetX;
      m_insetY = insetY;
      m_insetSize = insetSize;

      if (!m_data.isEmpty()) {
         double maxExtent = std::max(m_data.extents[0], m_data.extents[1]);
         m_magnification = (insetSize * 0.4) / maxExtent;
         m_enabled = (m_magnification >= 1.001);
         m_enabled = true;
      }
      else {
         m_enabled = false;
         m_magnification = 0.0;
      }
   }

   std::string writeZoomBox() const {
      return ::writeZoomBox(m_data);  // Call the global function with InsetData parameter
   }

   bool isEnabled() const { return m_enabled; }

   std::string writeSVG(const std::vector<PlottedPolar>& plottedData,
      size_t coordinate, const ColorRange& colRange) const {
      if (!m_enabled) return "";

      std::string result;

      // ===== ZOOM INSET SECTION =====
      result += "<!-- ===== ZOOM INSET " + std::to_string(coordinate) + " ===== -->\n";
      result += "<g transform=\"translate(" + LRL_DataToSVG(m_insetX) + "," + LRL_DataToSVG(m_insetY) + ")\">\n";

      // Inset background and border
      result += "<!-- Inset background and border -->\n";
      result += "<rect x=\"0\" y=\"0\" width=\"" + LRL_DataToSVG(m_insetSize) + "\" height=\"" + LRL_DataToSVG(m_insetSize) + "\" fill=\"white\" stroke=\"orange\" stroke-width=\"2\"/>\n";

      // Labels
      result += "<!-- Inset labels -->\n";
      result += "<text x=\"135\" y=\"15\" text-anchor=\"middle\" font-size=\"15\" fill=\"orange\">Zoom View</text>\n";

      // Add magnification text
      std::ostringstream magStream;
      magStream << std::fixed << std::setprecision(1) << m_magnification << "x";
      result += "<text x=\"265\" y=\"265\" text-anchor=\"end\" font-size=\"30\" fill=\"blue\">" + magStream.str() + "</text>\n";

      // Add the inset content
      result += "<!-- Inset data points -->\n";
      result += writeInsetContent(plottedData, coordinate, colRange);
      result += "</g>\n";
      result += "<!-- End zoom inset " + std::to_string(coordinate) + " -->\n\n";

      // Add connector lines
      result += "<!-- ===== CONNECTOR LINES " + std::to_string(coordinate) + " ===== -->\n";
      result += writeConnectorLines();
      result += "<!-- End connector lines " + std::to_string(coordinate) + " -->\n\n";

      return result;
   }

   const InsetData& getData() const { return m_data; }
   double getMagnification() const { return m_magnification; }
   bool isEmpty() const { return !m_enabled; }
};

#endif // ZOOM_INSET_H

