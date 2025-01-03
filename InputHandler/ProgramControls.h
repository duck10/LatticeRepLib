// ProgramControls.h
#ifndef INPUTHANDLER_PROGRAM_CONTROLS_H
#define INPUTHANDLER_PROGRAM_CONTROLS_H

#include "BaseControlVariables.h"
#include "BlockProcessing.h"
#include <string>

class OutputSizeControl : public ControlFeature {
public:
   OutputSizeControl();
   bool handleInput(const std::string& command, const std::string& value) override;
   std::string getFeatureState() const override;
   void writeToStream(std::ostream& os) const override;

   void setSize(size_t newSize);
   size_t getSize() const;

private:
   size_t size{ 0 };
};

// Now inherits from BaseControlVariables which has the features vector
class Program1Controls : public BaseControlVariables {
public:
   Program1Controls();
   void setupHandlers();
};

class Program2Controls : public BaseControlVariables {
public:
   Program2Controls();
   void setupHandlers();
};

#endif // INPUTHANDLER_PROGRAM_CONTROLS_H
