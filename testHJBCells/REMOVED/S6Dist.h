#ifndef S6DIST_H
#define S6DIST_H

#include <vector>

#include "S6.h"

class S6Dist {
public:
   S6Dist(const double dnearzero = 1.0);

   double DistanceBetween(const S6& d1, const S6& d2);
   double DistanceBetween1(const S6& d1, const S6& d2);
   double DistanceBetween2(const S6& d1, const S6& d2);
   void OneBoundaryDistance(const S6& s1, const S6& s2);
   void TwoBoundaryDistance(const S6& s1, const S6& s2);
   std::vector<S6> ReduceIfLessThanDmin(const double dmin, const S6 s) const;
   static std::vector<S6> Create_VCP_ForTwoScalars(const S6& s);
   static S6 Create_VCP_ForOneScalar(const size_t n, const S6& s);
   std::vector<S6> Create_VCP_s(const S6& s);


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

   S6 ApplyReductionFunction(const size_t n, const S6& d) const;
   S6 ApplyUnreduceFunction(const size_t n, const S6& d) const;

   std::pair<double, size_t> MinForListOfS6(const std::vector<S6>& v1, const std::vector<S6>& v2);
   std::pair<double, size_t> MinForListOfS6(const S6& d1, const std::vector<S6>& v);
   void SetDebug(const bool b) { m_debug = b; }

   void UnreduceAndAddToList(const S6& d, const size_t n, std::vector<S6>& v) const;
   std::vector<S6> UnreduceAndAddToList(const std::vector<S6>& v6, const size_t n) const;

   std::vector<S6> UnreduceAndAddToList(const S6& s6, const size_t n) const;
   std::vector<S6> GenerateReflectionsAtZero(const S6& s6) const;

   std::vector<S6> ResetNearZeroAndAddToList(const std::vector<S6>& v6, const size_t n) const;
   std::vector<S6> ResetNearZeroAndAddToList(const S6& s6) const;

   static const std::vector<S6> Generate24Reflections(const S6& s6in);
   static const std::vector<S6> Generate24Reflections(const std::vector<S6>& vin);
   std::string GetName(void) const { return "S6Dist"; }

private:

   double m_nearzero;
   double m_dmin;
   bool m_debug;

   static std::vector< S6(*)(const S6&)> m_reductionFunctions;
   static std::vector< S6(*)(const S6&)> m_UnReduceFunctions;
   static std::vector< S6(*)(const S6&)> m_reflectionFunctions;

};
#endif // S6DIST_H


