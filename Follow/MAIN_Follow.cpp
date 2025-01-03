
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "FollowControls.h"
#include "Follow.h"
#include "FollowInstance.h"
#include "InputHandler.h"
#include "LatticeCell.h"
#include "WebIO.h"

static void setupWebIO(WebIO& webio, const std::string filePrefix,
   std::vector<FollowInstance>& instances, int ntotal) {
   // Check web file limits first
   const std::string fileCountCheck = WebLimits::CheckWebFileLimits(
      ntotal,
      webio.m_hasWebInstructions,
      int(webio.m_blocksize),
      int(webio.m_blockstart));

   if (!fileCountCheck.empty()) {
      std::cout << fileCountCheck << std::endl;
   }

   // Update WebIO and create filenames
   webio.m_blocksize = ntotal;
   webio.CreateFilenamesAndLinks(ntotal, filePrefix);
}

static std::vector<FollowInstance> CreateFollowInstanceList(
   const FollowControls& controls,
   const std::vector<LatticeCell>& cells)
{
   std::vector<FollowInstance> instances;
   const size_t vectorsNeeded = controls.getVectorsPerTrial();
   const size_t numCompleteSets = cells.size() / vectorsNeeded;
   instances.reserve(numCompleteSets * controls.getPerturbations());  // Pre-allocate space

   for (size_t i = 0; i < numCompleteSets; ++i) {
      std::vector<LatticeCell> starter;
      starter.reserve(vectorsNeeded);  // Pre-allocate space

      const size_t baseIndex = i * vectorsNeeded;
      for (size_t j = 0; j < vectorsNeeded; ++j) {
         starter.emplace_back(cells[baseIndex + j]);
      }

      // Handle unperturbed case
      instances.emplace_back(starter, static_cast<int>(i), 0);

      // Handle perturbations
      for (size_t kk = 1; kk < controls.getPerturbations(); ++kk) {
         auto perturbedCells = FollowInstance::Perturb(starter, controls.getPerturbBy());
         instances.emplace_back(std::move(perturbedCells),
            static_cast<int>(i),
            static_cast<int>(kk));
      }
   }
   return instances;
}

static void processInstances(const std::vector<FollowInstance>& instances,
   std::pair<int, int>& limits,
   FollowControls& controls) {

   limits.second = std::min(limits.second, static_cast<int>(instances.size()));

   Follow follow(controls);
   for (size_t i = limits.first; i < limits.second; ++i) {
      if (follow.processPerturbation(instances[i].GetTrial(),
         instances[i].GetPerturbation(),
         instances[i])) {
         // Only output filename if file was successfully written
         std::cout << "; Follow graphics file(s) "
            << i << "  " << instances[i].GetFullFileName() << std::endl;
      }
   }
}

static void assignFilenamesToInstances(const WebIO& webio, std::vector<FollowInstance>& instances) {
   for (size_t i = 0; i < webio.m_basicfileNameList.size(); ++i) {
      instances[i].AddFileNames(
         webio.m_FileNameList[i],
         webio.m_basicfileNameList[i],
         webio.m_FullfileNameList[i]);
   }
}

int main(int argc, char* argv[]) {
   try {
      const std::string programName("Follow");
      std::cout << "; Follow\n";

      WebIO webio(argc, argv, programName, 0);
      FollowControls controls;

      std::vector<LatticeCell> inputVectors;
      std::cout << "; Enter control variables and input vectors (type 'end' to finish):\n";
      InputHandler::readMixedInput(controls, inputVectors, std::cin);

      if (inputVectors.empty()) {
         throw std::runtime_error("; No input vectors provided");
      }
      std::cout << "; Number of input vectors read: " << inputVectors.size() << std::endl;

      // Add vector count validation here
      std::string errorMsg;
      if (!FollowerModeUtils::validateVectorCount(controls.getMode(), inputVectors.size(), errorMsg)) {
         throw std::runtime_error(errorMsg);
      }

      WebLimits wl;
      wl.Update(webio);

      auto* blockProcessing = controls.getBlockProcessing();

      blockProcessing->updateLimits(inputVectors.size(), webio.m_hasWebInstructions,
         [&controls](size_t inputs) {
            return (inputs / controls.getVectorsPerTrial()) * controls.getPerturbations();
         });
      wl.Update(static_cast<int>(blockProcessing->getBlockSize()), static_cast<int>(blockProcessing->getBlockStart()));

      //std::cout << controls.getState() << "\n";

      auto instances = CreateFollowInstanceList(controls, inputVectors);
      const int ntotal = static_cast<int>(instances.size());

      setupWebIO(webio, controls.getFilePrefix()->getPrefix(), instances, ntotal);
      assignFilenamesToInstances(webio, instances);
      controls.updateFilenames(webio.m_FileNameList);

      auto limits = WebLimits::GetProcessingLimits(
         wl.GetBlockStart(),
         wl.GetBlockSize(),
         ntotal,
         controls.getPerturbations(),
         webio.m_hasWebInstructions);

      if ( controls.shouldShowControls())
      {
         std::cout << controls.getState() << "\n";
      }

      processInstances(instances, limits, controls);

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}

