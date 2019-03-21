#ifndef SELLA_H
#define SELLA_H

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <map>
#include <string>
#include <vector>

#include "Delone.h"
#include "LabeledSellaMatrices.h"
#include "MatS6.h"
#include "S6.h"
#include "S6_Ordinals.h"
#include "StoreResults.h"

class SellaBuild {
public:

   SellaBuild();
   void Build();
   double TestOneType(const std::string& label, const S6 &s6, const std::vector<MatS6>& vm);
   void TestAllTypes(const S6& s6);
   void TestPerps();
   std::vector<std::pair<std::string, double> > GetVectorOfFits(const S6& s6);
   void ShowIndexResults() const;

private:
   std::map<std::string, std::vector<S6_Ordinals> > themap;
   std::vector<LabeledSellaMatrices> projectors;
   std::vector<LabeledSellaMatrices> perps;
   StoreResults<std::string, S6_Ordinals> store;
   static std::vector< std::pair<std::string, MatS6> > vDeloneTypes;
   StoreResults<std::string, std::string> indexstore;

private:
   void ProcessItemStoreToVectorMap();
   void StoreAllReflections(const std::string& label, const S6_Ordinals& s1, const MatS6& transformations);
   void OneBound(const std::string& label, const S6_Ordinals& s1, MatS6 transformations);
   void ProcessZeros(const std::string& label, const S6_Ordinals& s6, MatS6 transformations);
   void Expand(const std::string& label, const MatS6& m, MatS6 transformations);

   static bool FindDuplicate(const std::vector<S6_Ordinals>& out, const S6_Ordinals s6);
   static S6 MakeSampleType(const MatS6& m);
   static std::vector<unsigned long> FindS6Zeros(const S6& s);
};

#endif // SELLA_H



