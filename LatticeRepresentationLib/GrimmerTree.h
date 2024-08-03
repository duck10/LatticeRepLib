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

class MapOfBravaisFits{
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
   GrimmerChainFailure CheckOneGrimmerChain() ;
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

   GrimmerChains(const S6& s6)
      : m_s6(s6)
      , m_hasChainFailure(false)
   {}

   GrimmerChains ReplaceRemediation(const DeloneFitResults& newFit) const;

   std::map<std::string, double> CreateMapOfDeloneFitValues(const std::vector<DeloneFitResults>& fits);
   std::map<std::string, double> CreateMapOfBravaisFitValues(const std::vector<DeloneFitResults>& fits);

   void  CreateGrimmerChains(MapOFDeloneFits& theDeloneMap, MapOfBravaisFits& theBravaisMap);
   std::vector<OneGrimmerChain> GetChains() const { return m_GrimmerChains; }
   void BestInBravaisType(const DeloneTypeForGrimmer& type) const;
   void CheckAllGrimmerChains();
   bool HasFailure() const { return m_hasChainFailure; }
   GrimmerChainFailure GetFirstFailure() const { return m_GrimmerFailures[0]; }
   DeloneFitResults Remediation(const std::string& bravaisName, const double deltaFromZeroAllowed);

private:
   void PopulateChainsWithFitValues(std::map<std::string, double>& themap);
   std::vector<OneGrimmerChain> m_GrimmerChains;
   std::vector<GrimmerChainFailure> m_GrimmerFailures;
   bool m_hasChainFailure;
   double m_fit;
   S6 m_s6;

   BravaisTypeForGrimmer m_btype_aP;
   BravaisTypeForGrimmer m_btype_mP;
   BravaisTypeForGrimmer m_btype_mC;
   BravaisTypeForGrimmer m_btype_oF;
   BravaisTypeForGrimmer m_btype_oI;

   BravaisTypeForGrimmer m_btype_oS;
   BravaisTypeForGrimmer m_btype_oP;
   BravaisTypeForGrimmer m_btype_hR;
   BravaisTypeForGrimmer m_btype_tI;
   BravaisTypeForGrimmer m_btype_tP;

   BravaisTypeForGrimmer m_btype_cI;
   BravaisTypeForGrimmer m_btype_cF;
   BravaisTypeForGrimmer m_btype_cP;
   BravaisTypeForGrimmer m_btype_hP;
};

#endif // GRIMMERTREE_H
