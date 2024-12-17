// ProgramControls.cpp
#include "ProgramControls.h"
#include "ControlFeatures.h"

// Initialize Program1Controls with BlockControl and OutputSizeControl
Program1Controls::Program1Controls() {
   features.push_back(std::make_unique<BlockControl>());
   features.push_back(std::make_unique<OutputSizeControl>());
}

// Initialize Program2Controls with SingleLatticeTypeControl
Program2Controls::Program2Controls() {
   features.push_back(std::make_unique<SingleLatticeTypeControl>());
}