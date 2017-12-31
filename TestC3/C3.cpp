

#include "stdafx.h"

#include "C3.h"
#include "S6.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>

std::vector< C3(*)(const C3&)> C3::m_reductionFunctions = C3::GetReduceFunctions();

C3::C3(void)
   : m_c(3) 
{

}

C3::C3(const S6& s6)
   : m_c(3)
{
   for (unsigned long i = 0; i < 3; ++i)
      m_c[i] = std::complex<double>(s6[i], s6[3 + i]);
}

C3::operator S6(void) const {
   S6 s;
   s[0] = m_c[0].real();
   s[3] = m_c[0].imag();

   s[1] = m_c[1].real();
   s[4] = m_c[1].imag();

   s[2] = m_c[2].real();
   s[5] = m_c[2].imag();

   return s;
}

void C3::report(void) const {
   for (unsigned long i = 0; i < 3; ++i) {
      std::cout << i << std::endl;
      std::cout << m_c[i] << "  magnitude  " << std::abs(m_c[i]) << std::endl << std::endl;
   }
}

std::complex<double> C3::operator[](const unsigned long n) const
{
   return m_c[n];
}

std::complex<double>& C3::operator[](const unsigned long n)
{
   return m_c[n];
}

double C3::norm(void) const {
   return sqrt(std::abs(m_c[0])*std::abs(m_c[0]) + std::abs(m_c[1])*std::abs(m_c[1]) + std::abs(m_c[2])*std::abs(m_c[2]));
}

std::ostream& operator<< (std::ostream& o, const C3& v) {
   std::streamsize oldPrecision = o.precision();
   o << std::fixed << std::setprecision(3);
   for (unsigned long i = 0; i < v.size(); ++i)
      o << std::setw(9) << v[i] << " ";
   o << std::setprecision(oldPrecision);
   o.unsetf(std::ios::floatfield);
   return o;
}

void SwapRealImag(std::complex<double>& c) {
   c = std::complex<double>(std::imag(c), std::real(c));
}

C3 C3::Refl1(const C3& c) {
   return c;
}
C3 C3::Refl2(const C3& c) {
   C3 c3(c);
   std::swap(c3[1], c3[2]);
   return c3;
}
C3 C3::Refl3(const C3& c) {
   C3 c3(c);
   SwapRealImag(c3[1]);
   SwapRealImag(c3[2]);
   return c3;
}
C3 C3::Refl4(const C3& c) {
   C3 c3(c);
   SwapRealImag(c3[1]);
   SwapRealImag(c3[2]);
   std::swap(c3[1], c3[2]);
   return c3;
}
C3 C3::Refl5(const C3& c) {
   C3 c3(c);
   std::swap(c3[0], c3[1]);
   return c3;
}
C3 C3::Refl6(const C3& c) {
   C3 c3(c);
   std::swap(c3[0], c3[1]);
   std::swap(c3[1], c3[2]);
   return c3;
}
C3 C3::Refl7(const C3& c) {
   C3 c3(Refl5(c));
   SwapRealImag(c3[1]);
   SwapRealImag(c3[2]);
   return c3;
}
C3 C3::Refl8(const C3&c) {
   C3 c3(Refl6(c));
   SwapRealImag(c3[1]);
   SwapRealImag(c3[2]);
   return c3;
}

C3 C3::Refl9(const C3& c) {
   C3 c3(c);
   std::swap(c3[0], c3[2]);
   std::swap(c3[1], c3[2]);
   return c3;
}
C3 C3::Refl10(const C3& c) {
   C3 c3(c);
   std::swap(c3[0], c3[2]);
   return c3;
}
C3 C3::Refl11(const C3& c) {
   C3 c3(Refl9(c));
   SwapRealImag(c3[1]);
   SwapRealImag(c3[2]);
   return c3;
}
C3 C3::Refl12(const C3& c) {
   C3 c3(Refl10(c));
   SwapRealImag(c3[1]);
   SwapRealImag(c3[2]);
   return c3;
}
C3 C3::Refl13(const C3& c) {
   C3 c3(c);
   SwapRealImag(c3[0]);
   SwapRealImag(c3[2]);
   return c3;
}
C3 C3::Refl14(const C3& c) {
   C3 c3(c);
   std::swap(c3[1], c3[2]);
   SwapRealImag(c3[0]);
   SwapRealImag(c3[2]);
   return c3;
}
C3 C3::Refl15(const C3& c) {
   C3 c3(c);
   SwapRealImag(c3[0]);
   SwapRealImag(c3[1]);
   return c3;
}
C3 C3::Refl16(const C3& c) {
   C3 c3(c);
   std::swap(c3[1], c3[2]);
   SwapRealImag(c3[0]);
   SwapRealImag(c3[1]);
   return c3;
}
C3 C3::Refl17(const C3& c) {
   C3 c3(c);
   std::swap(c3[0], c3[1]);
   SwapRealImag(c3[0]);
   SwapRealImag(c3[2]);
   return c3;
}
C3 C3::Refl18(const C3& c) {
   C3 c3(c);
   std::swap(c3[0], c3[1]);
   std::swap(c3[1], c3[2]);
   SwapRealImag(c3[0]);
   SwapRealImag(c3[2]);
   return c3;
}
C3 C3::Refl19(const C3& c) {
   C3 c3(c);
   std::swap(c3[0], c3[1]);
   SwapRealImag(c3[0]);
   SwapRealImag(c3[1]);
   return c3;
}
C3 C3::Refl20(const C3& c) {
   C3 c3(c);
   std::swap(c3[0], c3[1]);
   std::swap(c3[1], c3[2]);
   SwapRealImag(c3[0]);
   SwapRealImag(c3[1]);
   return c3;
}
C3 C3::Refl21(const C3& c) {
   C3 c3(Refl9(c));
   SwapRealImag(c3[0]);
   SwapRealImag(c3[2]);
   return c3;
}
C3 C3::Refl22(const C3& c) {
   C3 c3(Refl10(c));
   SwapRealImag(c3[0]);
   SwapRealImag(c3[2]);
   return c3;
}
C3 C3::Refl23(const C3& c) {
   C3 c3(Refl9(c));
   SwapRealImag(c3[0]);
   SwapRealImag(c3[1]);
   return c3;
}
C3 C3::Refl24(const C3& c) {
   C3 c3(Refl10(c));
   SwapRealImag(c3[0]);
   SwapRealImag(c3[1]);
   return c3;
}

