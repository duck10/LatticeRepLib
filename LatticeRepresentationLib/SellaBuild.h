#ifndef SELLA_H
#define SELLA_H

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "LabeledDeloneTypeMatrices.h"
#include "LabeledSellaMatrices.h"
#include "MatS6.h"
#include "S6.h"
#include "S6_Ordinals.h"
#include "StoreResults.h"
#include "Transformations.h"

class SellaBuild {
public:

   SellaBuild();
   std::vector<LabeledDeloneTypeMatrices>  Build();
   double TestOneType(const std::string& label, const S6 &s6, const std::vector<MatS6>& vm);
   void TestAllTypes(const S6& s6);
   void TestPerps();
   void ShowIndexResults() const;

private:
   std::map<std::string, std::vector<S6> > themap;  // no longer used
   std::map<std::string, std::vector<Transformations> > xfmap;
   std::vector<LabeledSellaMatrices> projectors;
   std::vector<LabeledSellaMatrices> perps;
   StoreResults<std::string, S6> store;
   StoreResults<std::string, Transformations> xfStore;
   static std::vector< std::pair<std::string, MatS6> > vDeloneTypes;
   StoreResults<std::string, std::string> indexstore;

public:
   std::vector<size_t> DetermineToProcess(const std::set<size_t>& exclusions,
      const S6& zeros);
   std::vector<size_t> DetermineToProcess(const S6& zeros,
      const std::set<size_t>& exclusions) {
      return DetermineToProcess(exclusions, zeros); }
   void ProcessItemStoreToVectorMap();
   void Store(const std::string& label, const S6_Ordinals& s);
   void Store(const std::string& label, const std::set<S6_Ordinals>& s);
   void StoreAllReflections(const std::string& label, const S6_Ordinals& s);
   void StoreAllReflections(const std::string& label, const S6& s);
   void StoreAllReflections(const std::string& label, const std::set<S6_Ordinals>& s);
   void StoreAllReflections(const std::string& label, const std::set<S6>& s);
   std::set<S6> GenerateAllReflections(const S6& s);
   std::set<S6> GenerateAllReflections(const std::set<S6_Ordinals>& s);
   std::set<S6> GenerateAllReflections(const std::set<S6>& s);
   void OneBound(const std::string& label, const S6_Ordinals& s1, const size_t zeroBound );
   void ProcessZeros(const std::string& label, const S6_Ordinals& s6 );
   void Expand(const std::string& label, const MatS6& fromCanon/*, MatS6 transformations*/);
   void Expand2(const std::string& label, const MatS6& fromCanon);
   //void Expand(const std::set<size_t>& exclusions, const std::string& label, const S6& sample);
   void ProcessTwoZeros(const std::string& label, const S6& s6);
   void ProcessThreeZeros(const std::string& label, const S6& s6);
   std::set<S6> BoundAndRefl(const size_t n, const S6& s);

   static bool FindDuplicate(const std::vector<S6>& out, const S6& s6);
   static S6 MakeSampleType(const MatS6& m);
   static std::vector<size_t> FindS6Zeros(const S6& s);
   static std::vector<size_t> FindNearS6Zeros(const S6& s);

   std::set<S6> Xpand1(const std::string& label, const std::set<S6>& vs, const MatS6& toCanon);
   std::set<S6> Xpand2(const std::string& label, const std::set<S6>& vs, const MatS6& toCanon);
   std::set<S6> Xpand3(const std::string& label, const std::set<S6>& vs, const MatS6& toCanon);
   std::set<S6> Xpand1(const std::string& label, const S6& vs, const MatS6& toCanon);
   std::set<S6> Xpand2(const std::string& label, const S6& vs, const MatS6& toCanon);
   std::set<S6> Xpand3(const std::string& label, const S6& vs, const MatS6& toCanon);
};

#endif // SELLA_H



