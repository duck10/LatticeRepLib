#ifndef LM_H
#define LM_H

#include "LRL_LatticeMatcher.h"
#include "S6.h"

class LM {
public:
   LM( );
   LM( const S6& s );
   double DistanceBetween( const S6& s );
   double DistanceBetween( const S6& s1, const S6& s2 );
   static std::string GetName( void ) { return "LM"; }
   S6 GetBestMatch(void) const { return m_bestFit; }

protected:
   LMDist m_lmdist;
   S6 m_bestFit;
};


#endif // LM_H
