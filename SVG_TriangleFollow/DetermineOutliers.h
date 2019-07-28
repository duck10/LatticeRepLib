#ifndef DETERMINEOUTLIERS_H
#define DETERMINEOUTLIERS_H

#include <vector>

#include "Glitch.h"
#include "GlitchElement.h"
#include "OutlierFinder.h"

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
template<typename TVEC>
std::vector<Glitch<TVEC> > DetermineOutliers(const std::vector<double> distanceList)
/*-------------------------------------------------------------------------------------*/
{
   OutlierFinder of(distanceList);
   std::vector<std::pair<double, double> > steps = of.FindDiscontinuities(GlobalConstants::globalPercentChangeToDetect);
   std::vector<Glitch<TVEC> >  glitches;

   for (size_t i = 0; i < steps.size(); ++i) {
      const size_t klow = (size_t)(steps[i].first);
      const size_t khigh = klow + 1UL;
      const GlitchElement<TVEC> Glitch1(distanceList[khigh], khigh, TVEC()/*[khigh]*/);
      const GlitchElement<TVEC> Glitch2(distanceList[klow], klow, TVEC()/*[klow]*/);
      glitches.push_back(Glitch<TVEC>(Glitch1, Glitch2));
   }

   return(glitches);
}

#endif  // DETERMINEOUTLIERS_H
