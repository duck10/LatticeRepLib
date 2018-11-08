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


class LabeledSellaMatrices {
public:
   LabeledSellaMatrices()
   {}
   LabeledSellaMatrices(const std::string& label, const std::vector<MatS6>& vm) : m_label(label), m_vector(vm)
   {}

   std::string GetLabel() const { return m_label; }

   std::vector<MatS6>  GetMatrices() const { return m_vector; }
   size_t size() const { return m_vector.size(); }
private:
   std::string m_label;
   std::vector<MatS6> m_vector;
};

class Sella {
public:

   Sella();
   double TestOneType(const std::string& label, const S6 &s6, const std::vector<MatS6>& vm);
   void TestAllTypes(const S6& s6);
   void TestPerps();
   std::vector<std::pair<std::string, double> > GetVectorOfFits(const S6& s6);
   void ShowIndexResults() const;
   void WriteSellaMatrices(const std::string& functinoName, const std::vector<LabeledSellaMatrices>& mat) const;

   std::vector<LabeledSellaMatrices> GetPerps( void ) const {
      return perps;
   }

   std::vector<LabeledSellaMatrices> GetProjectors( void) const {
      return projectors;
   }

   static std::vector<LabeledSellaMatrices> CreateAllPerps();
   static LabeledSellaMatrices CreatePerps_H4();
   static LabeledSellaMatrices CreatePerps_K1();
   static LabeledSellaMatrices CreatePerps_K3();
   static LabeledSellaMatrices CreatePerps_K5();
   static LabeledSellaMatrices CreatePerps_M1A();
   static LabeledSellaMatrices CreatePerps_M1B();
   static LabeledSellaMatrices CreatePerps_M2A();
   static LabeledSellaMatrices CreatePerps_M2B();
   static LabeledSellaMatrices CreatePerps_M3();
   static LabeledSellaMatrices CreatePerps_M4();
   static LabeledSellaMatrices CreatePerps_O1A();
   static LabeledSellaMatrices CreatePerps_O1B();
   static LabeledSellaMatrices CreatePerps_O2();
   static LabeledSellaMatrices CreatePerps_O3();
   static LabeledSellaMatrices CreatePerps_O4();
   static LabeledSellaMatrices CreatePerps_O5();
   static LabeledSellaMatrices CreatePerps_T1();
   static LabeledSellaMatrices CreatePerps_T2();
   static LabeledSellaMatrices CreatePerps_T5();
   static LabeledSellaMatrices CreatePerps_R1();
   static LabeledSellaMatrices CreatePerps_R3();

   static std::vector<LabeledSellaMatrices> CreateAllPrjs();
   static LabeledSellaMatrices CreatePrjs_H4();
   static LabeledSellaMatrices CreatePrjs_C1();
   static LabeledSellaMatrices CreatePrjs_C3();
   static LabeledSellaMatrices CreatePrjs_C5();
   static LabeledSellaMatrices CreatePrjs_M1A();
   static LabeledSellaMatrices CreatePrjs_M1B();
   static LabeledSellaMatrices CreatePrjs_M2A();
   static LabeledSellaMatrices CreatePrjs_M2B();
   static LabeledSellaMatrices CreatePrjs_M3();
   static LabeledSellaMatrices CreatePrjs_M4();
   static LabeledSellaMatrices CreatePrjs_O1A();
   static LabeledSellaMatrices CreatePrjs_O1B();
   static LabeledSellaMatrices CreatePrjs_O2();
   static LabeledSellaMatrices CreatePrjs_O3();
   static LabeledSellaMatrices CreatePrjs_O4();
   static LabeledSellaMatrices CreatePrjs_O5();
   static LabeledSellaMatrices CreatePrjs_T1();
   static LabeledSellaMatrices CreatePrjs_T2();
   static LabeledSellaMatrices CreatePrjs_T5();
   static LabeledSellaMatrices CreatePrjs_R1();
   static LabeledSellaMatrices CreatePrjs_R3();

private:
   std::map<std::string, std::vector<S6> > themap;
   std::vector<LabeledSellaMatrices> projectors;
   std::vector<LabeledSellaMatrices> perps;
   StoreResults<std::string, S6> store;
   static std::vector< std::pair<std::string, MatS6> > vDeloneTypes;
   StoreResults<std::string, std::string> indexstore;

private:
   void ProcessItemStoreToVectorMap();
   void StoreAllReflections(const std::string& label, const S6& s1);
   void OneBound(const std::string& label, const S6& s1);
   void ProcessZeros(const std::string& label, const S6& s6);
   void ProcessVectorMapToPerpsAndProjectors();
   void Expand(const std::string& label, const MatS6& m);

   static bool FindDuplicate(const std::vector<S6>& out, const S6 s6);
   static MatS6 ProjectorFromVector(const std::string& label, const S6& s);
   static S6 MakeSampleType(const MatS6& m);
   static unsigned long Index6(const unsigned long i, const unsigned long j);
   static double Fraction(const double d, const S6& s);
   static std::vector<unsigned long> FindS6Zeros(const S6& s);

};

#endif // SELLA_H



