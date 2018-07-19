
#include "RandTools.h"

#include <cmath>
#include <iostream>
#include <cstdio>
#include <time.h>

#include "Delone.h"
#include "FollowerConstants.h"
#include "ProjectorTools.h"
#include "Reducer.h"

static int iseed;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void SetSeed( const int n ) {
   iseed = n;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void initrn( int& iseed, int& indx, int& jndx, int& kndx, double buffer[56] )
{
    jndx = iseed;
    for( indx=0; indx<56; ++indx )
    {
        jndx = (jndx*2349 + 14867)%32767;
        buffer[indx] = ::fabs(double(jndx)/32767.0);
    }
    iseed = -iseed;
    if( iseed == 0 ) iseed = -1;
    indx = 55;
    kndx = 54;
    jndx = 31;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
double randfg( int& seed )
{
    //C   real function randfg( seed )
    //C        if seed .gt. 0 use seed to initialize
    //C        returns a normally distributed random number with unit variance
    //C
    //C   EXACTLY FOLLOWS THE ALGORITHM OF KNUTH, THE ART OF COMPUTER
    //C   PROGRAMMING, V. 22D. ED., 1981, P. 117-118  (THE POLAR METHOD)
    //C   EXCEPT THAT LIMITS ARE INCLUDED BY MAKING THE MINIMUM VALUE OF "S"
    //C   BE EPS
    static double randomNumberBuffer[56];
    static int indx = -1;
    static int jndx;
    static int kndx;
    static double eps;
    double r1 = 1.0;
    double r2 = 0.0;

    if( seed >= 0 || indx < 0 )
    {
        r1 = 1.0;
        for( int i=0; i<100; ++i )
        {
            eps = r1;
            r1 = r1/2.0;
            if( r1+1.0 == 1.0 ) break;
        }
        initrn( seed, indx, jndx, kndx, randomNumberBuffer );
    }

    double s;
    do{
        indx = indx%55 + 1;
        jndx = jndx%55 + 1;
        kndx = kndx%55 + 1;
        double dTemp;
        randomNumberBuffer[indx] = modf( randomNumberBuffer[jndx]+randomNumberBuffer[kndx], &dTemp );
        r1 = 2.0 * randomNumberBuffer[indx] - 1.0;
        indx = indx%55 + 1;
        jndx = jndx%55 + 1;
        kndx = kndx%55 + 1;
        randomNumberBuffer[indx] = modf( randomNumberBuffer[jndx]+randomNumberBuffer[kndx], &dTemp );
        r2 = 2.0 * randomNumberBuffer[indx] - 1.0;
        s = r1*r1 + r2*r2;
        if( s >= 1.0 ) continue;
        if( ::fabs(s) < eps ) s = ( s >= 0 )? eps : -eps;
    } while( s >= 1.0 );

    return( r1 * ::sqrt( -2.0 * log10(s)/s ) );
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
G6 GenRandG6( void ) {
   G6 rands;

   for ( int i=0; i<6; ++i )
      rands[i] = randfg( iseed );

   const double randsNorm = rands.norm( );
   return( rands / randsNorm );
}

G6 genRandG6V() {
   G6 g, vRan;

   for (int i = 0; i < 6; ++i)
      vRan[i] = randfg(iseed);

   vRan[0] = std::fabs(vRan.at(0));
   vRan[1] = std::fabs(vRan.at(1));
   vRan[2] = std::fabs(vRan.at(2));

   vRan[3] = 0.5*std::sqrt(vRan[1] * vRan[2]) * vRan[3];
   vRan[4] = 0.5*std::sqrt(vRan[0] * vRan[2]) * vRan[4];
   vRan[5] = 0.5*std::sqrt(vRan[1] * vRan[0]) * vRan[5];

   return(vRan);
}

Cell GenRandG6ReducedCell( void ) {
   G6 vRan;
   G6 v;

   Mat66 m;
   vRan = G6(1000.0 * genRandG6V());

   bool btemp = false;
   while (btemp = Reducer::Reduce(vRan, m, v, 0.0), !btemp) {
      vRan = G6(1000.0 * genRandG6V());
   }
   return Cell(v);
}

Cell GenRandG6DeloneReducedCell( void ) {
   G6 vRan;
   G6 v;

   Mat66 m;
   vRan = G6(1000.0 * genRandG6V());

   bool btemp = false;
   while (btemp =Delone::Reduce(vRan, m, v, 0.0), !btemp) {
      vRan = G6(1000.0 * genRandG6V());
   }
   return Cell(v);
}

G6 GenRandG6Cell( void ) {
   G6 vRan;
   G6 v;

   Mat66 m;
   vRan = G6(1000.0 * genRandG6V());

   bool btemp = false;
   while (btemp = Reducer::Reduce(vRan, m, v, 0.0), !btemp) {
      vRan = G6(1000.0 * genRandG6V());
   }
   return vRan;
}

bool GenRandReducedArray(double* a, double aRan[6]) {
   G6 vRan;
   G6 v;

   Mat66 m;
   vRan = G6(1000.0 * genRandG6V());

   bool btemp = false;
   while (btemp = Reducer::Reduce(vRan, m, v, 0.0), !btemp) {
      vRan = G6(1000.0 * genRandG6V());
   }
   a[0] = v.at(0);
   a[1] = v.at(1);
   a[2] = v.at(2);
   a[3] = v.at(3);
   a[4] = v.at(4);
   a[5] = v.at(5);

   ProjectorTools::ConvertG6ToArray(v, aRan);
   return btemp;
}

bool GenRandDeloneReducedArray(double* a, double aRan[6]) {
   G6 vRan;
   G6 v;

   Mat66 m;
   vRan = G6(1000.0 * genRandG6V());

   bool btemp = false;
   while (btemp = Delone::Reduce(vRan, m, v, 0.0), !btemp) {
      vRan = G6(1000.0 * genRandG6V());
   }
   a[0] = v.at(0);
   a[1] = v.at(1);
   a[2] = v.at(2);
   a[3] = v.at(3);
   a[4] = v.at(4);
   a[5] = v.at(5);

   ProjectorTools::ConvertG6ToArray(v, aRan);
   return btemp;
}

