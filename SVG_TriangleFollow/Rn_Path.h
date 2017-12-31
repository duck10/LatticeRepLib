#if _MSC_VER
#pragma once // this is to keep the Visual Studio compiler happy
#endif

#ifndef RN_PATH_H
#define RN_PATH_H

#include <algorithm>
#include <cassert>
#include <cfloat>

#include <list>
#include <string>
#include <vector>


#include "triple.h"

template<typename TVEC>
class Rn_Path
{
public:

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   Rn_Path(void)
      : m_probe()
      , m_secondProbe()
      , m_whichComponentsToPlot(std::make_pair(-1, -1))
      , m_xyMinMax(std::make_pair(std::make_pair(DBL_MAX, DBL_MAX), std::make_pair(DBL_MAX, DBL_MAX)))
      , staticXprev(0.0)
      , staticYprev(0.0)
      /*-------------------------------------------------------------------------------------*/
   {
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   Rn_Path(const std::pair<int, int>& whichComponentsToPlot, const TVEC& probe, const TVEC& reduced)
      : m_probe(probe)
      , m_secondProbe(reduced)
      , m_whichComponentsToPlot(whichComponentsToPlot)
      , m_xyMinMax(std::make_pair(std::make_pair(DBL_MAX, DBL_MAX), std::make_pair(DBL_MAX, DBL_MAX)))
      , staticXprev(0.0)
      , staticYprev(0.0)
      /*-------------------------------------------------------------------------------------*/
   { }
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   ~Rn_Path(void) { }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void AddProbe(const TVEC& probe) {
      m_probe = probe;
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void AddSecondProbe(const TVEC& secondProbe) {
      m_secondProbe = secondProbe;
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void Add(const TVEC& probe, const TVEC& reduced, const TVEC& bestmatch) {
      m_bestMatchList.push_back(bestmatch);
      Add(probe, reduced);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void Add(const TVEC& probe, const TVEC& reduced) {
      if (m_probeList.empty())
      {
         staticXprev = DBL_MAX;
         staticYprev = DBL_MAX;
      }

      m_probeList.push_back(probe);
      m_reducedList.push_back(reduced);

      const double x = reduced[m_whichComponentsToPlot.first];
      const double y = reduced[m_whichComponentsToPlot.second];

      double slope = std::atan2(y - staticYprev, x - staticXprev);
      if (std::fabs(slope) < 1.0E-8 || std::fabs(std::fabs(slope) - 2.0*4.0*std::atan(1.0)) < 0.001) slope = 0.0; // test slope for zero or 2 pi

      const triple<double, double, double> slopeData(slope, x, y);
      m_slopeData.push_back(slopeData);
      staticXprev = x;
      staticYprev = y;

      std::string G6Boundary = G6::GetName() + G6::IdentifyNearbyBoundaries(G6(reduced), 0.01).second;
      std::string D7Boundary = D7::GetName() + D7::IdentifyNearbyBoundaries(D7(reduced), 0.01).second;
      m_G6Boundaries.push_back(G6Boundary);
      m_D7Boundaries.push_back(D7Boundary);
   }
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void AddBoundaryString(const std::string& s)
      /*-------------------------------------------------------------------------------------*/
   {
   }
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void AddRunData(const long runOrdinal, const long runCount)
      /*-------------------------------------------------------------------------------------*/
   {
   }
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void AddSlopeData(const std::list<triple<double, double, double> >& slopeAndPoints) {
      std::copy(slopeAndPoints.begin(), slopeAndPoints.end(), m_slopeData.begin());
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void AddDistance(const double d) {
      m_distances.push_back(d);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void AddDeloneDistance(const double d) {
      m_Delonedistances.push_back(d);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void SetSlopeData(std::list<triple<double, double, double> >& slopeData) {
      std::copy(slopeData.begin(), slopeData.end(), m_slopeData.begin());
   }

   TVEC GetProbe(void) const { return(m_probe); };
   TVEC GetSecondProbe(void) const { return(m_secondProbe); }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   long GetRunOrdinal(void) const {
      assert(0);
      return(-1);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   long GetRunCount(void) const {
      assert(0);
      return(-1L);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string GetBoundaryString(void) const {
      assert(0);
      return("this is an error");
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string G6GetBoundaryString(void) const {
      assert(0);
      return("this is an error");
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string D7GetBoundaryString(void) const {
      assert(0);
      return("this is an error");
   }
   const std::vector<TVEC>&                                GetProbeList(void) const { return(m_probeList); }
   const std::vector<TVEC>&                                GetSecondProbeList(void) const { return(m_reducedList); }
   const std::list<double>&                                      GetDistances(void) const { return(m_distances); }
   const std::list<double>&                                      GetDeloneDistances(void) const { return(m_Delonedistances); }
   const std::vector<triple<double, double, double> >&             GetAngleData(void) const { return(m_slopeData); }
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   double GetCirclePlotDiagonal(void) {
      assert(!empty());
      const int i1 = m_whichComponentsToPlot.first;
      const int i2 = m_whichComponentsToPlot.second;
      double xmin = DBL_MAX;
      double ymin = DBL_MAX;
      double xmax = -DBL_MAX;
      double ymax = -DBL_MAX;

      for (typename std::vector<TVEC>::const_iterator it = m_reducedList.begin(); it != m_reducedList.end(); ++it)
      {
         xmin = std::min(xmin, (*it)[i1]);
         xmax = std::max(xmax, (*it)[i1]);
         ymin = std::min(ymin, (*it)[i2]);
         ymax = std::max(ymax, (*it)[i2]);
      }

      const std::pair<double, double> xlimits(std::make_pair(xmin, xmax));
      const std::pair<double, double> ylimits(std::make_pair(ymin, ymax));

      m_xyMinMax = std::make_pair(xlimits, ylimits);

      return(std::sqrt((xmax - xmin)*(xmax - xmin) + (ymax - ymin)*(ymax - ymin)));
   }
   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::pair<std::pair<double, double>, std::pair<double, double> >GetLimits(void)
      /*-------------------------------------------------------------------------------------*/
   {
      if (m_xyMinMax.first.first == DBL_MAX)
      {
         GetCirclePlotDiagonal();
      }

      return(m_xyMinMax);
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string G6GetBoundaryString(const unsigned long n) const {
      return m_G6Boundaries.empty() ? "" : m_G6Boundaries[n];
   }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   std::string D7GetBoundaryString(const unsigned long n) const {
      return m_D7Boundaries.empty() ? "" : m_D7Boundaries[n];
   }

   void SetWhichComponentsToPlot(const int c1, const int c2) { m_whichComponentsToPlot = std::make_pair(c1, c2); }

   /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
   void ApplyPerspective2Positions(const int nTotal) {
      const int first = GlobalConstants::globalWhichComponentsToPlot.first;
      const int second = GlobalConstants::globalWhichComponentsToPlot.second;
      const unsigned long nProbes = (unsigned long)m_reducedList.size();
      double xMax = -DBL_MAX;
      double yMax = -DBL_MAX;
      double xMin = DBL_MAX;
      double yMin = DBL_MAX;

      for (unsigned long i = 0; i < nProbes; ++i)
      {
         xMax = std::max(xMax, m_reducedList[i][first]);
         yMax = std::max(yMax, m_reducedList[i][second]);
         xMin = std::min(xMin, m_reducedList[i][first]);
         yMin = std::min(yMin, m_reducedList[i][second]);
      }

      const double xCenter = (xMax + xMin) / 2.0;
      const double yCenter = (yMax + yMin) / 2.0;

      for (unsigned long i = 0; i < nProbes; ++i)
      {
         const double factor = 1.0 - 0.75  * (double(i) / double(nTotal));
         double& x = m_reducedList[i][first];
         double& y = m_reducedList[i][second];
         x = factor * (x - xCenter) + xCenter;
         y = factor * (y - yCenter) + yCenter;
      }
   }

   bool empty(void) const { return(m_probeList.empty()); }
   unsigned long size(void) const { return(m_probeList.size()); }

private:
   TVEC m_probe;
   TVEC m_secondProbe;
   std::vector<TVEC> m_probeList;
   std::vector<TVEC> m_reducedList;
   std::vector<TVEC> m_bestMatchList;
   std::vector<std::string> m_G6Boundaries;
   std::vector<std::string> m_D7Boundaries;
   std::vector<triple<double, double, double> > m_slopeData;  // the slope of a segment, and the ending x,y
   std::list<double> m_distances;
   std::list<double> m_Delonedistances;
   std::pair<int, int> m_whichComponentsToPlot;
   std::pair< std::pair<double, double>, std::pair<double, double> > m_xyMinMax;
   double staticXprev;
   double staticYprev;
};


#endif
