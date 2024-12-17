// CmdToS6LFeatures.cpp
#include "CmdToS6LFeatures.h"
#include "InputHandler.h"

void CmdToS6LFormatControl::writeToStream(std::ostream& os) const {
   os << ";Output Format: " << format << "\n";
}

void CmdToS6LFormatControl::setFormat(const std::string& fmt) {
   format = fmt;
}

std::string CmdToS6LFormatControl::getFormat() const {
   return format;
}

CmdToS6LControlVariables::CmdToS6LControlVariables() {
   features.push_back(std::make_unique<CmdToS6LFormatControl>());
}

void CmdToS6LControlVariables::setupHandlers() {
   InputHandler::clearHandlers();

   InputHandler::registerHandler("FORMAT", 0.35,
      [](BaseControlVariables& cv, const std::string& value) {
         if (auto* formatCtrl = cv.getFeature<CmdToS6LFormatControl>()) {
            formatCtrl->setFormat(value);
         }
      });
}
