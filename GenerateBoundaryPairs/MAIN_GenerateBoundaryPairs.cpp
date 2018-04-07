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

int main()
{
   GeneratePairsOfTestData::GeneratePairSamplesByHighReductionCycleCount(100000);

   const unsigned long targetReducedUnreducedPairCount = 2000;
   GeneratePairsOfTestData::MultiGeneratePair_Reduced_Other_ReducesFarAway(targetReducedUnreducedPairCount);
 
   const unsigned long targetRandomCount = 10000;
   GeneratePairsOfTestData::GenerateRandomLatticesAndCatalogByReductionStepCount(targetRandomCount);

   const unsigned long targetNearDeloneType = 1200;
   GeneratePairsOfTestData::GenerateLatticeTypeExamplesNearDeloneTypes(targetNearDeloneType);

   TestLatticeTypes();
   return 0;
}
