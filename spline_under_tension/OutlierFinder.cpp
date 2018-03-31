
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
   for (unsigned long i = 0; i < m_data.size() - 1; ++i) {
      if ((i <= 3) || (i >= m_data.size() - 4)) {
         prevDataDiff = 0.0;
      }
      else {
         const double& mdatai = m_data[i];
         const double& mdatap1i = m_data[i + 1];
         const double absDataDiff = 100.0*std::abs(mdatai - mdatap1i) / std::max(mdatai, mdatap1i);
         if (absDataDiff > cutoff) {

            const double avg = (mdatai + mdatap1i) / 2.0;
            const double val = std::abs(GetValue(double(i) + 0.5));
            const double difpair = std::abs(avg - val);

            const double normalizedPercent = 100.0 * difpair / val;

            if ((normalizedPercent > cutoff) && (absDataDiff > 1.1*prevDataDiff))
               biggiesL.push_back(std::make_pair(double(i), normalizedPercent));
         }
         prevDataDiff = absDataDiff;
      }

   }
   return biggiesL;
}

std::vector<std::pair<double, double> > OutlierFinder::InsertSeparatorsBetweenRuns(const std::vector<std::pair<double, double> >& biggies) {
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
