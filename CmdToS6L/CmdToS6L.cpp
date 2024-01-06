
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#include "LRL_DataToSVG.h"
#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "LRL_MinMaxTools.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "RI.h"
#include "S6.h"
#include "S6L.h"
#include "Selling.h"

static const std::vector<MatS6> refl_one = MatS6::GetReflections();

std::vector<S6> GenerateReflections(const S6& s6) {
   std::vector<S6> out;
   for (size_t i = -0; i < refl_one.size(); ++i) {
      out.emplace_back(refl_one[i] * s6);
   }
   return out;
}

S6  MakeS6L(const S6& s6) {
   const S6 s(sqrt(s6[0]), sqrt(s6[1]), sqrt(s6[2]), sqrt(s6[3]), sqrt(s6[4]), sqrt(s6[5]));
   return s;
}

int main(int argc, char* argv[]) {
   static std::string name = ""; // blank or unrecognized gives all types

   std::cout << "; To Linearized S6 and root invariants" << std::endl;

   static const std::string nameRootFragment("root");
   static const std::string nameRootSLFragment("linear");
   if (argc > 1) {
      const std::string arg(argv[1]);
      if (arg.find(nameRootFragment) != std::string::npos) name = "R";
      if (arg.find(nameRootSLFragment) != std::string::npos) name = "S";
      if (arg.find("R") != std::string::npos) name = "R";
      if (arg.find("S") != std::string::npos) name = "S";
   }


   const std::vector<LRL_ReadLatticeData> inputList = LRL_ReadLatticeData().ReadLatticeData();

   std::vector<S6> summary;

   std::vector<std::pair<S6,std::string> >RIs;
   std::vector<S6L> SLs;
   for (size_t i = 0; i < inputList.size(); ++i) {
      const LRL_Cell cell = LatticeConverter::MakePrimitiveCell(inputList[i].GetLattice(), inputList[i].GetCell());
      S6 red;
      const bool b = Selling::Reduce(cell, red);
      const S6 s6(RI::AllPositive(red));

      // this is the calculation (at least my attempt) to compute Bright and Kurlin's "Root Invariant"
      RIs.emplace_back(RI::MakeRI(inputList[i], s6));
      SLs.emplace_back(red);
   }

   if ( name.empty() || name[0]=='R')
   {
      for (size_t i = 0; i < RIs.size(); ++i) {
         std::cout << "RI " << RI::ResetZeros(RIs[i].first) << std::endl
            << "; " << RIs[i].second << std::endl;
      }
   }

   std::cout << std::endl;

   if ( name.empty() || name[0] =='S')
   {
      for (size_t i = 0; i < SLs.size(); ++i) {
         S6L::ResetZeros(SLs[i]);
         std::cout << "SL " << S6L::ResetZeros(SLs[i]) << std::endl;
      }
   }
}
