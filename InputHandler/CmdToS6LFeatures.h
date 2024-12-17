// CmdToS6LFeatures.h
#ifndef CMDTOS6L_FEATURES_H
#define CMDTOS6L_FEATURES_H

#include "ControlFeature.h"
#include "BaseControlVariables.h"
#include <string>

// Control for output format
class CmdToS6LFormatControl : public ControlFeature {
private:
   std::string format{ "S6" };  // Default to S6

public:
   void writeToStream(std::ostream& os) const override;
   void setFormat(const std::string& fmt);
   std::string getFormat() const;
};

class CmdToS6LControlVariables : public BaseControlVariables {
public:
   CmdToS6LControlVariables();
   static void setupHandlers();
};

#endif // CMDTOS6L_FEATURES_H