#ifndef LRL_RANDTOOLS_H
#define LRL_RANDTOOLS_H

#include "rhrand.h"


void initrn( int& iseed, int& indx, int& jndx, int& kndx, double buffer[56] );

void SetSeed( const int n );

double randfg( int& iseed );

#endif  // LRL_RANDTOOLS_H

