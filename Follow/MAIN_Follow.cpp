
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

#include "ControlVariables.h"
#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include "Follow.h"
#include "FollowUtils.h"
#include "G6.h"
#include "InputHandler.h"
#include "LatticeCell.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_ReadLatticeData.h"
#include "Niggli.h"
#include "Polar.h"
#include "S6.h"
#include "Selling.h"
#include "WebIO.h"


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

void  CheckWebFileLimits(const int ntotalFiles, const bool hasWebInstructions, const int blocksize) {
   if (hasWebInstructions && ntotalFiles > blocksize) {
      // limits load to blocksize svg files
      std::cout << "the total number files generated in a single web\n";
      std::cout << "run is limited by blocksize (" << blocksize << "). If you need more, consider\n";
      std::cout << "running directly on a computer from a command interface.\n";
      if (hasWebInstructions) {
        std::cout << "On the web the blocksize limit is usually 20.\n";
      }
      std::cout << "Try with a smaller number of files and/or perturbations\n";
      std::cout << "or do multiple runs using control variable blockstart\n";
      // exit(-1);
   }
}


int main(int argc, char* argv[]) {
   const std::string programName("Follow");
   std::cout << "; Follow \n";
   std::vector<std::string> RawFileNameList;

   WebIO webio(argc, argv, programName, 0);

   try {
      ControlVariables controlVars;
      std::vector<LatticeCell> inputVectors;

      std::cout << "; Enter control variables and input vectors (type 'end' to finish):\n";
      InputHandler::readMixedInput(controlVars, inputVectors, std::cin);

      size_t blockstart = webio.m_blockstart;
      if (controlVars.blockstart >  blockstart) blockstart = controlVars.blockstart;
      size_t blocksize = webio.m_blocksize;
      if (webio.m_hasWebInstructions && blocksize > controlVars.blocksize) blocksize  = controlVars.blocksize;
      if (blocksize < controlVars.blocksize) blocksize  = controlVars.blocksize;

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

      const int ntotal = ComputeTotalNumberOfRuns(controlVars, int(inputVectors.size()));

      CheckWebFileLimits(ntotal, webio.m_hasWebInstructions, blocksize);

      webio.CreateFilenamesAndLinks(ntotal, controlVars.filePrefix);

      const std::vector<std::string>& basicfileNameList = webio.m_basicfileNameList;
      const std::vector<std::string>& FullfileNameList = webio.m_FullfileNameList;
      RawFileNameList.clear();

      std::cout << "; Follow cell block start " << blockstart << std::endl;
      std::cout << "; Follow cell block size " << blocksize << std::endl;
      std::cout << std::endl;

      for (size_t i = 0; i < blockstart; ++i) {
           RawFileNameList.push_back(std::string(""));
      }
      for (size_t i = blockstart; i < (FullfileNameList.size()) && (i < blockstart+blocksize); ++i)
      {
         RawFileNameList.push_back(webio.m_FileNameList[i]);
         std::cout << "; Follow graphics file(s) " <<
            i  << "  " << FullfileNameList[i - blockstart] << std::endl;
      }
      for (size_t i  = blockstart+blocksize; i < (FullfileNameList.size()); ++i )
      {
          RawFileNameList.push_back(std::string(""));
      }

      controlVars.updateFilenames(webio.m_FileNameList);

      Follow follow(controlVars);
      if (webio.m_hasWebInstructions) {
         follow.run(RawFileNameList, inputVectors, (size_t) blockstart, (size_t) blocksize);
      } else {
         follow.run(RawFileNameList, inputVectors);
      }

   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }

   return 0;
}
