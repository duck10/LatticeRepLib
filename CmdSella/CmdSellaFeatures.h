// CmdSellaFeatures.h
#ifndef INPUTHANDLER_CMD_SELLA_FEATURES_H
#define INPUTHANDLER_CMD_SELLA_FEATURES_H

#include "BaseControlVariables.h"
#include "BlockProcessing.h"

class CmdSellaControlVariables : public BaseControlVariables {
public:
   CmdSellaControlVariables() {
      features.push_back(std::make_unique<BlockProcessing>());
   }

   void setupHandlers();
};

#endif // INPUTHANDLER_CMD_SELLA_FEATURES_H
