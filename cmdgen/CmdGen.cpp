
#include <iostream>
#include <map>

#include "G6.h"
#include "LRL_Cell.h"
#include "Niggli.h"
#include "GenerateLatticeTypeExamples.h"

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
static std::string name = ""; // blank or unrecognized gives all types

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
      std::cout << "; lattice type = " << pt->GetITNumber() << std::endl;
      for (size_t i = 0; i < ngen; ++i) {
         const G6 g = Generate(vglb[lat]);
         std::cout << "G6 "
            << g << " "
            << " IT# = " << pt->GetITNumber() << "  "
            << pt->GetBravaisType()
            << std::endl;
      }
      std::cout << ";" << std::endl;
   }

}

void ForDeloneInput(
   const std::vector<std::shared_ptr<GenerateDeloneBase> >& vglb) {
   for (size_t lat = 0; lat < vglb.size(); ++lat) {
      const std::shared_ptr<GenerateDeloneBase> pt = vglb[lat];
      std::cout << "; lattice type = " << pt->GetName() << std::endl;
      for (size_t i = 0; i < ngen; ++i) {
         const G6 g = Generate(vglb[lat]);
         std::cout << "G6 "
            << g << " "
            << " IT# = " << pt->GetName() << "  "
            << pt->GetBravaisType()
            << std::endl;
      }
      std::cout << ";" << std::endl;
   }

}

static const std::string g_DeloneTypes("hR hP oS oC mS mC mS A1 A2 A3 C1 C3 C5 H4 M1A M1B M2A M2B M3 M4 O1A O1B O2 O3 O4 O5 R1 R3 T1 T2 T5");
static const std::string g_LatticeTypes("aP cF cI cP hP mC mP oF oI oP oS rP tI tP ");

int main(int argc, char* argv[])
{
   int test = 0;
   if (argc > 1) {
      test = atoi(argv[1]);
      if (test != 0) ngen = test;
      if (argc > 2) {
         const std::string strtest = argv[2];
         name = strtest;
         //const std::string translated = TranslateDeloneToIT(name);
         //if (translated != "") name = translated;
      }
   }

   if (name.length() > 1 && g_DeloneTypes.find(name) != std::string::npos) {
      std::vector<std::shared_ptr<GenerateDeloneBase> > DeloneTypes =
         GenerateDeloneBase().Select(name);
      std::cout << "Delone input " << std::endl;
      ForDeloneInput(DeloneTypes);
   }
   else if ((g_LatticeTypes.find(name) != std::string::npos) && (name != "") && (test == 0)) {
      std::vector<std::shared_ptr<GenerateDeloneBase> > DeloneTypes =
         GenerateDeloneBase().Select(name);
      std::cout << "Delone input " << std::endl;
      ForDeloneInput(DeloneTypes);
   }
   else {
      std::vector<std::shared_ptr<GenerateNiggliBase> > NiggiTypes =
         GenerateNiggliBase().Select(name);
      std::cout << "Niggli input " << std::endl;
      ForNiggliInput(NiggiTypes);
   }



   //std::cout << "; generate examples  ngen = " << ngen << "  lattice type selection = """ << name << std::endl;
   //const std::vector<std::shared_ptr<GenerateNiggliBase> >
   //   vglb = GenerateNiggliBase().Select(name);

   //for (size_t lat = 0; lat < vglb.size(); ++lat) {
   //   const std::shared_ptr<GenerateNiggliBase> pt = vglb[lat];
   //   std::cout << "; lattice type = " << pt->GetITNumber() << std::endl;
   //   for (size_t i = 0; i < ngen; ++i) {
   //      const G6 g = Generate(vglb[lat]);
   //      std::cout << "G6 "
   //         << g << " "
   //         << " IT# = " << pt->GetITNumber() << "  "
   //         << pt->GetBravaisType()
   //         << std::endl;
   //   }
   //   std::cout << ";" << std::endl;
   //}
}
