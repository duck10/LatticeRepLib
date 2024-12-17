// ScaleFeatures.h
#ifndef SCALE_FEATURES_H
#define SCALE_FEATURES_H

#include "ControlFeature.h"
#include "BaseControlVariables.h"
#include <string>
#include <stdexcept>
#include <ostream>  // for operator<<

// Enum for valid scale formats
enum class ScaleFormat {
   S6,
   V7,
   DC7u,
   RI
};

// Free function declarations
std::string toString(ScaleFormat format);
ScaleFormat toScaleFormat(const std::string& s);

class ScaleFormatControl : public ControlFeature {
private:
   ScaleFormat format{ ScaleFormat::S6 };  // Default to S6

public:
   void writeToStream(std::ostream& os) const override;
   void setFormat(ScaleFormat fmt);
   void setFormat(const std::string& fmt);
   ScaleFormat getFormat() const;
   std::string getFormatString() const;
};

class ScaleControlVariables : public BaseControlVariables {
public:
   ScaleControlVariables();
   static void setupHandlers();
};

#endif // SCALE_FEATURES_H