// FollowFeatures.h
#ifndef INPUTHANDLER_FOLLOW_FEATURES_H
#define INPUTHANDLER_FOLLOW_FEATURES_H

#include "BaseControlVariables.h"
#include "ControlFeature.h"
#include "FilePrefixFeatures.h"
#include "FollowerMode.h"
#include <string>

class FollowerControl : public ControlFeature {
public:
   FollowerControl();
   bool handleInput(const std::string& command, const std::string& value) override;
   std::string getFeatureState() const override;
   void writeToStream(std::ostream& os) const override;

   void setMode(const std::string& newMode);

   void setPerturbations(int n);
   int getPerturbations() const;

   void setPerturbBy(double value);
   double getPerturbBy() const;

   void setPoints(int n);
   int getPoints() const;

   void setPrintData(bool value);
   bool getPrintData() const;

   void setGlitchesOnly(bool value);
   bool getGlitchesOnly() const;

   void setMode(FollowerMode newMode) { mode = newMode; }
   void setFollowerPoints(int points_in) { points = points_in; }
   FollowerMode getMode() const { return mode; }
   //std::string getMode() const;

   int getFollowerPoints() const { return points; }

private:
   FollowerMode mode;
   int perturbations{ 1 };
   double perturbBy{ 0.1 };
   int points{ 100 };
   bool printData{ false };
   bool glitchesOnly{ false };
};

class FollowControlVariables : public BaseControlVariables {
public:
   FollowControlVariables();
   void setupHandlers();
};

#endif