// CmdSellaFeatures.h
#ifndef CMDSELLA_FEATURES_H
#define CMDSELLA_FEATURES_H

#include "ControlFeature.h"
#include "BaseControlVariables.h"
#include "ControlFeatures.h"  // For BlockControl

class CmdSellaControlVariables : public BaseControlVariables {
public:
   CmdSellaControlVariables();
   static void setupHandlers();
};

#endif // CMDSELLA_FEATURES_H
