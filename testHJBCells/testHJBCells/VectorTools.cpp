#include <algorithm>
#include <cfloat>
#include <cmath>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "VectorTools.h"
#include "G6.h"

G6 VectorTools::FromString( const std::string& s ) {
   std::istringstream istr( s );
   G6 t;
   double d;
   int i = 0;
   while ( istr && i < 6 ) {
      istr >> d;
      t[i] = d;
      ++i;
   }

   return t;
}

void VectorTools::CleanVector( VecN& v ) {
   double maxAbsValue = 0.0;
   for( size_t i = 0; i < v.size( ); ++i )
      maxAbsValue = std::max( maxAbsValue, std::fabs( v[i] ) );
   const double cutoff = 5000.0*DBL_EPSILON*maxAbsValue;
   bool makeAllCosTermsNegative = false;
   for( size_t i = 0; i < v.size( ); ++i ) {
      if ( std::fabs( v[i] ) < cutoff ) {
         v[i] = 0.0;
         makeAllCosTermsNegative = true;
      }
   }

   if ( makeAllCosTermsNegative ) {
      v[3] = v[3] != 0 ? -std::fabs( v[3] ) : v[3];
      v[4] = v[4] != 0 ? -std::fabs( v[4] ) : v[4];
      v[5] = v[5] != 0 ? -std::fabs( v[5] ) : v[5];
   } 
}

void VectorTools::CleanVector( G6& v, Mat66& m ) {
   double maxAbsValue = 0.0;
   for( size_t i = 0; i < v.size( ); ++i )
      maxAbsValue = std::max( maxAbsValue, std::abs( v[i] ) );
   const double cutoff = 5000.0*DBL_EPSILON*maxAbsValue;
   bool makeAllCosTermsNegative = false;
   for( size_t i = 0; i < v.size( ); ++i ) {
      if ( std::fabs( v[i] ) < cutoff ) {
         v[i] = 0.0;
         makeAllCosTermsNegative = true;
      }
   }

   if ( makeAllCosTermsNegative ) {
      v[3] = -std::fabs( v[3] );
      v[4] = -std::fabs( v[4] );
      v[5] = -std::fabs( v[5] );
   }
}

MatMN VectorTools::SetMatrix6To7( void ) {
   MatMN m6To7( 7,6 );
   return m6To7=MatMN( "1 0 0 0 0 0   0 1 0 0 0 0   0 0 1 0 0 0   1 1 1 1 1 1   0 1 1 1 0 0   1 0 1 0 1 0   1 1 0 0 0 1" );
}

MatMN VectorTools::SetMatrix7To6( void ){
   MatMN m7To6( 6,7 );
   return m7To6 = MatMN( "1 0 0 0 0 0 0   0 1 0 0 0 0 0   0 0 1 0 0 0 0   0 -1 -1 0 1 0 0   -1 0 -1 0 0 1 0   -1 -1 0 0 0 0 1" );
}

