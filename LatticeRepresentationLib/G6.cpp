#pragma warning( disable : 4189) //   local variable is initialized but not referenced

#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "LRL_Cell.h"
#include "Delone.h"
#include "D7.h"
#include "DC7u.h"
#include "G6.h"
#include "LRL_RandTools.h"
#include "MatS6.h"
#include "MatD7.h"
#include "MatG6.h"
#include "rhrand.h"
#include "S6.h"
#include "Selling.h"
#include "LRL_StringTools.h"
#include "LRL_Vector3.h"

#define Default6 6

static int randSeed1 = 19191;

G6::G6(void) {
   m_vec.resize(6);
   m_dim = 6;
   m_valid = true;
}

G6::G6(const G6& v)
: G6()
{
   m_vec = v.m_vec;
   m_valid = v.m_valid;
}

G6::G6(const double v[6])
   : G6()
{
   m_vec[G6_AA_idx] = v[G6_AA_idx];
   m_vec[G6_BB_idx] = v[G6_BB_idx];
   m_vec[G6_CC_idx] = v[G6_CC_idx];
   m_vec[G6_2BC_idx] = v[G6_2BC_idx];
   m_vec[G6_2AC_idx] = v[G6_2AC_idx];
   m_vec[G6_2AB_idx] = v[G6_2AB_idx];
   m_dim = 6;
}

G6::G6(const LRL_Cell& c) {
   static const double pi = 4.0*atan(1.0);
   static const double twopi = 2.0*pi;
   m_dim = 6;

   m_vec.resize(6);
   m_valid = c.GetValid();
   m_vec[G6_AA_idx] = c[0] * c[0];
   m_vec[G6_BB_idx] = c[1] * c[1];
   m_vec[G6_CC_idx] = c[2] * c[2];
   m_vec[G6_2BC_idx] = 2.0 * c[1] * c[2] * cos(c[3]);
   m_vec[G6_2AC_idx] = 2.0 * c[0] * c[2] * cos(c[4]);
   m_vec[G6_2AB_idx] = 2.0 * c[0] * c[1] * cos(c[5]);
   for (size_t i = 3; i<6; ++i) if (std::fabs(m_vec[i]) < 1.0E-10) m_vec[i] = 0.0;

   bool b1 = c.GetValid();
   bool b2 = c[3] < pi;
   bool b3 = c[4] < pi;
   bool b4 = c[5] < pi;
   bool b5 = (c[3] + c[4] + c[5])< twopi;
   m_valid = b1 && b2 && b3 && b4 && b5;
   m_vec.SetValid(m_valid);
}

G6::G6(const S6& ds)
   : G6()
{
   const double& p = ds[0];
   const double& q = ds[1];
   const double& r = ds[2];
   const double& s = ds[3];
   const double& t = ds[4];
   const double& u = ds[5];

   double& g1 = m_vec[G6_AA_idx];
   double& g2 = m_vec[G6_BB_idx];
   double& g3 = m_vec[G6_CC_idx];
   double& g4 = m_vec[G6_2BC_idx];
   double& g5 = m_vec[G6_2AC_idx];
   double& g6 = m_vec[G6_2AB_idx];

   g1 = -s - r - q;
   g2 = -t - r - p;
   g3 = -u - q - p;
   g4 = 2.0*p;
   g5 = 2.0*q;
   g6 = 2.0*r;
   m_valid = g1 > 0.001 && g2 > 0.001 && g3 > 0.001 &&
      std::abs(g4*g4 / g2 / g3) <= 4.0 &&
      std::abs(g5*g5 / g1 / g3) <= 4.0 &&
      std::abs(g6*g6 / g1 / g2) <= 4.0 &&
      ds.GetValid();
   m_vec.SetValid(m_valid);
   m_dim = 6;

}

G6::G6(const C3& c3) {
   m_dim = 6;
   m_vec.resize(6);
   *this = S6(c3);
}

