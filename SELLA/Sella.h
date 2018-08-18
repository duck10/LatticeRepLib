#ifndef SELLA_H
#define SELLA_H

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <map>
#include <string>
#include <vector>

#include "Delone.h"
#include "MatS6.h"
#include "S6.h"
#include "StoreResults.h"


class Sella {
public:

   //static const std::vector< std::pair<std::string, MatS6> > vDeloneTypes/* = Delone::LoadLabeledLatticeTypeProjectors();*/;

   Sella();
   double TestOneType(const std::string& label, const S6 &s6, const std::vector<MatS6>& vm);
   void TestAllTypes(const S6& s6);
   void TestPerps();
  

private:
   std::map<std::string, std::vector<S6> > themap;
   std::vector<std::pair<std::string, std::vector<MatS6> > > prjs;
   std::vector<std::pair<std::string, std::vector<MatS6> > > prps;
   StoreResults<std::string, S6> store;
   static std::vector< std::pair<std::string, MatS6> > vDeloneTypes;

   void ProcessItemStoreToVectorMap();
   void StoreAllReflections(const std::string& label, const S6& s1);
   void OneBound(const std::string& label, const S6& s1);
   void ProcessZeros(const std::string& label, const S6& s6);
   void ProcessVectorMapToPerpsAndProjectors();
   void Expand(const std::string& label, const MatS6& m);
   bool FindDuplicate(const std::vector<S6>& out, const S6 s6);

   static MatS6 ProjectorFromVector(const std::string& label, const S6& s);
   static S6 MakeSampleType(const MatS6& m);
   static unsigned long Index6(const unsigned long i, const unsigned long j);
   static double Fraction(const double d, const S6& s);
   static std::vector<unsigned long> FindS6Zeros(const S6& s);

};

#endif // SELLA_H



