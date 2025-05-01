#ifndef MULTI_TRANSFORM_FINDER_CONTROLS_H
#define MULTI_TRANSFORM_FINDER_CONTROLS_H

#include "BaseControlVariables.h"
#include "InputHandler.h"
#include "LRL_StringTools.h"

#include <sstream>

class MultiTransformFinderControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const MultiTransformFinderControls& ctrl) {
      os << "; MultiTransformFinder Controls" << std::endl;
      os << ";   Distance Threshold: " << ctrl.distanceThreshold << std::endl;
      os << ";   Max Results: " << ctrl.maxResults << std::endl;
      os << ";   Include Identity Matrix: " << (ctrl.includeIdentityMatrix ? "Yes" : "No") << std::endl;
      return os;
   }

   MultiTransformFinderControls() {
      // Register handlers for control variables
      // Note: The base class handles "show" automatically to set showControls

      InputHandler::registerHandler("DISTANCETHRESHOLD", 0.3,
         [this](const BaseControlVariables&, const std::string& value) {
            distanceThreshold = std::stod(value);
         });

      InputHandler::registerHandler("MAXRESULTS", 0.4,
         [this](const BaseControlVariables&, const std::string& value) {
            maxResults = std::stoul(value);
         });

      InputHandler::registerHandler("INCLUDEIDENTITYMATRIX", 0.5,
         [this](BaseControlVariables&, const std::string& value) {
            includeIdentityMatrix = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

      InputHandler::registerHandler("SHOWDETAILS", 0.5,
         [this](BaseControlVariables&, const std::string& value) {
            showDetails = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });

      InputHandler::registerHandler("DETAILS", 0.5,
         [this](BaseControlVariables&, const std::string& value) {
            showDetails = (value == "1" || LRL_StringTools::strToupper(value) == "TRUE" || value.empty());
         });
      }

   double getDistanceThreshold() const { return distanceThreshold; }
   size_t getMaxResults() const { return maxResults; }
   bool shouldIncludeIdentity() const { return includeIdentityMatrix; }
   bool shouldShowControls() const { return showControls; }
   bool shouldShowDetails() const { return showDetails; }

private:
   double distanceThreshold = 50.0;
   size_t maxResults = 5;
   bool includeIdentityMatrix = true;
   bool showDetails = false;
};

#endif // MULTI_TRANSFORM_FINDER_CONTROLS_H

