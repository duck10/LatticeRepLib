#ifndef Allman_H
#define Allman_H

#ifdef _MSC_VER
#pragma once
#endif

#include "MatG6.h"
#include "G6.h"
#include "D7.h"
#include "S6.h"

#include <ostream>
#include <string>

class Allman {

public:
   static S6 Reduce(const S6& d);
   static bool Reduce( const G6& vi, MatG6& m, G6& vout, const double delta  );
   static void sortN( G6& v, MatG6& m );
   static G6 Reduce( const G6& vi );
   static void Report( void );
   static bool IsAllMinus(const G6& v, const double delta);
   static bool IsDelone( const D7& v, const double delta = 1.0E-6 );
   static bool ReduceS6ToNiggli(const S6& si, S6& s6out);
};

#endif  // Allman_H

