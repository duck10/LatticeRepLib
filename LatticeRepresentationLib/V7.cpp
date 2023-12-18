#pragma warning( disable : 4189) //   local variable is initialized but not referenced



#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "LRL_Cell.h"
#include "Niggli.h"

#include "G6.h"
#include "LRL_RandTools.h"
#include "LRL_StringTools.h"
#include "rhrand.h"
#include "S6.h"
#include "V7.h"


V7::V7() {
   m_vec.resize(7);
   m_dim = 7;
   m_valid = true;
}
   //converting V7 to G6 (from HJB)

   //   The components of v7 are[v71, v72, v73, v74, v75, v76, v77] =
   //   [a, b, c, b * c * sinAlpha / v, a * c * sinBeta / v, a * b * sinGamma / v, v ^ (1 / 3)]

   //   Compute v = v77 ^ 3
   //   Compute sinAlpha = v74 / (v * b * c)
   //   Compute sinBeta = v75 / (v * a * c)
   //   Compute sinGamma = v76 / (v * a * b)
   //   Compute cosAlpha = sqrt(1. - sinAlpha * sinAlpha)
   //   Compute cosBeta = sqrt(1. - sinBeta * sinBeta)
   //   Compute cosGamma = sqrt(1. - sinGamma * sinGamma)

   //   Then compute G6 as[a ^ 2, b ^ 2, c ^ 2, b * c * cosAlpha, a * c * cosBeta, a * b * cosGamma)

V7::V7(const G6& g6)
: m_dim(7)
{
   *this = ConvertG6ToV7(g6);
}

V7::V7(const LRL_Cell& cell) {
   const G6 g6(cell);
   *this = ConvertG6ToV7(g6);
}


V7::V7(const std::vector<double>& v)
   : m_dim(v.size())
{
   m_vec.resize(7);
   if (v.size() < 7) {
      m_valid = false;
   }
   else
   {
      if (m_dim != 7) throw "bad dimension in V7 from a vector";
      m_vec = v;
      m_valid = true;
   }
}

double V7::DistanceBetween(const V7& v1, const V7& v2) const {
   double sum = 0.0;
   for (size_t i = 0; i < 6; ++i)  sum += (v1[i] - v2[i])*(v1[i] - v2[i]);
   return sqrt(sum);
}

V7& V7::operator= (const V7& v) {
   m_vec = v.m_vec;
   m_dim = v.m_dim;
   m_valid = v.m_valid;
   return *this;
}

V7& V7::operator= (const std::string& s)
{
   (*this) = G6(s);
   return *this;
}

V7& V7::operator= (const G6& v)
{
   (*this) = ConvertG6ToV7(v);
   m_vec.SetValid(v.GetValid());
   m_valid = v.GetValid();
   return *this;
}

V7& V7::operator= (const S6& v)
{
   (*this) = G6(v);
   m_vec.SetValid(v.GetValid());
   m_valid = v.GetValid();
   return *this;
}

//V7& V7::operator= (const C3& c3)
//{
//   (*this) = S6(c3);
//   m_vec.SetValid(c3.GetValid());
//   return *this;
//}

V7& V7::operator= (const LRL_Cell& v)
{
   static const double pi = 4.0*atan(1.0);
   static const double twopi = 2.0*pi;

   (*this) = G6(v);
   m_valid = v.GetValid() && v[3] < pi && v[4] < pi && v[5] < pi && (v[3] + v[4] + v[5])< twopi;
   return *this;
}

V7& V7::operator/= (const double d) {
   (*this).m_vec /= d;
   return *this;
}

V7& V7::operator*= (const double d) {
   (*this).m_vec *= d;
   return *this;
}

V7 V7::operator* (const double d) const {
   V7 v7(*this);
   v7.m_vec *= d;
   return v7;
}

V7 V7::operator/ (const double d) const {
   V7 v7(*this);
   v7.m_vec /= d;
   return v7;
}

V7 V7::operator- (void) const {
   V7 v7;
   v7.m_vec = -v7.m_vec;
   return *this; // unary
}

V7& V7::operator+= (const V7& v7) {
   for (size_t i = 0; i < v7.size(); ++i)
      m_vec[i] += v7.m_vec[i];
   return *this;
}

V7& V7::operator-= (const V7& g6) {
   for (size_t i = 0; i < g6.size(); ++i)
      m_vec[i] -= g6.m_vec[i];
   return *this;
}

bool V7::operator== (const V7& g6) const {
   return m_vec == g6.m_vec;
}

bool V7::operator!= (const V7& g6) const {
   return !((*this) == g6);
}

V7 V7::operator+ (const V7& v) const {
   V7 d;
   if ((*this).m_valid && v.m_valid) {
      d.m_vec = m_vec + v.m_vec;
   }
   else {
      d.m_valid = false;
   }
   return d;
}

V7 V7::operator- (const V7& v) const {
   V7 d;
   if ((*this).m_valid && v.m_valid) {
      d.m_vec = m_vec - v.m_vec;
   }
   else {
      d.m_valid = false;
   }
   return d;
}

//V7::operator V7() const {
//   return ConvertV7ToG6(*this);
//}


V7 V7::rand() {
   const S6 s6rand = S6::rand();
   return V7::ConvertG6ToV7(s6rand);;
}

std::ostream& operator<< (std::ostream& o, const V7& v) {
   std::streamsize oldPrecision = o.precision();
   o << std::fixed << std::setprecision(3);
   for (size_t i = 0; i < v.size(); ++i)
      o << std::setw(9) << v[i] << " ";
   o << std::setprecision(oldPrecision);
   o.unsetf(std::ios::floatfield);
   return o;
}

V7 V7::ConvertG6ToV7(const G6& g6) {
   G6 gred1;
   const bool b1 = Niggli::Reduce(g6, gred1);
   const G6 ginver = gred1.InverseG6Vector();
   G6 gred2;
   const bool b2 = Niggli::Reduce(ginver, gred2);
   V7 out;
   out.m_vec.resize(7);
   out.m_vec[0] = sqrt(gred1[0]);
   out.m_vec[1] = sqrt(gred1[1]);
   out.m_vec[2] = sqrt(gred1[2]);
   out.m_vec[3] = 1.0 / sqrt(gred2[0]);
   out.m_vec[4] = 1.0 / sqrt(gred2[1]);
   out.m_vec[5] = 1.0 / sqrt(gred2[2]);
   const LRL_Cell cell(gred1);
   const double volume = cell.Volume();
   out.m_vec[6] = pow(volume, 1.0 / 3.0);
   out.m_dim = 7;
   out.m_valid = true;
   return out;
}

G6 V7::ConvertV7ToG6(const V7& v) {
   const double& a = v[0];
   const double& b = v[1];
   const double& c = v[2];
   const double volume = v[6] * v[6] * v[6];
   const double sinAlpha = volume / b / c / v[3];
   const double sinBeta  = volume / a / c / v[4];
   const double sinGamma = volume / a / b / v[5];
   const double cosAlpha = sqrt(1.0 - sinAlpha * sinAlpha);
   const double cosBeta = sqrt(1.0 - sinBeta * sinBeta);
   const double cosGamma = sqrt(1.0 - sinGamma * sinGamma);

   const G6 g6(std::vector<double> {
      a* a, b* b, c* c,
         2.0 * b * c * cosAlpha, 2.0 * a * c * cosBeta, 2.0 * a * b * cosGamma});
   return g6;
}
