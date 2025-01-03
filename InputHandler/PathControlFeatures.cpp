// PathControlFeatures.cpp
#include "PathControlFeatures.h"
#include "InputHandler.h"
#include <sstream>

PathObjectCountControl::PathObjectCountControl() {
   InputHandler::registerHandler("COUNT", 0.35,
      [this](BaseControlVariables&, const std::string& value) {
         setCount(std::stoul(value));
      });
}

bool PathObjectCountControl::handleInput(const std::string& command, const std::string& value) {
   return false;  // Handled via InputHandler
}

std::string PathObjectCountControl::getFeatureState() const {
   std::ostringstream oss;
   oss << ";Path Object Count: " << count << "\n";
   return oss.str();
}

void PathObjectCountControl::writeToStream(std::ostream& os) const {
   os << getFeatureState();
}

void PathObjectCountControl::setCount(size_t n) {
   count = n;
}

size_t PathObjectCountControl::getCount() const {
   return count;
}

PathControlVariables::PathControlVariables() {
   features.push_back(std::make_unique<PathObjectCountControl>());
}

void PathControlVariables::setupHandlers() {
   InputHandler::clearHandlers();
}
