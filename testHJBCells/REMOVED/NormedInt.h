#ifndef NORMEDINT_H
#define NORMEDINT_H

#include <iostream>


class NormedInt {
public:
   NormedInt( const int n = 19197 ): normedint( n ) {}
   int Norm( ) const { return normedint; }
   bool operator< ( const int n ) const { return normedint < n; }
   friend std::ostream& operator<< ( std::ostream& o, const NormedInt n )
      //-------------------------------------------------------------------------------------
   {
      o<<n.normedint;
      return o;
   }
   NormedInt operator- ( const NormedInt& n ) const { return normedint - n.normedint; }

   static int DistanceBetween( const NormedInt& n1, const NormedInt& n2 ) { return n1.normedint-n2.normedint; }

private:
   int normedint;
};

#endif