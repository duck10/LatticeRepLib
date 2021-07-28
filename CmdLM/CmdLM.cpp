
#include <iostream>

#include "LatticeConverter.h"
#include "LRL_LatticeMatcher.h"
#include "LRL_ReadLatticeData.h"
#include "S6.h"


std::vector<S6> GetInputSellingReducedVectors(const std::vector<LRL_ReadLatticeData>& input) {
   std::vector<S6> v;
   LatticeConverter converter;

   MatS6 mat;
   for (size_t i = 0; i < input.size(); ++i) {
      const S6 s6 = converter.SellingReduceCell(input[i].GetLattice(), input[i].GetCell(), mat);
      v.push_back(s6);
   }
   return v;
}

std::string ListOutput(const std::vector<S6>& vs, const std::string& referenceLattice) {
   std::ostringstream ostr;
   for (size_t i2 = 0; i2 < vs.size(); ++i2) {
      ostr << referenceLattice + "  " << LRL_Cell_Degrees(vs[i2]);
      if (i2 == 0) ostr << "   REFERENCE";
      ostr << std::endl;
   }
   return ostr.str();
}

MatS6 GetMatrixToReturnToReference(const std::string& referenceLattice, const LRL_Cell& cell) {
   MatS6 mat_primitive_reduced;
   const LRL_Cell cell_reference = LatticeConverter::SellingReduceCell(referenceLattice, cell, mat_primitive_reduced);
   return  MatS6::Inverse(mat_primitive_reduced);
}

int main()
{
   std::cout << "; Lattice Matching" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();
   if (inputList.empty()) {
      std::cout << "Lattice match requires something to match to" << std::endl;
   } else {
      const std::string referenceLattice = inputList[0].GetLattice();
      const MatS6 mat_reference = GetMatrixToReturnToReference(referenceLattice, inputList[0].GetCell()); // NOTE: input cell

      const std::vector<S6> vLat = GetInputSellingReducedVectors(inputList);

      LRL_LatticeMatcher lm;
      lm.SetRecursionDepth(2);
      lm.SetReferenceLattice(vLat[0]); // Note that this uses the reduced cell

      //lm.PrintMVtree();

      const std::vector<S6> vs = lm.MatchReference(vLat, mat_reference);
      std::cout << ListOutput(vs, referenceLattice);
   }

}
