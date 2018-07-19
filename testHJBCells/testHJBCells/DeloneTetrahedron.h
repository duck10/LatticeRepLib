#ifndef DeloneTETRAHEDRON_H
#define DeloneTETRAHEDRON_H

#include "vector_3d.h"

#include <vector>

   class Cell;
   class D6;
   class G6;
   class D7;

#include <ostream>
#include <string>

class DeloneTetrahedron {
public:

   friend std::ostream& operator<< (std::ostream&, const DeloneTetrahedron&);
   friend DeloneTetrahedron operator* (const double d, const DeloneTetrahedron& dt);

   DeloneTetrahedron(void);
   DeloneTetrahedron(const DeloneTetrahedron& dt);
   //DeloneTetrahedron( 4 vectors );
   explicit DeloneTetrahedron(const D7& v7);
   explicit DeloneTetrahedron(const Cell& c);
   explicit DeloneTetrahedron(const D6& c);
   explicit DeloneTetrahedron(const G6& v6);
   explicit DeloneTetrahedron(const std::string& c);
   ~DeloneTetrahedron(void) {}

   DeloneTetrahedron& operator= (const std::string& s);
   bool operator== (const DeloneTetrahedron& dt);
   bool operator!= (const DeloneTetrahedron& dt);
   DeloneTetrahedron operator* (const double d) const;
   DeloneTetrahedron operator+ (const DeloneTetrahedron& dt) const;
   DeloneTetrahedron operator- (const DeloneTetrahedron& dt) const;

   Vector_3  operator[] (const size_t n) const;
   Vector_3& operator[] (const size_t n);

   static double DistanceBetween( const DeloneTetrahedron& v1, const DeloneTetrahedron& v2 );
   double norm(const DeloneTetrahedron& dt) const;
   double norm(void) const;

   static size_t size(void) { return 4; }

protected:
   std::vector<Vector_3> m_vec;
};


#endif