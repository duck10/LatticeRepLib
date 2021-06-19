#include "LM.h"
#include "LRL_LatticeMatcher.h"

LM::LM() {}

LM::LM( const S6& s) {
   m_lmdist.SetReferenceLattice( s );
}

LM::LM(const int recursionDepth, const double sphereEnlargement, const bool useOLD_Algorith)
   : m_lmdist(recursionDepth, sphereEnlargement, useOLD_Algorith)
{
}


double LM::DistanceBetween( const S6& s ) {
   return m_lmdist.DistanceBetween( s );
}
double LM::DistanceBetween( const S6& s1, const S6& s2 ) {
   m_bestFit = m_lmdist.GetBestMatch();
   return m_lmdist.DistanceBetween( s1, s2 );
}