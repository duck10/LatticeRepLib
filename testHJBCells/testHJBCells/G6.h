#ifndef NC_VEC_H
#define NC_VEC_H

#include "VecN.h"

class Cell;
class D6;
class DeloneTetrahedron;
class D7;

#include <ostream>
#include <string>

class G6 : public VecN {
public:
   friend std::ostream& operator<< ( std::ostream&, const D7& );
   friend class D6;

   G6(void);
   G6(const G6& v);
   G6(const double v[6]);
   G6(const D7& v);
   G6(const Cell& c);
   G6(const VecN& v);
   G6(const D6& ds);
   G6(const DeloneTetrahedron& dt);
   G6(const std::string& s);
   G6(const std::vector<double>& v);
   ~G6(void) {}

   G6& operator= (const G6& v);
   G6& operator= (const std::string& s);
   G6& operator= (const D7& v);
   G6& operator= (const D6& v);
   G6& operator= (const DeloneTetrahedron& v);
   G6& operator= (const Cell& v);

   static double DistanceBetween( const G6& v1, const G6& v2 );
   G6 InverseG6Vector( void ) const;

};

#endif

