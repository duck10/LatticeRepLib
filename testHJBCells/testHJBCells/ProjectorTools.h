#pragma once

#ifndef PROJECTORTOOLS_H
#define PROJECTORTOOLS_H

#include <algorithm>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>

//#include "NC_Boundary.h"
#include "Mat66.h"
#include "G6.h"

const double SmallEnoughToBeConsideredZero = 1.0E-30;

template<typename T>
T maxNC(const T& t1, const T& t2) {
   return (std::max(t1, t2));
}
;
template<typename T>
T maxNC(const T& t1, const T& t2, const T& t3) {
   const T t4 = std::max(t2, t3);
   return (std::max(t1, t4));
}
;
template<typename T>
T maxNC(const T& t1, const T& t2, const T& t3, const T& t4) {
   return (std::max(t1, maxNC(t2, t3, t4)));
}
;
template<typename T>
T maxNC(const T& t1, const T& t2, const T& t3, const T& t4, const T& t5) {
   return (std::max(t1, maxNC(t2, t3, t4, t5)));
}
;
template<typename T>
T maxNC(const T& t1, const T& t2, const T& t3, const T& t4, const T& t5,
      const T& t6) {
   return (std::max(t1, maxNC(t2, t3, t4, t5, t6)));
}
;
template<typename T>
T maxNC(const T& t1, const T& t2, const T& t3, const T& t4, const T& t5,
      const T& t6, const T& t7) {
   return (std::max(t1, maxNC(t2, t3, t4, t5, t6, t7)));
}
;

template<typename T>
T minNC(const T& t1, const T& t2) {
   return (std::min(t1, t2));
}
;
template<typename T>
T minNC(const T& t1, const T& t2, const T& t3) {
   return (std::min(t1, std::min(t2, t3)));
}
;
template<typename T>
T minNC(const T& t1, const T& t2, const T& t3, const T& t4) {
   return (std::min(t1, minNC(t2, t3, t4)));
}
;
template<typename T>
T minNC(const T& t1, const T& t2, const T& t3, const T& t4, const T& t5) {
   return (std::min(t1, minNC(t2, t3, t4, t5)));
}
;
template<typename T>
T minNC(const T& t1, const T& t2, const T& t3, const T& t4, const T& t5,
      const T& t6) {
   return (std::min(t1, minNC(t2, t3, t4, t5, t6)));
}
;
template<typename T>
T minNC(const T& t1, const T& t2, const T& t3, const T& t4, const T& t5,
      const T& t6, const T& t7) {
   return (std::min(t1, minNC(t2, t3, t4, t5, t6, t7)));
}
;

class ProjectorTools {
public:
   //ProjectorTools(void);
   //~ProjectorTools(void);
   //static double MinimumDistanceToBoundary(const G6& v);
   static bool BothEssentiallySameSign(const G6& v1, const G6& v2);  // specialized for G6
   static G6 ConvertArrayToG6(const double a[6]);  // specialized for G6
   static void ConvertG6ToArray(const G6 & v, double a[6]);  // specialized for G6
   static D7 ConvertArrayToD7(const double a[]);  // specialized for D7
   static void ConvertD7ToArray(const D7 & v, double a[]);  // specialized for D7

   //static double DistanceToClosestBoundary(const G6& v,
   //      std::vector<NC_Boundary>& bounds);
   //static std::vector<NC_Boundary> CreateBoundaryList(void);

   static bool EssentiallyPlus(const G6& t) {  // specialized for G6
      return t[3] > -SmallEnoughToBeConsideredZero
            || t[4] > -SmallEnoughToBeConsideredZero
            || t[5] > -SmallEnoughToBeConsideredZero;
   }

   static bool HasNearZero456(const G6& v) {  // specialized for G6

      return std::fabs(v[3]) < SmallEnoughToBeConsideredZero
            || std::fabs(v[4]) < SmallEnoughToBeConsideredZero
            || std::fabs(v[5]) < SmallEnoughToBeConsideredZero;
   }

   static bool EssentiallyNegative(const G6& t) {  // specialized for G6

      return t[3] < -SmallEnoughToBeConsideredZero
            || t[4] < -SmallEnoughToBeConsideredZero
            || t[5] < -SmallEnoughToBeConsideredZero;
   }


protected:
   std::vector<double> m_vCleanupList;
   size_t InitCleanupList(void);
   Mat66 CleanupMatrix_SetNearZeroToZero(const Mat66& projector) const;
   static double MatrixNorm(const Mat66& m);
   static std::vector<Mat66> GeneratePrincipalProjectors(
         const std::vector<Mat66>& perps);
   static Mat66 ComputeProjectorFromVector(const G6& v);  // specialized for G6
   static Mat66 ConvertStringToPerp(const std::string& s);
   static Mat66 ConvertStringToProjector(const std::string& s);
};

template<typename T>
bool IsPlus(const T& t) {
   return ProjectorTools::EssentiallyPlus(t);
}
;
template<typename T>
bool IsNegative(const T& t) {
   return ProjectorTools::EssentiallyNegative(t);
}
;

#endif // PROJECTORTOOL_H
