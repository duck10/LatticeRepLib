
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "ProgramSetup.h"
#include "FileOperations.h"
#include "Follow.h"
#include "FollowControls.h"
#include "FollowInstance.h"
#include "InputHandler.h"
#include "LatticeCell.h"
#include "WebIO.h"

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

static void assignFilenamesToInstances(
   const std::vector<std::string>& rawNames,
   const std::vector<std::string>& basicNames,
   const std::vector<std::string>& fullNames,
   std::vector<FollowInstance>& instances)
{
   for (size_t i = 0; i < instances.size() && i < rawNames.size(); ++i) {
      instances[i].AddFileNames(rawNames[i], basicNames[i], fullNames[i]);
   }
}

static void processInstances(
   const std::vector<FollowInstance>& instances,
   size_t blockStart,
   size_t blockEnd,
   FollowControls& controls,
   const std::vector<LatticeCell>& cells)
{
   Follow follow(controls);
   for (size_t i = blockStart; i < blockEnd && i < instances.size(); ++i) {
      if (follow.processPerturbation(instances[i].GetTrial(),
         instances[i].GetPerturbation(),
         instances[i], cells))
      {
         std::cout << "; Follow graphics file(s) "
            << i << "  " << instances[i].GetFullFileName() << std::endl;
      }
   }
}

int main(int argc, char* argv[]) {
   const std::string programName("Follow");
   std::cout << "; Follow\n";

   try {
      WebIO webio(argc, argv, "Follow", 0);
      FollowControls controls;
      controls.setWebRun(webio.m_hasWebInstructions);

      const WebFileBlockProgramInput<FollowControls> dc_setup(argc, argv, "Follow", 0, controls);

      if (controls.shouldShowControls()) {
         std::cout << controls.getState() << "\n";
      }

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      std::cout << "; Number of input vectors read: " << dc_setup.getInputList().size() << std::endl;

      // Validate vector count
      if (!FollowerModeUtils::validateVectorCount(controls.getMode(), dc_setup.getInputList().size())) {
         throw std::runtime_error("; Not enough vectors for MODE " +
            FollowerModeUtils::toString(controls.getMode()) +
            " (have " + std::to_string(dc_setup.getInputList().size()) + ", need " +
            std::to_string(FollowerModeUtils::getVectorsNeeded(controls.getMode())) + ")");
      }

      // Create and process instances
      auto instances = CreateFollowInstanceList(controls, dc_setup.getInputList());
      if (controls.getMode() == FollowerMode::SPLINE)
         instances.resize(1);

      // Assign filenames to instances
      assignFilenamesToInstances(
         dc_setup.getRawFileNames(),
         dc_setup.getBasicFileNames(),
         dc_setup.getFullFileNames(),
         instances);

      controls.updateFilenames(dc_setup.getRawFileNames());

      if (controls.shouldShowControls()) {
         std::cout << controls.getState() << "\n";
      }

      // Process instances for the current block
      Follow::processInstances(
         instances,
         dc_setup.getBlockStart(),
         dc_setup.getBlockEnd(),
         controls,
         dc_setup.getInputList());

      return 0;
   }
   catch (const std::exception& e) {
      std::cout << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}

