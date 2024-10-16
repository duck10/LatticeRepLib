#ifndef SELLA_H
#define SELLA_H

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <map>
#include <string>
#include <vector>

#include "Delone.h"
#include "EnhancedS6.h"
#include "LabeledSellaMatrices.h"
#include "MatS6.h"
//#include "S6.h"
//#include "S6_Ordinals.h"
#include "StoreResults.h"

class SellaBuild {
public:

   SellaBuild();
   void Build();
   static std::vector<unsigned long> FindS6Zeros(const EnhancedS6& s);
   //double TestOneType(const std::string& label, const S6 &s6, const std::vector<MatS6>& vm);
   //void TestAllTypes(const S6& s6);
   //void TestPerps();
   //std::vector<std::pair<std::string, double> > GetVectorOfFits(const S6& s6);
   //void ShowIndexResults() const;
   //size_t size(void) const { return themap.size(); }
   //size_t size(void) const { return xfmap.size(); }

private:
   //std::map<std::string, std::vector<S6_Ordinals> > themap;
   std::map<std::string, std::vector<EnhancedS6>> xfmap;
   //std::vector<LabeledSellaMatrices> projectors;
   //std::vector<LabeledSellaMatrices> perps;
   //StoreResults<std::string, S6_Ordinals> store;
   StoreResults<std::string, EnhancedS6> storeEN;
   static std::vector< std::pair<std::string, MatS6> > vDeloneTypes;
   //StoreResults<std::string, std::string> indexstore;

private:
   void ProcessItemStoreToVectorMap();
   //void StoreAllReflections(const std::string& label, const S6_Ordinals& s1, const MatS6& transformations);
   void StoreAllReflections(const std::string& label, const EnhancedS6& s1);
   static std::vector<EnhancedS6> CreateAllReflections(const std::string& label, const EnhancedS6& s1);
   //void OneBound(const std::string& label, const S6_Ordinals& s1, MatS6 transformations);
   void OneBound(const std::string& label, const EnhancedS6& s1);
   //void ProcessZeros(const std::string& label, const S6_Ordinals& s6, MatS6 transformations);
   void ProcessZeros(const std::string& label, const EnhancedS6& s6);
   void ProcessTwoZeros(const std::string& label, const EnhancedS6& s6);
   void ProcessTwoZeros(const std::string& label, const std::vector<EnhancedS6>& vs);
   void ProcessThreeZeros(const std::string& label, const EnhancedS6& s6);
   void Expand(const std::string& label, const MatS6& m);

   //static bool FindDuplicate(const std::vector<S6_Ordinals>& out, const S6_Ordinals s6);
   static bool FindDuplicate(const std::vector<EnhancedS6>& out, const EnhancedS6 s6);
   static EnhancedS6 MakeSampleType(const MatS6& m);
   //static std::vector<unsigned long> FindS6Zeros(const S6& s);
   void ProcessMultipleZeros(const std::string& label, const EnhancedS6& s6);
};

#endif // SELLA_H