G6::G6(const B4& dt) {
   m_vec.resize(6);
   m_dim = 6;
   (*this) = S6(dt);
   m_valid = dt.GetValid();
}

G6::G6(const VecN& v) {
   m_dim = 0;
   m_valid = false;
   if (v.size() == 6) {
      m_dim = 6;
      m_valid = true;
      m_vec = v;
   }
}

G6::G6(const D7& v7) {
   m_vec.resize(6);
   double& g1 = m_vec[0];
   double& g2 = m_vec[1];
   double& g3 = m_vec[2];
   double& g4 = m_vec[3];
   double& g5 = m_vec[4];
   double& g6 = m_vec[5];
   const double& d1 = v7[0];
   const double& d2 = v7[1];
   const double& d3 = v7[2];
   const double& b4 = v7[3];
   const double& d5 = v7[4];
   const double& s6 = v7[5];
   const double& d7 = v7[6];
   g1 = d1;
   g2 = d2;
   g3 = d3;
   g4 = d5 - d2 - d3;
   g5 = s6 - d1 - d3;
   g6 = d7 - d1 - d2;
   //   g4 = (b4 -g1 -g2 -g3 -g5 -g6 + d5 - g2 -g3)/2.0; 
   m_valid = g1 > 0.001 && g2 > 0.001 && g3 > 0.001 && v7.GetValid();
   m_dim = 6;
}

G6::G6(const DC7u& dc7u) {
   double delta;
   double u, v, w, testsign;
   int error, ii;
   m_vec.resize(6);
   error=0;
   delta=std::fabs(dc7u[DC7u_AA_idx])*1.e-10;
   for (ii=0;ii<7;ii++) {
     if (dc7u[ii]<delta) error++;
   }
   if (dc7u[DC7u_AA_idx] > dc7u[DC7u_BB_idx]+delta ||
       dc7u[DC7u_BB_idx] > dc7u[DC7u_CC_idx]+delta ) error++;
   if (error > 0) {
     throw std::invalid_argument( "invalid unsorted DC7 cell" );
     m_valid=false;
   } else {
     m_vec[G6_AA_idx]=dc7u[DC7u_AA_idx];
     m_vec[G6_BB_idx]=dc7u[DC7u_BB_idx];
     m_vec[G6_CC_idx]=dc7u[DC7u_CC_idx];
     u=dc7u[DC7u_MIN_BC_diagsq_idx]
       -dc7u[DC7u_BB_idx]-dc7u[DC7u_CC_idx];
     v=dc7u[DC7u_MIN_AC_diagsq_idx]
       -dc7u[DC7u_AA_idx]-dc7u[DC7u_CC_idx];
     w=dc7u[DC7u_MIN_AB_diagsq_idx]
       -dc7u[DC7u_AA_idx]-dc7u[DC7u_BB_idx];
     testsign=dc7u[DC7u_MIN_ABC_diagsq_idx]
             -(m_vec[G6_AA_idx]
              +m_vec[G6_BB_idx]
              +m_vec[G6_CC_idx]+u+v+w);
     if ((std::fabs(testsign)<=delta)
        || (std::fabs(u)<=delta)
        || (std::fabs(v)<=delta)
        || (std::fabs(w)<=delta)) {
        m_vec[G6_2BC_idx]=u;
        m_vec[G6_2AC_idx]=v;
        m_vec[G6_2AB_idx]=w;
      } else {
        m_vec[G6_2BC_idx]=-u;
        m_vec[G6_2AC_idx]=-v;
        m_vec[G6_2AB_idx]=-w;
      }
      if (dc7u[DC7u_AA_idx]>dc7u[DC7u_BB_idx]+delta ||
        dc7u[DC7u_BB_idx]>dc7u[DC7u_CC_idx]+delta ) error++;
      if (error > 0) {
        for (ii=0;ii<6;ii++) m_vec[ii]=0.;
         m_valid=false;
      } else {
        m_vec[G6_AA_idx]=dc7u[DC7u_AA_idx];
        m_vec[G6_BB_idx]=dc7u[DC7u_BB_idx];
        m_vec[G6_CC_idx]=dc7u[DC7u_CC_idx];
        u=dc7u[DC7u_MIN_BC_diagsq_idx]
          -dc7u[DC7u_BB_idx]-dc7u[DC7u_CC_idx];
        v=dc7u[DC7u_MIN_AC_diagsq_idx]
          -dc7u[DC7u_AA_idx]-dc7u[DC7u_CC_idx];
        w=dc7u[DC7u_MIN_AB_diagsq_idx]
          -dc7u[DC7u_AA_idx]-dc7u[DC7u_BB_idx];
        testsign=dc7u[DC7u_MIN_ABC_diagsq_idx]
             -(m_vec[G6_AA_idx]
              +m_vec[G6_BB_idx]
              +m_vec[G6_CC_idx]+u+v+w);
        if ((std::fabs(testsign)<=delta)
            || (std::fabs(u)<=delta)
            || (std::fabs(v)<=delta)
            || (std::fabs(w)<=delta)) {
          m_vec[G6_2BC_idx]=u;
          m_vec[G6_2AC_idx]=v;
          m_vec[G6_2AB_idx]=w;
        } else {
          m_vec[G6_2BC_idx]=-u;
          m_vec[G6_2AC_idx]=-v;
          m_vec[G6_2AB_idx]=-w;
        }
        m_valid=true;
      }
    }
  }

