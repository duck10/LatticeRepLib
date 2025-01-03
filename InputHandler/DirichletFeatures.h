// DirichletFeatures.h
#ifndef INPUTHANDLER_DIRICHLET_FEATURES_H
#define INPUTHANDLER_DIRICHLET_FEATURES_H

#include "BaseControlVariables.h"
#include "BlockProcessing.h"

class DirichletControlVariables : public BaseControlVariables {
public:
   DirichletControlVariables() {
      features.push_back(std::make_unique<BlockProcessing>());
   }

   void setupHandlers();
};

#endif  // INPUTHANDLER_DIRICHLET_FEATURES_H
