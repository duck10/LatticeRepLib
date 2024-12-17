// ScaleFeatures.cpp
#include "ScaleFeatures.h"
#include "InputHandler.h"

std::string toString(ScaleFormat format) {
   switch (format) {
   case ScaleFormat::S6: return "S6";
   case ScaleFormat::V7: return "V7";
   case ScaleFormat::DC7u: return "DC7u";
   case ScaleFormat::RI: return "RI";
   }
   throw std::runtime_error("Unknown scale format value");
}

ScaleFormat toScaleFormat(const std::string& s) {
   if (s == "S6") return ScaleFormat::S6;
   if (s == "V7") return ScaleFormat::V7;
   if (s == "DC7u") return ScaleFormat::DC7u;
   if (s == "RI") return ScaleFormat::RI;
   throw std::runtime_error("Invalid scale format: " + s);
}

void ScaleFormatControl::writeToStream(std::ostream& os) const {
   os << ";Scale Format: " << toString(format) << "\n";
}

void ScaleFormatControl::setFormat(ScaleFormat fmt) {
   format = fmt;
}

void ScaleFormatControl::setFormat(const std::string& fmt) {
   format = toScaleFormat(fmt);
}

ScaleFormat ScaleFormatControl::getFormat() const {
   return format;
}

std::string ScaleFormatControl::getFormatString() const {
   return toString(format);
}

ScaleControlVariables::ScaleControlVariables() {
   features.push_back(std::make_unique<ScaleFormatControl>());
}

void ScaleControlVariables::setupHandlers() {
   InputHandler::clearHandlers();

   InputHandler::registerHandler("FORMAT", 0.35,
      [](BaseControlVariables& cv, const std::string& value) {
         if (auto* formatCtrl = cv.getFeature<ScaleFormatControl>()) {
            try {
               formatCtrl->setFormat(value);
            }
            catch (const std::runtime_error& e) {
               std::cerr << ";Warning: " << e.what() << std::endl;
            }
         }
      });
}
