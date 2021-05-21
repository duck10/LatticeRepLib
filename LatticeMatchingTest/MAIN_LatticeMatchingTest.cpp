// LatticeMatchingTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <algorithm>
#include <utility>

#include "CellInputData.h"
#include "LRL_CreateFileName.h"
#include "CS6Dist.h"
#include "DeloneTypeList.h"
#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_LatticeMatcher.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "MatN.h"
#include "MatS6.h"
#include "rhrand.h"
#include "S6.h"
#include "S6Dist.h"
#include "TNear.h"

std::string Letters(void) {
   return "V,G,D,S,P,A,B,C,I,F,R,C3,G6,S6,B4,D7,H";
}

std::vector<LRL_ReadLatticeData> GetInputCells(void) {
   const std::string letters = Letters();
   std::string lattice;
   std::vector<LRL_ReadLatticeData> cellDataList;
   LRL_ReadLatticeData rcd;
   while (lattice != "EOF") {
      rcd.read();
      lattice = rcd.GetLattice();
      if ((!lattice.empty()) && (letters.find(static_cast<char>(toupper(lattice[0]))) != std::string::npos))
         cellDataList.push_back(rcd);
   }

   return cellDataList;
}

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

int main()
{
   std::cout << " text input" << std::endl;
   const std::vector<LRL_ReadLatticeData> inputList = GetInputCells();
   MatS6 mat_primitive_reduced;
   const std::string referenceLattice = inputList[0].GetLattice();
   const LRL_Cell cell_reference = LatticeConverter::SellingReduceCell(referenceLattice, inputList[0].GetCell(), mat_primitive_reduced);
   const MatS6 mat_reference = MatS6::Inverse(mat_primitive_reduced);
   const std::vector<S6> vLat = GetInputSellingReducedVectors(inputList);

   std::cout << std::endl;

   LRL_LatticeMatcher lm;
   lm.SetReferenceLattice(vLat[0]);

   //std::cout << LRL_Cell_Degrees(vLat[0]) << "  REFERENCE   " << vLat[0] << std::endl << std::endl;

   std::vector<S6> vs = lm.MatchReference(vLat, mat_reference);

   for (size_t i2 = 0; i2 < vs.size(); ++i2) {
      const double dist = (G6(vs[0]) - G6(vs[i2])).Norm();

      std::cout << referenceLattice + "  "
         << LRL_Cell_Degrees(vs[i2]) ;
      if (i2 == 0) std::cout << "   REFERENCE";
      std::cout << std::endl;
   }

   //
   // Find how many points are near to each of the input points
   // in the lattice matched results. The search radius will need
   // to be changed for each example. See variable searchRadius.
   // The search is done in space S6.
   //
   CNearTree<S6> matched(vs);

   std::vector<S6> vmatout;
   const double searchRadius = 1500.0;
   std::cout << std::endl << "How many points are near to each data point (in S6), with radius: searchRadius" << std::endl;
   for (size_t i = 0; i < vs.size(); ++i) {
      const long n = matched.FindInSphere(searchRadius, vmatout, vs[i]);
      std::cout << " " << LRL_Cell_Degrees(vs[i]) << "   " << (vs[0] - vs[i]).Norm() << "      " << n - 1 << std::endl;
   }

   //const double maxradius = lm.GetMaxRadius();
   //std::cout << "Maximum Radius " << maxradius << std::endl;

}