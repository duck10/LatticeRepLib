// Stereo3DPlot.cpp - Enhanced version
#include "Stereo3DPlot.h"
#include "LRL_DataToSVG.h"
#include "LRL_ToString.h"
#include "Stereo3DData.h"

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>


Stereo3DPlot::Stereo3DPlot(int width, int height, const std::string& filename)
   : m_width(width), m_height(height), m_filename(filename) {
}


std::string Stereo3DPlot::renderStereoPair(const std::vector<Vector_3>& leftEye,
   const std::vector<Vector_3>& rightEye,
   const ColorRange& colorRange,
   int xOffset, int yOffset,
   const std::string& title,
   const PlotPolarControls& controls) const {

   std::string svg;
   
   // Use controls for sizing if available, otherwise use reasonable defaults
   int viewSize = std::min(m_width / 2 - 50, m_height / 2 - 50);  
   int individualViewSize = (viewSize / 2) + 10;  // Moderate size increase

   // Left eye view
   svg += render3DView(leftEye, colorRange, xOffset, yOffset + 30, individualViewSize, controls);

   // Right eye view - tighter spacing
   svg += render3DView(rightEye, colorRange, xOffset + individualViewSize + 8, yOffset + 30, individualViewSize, controls);

   //// Left Eye label - smaller, gray, positioned above left view
   //int leftViewCenterX = xOffset + individualViewSize / 2;
   //svg += "<text x=\"" + LRL_DataToSVG(leftViewCenterX) + "\" y=\"" + LRL_DataToSVG(yOffset + 15) + "\" " +
   //   "text-anchor=\"middle\" font-size=\"9\" " +
   //   "font-family=\"Arial, Helvetica, sans-serif\" fill=\"gray\">Left Eye</text>\n";

   //// Right Eye label - smaller, gray, positioned above right view
   //int rightViewCenterX = xOffset + individualViewSize + 8 + individualViewSize / 2;
   //svg += "<text x=\"" + LRL_DataToSVG(rightViewCenterX) + "\" y=\"" + LRL_DataToSVG(yOffset + 15) + "\" " +
   //   "text-anchor=\"middle\" font-size=\"9\" " +
   //   "font-family=\"Arial, Helvetica, sans-serif\" fill=\"gray\">Right Eye</text>\n";

   // Projection title - smaller, gray, positioned alongside the stereo pair  
   int stereoPairCenterY = yOffset + individualViewSize / 2;
   int titleX = xOffset + (individualViewSize * 2) + 20;  // Right of the stereo pair
   svg += "<text x=\"" + LRL_DataToSVG(titleX) + "\" y=\"" + LRL_DataToSVG(stereoPairCenterY) + "\" " +
      "text-anchor=\"start\" font-size=\"11\" " +
      "font-family=\"Arial, Helvetica, sans-serif\" fill=\"gray\" " +
      "transform=\"rotate(90, " + LRL_DataToSVG(titleX) + ", " + LRL_DataToSVG(stereoPairCenterY) + ")\">" + title + "</text>\n";

   return svg;
}

std::string Stereo3DPlot::renderStereoSVG(const Stereo3DData& data,
   const PlotPolarControls& controls,
   const ColorRange& colorRange) const {

   std::string svg = generateSVGHeader();

   // Get stereo angle from controls
   double stereoAngle = controls.getStereoAngle();

   // Calculate layout for moderate-sized plots
   int viewSize = std::min(m_width / 2 - 50, m_height / 2 - 50);
   int individualViewSize = (viewSize / 2) + 10;
   int verticalSpacing = individualViewSize + 60;

   // Generate stereo data with actual stereo angle from controls
   auto [proj1Left, proj1Right] = data.generateStereoPair(1, stereoAngle);
   auto [proj2Left, proj2Right] = data.generateStereoPair(2, stereoAngle);

   // Cross-eyed stereo (left side)
   svg += renderStereoPair(proj1Left, proj1Right, colorRange,
      25, 35, "Projection 1 (Cross-Eyed)", controls);

   svg += renderStereoPair(proj2Left, proj2Right, colorRange,
      25, 35 + verticalSpacing, "Projection 2 (Cross-Eyed)", controls);

   // Parallel stereo (right side)
   int rightSideX = m_width / 2 + 25;

   svg += renderStereoPair(proj1Right, proj1Left, colorRange,
      rightSideX, 35, "Projection 1 (Parallel)", controls);

   svg += renderStereoPair(proj2Right, proj2Left, colorRange,
      rightSideX, 35 + verticalSpacing, "Projection 2 (Parallel)", controls);

   svg += generateSVGFooter();
   return svg;
}



