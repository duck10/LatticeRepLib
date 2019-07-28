#ifndef D7_H
#define D7_H

#include "BasisBase.h"
#include "VecN.h"

#include <string>
#include <vector>
class LRL_Cell;
class C3;
class S6;
class B4;
class G6;

#include <ostream>
#include <string>

class D7 : private BasisBase<D7> {
public:
   friend std::ostream& operator<< ( std::ostream&, const D7& );
   friend D7 operator* (const double d, const D7& v);
   friend class LRL_Cell;
   friend class S6;
   friend class B4;
   friend class G6;

   D7( void );
   D7( const D7& v );
   D7( const double v[7] );
   D7( const G6& v );
   D7(const LRL_Cell& c);
   D7( const VecN& v );
   D7(const C3& c3);
   D7(const S6& ds);
   D7(const B4& dt);
   D7( const std::string& s );
   D7( const std::vector<double>& v );
   ~D7(void) {}

   D7& operator= (const D7& v);
   D7& operator= (const std::string& s);
   D7& operator= (const G6& v);
   D7& operator= (const C3& c3);
   D7& operator= (const S6& v);
   D7& operator= (const B4& v);
   D7& operator= (const LRL_Cell& v);
   D7& operator/= (const double d);
   D7& operator*= (const double d);
   D7& operator+= (const D7& v);
   D7& operator-= (const D7& v);
   D7 operator+ (const D7& v) const;
   D7 operator- (const D7& v) const;
   D7 operator* (const double d) const;
   D7 operator/ (const double d) const;
   D7 operator- (void) const; // unary
   bool operator== (const D7& d7) const;
   bool operator!= (const D7& d7) const;

   double operator[](const size_t n) const { return m_vec[n]; }
   double& operator[](const size_t n) { return m_vec[n]; }
   double DistanceBetween(const D7& v1, const D7& v2);
   std::vector<std::pair<std::string, std::string> > ClassifyVector(const double delta) const;
   size_t size(void) const { return 7; }
   double norm() const { return m_vec.norm(); }
   double norm(const D7& d7) const { return d7.norm(); }
   double Norm() const { return m_vec.norm(); }
   double Norm(const D7& d7) const { return norm(d7); }

   std::vector<double> GetVector(void) const { return m_vec.GetVector(); }
   double* data() const { return const_cast<double*>(m_vec.data()); }
   void SetVector(const std::vector<double>& v) { m_vec = v; }
   bool GetValid(void) const { return m_valid; }
   bool IsValid(void) const { return m_valid; }
   void SetValid(const bool b) {m_valid = b; }

   double at(const size_t n) const { return m_vec[n]; }

   std::pair<std::string, std::string> SummarizeBoundaries(const double delta) const;

   static D7 rand();
   static D7 randDeloneReduced();
   static D7 randDeloneUnreduced();
   static D7 rand(const double d);
   static D7 randDeloneReduced(const double d);
   static D7 randDeloneUnreduced(const double d);
   static std::string GetName(void) { return "D7"; }

   static std::pair<int, std::string> IdentifyNearbyBoundaries(const D7& v, const double cutoff);

private:
   VecN m_vec;
   size_t m_dim;
   bool m_valid;
};

#endif

