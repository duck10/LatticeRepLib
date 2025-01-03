#ifndef INPUTHANDLER_CONTROL_FEATURE_H
#define INPUTHANDLER_CONTROL_FEATURE_H

#include <string>
#include <ostream>

class ControlFeature {
public:
   virtual ~ControlFeature() = default;
   virtual bool handleInput(const std::string& command, const std::string& value) = 0;
   virtual std::string getFeatureState() const = 0;

   // Change to inline implementation
   virtual void writeToStream(std::ostream& os) const {
      os << getFeatureState();
   }
};

#endif // INPUTHANDLER_CONTROL_FEATURE_H
