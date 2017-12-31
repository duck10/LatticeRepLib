
#include "stdafx.h"

#include "LRL_RandTools.h"

#include <cmath>
#include <iostream>
#include <cstdio>
#include <time.h>

static int iseed;

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void SetSeed( const int n ) {
   iseed = n;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void initrn( int& randToolsIseed, int& indx, int& jndx, int& kndx, double buffer[56] )
{
    jndx = randToolsIseed;
    for( indx=0; indx<56; ++indx )
    {
        jndx = (jndx*2349 + 14867)%32767;
        buffer[indx] = ::fabs(double(jndx)/32767.0);
    }
    randToolsIseed = -randToolsIseed;
    if( randToolsIseed == 0 ) randToolsIseed = -1;
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
