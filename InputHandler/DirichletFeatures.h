// DirichletFeatures.h
#ifndef DIRICHLET_FEATURES_H
#define DIRICHLET_FEATURES_H

#include "ControlFeature.h"
#include "BaseControlVariables.h"
#include "ControlFeatures.h"  // For BlockControl

class DirichletControlVariables : public BaseControlVariables {
public:
   DirichletControlVariables();
   static void setupHandlers();
};

#endif // DIRICHLET_FEATURES_H
