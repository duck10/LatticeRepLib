// CmdSellaFeatures.cpp
#include "CmdSellaFeatures.h"
#include "InputHandler.h"

void CmdSellaControlVariables::setupHandlers() {
   InputHandler::clearHandlers();

   // Handler for block size
   if (auto* blockCtrl = getFeature<BlockProcessing>()) {
      InputHandler::registerHandler("SIZE", 0.35,
         [blockCtrl](BaseControlVariables&, const std::string& value) {
            size_t size = std::stoul(value);
            blockCtrl->setBlockSize(size);
         });
   }
}
