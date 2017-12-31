
#include "stdafx.h"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>

#include "LRL_Cell.h"
#include "D7.h"
#include "D7_BoundaryList.h"
#include "D7_ClassifySubboundaries.h"
#include "S6.h"
#include "B4.h"
#include "G6.h"
#include "MatS6.h"
#include "MatG6.h"
#include "Selling.h"
#include "LRL_StringTools.h"
#include "LRL_Vector3.h"

#include "Delone.h"
#include "LRL_RandTools.h"

static int randSeed1 = 19191;

D7::D7( void ) {
   m_vec.resize(7);
   m_dim = 7;
   m_valid = true;
}

D7::D7( const D7& v )
//: m_vec(7)
{
   m_vec = v.GetVector();
   m_dim = 7;
   m_vec = v.m_vec;
   m_valid = v.m_valid;
}

D7::D7( const double v[7] ) {
    m_vec.resize(7);
    m_dim = 7;
    m_vec[0] = v[0];
    m_vec[1] = v[1];
    m_vec[2] = v[2];
    m_vec[3] = v[3];
    m_vec[4] = v[4];
    m_vec[5] = v[5];
    m_vec[6] = v[6];
    m_valid = true;
}

D7::D7(const LRL_Cell& c) {
   m_vec.resize(7);
   (*this) = G6(c);
   m_dim = 7;
   m_valid = true;
}

D7::D7(const S6& ds) {
   m_dim = 7;
   (*this) = G6(ds);
   m_valid = true;
}

D7::D7(const B4& dt) {
   m_vec.resize(7);
   m_dim = 7;
   (*this) = G6(dt);
   m_valid = true;
}


D7::D7(const VecN& v) {
   m_dim = 0;
   m_valid = false;
   if (v.size() == 7) {
      m_dim = 7;
      m_valid = true;
      m_vec = v;
   }
}

D7::D7(const G6& v6) {
   m_vec.resize(7);
   const double& g1 = v6[0];
   const double& g2 = v6[1];
   const double& g3 = v6[2];
   const double& g4 = v6[3];
   const double& g5 = v6[4];
   const double& g6 = v6[5];
   double& d1 = m_vec[0];
   double& d2 = m_vec[1];
   double& d3 = m_vec[2];
   double& b4 = m_vec[3];
   double& d5 = m_vec[4];
   double& s6 = m_vec[5];
   double& d7 = m_vec[6];
   d1 = g1;
   d2 = g2;
   d3 = g3;
   b4 = g1 + g2 + g3 + g6 + g5 + g4; // (a+b+c).(a+b+c)
   d5 = g2 + g3 + g4;  // (b+c).(b+c)
   s6 = g1 + g3 + g5;  // (a+c).(a+c)
   d7 = g1 + g2 + g6; // (a+b).(a+b)
   m_valid = true;
   m_dim = 7;
}

D7::D7(const std::string& s) {
   m_vec.resize(7);
   m_vec = LRL_StringTools::FromString(s);
   m_dim = m_vec.size();
   m_valid = true;
   if (m_dim != 7) throw "bad dimension in D7 from a string";
}

D7::D7(const std::vector<double>& v) {
   m_vec.resize(7);
   m_dim = 7;
   if (m_dim != 7) throw "bad dimension in D7 from a string";
   m_vec = v;
   m_valid = true;
}

double D7::DistanceBetween( const D7& v1, const D7& v2 ) {
   double sum = 0.0;
   for( unsigned long i=0; i<7; ++i )  sum += (v1[i]-v2[i])*(v1[i]-v2[i]);
   return sqrt( sum );
}

D7& D7::operator= ( const D7& v ) {
   m_vec = v.m_vec;
   m_dim = v.m_dim;
   m_valid = v.m_valid;
   return *this;
}

D7& D7::operator= (const std::string& s)
{
   (*this) = D7(s);
   return *this;
}