G6::G6(const std::string& s) {
   m_vec.resize(6);
   m_vec = LRL_StringTools::FromString(s);
   m_dim = m_vec.size();
   m_valid = m_vec[0] > 0.0;
   if (m_dim != 6) throw "bad dimension in G6 from a string";
}

G6::G6(const std::vector<double>& v) {
   m_vec.resize(6);
   m_dim = v.size();
   if (m_dim != 6) throw "bad dimension in G6 from a vector";
   m_vec = v;
   m_valid = true;
}

double G6::DistanceBetween(const G6& v1, const G6& v2) {
   double sum = 0.0;
   for (size_t i = 0; i < 6; ++i)  sum += (v1[i] - v2[i])*(v1[i] - v2[i]);
   return sqrt(sum);
}

G6& G6::operator= (const G6& v) {
   m_vec = v.m_vec;
   m_dim = v.m_dim;
   m_valid = v.m_valid;
   return *this;
}

G6& G6::operator= (const std::string& s)
{
   (*this) = G6(s);
   return *this;
}

G6& G6::operator= (const D7& v)
{
   (*this) = G6(v);
   m_vec.SetValid(v.GetValid());
   return *this;
}

G6& G6::operator= (const S6& v)
{
   (*this) = G6(v);
   m_vec.SetValid(v.GetValid());
   m_valid = v.GetValid();
   return *this;
}

G6& G6::operator= (const C3& c3)
{
   (*this) = S6(c3);
   m_vec.SetValid(c3.GetValid());
   return *this;
}

G6& G6::operator= (const B4& v)
{
   (*this) = G6(v);
   return *this;
}


G6& G6::operator= (const LRL_Cell& v)
{
   static const double pi = 4.0*atan(1.0);
   static const double twopi = 2.0*pi;

   (*this) = G6(v);
   m_valid = v.GetValid() && v[3] < pi && v[4] < pi && v[5] < pi && (v[3] + v[4] + v[5])< twopi;
   return *this;
}

G6& G6::operator/= (const double d) {
   (*this).m_vec /= d;
   return *this;
}

G6& G6::operator*= (const double d) {
   (*this).m_vec *= d;
   return *this;
}

G6 G6::operator* (const double d) const {
   G6 g6(*this);
   g6.m_vec *= d;
   return g6;
}

