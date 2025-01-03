// CmdToS6LFeatures.h
#ifndef INPUTHANDLER_CMD_TO_S6L_FEATURES_H
#define INPUTHANDLER_CMD_TO_S6L_FEATURES_H

#include "BaseControlVariables.h"
#include "ControlFeature.h"
#include <string>

class CmdToS6LFormatControl : public ControlFeature {
public:
   CmdToS6LFormatControl();
   bool handleInput(const std::string& command, const std::string& value) override;
   std::string getFeatureState() const override;
   void writeToStream(std::ostream& os) const override;

   void setFormat(const std::string& newFormat);
   std::string getFormat() const;

private:
   std::string format;
};

class CmdToS6LControlVariables : public BaseControlVariables {
public:
   CmdToS6LControlVariables();
   void setupHandlers();
};

#endif