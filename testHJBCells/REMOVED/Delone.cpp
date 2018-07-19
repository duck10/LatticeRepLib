#ifdef _MSC_VER
#pragma once
#endif

#include "Delone.h"
#include "Reducer.h"
#include "VecN.h"
#include "VectorTools.h"

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <iostream>
#include <sstream>
#include <utility>

//#include "StoreResults.h"
//
//StoreResults<Delone::Key, G6> g_sr(5);  // added for D7 so that samples of cells from 
//                                            // each boundary could be later displayed, and
                                            // some idea of population density found.

bool Delone::IsAllMinus(const G6& v, const double delta) {
   return v[3] <= delta && v[4] <= delta && v[5] <= delta;
}

G6 Delone::Reduce( const G6& vi ) {
   Mat66 m;
   G6 vout;
   const double cutoff = 5000.0*DBL_EPSILON*vi.norm();
   Reduce( vi, m, vout, cutoff );
   return vout;
}

G6 SortEdges( const G6& vin ) {
   G6 v(vin);

   for ( int i=0; i<15; ++i ) {
      if ( v[0] > v[1] ) {
         std::swap( v[0], v[1] );
         std::swap( v[3], v[4] );
      }
      if ( v[1]>v[2] ) {
         std::swap( v[1], v[2] );
         std::swap( v[4], v[5] );
      }
   }

   return v;
}

const Mat66 mInterchange12( "0 1 0 0 0 0  1 0 0 0 0 0  0 0 1 0 0 0  0 0 0 0 1 0  0 0 0 1 0 0  0 0 0 0 0 1" );
const Mat66 mInterchange23( "1 0 0 0 0 0  0 0 1 0 0 0  0 1 0 0 0 0  0 0 0 1 0 0  0 0 0 0 0 1  0 0 0 0 1 0" );

void Delone::sortN( G6& v, Mat66& m ) {
   for ( int i=0; i<2; ++i ) {
      if ( v[0] > v[1] ) {
         m = mInterchange12 * m;
         std::swap( v[0], v[1] );
         std::swap( v[3], v[4] );
      }
      if ( v[1] > v[2] ) {
         m = mInterchange23 * m;
         std::swap( v[1], v[2] );
         std::swap( v[4], v[5] );
      }
   }
}

bool Delone::Reduce( const G6& vi, Mat66& m, G6& vout, const double delta ) {
   static const Mat66 mDEL3a( "1 1 0 0 0 -1   0 1 0 0 0 0   0 0 1 0 0 0   0 0 0 -1 0 0   0 0 0 1 -1 0   0 -2 0 0 0 1" );
   static const Mat66 mDEL3b( "1 0 1 0 -1 0   0 1 0 0 0 0   0 0 1 0 0 0   0 0 0 -1 0 0   0 0 -2 0 1 0   0 0 0 1 0 -1" );
   static const Mat66 mDEL4a( "1 0 0 0 0 0   1 1 0 0 0 -1   0 0 1 0 0 0   0 0 0 -1 1 0   0 0 0 0 -1 0   -2 0 0 0 0 1" );
   static const Mat66 mDEL4b( "1 0 0 0 0 0   0 1 1 -1 0 0    0 0 1 0 0 0   0 0 -2 1 0 0   0 0 0 0 -1 0   0 0 0 0 1 -1" );

   static const Mat66 mDEL5a( "1 0 0 0 0 0   0 1 0 0 0 0   0 1 1 -1 0 0   0 -2 0 1 0 0    0 0 0 0 -1 1    0 0 0 0 0 -1" );
   static const Mat66 mDEL5b( "1 0 0 0 0 0   0 1 0 0 0 0   1 0 1 0 -1 0   0 0 0 -1 0 1    -2 0 0 0 1 0    0 0 0 0 0 -1" );
   G6 vRed;

   Mat66 mNiggli = m.Eye();
   if ( !Reducer::Reduce( vi, mNiggli, vRed, delta ) ) return false;

   VectorTools::CleanVector( vRed );

   Mat66 mLocal;
   const double& g1 = vRed[0];
   const double& g2 = vRed[1];

   const double& g3 = vRed[2];
   const double& g4 = vRed[3];
   const double& g5 = vRed[4];
   const double& g6 = vRed[5];

        if ( IsAllMinus( vRed, delta ) ) mLocal = m.Eye();                        
   else if ( g4 <= g5 && g4 <= g6 ) mLocal = g2-g6 < g3-g5 ? mDEL3a : mDEL3b;
   else if ( g5 <= g4 && g5 <= g6 ) mLocal = g1-g6 < g3-g4 ? mDEL4a : mDEL4b;
   else if ( g6 <= g5 && g6 <= g4 ) mLocal = g2-g4 < g1-g5 ? mDEL5a : mDEL5b;

   vout = mLocal * vRed;
   m = mLocal * mNiggli;
   sortN( vout, m );

   double label = -1.0;
      if ( mLocal == mLocal.Eye() ) label = 0.0;
   else if ( mLocal == mDEL3a     ) label = 3.1;
   else if ( mLocal == mDEL3b     ) label = 3.2;
   else if ( mLocal == mDEL4a     ) label = 4.1;
   else if ( mLocal == mDEL4b     ) label = 4.2;
   else if ( mLocal == mDEL5a     ) label = 5.1;
   else if ( mLocal == mDEL5b     ) label = 5.2;

   if ( !IsAllMinus( vout, 1.0E-12*vout.norm() ) ) {
      std::cout << "Delone reduction did not give ---" << std::endl;
      std::cout << "mDEL3a*vRed" << std::endl << mDEL3a*vRed << std::endl << std::endl;
      std::cout << "mDEL3b*vRed" << std::endl << mDEL3b*vRed << std::endl << std::endl;
      std::cout << "mDEL4a*vRed" << std::endl << mDEL4a*vRed << std::endl << std::endl;
      std::cout << "mDEL4b*vRed" << std::endl << mDEL4b*vRed << std::endl << std::endl;
      std::cout << "mDEL5a*vRed" << std::endl << mDEL5a*vRed << std::endl << std::endl;
      std::cout << "mDEL5b*vRed" << std::endl << mDEL5b*vRed << std::endl << std::endl;
      std::cout << "mLocal" << std::endl << mLocal << std::endl;
      std::cout << "m" << std::endl << m << std::endl;
      std::cout << "Niggli             " << vRed << std::endl;
      std::cout << "Failed to reduce   " << vi << std::endl;
      std::cout << "what resulted      " << vout << std::endl << std::endl;

      return false;
   }

   return true;
}

bool Delone::IsDelone( const D7& v, const double delta ) {
   const double Delonesum = v[0]+v[1]+v[2]+v[3] -v[4]-v[5]-v[6];
   return std::fabs( Delonesum ) < delta;
}

