#ifndef C3_H
#define C3_H

#include <complex>

#include "S6.h"

class C3 {
public:
   friend std::ostream& operator<< (std::ostream&, const C3&);


   C3();
   C3(const S6& s6);
   unsigned long size(void) const { return (unsigned long)(m_c.size()); }
   std::complex<double> operator[](const unsigned long n) const;
   std::complex<double>& operator[](const unsigned long n);
   void report() const;
   operator S6() const;
   double norm(void) const;

   //static C3(*)(void) SetUnreduceFunctions();

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

   static C3 Reduce1(const C3& c);
   static C3 Reduce2(const C3& c);
   static C3 Reduce3(const C3& c);
   static C3 Reduce4(const C3& c);
   static C3 Reduce5(const C3& c);
   static C3 Reduce6(const C3& c);

   static std::vector<C3(*)(const C3&)> GetReduceFunctions();

private:
   std::vector<std::complex<double> > m_c;
   static std::vector< C3(*)(const C3&)> m_reductionFunctions;
};

#endif // C3_H