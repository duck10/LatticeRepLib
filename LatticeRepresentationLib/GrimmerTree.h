#ifndef GRIMMERTREE_H
#define  GRIMMERTREE_H

#include <cmath>
#include <cfloat>
#include <string>
#include <map>
#include <vector>

class DeloneTypeForGrimmer {
public:
   friend std::ostream& operator<< (std::ostream& o, const DeloneTypeForGrimmer& dt) {
      o << "; Delone name " << dt.m_DeloneName << " Bravais name " << dt.m_BravaisType;
      o << "  fit  " << dt.m_fit << "\n";
      return o;
   }
   DeloneTypeForGrimmer() = default;
   DeloneTypeForGrimmer(const std::string& deloneName, const std::string bravaisType, const double fit = DBL_MAX);
   void SetFit(const double d) { m_fit = d; }
   double GetFit() const { return m_fit; }
   std::string GetDeloneName() const { return m_DeloneName; }
   std::string GetBravaisType() const { return m_BravaisType; }
private:
   std::string m_DeloneName;
   std::string m_BravaisType;
   double m_fit;
};

class MapOfBravaisFits {
public:
   friend std::ostream& operator<< (std::ostream& o, MapOfBravaisFits& mbf) {
      o << "; Bravais Fit Map\n";
      std::map<std::string, double>::iterator it;
      for (it = mbf.m_valuemap.begin(); it != mbf.m_valuemap.end(); ++it) {
         o << "; Bravais name " << (*it).first << "  fit  " << (*it).second << std::endl;
      }
      return o;
   }

   auto operator[] (const std::string& s) { return m_valuemap[s]; }
   MapOfBravaisFits() = default;
   std::map<std::string, double> CreateMapOFBravaisFits(std::vector<DeloneFitResults>& fits);
std::map<std::string, double> GetMap() const { return m_valuemap; }private:
   std::map<std::string, double> m_valuemap;
};

class MapOFDeloneFits {
public:
   friend std::ostream& operator<< (std::ostream& o, MapOFDeloneFits& mdf) {
      o << "; Delone Fit Map\n";
      std::map<std::string, double>::iterator it;
      for (it = mdf.m_valuemap.begin(); it != mdf.m_valuemap.end(); ++it) {
         o << "; Delone name " << (*it).first << "  fit  " << (*it).second << std::endl;
      }
      return o;
   }

   auto operator[] (const std::string& s) { return m_valuemap[s]; }
   MapOFDeloneFits() = default;
   std::map<std::string, double> CreateMapOFDeloneFits(std::vector<DeloneFitResults>& fits);
   const std::map<std::string, double>& GetValueMap() const { return m_valuemap; }
std::map<std::string, double> GetMap() const { return m_valuemap; }private:
   std::map<std::string, double> m_valuemap;
};

class BravaisTypeForGrimmer {
public:
   friend std::ostream& operator<< (std::ostream& o, const BravaisTypeForGrimmer& bt) {
      o << "; Bravais Fit Map " << bt.m_name << std::endl;
      o << "  fit  " << bt.m_fit << "\n";
      for (const auto& type : bt.m_vtypes) {
         o << "\t\t " << type;
      }
      return o;
   }
   BravaisTypeForGrimmer() = default;
   BravaisTypeForGrimmer(const std::string& name,
      const std::vector<DeloneTypeForGrimmer>& vdeloneTypes,
      const double fit = DBL_MAX);
   void SetFit(const double d) { m_fit = d; }
   double GetFit() const { return m_fit; }
   std::string GetBravaisType() const { return m_name; }
   std::vector<DeloneTypeForGrimmer> GetDeloneTypes() const { return m_vtypes; }
   void PopulateBravaisTypeWithFitValues(std::map<std::string, double>& themap);
private:
   std::string m_name;
   std::vector<DeloneTypeForGrimmer> m_vtypes;
   double m_fit;
};

class GrimmerChainFailure {
public:
   friend std::ostream& operator<< (std::ostream& o, const GrimmerChainFailure& obc) {
      if (obc.m_failGroup.empty()) {
         o << "; No Grimmer Chain failure\n";
      }
      else
      {
         o << "; Grimmer Chain failure\n";
         o << obc.m_failGroup[0].first << " " << obc.m_failGroup[0].second << std::endl;
         o << obc.m_failGroup[1].first << " " << obc.m_failGroup[1].second << "  ****" << std::endl;
         o << obc.m_failGroup[2].first << " " << obc.m_failGroup[2].second << std::endl;
      }
      return o;
   }

   GrimmerChainFailure() = default;
   GrimmerChainFailure(const std::string& bravaisName, const double fit);

   void AddBravaistypeAndFit(const std::string& bravaisName, const double fit) {
      m_failGroup.emplace_back(std::make_pair(bravaisName, fit));
   }
   std::pair<std::string, double> operator[](const std::size_t n) { return m_failGroup[n]; }
   bool empty() const { return m_failGroup.empty(); }
   std::vector<std::pair<std::string, double>> GetFailures() const { return m_failGroup; }
private:
   std::vector<std::pair<std::string, double>> m_failGroup;;
};

