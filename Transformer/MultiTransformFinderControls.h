#ifndef MULTITRANSFORMFINDERCONTROLS_H
#define MULTITRANSFORMFINDERCONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"
#include <iostream>
#include <string>

class MultiTransformFinderControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const MultiTransformFinderControls& mtfc) {
      os << "; MultiTransformFinderControls\n";
      os << "; show details " << (mtfc.m_showDetails ? "true" : "false") << std::endl;
      os << "; show extra transform info " << (mtfc.m_showExtraTransformInfo ? "true" : "false") << std::endl;
      os << "; matrix search depth " << mtfc.m_maxMatrixSearchDepth << std::endl;
      os << "; max transformations to show " << mtfc.m_maxTransformationsToShow << std::endl;
      os << "; max transformation distance " << mtfc.m_maxTransformationDistance << std::endl;
      os << "; distance threshold " << mtfc.m_distanceThreshold << std::endl;
      os << "; include identity matrix " << (mtfc.m_includeIdentityMatrix ? "true" : "false") << std::endl;
      return os;
   }

   MultiTransformFinderControls() {
      // Register handlers for control parameters
      InputHandler::registerHandler("SHOWDETAILS", 0.31,
         [this](const BaseControlVariables&, const std::string& value) {
            setShowDetails(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

      InputHandler::registerHandler("DETAILS", 0.32,
         [this](const BaseControlVariables&, const std::string& value) {
            setShowDetails(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

      InputHandler::registerHandler("SHOWTRANSFORMINFO", 0.33,
         [this](const BaseControlVariables&, const std::string& value) {
            setShowExtraTransformInfo(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

      InputHandler::registerHandler("MATRIXSEARCHDEPTH", 0.34,
         [this](const BaseControlVariables&, const std::string& value) {
            setMaxMatrixSearchDepth(std::stoi(value));
         });

      InputHandler::registerHandler("MAXTRANSFORMS", 0.35,
         [this](const BaseControlVariables&, const std::string& value) {
            setMaxTransformationsToShow(std::stoi(value));
         });

      InputHandler::registerHandler("MAXDISTANCE", 0.36,
         [this](const BaseControlVariables&, const std::string& value) {
            setMaxTransformationDistance(std::stod(value));
         });

      InputHandler::registerHandler("DISTANCETHRESHOLD", 0.37,
         [this](const BaseControlVariables&, const std::string& value) {
            setDistanceThreshold(std::stod(value));
         });

      InputHandler::registerHandler("INCLUDEIDENTITY", 0.38,
         [this](const BaseControlVariables&, const std::string& value) {
            setIncludeIdentityMatrix(value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

      InputHandler::registerHandler("SORTBY", 0.39,
         [this](const BaseControlVariables&, const std::string& value) {
            std::string upperValue = LRL_StringTools::strToupper(value);
            if (upperValue == "S6" || upperValue == "P3") {
               setSortBy(upperValue);
            }
            else {
               std::cout << "Warning: Unknown sort option: " << value << ". Using default (P3)." << std::endl;
               setSortBy("P3");
            }
         });
   }

   // Getters
   bool shouldShowDetails() const { return m_showDetails; }
   bool shouldShowExtraTransformInfo() const { return m_showExtraTransformInfo; }
   int getMaxMatrixSearchDepth() const { return m_maxMatrixSearchDepth; }
   int getMaxTransformationsToShow() const { return m_maxTransformationsToShow; }
   double getMaxTransformationDistance() const { return m_maxTransformationDistance; }
   double getDistanceThreshold() const { return m_distanceThreshold; }
   bool shouldIncludeIdentityMatrix() const { return m_includeIdentityMatrix; }

   // Setters
   void setShowDetails(const bool b) { m_showDetails = b; }
   void setShowExtraTransformInfo(const bool b) { m_showExtraTransformInfo = b; }
   void setMaxMatrixSearchDepth(const int depth) { m_maxMatrixSearchDepth = depth; }
   void setMaxTransformationsToShow(const int count) { m_maxTransformationsToShow = count; }
   void setMaxTransformationDistance(const double distance) { m_maxTransformationDistance = distance; }
   void setDistanceThreshold(const double threshold) { m_distanceThreshold = threshold; }
   void setIncludeIdentityMatrix(const bool b) { m_includeIdentityMatrix = b; }

   bool shouldSortByP3() const { return m_sortBy == "P3"; }
   std::string getSortBy() const { return m_sortBy; }
   void setSortBy(const std::string& sortBy) { m_sortBy = sortBy; }

private:
   std::string m_sortBy = "P3"; // Default to P3 distance
   bool m_showDetails = false;
   bool m_showExtraTransformInfo = false;
   int m_maxMatrixSearchDepth = 3;
   int m_maxTransformationsToShow = 3;
   double m_maxTransformationDistance = 100.0;
   double m_distanceThreshold = 50.0;
   bool m_includeIdentityMatrix = true;
};

#endif // MULTITRANSFORMFINDERCONTROLS_H

