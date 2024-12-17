// PathControlFeatures.cpp
#include "PathControlFeatures.h"
#include "InputHandler.h"

void PathObjectCountControl::writeToStream(std::ostream& os) const {
   os << ";Path Objects: " << count << "\n";
}

void PathObjectCountControl::setCount(size_t n) { count = n; }
size_t PathObjectCountControl::getCount() const { return count; }

PathControlVariables::PathControlVariables() {
   features.push_back(std::make_unique<PathObjectCountControl>());
}

void PathControlVariables::setupHandlers() {
   InputHandler::clearHandlers();

   InputHandler::registerHandler("PATHCOUNT", 0.35,
      [](BaseControlVariables& cv, const std::string& value) {
         if (auto* countCtrl = cv.getFeature<PathObjectCountControl>()) {
            countCtrl->setCount(std::stoul(value));
         }
      });
}