std::vector< C3(*)(const C3&)> C3::SetReflections() {
   std::vector<C3(*)(const C3&)> v;
   // The order is to agree with the reflection in D7Dist.h
   v.push_back(Refl1);
   v.push_back(Refl19);
   v.push_back(Refl2);
   v.push_back(Refl23);
   v.push_back(Refl18);
   v.push_back(Refl22);
   v.push_back(Refl5);
   v.push_back(Refl15);
   v.push_back(Refl6);
   v.push_back(Refl24);
   v.push_back(Refl14);
   v.push_back(Refl21);
   v.push_back(Refl9);
   v.push_back(Refl16);
   v.push_back(Refl10);
   v.push_back(Refl20);
   v.push_back(Refl13);
   v.push_back(Refl17);
   v.push_back(Refl12);
   v.push_back(Refl8);
   v.push_back(Refl11);
   v.push_back(Refl4);
   v.push_back(Refl7);
   v.push_back(Refl3);
   return v;
}


C3 C3::Reduce1(const C3& c) {
   C3 co;
   co[1] = std::complex<double>(c[1].real() + c[0].real(), c[2].real() + c[0].real());
   co[2] = std::complex<double>(c[1].imag() + c[0].real(), c[2].imag() + c[0].real());
   co[0] = std::complex<double>(-c[0].real(), c[0].imag() - c[0].real());
   return co;
}

C3 C3::Reduce2(const C3& c) {
   C3 co;
   co[1] = std::complex<double>(c[1].real() + c[0].imag(), c[2].real() + c[0].imag());
   co[2] = std::complex<double>(c[1].imag() + c[0].imag(), c[2].imag() + c[0].imag());
   co[0] = std::complex<double>(c[0].real() - c[0].imag(), -c[0].imag());
   return co;
}

C3 C3::Reduce3(const C3& c) {
   C3 co;
   co[0] = std::complex<double>(c[0].real() + c[1].real(), c[2].real() + c[1].real());
   co[2] = std::complex<double>(c[0].imag() + c[1].real(), c[2].imag() + c[1].real());
   co[1] = std::complex<double>(-c[1].real(), c[1].imag() - c[1].real());
   return co;
}

C3 C3::Reduce4(const C3& c) {
   C3 co;
   co[0] = std::complex<double>(c[0].real() + c[1].imag(), c[2].real() + c[1].imag());
   co[2] = std::complex<double>(c[0].imag() + c[1].imag(), c[2].imag() + c[1].imag());
   co[1] = std::complex<double>(c[1].imag() - c[1].imag(), -c[1].imag());
   return co;
}

C3 C3::Reduce5(const C3& c) {
   C3 co;
   co[0] = std::complex<double>(c[0].real() + c[2].real(), c[1].real() + c[1].real());
   co[1] = std::complex<double>(c[0].imag() + c[2].real(), c[1].imag() + c[1].real());
   co[2] = std::complex<double>(-c[2].real(), c[2].imag() - c[2].real());
   return co;
}

C3 C3::Reduce6(const C3& c) {
   C3 co;
   co[0] = std::complex<double>(c[0].real() + c[2].imag(), c[1].real() + c[2].imag());
   co[1] = std::complex<double>(c[0].imag() + c[2].imag(), c[1].imag() + c[2].imag());
   co[2] = std::complex<double>(c[2].real() - c[2].imag(), -c[2].imag());
   return co;
}

std::vector<C3(*)(const C3&)> C3::GetReduceFunctions()
{
   if (m_reductionFunctions.empty()) {
      m_reductionFunctions.push_back(C3::Reduce1);
      m_reductionFunctions.push_back(C3::Reduce2);
      m_reductionFunctions.push_back(C3::Reduce3);
      m_reductionFunctions.push_back(C3::Reduce4);
      m_reductionFunctions.push_back(C3::Reduce5);
      m_reductionFunctions.push_back(C3::Reduce6);
   }
   return m_reductionFunctions;
}
