#include "stdafx.h"

#include "OutlierFinder.h"
#include "spline_under_tension.hpp"

#include <algorithm>
#include <cfloat>
#include <list>

OutlierFinder::OutlierFinder( const std::vector<double>& data ) 
   : m_data(data)
   , m_sut(spline_under_tension(0.1, data, 3))
   , m_intrp()
{
   for (unsigned long i = 0; i < data.size(); ++i) {
      m_intrp.push_back(GetValue(double(i)));
   }
}

std::vector<std::pair<double, double> > OutlierFinder::SelectCandidateTransitionPoints(const double cutoff) {
   std::vector<std::pair<double, double> > biggiesL;
   double prevDataDiff = 0.0;
   double prevData = m_data[0];
   const unsigned long endMask = std::max(3UL, (unsigned long)(0.03*(double)(m_data.size())));
   for (unsigned long i = endMask; i < m_data.size() - 1 - endMask; ++i) {
      const double& mdatai = m_data[i];
      const double& mdataim1 = m_data[i - 1];
      const double& mdataip1 = m_data[i + 1];
      const double percentDelta2Diff = 50.0 * std::abs(mdataim1 - mdataip1) / (mdataim1 + mdataip1);
      const double interpolatedMiddle = std::abs(GetValue(double(i) + 0.5));
      const double absDataDiff = std::abs(100.0*std::abs(mdatai - mdataip1) / std::max(mdatai, mdataip1));

      if (absDataDiff > cutoff && percentDelta2Diff > cutoff/*&& std::abs(mdatai - mdataip1) > 0.1*/) {
         const double avg = (mdatai + mdataip1) / 2.0;
         const double difpair = (absDataDiff < cutoff) ? 0.0 : std::abs(avg - interpolatedMiddle);

         const double normalizedPercent = 100.0*difpair / interpolatedMiddle;

         if ((normalizedPercent > 1.0)
             && (absDataDiff > prevDataDiff) )
            biggiesL.push_back(std::make_pair(double(i), normalizedPercent));
      }

      prevDataDiff = absDataDiff;
      prevData = mdatai;
   }
   return biggiesL;
}

std::vector<std::pair<double, double> > OutlierFinder::InsertSeparatorsBetweenRuns(const std::vector<std::pair<double, double> >& biggies) {
   if (biggies.size() < 2) return biggies;
   std::vector<std::pair<double, double> > biggiesL(biggies);
   std::vector<std::pair<double, double> > biggiesL2;
   std::vector<std::pair<double, double> >::iterator it2(biggiesL.begin());
   if (biggiesL.size() > 1) {
      std::pair<double, double> stopper(std::make_pair(-DBL_MAX, -DBL_MAX));
      // start by inserting separators between runs of index
      for (std::vector<std::pair<double, double> >::iterator it = biggiesL.begin(); it != biggiesL.end(); ++it) {
         it2 = it, ++it2;
         biggiesL2.push_back(*it);
         if (it2 == biggiesL.end()) break;
         if ((*it).first + 1 != (*it2).first) {
            biggiesL2.push_back(stopper);
         }
      }
   }
   return biggiesL2;
}

std::vector<std::pair<double, double> > OutlierFinder::FilterForPeakOfRun(const std::vector<std::pair<double, double> >& biggiesL2) {
   std::vector<std::pair<double, double> > bigTemp;
   std::vector<std::pair<double, double> > biggiesL3;
   std::vector<std::pair<double, double> >::const_iterator it = biggiesL2.begin();
   std::vector<std::pair<double, double> >::const_iterator it2;
   while (it != biggiesL2.end()) {
      bigTemp.clear();
      if ((*it).first == -DBL_MAX) ++it;
      bigTemp.push_back(*it);
      it2 = it, ++it2;
      while (it2 != biggiesL2.end() && (*it2).first != -DBL_MAX) {
         if (bigTemp[0].second < (*it2).second) bigTemp[0] = *it2;
         ++it2;
      }

      biggiesL3.push_back(bigTemp[0]);
      if (it2 == biggiesL2.end()) break;
      it = it2;
      ++it;
      if (it == biggiesL2.end()) break;
      it2 = it, ++it2;
   }
   return biggiesL3;
}


std::vector<std::pair<double, double> > OutlierFinder::FindDiscontinuities(const double cutoff) {
   std::vector<std::pair<double, double> > biggiesL = SelectCandidateTransitionPoints(cutoff);
   std::vector<std::pair<double, double> > biggiesL2 = InsertSeparatorsBetweenRuns(biggiesL);
   std::vector<std::pair<double, double> > biggiesL3 = FilterForPeakOfRun(biggiesL2);
   return biggiesL3;
}

double OutlierFinder::GetValue(const double d) {
   return (double)(m_sut.getInterpValue((float)(d)));
}


OutlierFinder::~OutlierFinder()
{
}
