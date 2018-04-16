#ifndef C3_H
#define C3_H

#include "BasisBase.h"

class LRL_Cell;
class B4;
class D7;
class S6;
class G6;

#include <complex>
#include <ostream>
#include <string>
#include <vector>

class C3 : private BasisBase<C3, std::complex<double> > {
public:
   friend std::ostream& operator<< (std::ostream&, const C3&);
   friend B4 operator* (const double d, const B4& dt);

   C3(void);
   C3(const C3& c3);
   C3(const B4& b4);
   C3(const G6& v6);
   C3(const S6& s6);
   C3(const D7& v7);
   C3(const LRL_Cell& c);
   C3(const VecN& v);
   C3(const std::string& c);
   C3(const std::vector<double>& v);
   C3(const std::complex<double>& c1, const std::complex<double>& c2, const std::complex<double>& c3);
   ~C3(void) {}

   C3& operator= (const C3& v);
   C3& operator= (const std::string& s);
   C3& operator= (const G6& v);
   C3& operator= (const S6& v);
   C3& operator= (const B4& v);
   C3& operator= (const D7& v);
   C3& operator= (const LRL_Cell& v);
   C3& operator/= (const double d);
   C3& operator*= (const double d);
   C3 operator+ (const C3& dt) const;
   C3 operator- (const C3& dt) const;
   C3& operator+= (const C3& dt);
   C3& operator-= (const C3& dt);
   C3 operator* (const double d) const;
   C3 operator/ (const double d) const;
   C3 operator- (void) const; // unary
   bool operator== (const C3& c3) const;
   bool operator!= (const C3& c3) const;
   std::complex<double> operator[](const unsigned long n) const;
   std::complex<double>& operator[](const unsigned long n);
   void report() const;
   operator S6() const;

   double DistanceBetween(const C3& v1, const C3& v2);
   unsigned long size(void) const { return (unsigned long)(m_c.size()); }
   double norm(void) const;
   double norm(const C3& c3) const;
   double Norm() const { return norm(); }
   double Norm(const C3& c3) const { return norm(c3); }

   std::vector<std::complex<double> > GetVector(void) const;
   void SetVector(const std::vector<std::complex<double> >& v);
   typedef std::vector<std::complex<double> > C3Type;
   std::complex<double>* data() const { return const_cast<std::complex<double>*>(&m_c[0]); }  //???????????????????????????????
   bool GetValid(void) const { return m_valid; }
   void SetValid(const bool b) { m_valid = b; }

   std::complex<double> at(const unsigned long n) const { return m_c[n]; }


   //static std::vector<C3(*)(const C3&)> SetUnreduceFunctions();



   static std::vector< C3(*)(const C3&)> SetReflections();

   static C3 Refl1(const C3& c);
   static C3 Refl2(const C3& c);
   static C3 Refl3(const C3& c);
   static C3 Refl4(const C3& c);
   static C3 Refl5(const C3& c);
   static C3 Refl6(const C3& c);
   static C3 Refl7(const C3& c);
   static C3 Refl8(const C3& c);
   static C3 Refl9(const C3& c);
   static C3 Refl10(const C3& c);
   static C3 Refl11(const C3& c);
   static C3 Refl12(const C3& c);
   static C3 Refl13(const C3& c);
   static C3 Refl14(const C3& c);
   static C3 Refl15(const C3& c);
   static C3 Refl16(const C3& c);
   static C3 Refl17(const C3& c);
   static C3 Refl18(const C3& c);
   static C3 Refl19(const C3& c);
   static C3 Refl20(const C3& c);
   static C3 Refl21(const C3& c);
   static C3 Refl22(const C3& c);
   static C3 Refl23(const C3& c);
   static C3 Refl24(const C3& c);

   static std::vector<C3(*)(const C3&)> SetReduceFunctions();
   static C3 Reduce1(const C3& c);
   static C3 Reduce2(const C3& c);
   static C3 Reduce3(const C3& c);
   static C3 Reduce4(const C3& c);
   static C3 Reduce5(const C3& c);
   static C3 Reduce6(const C3& c);

   static C3 rand();
   static C3 randDeloneReduced();
   static C3 randDeloneUnreduced();
   static C3 rand(const double d);
   static C3 randDeloneReduced(const double d);
   static C3 randDeloneUnreduced(const double d);

   bool IsAllMinus() const;
   static std::string GetName(void) { return "C3, Selling as complex"; }
   static void ComplexInvertSwap(std::complex<double>& a1, std::complex<double>& a2);

public:
   static std::vector< C3(*)(const C3&)> m_reductionFunctions;
   static std::vector< C3(*)(const C3&)> m_reflectionFunctions;

private:
   std::vector<std::complex<double> > m_c;
   bool m_valid;
};

#endif // C3_H