D7& D7::operator= (const G6& v6)
{
   m_vec.resize(7);
   const double& g1 = v6[0];
   const double& g2 = v6[1];
   const double& g3 = v6[2];
   const double& g4 = v6[3];
   const double& g5 = v6[4];
   const double& g6 = v6[5];
   //double& d1 = m_vec[0];
   //double& d2 = m_vec[1];
   //double& d3 = m_vec[2];
   //double& b4 = m_vec[3];
   //double& d5 = m_vec[4];
   //double& s6 = m_vec[5];
   //double& d7 = m_vec[6];
   m_vec[0] = g1;
   m_vec[1] = g2;
   m_vec[2] = g3;
   m_vec[3] = g1 + g2 + g3 + g6 + g5 + g4; // (a+b+c).(a+b+c)
   m_vec[4] = g2 + g3 + g4;  // (b+c).(b+c)
   m_vec[5] = g1 + g3 + g5;  // (a+c).(a+c)
   m_vec[6] = g1 + g2 + g6; // (a+b).(a+b)
   m_valid = true;

   return *this;
}

D7& D7::operator= (const S6& v)
{
   (*this) = G6(v);
   return *this;
}


D7& D7::operator= (const B4& v) {
   (*this) = G6(v);
   return *this;
}


D7& D7::operator= (const LRL_Cell& v) {
   (*this) = G6(v);
   return *this;
}

D7& D7::operator/= (const double d) {
   (*this).m_vec /= d;
   return *this;
}

D7& D7::operator*= (const double d) {
   (*this).m_vec *= d;
   return *this;
}

D7 D7::operator+= (const D7& d7) {
   for (unsigned long i = 0; i < d7.size(); ++i)
      m_vec[i] += d7.m_vec[i];
   return *this;
}

D7 D7::operator-= (const D7& d7) {
   for (unsigned long i = 0; i < d7.size(); ++i)
      m_vec[i] -= d7.m_vec[i];
   return *this;
}

D7 D7::operator* (const double d) const {
   D7 d7(*this);
   d7.m_vec *= d;
   return d7;
}

D7 D7::operator/ (const double d) const {
   D7 d7(*this);
   d7.m_vec /= d;
   return d7;
}

D7 D7::operator- (void) const {
   D7 d7;
   d7.m_vec = -d7.m_vec;
   return *this; // unary
}

bool D7::operator== (const D7& d7) const {
   return m_vec == d7.m_vec;
}

bool D7::operator!= (const D7& d7) const {
   return !((*this) == d7);
}

D7 D7::operator+ (const D7& v) const {
   D7 d;
   if ((*this).m_valid && v.m_valid) {
      d.m_vec = m_vec + v.m_vec;
   }
   else {
      d.m_valid = false;
   }
   return d;
}

D7 D7::operator- (const D7& v) const {
   D7 d;
   if ((*this).m_valid && v.m_valid) {
      d.m_vec = m_vec - v.m_vec;
   }
   else {
      d.m_valid = false;
   }
   return d;
}

D7_BoundaryList g_bl_D7;

std::vector<std::pair<std::string, std::string> > D7::ClassifyVector(const double delta) const {
   std::vector<D7_Boundary> vbl;
   vbl.push_back(g_bl_D7.Case1());
   vbl.push_back(g_bl_D7.Case2());
   vbl.push_back(g_bl_D7.Case3());
   vbl.push_back(g_bl_D7.Case4());
   vbl.push_back(g_bl_D7.Case5());
   vbl.push_back(g_bl_D7.Case6());
   vbl.push_back(g_bl_D7.Case7());
   vbl.push_back(g_bl_D7.Case8());
   vbl.push_back(g_bl_D7.Case9());
   std::vector<std::pair<std::string, std::string> > vss;

   const unsigned long blsize = g_bl_D7.size();
   const unsigned long howbig = D7_BoundaryList::size();
   for (unsigned int ibl = 0; ibl < vbl.size(); ++ibl) {
      const double perpdist = (vbl[ibl].GetPerp() *(*this)).norm();
      const std::string casenum = vbl[ibl].GetName();
      if (perpdist < delta) {
         const std::string subcasenum = D7_ClassifySubboundaries::ClassifyCase(casenum, (*this), delta);
         vss.push_back(std::make_pair(casenum, subcasenum));
      }
   }

   return vss;
}

