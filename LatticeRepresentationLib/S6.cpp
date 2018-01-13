
#include "stdafx.h"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>

#include "LRL_Cell.h"
#include "Delone.h"
#include "C3.h"
#include "B4.h"
#include "D7.h"
#include "G6.h"
#include "LRL_RandTools.h"
#include "S6.h"
#include "Selling.h"
#include "LRL_StringTools.h"
#include "LRL_Vector3.h"

static int s6RandomSeed = 19191;

const double pi = 4.0*atan(1.0);
const double twopi = 2.0*pi;

static RHrand rhrand(s6RandomSeed);


S6::S6( void )
   : m_dim(6)
   , m_valid(true)
   , m_vec(VecN(6))
{
   m_vec.resize(6);
   
}

S6::S6(const S6& v)
   : m_dim(6)
   , m_valid(true)
   , m_vec(VecN(6))
{
   m_vec = v.m_vec;
   m_valid = v.m_valid;
}
S6::S6(const C3& c3)
   : m_dim(6)
   , m_valid(true)
   , m_vec(VecN(6))
{
   m_vec[0] = c3[0].real();
   m_vec[1] = c3[0].imag();
   m_vec[2] = c3[1].real();
   m_vec[3] = c3[1].imag();
   m_vec[4] = c3[2].real();
   m_vec[5] = c3[2].imag();
   m_valid = c3.GetValid();
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
   m_valid = c.GetValid() && c[3] < pi && c[4] < pi && c[5] < pi && (c[3] + c[4] + c[5])< twopi;
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

S6& S6::operator= (const C3& c3) {
   *this = S6(c3);
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
   m_valid = GetValid() && v.GetValid() && v[3] < pi && v[4] < pi && v[5] < pi && (v[3] + v[4] + v[5])< twopi;
   return *this;
}

S6 S6::operator- (void) const {
   S6 s6;
   for (unsigned long i = 0; i < 6; ++i)
      s6.m_vec[i] = -m_vec[i];
   return s6; // unary
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

S6 S6::rand(void) {
   S6 s6(randDeloneReduced());

   const double choice = rhrand.urand();
   // the constants are chosen from results for 
   // random generation of valid cells !!!!!!!!!!!!!!!!
   // but they are somewhat random and can be adjusted.
   if (choice < 0.055) return s6;
   else if (choice < 0.68) return S6::RandomUnreduceOne(s6);
   else if ( choice < 0.995) return S6::RandomUnreduceTwo(s6);
   else return S6::RandomUnreduceThree(s6);
}

S6 S6::rand(const double d) {
   return d*rand() / LRL_Cell::randomLatticeNormalizationConstantSquared;
}

S6 S6::randDeloneReduced() {
   S6 s6;
   for (unsigned long i = 0; i < 6; ++i)
      s6[i] = -rhrand.urand() * LRL_Cell::randomLatticeNormalizationConstantSquared;
   s6.m_valid = true;
   return s6;
}

S6 S6::randDeloneUnreduced() {
   S6 s6(S6::rand());
   while (S6::CountPositive(s6) == 0) s6 = S6::rand();
   return s6;
}

S6 S6::randDeloneReduced(const double d) {
   return d*randDeloneReduced( ) / LRL_Cell::randomLatticeNormalizationConstantSquared;
}

S6 S6::randDeloneUnreduced(const double d) {
   return d*randDeloneUnreduced( ) / LRL_Cell::randomLatticeNormalizationConstantSquared;
}

unsigned long S6::CountPositive(const S6& s6) {
   unsigned long sum = 0;
   for (unsigned long i = 0; i < 6; ++i) sum += (s6[i] > 0.0) ? 1 : 0;
   return sum;
}

S6 S6::RandomUnreduceOne(const S6& s6) {
   const double choice = 12.0 * rhrand.urand();

   if (choice < 1) return S6::Unreduce11(s6);
   else if (choice < 2) return S6::Unreduce12(s6);
   else if (choice < 3) return S6::Unreduce21(s6);
   else if (choice < 4) return S6::Unreduce22(s6);
   else if (choice < 5) return S6::Unreduce31(s6);
   else if (choice < 6) return S6::Unreduce32(s6);
   else if (choice < 7) return S6::Unreduce41(s6);
   else if (choice < 8) return S6::Unreduce42(s6);
   else if (choice < 9) return S6::Unreduce51(s6);
   else if (choice < 10) return S6::Unreduce52(s6);
   else if (choice < 11) return S6::Unreduce61(s6);
   return S6::Unreduce62(s6);
}

S6 S6::RandomUnreduceTwo(const S6& s6) {
   S6 s(s6);
   while (CountPositive(s) < 2)
      s = RandomUnreduceOne(s);
   return s;
}

S6 S6::RandomUnreduceThree(const S6& s6) {
   S6 s(s6);
   while (CountPositive(s) < 3) {
      s = RandomUnreduceOne(s);
      if ( !LRL_Cell(s).GetValid()) s = S6( "-1 -1 -1 -1 -1 -1");
   }
   return s;
}

std::vector<std::pair<MatS6, MatS6> > S6::SetUnreductionMatrices() {
   return  S6::SetUnreductionMatrices();
   std::vector<std::pair<MatS6, MatS6> > vUnRed;

   // For unreducing scalar 11
   vUnRed.push_back(std::make_pair(MatS6(" -1 0 0 0 0 0    1 1 0 0 0 0    1 0 0 0 1 0 -1 0 0 1 0 0    1 0 1 0 0 0    1 0 0 0 0 1"),

      // For unreducing scalar 12
      MatS6(" -1 0 0 0 0 0    1 0 0 0 0 1    1 0 1 0 0 0 -1 0 0 1 0 0    1 0 0 0 1 0    1 1 0 0 0 0")));

   // For unreducing scalar 21
   vUnRed.push_back(std::make_pair(MatS6("1 1 0 0 0 0    0 -1 0 0 0 0    0 1 0 1 0 0    0 1 1 0 0 0    0 -1 0 0 1 0    0 1 0 0 0 1"),

      // For unreducing scalar 22
      MatS6("0 1 0 0 0 1    0 -1 0 0 0 0    0 1 1 0 0 0    0 1 0 1 0 0    0 -1 0 0 1 0    1 1 0 0 0 0")));

   // For unreducing scalar 31
   vUnRed.push_back(std::make_pair(MatS6("1 0 1 0 0 0    0 0 1 1 0 0    0 0 -1 0 0 0    0 1 1 0 0 0    0 0 1 0 1 0    0 0 -1 0 0 1"),

      // For unreducing scalar 32
      MatS6("0 0 1 0 1 0    0 1 1 0 0 0    0 0 -1 0 0 0    0 0 1 1 0 0    1 0 1 0 0 0    0 0 -1 0 0 1")));

   // For unreducing scalar 41
   vUnRed.push_back(std::make_pair(MatS6("1 0 0 -1 0 0    0 0 1 1 0 0    0 1 0 1 0 0    0 0 0 -1 0 0    0 0 0 1 1 0    0 0 0 1 0 1"),

      // For unreducing scalar 42
      MatS6("1 0 0 -1 0 0    0 1 0 1 0 0    0 0 1 1 0 0    0 0 0 -1 0 0    0 0 0 1 0 1    0 0 0 1 1 0")));

   // For unreducing scalar 51
   vUnRed.push_back(std::make_pair(MatS6("0 0 1 0 1 0    0 1 0 0 -1 0    1 0 0 0 1 0    0 0 0 1 1 0    0 0 0 0 -1 0    0 0 0 0 1 1"),

      // For unreducing scalar 52
      MatS6("1 0 0 0 1 0    0 1 0 0 -1 0    0 0 1 0 1 0    0 0 0 0 1 1    0 0 0 0 -1 0    0 0 0 1 1 0")));

   // For unreducing scalar 61
   vUnRed.push_back(std::make_pair(MatS6("0 1 0 0 0 1    1 0 0 0 0 1    0 0 1 0 0 -1    0 0 0 1 0 1    0 0 0 0 1 1    0 0 0 0 0 -1"),

      // For unreducing scalar 62
      MatS6("1 0 0 0 0 1    0 1 0 0 0 1    0 0 1 0 0 -1    0 0 0 0 1 1    0 0 0 1 0 1    0 0 0 0 0 -1")));

   return vUnRed;
}

S6 S6::Reduce11(const S6& din) {
   // reduce scale 1 non-zero
   //   vRed.push_back(std::make_pair(Inverse(MatS6("-1 0 0 0 0 0   1 1 0 0  0 0   1 0 0 0 1 0   -1 0 0 1 0 0   1 0 1 0 0 0   1 0 0 0 0 1")),
   S6 d(din);
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss1 = -s1;
   ss2 = s1 + s2;
   ss3 = s1 + s5;
   ss4 = -s1 + s4;
   ss5 = s1 + s3;
   ss6 = s1 + s6;
   return d;
}

S6 S6::Reduce12(const S6& din) {
   // reduce scale 1 non-zero
   //                                 Inverse(MatS6("-1 0 0 0 0 0   1 0 0 0  0 1   1 0 1 0 0 0   -1 0 0 1 0 0   1 0 0 0 1 0   1 1 0 0 0 0"))));  // g or p
   S6 d(din);
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss1 = -s1;
   ss2 = s1 + s6;
   ss3 = s1 + s3;
   ss4 = -s1 + s4;
   ss5 = s1 + s5;
   ss6 = s1 + s2;
   return d;
}

S6 S6::Reduce21(const S6& din) {
   // reduce scale 2 non-zero
   //   vRed.push_back(std::make_pair(Inverse(MatS6(" 1 1 0 0 0 0   0 -1 0 0 0 0   0 1 0 1 0 0   0 1 1 0 0 0   0 -1 0 0 1 0   0 1 0 0 0 1")),
   S6 d(din);
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss1 = s2 + s6;
   ss2 = -s2;
   ss3 = s2 + s3;
   ss4 = s2 + s4;
   ss5 = -s2 + s5;
   ss6 = s2 + s1;
   return d;
}

S6 S6::Reduce22(const S6& din) {
   // reduce scale 2 non-zero
   //                                 Inverse(MatS6(" 0 1 0 0 0 1   0 -1 0 0 0 0   0 1 1 0 0 0   0 1 0 1 0 0   0 -1 0 0 1 0   1 1 0 0 0 0"))));  // h or q
   S6 d(din);
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss1 = s2 + s6;
   ss2 = -s2;
   ss3 = s2 + s3;
   ss4 = s2 + s4;
   ss5 = -s2 + s5;
   ss6 = s2 + s1;
   return d;
}

S6 S6::Reduce31(const S6& din) {
   // reduce scale 3 non-zero
   //   vRed.push_back(std::make_pair(Inverse(MatS6(" 1 0 1 0 0 0   0  0 1 1 0 0   0 0 -1 0 0 0   0 1 1 0 0 0   0 0 1 0  1 0   0 0 -1 0 0 1")),
   S6 d(din);
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss1 = s3 + s1;
   ss2 = s3 + s4;
   ss3 = -s3;
   ss4 = s3 + s2;
   ss5 = s3 + s5;
   ss6 = -s3 + s6;
   return d;
}

S6 S6::Reduce32(const S6& din) {
   // reduce scale 3 non-zero
   //                                 Inverse(MatS6(" 0 0 1 0 1 0   0  1 1 0 0 0   0 0 -1 0 0 0   0 0 1 1 0 0   1 0 1 0  0 0   0 0 -1 0 0 1"))));  // k or r
   S6 d(din);
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss1 = s3 + s5;
   ss2 = s3 + s2;
   ss3 = -s3;
   ss4 = s3 + s4;
   ss5 = s3 + s1;
   ss6 = -s3 + s6;
   return d;
}

S6 S6::Reduce41(const S6& din) {
   // reduce scale 4 non-zero
   //   vRed.push_back(std::make_pair(Inverse(MatS6(" 1 0 0 -1 0 0   0 0 1 1 0 0   0 1 0 1 0 0   0 0 0 -1 0 0   0 0 0 1 1 0   0 0 0 1 0 1")),
   S6 d(din);
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss1 = -s4 + s1;
   ss2 = s4 + s3;
   ss3 = s4 + s2;
   ss4 = -s4;
   ss5 = s4 + s5;
   ss6 = s4 + s6;
   return d;
}

S6 S6::Reduce42(const S6& din) {
   // reduce scale 4 non-zero
   //                                 Inverse(MatS6(" 1 0 0 -1 0 0   0 1 0 1 0 0   0 0 1 1 0 0   0 0 0 -1 0 0   0 0 0 1 0 1   0 0 0 1 1 0"))));  // l or s
   S6 d(din);
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss1 = -s4 + s1;
   ss2 = s4 + s2;
   ss3 = s4 + s3;
   ss4 = -s4;
   ss5 = s4 + s6;
   ss6 = s4 + s5;
   return d;
}

S6 S6::Reduce51(const S6& din) {
   // reduce scale 5 non-zero
   //   vRed.push_back(std::make_pair(Inverse(MatS6(" 0 0 1 0 1 0   0 1 0 0 -1 0   1 0 0 0 1 0   0 0 0 1 1 0   0 0 0 0 -1 0   0 0 0 0 1 1")),
   S6 d(din);
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss1 = s5 + s3;
   ss2 = -s5 + s2;
   ss3 = s5 + s1;
   ss4 = s5 + s4;
   ss5 = -s5;
   ss6 = s5 + s6;
   return d;
}

S6 S6::Reduce52(const S6& din) {
   // reduce scale 5 non-zero
   //                                 Inverse(MatS6(" 1 0 0 0 1 0   0 1 0 0 -1 0   0 0 1 0 1 0   0 0 0 0 1 1   0 0 0 0 -1 0   0 0 0 1 1 0"))));  // m or t
   S6 d(din);
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss1 = s5 + s1;
   ss2 = -s5 + s2;
   ss3 = s5 + s3;
   ss4 = s5 + s6;
   ss5 = -s5;
   ss6 = s5 + s4;
   return d;
}

S6 S6::Reduce61(const S6& din) {
   // reduce scale 6 non-zero
   //   vRed.push_back(std::make_pair(Inverse(MatS6(" 0 1 0 0 0 1   1 0 0 0 0 1   0 0 1 0 0 -1   0 0 0 1 0 1   0 0 0 0 1 1   0 0 0 0 0 -1")),
   S6 d(din);
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss1 = s6 + s2;
   ss2 = s6 + s1;
   ss3 = -s6 + s3;
   ss4 = s6 + s4;
   ss5 = s6 + s5;
   ss6 = -s6;
   return d;
}

S6 S6::Reduce62(const S6& din) {
   // reduce scale 6 non-zero
   //                                 Inverse(MatS6(" 1 0 0 0 0 1   0 1 0 0 0 1   0 0 1 0 0 -1   0 0 0 0 1 1   0 0 0 1 0 1   0 0 0 0 0 -1"))));  // n or u
   S6 d(din);
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss1 = s6 + s1;
   ss2 = s6 + s2;
   ss3 = -s6 + s3;
   ss4 = s6 + s5;
   ss5 = s6 + s4;
   ss6 = -s6;
   return d;
}

S6 S6::Unreduce11(const S6& din) {
   // For unreducing scalar 11
   // MatS6(“-1 0 0 0 0 0    1 1 0 0 0 0    1 0 0 0 1 0    -1 0 0 1 0 0    1 0 1 0 0 0    1 0 0 0 0 1”);
   S6 d;
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss1 = -s1;
   ss2 = s1 + s2;
   ss3 = s1 + s5;
   ss4 = -s1 + s4;
   ss5 = s1 + s3;
   ss6 = s1 + s6;
   return d;
}

S6 S6::Unreduce12(const S6& din) {
   // For unreducing scalar 12
   // MatS6(“-1 0 0 0 0 0    1 0 0 0 0 1    1 0 1 0 0 0    -1 0 0 1 0 0    1 0 0 0 1 0    1 1 0 0 0 0”);
   S6 d;
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss1 = -s1;
   ss2 = s1 + s6;
   ss3 = s1 + s3;
   ss4 = -s1 + s4;
   ss5 = s1 + s5;
   ss6 = s1 + s2;
   return d;
}

S6 S6::Unreduce21(const S6& din) {
   // For unreducing scalar 21
   // MatS6(“1 1 0 0 0 0    0 -1 0 0 0 0    0 1 0 1 0 0    0 1 1 0 0 0    0 -1 0 0 1 0    0 1 0 0 0 1”);
   S6 d;
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss1 = s2 + s1;
   ss2 = -s2;
   ss3 = s2 + s4;
   ss4 = s2 + s3;
   ss5 = -s2 + s5;
   ss6 = s2 + s6;
   return d;
}

S6 S6::Unreduce22(const S6& din) {
   // For unreducing scalar 22
   // MatS6(“0 1 0 0 0 1    0 -1 0 0 0 0    0 1 1 0 0 0    0 1 0 1 0 0    0 -1 0 0 1 0    1 1 0 0 0 0”);
   S6 d;
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss1 = s2 + s6;
   ss2 = -s2;
   ss3 = s2 + s3;
   ss4 = s2 + s4;
   ss5 = -s2 + s5;
   ss6 = s2 + s1;
   return d;
}

S6 S6::Unreduce31(const S6& din) {
   // For unreducing scalar 31
   // MatS6(“1 0 1 0 0 0    0 0 1 1 0 0    0 0 -1 0 0 0    0 1 1 0 0 0    0 0 1 0 1 0    0 0 -1 0 0 1”);
   S6 d;
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss1 = s3 + s1;
   ss2 = s3 + s4;
   ss3 = -s3;
   ss4 = s3 + s2;
   ss5 = s3 + s5;
   ss6 = -s3 + s6;
   return d;
}

S6 S6::Unreduce32(const S6& din) {
   // For unreducing scalar 32
   // MatS6(“0 0 1 0 1 0    0 1 1 0 0 0    0 0 -1 0 0 0    0 0 1 1 0 0    1 0 1 0 0 0    0 0 -1 0 0 1”);
   S6 d;
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss1 = s3 + s5;
   ss2 = s3 + s2;
   ss3 = -s3;
   ss4 = s3 + s4;
   ss5 = s3 + s1;
   ss6 = -s3 + s6;
   return d;
}

S6 S6::Unreduce41(const S6& din) {
   // For unreducing scalar 41
   // MatS6(“1 0 0 -1 0 0    0 0 1 1 0 0    0 1 0 1 0 0    0 0 0 -1 0 0    0 0 0 1 1 0    0 0 0 1 0 1”);
   S6 d;
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss1 = -s4 + s1;
   ss2 = s4 + s3;
   ss3 = s4 + s2;
   ss4 = -s4;
   ss5 = s4 + s5;
   ss6 = s4 + s6;
   return d;
}

S6 S6::Unreduce42(const S6& din) {
   // For unreducing scalar 42
   // MatS6(“1 0 0 -1 0 0    0 1 0 1 0 0    0 0 1 1 0 0    0 0 0 -1 0 0    0 0 0 1 0 1    0 0 0 1 1 0”);
   S6 d;
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss1 = -s4 + s1;
   ss2 = s4 + s2;
   ss3 = s4 + s3;
   ss4 = -s4;
   ss5 = s4 + s6;
   ss6 = s4 + s5;
   return d;
}

S6 S6::Unreduce51(const S6& din) {
   // For unreducing scalar 51
   // MatS6(“0 0 1 0 1 0    0 1 0 0 -1 0    1 0 0 0 1 0    0 0 0 1 1 0    0 0 0 0 -1 0    0 0 0 0 1 1”);
   S6 d;
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss1 = s5 + s3;
   ss2 = -s5 + s2;
   ss3 = s5 + s1;
   ss4 = s5 + s4;
   ss5 = -s5;
   ss6 = s5 + s6;
   return d;
}

S6 S6::Unreduce52(const S6& din) {
   // For unreducing scalar 52
   // MatS6(“1 0 0 0 1 0    0 1 0 0 -1 0    0 0 1 0 1 0    0 0 0 0 1 1    0 0 0 0 -1 0    0 0 0 1 1 0”);
   S6 d;
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss1 = s5 + s1;
   ss2 = -s5 + s2;
   ss3 = s5 + s3;
   ss4 = s5 + s6;
   ss5 = -s5;
   ss6 = s5 + s4;
   return d;
}

S6 S6::Unreduce61(const S6& din) {
   // For unreducing scalar 61
   // MatS6(“0 1 0 0 0 1    1 0 0 0 0 1    0 0 1 0 0 -1    0 0 0 1 0 1    0 0 0 0 1 1    0 0 0 0 0 -1”);
   S6 d;
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss1 = s6 + s2;
   ss2 = s6 + s1;
   ss3 = -s6 + s3;
   ss4 = s6 + s4;
   ss5 = s6 + s5;
   ss6 = -s6;
   return d;
}

S6 S6::Unreduce62(const S6& din) {
   S6 d;
   // For unreducing scalar 62
   // MatS6(“1 0 0 0 0 1    0 1 0 0 0 1    0 0 1 0 0 -1    0 0 0 0 1 1    0 0 0 1 0 1    0 0 0 0 0 -1”);
   const double& s1 = din[0];
   const double& s2 = din[1];
   const double& s3 = din[2];
   const double& s4 = din[3];
   const double& s5 = din[4];
   const double& s6 = din[5];
   double& ss1 = d[0];
   double& ss2 = d[1];
   double& ss3 = d[2];
   double& ss4 = d[3];
   double& ss5 = d[4];
   double& ss6 = d[5];
   ss1 = s6 + s1;
   ss2 = s6 + s2;
   ss3 = -s6 + s3;
   ss4 = s6 + s5;
   ss5 = s6 + s4;
   ss6 = -s6;
   return d;
}


