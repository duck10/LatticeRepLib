static const double randomLatticeNormalizationConstant = 10.0;
static const double randomLatticeNormalizationConstantSquared = randomLatticeNormalizationConstant*randomLatticeNormalizationConstant;



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
#include "rhrand.h"
#include "S6.h"
#include "Selling.h"
#include "LRL_StringTools.h"
#include "LRL_Vector3.h"

static int s6RandomSeedS6 = 19195;
static RHrand rhrandS6(s6RandomSeedS6);

S6::S6( void )
   : m_dim(6)
   , m_valid(true)
   , m_vec(VecN(6))
{
}

S6::S6(const S6& v)
   : m_dim(6)
   , m_valid(v.m_valid)
   , m_vec(v.m_vec)
{
}

S6::S6(const C3& c3)
   : S6()
{
   m_vec[0] = c3[0].real();
   m_vec[3] = c3[0].imag();
   m_vec[1] = c3[1].real();
   m_vec[4] = c3[1].imag();
   m_vec[2] = c3[2].real();
   m_vec[5] = c3[2].imag();
   m_valid = c3.GetValid();
}

S6::S6(const double v[6])
   : S6()
{
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
   : S6()
{
   static const double pi = 4.0*atan(1.0);
   static const double twopi = 2.0*pi;

   (*this) = G6(c);
   const bool btest1 = c.GetValid();
   const bool btest2 = (c[3] + c[4] + c[5])< twopi;
   m_valid = btest1 && c[3] < pi && c[4] < pi && c[5] < pi && btest2;
}

S6::S6(const D7& v7)
   : m_valid(v7.GetValid())
   , m_dim(6)
{
   (*this) = G6(v7);
}

S6::S6( const B4& del )
   : S6()
{
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

   m_valid = IsValid();
}

S6::S6(const VecN& v)
   : m_valid(false)
   , m_dim(0)
{
   if (v.size() == 6) {
      m_dim = 6;
      m_valid = true;
      m_vec = v;
      m_vec.SetValid(true);
   }
}

S6::S6(const G6& v6)
   : S6()
{
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
   m_valid = v6.GetValid();
}

S6::S6(const std::string& s)
   : S6()
{
   m_vec = LRL_StringTools::FromString(s);
   m_dim = m_vec.size();
   if (m_dim != 6)
   {
      throw "bad dimension in S6 from a string";
   }
}

S6::S6(const std::vector<double>& v)
   : S6()
{
   m_dim = 6;
   m_vec = v;
}

S6::S6(const double d1, const double d2, const double d3, const double d4, const double d5, const double d6)
{
   m_vec.resize(6);
   m_vec[0] = d1;
   m_vec[1] = d2;
   m_vec[2] = d3;
   m_vec[3] = d4;
   m_vec[4] = d5;
   m_vec[5] = d6;
   m_dim = 6;
   m_valid = IsValid();
}

bool S6::IsValid(const S6& s6)const {
   return s6.IsValid();
}

bool S6::IsValid(void) const {
   const size_t nPositive = CountPositive(*this);
   if (nPositive > 4) return false;
   if (CountZeros() > 3) return false;
   const double& p = (*this)[0];
   const double& q = (*this)[1];
   const double& r = (*this)[2];
   const double& s = (*this)[3];
   const double& t = (*this)[4];
   const double& u = (*this)[5];

   const double g1 = -s - r - q;
   const double g2 = -t - r - p;
   const double g3 = -u - q - p;
   const double g4 = 2.0*p;
   const double g5 = 2.0*q;
   const double g6 = 2.0*r;

   const double d_squared = -s -t -u;
   const double theta_a_squared = s * s / g1 / d_squared; // angle between d and a cosine squared
   const double theta_b_squared = t * t / g2 / d_squared;
   const double theta_c_squared = u * u / g3 / d_squared;

   return g1 > 0.001 && g2 > 0.001 && g3 > 0.001 && d_squared > 0.001 &&
      g4*g4 / g2 / g3 <= 4.0 &&
      g5*g5 / g1 / g3 <= 4.0 &&
      g6*g6 / g1 / g2 <= 4.0 &&
      (theta_a_squared <= 1.0) && (theta_b_squared <= 1.0) && (theta_c_squared <= 1.0);
}

size_t S6::CountZeros(void) const {
   size_t sum = 0;
   for (size_t i = 0; i < 6; ++i) sum += ((*this)[i] == 0.0) ? 1 : 0;
   return sum;
}

double S6::DistanceBetween( const S6& v1, const S6& v2 ) {
   return (v1-v2).norm();
}

double S6::AngleBetween( const S6& v1, const S6& v2 ) {
   const double norm1 = v1.norm( );
   const double norm2 = v2.norm( );
   const double dotproduct = VecN( v1.GetVector( ) ).dot( VecN( v2.GetVector( )));
   const double cosangle = dotproduct /norm1 / norm2;
   if (abs( cosangle ) > 1.0 && abs( cosangle ) - 1.0 < 1.0E-7) return cosangle > 0.0 ? acos( 1.0 ) : acos( -1.0 );
   const double returnvalue = acos( cosangle );
   return returnvalue;
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
   static const double pi = 4.0*atan(1.0);
   static const double twopi = 2.0*pi;
   (*this) = G6(v);
   m_valid = GetValid() && v.GetValid() && v[3] < pi && v[4] < pi && v[5] < pi && (v[3] + v[4] + v[5])< twopi;
   return *this;
}

S6 S6::operator- (void) const {
   S6 s6;
   for (size_t i = 0; i < 6; ++i)
      s6.m_vec[i] = -m_vec[i];
   return s6; // unary
}

S6& S6::operator+= (const S6& s6) {
   for (size_t i = 0; i < s6.size(); ++i)
      m_vec[i] += s6.m_vec[i];
   return *this;
}

S6& S6::operator-= (const S6& s6) {
   for (size_t i = 0; i < s6.size(); ++i)
      m_vec[i] -= s6.m_vec[i];
   return *this;
}

bool S6::operator== (const S6& s6) const {
   return m_vec == s6.m_vec;
}

bool S6::operator!= (const S6& s6) const {
   return !((*this) == s6);
}

bool S6::operator< (const S6& s2) const {
    const S6& s1(*this);
    const double delta =  1.0 + (1.0E-5 * DBL_EPSILON);

    for (size_t i = 0; i < 6; ++i) {
       if (s1[i] < delta*s2[i]) return true;
       if (s1[i] > s2[i]) return false;
    }
    return false;
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
   for (size_t i = 0; i < 6; ++i)
      dt[i] *= d;
   return dt;
}

S6 S6::operator/ (const double d) const {
   S6 dt(*this);
   for (size_t i = 0; i < 6; ++i)
      dt[i] /= d;
   return dt;
}

S6 S6::operator+ (const S6& ds) const {
   S6 v;
   for (size_t i = 0; i < 6; ++i)
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
   for( size_t i = 0; i < dc.size(); ++i)
      o << std::setw(9) << dc[i] << " ";
   o << std::setprecision(oldPrecision);
   o.unsetf(std::ios::floatfield);
   return o;
}

S6 operator* (const double d, const S6& ds) { // friend
   return ds*d;
}

bool S6::IsAllMinus() const {
   for (size_t i = 0; i < 6; ++i)
      if (m_vec[i] > 0.0) return false;
   return true;
}

S6 S6::InvertCoord(const size_t n, const S6& din) {
   S6 temp(din);
   temp[n] = -temp[n];
   return temp;
}

S6 S6::InvertCoord(const size_t n) const {
   S6 temp(*this);
   temp[n] = -temp[n];
   return temp;
}
std::string S6::Signature(const S6& s6) {
   std::string s;
   for (size_t i = 0; i < 6; ++i) {
      s.push_back((s6[i] <= 0.0) ? '-' : '+');
      if (i == 2) s.push_back(',');
   }
   return s;
}

void S6::SetSeed(const int n) { const int seedn = n;  rhrandS6.srandom(seedn); }

S6 S6::rand(const double d) {
   S6 s6(randDeloneReduced());

   const double choice = rhrandS6.urand();
   // the constants are chosen from results for 
   // random generation of valid cells !!!!!!!!!!!!!!!!
   // but they are somewhat random and can be adjusted.
   if (choice < 0.055) return s6;
   else if (choice < 0.68) return S6::RandomUnreduceOne(s6);
   else if ( choice < 0.995) return S6::RandomUnreduceTwo(s6);
   else return S6::RandomUnreduceThree(s6);
}

S6 S6::randDeloneReduced(const double d) {
   S6 s6;
   for (size_t i = 0; i < 6; ++i)
      s6[i] = -rhrandS6.urand()*randomLatticeNormalizationConstantSquared;
   s6.m_valid = true;
   return s6;
}

S6 S6::randDeloneUnreduced(const double d) {
   S6 s6(S6::rand());
   while (S6::CountPositive(s6) == 0) s6 = S6::rand();
   return s6;
}

size_t S6::CountPositive(const S6& s6) {
   size_t sum = 0;
   for (size_t i = 0; i < 6; ++i) sum += (s6[i] > 0.0) ? 1 : 0;
   return sum;
}

size_t S6::CountPositive(void) const {
   return CountPositive(*this);
}

S6 S6::FindBestMatchAmongReflections(const S6& s2) const {
   static const std::vector<MatS6> refl_one = MatS6::GetReflections();
   double bestDistance = DBL_MAX;
   S6 best;
   for (size_t i = 0; i < refl_one.size(); ++i) {
      const S6 testS6 = refl_one[i] * s2;
      const double test = (*this - testS6).norm();
      if (test < bestDistance) {
         best = testS6;
         bestDistance = test;
      }
   }
   return best;
}

S6 S6::RandomUnreduceOne(const S6& s6) {
   const double choice = 12.0*rhrandS6.urand();

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
   while (CountPositive(s) < 3)
      s = RandomUnreduceOne(s);
   return s;
}

std::vector<std::pair<MatS6, MatS6> > S6::SetUnreductionMatrices() {
   std::vector<std::pair<MatS6, MatS6> > vUnRed;

   // For unreducing scalar 11
   vUnRed.push_back(std::make_pair(MatS6(" -1 0 0 0 0 0    1 1 0 0 0 0    1 0 0 0 1 0    -1 0 0 1 0 0    1 0 1 0 0 0    1 0 0 0 0 1"),

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


std::vector< S6(*)(const S6&)> S6::SetReduceFunctions() {
   return SetUnreduceFunctions();
}

std::vector< S6(*)(const S6&)> S6::SetUnreduceFunctions() {
   std::vector<S6(*)(const S6&)> v;
   // The order is to agree with the reflection in D7Dist.h
   v.push_back(Unreduce11);
   //v.push_back(Unreduce12);
   v.push_back(Unreduce21);
   //v.push_back(Unreduce22);
   v.push_back(Unreduce31);
   //v.push_back(Unreduce32);
   v.push_back(Unreduce41);
   //v.push_back(Unreduce42);
   v.push_back(Unreduce51);
   //v.push_back(Unreduce52);
   //v.push_back(Unreduce61);
   v.push_back(Unreduce62);
   return v;
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
   // MatS6(�-1 0 0 0 0 0    1 1 0 0 0 0    1 0 0 0 1 0    -1 0 0 1 0 0    1 0 1 0 0 0    1 0 0 0 0 1�);
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
   // MatS6(�-1 0 0 0 0 0    1 0 0 0 0 1    1 0 1 0 0 0    -1 0 0 1 0 0    1 0 0 0 1 0    1 1 0 0 0 0�);
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
   // MatS6(�1 1 0 0 0 0    0 -1 0 0 0 0    0 1 0 1 0 0    0 1 1 0 0 0    0 -1 0 0 1 0    0 1 0 0 0 1�);
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
   // MatS6(�0 1 0 0 0 1    0 -1 0 0 0 0    0 1 1 0 0 0    0 1 0 1 0 0    0 -1 0 0 1 0    1 1 0 0 0 0�);
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
   // MatS6(�1 0 1 0 0 0    0 0 1 1 0 0    0 0 -1 0 0 0    0 1 1 0 0 0    0 0 1 0 1 0    0 0 -1 0 0 1�);
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
   // MatS6(�0 0 1 0 1 0    0 1 1 0 0 0    0 0 -1 0 0 0    0 0 1 1 0 0    1 0 1 0 0 0    0 0 -1 0 0 1�);
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
   // MatS6(�1 0 0 -1 0 0    0 0 1 1 0 0    0 1 0 1 0 0    0 0 0 -1 0 0    0 0 0 1 1 0    0 0 0 1 0 1�);
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
   // MatS6(�1 0 0 -1 0 0    0 1 0 1 0 0    0 0 1 1 0 0    0 0 0 -1 0 0    0 0 0 1 0 1    0 0 0 1 1 0�);
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
   // MatS6(�0 0 1 0 1 0    0 1 0 0 -1 0    1 0 0 0 1 0    0 0 0 1 1 0    0 0 0 0 -1 0    0 0 0 0 1 1�);
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
   // MatS6(�1 0 0 0 1 0    0 1 0 0 -1 0    0 0 1 0 1 0    0 0 0 0 1 1    0 0 0 0 -1 0    0 0 0 1 1 0�);
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
   // MatS6(�0 1 0 0 0 1    1 0 0 0 0 1    0 0 1 0 0 -1    0 0 0 1 0 1    0 0 0 0 1 1    0 0 0 0 0 -1�);
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
   // MatS6(�1 0 0 0 0 1    0 1 0 0 0 1    0 0 1 0 0 -1    0 0 0 0 1 1    0 0 0 1 0 1    0 0 0 0 0 -1�);
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

bool S6::IsValid(const std::pair<S6, S6>& p) {
   return p.first.IsValid() && p.second.IsValid();
}

bool S6::IsInvalidPair(const std::pair<S6, S6>& p) {
   return !(S6::IsValid(p));
}

std::vector< S6(*)(const S6&)> S6::SetRelectionFunctions() {
   std::vector<S6(*)(const S6&)> v;
   v.push_back(Relection01);
   v.push_back(Relection02);
   v.push_back(Relection03);
   v.push_back(Relection04);
   v.push_back(Relection05);
   v.push_back(Relection06);
   v.push_back(Relection07);
   v.push_back(Relection08);
   v.push_back(Relection09);
   v.push_back(Relection10);
   v.push_back(Relection11);
   v.push_back(Relection12);
   v.push_back(Relection13);
   v.push_back(Relection14);
   v.push_back(Relection15);
   v.push_back(Relection16);
   v.push_back(Relection17);
   v.push_back(Relection18);
   v.push_back(Relection19);
   v.push_back(Relection20);
   v.push_back(Relection21);
   v.push_back(Relection22);
   v.push_back(Relection23);
   v.push_back(Relection24);
   return v;
}

S6 S6::Relection01(const S6& din) {
   return din;
}

S6 S6::Relection02(const S6& din) {
   S6 d(din);
   std::swap(d.m_vec[0], d.m_vec[4]);
   std::swap(d.m_vec[1], d.m_vec[3]);
   return d;
}
S6 S6::Relection03(const S6& din) {
   S6 d(din);
   std::swap(d.m_vec[1], d.m_vec[2]);
   std::swap(d.m_vec[4], d.m_vec[5]);
   return d;
}

S6 S6::Relection04(const S6& din) {
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
   ss1 = s6;
   ss2 = s4;
   ss3 = s2;
   ss4 = s3;
   ss5 = s1;
   ss6 = s5;
   return d;
}

S6 S6::Relection05(const S6& din) {
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
   ss1 = s5;
   ss2 = s3;
   ss3 = s4;
   ss4 = s2;
   ss5 = s6;
   ss6 = s1;
   return d;
}

S6 S6::Relection06(const S6& din) {
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
   ss1 = s6;
   ss3 = s4;
   ss4 = s3;
   ss6 = s1;
   return d;
}

S6 S6::Relection07(const S6& din) {
   S6 d(din);
   std::swap(d.m_vec[1], d.m_vec[0]);
   std::swap(d.m_vec[3], d.m_vec[4]);
   return d;
}

S6 S6::Relection08(const S6& din) {
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
   ss1 = s4;
   ss2 = s5;
   ss4 = s1;
   ss5 = s2;
   return d;
}

S6 S6::Relection09(const S6& din) {
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
   ss1 = s2;
   ss2 = s3;
   ss3 = s1;
   ss4 = s5;
   ss5 = s6;
   ss6 = s4;
   return d;
}

S6 S6::Relection10(const S6& din) {
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
   ss1 = s6;
   ss2 = s5;
   ss3 = s1;
   ss4 = s3;
   ss5 = s2;
   ss6 = s4;
   return d;
}

S6 S6::Relection11(const S6& din) {
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
   ss1 = s4;
   ss2 = s3;
   ss3 = s5;
   ss4 = s1;
   ss5 = s6;
   ss6 = s2;
   return d;
}

S6 S6::Relection12(const S6& din) {
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
   ss1 = s6;
   ss2 = s1;
   ss3 = s5;
   ss4 = s3;
   ss5 = s4;
   ss6 = s2;
   return d;
}

S6 S6::Relection13(const S6& din) {
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
   ss1 = s3;
   ss2 = s1;
   ss3 = s2;
   ss4 = s6;
   ss5 = s4;
   ss6 = s5;
   return d;
}

S6 S6::Relection14(const S6& din) {
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
   ss1 = s4;
   ss2 = s6;
   ss3 = s2;
   ss4 = s1;
   ss5 = s3;
   ss6 = s5;
   return d;
}

S6 S6::Relection15(const S6& din) {
   S6 d(din);
   std::swap(d.m_vec[2], d.m_vec[0]);
   std::swap(d.m_vec[3], d.m_vec[5]);
   return d;
}

S6 S6::Relection16(const S6& din) {
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
   ss1 = s5;
   ss2 = s6;
   ss3 = s1;
   ss4 = s2;
   ss5 = s3;
   ss6 = s4;
   return d;
}

S6 S6::Relection17(const S6& din) {
   S6 d(din);
   std::swap(d.m_vec[0], d.m_vec[3]);
   std::swap(d.m_vec[2], d.m_vec[5]);
   return d;
}

S6 S6::Relection18(const S6& din) {
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
   ss1 = s5;
   ss2 = s1;
   ss3 = s6;
   ss4 = s2;
   ss5 = s4;
   ss6 = s3;
   return d;
}

S6 S6::Relection19(const S6& din) {
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
   ss1 = s3;
   ss2 = s5;
   ss3 = s4;
   ss4 = s6;
   ss5 = s2;
   ss6 = s1;
   return d;
}

S6 S6::Relection20(const S6& din) {
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
   ss1 = s2;
   ss2 = s6;
   ss3 = s4;
   ss4 = s5;
   ss5 = s3;
   ss6 = s1;
   return d;
}

S6 S6::Relection21(const S6& din) {
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
   ss1 = s3;
   ss2 = s4;
   ss3 = s5;
   ss4 = s6;
   ss5 = s1;
   ss6 = s2;
   return d;
}

S6 S6::Relection22(const S6& din) {
   S6 d(din);
   std::swap(d.m_vec[1], d.m_vec[5]);
   std::swap(d.m_vec[2], d.m_vec[4]);
   return d;
}

S6 S6::Relection23(const S6& din) {
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
   ss1 = s2;
   ss2 = s4;
   ss3 = s6;
   ss4 = s5;
   ss5 = s1;
   ss6 = s3;
   return d;
}

S6 S6::Relection24(const S6& din) {
   S6 d(din);
   std::swap(d.m_vec[1], d.m_vec[4]);
   std::swap(d.m_vec[2], d.m_vec[5]);
   return d;
}


//double AngularS6::AngleBetween(const AngularS6& s1, const AngularS6& s2) const {
//   double sum = 0.0;
//   for (size_t i = 0; i < 6; ++i) sum += s1.m_S6[i] * s2.m_S6[i];
//   const double cosine = sum;
//   const double sine = sqrt( abs(1.0 - sum*sum ));
//
//   const double angleInRadians = atan2( sine, cosine );
//   const double angleInDegrees = angleInRadians * 180.0 / (4.0 * atan( 1.0 ));
//   return angleInRadians;
//}
//
//AngularS6 AngularS6::operator- (const AngularS6& a) const {
//   AngularS6 as6;
//   as6.m_S6[0] = AngleBetween((*this), a);
//   return as6;
//}
