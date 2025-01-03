// FilePrefixFeatures.cpp
#include "FilePrefixFeatures.h"
#include "InputHandler.h"
#include <sstream>

FilePrefixControl::FilePrefixControl() {
   InputHandler::registerHandler("PREFIX", 0.35,
      [this](BaseControlVariables&, const std::string& value) {
         setPrefix(value);
      });
}

bool FilePrefixControl::handleInput(const std::string& command, const std::string& value) {
   return false;  // Handled via InputHandler
}

std::string FilePrefixControl::getFeatureState() const {
   std::ostringstream oss;
   oss << ";File Prefix: " << prefix << "\n";
   return oss.str();
}

void FilePrefixControl::writeToStream(std::ostream& os) const {
   os << getFeatureState();
}

void FilePrefixControl::setPrefix(const std::string& newPrefix) {
   prefix = newPrefix;
}

std::string FilePrefixControl::getPrefix() const {
   return prefix;
}
