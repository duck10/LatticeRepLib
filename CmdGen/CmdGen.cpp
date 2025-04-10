﻿#include <ctype.h>
#include <iostream>
#include <map>

#include "CmdGenControls.h"
#include "CmdGenTypes.h"
#include "FileOperations.h"
#include "GenerateLatticeTypeExamples.h"
#include "G6.h"
#include "InputHandler.h"
#include "LRL_Cell.h"
#include "LRL_StringTools.h"
#include "Niggli.h"
#include "NumericTools.h"
#include "Polar.h"
#include "ProgramSetup.h"
#include "S6BoundaryTransforms.h"
#include "WebIO.h"

/*
; test commands for CmdGen verification
cls
CmdGen 1 1
CmdGen 1 44
CmdGen 1 -1
CmdGen 1  66

CmdGen 1 m
CmdGen 1 h

CmdGen 1 O
CmdGen 1 S
CmdGen 1 C


CmdGen 1 mP
CmdGen 1 mS
CmdGen 1 mC
CmdGen 1 hR
CmdGen 1 hP

CmdGen 1 O4
CmdGen 1 C3
CmdGen 1 A2

CmdGen 1
CmdGen 1 xxxx
CmdGen
CmdGen xxx

CmdGen 1 all
CMdGen 1 all niggli and delone

CmdGen 1 delone
CmdGen 1 niggli
;
*/




// how many examples of each type to generate
static int ngen = 1;

/*
The "name" variable is the designator for the Bravais type(s)
you want to generate. Because the code below works in Niggli
space, the name should be one of the following types:
1) The number of a single type from the International Tables (1-44)
2) Bravais type, such as cI for body-centered cubic or oI for
body centered orthorhombic. Because there are multiple oI
cases, all will be used. (case sensitive) Hexagonal is hP,
and rhombohedral is hR.
3) Crystal system such as: c, t, h, o, m, a for cubic,
tetragonal, rhombohedral, hexagonal, orthorhombic, monoclinic,
anorthic (=triclinic). (case sensitive)
4) Empty string (or unrecognised) delivers all!!

For centered types, the output will be the centered, not the
primitive, cell.

Obviously, these two parameters could be made into input command
parameters. Values 5 cI would be obvious, but what is 10 15
supposed to mean?
*/

template<typename T>
G6 TryToGetAGoodProjection(const T& pt,
   const MatG6& projector, const int trials = 500) {
   G6 probe;
   probe.SetValid(false);
   int count = 0;
   while ((!LRL_Cell(probe).IsValid()) || (count > trials) || (!pt->IsMember(probe))) {
      const G6 start = G6::randDeloneReduced();
      Niggli::Reduce(start, probe);
      probe = projector * probe;
      Niggli::Reduce(probe, probe);
      ++count;
   }
   return probe;
}

G6 TryToGetAGoodProjection(const MatS6& projector, const int trials = 500) {
   S6 probe;
   probe.SetValid(false);
   int count = 0;
   while ((!LRL_Cell(probe).IsValid()) || (count > trials)) {
      const S6 start = S6::rand();
      Selling::Reduce(start, probe);
      probe = projector * probe;
      ++count;
   }
   //std::cout << "in TryToGetAGoodProjection, probe=  " << probe << std::endl;
   //std::cout << "in TryToGetAGoodProjection, probe=  " << G6(probe) << std::endl;
   return probe;
}

G6 Generate(const MatS6& pt) {
   const G6 probe = TryToGetAGoodProjection(pt, 50);

   //const G6 test = pt->GetToCenter() * probe;
   return probe;;
}

template<typename T>
G6 Generate(const T& pt) {
   const G6 probe = TryToGetAGoodProjection(pt, pt->GetPrj(), 50);

   //const G6 test = pt->GetToCenter() * probe;
   return probe;;
}

void ForNiggliInput(const int count,
   const std::vector<std::shared_ptr<GenerateNiggliBase> >& vglb) {
   for (size_t lat = 0; lat < vglb.size(); ++lat) {
      const std::shared_ptr<GenerateNiggliBase> pt = vglb[lat];
      //std::cout << "; lattice type = " << pt->GetITNumber() << std::endl;
      for (size_t i = 0; i < count; ++i) {
         const G6 g = Generate(vglb[lat]);
         std::cout << "G6 "
            << g << " "
            << " IT# = " << pt->GetITNumber() << "  "
            << pt->GetBravaisType()
            << std::endl;
      }
      //std::cout << ";" << std::endl;
   }

}

void ForDeloneInput(const int count,
   const std::vector<std::shared_ptr<GenerateDeloneBase> >& ptrDeloneBase) {
   for (size_t lat = 0; lat < ptrDeloneBase.size(); ++lat) {
      const std::shared_ptr<GenerateDeloneBase> pt = ptrDeloneBase[lat];
      //std::cout << "; lattice type = " << pt->GetName() << std::endl;
      for (size_t i = 0; i < count; ++i) {
         const G6 g = Generate(MatS6((*(ptrDeloneBase[lat])).GetPrj()));
         std::cout << "G6 "
            << g << " "
            << " Delone# = " << pt->GetName() << "  "
            << pt->GetBravaisType()
            << std::endl;
      }
      //std::cout << ";" << std::endl;
   }

}

