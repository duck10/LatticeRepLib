#include <iostream>

#include "CmdLMControls.h"
#include "LatticeCell.h"
#include "LatticeConverter.h"
#include "LRL_LatticeMatcher.h"
#include "LRL_ToString.h"
#include "ProgramSetup.h"
#include "S6.h"

std::vector<S6> GetInputSellingReducedVectors(const std::vector<LatticeCell>& input) {
   std::vector<S6> v;

   MatS6 mat;
   for (size_t i = 0; i < input.size(); ++i) {
      const S6 s6 = LatticeConverter::SellingReduceCell(input[i].getLatticeType(), input[i].getCell(), mat);
      v.push_back(s6);
   }
   return v;
}

MatS6 GetMatrixToReturnToReference(const std::string& referenceLattice, const LRL_Cell& cell) {
   MatS6 mat_primitive_reduced;
   const LRL_Cell cell_reference = LatticeConverter::SellingReduceCell(referenceLattice, cell, mat_primitive_reduced);
   return  MatS6::Inverse(mat_primitive_reduced);
}

int main() {
   std::cout << "; Lattice Matching" << std::endl;

   try {
      CmdLMControls controls;
      const BasicProgramInput<CmdLMControls> dc_setup("CmdLM", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; Lattice match requires something to match to");
      }

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      const std::string referenceLattice = dc_setup.getInputList()[0].getLatticeType();
      const MatS6 mat_reference = GetMatrixToReturnToReference(referenceLattice, dc_setup.getInputList()[0].getCell());

      std::vector<S6> vLat = GetInputSellingReducedVectors(dc_setup.getInputList());

      LRL_LatticeMatcher lm;
      lm.SetRecursionDepth(2);
      lm.SetReferenceLattice(vLat[0]); // Note that this uses the reduced cell

      const std::vector<S6> vs = lm.MatchReference(vLat, mat_reference);

      // Now use the member function instead of the standalone function
      std::cout << lm.ListOutput(vs, referenceLattice);

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}
