
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


class WebLimits {
public:

   static friend std::ostream& operator<<(std::ostream& os, const WebLimits& wl) {
      os << "WebLimits\n";
      os << "block size " << wl.blocksize << std::endl;
      os << "block start " << wl.blockstart << std::endl;
      os << "block size max " << wl.blocksizemax << std::endl;
      return os;
   }

   WebLimits() = default;

   void Update(const WebIO& wio) {
      blocksize = std::min(static_cast<int>(wio.m_blocksize), blocksizemax);
      blockstart = std::max(static_cast<int>(wio.m_blockstart), blockstart);
   }

   void Update(const ControlVariables& cv) {
      blocksize =std::min(blocksize, (std::min(static_cast<int>(cv.blocksize), blocksizemax)));
      blockstart = std::max(static_cast<int>(cv.blockstart), blockstart);
   }

   int GetBlockSize() const { return blocksize; }
   int GetBlockStart() const { return blockstart; }
   int GetGlockSizeMax() const { return blocksizemax; }

private:
   const int blocksizemax = 20;
   int blocksize = blocksizemax;
   int blockstart = 0;
};

std::vector<FollowInstance> CreateFollowInstanceList(ControlVariables& cv, const std::vector<LatticeCell>& cells) {
   std::vector<FollowInstance> instances;
   const size_t vectorsNeeded = cv.getVectorsPerTrial();

   // Only process complete sets of cells
   for (size_t i = 0; i + vectorsNeeded <= cells.size(); i += vectorsNeeded) {
      std::vector<LatticeCell> starter;
      for (size_t nnn = i; nnn < i + vectorsNeeded; ++nnn) {
         starter.emplace_back(cells[nnn]);
      }
      for (size_t kk = 0; kk < cv.perturbations; ++kk) {
         if (kk == 0) {
            instances.emplace_back(FollowInstance(starter, static_cast<int>(i / vectorsNeeded), static_cast<int>(kk)));
         }
         else {
            // Get the perturbed vector of LattleCells
            std::vector<LatticeCell> perturbedCells = FollowInstance::Perturb(starter, cv.perturbBy);
            instances.emplace_back(FollowInstance(perturbedCells, static_cast<int>(i / vectorsNeeded), static_cast<int>(kk)));
         }
      }
   }
   return instances;
}

bool validateFollowerMode(FollowerMode mode, size_t numInputVectors) {
   switch (mode) {
   case FollowerMode::POINT: return numInputVectors >= 1;
   case FollowerMode::LINE:
   case FollowerMode::CHORD: return numInputVectors >= 2;
   case FollowerMode::TRIANGLE:
   case FollowerMode::CHORD3: return numInputVectors >= 3;
   default: return false;
   }
}

int ComputeTotalNumberOfRuns(const ControlVariables& ctrlVars, const int inputCount) {
   const int perturbations = ctrlVars.perturbations;
   const int vectorPerTrial = ctrlVars.getVectorsPerTrial();
   const int nTrials = inputCount / vectorPerTrial;
   return nTrials * perturbations;
}

void  CheckWebFileLimits(const int ntotalFiles, const bool hasWebInstructions, 
   const int blocksize, const int blockstart) {
   if (hasWebInstructions && ntotalFiles > blocksize && blockstart == 0) {
      // limits load to blocksize svg files
      std::cout 
         << ";the total number files generated in a single web\n"
         << ";run is limited to blocksize (" << blocksize
         << ";). If you need more, consider\n"
         << ";running directly on a computer from a command interface.\n"
         << ";On the web the blocksize limit is usually 20.\n"
         << ";Try with a smaller number of files and/or perturbations\n"
         << ";or do multiple runs using control variable blockstart\n\n";
      // exit(-1);
   }
}

std::pair<int, int> GetProcessingLimits(
   const int blockstart,
   const int blocksize,
   const int numFiles,
   const bool isWeb)
{
   int newBlockSize(std::max(1, blocksize));
   newBlockSize = std::min(newBlockSize, numFiles);
   const int newBlockStart(std::max(0, blockstart));
   int runEnd;

   if (isWeb) {
      newBlockSize = std::min(newBlockSize, 20);
      runEnd = std::min(numFiles, newBlockStart + newBlockSize);
   }
   else
   {
      runEnd = numFiles;
   }

   return { newBlockStart, runEnd };  // does not account for start off  the end 
}

int main(int argc, char* argv[]) {

   const std::string programName("Follow");
   std::cout << "; Follow \n";

   WebIO webio(argc, argv, programName, 0);

   try {
      ControlVariables controlVars;
      std::vector<LatticeCell> inputVectors;

      std::cout << "; Enter control variables and input vectors (type 'end' to finish):\n";
      InputHandler::readMixedInput(controlVars, inputVectors, std::cin);

      //
      //
      //------------ all input complete ---------------
      //
      //

      WebLimits wl;
      wl.Update(webio); // update using webio FIRST
      wl.Update(controlVars);

      std::cout << controlVars << std::endl;
      std::cout << "; Number of input vectors read: " << inputVectors.size() << std::endl;

      if (inputVectors.empty()) {
         std::cout << "; No input vectors provided. Exiting." << std::endl;
         return 0;
      }

      if (!webio.m_hasWebInstructions) {
         webio.SetTimeStamp(controlVars.timestamp);
      }

      if (!validateFollowerMode(controlVars.followerMode, inputVectors.size())) {
         throw std::runtime_error("; Not enough input vectors for the selected follower mode");
      }

      //
      //
      //------------ end of input treatment ---------------
      //
      //

      std::vector<FollowInstance>  instances = CreateFollowInstanceList(controlVars, inputVectors);

      const int ntotal = static_cast<int>(instances.size());
      const std::pair<int, int> limits{ GetProcessingLimits(wl.GetBlockStart(),
         wl.GetBlockSize(), ntotal, webio.m_hasWebInstructions)};

      CheckWebFileLimits(ntotal, webio.m_hasWebInstructions,
         wl.GetBlockSize(), wl.GetBlockStart());

      webio.m_blocksize = ntotal;
      webio.CreateFilenamesAndLinks(ntotal, controlVars.filePrefix);

      const std::vector<std::string>& RawfileNameList = webio.m_FileNameList;
      const std::vector<std::string>& basicfileNameList = webio.m_basicfileNameList;
      const std::vector<std::string>& FullfileNameList = webio.m_FullfileNameList;

      controlVars.updateFilenames(RawfileNameList);

      for (size_t i = 0; i < basicfileNameList.size(); ++i) {
         instances[i].AddFileNames(
            RawfileNameList[i],
            basicfileNameList[i],
            FullfileNameList[i]);
         //std::cout << instances[i] << std::endl;
      }

      if (limits.first > ntotal) {
         std::cout << "; start point is already past the end point\n";
      }

      //
      //
      //------------ start treatment of the subset of instances between the limits ---------------
      //
      //

      for (size_t i = limits.first; i < limits.second; ++i) {
         std::cout << "; Follow graphics file(s) " <<
            i << "  " << instances[i].GetFullFileName() << std::endl;
      }

      Follow follow(controlVars);
      for (size_t i = limits.first; i < limits.second; ++i) {
         const int trialNum = instances[i].GetTrial();
         const int perturbation = instances[i].GetPerturbation();
         follow.processPerturbation(trialNum, perturbation, instances[i] );
      }
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }

   return 0;
}
