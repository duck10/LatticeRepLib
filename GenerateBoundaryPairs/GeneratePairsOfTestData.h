#ifndef GENERATEPAIRSOFTESTDATA_H
#define GENERATEPAIRSOFTESTDATA_H

#include <algorithm>
#include <utility>
#include <vector>

#include "S6.h"
#include "StoreResults.h"

class GeneratePairsOfTestData {
public:
   static std::pair<S6, S6> ScalePairToSpecifedSeparation(const S6& s1, const S6& s2, const double factor);
   static void GenerateRandomLatticesAndCatalogByReductionStepCount(const unsigned long n);
   static std::pair<S6, S6> GenerateLatticeTypeExamplesNearDeloneTypes(const unsigned long n);
   static S6 DoRandomReflection(const S6& s);


   static void MultiGeneratePair_Reduced_Other_ReducesFarAway(const unsigned long targetReducedUnreducedPairCount);
   static double PositiveDistance(const S6& s6);
   static std::string FormatPairOfTestS6Vectors(const S6& s1, const S6& s2, const std::string& id);
private:
   static bool IsGoodLatticeTypeForPerturbation(const unsigned long n);
   static std::pair<S6, S6> OneLatticeType(const MatS6& m);
   static std::pair<S6, S6> OneLatticeAttempt(const MatS6& m);
   static std::pair<S6, S6> GeneratePair_Reduced_Other_ReducesFarAway(const unsigned long numberOfZeros);
   static S6 DoReflection(const S6& s, const unsigned long n);
   static std::pair<S6, S6> BinarySearchReduced(const S6& s1, const S6& s2, const int npass);
   static S6 GenerateStartingPoint(const MatS6& m);
   static int Rank(const S6& s);
   static S6 Perturb(const S6& s, const double fraction);
   static void StoreOnePointIfValid(StoreResults<unsigned long, std::string >& store, const S6& s6in);
};

#endif  //  GENERATEPAIRSOFTESTDATA_H