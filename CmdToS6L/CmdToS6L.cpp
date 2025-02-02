#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#include "BlockUtils.h"
#include "InputHandler.h"

#include "BaseControlVariables.h"
#include "CmdToS6LControls.h"
#include "FileOperations.h"
#include "InputHandler.h"
#include "LatticeCell.h"
#include "LatticeConverter.h"
#include "LRL_Cell.h"
#include "LRL_DataToSVG.h"
#include "LRL_MinMaxTools.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
#include "ProgramSetup.h"
#include "RI.h"
#include "S6.h"
#include "S6L.h"
#include "Selling.h"
#include "WebIO.h"

static const std::vector<MatS6> refl_one = MatS6::GetReflections();

static std::vector<S6> GenerateReflections(const S6& s6) {
   std::vector<S6> out;
   for (size_t i = 0; i < refl_one.size(); ++i) {
      out.emplace_back(refl_one[i] * s6);
   }
   return out;
}

static S6 MakeS6L(const S6& s6) {
   const S6 s(sqrt(s6[0]), sqrt(s6[1]), sqrt(s6[2]), sqrt(s6[3]), sqrt(s6[4]), sqrt(s6[5]));
   return s;
}

static std::vector<std::complex<double>> MakeComplexS6L(const S6& s6) {
   const std::vector<std::complex<double>> out{
      sqrt(s6[0]), sqrt(s6[1]), sqrt(s6[2]),
      sqrt(s6[3]), sqrt(s6[4]), sqrt(s6[5])
   };
   return out;
}

static void ProcessInputs(const std::vector<LatticeCell>& inputList,
   const CmdToS6LControlVariables& controls,
   size_t blockStart, size_t blockEnd) {

   std::vector<std::pair<S6, std::string>> RIs;
   std::vector<S6L> SLs;

   for (size_t i = blockStart; i < blockEnd && i < inputList.size(); ++i) {
      const auto& input = inputList[i];
      const LRL_Cell cell = LatticeConverter::MakePrimitiveCell(input.getLatticeType(), input.getCell());
      S6 red;
      const bool b = Selling::Reduce(cell, red);
      const S6 s6Pos(RI::MakeAllPositive(red));

      RIs.emplace_back(s6Pos, "P");
      SLs.emplace_back(red);
   }

   if (controls.hasRoot()) {
      for (const auto& ri : RIs) {
         std::cout << "RI " << RI::ResetZeros(ri.first) << std::endl
            << "; " << ri.second << std::endl;
      }
   }

   std::cout << std::endl;

   if (controls.hasLinear()) {
      for (auto& sl : SLs) {
         S6L::ResetZeros(sl);
         std::cout << "SL " << S6L::ResetZeros(sl) << std::endl;
      }
   }
}

int main(int argc, char* argv[]) {
   std::cout << "; To Linearized S6 and root invariants" << std::endl;

   try {
      WebIO webio(argc, argv, "CmdToS6L", 0);

      CmdToS6LControlVariables controls;
      controls.setWebRun(webio.m_hasWebInstructions);

      // if there is input from commandline, then set it in controls before reading input
      if (webio.m_hasWebInstructions) {
         if (argc > 1) {
            controls.setFormat(std::string(argv[1]));
         }
      }

      const WebProgramInput<CmdToS6LControlVariables> dc_setup(argc, argv,"CmdToS6L", controls.getHasWebInput(), controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      std::cout << "; Number of input vectors read: " << dc_setup.getInputList().size() << std::endl;

      if (controls.getShowControls()) {
         std::cout << controls << std::endl;
      }

      ProcessInputs(dc_setup.getInputList(), controls, 
         0, dc_setup.getInputList().size());
      //static void ProcessInputs(const std::vector<LatticeCell>&inputList,
      //   const CmdToS6LControlVariables & controls,
      //   size_t blockStart, size_t blockEnd) {

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}