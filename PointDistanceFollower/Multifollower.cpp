#include "MultiFollower.h"

#include <vector>

#include "D7Dist.h"
#include "Delone.h"
#include "LRL_MinMaxTools.h"
#include "LRL_Path.h"
#include "NCDist.h"
#include "Niggli.h"
#include "D7.h"
#include "G6.h"
#include "S6.h"
#include "S6Dist.h"

MultiFollower::MultiFollower(const std::vector<std::pair<S6, S6> > & inputPath, const std::vector<std::pair<S6, S6> >& secondPath)
   : m_s6path(inputPath, secondPath)
   , m_g6path(Converter<G6, std::vector<std::pair<G6, G6> >, Niggli>(inputPath), Converter<G6, std::vector<std::pair<G6, G6> >, Niggli>(secondPath))
   , m_d7path(Converter<D7, std::vector<std::pair<D7, D7> >, Delone>(inputPath), Converter<D7, std::vector<std::pair<D7, D7> >, Delone>(secondPath))
   , m_cspath(inputPath, secondPath)
{
}

LRL_Path<S6> MultiFollower::GetS6(void) const {
   return m_s6path;
}

LRL_Path<G6> MultiFollower::GetG6(void) const {
   return m_g6path;
}

LRL_Path<D7> MultiFollower::GetD7(void) const {
   return m_d7path;
}

LRL_Path<S6> MultiFollower::GetCS(void) const {
   return m_cspath;
}

MultiFollower MultiFollower::CalculateDistancesS6(const MultiFollower& mf) const {
   MultiFollower m(mf);
   S6 out;
   std::vector<double> vdist;
   const std::vector<std::pair<S6, S6> > path(mf.GetS6().GetPath());
   const std::vector<std::pair<S6, S6> > secondPath(mf.GetS6().GetSecondPath());
   S6Dist s6dist(1);
   if (mf.GetS6().HasSecondPath()) {
      for (unsigned long i = 0; i < path.size(); ++i) {
         vdist.push_back(s6dist.DistanceBetween(path[0].second, secondPath[i].second));
         const std::pair<double, std::pair<S6, S6> > p_report(s6dist.GetBestPosition());
         //std::cout << p_report.first << "        " << p_report.second.first << "        " << p_report.second.second << std::endl;
      }
   }
   else
   {
      for (unsigned long i = 0; i < path.size(); ++i) {
         vdist.push_back(s6dist.DistanceBetween(path[0].second, path[i].second));
         const std::pair<double, std::pair<S6, S6> > p_report(s6dist.GetBestPosition());
         //std::cout << p_report.first << "        " << p_report.second.first << "        " << p_report.second.second << std::endl;
      }
   }
   const std::pair<double, std::pair<S6, S6> > p_report(s6dist.GetBestPosition());
   m.SetDistancesS6(vdist);
   return m;
}

MultiFollower MultiFollower::CalculateDistancesG6(const MultiFollower& mf) const {
   MultiFollower m(mf);
   G6 out;
   std::vector<double> vdist;
   const std::vector<std::pair<G6, G6> > path(mf.GetG6().GetPath());
   const std::vector<std::pair<S6, S6> > secondPath(mf.GetS6().GetSecondPath());
   if (mf.GetS6().HasSecondPath()) {
      for (unsigned long i = 0; i < path.size(); ++i)
         vdist.push_back(NCDist(path[0].second.data(), secondPath[i].second.data()));
   }
   else
   {
      for (unsigned long i = 0; i < path.size(); ++i)
         vdist.push_back(NCDist(path[0].second.data(), path[i].second.data()));
   }
   m.SetDistancesG6(vdist);
   return m;
}

MultiFollower MultiFollower::CalculateDistancesD7(const MultiFollower& mf) const {
   MultiFollower m(mf);
   D7 out;
   std::vector<double> vdist;
   const std::vector<std::pair<D7, D7> > path(mf.GetD7().GetPath());
   const std::vector<std::pair<S6, S6> > secondPath(mf.GetS6().GetSecondPath());
   if (mf.GetS6().HasSecondPath()) {
      for (unsigned long i = 0; i < path.size(); ++i)
         vdist.push_back(CS6Dist(path[0].second.data(), secondPath[i].second.data()));
   }
   else
   {
      for (unsigned long i = 0; i < path.size(); ++i)
         vdist.push_back(CS6Dist(path[0].second.data(), path[i].second.data()));
   }
   m.SetDistancesD7(vdist);
   return m;
}


