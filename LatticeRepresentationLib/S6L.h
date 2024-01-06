#ifndef S6L_H
#define S6L_H

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
#include "LRL_indices.h"


#include <ostream>
#include <string>
#include <vector>

class LRL_Cell;
class B4;
class C3;
class D7;
class G6;
class S6;
class DC7u;

class S6L /*: private BasisBase<S6L> */ {
public:
   friend std::ostream& operator<< (std::ostream&, const S6L&);
   friend S6L operator* (const double d, const S6L& v);
   friend S6L operator/ (const double d, const S6L& v);
   friend class S6;

   S6L(void);
   S6L(const S6L& v);
   S6L(const double v[6]);
   S6L(const double d1, const double d2, const double d3, const double d4, const double d5, const double d6);
   S6L(const D7& v);
   S6L(const DC7u& dc7u);
   S6L(const D13& d13);
   S6L(const LRL_Cell& c);
   S6L(const VecN& v);
   S6L(const S6& ds);
   S6L(const C3& c3);
   S6L(const B4& dt);
   S6L(const std::string& s);
   S6L(const std::vector<double>& v);

   S6L& operator= (const S6L& v);
   S6L& operator= (const std::string& s);
   S6L& operator= (const D7& v);
   S6L& operator= (const DC7u& dc7u);
   S6L& operator= (const S6& v);
   S6L& operator= (const C3& c3);
   S6L& operator= (const B4& v);
   S6L& operator= (const LRL_Cell& v);
   S6L& operator/= (const double d);
   S6L& operator*= (const double d);
   S6L& operator+= (const S6L& v);
   S6L& operator-= (const S6L& v);
   S6L operator+ (const S6L& v) const;
   S6L operator- (const S6L& v) const;
   S6L operator* (const double d) const;
   S6L operator/ (const double d) const;
   S6L operator- (void) const; // unary
   bool operator== (const S6L& ri) const;
   bool operator!= (const S6L& ri) const;

   double operator[](const size_t n) const { return m_vec[n]; }
   double& operator[](const size_t n) { return m_vec[n]; }
   double DistanceBetween(const S6L& v1, const S6L& v2);
   size_t size(void) const { return 6; }
   double norm() const { return m_vec.norm(); }
   static double norm(const S6L& ri) { return ri.norm(); }
   double Norm() const { return m_vec.norm(); }
   static double Norm(const S6L& ri) { return ri.norm(); }

   S6L InverseRIVector(void) const;
   S6L sort(const S6L& ri);
   std::vector<double> GetVector(void) const { return m_vec.GetVector(); }
   double* data() const { return const_cast<double*>(m_vec.data()); }
   void SetVector(const std::vector<double>& v) { m_vec = v; }
   bool GetValid(void) const { return m_valid; }
   bool IsValid(void) const { return m_valid; }
   void SetValid(const bool b) { m_valid = b; }

   double at(const size_t n) const { return m_vec[n]; }
   static std::string GetName(void) { return "S6L"; }

   static S6L rand();
   static std::vector<S6L> ResetZeros(const std::vector<S6L>& vs);
   static S6 ResetZeros(const S6L& vs);

protected:
   VecN m_vec;
   size_t m_dim;
   bool m_valid;

   static std::vector<S6> DoSqrt(const std::vector<S6>& vs);
   static std::vector<S6> GenerateReflections(const S6& s6);
   static std::vector<S6> EliminateDuplicates(const std::vector<S6>& vs);
   static void OutputVector(const std::vector<S6>& vs);



};

#endif // S6L_H

