

#include "stdafx.h"
#include "Delone.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

#include "C3.h"
#include "LRL_Cell.h"
#include "Delone.h"
#include "B4.h"
#include "D7.h"
#include "G6.h"
#include "S6.h"
#include "LRL_RandTools.h"
#include "C3.h"
#include "LRL_StringTools.h"

static int randSeed1 = 19191;
std::vector< C3(*)(const C3&)> C3::m_reductionFunctions = C3::GetReduceFunctions();

std::ostream& operator<< (std::ostream& o, const C3& v) {
   std::streamsize oldPrecision = o.precision();
   o << std::fixed << std::setprecision(3);
   for (unsigned long i = 0; i < v.size(); ++i)
      o << std::setw(9) << v[i] << " ";
   o << std::setprecision(oldPrecision);
   o.unsetf(std::ios::floatfield);
   return o;
}

C3::C3(void)
   : m_valid(true)
{
   m_c.resize(3);
   C3::m_reductionFunctions = C3::GetReduceFunctions();
}

C3::C3(const C3& c3)
   : m_valid(true)
{
   C3::m_reductionFunctions = C3::GetReduceFunctions();  m_c.resize(3);
   for (unsigned long i = 0; i < 3; ++i)
      m_c[i] = c3[i];
}

C3::C3(const std::complex<double>& c1, const std::complex<double>& c2, const std::complex<double>& c3) {
   C3::m_reductionFunctions = C3::GetReduceFunctions();  m_c.resize(3);
   m_c[0] = c1;
   m_c[1] = c2;
   m_c[2] = c3;
}

C3::C3(const LRL_Cell& c)
   : m_valid(true)
{
   C3::m_reductionFunctions = C3::GetReduceFunctions();  m_c.resize(3);
   (*this) = G6(c);
}

C3::C3(const D7& v7)
   : m_valid(true)
{
   C3::m_reductionFunctions = C3::GetReduceFunctions();   (*this) = G6(v7);
}

C3::C3( const B4& del )
   : m_valid(true)
{
   C3::m_reductionFunctions = C3::GetReduceFunctions();  (*this) = S6(del);
}

C3::C3(const VecN& v)
   : m_valid(false)
{
   if (v.size() == 6) {
      C3::m_reductionFunctions = C3::GetReduceFunctions();      m_c.resize(3);
      m_valid = true;
      for (unsigned long i = 0; i < C3::size(); ++i)
         m_c[i] = std::complex<double>(v[i], v[i + 3]);
   }
}

C3::C3(const std::string& s)
   : m_valid(false)
{
   VecN v = LRL_StringTools::FromString(s);
   if (v.size() == 6) {
      C3::m_reductionFunctions = C3::GetReduceFunctions();
      m_c.resize(3);
      m_valid = true;
      m_c[0] = std::complex<double>(v[0], v[1]);
      m_c[1] = std::complex<double>(v[2], v[3]);
      m_c[2] = std::complex<double>(v[4], v[5]);
   }
}

C3::C3(const G6& v6)
   : m_valid(true)
{
   C3::m_reductionFunctions = C3::GetReduceFunctions();
   (*this) = S6(v6);
}

C3::C3(const S6& s6)
   : m_valid(true)
{
   C3::m_reductionFunctions = C3::GetReduceFunctions();
   m_c.resize(3);
   m_c[0] = std::complex<double>(s6[0], s6[3]);
   m_c[1] = std::complex<double>(s6[1], s6[4]);
   m_c[2] = std::complex<double>(s6[2], s6[5]);
}

C3::C3(const std::vector<double>& v)
   : m_valid(false)
{
   if (v.size() == 6) {
      C3::m_reductionFunctions = C3::GetReduceFunctions();
      m_c.resize(3);
      m_valid = true;
      for (unsigned long i = 0; i < C3::size(); ++i)
         m_c[i] = std::complex<double>(v[i], v[i + 3]);
   }
}

