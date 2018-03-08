#if _MSC_VER
#pragma once
#endif

#ifndef FOLLOW_H
#define FOLLOW_H


#include "B4.h"
#include "CellInputData.h"
#include "Delone.h"
//#include "D7Dist.h"
#include "D7Dist_.hpp"
#include "FollowerConstants.h"
#include "Glitch.h"
#include "LRL_Cell.h"
#include "NCDist_.hpp"
#include "LRL_MinMaxTools.h"
#include "Niggli.h"
#include"OutlierFinder.h"
#include "Rn_Path.h"
#include "S6Dist.h"
#include "Selling.h"
#include "triple.h"

#include <algorithm>

#include <cassert>
#include <list>
#include <typeinfo>
#include <utility>


template<typename TVEC, typename TMAT, typename TREDUCE>
class Follow
{
public:
   Follow(const TVEC& v1, const TVEC& v2)
      : m_steps(0)
      , m_rnPath()
      , m_whichComponentsToPlot(-1, -1)
      , m_seconds2ComputerFrame(-1.0)
      , m_glitches()
      /*-------------------------------------------------------------------------------------*/
   {
      m_rnPath.AddProbe(v1);
      m_rnPath.AddSecondProbe(v2);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   TVEC GetProbe(void) const {
      return(m_rnPath.GetProbe());
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   TVEC GetSecondProbe(void) const {
      return(m_rnPath.GetSecondProbe());
   }


   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   // walk thru a set of perturbed vectors
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   Follow& Procession(void) {
      const double tend(double(m_steps - 1L));

      TVEC probe;
      probe = m_rnPath.GetProbe();
      G6 niggliReduced_1;
      niggliReduced_1 = m_rnPath.GetSecondProbe();
      G6 test;
      const bool bG6 = Niggli::Reduce(niggliReduced_1, test);
      if (!bG6) throw;
      S6 s6DeloneReduced_1;
      const bool testtest = TREDUCE::Reduce(S6(niggliReduced_1), s6DeloneReduced_1);   // for Delone::Reduce

      S6Dist s6dist(1.0);
      for (int istep = 0; istep < m_steps; ++istep) {
         //if (std::abs(istep - 516) > 1 && istep >0) continue;
         const double t(double(istep) / tend);
         TVEC vNext;
         vNext = TVEC(TVEC((1.0 - t)*probe) + TVEC(t*niggliReduced_1));
         G6 g6vNext(vNext);
         G6 g6NiggliReduced_2;
         S6 s6DeloneReduced_2;
         MatG6 mG6;
         MatS6 mS6;
         const bool b6 = TREDUCE::Reduce(S6(g6vNext), s6DeloneReduced_2);   // for Niggli::Reduce
         //const double distG6 = NCDist_<double[6], double[6] >(g6NiggliReducedArray_1, g6NiggliReducedArray_2);
         if (b6) {
            const double distS6 = s6dist.DistanceBetween(s6DeloneReduced_1, s6DeloneReduced_2);
            //const double distS6_2 = s6dist.DistanceBetween2(s6DeloneReduced_1, s6DeloneReduced_2);

            m_rnPath.AddDistance(distS6);
            //m_rnPath.AddDeloneDistance(distS6_2);
         }
         else {
            std::cout << "Reduce1 failed in Procession " << vNext << std::endl;
         }

         const bool b7 = false;
         if (b7) {
            double distD7 = D7Dist_<D7, D7>(D7(s6DeloneReduced_1), D7(s6DeloneReduced_2));
            m_rnPath.AddDeloneDistance(distD7);
         }
         //else {
         //   std::cout << "Reduce2 failed in Procession " << vNext << std::endl;
         //}
         m_rnPath.Add(vNext, TVEC(s6DeloneReduced_2));
      }
      return(*this);
   }


unsigned long size(void) const { return(m_rnPath.size()); };

bool empty(void)  const { return(m_rnPath.empty()); }

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Set the number of steps from the perturbed vector to the reduced one
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Follow& SetStepCount(const unsigned long count) {
   assert(count > 4);
   this->m_steps = count;
   return(*this);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
Follow& SetWhichComponentsToPlot(const int c1, const int c2) {
   m_rnPath.SetWhichComponentsToPlot(c1, c2);
   m_whichComponentsToPlot = std::make_pair(c1, c2);
   return(*this);
}


const std::vector<triple<double, double, double> >                      GetLinesFromAngles(void) const { return(m_rnPath.GetAngleData()); };
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const std::list<double> GetDistances(void) const {
   const std::list<double> distances(m_rnPath.GetDistances());
   assert(!distances.empty());
   return(distances);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const std::list<double> GetDeloneDistances(void) const {
   const std::list<double> distances(m_rnPath.GetDeloneDistances());
   //assert(!distances.empty());
   return(distances);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const std::pair< std::pair<double, double>, std::pair<double, double> > GetLimits(void)
/*-------------------------------------------------------------------------------------*/
{
   return(m_rnPath.GetLimits());
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Get the number of steps from the perturbed vector to the reduced one
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const double GetCirclePlotDiagonal(void) {
   return(m_rnPath.empty() ? 0.0 : m_rnPath.GetCirclePlotDiagonal());
}

const std::pair<int, int> GetWhichToPlot(void) const { return(m_whichComponentsToPlot); }

std::string GetBoundaryString(const unsigned long n) const {
   if (typeid(TVEC) == typeid(G6))
      return(m_rnPath.G6GetBoundaryString(n));
   else if (typeid(TVEC) == typeid(D7))
      return(m_rnPath.D7GetBoundaryString(n));
   else
      return "";
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const std::vector<TVEC> GetProbeList(void) const {
   return(m_rnPath.GetProbeList());
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const std::vector<TVEC> GetSecondProbeList(void) const {
   return(m_rnPath.GetSecondProbeList());
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const double GetTime2ComputerFrame(void) const {
   return(m_seconds2ComputerFrame);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const std::vector<Glitch<TVEC> > GetGlitches(void) const {
   return(m_glitches);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::vector<Glitch<TVEC> > DetermineOutliers(const std::vector<double> distanceList, const std::pair<double, double>& minMaxDeltaDistance)
/*-------------------------------------------------------------------------------------*/
{
   OutlierFinder of(distanceList);
   std::vector<std::pair<double, double> > steps = of.FindDiscontinuities(FollowerConstants::globalPercentChangeToDetect);
   m_glitches.clear();

   for (unsigned long i = 0; i < steps.size(); ++i) {
      const unsigned long klow = (unsigned long)(steps[i].first);
      const unsigned long khigh = klow + 1UL;
      const std::vector<TVEC> probes(GetProbeList());
      if (!probes.empty()) {
         const GlitchElement<TVEC> Glitch1(distanceList[khigh], khigh, probes[khigh]);
         const GlitchElement<TVEC> Glitch2(distanceList[klow], klow, probes[klow]);
         m_glitches.push_back(Glitch<TVEC>(Glitch1, Glitch2));
      }
   }

   return(m_glitches);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
// Essentially what is going on here is to identify the straight line segments and 
// where the breaks are.
std::vector<long> IdentifyRunsOfAngle(const std::vector<triple<double, double, double> >& angles)
/*-------------------------------------------------------------------------------------*/
{
   const unsigned long numAngles = (unsigned long)(angles.size());
   std::vector<long> runs(1L, 1);

   const std::vector<long> newruns = AssignRunOrdinalToEachPoint(angles);
   runs.insert(runs.end(), newruns.begin(), newruns.end());

   std::vector<long> numberOfPointsInRun(numAngles + 1, 0);

   // this is slow but easy to code
   // For each run of same slope, store the number of points in that run.
   for (unsigned long iangle = 1; iangle < numAngles; ++iangle)
      ++numberOfPointsInRun[runs[iangle]];
   std::vector<long> countOfItemsInThisRun;

   // Enumerate for each circle, how many total circles there are in its run.
   // If there is only one, then we will need to draw a line and add labels.
   for (unsigned long i = 0; i < angles.size(); ++i)
      countOfItemsInThisRun.push_back(numberOfPointsInRun[runs[i]]);

   return(countOfItemsInThisRun);
}


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::vector<long> AssignRunOrdinalToEachPoint(const std::vector<triple<double, double, double> >& angles)
/*-------------------------------------------------------------------------------------*/
{
   // For every point (except the first) store an ordinal of which run of slope
   // each point has.
   int runCount = 0;
   std::vector<long> runs;
   const unsigned long numAngles = (unsigned long)(angles.size());

   for (unsigned long i = 1; i < numAngles; ++i) {
      runCount += IncrementRunCount(angles[i].first, angles[i - 1].first);
      runs.push_back(runCount);
   }

   return(runs);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static const bool SameSlope(const double angle1, const double angle2) {
   const double pi = 4.0 * std::atan(1.0);
   const double anglediff = std::abs(angle1 - angle2);   // RADIANS
   const double angleDiffMin = minNC(anglediff, std::abs(anglediff - pi), std::abs(anglediff - 2.0*pi));
   return angleDiffMin < 0.001;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const std::vector<Glitch<TVEC> > DetermineIfSomeDeltaIsTooLarge(const std::vector<double>& distancesList)
/*-------------------------------------------------------------------------------------*/
{
   std::list<double> ld
      for (int i = 0; i < distanceList.size(); ++i) {
         ld.push_back(distanceList[i]);
      }
   return DetermineIfSomeDeltaIsTooLarge(ld);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const std::vector<Glitch<TVEC> > DetermineIfSomeDeltaIsTooLarge(const std::list<double>& distancesList)
/*-------------------------------------------------------------------------------------*/
{
   const std::vector<double> distances(distancesList.begin(), distancesList.end());

   const double maximumDistance = *std::max_element(distances.begin(), distances.end());
   const std::pair<double, double> minmaxDeltaDistance = GetMinMaxDeltaDistances(distances);
   std::vector<Glitch<TVEC> > glitches(DetermineOutliers(distances, minmaxDeltaDistance));

   return(glitches);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
const std::vector<Glitch<TVEC> > DetermineIfSomeDeltaIsTooLarge(void) {
   return(DetermineIfSomeDeltaIsTooLarge(this->GetDistances()));
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void SetTime2ComputeFrame(const double computeTime) {
   m_seconds2ComputerFrame = computeTime;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
static std::pair<double, double> GetMinMaxDeltaDistances(const std::vector<double>& distances) {
   double minDelta = DBL_MAX;
   double maxDelta = -DBL_MAX;

   std::vector<double>::const_iterator it = ++distances.begin();
   double prev = *distances.begin();
   for (; it != distances.end(); ++it) {
      const double absDelta = std::abs((*it) - prev);
      minDelta = std::min(minDelta, absDelta);
      maxDelta = std::max(maxDelta, absDelta);
      prev = *it;
   }

   return(std::make_pair(minDelta, maxDelta));
}
private:
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   int IncrementRunCount(const double& angle_I, const double& angle_IMinusOne) {
      const double anglediff(SameSlope(angle_I, angle_IMinusOne) ? 0.0 : angle_I - angle_IMinusOne);
      return((std::abs(anglediff) > 0.001) ? 1 : 0);
   }



   unsigned long m_steps;
   Rn_Path<TVEC> m_rnPath;
   std::pair<int, int> m_whichComponentsToPlot;
   double m_seconds2ComputerFrame;
   std::vector<Glitch<TVEC> > m_glitches;
};


#endif
