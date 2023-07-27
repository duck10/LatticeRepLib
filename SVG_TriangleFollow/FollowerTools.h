
#ifndef FOLLOWERTOOLS_H
#define FOLLOWERTOOLS_H

#include <algorithm>
#include <sstream>
#include <string>
#include <utility>

#include "G6.h"
#include "LRL_MinMaxTools.h"
#include "triple.h"

std::pair<int,std::string> IdentifyNearbyBoundaries(const G6& v, const double cutoff);
std::pair<int,std::string> IdentifyNearbyBoundaries(const D7& v, const double cutoff);

namespace FollowerTools {
   //double NCDist(const G6& v1, const G6& v2);  // particular for G6


   static double TriangleInequality(const double d1, const double d2, const double d3) {
      // negative return is a triangle violation
      if ( d1+d2+d3 < 1.0E-10 ) return 1.0E-6;
      return (2.0*maxNC(d1, d2, d3) - d1 - d2 - d3) / maxNC(d1, d2, d3);
   }

   template<typename TVEC>
   static double TriangleInequality(const triple<TVEC,TVEC,TVEC>& tr) {
      // negative return is a triangle violation
      const double d1 = NCDist(tr.first, tr.second);   // specialized for G6
      const double d2 = NCDist(tr.first, tr.third) ;   // specialized for G6
      const double d3 = NCDist(tr.second, tr.third);   // specialized for G6
      const double worstTriangleViolation = 100.0*FollowerTools::TriangleInequality(d1, d2, d3);
      return(2.0*maxNC(d1, d2, d3) - d1 - d2 - d3) / maxNC(d1, d2, d3);
   }

};
#endif
