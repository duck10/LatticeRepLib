#include <iostream>
#include <vector>
#include <string>

#include "CmdP3AsymControls.h"
#include "B4.h"
#include "ProgramSetup.h"

#include "LRL_Cell.h"
#include "LRL_ToString.h"
#include "LRL_Vector3.h"

static std::vector<Vector_3> GetBaseVectors(const LRL_Cell& cell) {
   const B4 b4(cell);
   return { b4[0], b4[1], b4[2] };
}



static LRL_Cell GetCellFromBaseVectors(const std::vector<Vector_3>& vecs) {
   LRL_Cell out;
   for (size_t i = 0; i < 3; ++i) {
      out[i] = vecs[i].norm();
   }
   out[3] = vecs[1].Dot(vecs[2]);
   out[4] = vecs[0].Dot(vecs[2]);
   out[5] = vecs[0].Dot(vecs[1]);
   return out;
}



// Inject a controlled asymmetry by modifying one vector component
LRL_Cell applyAsymmetry(const LRL_Cell& cell, const double epsilon, const size_t targetIndex) {
   std::vector<Vector_3> vecs = GetBaseVectors(cell);

   if (targetIndex < vecs.size()) {
      vecs[targetIndex][1] += epsilon;  // Inject asymmetry into y-component
   }
   return GetCellFromBaseVectors(vecs);
}

int main(int argc, char* argv[]) {
   std::cout << "; CmdP3Asym — Inject controlled asymmetry\n";

   // Default parameters
   int stride = 10;
   double epsilon = 0.05;
   size_t targetIndex = 1;

   // Simple command-line parsing
   if (argc > 1) stride = std::max(1, atoi(argv[1]));
   if (argc > 2) epsilon = atof(argv[2]);
   if (argc > 3) targetIndex = static_cast<size_t>(atoi(argv[3]));

   CmdP3AsymControls controls;

   const BasicProgramInput<CmdP3AsymControls> setup("CmdP3Asym", controls);

   const std::vector<LatticeCell> inputCells = setup.getInputList();
   if (inputCells.empty()) {
      std::cerr << "; CmdP3Asym error: No input cells provided.\n";
      return 1;
   }

   std::vector<LRL_Cell> outputCells;
   for (size_t i = 0; i < inputCells.size(); ++i) {
      const bool applyShift = (i % stride == 0);
      const LRL_Cell modifiedCell = applyShift ?
         applyAsymmetry(inputCells[i].getCell(), epsilon, targetIndex) :
         inputCells[i].getCell();

      outputCells.push_back(modifiedCell);
   }

   for (const auto& cell : outputCells) {
      std::cout << LRL_ToString("P", cell) << std::endl;
   }

   std::cout << "; CmdP3Asym complete: injected epsilon=" << epsilon
      << " at stride=" << stride
      << ", target vector index=" << targetIndex << std::endl;

   return 0;
}
