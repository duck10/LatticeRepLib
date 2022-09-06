#ifndef SELLA_H
#define SELLA_H

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <map>
#include <string>
#include <vector>

#include "LabeledDeloneTypeMatrices.h"
#include "LabeledSellaMatrices.h"
#include "MatS6.h"
#include "S6.h"
#include "S6_Ordinals.h"
#include "StoreResults.h"

class SellaBuild {
public:

   SellaBuild();
   std::vector<LabeledDeloneTypeMatrices>  Build();
   double TestOneType(const std::string& label, const S6 &s6, const std::vector<MatS6>& vm);
   void TestAllTypes(const S6& s6);
   void TestPerps();
   void ShowIndexResults() const;
   const std::map<std::string, std::vector<S6_Ordinals> >& SellaBuildMap() { return themap; }

private:
   std::map<std::string, std::vector<S6_Ordinals> > themap;
   std::vector<LabeledSellaMatrices> projectors;
   std::vector<LabeledSellaMatrices> perps;
   StoreResults<std::string, S6_Ordinals> store;
   static std::vector< std::pair<std::string, MatS6> > vDeloneTypes;
   StoreResults<std::string, std::string> indexstore;

private:
   void ProcessItemStoreToVectorMap();
   void StoreAllReflections(const std::string& label, const S6_Ordinals& s);
   void OneBound(const std::string& label, const S6_Ordinals& s1, const size_t zeroBound );
   void ProcessZeros(const std::string& label, const S6_Ordinals& s6 );
   void Expand(const std::string& label, const MatS6& m, MatS6 transformations);
   void Expand(const std::string& label, const S6& sample);
   void ProcessTwoZeros(const std::string& label, const S6& s6, const std::vector<size_t>& vZeros);
   void ProcessThreeZeros(const std::string& label, const S6& s6, const std::vector<size_t>& vZeros);

   static bool FindDuplicate(const std::vector<S6_Ordinals>& out, const S6_Ordinals s6);
   static S6 MakeSampleType(const MatS6& m);
   static std::vector<size_t> FindS6Zeros(const S6& s);
};

#endif // SELLA_H



