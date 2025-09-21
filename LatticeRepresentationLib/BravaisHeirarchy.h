#pragma warning (disable: 4100) // Visual Studio --  unreferenced formal parameter

#ifndef BRAVAISHEIRARCHY_H
#define BRAVAISHEIRARCHY_H

#include "C3.h"
#include "DeloneFitResults.h"
//#include "GenerateLatticeTypeExamples.h"
#include "LRL_ReadLatticeData.h"
#include "LatticeCell.h"
#include "PairReporter.h"
#include "S6.h"
#include "S6BoundaryTransforms.h"
#include "Sella.h"

#include <algorithm>
#include <cmath>
#include <map>
#include <string>
#include <vector>
#include <utility>

class BravaisChainFail {
public:
   friend std::ostream& operator<< (std::ostream& o, const BravaisChainFail& bcf) {
      //o << "; #### Bravais chain failure A\n";
      o << ";  " << bcf.m_minus.first << " " << bcf.m_minus.second << "\n";
      o << ";  " << bcf.m_hit.first << " " << bcf.m_hit.second << "\n";
      o << ";  " << bcf.m_plus.first << " " << bcf.m_plus.second << "\n";

      o << std::endl;
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
      //o << "; #### Bravais chain failure\n";
      //for (size_t i = 0; i < bcf.m_failList.size(); ++i)
      //{
      //   o << ";  " << bcf.m_failList[i] << "\n";
      //}
      //o << std::endl;
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

   static int countC3Zeros(const C3& c);

   static int countC3Zeros(const C3& c, const double upperLimit);

   static int CountS6Zeros(const S6& s6 );

   static int CountS6Zeros(const S6& s6, const double upperLimit);

   static int FindLoneS6Zero(const size_t n1, const size_t n2, const S6& s6, const double upper);

   std::vector< BravaisChainFail> getFailList() const;

   size_t size() const { return m_failList.size(); }
   bool empty() const { return m_failList.empty(); }
   void SetS6(const S6& s6) { m_s6 = s6; }
   S6 GetS6() const { return m_s6; }
   DeloneFitResults GetRemediationResult() const { return m_remediationResult; }
   void SetRemediationResult( const DeloneFitResults& result) { m_remediationResult = result; }
   //void SetStrCell(const std::string& s) { m_strCell = s; }
   //std::string GetStrCell() const { return m_strCell; }

private:
   std::vector<BravaisChainFail> m_failList;
   S6 m_s6;
   DeloneFitResults m_remediationResult;
   //std::string m_strCell;
};

class BravaisHeirarchy {
public:
   BravaisHeirarchy();
   static BravaisChainFailures CheckOneBravaisChain(
      const std::vector<std::string>& bravaisChain,
      const std::vector<DeloneFitResults>& v,
      std::map<std::string, double>& valueMap,
      std::vector<std::string>& errorList);

   static std::vector< BravaisChainFailures> CheckBravaisChains(const std::vector<DeloneFitResults>& v);
   static std::vector<std::vector<std::string> > CreateBravaisChains();
   std::vector<std::vector<std::string>> CreateBravaisTrees();
   static std::map<std::string, double> GetBestOfEachBravaisType(
      const std::vector<DeloneFitResults>& vDeloneFitResults);
   static std::map<std::string, DeloneFitResults>  CreateMapForBestExamples(
      const std::vector<DeloneFitResults>& vDeloneFitResults);

   static std::string ProduceSVG(
      const LatticeCell& input, const S6& reducedCell,
      const std::vector<std::pair<std::string, double> >& scores,
      const std::vector<std::string>& projectedCells);
   static std::string BoilerPlate_1();
   static std::string BoilerPlate_2();
   static std::string ScoreLabels(const std::vector<std::pair<std::string, double> >& scores);
   static std::vector<std::string> FormatProjectedCells(const std::vector<std::string>& s);
};

#endif // BRAVAISHEIRARCHY_H