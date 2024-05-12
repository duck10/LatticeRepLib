#include <ctype.h>
#include <iostream>
#include <map>

#include "G6.h"
#include "LRL_Cell.h"
#include "Niggli.h"
#include "NumericTools.h"
#include "GenerateLatticeTypeExamples.h"
#include "Polar.h"
#include "S6BoundaryTransforms.h"
#include "LRL_StringTools.h"
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

G6 TryToGetAGoodProjection( const MatS6& projector, const int trials = 500) {
   S6 probe;
   probe.SetValid(false);
   int count = 0;
   while ((!LRL_Cell(probe).IsValid()) || (count > trials) ) {
      const S6 start = S6::rand();
      Selling::Reduce(start, probe);
      probe = projector * probe;
      ++count;
   }
   //std::cout << "in TryToGetAGoodProjection, probe=  " << probe << std::endl;
   //std::cout << "in TryToGetAGoodProjection, probe=  " << G6(probe) << std::endl;
   return probe;
}


static const std::string g_AllowedDelone("CTROMAH");
static const std::string g_DeloneTypes("hR hP oS oC mS mC mS A1 A2 A3 C1 C3 C5 H4 M1A M1B M2A M2B M3 M4 O1A O1B O2 O3 O4 O5 R1 R3 T1 T2 T5");
static const std::string g_LatticeTypes("aP cF cI cP hP mC mP oF oI oP oS rP tI tP ");
static const std::string g_Complex("X x");

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

std::string TranslateDeloneToIT(const std::string& s) {

   std::map<std::string, std::string> m;
   m.insert(std::make_pair("C1", "cI"));
   m.insert(std::make_pair("C3", "cF"));
   m.insert(std::make_pair("C5", "cP"));
   m.insert(std::make_pair("T1", "tI"));
   m.insert(std::make_pair("T2", "tI"));
   m.insert(std::make_pair("T5", "tP"));
   m.insert(std::make_pair("R1", "hR"));
   m.insert(std::make_pair("R3", "hR"));
   m.insert(std::make_pair("O1A", "oF"));
   m.insert(std::make_pair("O2", "oI"));
   m.insert(std::make_pair("O3", "oI"));
   m.insert(std::make_pair("O4", "oS"));
   //m.insert(std::make_pair("O4B", "oS"));
   m.insert(std::make_pair("O5", "oP"));
   m.insert(std::make_pair("O1B", "oI"));
   m.insert(std::make_pair("M1A", "mC"));
   m.insert(std::make_pair("M1B", "mC"));
   m.insert(std::make_pair("M2", "mC"));
   m.insert(std::make_pair("M3", "mC"));
   m.insert(std::make_pair("M4", "mP"));
   m.insert(std::make_pair("M2B", "mC"));
   m.insert(std::make_pair("H4", "hP"));   std::string out{ "" };
   if (m.find(s) != m.end()) out = m[s];
   return out;
}

void ForNiggliInput(
   const std::vector<std::shared_ptr<GenerateNiggliBase> >& vglb) {
   for (size_t lat = 0; lat < vglb.size(); ++lat) {
      const std::shared_ptr<GenerateNiggliBase> pt = vglb[lat];
      //std::cout << "; lattice type = " << pt->GetITNumber() << std::endl;
      for (size_t i = 0; i < ngen; ++i) {
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

void ForDeloneInput(
   const std::vector<std::shared_ptr<GenerateDeloneBase> >& ptrDeloneBase) {
   for (size_t lat = 0; lat < ptrDeloneBase.size(); ++lat) {
      const std::shared_ptr<GenerateDeloneBase> pt = ptrDeloneBase[lat];
      //std::cout << "; lattice type = " << pt->GetName() << std::endl;
      for (size_t i = 0; i < ngen; ++i) {
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

void CreateCells(const int argc, const std::string& namen) {
   std::string name(namen);
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


   if (argc == 1) doNiggli = doDelone = true;
   else if (argc > 1 && LRL_StringTools::strToupper(name) == "GRUBER") doGruber = true;
   //else if (argc > 2 && LRL_StringTools::strToupper(argv[2]) == "GRUBER") doGruber = true;
   else if (argc <= 2) doNiggli = doDelone = true;
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
   else if (LRL_StringTools::strToupper(name).find("RANDOM") != std::string::npos) {
      doNiggli = false;
      doDelone = false;
      doPolar = true;
   }
   else if (isNumber<int>(name) && badNumber) doNiggli = doDelone = true;
   else if (isupper(name[0])) doDelone = true; // "A??" will incorrectly come here, but doNiggli already set
   else if (islower(name[0])) doNiggli = doDelone = true;
   else {
      const int i19191 = 19191;
   }
   if (badNumber) name.clear();
   if (isupper(name[0]) && g_AllowedDelone.find(name[0]) == std::string::npos)
   {
      doNiggli = doDelone = true;
      name.clear();
   }


   if (doNiggli) {
      const std::vector<std::shared_ptr<GenerateNiggliBase> > NiggiTypes =
         GenerateNiggliBase().Select(name);
      std::cout << "; Niggli lattice types requested " << std::endl;
      ForNiggliInput(NiggiTypes);
   }

   if (doDelone) {
      std::vector<std::shared_ptr<GenerateDeloneBase> > DeloneTypes =
         GenerateDeloneBase().Select(name);
      std::cout << "; Delone lattice type requested " << std::endl;
      ForDeloneInput(DeloneTypes);
   }

   if (doGruber) {
      for (size_t i = 0; i < ngen; ++i)
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
   static std::string name = ""; // blank or unrecognized gives all types

   int test = 0;
   if (argc > 1) {
      test = atoi(argv[1]);
      if (test != 0) ngen = test;
      if (argc > 2) {
         const std::string strtest = argv[2];
         name = strtest;
      }
   }

   std::cout << "; Generate cells, ngen= " << ngen << std::endl;

   std::vector<std::string> names;
   if (argc > 2) {
      for (size_t i = 2; i < argc; ++i) {
         names.emplace_back(std::string(argv[i]));
      }
   }

   if (names.empty()) {
      names.emplace_back("ALL");
   }

   for ( const std::string& namen: names)
   {
      CreateCells(argc, namen);
   }

}
