#include "C3.h"
#include "DeloneFitResults.h"
#include "DeloneTypeList.h"
#include "GrimmerTree.h"
#include "S6.h"
#include "S6BoundaryTransforms.h"
#include "Sella.h"

#include <algorithm>
#include <utility>
#include <cmath>

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

std::map<std::string, double> GrimmerChains::CreateMapOfDeloneFitValues(const std::vector<DeloneFitResults>& fits) {
   std::map<std::string, double> fitmap;
   for (const auto& fit : fits) {
      fitmap[fit.GetLatticeType()] = fit.GetRawFit();
   }
   return fitmap;
}

std::map<std::string, double> GrimmerChains::CreateMapOfBravaisFitValues(const std::vector<DeloneFitResults>& fits) {
   std::map<std::string, double> fitmap;
   for (const auto& fit : fits) {
      fitmap[fit.GetDeloneType()] = fit.GetRawFit();
   }
   return fitmap;
}

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

void  GrimmerChains::CreateGrimmerChains(MapOFDeloneFits& theDeloneMap, MapOfBravaisFits& theBravaisMap)
{
   // there are 19 non-degenerate Delone types
   const DeloneTypeForGrimmer dtype_a1("A1", "aP", theDeloneMap["A1"]);
   const DeloneTypeForGrimmer dtype_m1a("M1A", "mC", theDeloneMap["M1A"]);
   const DeloneTypeForGrimmer dtype_m1b("M1B", "mC", theDeloneMap["M1B"]);
   const DeloneTypeForGrimmer dtype_m2a("M2A", "mC", theDeloneMap["M2A"]);
   const DeloneTypeForGrimmer dtype_m4("M4", "mP", theDeloneMap["M4"]);

   const DeloneTypeForGrimmer dtype_o1a("O1A", "oF", theDeloneMap["O1A"]);
   const DeloneTypeForGrimmer dtype_o1b("O1B", "oI", theDeloneMap["O1B"]);
   const DeloneTypeForGrimmer dtype_o2("O2", "oI", theDeloneMap["O2"]);
   const DeloneTypeForGrimmer dtype_o4("O4", "oS", theDeloneMap["O4"]);
   const DeloneTypeForGrimmer dtype_o5("O5", "oP", theDeloneMap["O5"]);

   const DeloneTypeForGrimmer dtype_r1("R1", "hR", theDeloneMap["R1"]);
   const DeloneTypeForGrimmer dtype_r3("R3", "hR", theDeloneMap["R3"]);
   const DeloneTypeForGrimmer dtype_t1("T1", "tI", theDeloneMap["T1"]);
   const DeloneTypeForGrimmer dtype_t2("T2", "tI", theDeloneMap["T2"]);
   const DeloneTypeForGrimmer dtype_t5("T5", "tP", theDeloneMap["T5"]);

   const DeloneTypeForGrimmer dtype_c1("C1", "cI", theDeloneMap["C1"]);
   const DeloneTypeForGrimmer dtype_c3("C3", "cf", theDeloneMap["C3"]);
   const DeloneTypeForGrimmer dtype_c5("C5", "cP", theDeloneMap["C5"]);
   const DeloneTypeForGrimmer dtype_h4("H4", "hP", theDeloneMap["H4"]);

   //// there are 14 Bravais typs with Delone synonyms
   const BravaisTypeForGrimmer btype_aP("aP", std::vector<DeloneTypeForGrimmer>{ dtype_a1 }, theBravaisMap["aP"]);
   const BravaisTypeForGrimmer btype_mP("mP", { dtype_m4 }, theBravaisMap["mP"]);
   const BravaisTypeForGrimmer btype_mC("mS", { dtype_m1a, dtype_m1b, dtype_m2a }, theBravaisMap["mC"]);
   const BravaisTypeForGrimmer btype_oF("oF", { dtype_o1a }, theBravaisMap["oF"]);
   const BravaisTypeForGrimmer btype_oI("oI", { dtype_o1b, dtype_o2 }, theBravaisMap["oI"]);

   const BravaisTypeForGrimmer btype_oS("oS", { dtype_o4 }, theBravaisMap["oC"]);
   const BravaisTypeForGrimmer btype_oP("oP", { dtype_o5 }, theBravaisMap["oP"]);
   const BravaisTypeForGrimmer btype_hR("hR", { dtype_r1, dtype_r3 }, theBravaisMap["hR"]);
   const BravaisTypeForGrimmer btype_tI("tI", { dtype_t1, dtype_t2 }, theBravaisMap["tI"]);
   const BravaisTypeForGrimmer btype_tP("tP", { dtype_t5 }, theBravaisMap["tP"]);

   const BravaisTypeForGrimmer btype_cI("cI", { dtype_c1 }, theBravaisMap["cI"]);
   const BravaisTypeForGrimmer btype_cF("cF", { dtype_c3 }, theBravaisMap["cF"]);
   const BravaisTypeForGrimmer btype_cP("cP", { dtype_c5 }, theBravaisMap["cP"]);
   const BravaisTypeForGrimmer btype_hP("hP", { dtype_h4 }, theBravaisMap["hP"]);
   m_btype_aP = btype_aP;
   m_btype_mP = btype_mP;
   m_btype_mC = btype_mC;
   m_btype_oF = btype_oF;
   m_btype_oI = btype_oI;

   m_btype_oS = btype_oS;
   m_btype_oP = btype_oP;
   m_btype_hR = btype_hR;
   m_btype_tI = btype_tI;
   m_btype_tP = btype_tP;

   m_btype_cI = btype_cI;
   m_btype_cF = btype_cF;
   m_btype_cP = btype_cP;
   m_btype_hP = btype_hP;

   //// now make the Grimmer chains
   //// there are only 12 Grimmer chains
   typedef std::vector<BravaisTypeForGrimmer> Q;
   m_GrimmerChains.emplace_back(Q{ btype_aP, btype_mP, btype_oP, btype_tP, btype_cP });
   m_GrimmerChains.emplace_back(Q{ btype_aP, btype_mP, btype_oS, btype_tP, btype_cP });
   m_GrimmerChains.emplace_back(Q{ btype_aP, btype_mP, btype_oS, btype_hP });
   m_GrimmerChains.emplace_back(Q{ btype_aP, btype_mC, btype_oS, btype_tP, btype_cP });
   m_GrimmerChains.emplace_back(Q{ btype_aP, btype_mC, btype_oS, btype_hP });

   m_GrimmerChains.emplace_back(Q{ btype_aP, btype_mC, btype_oI, btype_tI, btype_cI });
   m_GrimmerChains.emplace_back(Q{ btype_aP, btype_mC, btype_oI, btype_tI, btype_cF });
   m_GrimmerChains.emplace_back(Q{ btype_aP, btype_mC, btype_oF, btype_tI, btype_cI });
   m_GrimmerChains.emplace_back(Q{ btype_aP, btype_mC, btype_oF, btype_tI, btype_cF });
   m_GrimmerChains.emplace_back(Q{ btype_aP, btype_mC, btype_hR, btype_cP });

   m_GrimmerChains.emplace_back(Q{ btype_aP, btype_mC, btype_hR, btype_cI });
   m_GrimmerChains.emplace_back(Q{ btype_aP, btype_mC, btype_hR, btype_cF }); 
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
      const std::string bravaisName = fit.GetBravaisType();
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
      if (fit < (1.0-1.0E-8)*prevFit) {
         const double test = fit - prevFit;
         m_fail = true;
         fails.AddBravaistypeAndFit(m_chain[i - 2].GetBravaisType(), m_chain[i - 2].GetFit());
         fails.AddBravaistypeAndFit(m_chain[i-1].GetBravaisType(), m_chain[i-1].GetFit());
         fails.AddBravaistypeAndFit(m_chain[i].GetBravaisType(), m_chain[i].GetFit());
         std::cout << fails << std::endl;
         m_failures = fails;
      }
      prevFit = fit;
   }
   return fails;
}

void GrimmerChains::CheckAllGrimmerChains()
{
   m_hasChainFailure = false;
   for (auto& chain : m_GrimmerChains) {
      const GrimmerChainFailure fail = chain.CheckOneGrimmerChain();
      if (!fail.empty()) {
         m_GrimmerFailures.emplace_back(fail);
         m_hasChainFailure = true;
         std::cout << fail << std::endl;
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

   S6BoundaryTransforms sbt;
   for (size_t i = 0; i < 6; ++i)
   {
      // apply a boundary transform
      const MatS6 matrix = sbt.GetOneTransform(i);
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

