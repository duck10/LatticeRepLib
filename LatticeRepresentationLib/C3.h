#pragma warning (disable: 4100) // Visual Studio --  unreferenced formal parameter

#ifndef C3_H
#define C3_H

#include "BasisBase.h"
#include "S6.h"
#include "Selling.h"

class LRL_Cell;
class B4;
class D7;
class S6;
class G6;

#include <complex>
#include <ostream>
#include <string>
#include <vector>


/*
std::vector< S6(*)(const S6&)> FillReduceFunctionArray() {
   static std::vector< S6(*)(const S6&)> vf;
   if (vf.empty()) {
      // THESE ARE TRANSFORMATIONS IN S6 (I THINK), NOT NOT NOT IN G6 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      vf.push_back(&S6Dist::Reduce11);  // g or p
      vf.push_back(&S6Dist::Reduce21);  // h or q
      vf.push_back(&S6Dist::Reduce31);  // k or r
      vf.push_back(&S6Dist::Reduce41);  // l or s
      vf.push_back(&S6Dist::Reduce51);  // m or t
      vf.push_back(&S6Dist::Reduce61);  // n or u
   }
   return vf;

    static const std::vector< S6(*)(const S6&)>

   static S6 Reduce11(const S6& din);

S6 S6Dist::Reduce11(const S6& din) {
   return S6::Reduce11(din);
}

}
*/

class C3 : private BasisBase<C3, std::complex<double> > {
public:
   friend std::ostream& operator<< (std::ostream&, const C3&);
   friend C3 operator* (const double d, const C3& dt);

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

   static std::complex<double> c3_Minus_Real(std::complex<double>& a);
   static std::complex<double> c3_Minus_Imag(const std::complex<double>& a);
   static std::complex<double> c3_Plus_Real(const std::complex<double>& a);
   static std::complex<double> c3_Plus_Imag(const std::complex<double>& a);
   static double c3_Real(const std::complex<double>& a);
   static double c3_Imag(const std::complex<double>& a);

   static C3 c3_s1(const C3& a);

   static std::complex<double>  Exchange(const std::complex<double>& cin) {
      return std::complex<double>(cin.imag(), cin.real());
   }

   static void Test_C3();


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
   std::complex<double> operator[](const size_t n) const;
   std::complex<double>& operator[](const size_t n);
   void report() const;
   operator S6() const;

   double DistanceBetween(const C3& v1, const C3& v2);
   size_t size(void) const { return (size_t)(m_c.size()); }
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

   std::complex<double> at(const size_t n) const { return m_c[n]; }


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

   template<typename T>
   static C3 ConvertToFundamentalUnit(const T& tin) {
      C3 c3(tin);
      if (abs(c3[0]) > abs(c3[1])) std::swap(c3[0], c3[1]);
      if (abs(c3[1]) > abs(c3[2])) std::swap(c3[1], c3[2]);

      if (std::abs(c3[0].real()) > std::abs(c3[0].imag())) {
         c3[0] = std::complex<double>(c3[0].imag(), c3[0].real());
         c3[2] = std::complex<double>(c3[2].imag(), c3[2].real());
      };

      if (std::abs(c3[1].real()) > std::abs(c3[1].imag())) {
         c3[1] = std::complex<double>(c3[1].imag(), c3[1].real());
         c3[2] = std::complex<double>(c3[2].imag(), c3[2].real());
      };

      return c3;
   }

   template<typename T>
   static C3 ReduceToFundamentalUnit(const T& tin) {
      S6 vout;
      return ConvertToFundamentalUnit(C3(vout));
   }

private:
   static C3 SortC3(const C3& c3in);

public:
   static std::vector< C3(*)(const C3&)> m_reductionFunctions;
   static std::vector< C3(*)(const C3&)> m_reflectionFunctions;

private:
   std::vector<std::complex<double> > m_c;
   bool m_valid;
};


