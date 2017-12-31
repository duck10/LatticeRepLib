#ifndef G6_H
#define G6_H

#ifdef _MSC_VER
#define USE_LOCAL_HEADERS
#endif
#ifndef USE_LOCAL_HEADERS
//#include <rhrand.h>
#else
#include "rhrand.h"
#endif

#include "BasisBase.h"
#include "VecN.h"

class LRL_Cell;
class S6;
class D7;
class B4;

#include <ostream>
#include <string>

class G6 : private BasisBase<G6> {
public:
   friend std::ostream& operator<< (std::ostream&, const G6&);
   friend G6 operator* (const double d, const G6& v);
   friend class S6;

   G6(void);
   G6(const G6& v);
   G6(const double v[6]);
   G6(const D7& v);
   G6(const LRL_Cell& c);
   G6(const VecN& v);
   G6(const S6& ds);
   G6(const B4& dt);
   G6(const std::string& s);
   G6(const std::vector<double>& v);
   ~G6(void) {}

   G6& operator= (const G6& v);
   G6& operator= (const std::string& s);
   G6& operator= (const D7& v);
   G6& operator= (const S6& v);
   G6& operator= (const B4& v);
   G6& operator= (const LRL_Cell& v);
   G6& operator/= (const double d);
   G6& operator*= (const double d);
   G6 operator+= (const G6& v);
   G6 operator-= (const G6& v);
   G6 operator+ (const G6& v) const;
   G6 operator- (const G6& v) const;
   G6 operator* (const double d) const;
   G6 operator/ (const double d) const;
   G6 operator- (void) const; // unary
   bool operator== (const G6& g6) const;
   bool operator!= (const G6& g6) const;

   double operator[](const unsigned long n) const { return m_vec[n]; }
   double& operator[](const unsigned long n) { return m_vec[n]; }
   double DistanceBetween(const G6& v1, const G6& v2);
   std::vector<std::pair<std::string, std::string> > ClassifyVector(const double delta) const;
   bool TestBoundaryDetection(void);
   unsigned long size(void) const { return 6; }
   double norm() const { return m_vec.norm(); }
   double norm(const G6& g6) const { return g6.norm(); }
   double Norm() const { return m_vec.norm(); }
   double Norm(const G6& g6) const { return g6.norm(); }

   G6 InverseG6Vector(void) const;
   G6 sort(const G6& g6);
   std::vector<double> GetVector(void) const { return m_vec.GetVector(); }
   double* data() const { return const_cast<double*>(m_vec.data()); }
   void SetVector(const std::vector<double>& v) { m_vec = v; }
   bool GetValid(void) const { return m_valid; }
   void SetValid(const bool b) { m_valid = b; }

   double at(const unsigned long n) const { return m_vec[n]; }

   static G6 rand();
   static G6 randDeloneReduced();
   static G6 randDeloneUnreduced();
   static G6 rand(const double d);
   static G6 randDeloneReduced(const double d);
   static G6 randDeloneUnreduced(const double d);
   static std::string GetName(void) { return "G6"; }

   static std::pair<int, std::string> IdentifyNearbyBoundaries(const G6& v, const double cutoff);

private:
   VecN m_vec;
   unsigned long m_dim;
   bool m_valid;
};

#endif

