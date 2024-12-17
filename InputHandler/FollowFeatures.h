// FollowFeatures.h
#ifndef FOLLOW_FEATURES_H
#define FOLLOW_FEATURES_H

#include "ControlFeature.h"
#include "BaseControlVariables.h"
#include "ControlFeatures.h"  // For BlockControl
#include "FollowerMode.h"
#include <string>

// Main Follow control feature
class FollowerControl : public ControlFeature {
private:
   FollowerMode mode{ FollowerMode::POINT };
   int perturbations{ 1 };
   double perturbBy{ 0.1 };
   int followerPoints{ 100 };

public:
   void writeToStream(std::ostream& os) const override;
   void setMode(FollowerMode newMode);
   void setPerturbations(int count);
   void setPerturbBy(double amount);
   void setFollowerPoints(int points);

   FollowerMode getMode() const;
   int getPerturbations() const;
   double getPerturbBy() const;
   int getFollowerPoints() const;
};

// For file prefix control
class FilePrefixControl : public ControlFeature {
private:
   std::string prefix{ "FOL" };

public:
   void writeToStream(std::ostream& os) const override;
   void setPrefix(const std::string& newPrefix);
   std::string getPrefix() const;
};

class FollowControlVariables : public BaseControlVariables {
public:
   FollowControlVariables();
   static void setupHandlers();
};

#endif // FOLLOW_FEATURES_H