G6 G6::operator/ (const double d) const {
   G6 g6(*this);
   g6.m_vec /= d;
   return g6;
}

G6 G6::operator- (void) const {
   G6 g6;
   g6.m_vec = -g6.m_vec;
   return *this; // unary
}

G6& G6::operator+= (const G6& g6) {
   for (size_t i = 0; i < g6.size(); ++i)
      m_vec[i] += g6.m_vec[i];
   return *this;
}

G6& G6::operator-= (const G6& g6) {
   for (size_t i = 0; i < g6.size(); ++i)
      m_vec[i] -= g6.m_vec[i];
   return *this;
}

bool G6::operator== (const G6& g6) const {
   return m_vec == g6.m_vec;
}

bool G6::operator!= (const G6& g6) const {
   return !((*this) == g6);
}

G6 G6::operator+ (const G6& v) const {
   G6 d;
   if ((*this).m_valid && v.m_valid) {
      d.m_vec = m_vec + v.m_vec;
   }
   else {
      d.m_valid = false;
   }
   return d;
}

G6 G6::operator- (const G6& v) const {
   G6 d;
   if ((*this).m_valid && v.m_valid) {
      d.m_vec = m_vec - v.m_vec;
   }
   else {
      d.m_valid = false;
   }
   return d;
}

G6 G6::InverseG6Vector(void) const {
   G6 v(*this);

   double a = sqrt(v[G6_AA_idx]);
   double b = sqrt(v[G6_BB_idx]);
   double c = sqrt(v[G6_CC_idx]);
   double cosalpha = 0.5*v[G6_2BC_idx] / v[G6_BB_idx] / v[G6_CC_idx];
   double cosbeta = 0.5*v[G6_2AC_idx] / v[G6_AA_idx] / v[G6_CC_idx];
   double cosgamma = 0.5*v[G6_2AB_idx] / v[G6_AA_idx] / v[G6_BB_idx];

   const double sinalpha = sqrt(1.0 - cosalpha*cosalpha);
   const double sinbeta = sqrt(1.0 - cosbeta *cosbeta);
   const double singamma = sqrt(1.0 - cosgamma*cosgamma);

   const double volume = a*b*c*sqrt(1.0 - cosalpha*cosalpha - cosbeta*cosbeta - cosgamma*cosgamma) -
      2.0*cosalpha*cosbeta*cosgamma;

   const double astar = b*c*sinalpha / volume;
   const double bstar = a*c*sinbeta / volume;
   const double cstar = a*b*singamma / volume;

   const double cosalphastar = (cosbeta *cosgamma - cosalpha) / std::fabs(sinbeta *singamma);
   const double cosbetastar = (cosalpha*cosgamma - cosbeta) / std::fabs(sinalpha*singamma);
   const double cosgammastar = (cosalpha*cosbeta - cosgamma) / std::fabs(sinalpha*sinbeta);

   G6 vout;
   vout[0] = astar*astar;
   vout[1] = bstar*bstar;
   vout[2] = cstar*cstar;
   vout[3] = 2.0*bstar*cstar*cosalphastar;
   vout[4] = 2.0*astar*cstar*cosbetastar;
   vout[5] = 2.0*astar*bstar*cosgammastar;

   return vout;
}

G6 G6::sort(const G6& g6) {
   G6 g(g6);
   bool again = true;
   while (again) {
      if (g[1] < g[0]) {
         again = true;
         std::swap(g[0], g[1]);
         std::swap(g[3], g[4]);
      }
      else if (g[2] < g[1]) {
         again = true;
         std::swap(g[1], g[2]);
         std::swap(g[4], g[5]);
      }
      else
         again = false;
   }
   return g;
}

