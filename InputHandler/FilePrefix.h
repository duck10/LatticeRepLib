
// InputHandler/FilePrefix.h
#ifndef INPUTHANDLER_FILE_PREFIX_H
#define INPUTHANDLER_FILE_PREFIX_H

#include "ControlFeature.h"
#include <string>

class FilePrefix : public ControlFeature {
public:
   explicit FilePrefix(const std::string_view& defaultPrefix = "FILE")
      : prefix(defaultPrefix) {

      InputHandler::registerHandler("PREFIX", 0.35,
         [this](BaseControlVariables&, const std::string_view& value) {
            if (!value.empty()) prefix = value;
         });
   }

   bool handleInput(const std::string& command, const std::string& value) override {
      return false;  // Now handled via InputHandler
   }

   std::string getFeatureState() const override {
      std::ostringstream oss;
      oss << ";File Prefix: " << prefix << "\n";
      return oss.str();
   }

   void writeToStream(std::ostream& os) const override {
      os << getFeatureState();
   }

   void setPrefix(const std::string_view& newPrefix) { prefix = newPrefix; }
   std::string getPrefix() const { return prefix; }

private:
   std::string prefix;
};

//---------------------------------------------------------------------------
#endif // INPUTHANDLER_FILE_PREFIX_H
