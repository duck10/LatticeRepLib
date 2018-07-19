#ifndef D7_H
#define D7_H

#include "VecN.h"

class Cell;
class D6;
class DeloneTetrahedron;
class G6;

#include <ostream>
#include <string>

class D7: public VecN {
public:
   friend std::ostream& operator<< ( std::ostream&, const D7& );

   D7( void );
   D7( const D7& v );
   explicit D7( const double v[7] );
   explicit D7( const G6& v );
   D7(const Cell& c);
   D7( const VecN& v );
   explicit D7(const D6& ds);
   explicit D7(const DeloneTetrahedron& dt);
   D7( const std::string& s );
   D7( const std::vector<double>& v );
   ~D7(void) {}

   D7& operator= (const D7& v);
   D7& operator= (const std::string& s);
   D7& operator= (const G6& v);
   D7& operator= (const D6& v);
   D7& operator= (const DeloneTetrahedron& v);
   D7& operator= (const Cell& v);

   static double DistanceBetween( const D7& v1, const D7& v2 );

};

#endif