MultiFollower MultiFollower::CalculateDistancesCS(const MultiFollower& mf) const {
   MultiFollower m(mf);
   S6 out;
   std::vector<double> vdist;
   const std::vector<std::pair<S6, S6> > path(mf.GetCS().GetPath());
   const std::vector<std::pair<S6, S6> > secondPath(mf.GetS6().GetSecondPath());
   if (mf.GetS6().HasSecondPath()) {
      for (unsigned long i = 0; i < path.size(); ++i)
         vdist.push_back(CS6Dist(path[0].second.data(), secondPath[i].second.data()));
   }
   else
   {
      for (unsigned long i = 0; i < path.size(); ++i)
         vdist.push_back(CS6Dist(path[0].second.data(), path[i].second.data()));
   }
   m.SetDistancesCS(vdist);
   return m;
}

MultiFollower MultiFollower::GenerateAllDistances(void) const {
   MultiFollower m(*this);
   m.SetComputeStartTime();
   m = CalculateDistancesS6(m);
   //m = CalculateDistancesG6(m);
   //m = CalculateDistancesD7(m);
   m = CalculateDistancesCS(m);
   m.SetTime2ComputeFrame( std::clock() - m.GetComputeStartTime());

   m.GetPathS6().SetGlitches(m.DetermineOutliers(m.GetPathS6().GetDistances()));
   m.GetPathG6().SetGlitches(m.DetermineOutliers(m.GetPathG6().GetDistances()));
   m.GetPathD7().SetGlitches(m.DetermineOutliers(m.GetPathD7().GetDistances()));
   m.GetPathCS().SetGlitches(m.DetermineOutliers(m.GetPathCS().GetDistances()));

   return m;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const std::set<unsigned long> MultiFollower::DetermineIfSomeDeltaIsTooLarge(const std::vector<double>& distances) const
/*-------------------------------------------------------------------------------------*/
{
   const double maximumDistance = *std::max_element(distances.begin(), distances.end());
   std::set<unsigned long> glitches(DetermineOutliers(distances));

   return(glitches);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::set<unsigned long> MultiFollower::DetermineOutliers(const std::vector<double> distanceList) const
/*-------------------------------------------------------------------------------------*/
{
   std::set<unsigned long> glitches;
   if (distanceList.empty()) return glitches;
   OutlierFinder of(distanceList);
   std::vector<std::pair<double, double> > steps = of.FindDiscontinuities(FollowerConstants::globalPercentChangeToDetect);

   for (unsigned long i = 0; i < steps.size(); ++i) {
      glitches.insert(steps[i].first + 1UL);
   }
   return glitches;
}

template<typename T>
std::pair<double,double> GetPathMinMax( const LRL_Path<T>& path ) {
   std::pair<double, double> p(std::make_pair(DBL_MAX, 0));
   if ( !path.GetDistances().empty()) {
      p = std::make_pair(path.GetMin(), path.GetMax());
   }
   return p;
}

std::pair<double, double> MultiFollower::GetMinMax(void) const {
   std::pair<double, double> p;
   const std::pair<double, double> pS6 = GetPathMinMax(m_s6path);
   const std::pair<double, double> pG6 = GetPathMinMax(m_g6path);
   const std::pair<double, double> pD7 = GetPathMinMax(m_d7path);
   const std::pair<double, double> pCS = GetPathMinMax(m_cspath);

   const double minp = minNC(pS6.first, pG6.first, pD7.first, pCS.first);
   const double maxp = maxNC(pS6.second, pG6.second, pD7.second, pCS.second);
   return std::make_pair(minp,maxp);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const double MultiFollower::GetTime2ComputerFrame(void) const {
   return(m_seconds2ComputerFrame);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void MultiFollower::SetTime2ComputeFrame(const double computeTime) {
   m_seconds2ComputerFrame = computeTime;
}

bool MultiFollower::HasGlitches(void) const {
   return m_cspath.HasGlitches() || m_d7path.HasGlitches() || m_s6path.HasGlitches() || m_g6path.HasGlitches();
}