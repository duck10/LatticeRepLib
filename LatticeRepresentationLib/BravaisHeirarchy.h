#ifndef BRAVAISHEIRARCHY_H
#define BRAVAISHEIRARCHY_H

#include "C3.h"
#include "DeloneFitResults.h"
#include "LRL_ReadLatticeData.h"
#include "S6.h"
#include "S6BoundaryTransforms.h"

#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include <utility>

class BravaisChainFail {
public:
   friend std::ostream& operator<< (std::ostream& o, const BravaisChainFail& bcf) {
      o << "; #### Bravais chain failure\n";
      o << ";  " << bcf.m_minus.first << " " << bcf.m_minus.second << "\n";
      o << ";  " << bcf.m_hit.first << " " << bcf.m_hit.second << "\n";
      o << ";  " << bcf.m_plus.first << " " << bcf.m_plus.second << "\n";

      std::cout << std::endl;
      return o;
   }

   BravaisChainFail
   (
      const std::pair<std::string, double>& minus,
      const std::pair<std::string, double>& hit,
      const std::pair<std::string, double>& plus)
      : m_minus(minus)
      , m_hit(hit)
      , m_plus(plus)
   { }

   void SetDescription(const std::string s) {
      m_description = s;
   }

   std::pair<std::string, double> GetPlus()  const { return m_plus; }
   std::pair<std::string, double> GetHit()   const { return m_hit; }
   std::pair<std::string, double> GetMinus() const { return m_minus; }

private:
   std::pair<std::string, double> m_minus;
   std::pair<std::string, double> m_hit;
   std::pair<std::string, double> m_plus;
   std::string m_description;
};

class BravaisChainFailures {
public:
   friend std::ostream& operator<< (std::ostream& o, const BravaisChainFailures& bcf) {
      o << "; #### Bravais chain failure\n";
      for (size_t i = 0; i < bcf.m_failList.size(); ++i)
      {
         o << ";  " << bcf.m_failList[i] << "\n";
      }
      o << std::endl;
   return o;
   }

   BravaisChainFailures() {}

   void insert
   (
      const std::pair<std::string, double>& minus,
      const std::pair<std::string, double>& hit,
      const std::pair<std::string, double>& plus)
   {
      m_failList.emplace_back(BravaisChainFail{ minus,hit,plus });
   }

   void insert(const BravaisChainFail& bf) {
      m_failList.emplace_back(bf);
   }

   static int countC3Zeros(const C3& c) {
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

   static int countC3Zeros(const C3& c, const double upperLimit) {
      int count = 0;
      const double c3norm = c.norm();
      const double c1 = abs(c[0]);
      const double c2 = abs(c[1]);
      const double c3 = abs(c[2]);
      if (c1 / c3norm < 1.01*upperLimit) ++count;
      if (c2 / c3norm < 1.01*upperLimit) ++count;
      if (c3 / c3norm < 1.01*upperLimit) ++count;
      return count;
   }

   static int CountS6Zeros(const S6& s6 ) {
      int count = 0;
      const double s6norm = s6.norm();
      for (size_t i = 0; i < 6; ++i) {
         if (abs(s6[i]) <= 0.0) ++count;
      }
      return count;
   }

   static int CountS6Zeros(const S6& s6, const double upperLimit) {
      int count = 0;
      const double s6norm = s6.norm();
      for (size_t i = 0; i < 6; ++i) {
         if (abs(s6[i]) <= 1.01 * upperLimit) ++count;
      }
      return count;
   }

   void Remediation()
   {
      if (m_failList.empty()) return;
      const double upper = getFailList()[0].GetPlus().second;
      std::cout << "; cell in BravaisChainFailures::Remediation " << GetS6() << std::endl;
      const C3 c3v(GetS6());
      const int n = countC3Zeros(c3v, upper);
      if (n == 0) {
         std::cout << "; in  BravaisChainFailures::Remediation, "
            "no C3 zeros case is not implemented" << std::endl;
         return;
      }
      else if (n <1 ) {
         std::cout << "; in  BravaisChainFailures::Remediation, "
            "no C3 zeros >1 is not implemented" << std::endl;
         return;
      }

      // if we get to here, we have a case of a single (s1/s4) or (s2/s5)
      // or (s3/s6) pair that is essentially zero. Find a single other 
      // near zero to fiddle. For now, multiples will be not implemented.
      else if (CountS6Zeros(GetS6(), upper) != 3) {
         std::cout << "; multiple S6 zeros that is not implemented" << std::endl;
         return;
      }

      // Get the upper bound on the allowed fit of the failing value.
   
      const auto xxx = S6BoundaryTransforms();

   }

   std::vector< BravaisChainFail> getFailList() const { return m_failList; }

   size_t size() const { return m_failList.size(); }
   bool empty() const { return m_failList.size() == 0; }
   void SetS6(const S6& s6) { m_s6 = s6; }
   S6 GetS6() const { return m_s6; }
   //void SetStrCell(const std::string& s) { m_strCell = s; }
   //std::string GetStrCell() const { return m_strCell; }

private:
   std::vector< BravaisChainFail> m_failList;
   S6 m_s6;
   //std::string m_strCell;
};

class BravaisHeirarchy {
public:
   BravaisHeirarchy();
   static std::string CheckOneBravaisChain(
      const std::vector<std::string>& bravaisChain,
      const std::vector<DeloneFitResults>& v,
      std::map<std::string, double>& valueMap,
      std::vector<std::string>& errorList);

   static std::string CheckBravaisChains(const std::vector<DeloneFitResults>& v);
   static std::vector<std::vector<std::string> > CreateBravaisChains();
   static std::map<std::string, double> GetBestOfEachBravaisType(
      const std::vector<DeloneFitResults>& vDeloneFitResults);
   static std::map<std::string, DeloneFitResults>  CreateMapForBestExamples(
      const std::vector<DeloneFitResults>& vDeloneFitResults);

   static std::string ProduceSVG(
      const LRL_ReadLatticeData& input, const S6& reducedCell,
      const std::vector<std::pair<std::string, double> >& scores);
   static std::string BoilerPlate_1();
   static std::string BoilerPlate_2();
   static std::string ScoreLabels(const std::vector<std::pair<std::string, double> >& scores);

};

#endif // BRAVAISHEIRARCHY_H