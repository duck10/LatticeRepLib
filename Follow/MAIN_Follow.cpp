#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
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

// Instance creation, file name assignment and processing all live in
// Follow.cpp as static members of Follow. The duplicate copies that used to
// be in this file were removed (Oct 2026); the local copy of
// assignFilenamesToInstances ignored BLOCKSTART, which mislabelled every
// output file and left the end of each block without file names.
//
// Oct 2026: Follow writes one file per INSTANCE (inputs x perturbations,
// or inputs / vectors-per-trial for LINE, CHORD, etc.), not one per input.
// Block limits and file names are therefore computed here from
// instances.size(), after the instances exist. The ones computed inside
// WebFileBlockProgramInput are based on the input count and are not used.

int main(int argc, char* argv[]) {
   const std::string programName("Follow");
   std::cout << "; Follow" << std::endl;

   try {
      WebIO webio(argc, argv, "Follow", 0);
      FollowControls controls;
      controls.setWebRun(webio.m_hasWebInstructions);

      const WebFileBlockProgramInput<FollowControls> dc_setup(argc, argv, "Follow", 0, controls);

      if (controls.shouldShowControls()) {
         std::cout << controls.getState() << std::endl;
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

      // Create instances
      auto instances = Follow::CreateFollowInstanceList(controls, dc_setup.getInputList());
      if (controls.getMode() == FollowerMode::SPLINE)
         instances.resize(1);

      // Block limits and file names, based on the number of output files
      // (instances). ApplyBlockLimits enforces the web cap and clamps the
      // block to the instance count.
      webio.m_blockstart = controls.getBlockStart();
      webio.m_blocksize = controls.getBlockSize();
      webio.ApplyBlockLimits(instances.size(),
         webio.m_hasWebInstructions || controls.getWebRun());
      webio.CreateFilenamesAndLinks(instances.size(), controls.getPrefix());

      const size_t blockStart = webio.m_blockstart;
      const size_t blockEnd = webio.m_blockstart + webio.m_blocksize;

      // Assign filenames to instances (name entry k -> instance blockStart + k)
      Follow::assignFilenamesToInstances(
         webio.m_FileNameList,
         webio.m_basicfileNameList,
         webio.m_FullfileNameList,
         blockStart,
         instances);

      controls.updateFilenames(webio.m_FileNameList);

      if (controls.shouldShowControls()) {
         std::cout << controls.getState() << std::endl;
      }

      // Process instances for the current block
      Follow::processInstances(
         instances,
         blockStart,
         blockEnd,
         controls,
         dc_setup.getInputList());

      return 0;
   }
   catch (const std::exception& e) {
      std::cout << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}
