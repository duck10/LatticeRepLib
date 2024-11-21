#ifndef OUTLIERFINDER_H
#define OUTLIERFINDER_H

#include <cmath>
#include <vector>

#include "spline_under_tension.hpp"

class OutlierFinder
{
public:
   OutlierFinder(const std::vector<double>& data);
   ~OutlierFinder();

   double GetValue(const double d);

   std::vector<std::pair<double, double> > FindDiscontinuities(const double cutoff);


protected:
   const std::vector<double> m_data;
   spline_under_tension m_sut;
   std::vector<double> m_intrp;

   std::vector<std::pair<double, double> > SelectCandidateTransitionPoints(const double cutoff);
   std::vector<std::pair<double, double> > InsertSeparatorsBetweenRuns(const std::vector<std::pair<double, double> >& biggiesL);
   std::vector<std::pair<double, double> > FilterForPeakOfRun(const std::vector<std::pair<double, double> >& biggiesL);

};

#endif  // OUTLIERFINDER_H