// BaseControlVariables.h
#ifndef BASE_CONTROL_VARIABLES_H
#define BASE_CONTROL_VARIABLES_H

#include "ControlFeature.h"
#include "InputHandler.h"

#include <vector>
#include <memory>
#include <ostream>
#include <sstream>

class BaseControlVariables {
protected:
   std::vector<std::unique_ptr<ControlFeature>> features;
   bool webRun = false;
   bool showControls = false;
   bool echo = false;

   //BaseControlVariables(const BaseControlVariables&) = delete;
   //BaseControlVariables& operator=(const BaseControlVariables&) = delete;

public:
   virtual ~BaseControlVariables() = default;

   BaseControlVariables() {
      InputHandler::registerHandler("ECHO", .5,
         [this](BaseControlVariables& controls, const std::string& value) {
            echo = (value == "1" || value == "TRUE" || value.empty());
         }
      );

      InputHandler::registerHandler("SHOW", .5,
         [this](BaseControlVariables& controls, const std::string& value) {
            showControls = (value == "1" || value == "TRUE" || value.empty());
         }
      );

      InputHandler::registerHandler("SHOWCONTROLS", .5,
         [this](BaseControlVariables& controls, const std::string& value) {
            showControls = (value == "1" || value == "TRUE" || value.empty());
         }
      );

      InputHandler::registerHandler("WEB_INPUT", .5,
         [this](BaseControlVariables& controls, const std::string& value) {
            webRun = (value == "1" || value == "TRUE" || value.empty());
         }
      );
   }

   template<typename T>
   T* getFeature() {
      for (auto& feature : features) {
         if (auto* p = dynamic_cast<T*>(feature.get())) {
            return p;
         }
      }
      return nullptr;
   }

   virtual bool handleInput(const std::string& command, const std::string& value) {
      for (auto& feature : features) {
         if (feature->handleInput(command, value)) {
            return true;
         }
      }
      return false;
   }

   virtual std::string getState() const {
      std::ostringstream oss;
      for (const auto& feature : features) {
         oss << feature->getFeatureState();
      }
      return oss.str();
   }

   bool getWebRun() const { return webRun; }
   void setWebRun(const bool webrun) { webRun = webrun; }
   void setHasWebInput(const bool b) { webRun = b; }
   bool getHasWebInput() const { return webRun; }
   bool getShowControls() const { return showControls; }
   bool getShouldControls() const { return showControls; }
   void setShowContols(const bool b) { showControls = b; }
   bool getEcho() const { return echo; }
   void setEcho(const bool b) { echo = b; }
   bool shouldShowControls() const { return showControls; }

   friend std::ostream& operator<<(std::ostream& os, const BaseControlVariables& cv);
};

inline std::ostream& operator<<(std::ostream& os, const BaseControlVariables& cv) {
   os << ";Control Variables:\n";
   for (const auto& feature : cv.features) {
      feature->writeToStream(os);
   }
   return os;
}

#endif // BASE_CONTROL_VARIABLES_H
