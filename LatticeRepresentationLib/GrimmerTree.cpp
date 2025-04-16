#pragma warning (disable: 4189) // Visual Studio -- local variable is initialized but not referenced

#include "C3.h"
#include "DeloneFitResults.h"
#include "DeloneTypeList.h"
#include "GrimmerTree.h"
#include "S6.h"
#include "S6BoundaryTransforms.h"
#include "Sella.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>



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
         // Create a new BravaisTypeForGrimmer with the template's Delone types
         BravaisTypeForGrimmer newBType(
            btype->GetBravaisType(),
            btype->GetDeloneTypes(),
            m_bravaisFits[btype->GetBravaisType()]);

         // Update the Delone fits from m_deloneFits
         newBType.UpdateDeloneFits(m_deloneFits.GetValueMap());

         chain.push_back(newBType);
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

void GrimmerChains::ListAllGrimmerChains(const std::string& st) const {
   std::cout << "Listing all Grimmer chains:" << std::endl;
   std::cout << std::string(70, '-') << std::endl;

   for (size_t chainIndex = 0; chainIndex < m_GrimmerChains.size(); ++chainIndex) {
      const OneGrimmerChain& currentChain = m_GrimmerChains[chainIndex];
      const std::vector<BravaisTypeForGrimmer>& bravaisChain = currentChain.GetChain();

      std::cout << "Chain " << (chainIndex + 1) << ": ";

      // Print all Bravais types in one line
      for (size_t pointIndex = 0; pointIndex < bravaisChain.size(); ++pointIndex) {
         std::cout << bravaisChain[pointIndex].GetBravaisType();
         if (pointIndex < bravaisChain.size() - 1) {
            std::cout << "  ";
         }
      }
      std::cout << std::endl;

      // Print all Bravais fits in one line
      std::cout << "Fits:    ";
      for (size_t pointIndex = 0; pointIndex < bravaisChain.size(); ++pointIndex) {
         std::cout << std::fixed << std::setprecision(2) << bravaisChain[pointIndex].GetFit();
         if (pointIndex < bravaisChain.size() - 1) {
            std::cout << "  ";
         }
      }
      std::cout << std::endl;

      // Print all associated Delone types with their fits
      for (size_t pointIndex = 0; pointIndex < bravaisChain.size(); ++pointIndex) {
         const BravaisTypeForGrimmer& bravaisType = bravaisChain[pointIndex];
         const std::vector<DeloneTypeForGrimmer>& deloneTypes = bravaisType.GetDeloneTypes();

         std::cout << "  " << bravaisType.GetBravaisType() << " Delone types: ";
         for (size_t i = 0; i < deloneTypes.size(); ++i) {
            std::cout << deloneTypes[i].GetDeloneName() << " ("
               << std::fixed << std::setprecision(2) << deloneTypes[i].GetFit() << ")";
            if (i < deloneTypes.size() - 1) {
               std::cout << ", ";
            }
         }
         std::cout << std::endl;
      }

      // Print a separator between chains
      std::cout << std::string(70, '-') << std::endl;
   }
}

