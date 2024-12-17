// DirichletFeatures.cpp
#include "DirichletFeatures.h"
#include "InputHandler.h"
#include "BlockLimits.h"

DirichletControlVariables::DirichletControlVariables() {
   features.push_back(std::make_unique<BlockControl>());
}

void DirichletControlVariables::setupHandlers() {
   InputHandler::clearHandlers();

   InputHandler::registerHandler("BLOCKSIZE", 0.35,
      [](BaseControlVariables& cv, const std::string& value) {
         if (auto* blockCtrl = cv.getFeature<BlockControl>()) {
            size_t size = std::stoul(value);
            if (BlockLimits::validateBlockSize(size)) {
               blockCtrl->setBlockSize(size);
            }
            else {
               std::cerr << ";Warning: Invalid block size: " << size << std::endl;
            }
         }
      });

   InputHandler::registerHandler("BLOCKSTART", 0.35,
      [](BaseControlVariables& cv, const std::string& value) {
         if (auto* blockCtrl = cv.getFeature<BlockControl>()) {
            blockCtrl->setBlockStart(std::stoul(value));
         }
      });
}