
#include <iostream>

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
static std::string name = "cI"; // blank or unrecognized gives all types


G6 TryToGetAGoodProjection(const std::shared_ptr<GenerateNiggliBase>& pt,
   const MatG6& projector, const int trials = 500) {
   G6 probe;
   probe.SetValid(false);
   int count = 0;
   while ((!LRL_Cell(probe).IsValid()) ||( count > trials) || (!pt->IsMember(probe))) {
      const G6 start = G6::randDeloneReduced();
      Niggli::Reduce(start, probe);
      probe = projector * probe;
      Niggli::Reduce(probe, probe);
      ++count;
   }
   return probe;
}

G6 Generate(const std::shared_ptr<GenerateNiggliBase>& pt) {
   const G6 probe = TryToGetAGoodProjection(pt, pt->GetPrj(), 50);

   const G6 test = pt->GetToCenter() * probe;
   return probe;;
}

int main(int argc, char* argv[])
{
   if (argc > 1) {
      const int test = atoi(argv[1]);
      if (test != 0) ngen = test;
      if (argc > 2) {
         const std::string strtest = argv[2];
         name = strtest;
      }
   }

   std::cout << "; generate examples  ngen = " << ngen << "  lattice type selection = """ << name << std::endl;
   const std::vector<std::shared_ptr<GenerateNiggliBase> >
      vglb = GenerateNiggliBase().Select(name);
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
