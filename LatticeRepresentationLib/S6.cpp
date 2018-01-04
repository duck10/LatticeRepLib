
#include "stdafx.h"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>

#include "LRL_Cell.h"
#include "Delone.h"
#include "B4.h"
#include "D7.h"
#include "G6.h"
#include "LRL_RandTools.h"
#include "MatS6.h"
#include "S6.h"
#include "Selling.h"
#include "LRL_StringTools.h"
#include "LRL_Vector3.h"

static int randSeed1 = 19191;

S6::S6( void )
   : m_dim(6)
   , m_valid(true)
   , m_vec(VecN(6))
{
   m_vec.resize(6);
   
}

S6::S6( const S6& v ) 
   : m_dim(6)
   , m_valid(true)
   , m_vec(VecN(6))
{

   m_vec = v.m_vec;
   m_valid = v.m_valid;
}

S6::S6(const double v[6])
   : m_valid(true)
   , m_dim(6)
{
   m_vec.resize(6);
   double& p = (*this)[0];
   double& q = (*this)[1];
   double& r = (*this)[2];
   double& s = (*this)[3];
   double& t = (*this)[4];
   double& u = (*this)[5];

   const double& g1 = v[0];
   const double& g2 = v[1];
   const double& g3 = v[2];
   const double& g4 = v[3];
   const double& g5 = v[4];
   const double& g6 = v[5];

   p = g4 / 2.0;
   q = g5 / 2.0;
   r = g6 / 2.0;
   s = (-2.0*g1 - g6 - g5) / 2.0;
   t = (-g6 - 2.0*g2 - g4) / 2.0;
   u = (-g5 - g4 - 2.0*g3) / 2.0;
}

S6::S6(const LRL_Cell& c)
   : m_valid(true)
   , m_dim(6)
{
   m_vec.resize(6);
   (*this) = G6(c);
}

S6::S6(const D7& v7)
   : m_valid(true)
   , m_dim(6)
{
   (*this) = G6(v7);
}

S6::S6( const B4& del )
   : m_valid(true)
   , m_dim(6)
{
   m_vec.resize(6);
   const Vector_3 a = del[0UL];
   const Vector_3 b = del[1UL];
   const Vector_3 c = del[2UL];
   const Vector_3 d = del[3UL];
   double& p = (*this)[0];
   double& q = (*this)[1];
   double& r = (*this)[2];
   double& s = (*this)[3];
   double& t = (*this)[4];
   double& u = (*this)[5];

   p = b.Dot(c);
   q = a.Dot(c);
   r = a.Dot(b);
   s = a.Dot(d);
   t = b.Dot(d);
   u = c.Dot(d);
}

S6::S6(const VecN& v)
   : m_valid(false)
   , m_dim(0)
{
   if (v.size() == 6) {
      m_dim = 6;
      m_valid = true;
      m_vec = v;
   }
}

S6::S6(const G6& v6)
   : m_valid(true)
   , m_dim(6)
{
   m_vec.resize(6);
   const double& g1 = v6[0];
   const double& g2 = v6[1];
   const double& g3 = v6[2];
   const double& g4 = v6[3];
   const double& g5 = v6[4];
   const double& g6 = v6[5];

   double& p = (*this)[0];
   double& q = (*this)[1];
   double& r = (*this)[2];
   double& s = (*this)[3];
   double& t = (*this)[4];
   double& u = (*this)[5];

   p = g4 / 2.0;
   q = g5 / 2.0;
   r = g6 / 2.0;
   s = (-2.0*g1 - g6 - g5) / 2.0;
   t = (-g6 - 2.0*g2 - g4) / 2.0;
   u = (-g5 - g4 - 2.0*g3) / 2.0;
}

S6::S6(const std::string& s)
   : m_valid(true)
{
   m_vec.resize(6);
   m_vec = LRL_StringTools::FromString(s);
   m_dim = m_vec.size();
   if (m_dim != 6)
   {
      throw "bad dimension in S6 from a string";
   }
}

S6::S6(const std::vector<double>& v)
   : m_valid(true)
{
   m_vec.resize(6);
   m_dim = (unsigned long)(v.size());
   if (m_dim != 6) throw "bad dimension in D7 from a string";
   m_vec = v;
}

