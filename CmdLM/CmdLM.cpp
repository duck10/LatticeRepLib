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
   LatticeConverter converter;

   MatS6 mat;
   for (size_t i = 0; i < input.size(); ++i) {
      const S6 s6 = converter.SellingReduceCell(input[i].getLatticeType(), input[i].getCell(), mat);
      v.push_back(s6);
   }
   return v;
}

std::string ListOutput(const std::vector<S6>& vs,
   const std::vector<double>& angles,
   const std::string& referenceLattice) {

   std::ostringstream ostr;
   for (size_t i2 = 0; i2 < vs.size(); ++i2) {
      ostr << referenceLattice + "  " << LRL_Cell_Degrees(vs[i2]);
      ostr << LRL_ToString("(", angles[i2], " degrees in S6)");
      if (i2 == 0) {
         ostr << "   REFERENCE";
      }
      ostr << std::endl;
   }
   return ostr.str();
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
      const std::vector<double> angles(lm.GetAngleAgreements());
      std::cout << ListOutput(vs, angles, referenceLattice);

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}