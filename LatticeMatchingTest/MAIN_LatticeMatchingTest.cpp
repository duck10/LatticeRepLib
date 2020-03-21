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
   const std::vector<LRL_ReadLatticeData> input = GetInputCells();
   MatS6 mat_reference;
   const LRL_Cell cell_reference = LatticeConverter::SellingReduceCell(input[0].GetLattice(), input[0].GetCell(), mat_reference);

   const std::vector<S6> vLat = GetInputSellingReducedVectors(input);
   std::cout << std::endl << "Reduced input cells " << std::endl;
   for (size_t lat = 0; lat < vLat.size(); ++lat) {
      std::cout << LRL_Cell_Degrees(vLat[lat]) << std::endl;
   }
   std::cout << std::endl;
   std::cout << std::endl;
   std::cout << std::endl;

   RHrand rhrand(s6RandomSeed);

   // do some tests
   LRL_LatticeMatcher lm;
   for (size_t i1 = 0; i1 < input.size(); ++i1) {
      for (size_t i2 = 0; i2 < input.size(); ++i2) {
         if (i1 == i2) continue;
         lm.SetReferenceLattice(vLat[i1]);
         const S6 s6match = lm.MatchReference(vLat[i2]);

         std::cout << i1 << "    " << (vLat[i1]) << std::endl;
         std::cout << i2 << "    " << (s6match) << std::endl;
         std::cout << (vLat[i1] - s6match).norm() << std::endl;
         std::cout << std::endl;
      }
   }

   //if (vLat.size() > 0) {
   //   LRL_LatticeMatcher lm;
   //   lm.SetReferenceLattice(mv_reducedReference);
   //   //lm.SetReferenceLattice( MV_Pair( cell_reference, MatS6().unit() ));
   //   std::cout << "MV tree size = " << lm.size() << std::endl;
   //   //std::cout << "before lattice match " << LRL_CreateFileName( ).Create( "", "" ) << std::endl;

   //   const std::vector<S6> vs6(lm.MatchReference(vLat));
   //   //std::cout << "after lattice match " << LRL_CreateFileName( ).Create( "", "" ) << std::endl;

   //   std::cout << "matched cells including reference" << std::endl;
   //   for (size_t lat = 0; lat < vLat.size(); ++lat) {
   //      std::cout << LRL_Cell_Degrees(vs6[lat]) << std::endl;
   //      //if (!vs6[lat].IsValid()) throw;
   //   }
   //   exit(0);
}