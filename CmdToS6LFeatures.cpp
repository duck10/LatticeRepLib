// CmdToS6LFeatures.cpp
#include "CmdToS6LFeatures.h"
#include "InputHandler.h"
#include <sstream>

CmdToS6LFormatControl::CmdToS6LFormatControl() {
   InputHandler::registerHandler("FORMAT", 0.35,
      [this](BaseControlVariables&, const std::string& value) {
         setFormat(value);
      });
}

bool CmdToS6LFormatControl::handleInput(const std::string& command, const std::string& value) {
   return false;  // Handled via InputHandler
}

std::string CmdToS6LFormatControl::getFeatureState() const {
   std::ostringstream oss;
   oss << ";Format: " << format << "\n";
   return oss.str();
}

void CmdToS6LFormatControl::writeToStream(std::ostream& os) const {
   os << getFeatureState();
}

void CmdToS6LFormatControl::setFormat(const std::string& newFormat) {
   format = newFormat;
}

std::string CmdToS6LFormatControl::getFormat() const {
   return format;
}

CmdToS6LControlVariables::CmdToS6LControlVariables() {
   features.push_back(std::make_unique<CmdToS6LFormatControl>());
}

void CmdToS6LControlVariables::setupHandlers() {
   InputHandler::clearHandlers();
   // Any additional handler setup if needed
}
