#include "LM.h"


LM::LM() {}

LM::LM( const S6& s) {
   m_lmdist.SetReferenceLattice( s );
}

double LM::DistanceBetween( const S6& s ) {
   return m_lmdist.DistanceBetween( s );
}
double LM::DistanceBetween( const S6& s1, const S6& s2 ) {
   return m_lmdist.DistanceBetween( s1, s2 );
}