std::vector<std::pair<std::string, std::string> > G6::ClassifyVector(const double delta) const {
   std::vector<std::pair<std::string, std::string> > vss;
   const std::pair<std::string, std::string> noop = std::make_pair(".", "");
   const double g1 = (*this)[0];
   const double g2 = (*this)[1];
   const double g3 = (*this)[2];
   const double g4 = (*this)[3];
   const double g5 = (*this)[4];
   const double g6 = (*this)[5];

   if (std::abs(g1 - g2) <= delta) {
      const std::string s = "1";
      vss.push_back(std::make_pair("1", "1"));
   }
   else {
      vss.push_back(noop);
   }

   if (std::abs(g2 - g3) <= delta) {
      const std::string s = "2";
      vss.push_back(std::make_pair("2", "2"));
   }
   else {
      vss.push_back(noop);
   }

   if (std::abs(g4) <= delta) {
      const std::string s = "3";
      vss.push_back(std::make_pair("3", "3"));
   }
   else {
      vss.push_back(noop);
   }

   if (std::abs(g5) <= delta) {
      const std::string s = "4";
      vss.push_back(std::make_pair("4", "4"));
   }
   else {
      vss.push_back(noop);
   }

   if (std::abs(g6) <= delta) {
      const std::string s = "5";
      vss.push_back(std::make_pair("5", "5"));
   }
   else {
      vss.push_back(noop);
   }

   if ((std::abs(g2 - g4) <= delta) && ((g5 - g6) >= -delta)) {
      const std::string s = "6";
      vss.push_back(std::make_pair("6", "6"));
   }
   else {
      vss.push_back(noop);
   }

   if ((std::abs(g2 - g4) <= delta) + ((g5 - g6) <= delta)) {
      const std::string s = "7";
      vss.push_back(std::make_pair("7", "7"));
   }
   else {
      vss.push_back(noop);
   }

   if (std::abs(g2 + g4) <= delta) {
      const std::string s = "8";
      vss.push_back(std::make_pair("8", "8"));
   }
   else {
      vss.push_back(noop);
   }

   if ((std::abs(g1 - g5)) <= delta && ((g4 - g6) >= -delta)) {
      const std::string s = "9";
      vss.push_back(std::make_pair("9", "9"));
   }
   else {
      vss.push_back(noop);
   }

   if ((std::abs(g1 - g5)) <= delta && ((g4 - g6) <= delta)) {
      const std::string s = "A";
      vss.push_back(std::make_pair("A", "A"));
   }
   else {
      vss.push_back(noop);
   }

   if (std::abs(g1 + g5) <= delta) {
      const std::string s = "B";
      vss.push_back(std::make_pair("B", "B"));
   }
   else {
      vss.push_back(noop);
   }

   if ((std::abs(g1 - g6) <= delta) && ((g4 - g5) >= -delta)) {
      const std::string s = "C";
      vss.push_back(std::make_pair("C", "C"));
   }
   else {
      vss.push_back(noop);
   }

   if ((std::abs(g1 - g6) <= delta) && ((g4 - g5) <= delta)) {
      const std::string s = "D";
      vss.push_back(std::make_pair("D", "D"));
   }
   else {
      vss.push_back(noop);
   }

   if (std::abs(g1 + g6) <= delta) {
      const std::string s = "E";
      vss.push_back(std::make_pair("E", "E"));
   }
   else {
      vss.push_back(noop);
   }

   if (std::abs(g1 + g2 + g3 + g4 + g5 + g6 - g3) <= delta) {
      const std::string s = "F";
      vss.push_back(std::make_pair("F", "F"));
   }
   else {
      vss.push_back(noop);
   }

   return vss;
}

G6 G6::rand() {
   return S6::rand();
}

G6 G6::randDeloneReduced() {
   return S6::randDeloneReduced();
}

G6 G6::randDeloneUnreduced() {
   return S6::randDeloneUnreduced();
}

G6 G6::rand(const double d) {
   return d*rand() / LRL_Cell::randomLatticeNormalizationConstantSquared;
}

