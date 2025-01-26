#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "CmdScaleControls.h"
#include "DC7u.h"
#include "FileOperations.h"
#include "InputHandler.h"
#include "LRL_ReadLatticeData.h"
#include "LRL_StringTools.h"
#include "ProgramSetup.h"
#include "RI.h"
#include "S6.h"
#include "V7.h"
#include "WebIO.h"

template<typename T>
static T Scale(const double scale, const T& t) {
   return scale * t / t.norm();
}

template<typename T>
static void Output(const double scaleFactor, const T& t, const std::string& label) {
   std::cout << T::GetName() + " " << Scale<T>(scaleFactor, t) << " " + label << std::endl;
}

static void ScaleByS6(const std::vector<LatticeCell>& inputList) {
   if (inputList.empty()) return;
   const double scaleFactor = S6(inputList[0].getCell()).norm();
   std::string label =  "REFERENCE";
   for (const auto& i : inputList) {
      const S6 s6(i.getCell());
      const S6 newS6 = s6 * scaleFactor / s6.norm();
      std::cout << S6::GetName() + " " << newS6 << label << std::endl;
      label = " scaled in S6";
   }
}

static void ScaleByVolume(const std::vector<LatticeCell>& inputList) {
   if (inputList.empty()) return;
   const double scaleFactor = LRL_Cell(inputList[0]).Volume();
   std::string label = "REFERENCE";
   for (const auto& i : inputList) {
      const LRL_Cell cell(i);
      const double cellV = cell.Volume();
      const double linearRatio = pow(scaleFactor / cellV, 1.0 / 3.0);
      std::cout << i.getLatticeType() + " " 
         << LRL_Cell_Degrees(cell * linearRatio) << label << std::endl;
      label = " scaled by volume";
   }
}

static void ProcessInputs(const std::vector<LatticeCell>& inputList, const CmdScaleControls& controls) {

   if (inputList.empty()) return;

   if ( controls.scaleType ==  "S6")
   {
      ScaleByS6(inputList);
   }
   else if (controls.scaleType =="V" || controls.scaleType == "VOLUME")
   {
      ScaleByVolume(inputList);
   }
}

static std::string GetTypeFromArgs(const int argc, char* argv[]) {
   const std::vector<std::string> allowedTypes{ "S6","V7","U","DC7U","RI" };
   if (argc > 1) {
      const std::string arg(LRL_StringTools::strToupper(argv[1]));
      return CmdScaleUtils::verifyScaleType(arg);
   }
   return "S6";
}

int main(int argc, char* argv[]) {
   std::cout << "; Scale Vectors" << std::endl;

   try {
      WebIO webio(argc, argv, "CmdScale", 0);

      CmdScaleControls controls;
      controls.setWebRun(webio.m_hasWebInstructions);

      // Always check command line args for type, even when running from web
      const std::string typeArg = GetTypeFromArgs(argc, argv);
      controls.setScaleType(typeArg);

      const BasicProgramInput<CmdScaleControls> dc_setup("CmdScale", controls);

      if (dc_setup.getInputList().empty()) {
         throw std::runtime_error("; No input vectors provided");
      }

      std::cout << "; Number of input vectors read: " << dc_setup.getInputList().size() << std::endl;

      if (controls.shouldShowControls()) {
         std::cout << controls << std::endl;
      }

      ProcessInputs(dc_setup.getInputList(),controls);

      return 0;
   }
   catch (const std::exception& e) {
      std::cerr << "; An error occurred: " << e.what() << std::endl;
      return 1;
   }
}