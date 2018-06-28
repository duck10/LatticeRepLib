#ifndef S6DIST_H
#define S6DIST_H

#include <ostream>
#include <vector>

#include "MatB4.h"
#include "MatS6.h"
#include "MatD7.h"
#include "MatG6.h"
#include "S6.h"
#include "TNear.h"

class S6Dist {
public:
   S6Dist(const double dnearzero = 1.0);

   double DistanceBetween(const S6& d1, const S6& d2);
   static double new_DistanceBetween(const S6& d1, const S6& d2);
   void OneBoundaryDistance(const S6& s1, const S6& s2);
   static S6 Create_VCP_ForOneScalar(const unsigned long n, const S6& s);
   static std::vector<S6> Create_VCP_s(const S6& s);
   static std::vector<S6> Create_VCP_s(const std::vector<S6>& s);
   static std::vector<S6> CreateSecondBoundary_VCP_s(const S6& s);
   static std::vector<std::pair<MatS6, MatS6> > SetunreductionReductionMatricesFromReductionMatrices();
   static std::vector<std::pair<MatS6, MatS6> > SetUnreductionMatrices();
   static std::vector< std::pair<S6(*)(const S6&), S6(*)(const S6&)> > SetUnreduceFunctionPairs();
   void SetReduceFunctions();
   void SetUnreduceFunctions();
   static S6 Reduce11(const S6& din);
   static S6 Reduce12(const S6& din);
   static S6 Reduce21(const S6& din);
   static S6 Reduce22(const S6& din);
   static S6 Reduce31(const S6& din);
   static S6 Reduce32(const S6& din);
   static S6 Reduce41(const S6& din);
   static S6 Reduce42(const S6& din);
   static S6 Reduce51(const S6& din);
   static S6 Reduce52(const S6& din);
   static S6 Reduce61(const S6& din);
   static S6 Reduce62(const S6& din);
   static S6 Unreduce11(const S6& din);
   static S6 Unreduce12(const S6& din);
   static S6 Unreduce21(const S6& din);
   static S6 Unreduce22(const S6& din);
   static S6 Unreduce31(const S6& din);
   static S6 Unreduce32(const S6& din);
   static S6 Unreduce41(const S6& din);
   static S6 Unreduce42(const S6& din);
   static S6 Unreduce51(const S6& din);
   static S6 Unreduce52(const S6& din);
   static S6 Unreduce61(const S6& din);
   static S6 Unreduce62(const S6& din);

   std::pair< double, std::pair<S6, S6> > GetBestPosition() const;

   S6 ApplyReductionFunction(const unsigned long n, const S6& d) const;
   S6 ApplyUnreduceFunction(const unsigned long n, const S6& d) const;

   std::pair<double, unsigned long> MinForListOfS6(const std::vector<S6>& v1, const std::vector<S6>& v2);
   std::pair<double, unsigned long> MinForListOfS6(const S6& d1, const std::vector<S6>& v);
   std::pair<double, unsigned long> MinForListOfS6(const std::vector<S6>& v1, const CNearTree<S6>& tree);
   void SetDebug(const bool b) { m_s6Debug = b; }

   static const std::vector<S6> Generate24Reflections(const S6& s6in);
   static const std::vector<S6> Generate24Reflections(const std::vector<S6>& vin);
   static std::string GetName(void) { return "S6Dist"; }
   static std::string ReportS6Best(const S6Dist& s6dist);

private:
   static std::vector<MatB4> vB4_Refl;
   static std::vector<MatD7> vD7_Refl;
   static std::vector<MatG6> vG6_Refl;

   double m_nearzero;
   double m_dmin;
   bool m_s6Debug;

   static std::vector< S6(*)(const S6&)> m_reductionFunctions;
   static std::vector< S6(*)(const S6&)> m_UnReduceFunctions;
   static std::vector< S6(*)(const S6&)> m_reflectionFunctions;

public:
   class Tracker {
   public:
      Tracker() {}
      Tracker( const std::string& label, const S6& s6) : m_label(label), m_s6(s6) {}
      Tracker(const Tracker& t) : m_label(t.m_label), m_s6(t.m_s6) {}
      Tracker& operator= (const Tracker& t) { m_label = t.m_label; m_s6 = t.m_s6; }
      S6 GetVector(void) const { return m_s6; }
      std::string GetLabel(void)const { return m_label; }

      friend std::ostream& operator<< (std::ostream& o, const Tracker& tr) {
         o << tr.m_label << "   " << tr.m_s6;
         return o;
      }

   private:
      std::string m_label;
      S6 m_s6;
   };

   class TrackerHistory {
   public:
      TrackerHistory(void) { m_vtrak.reserve(200); }
      void Add(const Tracker& t) {
         m_vtrak.push_back(t);
      }
      Tracker GetItem(const unsigned long n) { return m_vtrak[n]; }

      Tracker operator[] (const unsigned long n) { return m_vtrak[n]; }

      size_t size() const { return m_vtrak.size(); }


      friend std::ostream& operator<< (std::ostream& o, const TrackerHistory& th) {
         for (unsigned long i = 0; i < th.m_vtrak.size(); ++i)
            o << th.m_vtrak[i] << std::endl;
         return o;
      }

   
   private:
      std::vector<Tracker> m_vtrak;
   };

};
#endif // S6DIST_H