std::string GrimmerChains::GetAllGrimmerChainsAsString() const {
   std::stringstream ss;

   ss << "Listing all Grimmer chains:" << std::endl;
   ss << std::string(70, '-') << std::endl;

   for (size_t chainIndex = 0; chainIndex < m_GrimmerChains.size(); ++chainIndex) {
      const OneGrimmerChain& currentChain = m_GrimmerChains[chainIndex];
      const std::vector<BravaisTypeForGrimmer>& bravaisChain = currentChain.GetChain();

      ss << "Chain " << (chainIndex + 1) << ": ";

      // Print all Bravais types in one line
      for (size_t pointIndex = 0; pointIndex < bravaisChain.size(); ++pointIndex) {
         ss << bravaisChain[pointIndex].GetBravaisType();
         if (pointIndex < bravaisChain.size() - 1) {
            ss << "  ";
         }
      }
      ss << std::endl;

      // Print all Bravais fits in one line
      ss << "Fits:    ";
      for (size_t pointIndex = 0; pointIndex < bravaisChain.size(); ++pointIndex) {
         ss << std::fixed << std::setprecision(2) << bravaisChain[pointIndex].GetFit();
         if (pointIndex < bravaisChain.size() - 1) {
            ss << "  ";
         }
      }
      ss << std::endl;

      // Print all associated Delone types with their fits
      for (size_t pointIndex = 0; pointIndex < bravaisChain.size(); ++pointIndex) {
         const BravaisTypeForGrimmer& bravaisType = bravaisChain[pointIndex];
         const std::vector<DeloneTypeForGrimmer>& deloneTypes = bravaisType.GetDeloneTypes();

         ss << "  " << bravaisType.GetBravaisType() << " Delone types: ";
         for (size_t i = 0; i < deloneTypes.size(); ++i) {
            ss << deloneTypes[i].GetDeloneName() << " ("
               << std::fixed << std::setprecision(2) << deloneTypes[i].GetFit() << ")";
            if (i < deloneTypes.size() - 1) {
               ss << ", ";
            }
         }
         ss << std::endl;
      }

      // Print a separator between chains
      ss << std::string(70, '-') << std::endl;
   }

   return ss.str();
}

