#include "ProgressData.h"

#include <cmath>
const bool NormedDist::PointIsError( const int n, const double rawDeltaDist12, const double rawDeltaDist13, const double rawDeltaDist23 ) const {
   if (n >= 0 && n+1 < int(m_iserror.size( )) )
      return m_iserror[n];
   else
      return false;
}

void NormedDist::SetPointIsError( const std::vector<double>& tanhHaar ) {
   m_iserror.clear( );
   m_iserror.resize( tanhHaar.size( ), false );

   for (unsigned int i = 0; i < tanhHaar.size( ); ++i) {
      if (std::abs( tanhHaar[i] ) > 0.2) {
         m_iserror[i] = true;
         if (i > 0 ) m_iserror[i - 1] = true;
         if (i > 1 ) m_iserror[i - 2] = true;
         if (i < tanhHaar.size() ) m_iserror[i + 1] = true;
      }
      else
         m_iserror[i] = false;

   }
}
