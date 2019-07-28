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
#include "OutlierFinder.h"
#include "S6.h"
#include "S6Dist.h"
#include "V7Dist.h"

MultiFollower::MultiFollower(const std::vector<std::pair<S6, S6> > & inputPath, const std::vector<std::pair<S6, S6> >& secondPath)
   : m_s6path(inputPath, secondPath)
   , m_g6path(Converter<G6, std::vector<std::pair<G6, G6> >, Niggli>(inputPath), Converter<G6, std::vector<std::pair<G6, G6> >, Niggli>(secondPath))
   , m_d7path(Converter<D7, std::vector<std::pair<D7, D7> >, Delone>(inputPath), Converter<D7, std::vector<std::pair<D7, D7> >, Delone>(secondPath))
   , m_cspath(inputPath, secondPath)
   , m_v7path(Converter<G6, std::vector<std::pair<G6, G6> >, Niggli>(inputPath), Converter<G6, std::vector<std::pair<G6, G6> >, Niggli>(secondPath))

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

LRL_Path<G6> MultiFollower::GetV7(void) const {
   return m_v7path;
}

MultiFollower MultiFollower::CalculateDistancesS6(const MultiFollower& mf) const {
   MultiFollower m(mf);
   S6 out;
   std::vector<double> vdist;
   const std::vector<std::pair<S6, S6> > path(mf.GetS6().GetPath());
   const std::vector<std::pair<S6, S6> > secondPath(mf.GetS6().GetSecondPath());
   S6Dist s6dist(1);
   if (! S6::IsInvalidPair(path[0])) {
      if (mf.GetS6().HasSecondPath()) {
         double distance;
         for (size_t i = 0; i < path.size(); ++i) {
            if (S6::IsInvalidPair(path[i]) || S6::IsInvalidPair(secondPath[i]))
               distance = -1.0;
            else
               distance = s6dist.DistanceBetween(path[0].second, secondPath[i].second);
            vdist.push_back(distance);
         }
      }
      else
      {
         if (!S6::IsInvalidPair(path[0])) {
            for (size_t i = 0; i < path.size(); ++i) {
               double distance = s6dist.DistanceBetween(path[0].second, path[i].second);
               if (S6::IsInvalidPair(path[i])) distance = -1.0;
               vdist.push_back(distance);
            }
         }
      }
   }
   m.SetDistancesS6(vdist);
   return m;
}

MultiFollower MultiFollower::CalculateDistancesG6(const MultiFollower& mf) const {
   MultiFollower m(mf);
   G6 out;

   std::vector<double> vdist;
   const std::vector<std::pair<G6, G6> > path(mf.GetG6().GetPath());
   const std::vector<std::pair<G6, G6> > secondPath(mf.GetG6().GetSecondPath());
   /*if (!S6::IsInvalidPair(path[0]))*/ {
      if (mf.GetG6().HasSecondPath()) {
         double distance;
         for (size_t i = 0; i < path.size(); ++i) {
            if (S6::IsInvalidPair(path[i]) || S6::IsInvalidPair(secondPath[i]))
               distance = -1.0;
            else
               distance = NCDist(path[0].second.data(), secondPath[i].second.data());
            vdist.push_back(distance);
         }
      }
      else
      {

         /*if (path[0].second.IsValid())*/ {
            for (size_t i = 0; i < path.size(); ++i) {
               double distance = NCDist(path[0].second.data(), path[i].second.data());
               if (S6::IsInvalidPair(path[i])) distance = -1.0;
               vdist.push_back(distance);
            }
         }
      }
   }
   m.SetDistancesG6(vdist);
   return m;
}