void BravaisTypeForGrimmer::UpdateDeloneFits(const std::map<std::string, double>& deloneFits) {
   for (auto& deloneType : m_vtypes) {
      const std::string& deloneName = deloneType.GetDeloneName();
      if (deloneFits.find(deloneName) != deloneFits.end()) {
         deloneType.SetFit(deloneFits.at(deloneName));
      }
   }
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

// Fixed ReplaceRemediation implementation for GrimmerTree.cpp

GrimmerChains GrimmerChains::ReplaceRemediation(const DeloneFitResults& newFit) const {
   GrimmerChains out(*this);
   const std::string bravaisName = newFit.GetBravaisType();
   const std::string deloneName = newFit.GetGeneralType();
   const double newFitValue = newFit.GetRawFit();

   for (auto& oneChain : out.m_GrimmerChains) {
      std::vector<BravaisTypeForGrimmer> chain = oneChain.GetChain();
      for (auto& btype : chain) {
         if (btype.GetBravaisType() == bravaisName) {
            // Update the Bravais fit
            btype.SetFit(newFitValue);

            // Update the associated Delone type fits
            std::vector<DeloneTypeForGrimmer> deloneTypes = btype.GetDeloneTypes();
            for (auto& deloneType : deloneTypes) {
               if (deloneType.GetDeloneName() == deloneName) {
                  deloneType.SetFit(newFitValue);
               }
            }

            // Update the chain with modified Delone types
            btype = BravaisTypeForGrimmer(bravaisName, deloneTypes, newFitValue);
         }
      }

      // Update the chain in the OneGrimmerChain
      oneChain = OneGrimmerChain(chain);
   }

   return out;
}


#include <sstream>
#include <iomanip>
#include <algorithm>

#include <sstream>
#include <iomanip>
#include <algorithm>

#include <sstream>
#include <iomanip>
#include <algorithm>

#include <sstream>
#include <iomanip>
#include <algorithm>

#include <sstream>
#include <iomanip>
#include <algorithm>
#include <unordered_map>


// FitData struct for storing type and fit value pairs
struct FitData {
   std::string typeName;
   double fitValue;

   // Default constructor required for std::vector
   FitData() : typeName(""), fitValue(0.0) {}

   // Constructor with parameters
   FitData(const std::string& name, double fit) : typeName(name), fitValue(fit) {}

   // For sorting from smallest to highest fit value
   bool operator<(const FitData& other) const {
      return fitValue < other.fitValue;
   }
};

// Helper for drawing the Y-axis with better distributed grid values
void CreateBetterYAxisGridValues(std::vector<double>& yAxisValues, double maxValue) {
   // Clear any existing values
   yAxisValues.clear();

   // Always start with 0
   yAxisValues.push_back(0.0);

   // Add 0.3 instead of 0.5 for better placement
   yAxisValues.push_back(0.3);

   // Add 1.0
   yAxisValues.push_back(1.0);

   // Add 2.0 and 5.0 (skip 3.0 and 4.0 to avoid overcrowding)
   yAxisValues.push_back(2.0);
   yAxisValues.push_back(5.0);

   // Add 10.0, 20.0, 50.0
   yAxisValues.push_back(10.0);
   yAxisValues.push_back(20.0);
   yAxisValues.push_back(50.0);

   // Add 100.0, 200.0, 500.0 if within range
   if (maxValue > 100.0) {
      yAxisValues.push_back(100.0);
   }
   if (maxValue > 200.0) {
      yAxisValues.push_back(200.0);
   }
   if (maxValue > 500.0) {
      yAxisValues.push_back(500.0);
   }

   // Always add max value if it's higher than our predefined values
   if (maxValue > 500.0) {
      yAxisValues.push_back(maxValue);
   }
}

// Helper function to compress Y-axis values for better visualization
double CompressYAxisValue(const double value, const double minValue, const double maxValue, const double powerFactor = 0.3) {
   // Handle out-of-range values
   if (value <= minValue) return 0.0;
   if (value >= maxValue) return 1.0;

   // Normalize the value to 0-1 range
   const double normalizedValue = (value - minValue) / (maxValue - minValue);

   // Apply power function for smooth compression
   // This gives more space to smaller values
   return std::pow(normalizedValue, powerFactor);
}

// Class to collect and process fit data
class FitDataCollector {
private:
   std::map<std::string, double> deloneFits;
   std::map<std::string, double> pearsonFits;
   std::map<std::string, std::vector<std::pair<std::string, double>>> pearsonToDeloneMap;
   std::vector<FitData> sortedDeloneFits;
   std::vector<FitData> sortedPearsonFits;
   double maxDeloneFit = 0.0;
   double maxPearsonFit = 0.0;

public:
   // Extract fit data from Grimmer chains
   void collectFitData(const std::vector<OneGrimmerChain>& chains) {
      for (const auto& chain : chains) {
         for (const auto& bravaisType : chain.GetChain()) {
            // Store Bravais (Pearson) type fits
            const std::string bravaisName = bravaisType.GetBravaisType();
            const double bravaisFit = bravaisType.GetFit();

            if (pearsonFits.find(bravaisName) == pearsonFits.end() ||
               pearsonFits[bravaisName] > bravaisFit) {
               pearsonFits[bravaisName] = bravaisFit;
            }

            // Store Delone type fits and build Pearson-to-Delone mapping
            for (const auto& deloneType : bravaisType.GetDeloneTypes()) {
               const std::string deloneName = deloneType.GetDeloneName();
               const double deloneFit = deloneType.GetFit();

               if (deloneFits.find(deloneName) == deloneFits.end() ||
                  deloneFits[deloneName] > deloneFit) {
                  deloneFits[deloneName] = deloneFit;

                  // Add this Delone type to the corresponding Pearson type
                  bool found = false;
                  for (auto& pair : pearsonToDeloneMap[bravaisName]) {
                     if (pair.first == deloneName) {
                        pair.second = deloneFit;
                        found = true;
                        break;
                     }
                  }
                  if (!found) {
                     pearsonToDeloneMap[bravaisName].push_back(std::make_pair(deloneName, deloneFit));
                  }
               }
            }
         }
      }

      // Convert to sortable vectors
      for (const auto& pair : deloneFits) {
         sortedDeloneFits.push_back(FitData(pair.first, pair.second));
      }

      for (const auto& pair : pearsonFits) {
         sortedPearsonFits.push_back(FitData(pair.first, pair.second));
      }

      // Sort from smallest to highest fit value
      std::sort(sortedDeloneFits.begin(), sortedDeloneFits.end());
      std::sort(sortedPearsonFits.begin(), sortedPearsonFits.end());

      // Find max fit value for scaling
      if (!sortedDeloneFits.empty())
         maxDeloneFit = sortedDeloneFits.back().fitValue;
      if (!sortedPearsonFits.empty())
         maxPearsonFit = sortedPearsonFits.back().fitValue;

      // Make sure max values are not zero
      if (maxDeloneFit <= 0.0) maxDeloneFit = 1.0;
      if (maxPearsonFit <= 0.0) maxPearsonFit = 1.0;
   }

   const std::vector<FitData>& getDeloneFits() const { return sortedDeloneFits; }
   const std::vector<FitData>& getPearsonFits() const { return sortedPearsonFits; }
   double getMaxDeloneFit() const { return maxDeloneFit; }
   double getMaxPearsonFit() const { return maxPearsonFit; }

   std::vector<std::pair<std::string, double>> getSortedPearsonList() const {
      std::vector<std::pair<std::string, double>> sortedPearsonList;
      for (const auto& pair : pearsonFits) {
         sortedPearsonList.push_back(std::make_pair(pair.first, pair.second));
      }
      std::sort(sortedPearsonList.begin(), sortedPearsonList.end(),
         [](const auto& a, const auto& b) { return a.second < b.second; });
      return sortedPearsonList;
   }

   const std::map<std::string, std::vector<std::pair<std::string, double>>>& getPearsonToDeloneMap() const {
      return pearsonToDeloneMap;
   }
};

// SVG drawing helper for plots
class SVGPlotDrawer {
private:
   std::stringstream& ss;
   const int plotX;
   const int plotWidth;
   const int plotHeight;
   const std::string axisColor;
   const std::string gridColor;

public:
   SVGPlotDrawer(std::stringstream& stream, int x, int width, int height,
      const std::string& axis_color = "#333333",
      const std::string& grid_color = "#DDDDDD")
      : ss(stream), plotX(x), plotWidth(width), plotHeight(height),
      axisColor(axis_color), gridColor(grid_color) {
   }

   void drawPlot(int plotY, const std::vector<FitData>& data, const std::string& title,
      const std::string& lineColor, double maxValue, const double s6Norm) {

      // Create an SVG group for the entire plot for better positioning
      ss << "  <g id=\"plot_" << title << "\" transform=\"translate(" << plotX << "," << plotY << ")\">\n";

      // Plot border and background (now relative to group)
      ss << "    <rect x=\"0\" y=\"0\" width=\"" << plotWidth
         << "\" height=\"" << plotHeight << "\" fill=\"#FFFFFF\" stroke=\"" << axisColor << "\" />\n";

      // Plot title (now relative to group)
      ss << "    <text x=\"" << (plotWidth / 2) << "\" y=\"-20"
         << "\" text-anchor=\"middle\" font-family=\"Arial\" font-size=\"16\" font-weight=\"bold\">"
         << title << "</text>\n";

      // Y-axis label (now relative to group)
      ss << "    <text x=\"-60\" y=\"" << (plotHeight / 2)
         << "\" transform=\"rotate(-90,-60," << (plotHeight / 2) << ")\" "
         << "text-anchor=\"middle\" font-family=\"Arial\" font-size=\"14\">Fit Value</text>\n";

      // X-axis label (now relative to group)
      ss << "    <text x=\"" << (plotWidth / 2) << "\" y=\"" << (plotHeight + 35)
         << "\" text-anchor=\"middle\" font-family=\"Arial\" font-size=\"14\">Types (sorted by fit value)</text>\n";

      // Create y-axis ticks with predefined values
      std::vector<double> yAxisValues;
      CreateBetterYAxisGridValues(yAxisValues, maxValue);

      drawYAxis(yAxisValues, maxValue);
      drawNormReferenceLines(s6Norm, maxValue);

      if (!data.empty()) {
         drawDataPoints(data, lineColor, maxValue);
      }
      else {
         // No data message (now relative to group)
         ss << "    <text x=\"" << (plotWidth / 2) << "\" y=\"" << (plotHeight / 2)
            << "\" text-anchor=\"middle\" font-family=\"Arial\" font-size=\"14\" fill=\"#999999\">"
            << "No data available" << "</text>\n";
      }

      // Close the main fit list group
      ss << "  </g>\n";
   }

private:
   void drawYAxis(const std::vector<double>& yAxisValues, double maxValue) {
      // Draw each grid line and label
      for (double value : yAxisValues) {
         // Use the compression function to get the relative Y position (0-1)
         double relativeY = CompressYAxisValue(value, 0.0, maxValue);

         // Convert to actual Y coordinate in the SVG (relative to group)
         int yPos = plotHeight - relativeY * plotHeight;

         // Start a group for this gridline and label
         ss << "    <g id=\"grid_" << std::fixed << std::setprecision(3) << value << "\">\n";

         // Grid line
         ss << "      <line x1=\"0\" y1=\"" << yPos << "\" x2=\"" << plotWidth
            << "\" y2=\"" << yPos << "\" stroke=\"" << gridColor << "\" stroke-width=\"1\" />\n";

         // Y-axis label with appropriate precision
         std::stringstream valueStr;
         if (value == 0.0) {
            valueStr << "0";
         }
         else if (value < 1.5) {
            valueStr << std::fixed << std::setprecision(3) << value; // 3 decimal places for values under 1.5
         }
         else if (value < 10.0) {
            valueStr << std::fixed << std::setprecision(2) << value; // 2 decimal places for values between 1.5 and 10
         }
         else if (value < 100.0) {
            valueStr << std::fixed << std::setprecision(1) << value;
         }
         else {
            valueStr << std::fixed << std::setprecision(0) << value;
         }

         ss << "      <text x=\"-5\" y=\"" << (yPos + 5)
            << "\" text-anchor=\"end\" font-family=\"Arial\" font-size=\"12\">"
            << valueStr.str() << "</text>\n";

         // Close the group
         ss << "    </g>\n";
      }
   }

   void drawDataPoints(const std::vector<FitData>& data,
      const std::string& lineColor, double maxValue) {
      const int pointRadius = 4;

      // Determine X spacing
      double xSpacing = static_cast<double>(plotWidth - 20) / (data.size() - 1);
      if (data.size() == 1) xSpacing = 0; // Special case for single point

      // Draw the polyline connecting all points (now relative to group)
      ss << "    <polyline points=\"";
      for (size_t i = 0; i < data.size(); i++) {
         const auto& item = data[i];
         double xPos = 10 + xSpacing * i;
         double relativeY = CompressYAxisValue(item.fitValue, 0.0, maxValue);
         double yPos = plotHeight - relativeY * plotHeight;

         ss << xPos << "," << yPos << " ";
      }
      ss << "\" fill=\"none\" stroke=\"" << lineColor << "\" stroke-width=\"2\" />\n";

      // Draw the points and labels (now relative to group)
      for (size_t i = 0; i < data.size(); i++) {
         const auto& item = data[i];
         double xPos = 10 + xSpacing * i;

         // Use the compression function for each data point
         double relativeY = CompressYAxisValue(item.fitValue, 0.0, maxValue);
         double yPos = plotHeight - relativeY * plotHeight;

         // Draw the point (now relative to group)
         ss << "    <circle cx=\"" << xPos << "\" cy=\"" << yPos
            << "\" r=\"" << pointRadius << "\" fill=\"" << lineColor << "\" />\n";

         // Draw value tooltip on hover
         ss << "    <title>" << item.typeName << ": "
            << std::fixed
            << (item.fitValue < 1.5 ? std::setprecision(3) : std::setprecision(2))
            << item.fitValue << "</title>\n";

         // Draw type labels (now relative to group)
         ss << "    <text x=\"" << xPos << "\" y=\"" << (plotHeight + 15)
            << "\" text-anchor=\"middle\" font-family=\"Arial\" font-size=\"10\" transform=\"rotate(45,"
            << xPos << "," << (plotHeight + 15) << ")\">"
            << item.typeName << "</text>\n";

         // Draw vertical guide line for each point (now relative to group)
         ss << "    <line x1=\"" << xPos << "\" y1=\"" << yPos << "\" x2=\"" << xPos
            << "\" y2=\"" << plotHeight << "\" stroke=\"" << gridColor
            << "\" stroke-dasharray=\"2,2\" stroke-width=\"1\" />\n";
      }
   }


   
   // Add this method to the private section of the SVGPlotDrawer class in GrimmerTree.cpp
   void drawNormReferenceLines(const double s6Norm, double maxValue) {
      // 0.1% reference line
      const double pointOnePctValue = 0.001 * s6Norm;
      double relativeY_pointOnePct = CompressYAxisValue(pointOnePctValue, 0.0, maxValue);
      int yPos_pointOnePct = plotHeight - relativeY_pointOnePct * plotHeight;

      ss << "    <line x1=\"0\" y1=\"" << yPos_pointOnePct << "\" x2=\"" << plotWidth
         << "\" y2=\"" << yPos_pointOnePct << "\" stroke=\"#00CC00\" stroke-width=\"1\" stroke-dasharray=\"5,3\" />\n";
      ss << "    <text x=\"" << (plotWidth - 5) << "\" y=\"" << (yPos_pointOnePct - 5)
         << "\" text-anchor=\"end\" font-family=\"Arial\" font-size=\"12\" fill=\"#00CC00\">"
         << "0.1% (" << std::fixed << std::setprecision(3) << pointOnePctValue << ")</text>\n";

      // 1% reference line
      const double onePctValue = 0.01 * s6Norm;
      double relativeY_1pct = CompressYAxisValue(onePctValue, 0.0, maxValue);
      int yPos_1pct = plotHeight - relativeY_1pct * plotHeight;

      ss << "    <line x1=\"0\" y1=\"" << yPos_1pct << "\" x2=\"" << plotWidth
         << "\" y2=\"" << yPos_1pct << "\" stroke=\"#FF6600\" stroke-width=\"1\" stroke-dasharray=\"5,3\" />\n";
      ss << "    <text x=\"" << (plotWidth - 5) << "\" y=\"" << (yPos_1pct - 5)
         << "\" text-anchor=\"end\" font-family=\"Arial\" font-size=\"12\" fill=\"#FF6600\">"
         << "1% (" << std::fixed << std::setprecision(3) << onePctValue << ")</text>\n";

      // 5% reference line
      const double fivePctValue = 0.05 * s6Norm;
      double relativeY_5pct = CompressYAxisValue(fivePctValue, 0.0, maxValue);
      int yPos_5pct = plotHeight - relativeY_5pct * plotHeight;

      ss << "    <line x1=\"0\" y1=\"" << yPos_5pct << "\" x2=\"" << plotWidth
         << "\" y2=\"" << yPos_5pct << "\" stroke=\"#CC0000\" stroke-width=\"1\" stroke-dasharray=\"5,3\" />\n";
      ss << "    <text x=\"" << (plotWidth - 5) << "\" y=\"" << (yPos_5pct - 5)
         << "\" text-anchor=\"end\" font-family=\"Arial\" font-size=\"12\" fill=\"#CC0000\">"
         << "5% (" << std::fixed << std::setprecision(3) << fivePctValue << ")</text>\n";
   }

};

// Class to render the mapping list with double columns
class SVGFitListDrawer {
private:
   std::stringstream& ss;
   const int startX;
   const int startY;

public:
   SVGFitListDrawer(std::stringstream& stream, int x, int y)
      : ss(stream), startX(x), startY(y) {
   }

   void drawDoubleColumnFitList(
      const std::vector<std::pair<std::string, double>>& sortedPearsonList,
      const std::map<std::string, std::vector<std::pair<std::string, double>>>& pearsonToDeloneMap) {

      // Start SVG group for the fit list to make positioning more transparent
      ss << "  <g id=\"fitList\" transform=\"translate(" << startX << "," << startY << ")\">\n";

      // Draw the title (now using relative positioning within the group)
      ss << "    <text x=\"0\" y=\"-20\" font-family=\"Arial\" font-size=\"16\" font-weight=\"bold\">"
         << "Delone Types by Pearson Type" << "</text>\n";

      const int columnWidth = 160;     // Column width
      const int titleHeight = 20;      // Height for Pearson type titles
      const int itemHeight = 18;       // Height for Delone type items
      const int spaceBetweenGroups = 5; // Space between Pearson groups

      // Column positions - first column at x=0, second at x=160 (overlapping slightly)
      const int leftColX = 0;
      const int rightColX = 160;      // Moved closer to create overlap with first column

      // Track positions for each column
      int leftY = 0;
      int rightY = 0;

      // Count total items to balance columns
      int totalItems = 0;
      for (const auto& pearsonPair : sortedPearsonList) {
         // Count Pearson type + all its Delone types + spacing
         totalItems += 1 + pearsonToDeloneMap.at(pearsonPair.first).size() + 1;
      }

      int itemsPerColumn = totalItems / 2 + (totalItems % 2);
      int currentItemCount = 0;

      for (const auto& pearsonPair : sortedPearsonList) {
         const std::string& pearsonType = pearsonPair.first;
         const double pearsonFit = pearsonPair.second;

         // Sort associated Delone types by fit value
         std::vector<std::pair<std::string, double>> deloneList = pearsonToDeloneMap.at(pearsonType);
         std::sort(deloneList.begin(), deloneList.end(),
            [](const auto& a, const auto& b) { return a.second < b.second; });

         // Calculate items in this group (Pearson + Delone types + spacing)
         int groupItems = 1 + deloneList.size() + 1;

         // Determine which column to use
         int& currentY = (currentItemCount < itemsPerColumn) ? leftY : rightY;
         int currentX = (currentItemCount < itemsPerColumn) ? leftColX : rightColX;

         // Draw Pearson type with fit value
         ss << "    <text x=\"" << currentX << "\" y=\"" << currentY
            << "\" font-family=\"Arial\" font-size=\"14\" font-weight=\"bold\">"
            << pearsonType << " ("
            << std::fixed
            << (pearsonFit < 1.5 ? std::setprecision(3) : std::setprecision(2))
            << pearsonFit << ")"
            << "</text>\n";

         currentY += titleHeight;

         // Draw associated Delone types
         for (const auto& delonePair : deloneList) {
            ss << "    <text x=\"" << (currentX + 20) << "\" y=\"" << currentY
               << "\" font-family=\"Arial\" font-size=\"12\">"
               << delonePair.first << ": "
               << std::fixed
               << (delonePair.second < 1.5 ? std::setprecision(3) : std::setprecision(2))
               << delonePair.second
               << "</text>\n";

            currentY += itemHeight;
         }

         // Add spacing between Pearson types
         currentY += spaceBetweenGroups;

         // Update total item count
         currentItemCount += groupItems;
      }

      // Close the SVG group
      ss << "  </g>\n";
   }
};

std::string GrimmerChains::GenerateSortedFitPlots(const int width, const int height,
   const std::string& st) const {
   std::stringstream ss;

   // Define graph dimensions and positions
   const int plotWidth = (width - 400) * 0.75;    // Make plots 50% wider, but still leave room for columns
   const int plotHeight = (height - 200) / 2;     // Height of each plot
   const int topPlotY = 80;                // Y position of top plot
   const int bottomPlotY = topPlotY + plotHeight + 80; // Y position of bottom plot
   const int plotX = 100;                  // X position of both plots
   const int listX = plotX + plotWidth + 40; // X position for the fit list

   // Calculate reduced S6 norm
   const double reducedS6Norm = m_s6.norm();

   // Colors
   const std::string lineColorDelone = "#56B4E9";  // Sky blue
   const std::string lineColorPearson = "#E69F00";  // Orange
   const std::string axisColor = "#333333";       // Dark gray for axes
   const std::string gridColor = "#DDDDDD";       // Light gray for grid lines

   // SVG header
   ss << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
   ss << "<svg width=\"" << width << "\" height=\"" << height << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";

   // Title
   ss << "  <text x=\"" << width / 2 << "\" y=\"30\" text-anchor=\"middle\" font-family=\"Arial\" font-size=\"20\" font-weight=\"bold\">"
      << "SELLA Lattice Fit Analysis   " << st << "</text>\n";

   // Collect and process all fit data
   FitDataCollector dataCollector;
   dataCollector.collectFitData(m_GrimmerChains);

   // Draw the plots
   SVGPlotDrawer plotDrawer(ss, plotX, plotWidth, plotHeight, axisColor, gridColor);
   plotDrawer.drawPlot(topPlotY, dataCollector.getPearsonFits(), "Pearson Types (Sorted by Fit Value)",
      lineColorPearson, dataCollector.getMaxPearsonFit(), reducedS6Norm);
   plotDrawer.drawPlot(bottomPlotY, dataCollector.getDeloneFits(), "Delone Types (Sorted by Fit Value)",
      lineColorDelone, dataCollector.getMaxDeloneFit(), reducedS6Norm);

   // Draw the double-column fit list
   SVGFitListDrawer listDrawer(ss, listX, topPlotY);
   listDrawer.drawDoubleColumnFitList(
      dataCollector.getSortedPearsonList(),
      dataCollector.getPearsonToDeloneMap());

   // Add legend under the "Delone Types by Pearson Type" section
   // Calculate position - 50px below the last column item
   const int legendY = bottomPlotY +50; // Start at the same height as the bottom plot
   ss << "  <g id=\"legend\" transform=\"translate(" << listX << "," << legendY << ")\">\n";

   // Legend title
   ss << "    <text x=\"0\" y=\"0\" font-family=\"Arial\" font-size=\"14\" font-weight=\"bold\">Legend</text>\n";

   // Reference lines explanation
   ss << "    <text x=\"0\" y=\"25\" font-family=\"Arial\" font-size=\"12\">Reference Lines:</text>\n";

   // 0.1% reference line
   ss << "    <line x1=\"20\" y1=\"40\" x2=\"50\" y2=\"40\" stroke=\"#00CC00\" stroke-width=\"1\" stroke-dasharray=\"5,3\" />\n";
   ss << "    <text x=\"60\" y=\"45\" font-family=\"Arial\" font-size=\"12\">0.1% of S6 norm (A^2)</text>\n";

   // 1% reference line
   ss << "    <line x1=\"20\" y1=\"60\" x2=\"50\" y2=\"60\" stroke=\"#FF6600\" stroke-width=\"1\" stroke-dasharray=\"5,3\" />\n";
   ss << "    <text x=\"60\" y=\"65\" font-family=\"Arial\" font-size=\"12\">1% of S6 norm (A^2)</text>\n";

   // 5% reference line
   ss << "    <line x1=\"20\" y1=\"80\" x2=\"50\" y2=\"80\" stroke=\"#CC0000\" stroke-width=\"1\" stroke-dasharray=\"5,3\" />\n";
   ss << "    <text x=\"60\" y=\"85\" font-family=\"Arial\" font-size=\"12\">5% of S6 norm (A^2)</text>\n";

   // Plot line colors
   ss << "    <text x=\"0\" y=\"105\" font-family=\"Arial\" font-size=\"12\">Plot Lines:</text>\n";

   // Delone types line
   ss << "    <line x1=\"20\" y1=\"120\" x2=\"50\" y2=\"120\" stroke=\"" << lineColorDelone << "\" stroke-width=\"2\" />\n";
   ss << "    <text x=\"60\" y=\"125\" font-family=\"Arial\" font-size=\"12\">Delone types</text>\n";

   // Pearson types line
   ss << "    <line x1=\"20\" y1=\"140\" x2=\"50\" y2=\"140\" stroke=\"" << lineColorPearson << "\" stroke-width=\"2\" />\n";
   ss << "    <text x=\"60\" y=\"145\" font-family=\"Arial\" font-size=\"12\">Pearson types</text>\n";

   // Additional explanation
   ss << "    <text x=\"0\" y=\"165\" font-family=\"Arial\" font-size=\"12\" font-style=\"italic\">"
      << "Lower fit values indicate better matches</text>\n";
   ss << "    <text x=\"0\" y=\"185\" font-family=\"Arial\" font-size=\"12\" fill=\"#666666\">"
      << "Y-axis uses logarithmic compression</text>\n";
   ss << "    <text x=\"0\" y=\"205\" font-family=\"Arial\" font-size=\"12\" fill=\"#666666\">"
      << "to emphasize small values</text>\n";

   ss << "  </g>\n";

   // Close SVG
   ss << "</svg>";

   return ss.str();
}