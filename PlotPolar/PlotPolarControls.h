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


class PlotPolarControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const PlotPolarControls& ctrl) {
      os << "; PlotPolarControls\n";
      os << "; Inset size " << ctrl.insetSize << std::endl;
      os << "; Cluster inclusion fraction " << ctrl.m_clusterPercent << std::endl;
      if (!ctrl.m_title.empty()) {
         os << "; Title: " << ctrl.m_title << std::endl;
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
   std::string getTitle() const { return m_title; }  // Added getter for title

private:
   const std::string prefix = "PPL";
   bool enableZoom = true;        // Set to true for testing
   bool autoDetectZoom = true;
   double zoomMinX = 19191.0;
   double zoomMinY = 19191.0;
   double zoomMaxX = 19191.0;
   double zoomMaxY = 19191.0;
   bool m_showDetails = false;
   double insetSize = 270;
   double m_clusterPercent = 0.6;  // Default 60%
   std::string m_title;
};

#endif // CMD_PLOT_POLAR_CONTROLS_H

