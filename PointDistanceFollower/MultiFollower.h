#ifndef MULTIFOLLOWER_H
#define MULTIFOLLOWER_H

#include <algorithm>
#include <ctime>
#include <list>
#include <set>
#include <utility>
#include <vector>

#include "CS6Dist.h"
#include "DistanceLineDescriptions.h"
#include "FollowerConstants.h"
#include "G6.h"
#include "LRL_Path.h"
#include "S6.h"
#include "D7.h"
#include "OutlierFinder.h"

class CS6 {};

class MultiFollower {
public:
   MultiFollower(){}
   MultiFollower(const std::vector<std::pair<S6, S6> >& inputPath);

   LRL_Path<S6> GetS6(void) const;
   LRL_Path<G6> GetG6(void) const;
   LRL_Path<D7> GetD7(void) const;
   LRL_Path<S6> GetCS(void) const;

   MultiFollower CalculateDistancesS6(const MultiFollower& mf) const;
   MultiFollower CalculateDistancesG6(const MultiFollower& mf) const;
   MultiFollower CalculateDistancesD7(const MultiFollower& mf) const;
   MultiFollower CalculateDistancesCS(const MultiFollower& mf) const;
   MultiFollower GenerateAllDistances(void) const;
   void SetDistancesS6(const std::vector<double>& v) { m_s6path.SetDistances(v); }
   void SetDistancesG6(const std::vector<double>& v) { m_g6path.SetDistances(v); }
   void SetDistancesD7(const std::vector<double>& v) { m_d7path.SetDistances(v); }
   void SetDistancesCS(const std::vector<double>& v) { m_cspath.SetDistances(v); }
   LRL_Path<S6>& GetPathS6(void) { return m_s6path; }
   LRL_Path<G6>& GetPathG6(void) { return m_g6path; }
   LRL_Path<D7>& GetPathD7(void) { return m_d7path; }
   LRL_Path<S6>& GetPathCS(void) { return m_cspath; }
   std::pair<double, double> GetMinMax(void) const;
   unsigned long size(void)const { return m_s6path.size(); }
   const double GetTime2ComputerFrame(void) const;
   void SetTime2ComputeFrame(const double computeTime);
   DistanceLineDescriptions GetLineDescriptions(void) const { return m_lineDescription; }
   void SetComputeStartTime(void) { m_ComputeStartTime = std::clock(); }
   std::clock_t GetComputeStartTime(void) const {return m_ComputeStartTime; }

   std::set<unsigned long> DetermineOutliers(const std::vector<double> distanceList) const;
   const std::set<unsigned long> DetermineIfSomeDeltaIsTooLarge(const std::vector<double>& distances) const;

private:
   LRL_Path<S6>  m_s6path;
   LRL_Path<G6>  m_g6path;
   LRL_Path<D7>  m_d7path;
   LRL_Path<S6> m_cspath;
   std::clock_t m_ComputeStartTime;
   double m_seconds2ComputerFrame;
   std::set<unsigned long> m_glitches;
   DistanceLineDescriptions m_lineDescription;


   template<typename T1, typename T2, typename TREDUCE>
   T2 Converter(const std::vector<std::pair<S6, S6> >& s6path) {
      std::vector<std::pair<T1, T1> > v;
      for (unsigned long i = 0; i < s6path.size(); ++i) {
         const std::pair<S6, S6> p(s6path[i]);
         T1 out;
         const bool b = TREDUCE::Reduce(p.first, out);
         v.push_back(std::make_pair(T1(s6path[i].first), out));
      }
      return v;
   }

   template<typename TVEC>
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   const std::vector<unsigned long> DetermineIfSomeDeltaIsTooLarge(const LRL_Path<TVEC>& path) {
      return(DetermineIfSomeDeltaIsTooLarge(path.GetDistances()));
   }

};

#endif // MULTIFOLLOWER_H