G6 G6::randDeloneReduced(const double d) {
   return d*randDeloneReduced() / LRL_Cell::randomLatticeNormalizationConstantSquared;
}

G6 G6::randDeloneUnreduced(const double d) {
   return d*randDeloneUnreduced( )/ LRL_Cell::randomLatticeNormalizationConstantSquared;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::pair<int, std::string> G6::IdentifyNearbyBoundaries(const G6& v, const double cutoff)
/*-------------------------------------------------------------------------------------*/
{
   std::string s1, s2;

   if (std::abs(v[0] - v[1]) < cutoff)
   {
      s1 += '1';
      s2 += '1';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(v[1] - v[2]) < cutoff)
   {
      s1 += '2';
      s2 += '2';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(v[3]) < cutoff)
   {
      s1 += '3';
      s2 += '3';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(v[4]) < cutoff)
   {
      s1 += '4';
      s2 += '4';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(v[5]) < cutoff)
   {
      s1 += '5';
      s2 += '5';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(v[1] - v[3]) < cutoff &&
      (v[5] - v[4]) < cutoff)
   {
      s1 += '6';
      s2 += '6';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(v[1] - v[3]) < cutoff &&
      (v[4] - v[5]) < cutoff)
   {
      s1 += '7';
      s2 += '7';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(v[1] + v[3]) < cutoff)
   {
      s1 += '8';
      s2 += '8';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(v[0] - v[4]) < cutoff &&
      (v[5] - v[3]) < cutoff)
   {
      s1 += '9';
      s2 += '9';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(v[0] - v[4]) < cutoff &&
      (v[3] - v[5]) < cutoff)
   {
      s1 += 'A';
      s2 += 'A';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(v[0] + v[4]) < cutoff)
   {
      s1 += 'B';
      s2 += 'B';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(v[0] - v[5]) < cutoff &&
      (v[4] - v[3]) < cutoff)
   {
      s1 += 'C';
      s2 += 'C';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(v[0] - v[5]) < cutoff &&
      (v[3] - v[4]) < cutoff)
   {
      s1 += 'D';
      s2 += 'D';
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(v[0] + v[5]) < cutoff)
   {
      s1 += 'E';
      s2 += 'E';
   }
   else
   {
      s2 += '.';
   }

   if (abs(v[0] + v[1] + v[3] + v[4] + v[5]) < cutoff)
   {
      s1 += 'F';
      s2 += 'F';
   }
   else
   {
      s2 += '.';
   }

   return std::make_pair(-1, (s1.empty()) ? "" : "(" + s1 + ") \t(" + s2 + ")");
}

static bool TestCase1(const G6& g) { return g[0] == g[1]; }
static bool TestCase2(const G6& g) { return g[1] == g[2]; }
static bool TestCase3(const G6& g) { return g[3] == 0; }
static bool TestCase4(const G6& g) { return g[4] == 0; }
static bool TestCase5(const G6& g) { return g[5] == 0; }
static bool TestCase6(const G6& g) { return (g[1] == g[3]) && (g[4] >= g[5]); }
static bool TestCase7(const G6& g) { return (g[1] == g[3]) && (g[4] < g[5]); }
static bool TestCase8(const G6& g) { return g[1] == -g[3]; }
static bool TestCase9(const G6& g) { return (g[0] == g[4]) && (g[3] >= g[5]); }
static bool TestCaseA(const G6& g) { return (g[0] == g[4]) && (g[3] < g[5]); }
static bool TestCaseB(const G6& g) { return g[0] == -g[4]; }
static bool TestCaseC(const G6& g) { return (g[0] == g[5]) && (g[3] >= g[4]); }
static bool TestCaseD(const G6& g) { return (g[0] == g[5]) && (g[3] < g[4]); }
static bool TestCaseE(const G6& g) { return g[0] == -g[5]; }
static bool TestCaseF(const G6& g) {
   return (g[0] + g[1] + g[2] + g[3] + g[4] + g[5]) == g[2];
}

bool G6::TestBoundaryDetection(void) {
   bool b1 = true;
   b1 = b1 && TestCase1(G6("1 1 0 0 0 0"));
   b1 = b1 && TestCase2(G6("0 1 1 0 0 0"));
   b1 = b1 && TestCase3(G6("1 2 3 0 5 6"));
   b1 = b1 && TestCase4(G6("1 2 3 4 0 6"));
   b1 = b1 && TestCase5(G6("1 2 3 4 5 0"));
   b1 = b1 && TestCase6(G6("0 1 0 1 9 3"));
   b1 = b1 && TestCase7(G6("0 1 0 1 3 9"));
   b1 = b1 && TestCase8(G6("0 1 0 -1 0 0"));
   b1 = b1 && TestCase9(G6("1 0 0 3 1 2"));
   b1 = b1 && TestCaseA(G6("1 0 0 2 1 3"));
   b1 = b1 && TestCaseB(G6("1 0 0 0 -1 0"));
   b1 = b1 && TestCaseC(G6("1 0 0 9 2 1"));
   b1 = b1 && TestCaseD(G6("1 0 0 2 9 1"));
   b1 = b1 && TestCaseE(G6("1 0 0 0 0 -1"));
   b1 = b1 && TestCaseF(G6("1 3 0 5 7 -16"));

   std::vector<std::pair<std::string, std::string> > s1 = G6("1 1 0 0 0 0"  ).ClassifyVector(0.0);
   std::vector<std::pair<std::string, std::string> > s2 = G6("0 1 1 0 0 0"  ).ClassifyVector(0.0);
   std::vector<std::pair<std::string, std::string> > s3 = G6("1 2 3 0 5 6"  ).ClassifyVector(0.0);
   std::vector<std::pair<std::string, std::string> > s4 = G6("1 2 3 4 0 6"  ).ClassifyVector(0.0);
   std::vector<std::pair<std::string, std::string> > s5 = G6("1 2 3 4 5 0"  ).ClassifyVector(0.0);
   std::vector<std::pair<std::string, std::string> > s6 = G6("0 1 0 1 9 3"  ).ClassifyVector(0.0);
   std::vector<std::pair<std::string, std::string> > s7 = G6("0 1 0 1 3 9"  ).ClassifyVector(0.0);
   std::vector<std::pair<std::string, std::string> > s8 = G6("0 1 0 -1 0 0" ).ClassifyVector(0.0);
   std::vector<std::pair<std::string, std::string> > s9 = G6("1 0 0 3 1 2"  ).ClassifyVector(0.0);
   std::vector<std::pair<std::string, std::string> > sA = G6("1 0 0 2 1 3"  ).ClassifyVector(0.0);
   std::vector<std::pair<std::string, std::string> > sB = G6("1 0 0 0 -1 0" ).ClassifyVector(0.0);
   std::vector<std::pair<std::string, std::string> > sC = G6("1 0 0 9 2 1"  ).ClassifyVector(0.0);
   std::vector<std::pair<std::string, std::string> > sD = G6("1 0 0 2 9 1"  ).ClassifyVector(0.0);
   std::vector<std::pair<std::string, std::string> > sE = G6("1 0 0 0 0 -1" ).ClassifyVector(0.0);
   std::vector<std::pair<std::string, std::string> > sF = G6("1 3 0 5 7 -16").ClassifyVector(0.0);

   return b1;
}

std::ostream& operator<< (std::ostream& o, const G6& v) {
   std::streamsize oldPrecision = o.precision();
   o << std::fixed << std::setprecision(3);
   for (size_t i = 0; i < v.size(); ++i)
      o << std::setw(9) << v[i] << " ";
   o << std::setprecision(oldPrecision);
   o.unsetf(std::ios::floatfield);
   return o;
}

G6 operator*(const double d, const G6& v) {
   return v*d;
}