D7 D7::rand() {
   G6 g6;
   for (int i = 0; i < 6; ++i)
      g6[i] = randfg(randSeed1);

   D7 d7(g6);
   const double randsNorm = d7.norm();
   return(d7 / randsNorm);
}

D7 D7::randDeloneReduced() {
   S6 vRan;
   S6 v;

   MatS6 m;
   vRan = S6(G6::rand(1000.0));

   while (!Delone::IsReduced(vRan,0.0)) {
      vRan = S6(G6::rand(1000.0));
   }
   return D7(v);
}

D7 D7::randDeloneUnreduced() {
   S6 vRan;
   S6 v;

   MatS6 m;
   vRan = S6(1000.0 * rand());

   while (Delone::IsReduced(vRan,0.0)) {
      vRan = S6(1000.0 * rand());
   }
   return D7(v);
}

D7 D7::rand(const double d) {
   return d*rand();
}

D7 D7::randDeloneReduced(const double d) {
   return d*randDeloneReduced();
}

D7 D7::randDeloneUnreduced(const double d) {
   return d*randDeloneUnreduced();
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
std::pair<int, std::string> D7::IdentifyNearbyBoundaries(const D7& v, const double cutoff)
/*-------------------------------------------------------------------------------------*/
{
   std::string s1, s2;
   int boundaryNumber = -1;
   const double& d1 = v[0];
   const double& d2 = v[1];
   const double& d3 = v[2];
   const double& d4 = v[3];
   const double& d5 = v[4];
   const double& d6 = v[5];
   const double& d7 = v[6];

   if (std::abs(d1 - d2) < cutoff)
   {
      s1 += '1';
      s2 += '1';
      boundaryNumber = 1;
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(d2 - d3) < cutoff)
   {
      s1 += '2';
      s2 += '2';
      boundaryNumber = 2;
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(d3 - d4) < cutoff)
   {
      s1 += '3';
      s2 += '3';
      boundaryNumber = 3;
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(d2 + d3 - d5) < cutoff)
   {
      s1 += '4';
      s2 += '4';
      boundaryNumber = 4;
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(d1 + d4 - d5) < cutoff)
   {
      s1 += '5';
      s2 += '5';
      boundaryNumber = 5;
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(d1 + d3 - d6) < cutoff)
   {
      s1 += '6';
      s2 += '6';
      boundaryNumber = 6;
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(d2 + d4 - d6) < cutoff)
   {
      s1 += '7';
      s2 += '7';
      boundaryNumber = 7;
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(d1 + d2 - d7) < cutoff)
   {
      s1 += '8';
      s2 += '8';
      boundaryNumber = 8;
   }
   else
   {
      s2 += '.';
   }

   if (std::abs(d3 + d4 - d7) < cutoff)
   {
      s1 += '9';
      boundaryNumber = 9;
      s2 += '9';
   }
   else
   {
      s2 += '.';
   }


   return std::make_pair(boundaryNumber, (s1.empty()) ? "" : "(" + s1 + ") \t(" + s2 + ")");
}

std::ostream& operator<< ( std::ostream& o, const D7& v ) {
   std::streamsize oldPrecision = o.precision();
   o << std::fixed <<std::setprecision(3);
   for( unsigned long i=0; i<v.size( ); ++i )
      o << std::setw(9) << v[i] << " ";
   o << std::setprecision(oldPrecision);
   o.unsetf(std::ios::floatfield);
   return o;
}

D7 operator*(const double d, const D7& v) { // friend
   return v*d;
}

std::pair<std::string, std::string> D7::SummarizeBoundaries( const double delta) const {
   const std::vector<std::pair<std::string, std::string> > bounds = ClassifyVector(delta);
   std::string boundList;
   std::string subboundList;
   for (unsigned long i = 0; i < bounds.size(); ++i) {
      boundList += bounds[i].first + " ";
      subboundList += bounds[i].second + " ";
   }
   return std::make_pair(boundList, subboundList);
}
