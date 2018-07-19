#ifndef D6_H
#define D6_H

#include "VecN.h"

class Cell;
class D7;
class DeloneTetrahedron;
class G6;

#include "VecN.h"

#include <ostream>
#include <string>

class D6 : public VecN
{
public:
   friend std::ostream& operator<< (std::ostream&, const D6&);
   friend class D7;

   D6(void);
   explicit D6(const G6& v6);
   explicit D6(const double v[6]);
   explicit D6(const D6& d6);
   D6(const Cell& c);
   D6(const VecN& v);
   explicit D6(const D7& v7);
   explicit D6(const DeloneTetrahedron& del);
   D6(const std::string& s);
   D6(const std::vector<double>& v);
      ~D6(void) {}


   D6& operator= (const D6& v);
   D6& operator=(const std::string& s);

   //D6 operator- (const D6& ds) const;
   //D6 operator+ (const D6& ds) const;
   //D6 operator* (const double& d) const;
   static double DistanceBetween(const D6& v1, const D6& v2);
   //double norm(const D6& ds) const;
   //double norm(void) const;
   //double Norm(const D6& ds) const { return norm(ds); }
   //double Norm(void) const { return (*this).norm(); }
   double operator[](const size_t n) const { return m_vec[n]; }
   double& operator[](const size_t n) { return m_vec[n]; }

};


#endif
