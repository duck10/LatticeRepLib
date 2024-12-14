
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <ostream>
#include <random>
#include <string>
#include <vector>

#include "ControlVariables.h"
#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include "Follow.h"
#include "FollowUtils.h"
#include "G6.h"
#include "FollowInstance.h"
#include "InputHandler.h"
#include "LatticeCell.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_ReadLatticeData.h"
#include "Niggli.h"
#include "Polar.h"
#include "rhrand.h"
#include "S6.h"
#include "Selling.h"
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

std::vector<LatticeCell> handleInput(ControlVariables& controlVars, WebIO& webio) {
   std::vector<LatticeCell> inputVectors;

   std::cout << "; Enter control variables and input vectors (type 'end' to finish):\n";
   InputHandler::readMixedInput(controlVars, inputVectors, std::cin);

   if (controlVars.getVectorsPerTrial() > inputVectors.size()) {
      throw std::runtime_error("; Not enough input vectors for the selected follower mode");
   }

   std::cout << "; Number of input vectors read: " << inputVectors.size() << std::endl;
   if (webio.m_hasWebInstructions) {
      webio.SetTimeStamp(true);
   }

   return inputVectors;
}

static std::vector<FollowInstance> CreateFollowInstanceList(
   const ControlVariables& cv,
   const std::vector<LatticeCell>& cells)
{
   std::vector<FollowInstance> instances;
   const size_t vectorsNeeded = cv.getVectorsPerTrial();
   const size_t numCompleteSets = cells.size() / vectorsNeeded;
   instances.reserve(numCompleteSets * cv.perturbations);  // Pre-allocate space

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
      for (size_t kk = 1; kk < cv.perturbations; ++kk) {
         auto perturbedCells = FollowInstance::Perturb(starter, cv.perturbBy);
         instances.emplace_back(std::move(perturbedCells),
            static_cast<int>(i),
            static_cast<int>(kk));
      }
   }
   return instances;
}

static void processInstances(const std::vector<FollowInstance>& instances,
   const std::pair<int, int>& limits,
   ControlVariables& controlVars) {
   Follow follow(controlVars);
   for (size_t i = limits.first; i < limits.second; ++i) {
      std::cout << "; Follow graphics file(s) "
         << i << "  " << instances[i].GetFullFileName() << std::endl;

      follow.processPerturbation(instances[i].GetTrial(),
         instances[i].GetPerturbation(),
         instances[i]);
   }
}

int main(int argc, char* argv[]) {
   try {
      const std::string programName("Follow");
      std::cout << "; Follow\n";

      WebIO webio(argc, argv, programName, 0);
      ControlVariables controlVars;

      const auto inputVectors = handleInput(controlVars, webio);

      if (inputVectors.empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      WebLimits wl;
      wl.Update(webio); // update using webio FIRST
      wl.Update(int(controlVars.blocksize), int(controlVars.blockstart));

      auto instances = CreateFollowInstanceList(controlVars, inputVectors);
      const int ntotal = static_cast<int>(instances.size());

      const auto limits = WebLimits::GetProcessingLimits(
         wl.GetBlockStart(),
         wl.GetBlockSize(),
         ntotal,
         webio.m_hasWebInstructions);

      if (limits.first > ntotal) {
         throw std::runtime_error("; start point is already past the end point");
      }

      setupWebIO(webio, controlVars.filePrefix, instances, ntotal);
      // Update control variables with file names
      controlVars.updateFilenames(webio.m_FileNameList);
      // Add filenames to instances
      for (size_t i = 0; i < webio.m_basicfileNameList.size(); ++i) {
         instances[i].AddFileNames(
            webio.m_FileNameList[i],
            webio.m_basicfileNameList[i],
            webio.m_FullfileNameList[i]);
      }


      processInstances(instances, limits, controlVars);

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}