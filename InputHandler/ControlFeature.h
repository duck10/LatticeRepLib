// ControlFeature.h
#ifndef CONTROL_FEATURE_H
#define CONTROL_FEATURE_H

#include <ostream>

class ControlFeature {
public:
   virtual ~ControlFeature() = default;
   virtual void writeToStream(std::ostream& os) const = 0;
};

#endif // CONTROL_FEATURE_H