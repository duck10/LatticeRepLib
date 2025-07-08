#ifndef CMD_PLOT_POLAR_CONTROLS_H
#define CMD_PLOT_POLAR_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"

static const int DEFAULT_INSETSIZE = 270;
static const int MININSETSIZE = 0;
static const int MAXINSETSIZE = 1000;

static int validateInsetSize(const int value) {
   if (value < MININSETSIZE || value > MAXINSETSIZE) {
      std::cerr << ";Warning: Point count " << value << " out of range ["
         << MININSETSIZE << "," << MAXINSETSIZE << "], using "
         << DEFAULT_INSETSIZE << std::endl;
      return DEFAULT_INSETSIZE;
   }
   return value;
}

static const double MIN_FRAC = 0.1;
static const double MAX_FRAC = 1.0;
static const double DEFAULT_FRAC = 0.6;

static double validateClusterFraction(const std::string& value) {
   try {
      double amount = std::stod(value);
      if (amount < MIN_FRAC || amount > MAX_FRAC) {
         std::cerr << ";Warning: Cluster fraction " << amount
            << " out of range [" << MIN_FRAC
            << "," << MAX_FRAC << "], using "
            << DEFAULT_FRAC << std::endl;
         return DEFAULT_FRAC;
      }
      return amount;
   }
   catch (...) {
      std::cerr << ";Warning: Invalid perturb amount '" << value
         << "', using " << DEFAULT_FRAC << std::endl;
      return DEFAULT_FRAC;
   }
}

// Enhanced validation functions for 3D controls
static double validateStereoAngle(const std::string& value, double defaultVal = 10.0) {
   try {
      double angle = std::stod(value);
      if (angle >= 0.0 && angle <= 45.0) {
         return angle;
      }
      else {
         std::cerr << ";Warning: Stereo angle " << angle << " out of range [0,45], using " << defaultVal << std::endl;
         return defaultVal;
      }
   }
   catch (...) {
      std::cerr << ";Warning: Invalid stereo angle '" << value << "', using " << defaultVal << std::endl;
      return defaultVal;
   }
}

static int validateCanvasSize(const std::string& value, int defaultVal, int minVal = 400, int maxVal = 2000) {
   try {
      int size = std::stoi(value);
      if (size >= minVal && size <= maxVal) {
         return size;
      }
      else {
         std::cerr << ";Warning: Canvas size " << size << " out of range [" << minVal << "," << maxVal << "], using " << defaultVal << std::endl;
         return defaultVal;
      }
   }
   catch (...) {
      std::cerr << ";Warning: Invalid canvas size '" << value << "', using " << defaultVal << std::endl;
      return defaultVal;
   }
}

static double validatePointScale(const std::string& value, double defaultVal = 1.0) {
   try {
      double scale = std::stod(value);
      if (scale >= 0.1 && scale <= 5.0) {
         return scale;
      }
      else {
         std::cerr << ";Warning: Point scale " << scale << " out of range [0.1,5.0], using " << defaultVal << std::endl;
         return defaultVal;
      }
   }
   catch (...) {
      std::cerr << ";Warning: Invalid point scale '" << value << "', using " << defaultVal << std::endl;
      return defaultVal;
   }
}

class PlotPolarControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const PlotPolarControls& ctrl) {
      os << "; PlotPolarControls\n";
      os << "; Inset size (INSETSIZE) " << ctrl.insetSize << std::endl;
      os << "; Cluster inclusion fraction (CLUSTERFRACTION) " << ctrl.m_clusterPercent << std::endl;
      os << "; Show details " << ctrl.m_showDetails << std::endl;
      os << "; Enable 3D (ENABLE3D) " << ctrl.m_enable3D << std::endl;
      os << "; Stereo angle (STEREOANGLE) " << ctrl.m_stereoAngle << " degrees" << std::endl;
      os << "; 3D Canvas width (CANVAS3DWIDTH) " << ctrl.m_canvas3DWidth << std::endl;
      os << "; 3D Canvas height (CANVAS3DHEIGHT) " << ctrl.m_canvas3DHeight << std::endl;
      os << "; 3D Point scale (POINTSCALE3D) " << ctrl.m_pointScale3D << std::endl;
      if (!ctrl.m_title.empty()) {
         os << "; Title: (TITLE) " << ctrl.m_title << std::endl;
      }
      return os;
   }

   PlotPolarControls() {

      InputHandler::registerHandler("INSETSIZE", 0.35,
         [this](BaseControlVariables&, const std::string& value) {
            insetSize = validateInsetSize(stoi(value));
         });

      InputHandler::registerHandler("DETAILS", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            setShowDetails(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

      InputHandler::registerHandler("CLUSTERFRACTION", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            m_clusterPercent = validateClusterFraction(value);
         });

      InputHandler::registerHandler("TITLE", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            m_title = convertFromAllCaps(value);
         });

      InputHandler::registerHandler("ENABLE3D", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            setEnable3D(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

      InputHandler::registerHandler("STEREOANGLE", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            m_stereoAngle = validateStereoAngle(value, m_stereoAngle);
         });

      // New enhanced 3D controls
      InputHandler::registerHandler("CANVAS3DWIDTH", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            m_canvas3DWidth = validateCanvasSize(value, m_canvas3DWidth, 600, 2000);
         });

      InputHandler::registerHandler("CANVAS3DHEIGHT", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            m_canvas3DHeight = validateCanvasSize(value, m_canvas3DHeight, 400, 1600);
         });

      InputHandler::registerHandler("POINTSCALE3D", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            m_pointScale3D = validatePointScale(value, m_pointScale3D);
         });

      InputHandler::registerHandler("VIEWSIZE3D", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            m_viewSize3D = validateCanvasSize(value, m_viewSize3D, 100, 500);
         });

      // Advanced stereo controls
      InputHandler::registerHandler("CROSSEYED", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            m_showCrossEyed = value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty();
         });

      InputHandler::registerHandler("PARALLEL", 0.30,
         [this](const BaseControlVariables&, const std::string& value) {
            m_showParallel = value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty();
         });
   }

   // Utility function to convert ALL CAPS to Title Case
   std::string convertFromAllCaps(const std::string& input) const {
      if (input.empty()) return input;

      std::string result;
      result.reserve(input.length());

      bool capitalize = true;
      for (char c : input) {
         if (std::isspace(c)) {
            result += c;
            capitalize = true;
         }
         else if (capitalize) {
            result += std::toupper(c);
            capitalize = false;
         }
         else {
            result += std::tolower(c);
         }
      }

      return result;
   }

   // Existing getters
   std::string getPrefix() const { return prefix; }
   bool getEnableZoom() const { return enableZoom; }
   bool getAutoDetectZoom() const { return autoDetectZoom; }
   double getZoomMinX() const { return zoomMinX; }
   double getZoomMinY() const { return zoomMinY; }
   double getZoomMaxX() const { return zoomMaxX; }
   double getZoomMaxY() const { return zoomMaxY; }
   double getInsetSize() const { return insetSize; }
   bool shouldShowDetails() const { return m_showDetails; }
   void setShowDetails(const bool b) { m_showDetails = b; }
   double getClusterPercent() const { return m_clusterPercent; }
   void setClusterPercent(double percent) { m_clusterPercent = percent; }
   std::string getTitle() const { return m_title; }

   // 3D getters and setters
   bool getEnable3D() const { return m_enable3D; }
   void setEnable3D(bool enable) { m_enable3D = enable; }
   double getStereoAngle() const { return m_stereoAngle; }
   void setStereoAngle(double angle) {
      if (angle >= 0.0 && angle <= 45.0) {
         m_stereoAngle = angle;
      }
   }

   // New enhanced 3D getters
   int getCanvas3DWidth() const { return m_canvas3DWidth; }
   int getCanvas3DHeight() const { return m_canvas3DHeight; }
   double getPointScale3D() const { return m_pointScale3D; }
   int getViewSize3D() const { return m_viewSize3D; }
   bool getShowCrossEyed() const { return m_showCrossEyed; }
   bool getShowParallel() const { return m_showParallel; }

   // Setters for programmatic control
   void setCanvas3DSize(int width, int height) {
      m_canvas3DWidth = width;
      m_canvas3DHeight = height;
   }
   void setPointScale3D(double scale) {
      if (scale >= 0.1 && scale <= 5.0) {
         m_pointScale3D = scale;
      }
   }

private:
   const std::string prefix = "PPL";
   bool enableZoom = true;
   bool autoDetectZoom = true;
   double zoomMinX = 19191.0;
   double zoomMinY = 19191.0;
   double zoomMaxX = 19191.0;
   double zoomMaxY = 19191.0;
   bool m_showDetails = false;
   double insetSize = 270;
   double m_clusterPercent = 0.6;  // Default 60%
   std::string m_title;

   // Enhanced 3D controls
   bool m_enable3D = false;             // Default enabled
   double m_stereoAngle = 10.0;        // Default 10 degrees
   int m_canvas3DWidth = 1200;         // Default larger canvas
   int m_canvas3DHeight = 900;         // Default larger canvas
   double m_pointScale3D = 1.0;        // Default point scaling
   int m_viewSize3D = 200;             // Default individual view size
   bool m_showCrossEyed = true;        // Show cross-eyed stereo
   bool m_showParallel = true;         // Show parallel stereo
};

#endif // CMD_PLOT_POLAR_CONTROLS_H