MultiFollower MultiFollower::CalculateDistancesV7(const MultiFollower& mf) const {
   MultiFollower m(mf);
   G6 out;
   std::vector<double> vdist;
   const std::vector<std::pair<G6, G6> > path(mf.GetG6().GetPath());
   const std::vector<std::pair<G6, G6> > secondPath(mf.GetG6().GetSecondPath());
   if (!S6::IsInvalidPair(path[0])) {
      if (mf.GetG6().HasSecondPath()) {
         double distance;
         for (size_t i = 0; i < path.size(); ++i) {
            if (S6::IsInvalidPair(path[i]) || S6::IsInvalidPair(secondPath[i]))
               distance = -1.0;
            else
               distance = V7Dist(path[0].second, secondPath[i].second);
            vdist.push_back(distance);
         }
      }
      else
      {
         if (path[0].second.IsValid()) {
            for (size_t i = 0; i < path.size(); ++i) {
               double distance = V7Dist(path[0].second, path[i].second);
               if (S6::IsInvalidPair(path[i])) distance = -1.0;
               vdist.push_back(distance);
            }
         }
      }
   }
   m.SetDistancesV7(vdist);
   return m;
}

MultiFollower MultiFollower::CalculateDistancesD7(const MultiFollower& mf) const {
   MultiFollower m(mf);
   D7 out;
   std::vector<double> vdist;
   const std::vector<std::pair<D7, D7> > path(mf.GetD7().GetPath());
   const std::vector<std::pair<D7, D7> > secondPath(mf.GetD7().GetSecondPath());
   if (!S6::IsInvalidPair(path[0])) {
      if (mf.GetD7().HasSecondPath()) {
         double distance;
         for (size_t i = 0; i < path.size(); ++i) {
            if (S6::IsInvalidPair(path[i]) || S6::IsInvalidPair(secondPath[i]))
               distance = -1.0;
            else
               distance = D7Dist(path[0].second.data(), secondPath[i].second.data());
            vdist.push_back(distance);
         }
      }
      else
      {
         if (path[0].second.IsValid()) {
            for (size_t i = 0; i < path.size(); ++i) {
               double distance = D7Dist(path[0].second.data(), path[i].second.data());
               if (S6::IsInvalidPair(path[i])) distance = -1.0;
               vdist.push_back(distance);
            }
         }
      }
   }
   m.SetDistancesD7(vdist);
   return m;
}

MultiFollower MultiFollower::CalculateDistancesCS( const MultiFollower& mf) const  {
   MultiFollower m(mf);
   S6 out;
   std::vector<double> vdist;
   const std::vector<std::pair<S6, S6> > path(mf.GetCS().GetPath());
   const std::vector<std::pair<S6, S6> > secondPath(mf.GetCS().GetSecondPath());
   if (!S6::IsInvalidPair(path[0])) {
      if (mf.GetS6().HasSecondPath()) {
         double distance;
         for (size_t i = 0; i < path.size(); ++i) {
            if (S6::IsInvalidPair(path[i]) || S6::IsInvalidPair(secondPath[i])) 
               distance = -1.0;
            else
               distance = CS6Dist(path[0].second.data(), secondPath[i].second.data());
            vdist.push_back(distance);
         }
      }
      else
      {
         if (path[0].second.IsValid()) {
            for (size_t i = 0; i < path.size(); ++i) {
               double distance = CS6Dist(path[0].second.data(), path[i].second.data());
               if (S6::IsInvalidPair(path[i])) distance = -1.0;
               vdist.push_back(distance);
            }
         }
      }
   }
   m.SetDistancesCS(vdist);
   return m;
}

