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
private:
   std::string m_label;
   std::vector<MatS6> m_vector;
};

class Sella {
public:

   Sella();
   double TestOneType(const std::string& label, const S6 &s6, const std::vector<MatS6>& vm);
   std::vector<std::pair<std::string, double> > GetVectorOfFits(const S6& s6);

   std::vector<LabeledSellaMatrices> GetPerps(void) const {
      return perps;
   }

   std::vector<LabeledSellaMatrices> GetProjectors(void) const {
      return projectors;
   }

   static std::vector<LabeledSellaMatrices> CreateAllPerps();
   static std::vector<LabeledSellaMatrices> CreateAllPrjs();

   std::pair<std::string, double>  GetBestFitForCrystalSystem(const std::string& type, const S6& s6);
   double GetFitForDeloneType(const std::string& type, const S6& s6);
   std::vector<std::pair<std::string, double> > GetAllFitsForCrystalSystem(const std::string& type);

   void GetDeloneTypesForCrystalType();
   void GetCrystalTypesForDeloneType();

private:
   static const std::vector<LabeledSellaMatrices> projectors;
   static const std::vector<LabeledSellaMatrices> perps;

private:
   static LabeledSellaMatrices CreatePerps_H4();
   static LabeledSellaMatrices CreatePerps_C1();
   static LabeledSellaMatrices CreatePerps_C3();
   static LabeledSellaMatrices CreatePerps_C5();
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

};

#endif // SELLA_H



