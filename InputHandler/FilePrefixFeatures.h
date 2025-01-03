// FilePrefixFeatures.h
#ifndef INPUTHANDLER_FILE_PREFIX_FEATURES_H
#define INPUTHANDLER_FILE_PREFIX_FEATURES_H

#include "ControlFeature.h"
#include <string>

class FilePrefixControl : public ControlFeature {
public:
   FilePrefixControl();
   bool handleInput(const std::string& command, const std::string& value) override;
   std::string getFeatureState() const override;
   void writeToStream(std::ostream& os) const override;

   void setPrefix(const std::string& newPrefix);
   std::string getPrefix() const;

private:
   std::string prefix;
};

#endif // INPUTHANDLER_FILE_PREFIX_FEATURES_H

