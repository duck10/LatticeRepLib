#ifndef S6DIST_H
#define S6DIST_H

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
   void OneBoundaryDistance(const S6& s1, const S6& s2);
   static S6 Create_VCP_ForOneScalar(const unsigned long n, const S6& s);
   std::vector<S6> Create_VCP_s(const S6& s);
   std::vector<S6> Create_VCP_s(const std::vector<S6>& s);

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

   void UnreduceAndAddToList(const S6& d, const unsigned long n, std::vector<S6>& v) const;
   std::vector<S6> UnreduceAndAddToList(const std::vector<S6>& v6, const unsigned long n) const;

   std::vector<S6> UnreduceAndAddToList(const S6& s6, const unsigned long n) const;
   std::vector<S6> GenerateReflectionsAtZero(const S6& s6) const;

   std::vector<S6> ResetNearZeroAndAddToList(const std::vector<S6>& v6, const unsigned long n) const;
   std::vector<S6> ResetNearZeroAndAddToList(const S6& s6) const;

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

};
#endif // S6DIST_H


