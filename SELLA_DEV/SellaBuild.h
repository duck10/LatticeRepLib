#ifndef SELLABUILD_H
#define SELLABUILD_H

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "EnhancedS6.h"
#include "VCPVector.h"
#include "LabeledDeloneTypeMatrices.h"
#include "LabeledSellaMatrices.h"
#include "MatS6.h"
#include "StoreResults.h"

class SellaBuild {
public:
   SellaBuild();
   std::vector<LabeledDeloneTypeMatrices> Build();
   double TestOneType(const std::string& label, const EnhancedS6& s6, const std::vector<MatS6>& vm);
   void TestAllTypes(const EnhancedS6& s6);
   void TestPerps();
   void ShowIndexResults() const;
private:
   std::map<std::string, std::vector<EnhancedS6>> xfmap;
   bool FindDuplicate(const std::vector<EnhancedS6>& out, const EnhancedS6& s6);
   std::vector<LabeledSellaMatrices> projectors;
   std::vector<LabeledSellaMatrices> perps;
   StoreResults<std::string, EnhancedS6> store;
   StoreResults<std::string, Transformations> xfStore;
   static std::vector<std::pair<std::string, MatS6>> vDeloneTypes;
   StoreResults<std::string, std::string> indexstore;
   VCPVector vcpVector;

public:
   std::vector<size_t> DetermineToProcess(const std::set<size_t>& exclusions,
      const EnhancedS6& zeros);
   std::vector<size_t> DetermineToProcess(const EnhancedS6& zeros,
      const std::set<size_t>& exclusions) {
      return DetermineToProcess(exclusions, zeros);
   }
   void ProcessItemStoreToVectorMap();
   void StoreAllReflections(const std::string& label, const EnhancedS6& s);
   void StoreAllReflections(const std::string& label, const std::set<EnhancedS6>& s);
   static std::set<EnhancedS6> GenerateAllReflections(const EnhancedS6& s);
   static std::set<EnhancedS6> GenerateAllReflections(const std::set<EnhancedS6>& s);
   void OneBound(const std::string& label, const EnhancedS6& s1, const size_t zeroBound);
   void ProcessZeros(const std::string& label, const EnhancedS6& s6);
   void Expand(const std::string& label, const MatS6& fromCanon);
   void Expand2(const std::string& label, const MatS6& fromCanon);
   void ProcessTwoZeros(const std::string& label, const EnhancedS6& s6);
   void ProcessThreeZeros(const std::string& label, const EnhancedS6& s6);
   std::set<EnhancedS6> BoundAndRefl(const size_t n, const EnhancedS6& s);

   static bool FindDuplicate(const std::vector<Transformations>& out, const EnhancedS6& s6);
   static EnhancedS6 MakeSampleType(const MatS6& m);
   static std::vector<size_t> FindS6Zeros(const EnhancedS6& s);
   static std::vector<size_t> FindNearS6Zeros(const EnhancedS6& s);

   std::set<EnhancedS6> Xpand1(const std::string& label, const std::set<EnhancedS6>& vs, const MatS6& toCanon);
   std::set<EnhancedS6> Xpand2(const std::string& label, const std::set<EnhancedS6>& vs, const MatS6& toCanon);
   std::set<EnhancedS6> Xpand_A(const std::string& label, const std::set<EnhancedS6>& vs, const MatS6& toCanon);
   std::set<EnhancedS6> Xpand1(const std::string& label, const EnhancedS6& vs, const MatS6& toCanon);
   std::set<EnhancedS6> Xpand2(const std::string& label, const EnhancedS6& vs, const MatS6& toCanon);
   std::set<EnhancedS6> Xpand_A(const std::string& label, const EnhancedS6& vs, const MatS6& toCanon);

   //static EnhancedS6 Rounder(const EnhancedS6& s6);
   //static std::set<EnhancedS6> Rounder(const std::set<EnhancedS6>& ss);
   static EnhancedS6 SetPos(const size_t n, const EnhancedS6& s6);
   static std::set<EnhancedS6> SetPos(const size_t n, const std::set<EnhancedS6>& s6set);
   static EnhancedS6 SetPos(const size_t n1, const size_t n2, const EnhancedS6& s6);
   static std::vector<EnhancedS6(*)(const EnhancedS6&)> reflectionFunctions;
};

#endif // SELLABUILD_H
