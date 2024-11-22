#include "ControlVariables.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

#include "CS6Dist.h"
#include "CS6Dist.cpp"
#include "Follow.h"
#include "FollowUtils.h"
#include "G6.h"
#include "InputHandler.h"
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

void  CheckWebFileLimits(const int ntotalFiles, const bool hasWebInstructions) {
   if (hasWebInstructions && ntotalFiles > 8) {
      // limits web resources to 8 svg files
      // web is determined by command line input containing the string --
      std::cout << "Do to limitations of running on a web server,\n";
      std::cout << "the total number files generated in a single web\n";
      std::cout << "run is limited to 8. If you need more, consider\n";
      std::cout << "running directly on a computer from a command interface.\n";
      exit(55555);
   }
}


int main(int argc, char* argv[]) {
   WebIO webio(argc, argv, "Follow", 0);

   try {
      ControlVariables controlVars;
      std::vector<G6> inputVectors;

      std::cout << "Enter control variables and input vectors (type 'end' to finish):\n";
      InputHandler::readMixedInput(controlVars, inputVectors, std::cin);

      std::cout << "\nControl Variables set:\n" << controlVars << std::endl;
      std::cout << "Number of input vectors read: " << inputVectors.size() << std::endl;

      if (inputVectors.empty()) {
         std::cout << "No input vectors provided. Exiting." << std::endl;
         return 0;
      }
      if (!webio.m_hasWebInstructions) {
         webio.SetTimeStamp(controlVars.timestamp);
      }

      if (!validateFollowerMode(controlVars.followerMode, inputVectors.size())) {
         throw std::runtime_error("Not enough input vectors for the selected follower mode");
      }

      const int ntotal = ComputeTotalNumberOfRuns(controlVars, inputVectors.size());

      CheckWebFileLimits(ntotal, webio.m_hasWebInstructions);

      webio.CreateFilenamesAndLinks(ntotal, "FOL");

      controlVars.updateFilenames(webio.m_basicfileNameList);

      Follow follow(controlVars);
      follow.run(controlVars.filenames, inputVectors);

   }
   catch (const std::exception& e) {
      std::cerr << "An error occurred: " << e.what() << std::endl;
      return 1;
   }

   return 0;
}
