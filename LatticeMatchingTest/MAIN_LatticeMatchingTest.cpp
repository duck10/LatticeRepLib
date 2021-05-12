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
   MatS6 mat_reference;
   const LRL_Cell cell_reference = LatticeConverter::SellingReduceCell(inputList[0].GetLattice(), inputList[0].GetCell(), mat_reference);
   mat_reference = MatS6::Inverse(mat_reference);
   const std::vector<S6> vLat = GetInputSellingReducedVectors(inputList);

   std::cout << std::endl << "Reduced input cells " << std::endl;
   for (size_t lat = 0; lat < vLat.size(); ++lat) {
      std::cout << lat << "   " << LRL_Cell_Degrees(vLat[lat]) << "   " << (vLat[lat]) << std::endl;
   }
   std::cout << "=============================" << std::endl;

   LRL_LatticeMatcher lm;
   lm.SetReferenceLattice(vLat[0]);
   for (size_t i1 = 0; i1 < 1; ++i1) {
      std::cout << LRL_Cell_Degrees(vLat[0]) << "  REFERENCE   "  << vLat[0] << std::endl << std::endl;

      std::vector<S6> vs;


      for (size_t i2 = 0; i2 < inputList.size(); ++i2) {
         const S6 s6match = lm.MatchReference(vLat[i2]);
         vs.push_back(mat_reference*s6match);
      }

      const std::vector<S6> vmatches = lm.MatchReference(vs);

      for (size_t i2 = 0; i2 < vs.size(); ++i2 ) {
         const double dist = (vmatches[0] - vmatches[i2]).Norm();

         std::cout << " " << i2 << "  " << LRL_Cell_Degrees(vs[i2]) << " DC_delta " << dist
            << "  S6_reduced " << LRL_Cell_Degrees(vLat[i2]);
         if (i2 == 0) std::cout << "   REFERENCE";
         std::cout << std::endl;
      }

      //CNearTree<S6> matched(vmatches);

      //std::vector<S6> vmatout;
      //for (size_t i = 0; i < vmatches.size(); ++i) {
      //   const long n = matched.FindInSphere(150., vmatout, vmatches[i]);
      //   std::cout << " " << LRL_Cell_Degrees(vmatches[i]) << "   " << (vmatches[0]-vmatches[i]).Norm() << "      " << n-1 << std::endl;
      //}
      //lm.PrintMVtree();
      //const double maxradius = lm.GetMaxRadius();
      //std::cout << "Maximum Radius " << maxradius << std::endl;
   }
}