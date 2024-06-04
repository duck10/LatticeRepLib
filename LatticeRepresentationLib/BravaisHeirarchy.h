#ifndef BRAVAISHEIRARCHY_H
#define BRAVAISHEIRARCHY_H

#include "C3.h"
#include "DeloneFitResults.h"
#include "GenerateLatticeTypeExamples.h"
#include "LRL_ReadLatticeData.h"
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
      if (c1 < 1.01*upperLimit) ++count;
      if (c2 < 1.01*upperLimit) ++count;
      if (c3 < 1.01*upperLimit) ++count;
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

   static int FindLoneS6Zero(const size_t n1, const size_t n2, const S6& s6, const double upper) {
      for (size_t i = 0; i < 6; ++i) {
         if (i == n1 || i == n2) continue;
         if ( abs(s6[i]) < upper)  return int(i);
      }
      return 19191;
   }

   static void PrintWork(const std::vector<MatS6>& vm, const S6& s6) {
      for (const auto& m : vm) {
         std::cout << m << std::endl << std::endl;
      }
      for (const auto& m : vm) {
         std::cout << m * s6 << std::endl;
      }
      for (const auto& m : vm) {
         std::cout << C3(m * s6) << std::endl;
      }
      std::cout << std::endl;
   }

   static void PrintWork(const MatS6& vm, const S6& s6) {
      std::cout << vm * s6 << std::endl << std::endl;
      std::cout << C3(vm * s6) << std::endl << std::endl;
      std::cout << std::endl;
   }

   //void AnalyzeS6(const S6 s6) {

   //   const double average = (abs(s6[0]) + abs(s6[1]) + abs(s6[2]) + abs(s6[3]) + abs(s6[4]) + abs(s6[5]))/s6.size();
   //   std::vector<double> vec(s6.GetVector());
   //   std::sort(vec.begin(), vec.end());

   //   for (size_t i = 0; i < 6; ++i) {
   //      vec[i] = abs(vec[i]);
   //   }
   //   int lowCount = 0;
   //   for (size_t i = 0; i < 6; ++i) {
   //      if (vec[i] < average) ++lowCount;
   //   }
   //   std::cout << "average " << average << std::endl;
   //   std::cout << s6 << std::endl;
   //   std::cout << S6(vec) << std::endl;
   //   std::cout << "lowCount " << lowCount << std::endl;
   //}

   DeloneFitResults Remediation()
   {
      DeloneFitResults dfr;
      if (m_failList.empty()) return dfr;
      dfr.SetRawFit(DBL_MAX);

      //AnalyzeS6(GetS6());

      double lowestValue = DBL_MAX;
      for (size_t i = 0; i < 6; ++i) lowestValue = std::min(lowestValue, abs(GetS6()[i]));
      const double length = GetS6().norm();


      int zeroPosition = -1;
      const auto failList = getFailList();
      const auto xxxx = getFailList()[0].GetPlus().second;
      const auto thePlus = getFailList()[0].GetPlus();
      const double upper = 2.0 * getFailList()[0].GetPlus().second;

      std::cout << "; cell in BravaisChainFailures::Remediation \n" << GetS6() << std::endl;
      std::cout << "; cell in BravaisChainFailures::Remediation \n" << LRL_Cell_Degrees(GetS6()) << std::endl;
      std::cout << "; cell in BravaisChainFailures::Remediation \n" << C3(GetS6()) << std::endl;
      const C3 c3v(GetS6());
      const int nc3 = countC3Zeros(c3v, upper);
      const int ns6 = CountS6Zeros(GetS6(), upper);
      std::cout << ";Remdiation # C3 zeros " << nc3 << std::endl;
      std::cout << ";Remediation # S6 zeros " << ns6 << std::endl;

      std::cout << ";C3 magnitudes " << abs(c3v[0]) << ", " << abs(c3v[1]) << ", " << abs(c3v[2]) << std::endl;



      if (ns6 > 3) {
         std::cout << ";in  BravaisChainFailures::Remediation the case "
            "of one C3 zero and other than 3 S6 zeroes is not implemented" << std::endl;
         return dfr;
      }
      else if (nc3 == 0) {
         std::cout << "; in  BravaisChainFailures::Remediation, "
            "no C3 zeros, case is not implemented" << std::endl;
             std::cout << c3v << std::endl;
        return dfr;
      }
      else if (nc3 > 1) {
         std::cout << "; in  BravaisChainFailures::Remediation, "
            "C3 zeros count >1 is not implemented" << std::endl;
         return dfr;
      }
      else {
         if (abs(c3v[0]) < upper) {
            zeroPosition = FindLoneS6Zero(0, 3, GetS6(), upper);
         }
         else if (abs(c3v[1]) < upper) {
            zeroPosition = FindLoneS6Zero(1, 4, GetS6(), upper);
         }
         else if (abs(c3v[2]) < upper) {
            zeroPosition = FindLoneS6Zero(2, 5, GetS6(), upper);
         }
         else {
            std::cout << ";this is NOT supposed to be possible in in  BravaisChainFailures::Remediation" << std::endl;
            zeroPosition = 19191;
         }
      }

      //std::cout << " upper " << upper << "  lone zero position " << zeroPosition << std::endl;

      S6BoundaryTransforms sbt;
      for ( size_t i=0;i<6;++i)
      {
         const MatS6 matrix = sbt.GetOneTransform((zeroPosition + i)%6);
         std::cout << "; in Remediation, i = " << i << std::endl;

      //std::cout << " all 6 of the reduction matrices " << std::endl;
      //for (size_t i = 0; i < 6; ++i) {
      //   std::cout << "matrix " << i << std::endl;
      //   std::cout << sbt.GetOneTransform(i) << std::endl;
      //}

      ////debugging lc
      //const S6 v{ 0,1,2,3,4,5 };
      //std::cout << " test vector " << v << std::endl;
      //for (const auto& x : xxx) {
      //   const MatS6 xi{ x.Inverse(x) };
      //   std::cout << std::endl;
      //   std::cout << x << std::endl;
      //   std::cout << std::endl;
      //   std::cout << x * v << std::endl;
      //}

      //std::cout << std::endl;
      //std::cout << matrix << std::endl << std::endl;

      PrintWork(matrix, S6(c3v));

      std::pair<std::string, double> plus = getFailList()[0].GetPlus();
      std::pair<std::string, double> hit = getFailList()[0].GetHit();
      std::pair<std::string, double> minus = getFailList()[0].GetMinus();
      plus.first += " ";
      std::cout << "plus " << PairReporter(plus) << " " << std::endl;
      std::cout << "hit " << hit.first << "  " << hit.second << std::endl;
      std::cout << "minus " << PairReporter(minus) << " " << std::endl;
      static const std::vector<std::shared_ptr<GenerateDeloneBase> > sptypes =
         GenerateDeloneBase().Select(hit.first);

      const auto dfrTemp = Sella::SellaFitXXXXXX(sptypes[0], matrix * GetS6());
      if (dfrTemp.GetRawFit() < dfr.GetRawFit()) {
         dfr = dfrTemp;
      }


      std::cout << dfr.GetGeneralType() << " " << dfr.GetRawFit() << std::endl;

      //const S6 modifidedS6A = xxx[zeroPosition][0] * GetS6();
      //const S6 modifidedS6B = xxx[zeroPosition][1] * GetS6();
      //const S6 modifidedS6C = xxx[zeroPosition][2] * GetS6();
      //std::cout << modifidedS6A << std::endl;
      //std::cout << modifidedS6B << std::endl;
      //std::cout << modifidedS6C << std::endl;
      //std::cout << C3(modifidedS6A) << std::endl;
      //std::cout << C3(modifidedS6B) << std::endl;
      //std::cout << C3(modifidedS6C) << std::endl;
      const int i19191 = 19191;
      }
      return dfr;
   }

   std::vector< BravaisChainFail> getFailList() const { return m_failList; }

   size_t size() const { return m_failList.size(); }
   bool empty() const { return m_failList.empty(); }
   void SetS6(const S6& s6) { m_s6 = s6; }
   S6 GetS6() const { return m_s6; }
   //void SetStrCell(const std::string& s) { m_strCell = s; }
   //std::string GetStrCell() const { return m_strCell; }

private:
   std::vector<BravaisChainFail> m_failList;
   S6 m_s6;
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
   static std::map<std::string, double> GetBestOfEachBravaisType(
      const std::vector<DeloneFitResults>& vDeloneFitResults);
   static std::map<std::string, DeloneFitResults>  CreateMapForBestExamples(
      const std::vector<DeloneFitResults>& vDeloneFitResults);

   static std::string ProduceSVG(
      const LRL_ReadLatticeData& input, const S6& reducedCell,
      const std::vector<std::pair<std::string, double> >& scores,
      const std::vector<std::string>& projectedCells);
   static std::string BoilerPlate_1();
   static std::string BoilerPlate_2();
   static std::string ScoreLabels(const std::vector<std::pair<std::string, double> >& scores);
   static std::vector<std::string> FormatProjectedCells(const std::vector<std::string>& s);
};

#endif // BRAVAISHEIRARCHY_H