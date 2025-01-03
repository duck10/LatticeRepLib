// ScaleFeatures.h
#ifndef INPUTHANDLER_SCALE_FEATURES_H
#define INPUTHANDLER_SCALE_FEATURES_H

#include "ControlFeature.h"
#include "BaseControlVariables.h"
#include <string>

class ScaleFormatControl : public ControlFeature {
public:
   ScaleFormatControl();
   bool handleInput(const std::string& command, const std::string& value) override;
   std::string getFeatureState() const override;
   void writeToStream(std::ostream& os) const override;

   void setFormat(const std::string& newFormat);
   std::string getFormat() const;

private:
   std::string format;
};

class ScaleControlVariables : public BaseControlVariables {
public:
   ScaleControlVariables();
   void setupHandlers();
};

#endif