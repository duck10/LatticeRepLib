#ifndef V7_H
#define V7_H

#ifdef _MSC_VER
#define USE_LOCAL_HEADERS
#endif
#ifndef USE_LOCAL_HEADERS
//#include <rhrand.h>
#else
#include "rhrand.h"
#endif

#include "BasisBase.h"
#include "G6.h"
#include "VecN.h"
#include "LRL_indices.h"

class LRL_Cell;
class C3;
class S6;
class D7;
class B4;
class DC7u;

#include <ostream>
#include <string>


class V7 /*: private BasisBase<V7>*/
{
public:
   friend std::ostream& operator<< (std::ostream&, const V7&);
   friend V7 operator* (const double d, const V7& v);
   V7(void);
   V7(const G6& v);
   V7(const LRL_Cell& cell);
   V7(const VecN& v);
   V7(const S6& ds);
   V7(const C3& c3);
   V7(const double v[7]);
   V7(const std::string& s);
   explicit V7(const std::vector<double>& v);
   ~V7(void) {}
   V7& operator= (const G6& v);
   V7& operator= (const std::string& s);
   V7& operator= (const DC7u& dc7u);
   V7& operator= (const S6& v);
   V7& operator= (const C3& c3);
   V7& operator= (const LRL_Cell& v);
   V7& operator/= (const double d);
   V7& operator*= (const double d);
   V7& operator+= (const V7& v);
   V7& operator-= (const V7& v);
   V7 operator+ (const V7& v) const;
   V7 operator- (const V7& v) const;
   V7 operator* (const double d) const;
   V7 operator/ (const double d) const;
   V7& operator= (const V7& v);

   V7 operator- (void) const; // unary
   bool operator== (const V7& g6) const;
   bool operator!= (const V7& g6) const;

   double operator[](const size_t n) const { return m_vec[n]; }
   double& operator[](const size_t n) { return m_vec[n]; }
   double DistanceBetween(const V7& v1, const V7& v2) const;
   double norm() const { return m_vec.norm(); }
   double norm(const V7& v7) const { return v7.norm(); }
   double Norm() const { return m_vec.norm(); }
   double Norm(const V7& v7) const { return v7.norm(); }
   operator V7() const;
   std::vector<double> GetVector(void) const { return m_vec.GetVector(); }
   double* data() const { return const_cast<double*>(m_vec.data()); }
   void SetVector(const std::vector<double>& v) { m_vec = v; }
   bool GetValid(void) const { return m_valid; }
   bool IsValid(void) const { return m_valid; }
   void SetValid(const bool b) { m_valid = b; }

   double at(const size_t n) const { return m_vec[n]; }

   static V7 rand();
   static std::string GetName(void) { return "V7"; }
   size_t size() const { return 7; }

   static G6 ConvertV7ToG6(const V7& v7);
   static V7 ConvertG6ToV7(const G6& g6);
protected:

   VecN m_vec;
   size_t m_dim;
   bool m_valid;
};

#endif // V7_H
