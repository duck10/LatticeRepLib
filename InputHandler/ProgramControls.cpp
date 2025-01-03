// ProgramControls.cpp
#include "ProgramControls.h"
#include "InputHandler.h"
#include <sstream>

OutputSizeControl::OutputSizeControl() {
   InputHandler::registerHandler("SIZE", 0.35,
      [this](BaseControlVariables&, const std::string& value) {
         setSize(std::stoul(value));
      });
}

bool OutputSizeControl::handleInput(const std::string& command, const std::string& value) {
   return false;  // Now handled via InputHandler
}

std::string OutputSizeControl::getFeatureState() const {
   std::ostringstream oss;
   oss << ";Output Size: " << size << "\n";
   return oss.str();
}

void OutputSizeControl::writeToStream(std::ostream& os) const {
   os << getFeatureState();
}

void OutputSizeControl::setSize(size_t newSize) {
   size = newSize;
}

size_t OutputSizeControl::getSize() const {
   return size;
}

Program1Controls::Program1Controls() {
   // Now using features from BaseControlVariables
   features.push_back(std::make_unique<BlockProcessing>());
   features.push_back(std::make_unique<OutputSizeControl>());
}

void Program1Controls::setupHandlers() {
   InputHandler::clearHandlers();
}

Program2Controls::Program2Controls() {
   features.push_back(std::make_unique<BlockProcessing>());
}

void Program2Controls::setupHandlers() {
   InputHandler::clearHandlers();
}