std::string Stereo3DPlot::render3DView(const std::vector<Vector_3>& points,
   const ColorRange& colorRange,
   int xOffset, int yOffset, int viewSize,
   const PlotPolarControls& controls) const {

   if (points.empty()) {
      return "<!-- No points to render -->\n";
   }

   std::string svg;

   // Calculate scaling
   double scale = calculateScale(points, viewSize);

   // Center of the view
   double centerX = xOffset + viewSize / 2.0;
   double centerY = yOffset + viewSize / 2.0;

   // Background rectangle with 4px border
   svg += "<rect x=\"" + LRL_DataToSVG(xOffset) + "\" y=\"" + LRL_DataToSVG(yOffset) + "\" " +
      "width=\"" + LRL_DataToSVG(viewSize) + "\" height=\"" + LRL_DataToSVG(viewSize) + "\" " +
      "fill=\"white\" stroke=\"lightgray\" stroke-width=\"4\"/>\n";

   // Draw axes - proportional to view size
   svg += "<!-- 3D View Axes -->\n";
   double axisLen = viewSize * 0.12;

   // X-axis (red)
   svg += "<line x1=\"" + LRL_DataToSVG(centerX) + "\" y1=\"" + LRL_DataToSVG(centerY) + "\" " +
      "x2=\"" + LRL_DataToSVG(centerX + axisLen) + "\" y2=\"" + LRL_DataToSVG(centerY) + "\" " +
      "stroke=\"red\" stroke-width=\"1.5\"/>\n";
   svg += "<text x=\"" + LRL_DataToSVG(centerX + axisLen + 3) + "\" y=\"" + LRL_DataToSVG(centerY + 3) + "\" " +
      "font-size=\"10\" fill=\"red\">X</text>\n";

   // Z-axis (blue)
   svg += "<line x1=\"" + LRL_DataToSVG(centerX) + "\" y1=\"" + LRL_DataToSVG(centerY) + "\" " +
      "x2=\"" + LRL_DataToSVG(centerX) + "\" y2=\"" + LRL_DataToSVG(centerY - axisLen) + "\" " +
      "stroke=\"blue\" stroke-width=\"1.5\"/>\n";
   svg += "<text x=\"" + LRL_DataToSVG(centerX + 2) + "\" y=\"" + LRL_DataToSVG(centerY - axisLen - 2) + "\" " +
      "font-size=\"10\" fill=\"blue\">Z</text>\n";

   // Draw background grid using direct SVG coordinates - much simpler!
   svg += "<!-- Background Grid -->\n";

   // Grid goes directly on the SVG coordinate system, no transforms needed
   int gridLines = 8;  // Number of interior grid lines
   double gridSpacing = viewSize / (gridLines + 1.0);  // Space between lines

   // Create grid as paths using the actual frame boundaries
   std::string horizontalPath = "<path d=\"";
   std::string verticalPath = "<path d=\"";

   // Frame boundaries for the grid
   int left = xOffset;
   int right = xOffset + viewSize;
   int top = yOffset;
   int bottom = yOffset + viewSize;

   // Add horizontal lines (including top and bottom boundaries)
   for (int i = 0; i <= gridLines + 1; ++i) {
      int y = int(top + i * gridSpacing);
      horizontalPath += "M" + LRL_DataToSVG(left) + "," + LRL_DataToSVG(y) +
         " L" + LRL_DataToSVG(right) + "," + LRL_DataToSVG(y) + " ";
   }

   // Add vertical lines (including left and right boundaries)
   for (int i = 0; i <= gridLines + 1; ++i) {
      int x = int(left + i * gridSpacing);
      verticalPath += "M" + LRL_DataToSVG(x) + "," + LRL_DataToSVG(top) +
         " L" + LRL_DataToSVG(x) + "," + LRL_DataToSVG(bottom) + " ";
   }

   horizontalPath += "\" stroke=\"#e0e0e0\" stroke-width=\"0.5\" opacity=\"0.4\" fill=\"none\">\n";
   horizontalPath += "<title>Horizontal grid lines for depth reference</title>\n";
   horizontalPath += "</path>\n";

   verticalPath += "\" stroke=\"#e0e0e0\" stroke-width=\"0.5\" opacity=\"0.4\" fill=\"none\">\n";
   verticalPath += "<title>Vertical grid lines for depth reference</title>\n";
   verticalPath += "</path>\n";

   svg += horizontalPath;
   svg += verticalPath;

   // Draw points with size from controls
   svg += "<!-- 3D Data Points -->\n";
   svg += "<g transform=\"translate(" + LRL_DataToSVG(centerX) + "," + LRL_DataToSVG(centerY) + ") scale(1,-1)\">\n";

   // Calculate point radius - use controls if available
   double baseRadius = std::max(3.0, viewSize * 0.025);
   double pointRadius = baseRadius * controls.getPointScale3D();

   for (size_t i = 0; i < points.size(); ++i) {
      auto [x2d, y2d] = project2D(points[i]);

      // Scale and position
      double scaledX = x2d * scale;
      double scaledY = y2d * scale;

      // Get color based on point index
      unsigned long r, g, b;
      double frac = (points.size() == 1) ? 1.0 : double(i) / double(points.size() - 1);
      colorRange.GetRGBFromRangeFraction(frac, r, g, b);

      svg += "<circle r=\"" + LRL_DataToSVG(pointRadius) + "\" fill=\"rgb(" + LRL_ToString(r) + "," +
         LRL_ToString(g) + "," + LRL_ToString(b) + ")\" " +
         "cx=\"" + LRL_DataToSVG(scaledX) + "\" cy=\"" + LRL_DataToSVG(scaledY) + "\" " +
         "stroke=\"black\" stroke-width=\"0.5\"/>\n";
   }

   svg += "</g>\n";

   return svg;
}


