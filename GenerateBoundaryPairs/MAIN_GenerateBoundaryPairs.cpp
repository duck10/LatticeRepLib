// GenerateBoundaryPairs.cpp : Defines the entry point for the console application.
//


#include "Delone.h"
#include "GeneratePairsOfTestData.h"
#include "GenerateRandomLattice.h"
#include "LRL_Cell_Degrees.h"
#include "S6.h"


//////////////////////////TEMP
#include "StoreResults.h"
#include <string>
#include "LRL_ToString.h"
#include <algorithm>
#include <utility>

static int seed = 2;

void TestLatticeTypes() {
   static int seed = 1;
   static const std::vector<MatS6> vDeloneTypes = Delone::LoadLatticeTypeProjectors();
   GenerateRandomLattice<S6> generator(seed);
   generator.randSellingReduced();  // try to get a random lattice that gives valid unit cells for all 
   const S6 temp = generator.randSellingReduced();

   for (unsigned long i = 0; i < vDeloneTypes.size(); ++i) {
      const S6 test = vDeloneTypes[i] * temp;
      const bool b = LRL_Cell_Degrees(test).GetValid();
      std::cout << LRL_Cell_Degrees(test) << std::endl;
   }
}

void GeneratePairSamplesByHighReductionCycleCount(const unsigned long n) {
   GenerateRandomLattice<S6> grl(19191);
   StoreResults<unsigned long, std::string > store(4);
   store.SetKeyLabel("cycles");
   S6 s6in, out;
   bool b;

   for (unsigned long i = 0; i < n; ++i) {
      s6in = grl.GenerateExtreme();
      if (s6in.GetValid()) {
         b = Selling::Reduce(s6in, out);
         const unsigned long ncycles = Selling::GetCycles();
         const std::string label = LRL_ToString(ncycles)+ " Ex";
         store.Store(Selling::GetCycles(), GeneratePairsOfTestData::FormatPairOfTestS6Vectors(s6in, out, label));
      }

      s6in = grl.randSellingUnreduced();
      LRL_Cell cell = LRL_Cell(s6in);
      if (cell.GetValid()) {
         double test = std::abs(39.4918 - s6in[0]);
         b = Selling::Reduce(s6in, out);
         const unsigned long ncycles = Selling::GetCycles();
         const std::string label = LRL_ToString(ncycles) + " Re";
         store.Store(Selling::GetCycles(), GeneratePairsOfTestData::FormatPairOfTestS6Vectors(s6in, out, label));
      }
   }

   std::vector<unsigned long> v = store.GetKeys();
   std::sort(v.begin(), v.end());
   std::reverse(v.begin(), v.end());

   const unsigned long nToProcess = v.size() / 3;

   for (unsigned long i = 0; i < nToProcess; ++i) {
      const std::vector<std::pair<unsigned long, std::string > > itemdata = store.FindResult(v[i]);
      for ( unsigned long k=0; k<itemdata.size(); ++k ) {
         std::cout << itemdata[k].second << std::endl;
      }
   }
}

int main()
{
   GeneratePairSamplesByHighReductionCycleCount(100000);

   exit(0);
   const unsigned long targetReducedUnreducedPairCount = 2000;
   GeneratePairsOfTestData::MultiGeneratePair_Reduced_Other_ReducesFarAway(targetReducedUnreducedPairCount);
   const unsigned long targetRandomCount = 10000;
   GeneratePairsOfTestData::GenerateRandomLatticesAndCatalogByReductionStepCount(targetRandomCount);
   const unsigned long targetNearDeloneType = 1200;
   GeneratePairsOfTestData::GenerateLatticeTypeExamplesNearDeloneTypes(targetNearDeloneType);
   TestLatticeTypes();
   return 0;
}