class OneGrimmerChain {
public:
   GrimmerChainFailure CheckOneGrimmerChain();
   friend std::ostream& operator<< (std::ostream& o, const OneGrimmerChain& obc) {
      o << ";--------- One Grimmer Chain -------------------------------------\n";
      for (const auto& btype : obc.m_chain) {
         o << btype << std::endl;
      }
      return o;
   }
   OneGrimmerChain() = default;
   OneGrimmerChain(const std::vector<BravaisTypeForGrimmer>& chain) { m_chain = chain; }
   void PopulateOneBChainWithFitValues(std::map<std::string, double>& themap);
   std::vector<BravaisTypeForGrimmer> GetChain() const { return m_chain; }
private:
   std::vector<BravaisTypeForGrimmer> m_chain;
   GrimmerChainFailure m_failures;
   bool m_fail;
};

class GrimmerChains {
private:
   // Static constant data
   static const std::map<std::string, double> baseDeloneMap;
   static const std::map<std::string, double> baseBravaisMap;

   static const DeloneTypeForGrimmer m_dtype_a1;
   static const DeloneTypeForGrimmer m_dtype_m1a;
   static const DeloneTypeForGrimmer m_dtype_m1b;
   static const DeloneTypeForGrimmer m_dtype_m2a;
   static const DeloneTypeForGrimmer m_dtype_m4;
   static const DeloneTypeForGrimmer m_dtype_o1a;
   static const DeloneTypeForGrimmer m_dtype_o1b;
   static const DeloneTypeForGrimmer m_dtype_o2;
   static const DeloneTypeForGrimmer m_dtype_o4;
   static const DeloneTypeForGrimmer m_dtype_o5;
   static const DeloneTypeForGrimmer m_dtype_r1;
   static const DeloneTypeForGrimmer m_dtype_r3;
   static const DeloneTypeForGrimmer m_dtype_t1;
   static const DeloneTypeForGrimmer m_dtype_t2;
   static const DeloneTypeForGrimmer m_dtype_t5;
   static const DeloneTypeForGrimmer m_dtype_c1;
   static const DeloneTypeForGrimmer m_dtype_c3;
   static const DeloneTypeForGrimmer m_dtype_c5;
   static const DeloneTypeForGrimmer m_dtype_h4;

   static const BravaisTypeForGrimmer m_btype_aP;
   static const BravaisTypeForGrimmer m_btype_mP;
   static const BravaisTypeForGrimmer m_btype_mC;
   static const BravaisTypeForGrimmer m_btype_oF;
   static const BravaisTypeForGrimmer m_btype_oI;
   static const BravaisTypeForGrimmer m_btype_oC;
   static const BravaisTypeForGrimmer m_btype_oP;
   static const BravaisTypeForGrimmer m_btype_hR;
   static const BravaisTypeForGrimmer m_btype_tI;
   static const BravaisTypeForGrimmer m_btype_tP;
   static const BravaisTypeForGrimmer m_btype_cI;
   static const BravaisTypeForGrimmer m_btype_cF;
   static const BravaisTypeForGrimmer m_btype_cP;
   static const BravaisTypeForGrimmer m_btype_hP;

   static const std::vector<std::vector<const BravaisTypeForGrimmer*>> chainTemplates;

   // Instance data
   std::vector<OneGrimmerChain> m_GrimmerChains;
   std::vector<GrimmerChainFailure> m_GrimmerFailures;
   bool m_hasChainFailure;
   S6 m_s6;
   MapOFDeloneFits m_deloneFits;
   MapOfBravaisFits m_bravaisFits;

public:
   friend std::ostream& operator<< (std::ostream& o, const GrimmerChains& obc) {
      if (obc.m_GrimmerChains.empty()) {
         o << "; There Are No Grimmer Chains\n";
      }
      else
      {
         o << "; Grimmer Chains\n";
         for (const auto& btypeChain : obc.m_GrimmerChains) {
            o << btypeChain << std::endl;
         }
         for (size_t i = 0; i < obc.m_GrimmerFailures.size(); ++i) {
            o << i << " " << obc.m_GrimmerFailures[i] << std::endl;

         }
      }
      return o;
   }

   GrimmerChains(const S6& s6) : m_s6(s6), m_hasChainFailure(false) {}

   void updateChains(MapOFDeloneFits& deloneFits, MapOfBravaisFits& bravaisFits) {
      m_deloneFits = deloneFits;
      m_bravaisFits = bravaisFits;
      initializeChains();
   }

   void updateFits(MapOFDeloneFits& deloneFits, MapOfBravaisFits& bravaisFits);
   void CreateGrimmerChains(MapOFDeloneFits& theDeloneMap, MapOfBravaisFits& theBravaisMap);
   void CheckAllGrimmerChains();
   bool HasFailure() const { return m_hasChainFailure; }
   GrimmerChainFailure GetFirstFailure() const { return m_GrimmerFailures[0]; }
   DeloneFitResults Remediation(const std::string& bravaisName, const double deltaFromZeroAllowed);
   std::vector<OneGrimmerChain> GetChains() const { return m_GrimmerChains; }
   GrimmerChains ReplaceRemediation(const DeloneFitResults& newFit) const;

   friend std::ostream& operator<< (std::ostream& o, const GrimmerChains& obc);

private:
   void initializeChains();
   void PopulateChainsWithFitValues();
   void BestInBravaisType(const DeloneTypeForGrimmer& type) const;
};

#endif // GRIMMERTREE_H