double Stereo3DPlot::calculateScale(const std::vector<Vector_3>& points, int viewSize) const {
   if (points.empty()) return 1.0;

   // Find max extent in X and Z
   double maxExtent = 0.0;

   for (const Vector_3& point : points) {
      auto [x, z] = project2D(point);
      maxExtent = std::max(maxExtent, std::max(std::abs(x), std::abs(z)));
   }

   if (maxExtent == 0.0) return 1.0;

   // Use 80% of view size for data
   return (viewSize * 0.40) / maxExtent;
}

std::string Stereo3DPlot::generateSVGHeader() const {
   std::ostringstream svg;
   svg << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
   svg << "<svg width=\"" << m_width << "\" height=\"" << m_height << "\" ";
   svg << "viewBox=\"0 0 " << m_width << " " << m_height << "\" ";
   svg << "xmlns=\"http://www.w3.org/2000/svg\">\n";
   svg << "<desc>3D Stereo Polar Plot - " << m_filename << "</desc>\n";

   // Title
   svg << "<text x=\"" << m_width / 2 << "\" y=\"15\" text-anchor=\"middle\" ";
   svg << "font-size=\"18\" font-weight=\"bold\" ";
   svg << "font-family=\"Arial, Helvetica, sans-serif\">";
   svg << "3D Stereo Polar Projections</text>\n";

   return svg.str();
}

std::string Stereo3DPlot::generateSVGFooter() const {
   return "</svg>\n";
}

std::pair<double, double> Stereo3DPlot::project2D(const Vector_3& point) const {
   // Simple orthographic projection: project 3D (x,y,z) to 2D (x,z)
   // This assumes Y is the "up" direction and we're looking along the Y axis
   // You can modify this projection based on your coordinate system

   return std::make_pair(point[0], point[2]);  // X and Z coordinates

}

// Usage example for main():
/*
// Add to main() after existing polar plots:
if (controls.shouldShowDetails() || controls.getEnable3D()) {
    std::cout << "; Generating 3D stereo plots..." << std::endl;
    std::cout << "; Using stereo angle: " << controls.getStereoAngle() << " degrees" << std::endl;

    Stereo3DData stereoData(vData);
    Stereo3DPlot stereoPlot(1000, 800, "stereo3d_output.svg");  // Larger canvas

    std::string stereoSVG = stereoPlot.renderStereoSVG(stereoData, controls, colorRange);

    // Write to file
    std::ofstream stereoFile("stereo3d_output.svg");
    if (stereoFile.is_open()) {
        stereoFile << stereoSVG;
        std::cout << "; 3D stereo SVG written to stereo3d_output.svg" << std::endl;
    }
}
*/