class C3Matrices {
public:
   //std::vector< S6(*)(const S6&)> FillReduceFunctionArray() {
   std::vector<std::complex<double>(*)(const std::complex<double>&) > ms1r;
   const std::vector<std::complex<double>(*)(const std::complex<double>&) > ms1rX = { &Mr, &Zero };
   C3Matrices()
   {
      ms1r.push_back(&Mr);
      ms1r.push_back(&Zero);
      ms1r.push_back(&Zero);
      ms1r.push_back(Pr);
      ms1r.push_back(&iR);
      ms1r.push_back(&R);
      ms1r.push_back(&Pr);
      ms1r.push_back(&iI);
      ms1r.push_back(&I);

      const S6 in("1 -20 -30 -100 -110 -120");
      S6 out;
      const C3 c3in(in);
      C3 c3out;

      size_t count = 0;
      for (size_t i = 0; i < ms1r.size(); i += 3) {
         c3out[count] = ms1r[i](c3in[0]) + ms1r[i + 1](c3in[1]) + ms1r[i + 2](c3in[2]);
         ++count;
      }
      const S6 sxxx = S6(C3::ReduceToFundamentalUnit(in));
      const C3 cxxx = S6(C3::ReduceToFundamentalUnit(c3out));
      std::cout << in << std::endl << std::endl;
      std::cout << S6(c3out) << std::endl << std::endl;
      std::cout << "the next two should be reduced to the C3 fundamental unit" << std::endl;
      std::cout << sxxx << std::endl;
      std::cout << S6(cxxx) << std::endl;
      std::cout << C3(sxxx) << std::endl;
      std::cout << (cxxx) << std::endl;

      //typedef std::complex<double>(*)(const std::complex<double>&) C3BoundaryTransform;
      {
         const std::vector <std::vector < std::vector < std::complex<double>(*)(const std::complex<double>&) > > >
            xx =
         {
            // For the boundary at s1: (the real component of c1).
            {
               { &Mr, &Zero, &Zero, &Pr, &iR, &R, &Pr, &iI, &I }, 
               { &Mr, &Zero, &Zero, &Pr, &iI, &I, &Pr, &iR, &R }, 
               { &Mr, &Zero, &Zero, &Pr, &R, &iR, &Pr, &I, &iI }, 
               { &Mr, &Zero, &Zero, &Pr, &I, &iI, &Pr, &R, &iR }
            },
            // For the boundary at s4: (the imaginary component of c1).
            {
               { &Mi, &Zero, &Zero, &Pi, &iR, &R, &Pi, &iI, &I }, 
               { &Mi, &Zero, &Zero, &Pi, &iI, &I, &Pi, &iR, &R }, 
               { &Mi, &Zero, &Zero, &Pi, &R, &iR, &Pi, &I, &iI }, 
               { &Mi, &Zero, &Zero, &Pi, &I, &iI, &Pi, &R, &iR }
            },
            // For the boundary at s2 (the real component of c2):
            {
               { &iR, &Pr, &R, &Zero, &Mr, &Zero, &iI, &Pr, &I }, 
               { &iI, &Pr, &I, &Zero, &Mr, &Zero, &iR, &Pr, &R }, 
               { &R, &Pr, &iR, &Zero, &Mr, &Zero, &I, &Pr, &iI }, 
               { &I, &Pr, &iI, &Zero, &Mr, &Zero, &R, &Pr, &iR }
            },
            // For the boundary at s5: (the imaginary component of c2).
            {
               { &iR, &Pi, &R, &Zero, &Mi, &Zero, &iI, &Pi, &I }, 
               { &iI, &Pi, &I, &Zero, &Mi, &Zero, &iR, &Pi, &R }, 
               { &R, &Pi, &iR, &Zero, &Mi, &Zero, &I, &Pi, &iI }, 
               { &I, &Pi, &iI, &Zero, &Mi, &Zero, &R, &Pi, &iR }
            },
            // For the boundary at s3 (the real component of c3):
            {
               { &iR, &R, &Pr, &iI, &I, &Pr, &Zero, &Zero, &Mr }, 
               { &iI, &I, &Pr, &iR, &R, &Pr, &Zero, &Zero, &Mr }, 
               { &R, &iR, &Pr, &I, &iI, &Pr, &Zero, &Zero, &Mr }, 
               { &I, &iI, &Pr, &R, &iR, &Pr, &Zero, &Zero, &Mr }
            },
            // For the boundary at s6 (the imaginary component of c3):
            {
               { &iR, &R, &Pi, &iI, &I, &Pi, &Zero, &Zero, &Mi }, 
               { &iI, &I, &Pi, &iR, &R, &Pi, &Zero, &Zero, &Mi }, 
               { &R, &iR, &Pi, &I, &iI, &Pi, &Zero, &Zero, &Mi }, 
               { &I, &iI, &Pi, &R, &iR, &Pi, &Zero, &Zero, &Mi }
            }
         };
      }
      exit(0);
   }

private:
   static std::complex<double> R(const std::complex<double>& in);
   static std::complex<double> I(const std::complex<double>& in);
   static std::complex<double> iR(const std::complex<double>& in);
   static std::complex<double> iI(const std::complex<double>& in);
   static std::complex<double> Mr(const std::complex<double>& in);
   static std::complex<double> Mi(const std::complex<double>& in);
   static std::complex<double> Pr(const std::complex<double>& in);
   static std::complex<double> Pi(const std::complex<double>& in);
   static std::complex<double> Zero(const std::complex<double>& in);
};

class MatC3 {
public:
   //MatC3() {
   //   m_functs[0] = c3_Minus_Real;
   //}

private:
   std::vector<std::complex<double>  (*) (const C3&)> m_functs;
};

#endif // C3_H
