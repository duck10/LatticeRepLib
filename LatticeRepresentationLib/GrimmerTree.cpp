#pragma warning (disable: 4189) // Visual Studio -- local variable is initialized but not referenced

#include "C3.h"
#include "DeloneFitResults.h"
#include "DeloneTypeList.h"
#include "GrimmerTree.h"
#include "S6.h"
#include "S6BoundaryTransforms.h"
#include "Sella.h"

#include <algorithm>
#include <set>
#include <utility>
#include <cmath>

const std::map<std::string, double> GrimmerChains::baseDeloneMap = {
   {"A1", 0.0}, {"M1A", 0.0}, {"M1B", 0.0}, {"M2A", 0.0}, {"M4", 0.0},
   {"O1A", 0.0}, {"O1B", 0.0}, {"O2", 0.0}, {"O4", 0.0}, {"O5", 0.0},
   {"R1", 0.0}, {"R3", 0.0}, {"T1", 0.0}, {"T2", 0.0}, {"T5", 0.0},
   {"C1", 0.0}, {"C3", 0.0}, {"C5", 0.0}, {"H4", 0.0}
};

const std::map<std::string, double> GrimmerChains::baseBravaisMap = {
   {"aP", 0.0}, {"mP", 0.0}, {"mC", 0.0}, {"oF", 0.0}, {"oI", 0.0},
   {"oC", 0.0}, {"oP", 0.0}, {"hR", 0.0}, {"tI", 0.0}, {"tP", 0.0},
   {"cI", 0.0}, {"cF", 0.0}, {"cP", 0.0}, {"hP", 0.0}
};