void CreateCells(const int count, const std::string& namen) {
   std::string name(namen);
   ngen = count;
   const int number = getNumber<int>(name);
   const bool isNumIn_1_44 = isNumber<int>(name) && number >= 1 && number <= 44;
   const bool badNumber = isNumber<int>(name) && (number < 1 || number > 44);

   bool doNiggli = false;
   bool doDelone = false;
   bool doGruber = false;
   bool doPolar = false;


   if (LRL_StringTools::strToupper(name).find("ALL") != std::string::npos)
   {
      name.clear();
      doNiggli = doDelone = true;
   }

   if (LRL_StringTools::strToupper(name).find("RANDOM") != std::string::npos) {
      doNiggli = true;
      doDelone = false;
      doPolar = true;
   }

   if (LRL_StringTools::strToupper(name) == "GRUBER") doGruber = true;
   else if (count == 1) doNiggli = doDelone = true;
   //else if (argc > 2 && LRL_StringTools::strToupper(argv[2]) == "GRUBER") doGruber = true;
   else if (isNumIn_1_44) doNiggli = true;
   else if (LRL_StringTools::strToupper(name).find("NIGGLI") != std::string::npos) {
      doNiggli = true;
      doDelone = false;
      doPolar = false;
   }
   else if (LRL_StringTools::strToupper(name).find("DELONE") != std::string::npos) {
      doNiggli = false;
      doDelone = true;
      doPolar = false;
   }
   else if (isNumber<int>(name) && badNumber) doNiggli = doDelone = true;
   else if (isupper(name[0])) doDelone = true; // "A??" will incorrectly come here, but doNiggli already set
   else if (islower(name[0])) doNiggli = doDelone = true;
   else {
      const int i19191 = 19191;
   }
   if (badNumber) name.clear();
   if (isupper(name[0]) && CmdGenTypes::g_AllowedDelone.find(name[0]) == std::string::npos)
   {
      doNiggli = doDelone = true;
      name.clear();
   }

   if (doNiggli) {
      const std::vector<std::shared_ptr<GenerateNiggliBase> > NiggiTypes =
         GenerateNiggliBase().Select(name);
      std::cout << "; Niggli lattice types requested " << std::endl;
      ForNiggliInput(count, NiggiTypes);
   }

   if (doDelone) {
      std::vector<std::shared_ptr<GenerateDeloneBase> > DeloneTypes =
         GenerateDeloneBase().Select(name);
      std::cout << "; Delone lattice type requested " << std::endl;
      ForDeloneInput(count, DeloneTypes);
   }

   if (doGruber) {
      for (size_t i = 0; i < 1; ++i)
      {
         std::cout << "G6 " << G6(std::vector<double>{ 4., 16., 16., 16., 3., 4.   }) << std::endl;// Niggli reduced
         std::cout << "G6 " << G6(std::vector<double>{ 4., 16., 16., 16., 1., 4.   }) << std::endl;
         std::cout << "G6 " << G6(std::vector<double>{ 4., 16., 16., -16., -1., -3.}) << std::endl;
         std::cout << "G6 " << G6(std::vector<double>{ 4., 16., 16., -15., -1., -4.}) << std::endl;
         std::cout << "G6 " << G6(std::vector<double>{ 4., 16., 16., -13., -3., -4.}) << std::endl;
      }
   }

   if (doPolar) {
      Polar p;
      for (size_t i = 0; i < ngen; ++i)
      {
         std::cout << "P " << LRL_Cell_Degrees(p.rand()) << std::endl;
      }
   }
}

int main(int argc, char* argv[])
{
   //static std::string name = ""; // blank or unrecognized gives all types


   std::string input = R"(
count 3
type 33 A1 C1 C5 O3
; this is a comment
P 10 10 10 90 90 90
F 12 12 12 90 90 90
END
)";

   std::cout << "; generate unit cells\n";
   WebIO webio(argc, argv, "CmdGen", 0);

   CmdGenControls controls;

   // if there is web data, update controls before we read from the input file.
   if (argc == 1) {}
   else {
      if (argc > 1) {
         const std::string s(argv[1]);
         if (s[0] != '-')
            controls.setCount(std::stoi(argv[1]));
      }
      if (argc > 2) {
         for (size_t i = 2; i < argc; ++i) {
            const std::string s(argv[i]);
            if (s[0] != '-')
               controls.addType(s);
         }
      }
   }


   // Now create setup which will process input file and override previous values
   BasicProgramInput<CmdGenControls> dc_setup("CmdGen", controls);

   const std::vector<LatticeCell>& inputList = dc_setup.getInputList();

   std::istringstream inputStream(input);

   if (dc_setup.getControls().getShowControls()) {
      std::cout << dc_setup.getControls() << std::endl;
   }

   // ------------ treat input number to generate ----------------
   // Get the generation count from controls
   int numToGenerate = 1;  // default

   if (controls.getCount() > 0)
   {
      numToGenerate = int(controls.getCount());
   }
   else if (argc > 1 && std::stoul(argv[1]) > 0) {
      numToGenerate = std::stoi(argv[1]);
   }

   numToGenerate = std::max(numToGenerate, 1);

   //// ------------ treat input type names  ----------------

   std::vector<std::string> names;
   if (argc > 2) {
      for (size_t i = 2; i < argc; ++i) {
         names.emplace_back(std::string(argv[i]));
      }
   }

   // Get the type list
   std::vector<std::string> typeList = controls.getTypesAsVector();
   std::cout << "; Generate cells, ngen= " << numToGenerate << std::endl;

   if (!typeList.empty()) {
      names = typeList;
   }

   if (names.empty()) {
      names.emplace_back("ALL");
   }

   // echo any incoming vectors
   for (const auto& v : inputList) {
      std::cout << v.GetInput() << std::endl;
   }

   for (const std::string& namen : names)
   {
      CreateCells(numToGenerate, namen);
   }

   return 0;
}
