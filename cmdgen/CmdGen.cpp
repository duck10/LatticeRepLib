
#include <iostream>

#include"LatticeConverter.h"
#include "LRL_ReadLatticeData.h"
#include "G6.h"
#include "Niggli.h"
#include "GenerateLatticeTypeExamples.h"

static const int ngen = 1;
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
   //std::cout << "probe in TryToGetAGoodProjection "
   //   << probe << std::endl
   //   << LRL_Cell_Degrees(probe) << std::endl << std::endl;
   return probe;
}

G6 Generate(const std::shared_ptr<GenerateNiggliBase>& pt) {
   const G6 probe = TryToGetAGoodProjection(pt, pt->GetPrj(), 50);

   const G6 test = pt->GetToCenter() * probe;
   //std::cout
   //   << pt->GetBravaisType() << " "
   //   << pt->GetITNum() << " "
   //   << LRL_Cell_Degrees(test)
   //   << std::endl;
   return probe;;
}

int main()
{
   std::cout << "; generate examples  ngen = " << ngen << "  lattice type selection = """ << name << std::endl;
   const std::vector<std::shared_ptr<GenerateNiggliBase> >
      vglb = GenerateNiggliBase().Select(name);
   for (size_t lat = 0; lat < vglb.size(); ++lat) {
      const std::shared_ptr<GenerateNiggliBase> pt = vglb[lat];
      std::cout << "; lattice type = " << pt->GetITNumber() << std::endl;
      for (size_t i = 0; i < ngen; ++i) {
         const G6 g = Generate(vglb[lat]);
         std::cout << "G6 "
            << (g) << " "
            << " IT# = " << pt->GetITNumber() << "  "
            << pt->GetBravaisType()
            << std::endl;
      }
      std::cout << ";" << std::endl;
   }
}