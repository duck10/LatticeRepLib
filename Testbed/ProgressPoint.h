#ifndef PROGRESSPOINT_H
#define PROGRESSPOINT_H

#include "ProgressPointBase.h"

template<typename T>
class ProgressPoint : public ProgressPointBase {
public:
   ProgressPoint(const unsigned long ordinal, const double dmin)
      : ProgressPointBase()
      , m_ordinal(ordinal)
      , m_minimumdist(dmin)
      , m_validToAddMoreLatticeTypes(T::GetName())
   {
   }

   ProgressPoint(const ProgressPoint& point )
      : ProgressPointBase()
      , m_ordinal(point.m_ordinal)
      , m_minimumdist(point.m_minimumdist)
      , m_progress1(point.progress1)
      , m_progress2(point.progress2)
      , m_best1(point.m_best1)
      , m_best2(point.m_best2)
      , m_validToAddMoreLatticeTypes(point.m_validToAddMoreLatticeTypes)
   {
   }

   void SetProgress(const T& progress1, const T& progress2) {
      m_progress1 = progress1;
      m_progress2 = progress2;
   }

   void SetBest(const T& best1, const T& best2) {
      m_best1 = best1;
      m_best2 = best2;
   }

   ProgressPoint& operator= (const ProgressPoint& point) {
      m_ordinal = point.m_ordinal;
      m_minimumdist = point.m_minimumdist;
      m_progress1 = point.progress1;
      m_progress2 = point.progress2;
      m_best1 = point.m_best1;
      m_best2 = point.m_best2;
   }

   ProgressPoint GetProgressPoint( void ) const {
      return *this;
   }

private:
   unsigned long m_ordinal;
   double m_minimumdist;
   std::string m_latticeName;
   T m_progress1;
   T m_progress2;
   T m_best1;
   T m_best2;
};

#endif  //  PROGRESSPOINT_H

