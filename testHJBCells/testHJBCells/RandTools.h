#if _MSC_VER
#pragma once
#endif // _MSC_VER

#ifndef RANDTOOLS_H
#define RANDTOOLS_H

#include "Cell.h"
#include "rhrand.h"
#include "G6.h"


void initrn( int& iseed, int& indx, int& jndx, int& kndx, double buffer[56] );

void SetSeed( const int n );

double randfg( int& iseed );

G6 GenRandG6( void );

G6 genRandG6V() ;

Cell GenRandG6ReducedCell( void );
Cell GenRandG6DeloneReducedCell( void );

G6 GenRandG6Cell( void );

bool GenRandReducedArray(double* a, double aRan[6]);
bool GenRandDeloneReducedArray(double* a, double aRan[6]);

#endif

