#ifdef _MSC_VER
#pragma once
#endif

#include "Allman.h"
#include "B4.h"
#include "Delone.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_MaximaTools.h"
#include "Niggli.h"
#include "Selling.h"

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <iostream>
#include <sstream>
#include <utility>

bool Allman::IsAllMinus(const G6& v, const double delta) {
   return v[3] <= delta && v[4] <= delta && v[5] <= delta;
}

G6 Allman::Reduce( const G6& vi ) {
   MatG6 m;
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

const MatG6 mInterchange12( "0 1 0 0 0 0  1 0 0 0 0 0  0 0 1 0 0 0  0 0 0 0 1 0  0 0 0 1 0 0  0 0 0 0 0 1" );
const MatG6 mInterchange23( "1 0 0 0 0 0  0 0 1 0 0 0  0 1 0 0 0 0  0 0 0 1 0 0  0 0 0 0 0 1  0 0 0 0 1 0" );

void Allman::sortN( G6& v, MatG6& m ) {
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

void CleanVector(G6& v) {
   double maxAbsValue = 0.0;
   for (unsigned long i = 0; i < v.size(); ++i)
      maxAbsValue = std::max(maxAbsValue, std::fabs(v[i]));
   const double cutoff = 5000.0*DBL_EPSILON*maxAbsValue;
   bool makeAllCosTermsNegative = false;
   for (unsigned long i = 0; i < v.size(); ++i) {
      if (std::fabs(v[i]) < cutoff) {
         v[i] = 0.0;
         makeAllCosTermsNegative = true;
      }
   }

   if (makeAllCosTermsNegative) {
      v[3] = v[3] != 0 ? -std::fabs(v[3]) : v[3];
      v[4] = v[4] != 0 ? -std::fabs(v[4]) : v[4];
      v[5] = v[5] != 0 ? -std::fabs(v[5]) : v[5];
   }
}

std::vector<MatS6> FillFaceDiagonalCreationMatrices( ) {
   std::vector<MatS6> vminv;
   vminv.push_back(MatS6(MatG6(("1 1 0 0 0 1 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 -1 0 0 0 0 0 -1 -1 0 0 2 0 0 0 1"))));
   vminv.push_back(MatS6(MatG6(("1 0 1 0 1 0 0 1 0 0 0 0 0 0 1 0 0 0 0 0 0 -1 0 0 0 0 2 0 1 0 0 0 0 -1 0 -1"))));
   vminv.push_back(MatS6(MatG6(("1 0 0 0 0 0 1 1 0 0 0 1 0 0 1 0 0 0 0 0 0 -1 -1 0 0 0 0 0 -1 0 2 0 0 0 0 1"))));
   vminv.push_back(MatS6(MatG6(("1 0 0 0 0 0 0 1 1 1 0 0 0 0 1 0 0 0 0 0 2 1 0 0 0 0 0 0 -1 0 0 0 0 0 -1 -1"))));
   vminv.push_back(MatS6(MatG6(("1 0 0 0 0 0 0 1 0 0 0 0 0 1 1 1 0 0 0 2 0 1 0 0 0 0 0 0 -1 -1 0 0 0 0 0 -1"))));
   vminv.push_back(MatS6(MatG6(("1 0 0 0 0 0 0 1 0 0 0 0 1 0 1 0 1 0 0 0 0 -1 0 -1 2 0 0 0 1 0 0 0 0 0 0 -1"))));
   return vminv;
}

bool Allman::ReduceS6ToNiggli( const S6& si, S6& s6out) {
   static const std::vector<MatS6> vminv{ FillFaceDiagonalCreationMatrices() };

   const bool b = Selling::Reduce(si, s6out);
   if (!b) return false;

   for (unsigned long i = 0; i < vminv.size(); ++i) {
      std::cout << LRL_Cell_Degrees(vminv[i] * s6out) << std::endl;
   }

   for ( unsigned long i = 0; i < vminv.size(); ++i) {
      const S6 s6 = vminv[i] * s6out;
      const B4 b4(s6);
      for (int k = 0; k < 4; ++k)
         std::cout << b4[k].Norm() << "  ";
      std::cout << std::endl;
      Delone::Reduce(S6(b4), s6out);
      std::cout << s6out << std::endl;
      std::cout << std::endl;
   }
   return true;
}

bool Allman::Reduce( const G6& vi, MatG6& m, G6& vout, const double delta ) {
   static const MatG6 mDEL3a( "1 1 0 0 0 -1   0 1 0 0 0 0   0 0 1 0 0 0   0 0 0 -1 0 0   0 0 0 1 -1 0   0 -2 0 0 0 1" );
   static const MatG6 mDEL3b( "1 0 1 0 -1 0   0 1 0 0 0 0   0 0 1 0 0 0   0 0 0 -1 0 0   0 0 -2 0 1 0   0 0 0 1 0 -1" );
   static const MatG6 mDEL4a( "1 0 0 0 0 0   1 1 0 0 0 -1   0 0 1 0 0 0   0 0 0 -1 1 0   0 0 0 0 -1 0   -2 0 0 0 0 1" );
   static const MatG6 mDEL4b( "1 0 0 0 0 0   0 1 1 -1 0 0    0 0 1 0 0 0   0 0 -2 1 0 0   0 0 0 0 -1 0   0 0 0 0 1 -1" );

   static const MatG6 mDEL5a( "1 0 0 0 0 0   0 1 0 0 0 0   0 1 1 -1 0 0   0 -2 0 1 0 0    0 0 0 0 -1 1    0 0 0 0 0 -1" );
   static const MatG6 mDEL5b( "1 0 0 0 0 0   0 1 0 0 0 0   1 0 1 0 -1 0   0 0 0 -1 0 1    -2 0 0 0 1 0    0 0 0 0 0 -1" );
   G6 vRed;

   MatG6 mNiggli = MatG6::Eye();
   if ( !Niggli::Reduce( vi, mNiggli, vRed, delta ) ) return false;

   CleanVector( vRed );

   MatG6 mLocal;
   const double& g1 = vRed[0];
   const double& g2 = vRed[1];

   const double& g3 = vRed[2];
   const double& g4 = vRed[3];
   const double& g5 = vRed[4];
   const double& g6 = vRed[5];

        if ( IsAllMinus( vRed, delta ) ) mLocal = MatG6::Eye();                        
   else if ( g4 <= g5 && g4 <= g6 ) mLocal = g2-g6 < g3-g5 ? mDEL3a : mDEL3b;
   else if ( g5 <= g4 && g5 <= g6 ) mLocal = g1-g6 < g3-g4 ? mDEL4a : mDEL4b;
   else if ( g6 <= g5 && g6 <= g4 ) mLocal = g2-g4 < g1-g5 ? mDEL5a : mDEL5b;

   vout = mLocal * vRed;
   m = mLocal * mNiggli;
   sortN( vout, m );

   double label = -1.0;
      if ( mLocal == MatG6::Eye() ) label = 0.0;
   else if ( mLocal == mDEL3a     ) label = 3.1;
   else if ( mLocal == mDEL3b     ) label = 3.2;
   else if ( mLocal == mDEL4a     ) label = 4.1;
   else if ( mLocal == mDEL4b     ) label = 4.2;
   else if ( mLocal == mDEL5a     ) label = 5.1;
   else if ( mLocal == mDEL5b     ) label = 5.2;

   if ( !IsAllMinus( vout, 1.0E-12*vout.norm() ) ) {
      std::cout << "Allman reduction did not give ---" << std::endl;
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

bool Allman::IsDelone( const D7& v, const double delta ) {
   const double Allmansum = v[0]+v[1]+v[2]+v[3] -v[4]-v[5]-v[6];
   return std::fabs( Allmansum ) < delta;
}

