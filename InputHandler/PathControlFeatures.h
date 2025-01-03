// PathControlFeatures.h
#ifndef PATH_CONTROL_FEATURES_H
#define PATH_CONTROL_FEATURES_H

#include "ControlFeature.h"
#include "BaseControlVariables.h"
#include <string>

// Control for number of path objects with different default
class PathObjectCountControl : public ControlFeature {
public:
   PathObjectCountControl();
   bool handleInput(const std::string& command, const std::string& value) override;
   std::string getFeatureState() const override;
   void writeToStream(std::ostream& os) const override;

   void setCount(size_t n);
   size_t getCount() const;

private:
   size_t count{ 0 };
};

class PathControlVariables : public BaseControlVariables {
public:
   PathControlVariables();
   void setupHandlers();
};

#endif // PATH_CONTROL_FEATURES_H
