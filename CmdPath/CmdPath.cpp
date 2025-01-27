#include <iostream>
#include <vector>

#include "CmdPathControls.h"
#include "G6.h"
#include "LatticeConverter.h"
#include "LRL_ReadLatticeData.h"
#include "Niggli.h"
#include "ProgramSetup.h"

std::vector<S6> CreatePathBetweenTwoPoints(const int npts, const S6& delta, const S6& s1, const S6& s2) {
   std::vector<S6> out(1, s1);
   for (int i = 0; i < npts - 1; ++i) {
      const S6 diff = double(i + 1) * delta;
      const S6 temp = s1 + diff;
      out.push_back(temp);
   }
   return out;
}

int main(int argc, char* argv[]) {
   std::cout << "; Path generator" << std::endl;

   try {
      // Get path length from command line if provided
      int commandLineLength = 20;
      if (argc > 1) {
         commandLineLength = atoi(argv[1]);
         if (commandLineLength == 0) commandLineLength = 20;
      }

      CmdPathControls controls(commandLineLength);
      const BasicProgramInput<CmdPathControls> dc_setup("CmdPath", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      std::vector<S6> s6Cells;
      for (const auto& input : dc_setup.getInputList()) {
         s6Cells.push_back(input.getCell());
      }

      // Handle single input case
      if (s6Cells.size() == 1) {
         const std::string lattice = dc_setup.getInputList()[0].getLatticeType();
         const G6 g6red = LatticeConverter().NiggliReduceCell(lattice, s6Cells[0]);

         if (G6(s6Cells[0] - g6red).norm() > 1.0) {
            s6Cells.emplace_back(g6red);
            std::cout << "; Path Generator found only one point. Using Niggli reduced input for 2nd point" << std::endl;
         }
         else {
            std::cout << "; Path Generator found only one point. Using random cell for 2nd point" << std::endl;
            s6Cells.emplace_back(S6::rand());
         }
      }

      std::vector<S6> path;
      const int pathLength = controls.getPathLength();

      for (size_t i = 0; i < s6Cells.size() - 1; ++i) {
         const S6 delta = (s6Cells[i + 1] - s6Cells[i]) / double(pathLength - 1);
         const std::vector<S6> pathStep = CreatePathBetweenTwoPoints(pathLength, delta, s6Cells[i], s6Cells[i + 1]);
         path.insert(path.end(), pathStep.begin(), pathStep.end());
      }

      std::cout << "; no. of points output is " << path.size() << std::endl;
      for (size_t i = 0; i < path.size(); ++i) {
         const size_t segmentOrdinal = i % pathLength;
         if (segmentOrdinal == 0) std::cout << "; path segment " << i / pathLength + 1 << std::endl;
         std::cout << "S  " << path[i]
            << "  segment " << i / pathLength + 1
            << "  point " << segmentOrdinal + 1
            << " count " << i + 1 << std::endl;
      }

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}