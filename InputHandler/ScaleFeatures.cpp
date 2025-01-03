// ScaleFeatures.cpp
#include "ScaleFeatures.h"
#include "InputHandler.h"
#include <sstream>

ScaleFormatControl::ScaleFormatControl() {
   InputHandler::registerHandler("FORMAT", 0.35,
      [this](BaseControlVariables&, const std::string& value) {
         setFormat(value);
      });
}

bool ScaleFormatControl::handleInput(const std::string& command, const std::string& value) {
   return false;  // Handled via InputHandler
}

std::string ScaleFormatControl::getFeatureState() const {
   std::ostringstream oss;
   oss << ";Scale Format: " << format << "\n";
   return oss.str();
}

void ScaleFormatControl::writeToStream(std::ostream& os) const {
   os << getFeatureState();
}

void ScaleFormatControl::setFormat(const std::string& newFormat) {
   format = newFormat;
}

std::string ScaleFormatControl::getFormat() const {
   return format;
}

ScaleControlVariables::ScaleControlVariables() {
   features.push_back(std::make_unique<ScaleFormatControl>());
}

void ScaleControlVariables::setupHandlers() {
   InputHandler::clearHandlers();
}