// DirichletFeatures.h
#ifndef INPUTHANDLER_DIRICHLET_FEATURES_H
#define INPUTHANDLER_DIRICHLET_FEATURES_H

#include "BaseControlVariables.h"

class DirichletControlVariables : public BaseControlVariables {
public:
   DirichletControlVariables() {
   }

   void setupHandlers();
};

#endif  // INPUTHANDLER_DIRICHLET_FEATURES_H