// Delone types
const DeloneTypeForGrimmer GrimmerChains::m_dtype_a1("A1", "aP", baseDeloneMap.at("A1"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_m1a("M1A", "mC", baseDeloneMap.at("M1A"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_m1b("M1B", "mC", baseDeloneMap.at("M1B"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_m2a("M2A", "mC", baseDeloneMap.at("M2A"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_m4("M4", "mP", baseDeloneMap.at("M4"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_o1a("O1A", "oF", baseDeloneMap.at("O1A"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_o1b("O1B", "oI", baseDeloneMap.at("O1B"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_o2("O2", "oI", baseDeloneMap.at("O2"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_o4("O4", "oS", baseDeloneMap.at("O4"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_o5("O5", "oP", baseDeloneMap.at("O5"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_r1("R1", "hR", baseDeloneMap.at("R1"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_r3("R3", "hR", baseDeloneMap.at("R3"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_t1("T1", "tI", baseDeloneMap.at("T1"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_t2("T2", "tI", baseDeloneMap.at("T2"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_t5("T5", "tP", baseDeloneMap.at("T5"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_c1("C1", "cI", baseDeloneMap.at("C1"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_c3("C3", "cF", baseDeloneMap.at("C3"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_c5("C5", "cP", baseDeloneMap.at("C5"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_h4("H4", "hP", baseDeloneMap.at("H4"));

// Bravais types
const BravaisTypeForGrimmer GrimmerChains::m_btype_aP("aP", { m_dtype_a1 }, baseBravaisMap.at("aP"));
const BravaisTypeForGrimmer GrimmerChains::m_btype_mP("mP", { m_dtype_m4 }, baseBravaisMap.at("mP"));
const BravaisTypeForGrimmer GrimmerChains::m_btype_mC("mC", { m_dtype_m1a, m_dtype_m1b, m_dtype_m2a }, baseBravaisMap.at("mC"));
const BravaisTypeForGrimmer GrimmerChains::m_btype_oF("oF", { m_dtype_o1a }, baseBravaisMap.at("oF"));
const BravaisTypeForGrimmer GrimmerChains::m_btype_oI("oI", { m_dtype_o1b, m_dtype_o2 }, baseBravaisMap.at("oI"));
const BravaisTypeForGrimmer GrimmerChains::m_btype_oC("oC", { m_dtype_o4 }, baseBravaisMap.at("oC"));
const BravaisTypeForGrimmer GrimmerChains::m_btype_oP("oP", { m_dtype_o5 }, baseBravaisMap.at("oP"));
const BravaisTypeForGrimmer GrimmerChains::m_btype_hR("hR", { m_dtype_r1, m_dtype_r3 }, baseBravaisMap.at("hR"));
const BravaisTypeForGrimmer GrimmerChains::m_btype_tI("tI", { m_dtype_t1, m_dtype_t2 }, baseBravaisMap.at("tI"));
const BravaisTypeForGrimmer GrimmerChains::m_btype_tP("tP", { m_dtype_t5 }, baseBravaisMap.at("tP"));
const BravaisTypeForGrimmer GrimmerChains::m_btype_cI("cI", { m_dtype_c1 }, baseBravaisMap.at("cI"));
const BravaisTypeForGrimmer GrimmerChains::m_btype_cF("cF", { m_dtype_c3 }, baseBravaisMap.at("cF"));
const BravaisTypeForGrimmer GrimmerChains::m_btype_cP("cP", { m_dtype_c5 }, baseBravaisMap.at("cP"));
const BravaisTypeForGrimmer GrimmerChains::m_btype_hP("hP", { m_dtype_h4 }, baseBravaisMap.at("hP"));
const std::vector<std::vector<const BravaisTypeForGrimmer*>> GrimmerChains::chainTemplates = {
   {&m_btype_aP, &m_btype_mP, &m_btype_oP, &m_btype_tP, &m_btype_cP},
   {&m_btype_aP, &m_btype_mP, &m_btype_oC, &m_btype_tP, &m_btype_cP},
   {&m_btype_aP, &m_btype_mP, &m_btype_oC, &m_btype_hP},
   {&m_btype_aP, &m_btype_mC, &m_btype_oC, &m_btype_tP, &m_btype_cP},
   {&m_btype_aP, &m_btype_mC, &m_btype_oC, &m_btype_hP},

   {&m_btype_aP, &m_btype_mC, &m_btype_oI, &m_btype_tI, &m_btype_cI},
   {&m_btype_aP, &m_btype_mC, &m_btype_oI, &m_btype_tI, &m_btype_cF},
   {&m_btype_aP, &m_btype_mC, &m_btype_oF, &m_btype_tI, &m_btype_cI},
   {&m_btype_aP, &m_btype_mC, &m_btype_oF, &m_btype_tI, &m_btype_cF},
   {&m_btype_aP, &m_btype_mC, &m_btype_hR, &m_btype_cP},

   {&m_btype_aP, &m_btype_mC, &m_btype_hR, &m_btype_cI},
   {&m_btype_aP, &m_btype_mC, &m_btype_hR, &m_btype_cF}
};


void GrimmerChains::updateFits(MapOFDeloneFits& deloneFits, MapOfBravaisFits& bravaisFits) {
   m_deloneFits = deloneFits;
   m_bravaisFits = bravaisFits;
   initializeChains();
}

void GrimmerChains::initializeChains() {
   m_GrimmerChains.clear();
   for (const auto& chainTemplate : chainTemplates) {
      std::vector<BravaisTypeForGrimmer> chain;
      for (const auto* btype : chainTemplate) {


         auto x1 = btype->GetBravaisType();
         auto x2 = btype->GetDeloneTypes();
         auto x3 = btype->GetBravaisType();
         auto x4 = m_bravaisFits[x3];




         chain.push_back(BravaisTypeForGrimmer(
            btype->GetBravaisType(),
            btype->GetDeloneTypes(),
            m_bravaisFits[btype->GetBravaisType()]));
      }
      m_GrimmerChains.emplace_back(chain);
   }
}

DeloneTypeForGrimmer::DeloneTypeForGrimmer(const std::string& deloneName, const std::string bravaisType, const double fit/*=DBL_MAX*/)
   :m_DeloneName(deloneName)
   , m_BravaisType(bravaisType)
   , m_fit(fit)
{      }

BravaisTypeForGrimmer::BravaisTypeForGrimmer(const std::string& name,
   const std::vector<DeloneTypeForGrimmer>& vdeloneTypes,
   const double fit)
   : m_name(name)
   , m_vtypes(vdeloneTypes)
   , m_fit(fit)
{
}

void BravaisTypeForGrimmer::PopulateBravaisTypeWithFitValues(std::map<std::string, double>& themap) {
   for (auto& type : m_vtypes) {
      const std::string name = type.GetDeloneName();
      const double fit = themap[name];
      type.SetFit(fit);
   }
}
//
//void OneBravaisChain::PopulateOneBChainWithFitValues(std::map<std::string, double>& themap) {
//   for (auto& bType : m_chain) {
//      bType.PopulateBravaisTypeWithFitValues(themap);
//   }
//}

//BravaisChains::BravaisChains(const std::vector<OneBravaisChain>& chain) {
//   throw; //LCA  TBD
//}


void GrimmerChains::BestInBravaisType(const DeloneTypeForGrimmer& type) const { // member of GrimmerChains
   GrimmerChains gcs = (*this);
   for (const auto& gc : gcs.GetChains()) {
      for (auto b : gc.GetChain()) {
         double dmin = DBL_MAX;
         for (const auto& btype : b.GetDeloneTypes()) {
            dmin = std::min(dmin, btype.GetFit());
         }
         b.SetFit(dmin);
      }
   }
}



std::vector<std::vector<std::string> > CreateBravaisChains()
{
   std::vector<std::vector<std::string> > v;
   v.push_back({ "aP", "mP", "oP", "tP", "cP" });
   v.push_back({ "aP", "mP", "oS", "tP", "cP" });
   v.push_back({ "aP", "mP", "oS", "hP" });
   v.push_back({ "aP", "mS", "oS", "hP" });
   v.push_back({ "aP", "mS", "oS", "tP", "cP" });

   v.push_back({ "aP", "mS", "oI", "tI", "cI" });
   v.push_back({ "aP", "mS", "oI", "tI", "cF" });
   v.push_back({ "aP", "mS", "oF", "tI", "cI" });
   v.push_back({ "aP", "mS", "oF", "tI", "cF" });
   v.push_back({ "aP", "mS", "hR", "cP" });

   v.push_back({ "aP", "mS", "hR", "cI" });
   v.push_back({ "aP", "mS", "hR", "cF" });



   return v;
}

std::map<std::string, double> MapOfBravaisFits::CreateMapOFBravaisFits(std::vector<DeloneFitResults>& fits) {
   std::map<std::string, double> valuemap;
   for (const auto& fit : fits) {
      const double fitvalue = fit.GetRawFit();
      const std::string bravaisName = fit.GetDeloneType();
      if (valuemap.find(bravaisName) == valuemap.end()) {
         valuemap[bravaisName] = fitvalue;
      }
      else
      {
         if (valuemap[bravaisName] > fitvalue)
         {
            valuemap[bravaisName] = fitvalue;
         }
      }
   }
   m_valuemap = valuemap;
   return valuemap;
}

std::map<std::string, double> MapOFDeloneFits::CreateMapOFDeloneFits(std::vector<DeloneFitResults>& fits) {
   std::map<std::string, double> valuemap;
   for (const auto& fit : fits) {
      const std::string name = fit.GetGeneralType();
      const double fitvalue = fit.GetRawFit();
      m_valuemap[name] = fitvalue;
   }
   return m_valuemap;
}

GrimmerChainFailure OneGrimmerChain::CheckOneGrimmerChain()
{
   m_fail = false;
   double prevFit = m_chain[0].GetFit();
   GrimmerChainFailure fails;
   for (size_t i = 0; i < m_chain.size(); ++i) {
      const double fit = m_chain[i].GetFit();
      if (fit < (1.0 - 1.0E-8) * prevFit) {
         const double test = fit - prevFit;
         m_fail = true;
         fails.AddBravaistypeAndFit(m_chain[i - 2].GetBravaisType(), m_chain[i - 2].GetFit());
         fails.AddBravaistypeAndFit(m_chain[i - 1].GetBravaisType(), m_chain[i - 1].GetFit());
         fails.AddBravaistypeAndFit(m_chain[i].GetBravaisType(), m_chain[i].GetFit());
         if (const bool debug = false == true)
         {
            std::cout << fails << std::endl;
         }
         m_failures = fails;
      }
      prevFit = fit;
   }
   return fails;
}


void GrimmerChains::CreateGrimmerChains(MapOFDeloneFits& theDeloneMap, MapOfBravaisFits& theBravaisMap) {
   m_deloneFits = theDeloneMap;
   m_bravaisFits = theBravaisMap;
   initializeChains();
}

void GrimmerChains::CheckAllGrimmerChains()
{
   m_hasChainFailure = false;
   std::set<std::string> uniqueFailures; //lca save these up!!!!!!!!!!!!!!
   for (auto& chain : m_GrimmerChains) {
      const GrimmerChainFailure fail = chain.CheckOneGrimmerChain();
      if (!fail.empty()) {
         std::stringstream ss;
         ss << fail << std::endl;
         uniqueFailures.insert(ss.str());
         m_GrimmerFailures.emplace_back(fail);
         m_hasChainFailure = true;
         if (const bool debug = false == true)
         {
            std::cout << fail << std::endl;
         }
      }
   }
}

static inline int countC3Zeros(const C3& c) {
   int count = 0;
   const double c3norm = c.norm();
   const double c1 = abs(c[0]);
   const double c2 = abs(c[1]);
   const double c3 = abs(c[2]);
   if (c1 / c3norm < 1.0e-3) ++count;
   if (c2 / c3norm < 1.0e-3) ++count;
   if (c3 / c3norm < 1.0e-3) ++count;
   return count;
}

static inline int countC3Zeros(const C3& c, const double upperLimit) {
   // upper limit should be the agreement fit of the Delone type above the failure point !!!!!!!!!!
   int count = 0;
   const double c1 = abs(c[0]);
   const double c2 = abs(c[1]);
   const double c3 = abs(c[2]);
   if (c1 < 1.01 * upperLimit) ++count;
   if (c2 < 1.01 * upperLimit) ++count;
   if (c3 < 1.01 * upperLimit) ++count;
   return count;
}

static inline int CountS6Zeros(const S6& s6) {
   int count = 0;
   for (size_t i = 0; i < 6; ++i) {
      if (abs(s6[i]) <= 0.0) ++count;
   }
   return count;
}

static inline int CountS6Zeros(const S6& s6, const double upperLimit) {
   int count = 0;
   for (size_t i = 0; i < 6; ++i) {
      if (abs(s6[i]) <= 1.01 * upperLimit) ++count;
   }
   return count;
}

static inline int FindLoneS6Zero(const size_t n1, const size_t n2, const S6& s6, const double upper) {
   for (size_t i = 0; i < 6; ++i) {
      if (i == n1 || i == n2) continue;
      if (abs(s6[i]) < upper)  return int(i);
   }
   return 19191;
}

GrimmerChainFailure::GrimmerChainFailure(const std::string& bravaisName, const double fit) {
   //    std::vector<std::pair<std::string, double>> m_failGroup;
   m_failGroup.emplace_back(bravaisName, fit);
}


DeloneFitResults GrimmerChains::Remediation(const std::string& bravaisName, const double initialFit) {

   static const std::vector<std::shared_ptr<GenerateDeloneBase> > sptypes =
      GenerateDeloneBase().Select(bravaisName);

   const DeloneFitResults dfrInit = Sella::SellaFitXXXXXX(sptypes[0], m_s6);
   auto dfr = dfrInit;

   for (size_t i = 1; i <= 6; ++i)
   {
      // apply a boundary transform
      const MatS6 matrix = S6BoundaryTransforms::generateOneBoundaryTransform(i);
      const S6 mod = matrix * m_s6;
      const auto dfrTemp = Sella::SellaFitXXXXXX(sptypes[0], matrix * m_s6);
      if (dfrTemp.GetRawFit() < dfr.GetRawFit()) {
         dfr = dfrTemp;
      }
   }
   return dfr;
}

GrimmerChains GrimmerChains::ReplaceRemediation(const DeloneFitResults& newFit) const {
   std::vector<OneGrimmerChain> chain;
   GrimmerChains out(*this);
   const std::string name = newFit.GetDeloneType();

   for (auto& oneChain : m_GrimmerChains) {
      //chain.emplace_back(oneChain);
      for (auto& b : oneChain.GetChain()) {
         if (b.GetBravaisType() == name) {
            const int i19191 = 19191;
            b.SetFit(newFit.GetRawFit());
         }
      }
   }
   return out;
}