double S6::DistanceBetween( const S6& v1, const S6& v2 ) {
   double sum = 0.0;
   for( unsigned long i=0; i<6; ++i )  sum += (v1[i]-v2[i])*(v1[i]-v2[i]);
   return sqrt( sum );
}

S6& S6::operator= ( const S6& v ) {
   m_vec = v.m_vec;
   m_dim = v.m_dim;
   m_valid = v.m_valid;
   return *this;
}

S6& S6::operator= (const std::string& s)
{
   (*this) = S6(s);
   return *this;
}
S6& S6::operator= (const G6& v) {
   *this = S6(v);
   return *this;
}

S6& S6::operator= (const D7& v) {
   *this = S6(v);
   return *this;
}

S6& S6::operator= (const B4& v) {
   *this = S6(v);
   return *this;
}

S6& S6::operator= (const LRL_Cell& v) {
   (*this) = G6(v);
   return *this;
}

S6 S6::operator- (void) const {
   S6 s6;
   s6.m_vec = -s6.m_vec;
   return *this; // unary
}

S6& S6::operator+= (const S6& s6) {
   for (unsigned long i = 0; i < s6.size(); ++i)
      m_vec[i] += s6.m_vec[i];
   return *this;
}

S6& S6::operator-= (const S6& s6) {
   for (unsigned long i = 0; i < s6.size(); ++i)
      m_vec[i] -= s6.m_vec[i];
   return *this;
}

bool S6::operator== (const S6& s6) const {
   return m_vec == s6.m_vec;
}

bool S6::operator!= (const S6& s6) const {
   return !((*this) == s6);
}

S6& S6::operator/= (const double d) {
   (*this).m_vec /= d;
   return *this;
}

S6& S6::operator*= (const double d) {
   (*this).m_vec *= d;
   return *this;
}

S6 S6::operator* (const double d) const {
   S6 dt(*this);
   for (unsigned long i = 0; i < 6; ++i)
      dt[i] *= d;
   return dt;
}

S6 S6::operator/ (const double d) const {
   S6 dt(*this);
   for (unsigned long i = 0; i < 6; ++i)
      dt[i] /= d;
   return dt;
}

S6 S6::operator+ (const S6& ds) const {
   S6 v;
   for (unsigned long i = 0; i < 6; ++i)
      v[i] = (*this)[i] + ds[i];
   return v;
}

S6 S6::operator- (const S6& v) const {
   S6 d;
   if ((*this).m_valid && v.m_valid) {
      d.m_vec = m_vec - v.m_vec;
   }
   else {
      d.m_valid = false;
   }
   return d;
}

std::ostream& operator<< (std::ostream& o, const S6& dc) {
   std::streamsize oldPrecision = o.precision();
   o << std::fixed << std::setprecision(5);
   for( unsigned long i = 0; i < dc.size(); ++i)
      o << std::setw(9) << dc[i] << " ";
   o << std::setprecision(oldPrecision);
   o.unsetf(std::ios::floatfield);
   return o;
}

S6 operator* (const double d, const S6& ds) { // friend
   return ds * d;
}

bool S6::IsAllMinus() const {
   for (unsigned long i = 0; i < 6; ++i)
      if (m_vec[i] > 0.0) return false;
   return true;
}

S6 S6::rand() {
   S6 s6(LRL_Cell::rand());
   return s6 * LRL_Cell::randomLatticeNormalizationConstantSquared / s6.norm();
}

S6 S6::randDeloneReduced() {
   S6 s6(LRL_Cell::randDeloneReduced());
   return s6 * LRL_Cell::randomLatticeNormalizationConstantSquared / s6.norm();
}

S6 S6::randDeloneUnreduced() {
   S6 s6(LRL_Cell::randDeloneUnreduced());
   return s6 * LRL_Cell::randomLatticeNormalizationConstantSquared / s6.norm();
}

S6 S6::rand(const double d) {
   return d*rand() / LRL_Cell::randomLatticeNormalizationConstant;
}

S6 S6::randDeloneReduced(const double d) {
   return d*randDeloneReduced() / LRL_Cell::randomLatticeNormalizationConstant;
}

S6 S6::randDeloneUnreduced(const double d) {
   return d*randDeloneUnreduced( )/ LRL_Cell::randomLatticeNormalizationConstant;
}

