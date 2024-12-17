// PathControlFeatures.h
#ifndef PATH_CONTROL_FEATURES_H
#define PATH_CONTROL_FEATURES_H

#include "ControlFeature.h"
#include "BaseControlVariables.h"

// Control for number of path objects with different default
class PathObjectCountControl : public ControlFeature {
private:
   size_t count{ 50 };  // Default to 50 for Path

public:
   void writeToStream(std::ostream& os) const override;
   void setCount(size_t n);
   size_t getCount() const;
};

class PathControlVariables : public BaseControlVariables {
public:
   PathControlVariables();
   static void setupHandlers();
};

#endif // PATH_CONTROL_FEATURES_H