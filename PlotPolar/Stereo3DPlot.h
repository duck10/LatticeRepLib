#ifndef STEREO3DPLOT_H
#define STEREO3DPLOT_H

#include <string>
#include <vector>

#include "LRL_Vector3.h"
#include "ColorTables.h"
#include "PlotPolarControls.h"
#include "Stereo3DData.h"

// Forward declarations
class Stereo3DData;

class Stereo3DPlot {
public:
   // Constructor - takes width, height, and filename
   Stereo3DPlot(int width, int height, const std::string& filename);

   // Main function to render complete stereo SVG
   std::string renderStereoSVG(const Stereo3DData& data,
      const PlotPolarControls& controls,
      const ColorRange& colorRange) const;

private:
   // Render a stereo pair (left and right eye views)
   std::string renderStereoPair(const std::vector<Vector_3>& leftEye,
      const std::vector<Vector_3>& rightEye,
      const ColorRange& colorRange,
      int xOffset, int yOffset,
      const std::string& title,
      const PlotPolarControls& controls) const;

   // Render a single 3D view
   std::string render3DView(const std::vector<Vector_3>& points,
      const ColorRange& colorRange,
      int xOffset, int yOffset, int viewSize,
      const PlotPolarControls& controls) const;

   // Helper functions
   double calculateScale(const std::vector<Vector_3>& points, int viewSize) const;
   std::string generateSVGHeader() const;
   std::string generateSVGFooter() const;

   // Project 3D point to 2D (you'll need to implement this based on your projection)
   std::pair<double, double> project2D(const Vector_3& point) const;

   // Member variables
   int m_width;
   int m_height;
   std::string m_filename;
};

#endif // STEREO3DPLOT_H