MultiFollower MultiFollower::GenerateAllDistances(void) {
   MultiFollower m(*this);
   m.SetComputeStartTime();
   {
      m.m_v7path.SetComputeStartTime();
      if (FollowerConstants::IsEnabled("V7")) m = CalculateDistancesV7(m);
      const double computetimev7 = std::clock() - m.m_v7path.GetComputeStartTime();
      m.m_v7path.SetTime2ComputeFrame(computetimev7);
      m.SetComputeTime("V7", computetimev7);
   }
   {
      //m.ComputeAndTimeOnePath(m.m_cspath, "CS", &MultiFollower::CalculateDistancesCS);
      m.m_cspath.SetComputeStartTime();
      if (FollowerConstants::IsEnabled("CS")) m = CalculateDistancesCS(m);
      const double computetimecs = std::clock() - m.m_cspath.GetComputeStartTime();
      m.m_cspath.SetTime2ComputeFrame(computetimecs);
      m.SetComputeTime("CS", computetimecs);
   }
   {
      m.m_s6path.SetComputeStartTime();
      if (FollowerConstants::IsEnabled("S6")) m = CalculateDistancesS6(m);
      const double computetimes6 = std::clock() - m.m_s6path.GetComputeStartTime();
      m.m_s6path.SetTime2ComputeFrame(computetimes6);
      m.SetComputeTime("S6", computetimes6);
   }
   {
      m.m_g6path.SetComputeStartTime();
      if (FollowerConstants::IsEnabled("G6")) m = CalculateDistancesG6(m);
      const double computetimeg6 = std::clock() - m.m_g6path.GetComputeStartTime();
      m.m_g6path.SetTime2ComputeFrame(computetimeg6);
      m.SetComputeTime("G6", computetimeg6);
   }
   {
      m.m_d7path.SetComputeStartTime();
      if (FollowerConstants::IsEnabled("D7")) m = CalculateDistancesD7(m);
      const double computetimed7 = std::clock() - m.m_d7path.GetComputeStartTime();
      m.m_d7path.SetTime2ComputeFrame(computetimed7);
      m.SetComputeTime("D7", computetimed7);
   }
   m.SetTime2ComputeFrame(std::clock() - m.GetComputeStartTime());

   m.GetPathS6().SetGlitches(m.DetermineOutliers(m.GetPathS6().GetDistances()));
   m.GetPathG6().SetGlitches(m.DetermineOutliers(m.GetPathG6().GetDistances()));
   m.GetPathD7().SetGlitches(m.DetermineOutliers(m.GetPathD7().GetDistances()));
   m.GetPathCS().SetGlitches(m.DetermineOutliers(m.GetPathCS().GetDistances()));
   m.GetPathV7().SetGlitches(m.DetermineOutliers(m.GetPathV7().GetDistances()));

   return m;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const std::set<size_t> MultiFollower::DetermineIfSomeDeltaIsTooLarge(const std::vector<double>& distances) const
/*-------------------------------------------------------------------------------------*/
{
   const double maximumDistance = *std::max_element(distances.begin(), distances.end());
   std::set<size_t> glitches(DetermineOutliers(distances));

   return(glitches);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::set<size_t> MultiFollower::DetermineOutliers(const std::vector<double> distanceList) const
/*-------------------------------------------------------------------------------------*/
{
   std::set<size_t> glitches;
   if (distanceList.empty()) return glitches;
   OutlierFinder of(distanceList);
   std::vector<std::pair<double, double> > steps = of.FindDiscontinuities(FollowerConstants::globalPercentChangeToDetect);

   for (size_t i = 0; i < steps.size(); ++i) {
      glitches.insert(steps[i].first + 1UL);
   }
   return glitches;
}

template<typename T>
std::pair<double, double> GetPathMinMax(const LRL_Path<T>& path) {
   std::pair<double, double> p(std::make_pair(DBL_MAX, 0));
   if (!path.GetDistances().empty()) {
      p = std::make_pair(path.GetMin(), path.GetMax());
      if (p.first < 0.0) {
         const std::vector<double> distances = path.GetDistances();
         double distmin = DBL_MAX;
         for (size_t i = 0; i < distances.size(); ++i) {
            if (distances[i] > 0.0)
               distmin = std::min(distmin, distances[i]);
         }
         p.first = distmin;
      }
   }
   return p;
}

std::pair<double, double> MultiFollower::GetMinMax(void) const {
   std::pair<double, double> p;
   const std::pair<double, double> pS6 = GetPathMinMax(m_s6path);
   const std::pair<double, double> pG6 = GetPathMinMax(m_g6path);
   const std::pair<double, double> pD7 = GetPathMinMax(m_d7path);
   const std::pair<double, double> pCS = GetPathMinMax(m_cspath);
   const std::pair<double, double> pV7 = GetPathMinMax(m_v7path);

   const double minp = minNC(pS6.first, pG6.first, pD7.first, pCS.first, pV7.first);
   const double maxp = maxNC(pS6.second, pG6.second, pD7.second, pCS.second, pV7.second);
   return std::make_pair(minp, maxp);
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

void MultiFollower::SetComputeTime(const std::string& name, const double time) {
   m_lineDescription.SetComputeTime(name, time);
}