double C3::DistanceBetween(const C3& v1, const C3& v2) {
   double sum = 0.0;
   for (unsigned long i = 0; i < C3::size(); ++i)  sum += std::abs(v1[i] - v2[i]) * std::abs(v1[i] - v2[i]);
   return sqrt(sum);
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

void C3::SetVector(const std::vector<std::complex<double> >& v) {
   m_c = v;
}

std::vector<std::complex<double>> C3::GetVector(void) const {
   std::vector<std::complex<double>> v((*this).m_c);
   return v;
}

std::complex<double> C3::operator[] (const unsigned long n) const {
   return m_c[n];
}

std::complex<double>& C3::operator[] (const unsigned long n) {
   return m_c[n];
}

C3& C3::operator= ( const C3& v ) {
   m_c = v.m_c;
   m_valid = v.m_valid;
   return *this;
}

C3& C3::operator= (const std::string& s)
{
   (*this) = C3(s);
   return *this;
}

C3& C3::operator= (const G6& v) {
   *this = C3(v);
   return *this;
}

C3& C3::operator= (const S6& v) {
   *this = C3(v);
   return *this;
}

C3& C3::operator= (const D7& v) {
   *this = C3(v);
   return *this;
}

C3& C3::operator= (const B4& v) {
   *this = C3(v);
   return *this;
}

C3& C3::operator= (const LRL_Cell& v) {
   (*this) = G6(v);
   return *this;
}

C3 C3::operator- (void) const {
   C3 temp;
   for (unsigned long i = 0; i < C3::size(); ++i) temp[i] = -m_c[i];
   return temp; // unary
}

C3& C3::operator+= (const C3& c3) {
   for (unsigned long i = 0; i < C3::size(); ++i)
      m_c[i] += c3.m_c[i];
   return *this;
}

C3& C3::operator-= (const C3& c3) {
   for (unsigned long i = 0; i < C3::size(); ++i)
      m_c[i] -= c3.m_c[i];
   return *this;
}

bool C3::operator== (const C3& c3) const {
   return m_c == c3.m_c;
}

bool C3::operator!= (const C3& C3) const {
   return !((*this) == C3);
}

C3& C3::operator/= (const double d) {
   for (unsigned long i=0; i<C3::size(); ++i)
      (*this).m_c[i] /= d;
   return *this;
}

C3& C3::operator*= (const double d) {
   for (unsigned long i = 0; i<C3::size(); ++i)
      (*this).m_c[i] *= d;
   return *this;
}

C3 C3::operator* (const double d) const {
   C3 c3(*this);
   for (unsigned long i = 0; i < C3::size(); ++i)
      c3[i] *= d;
   return c3;
}

C3 C3::operator/ (const double d) const {
   C3 c3(*this);
   for (unsigned long i = 0; i < C3::size(); ++i)
      c3[i] /= d;
   return c3;
}

C3 C3::operator+ (const C3& ds) const {
   C3 c3(*this);
   for (unsigned long i = 0; i < C3::size(); ++i)
      c3[i] += ds[i];
   return c3;
}

C3 C3::operator- (const C3& ds) const {
   C3 c3(*this);
   for (unsigned long i = 0; i < C3::size(); ++i)
      c3[i] -= ds[i];
   return c3;
}

C3 operator* (const double d, const C3& c3) { // friend
   return c3 * d;
}

double C3::norm(void) const {
   return sqrt(std::abs(m_c[0])*std::abs(m_c[0]) + std::abs(m_c[1])*std::abs(m_c[1]) + std::abs(m_c[2])*std::abs(m_c[2]));
}

double C3::norm(const C3& c3) const {
   return c3.norm();
}

void C3::report(void) const {
   for (unsigned long i = 0; i < C3::size(); ++i) {
      std::cout << i << std::endl;
      std::cout << m_c[i] << "  magnitude  " << std::abs(m_c[i]) << std::endl << std::endl;
   }
}


bool C3::IsAllMinus() const {
   for (unsigned long i = 0; i < C3::size(); ++i)
      if (m_c[i].real() > 0.0 || m_c[i].imag() > 0.0) return false;
   return true;
}

void SwapRealImag(std::complex<double>& c) {
   c = std::complex<double>(std::imag(c), std::real(c));
}

C3 C3::rand() {
   G6 g6;
   for (int i = 0; i < 6; ++i)
      g6[i] = randfg(randSeed1);

   const C3 C3(g6);
   const double randsNorm = g6.norm();
   return(C3 / randsNorm);
}

C3 C3::randDeloneReduced() {
   C3 vRan = C3(1000.0 * rand());

   while (!Delone::IsReduced(S6(vRan))) {
      vRan = 1000.0 * C3::rand();
   }
   return vRan;
}

C3 C3::randDeloneUnreduced() {
   C3 vRan = rand()*1000.0;

   while (Delone::IsReduced(S6(vRan))) {
      vRan = 1000.0*C3::rand();
   }
   return vRan;
}

C3 C3::rand(const double d) {
   return d*rand();
}

C3 C3::randDeloneReduced(const double d) {
   return d*randDeloneReduced();
}

C3 C3::randDeloneUnreduced(const double d) {
   return d*randDeloneUnreduced();
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
   co.m_c.resize(3);
   co[1] = std::complex<double>(c[1].real() + c[0].real(), c[2].real() + c[0].real());
   co[2] = std::complex<double>(c[1].imag() + c[0].real(), c[2].imag() + c[0].real());
   co[0] = std::complex<double>(-c[0].real(), c[0].imag() - c[0].real());
   return co;
}

C3 C3::Reduce2(const C3& c) {
   C3 co;
   co.m_c.resize(3);
   co[1] = std::complex<double>(c[1].real() + c[0].imag(), c[2].real() + c[0].imag());
   co[2] = std::complex<double>(c[1].imag() + c[0].imag(), c[2].imag() + c[0].imag());
   co[0] = std::complex<double>(c[0].real() - c[0].imag(), -c[0].imag());
   return co;
}

C3 C3::Reduce3(const C3& c) {
   C3 co;
   co.m_c.resize(3);
   co[0] = std::complex<double>(c[0].real() + c[1].real(), c[2].real() + c[1].real());
   co[2] = std::complex<double>(c[0].imag() + c[1].real(), c[2].imag() + c[1].real());
   co[1] = std::complex<double>(-c[1].real(), c[1].imag() - c[1].real());
   return co;
}

C3 C3::Reduce4(const C3& c) {
   C3 co;
   co.m_c.resize(3);
   co[0] = std::complex<double>(c[0].real() + c[1].imag(), c[2].real() + c[1].imag());
   co[2] = std::complex<double>(c[0].imag() + c[1].imag(), c[2].imag() + c[1].imag());
   co[1] = std::complex<double>(c[1].imag() - c[1].imag(), -c[1].imag());
   return co;
}

C3 C3::Reduce5(const C3& c) {
   C3 co;
   co.m_c.resize(3);
   co[0] = std::complex<double>(c[0].real() + c[2].real(), c[1].real() + c[1].real());
   co[1] = std::complex<double>(c[0].imag() + c[2].real(), c[1].imag() + c[1].real());
   co[2] = std::complex<double>(-c[2].real(), c[2].imag() - c[2].real());
   return co;
}

C3 C3::Reduce6(const C3& c) {
   C3 co;
   co.m_c.resize(3);
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
