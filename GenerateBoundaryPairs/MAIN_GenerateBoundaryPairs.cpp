// GenerateBoundaryPairs.cpp : Defines the entry point for the console application.
//

#include <algorithm>
#include <cfloat>
#include <iomanip>
#include <iostream>
#include <utility>

#include "Delone.h"
#include "GeneratePairsOfTestData.h"
#include "GenerateRandomLattice.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_ToString.h"
#include "PairReporter.h"
#include "S6.h"
#include "StoreResults.h"

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
   GeneratePairsOfTestData::MultiGeneratePair_Reduced_Other_ReducesFarAway();
   GeneratePairsOfTestData::GenerateRandomLatticesAndCatalogByReductionSteps();
   GeneratePairsOfTestData::GenerateLatticeTypeExamples(1200);
   TestLatticeTypes();
   return 0;
}
