#pragma warning( disable : 4100) //  unreferenced formal parameter

#include "stdafx.h"

#include "LRL_Cell.h"
#include "S6.h"
#include "Delone.h"
#include "B4.h"
#include "G6.h"
#include "D7.h"
#include "MatS6.h"
#include "LRL_RandTools.h"
#include "LRL_Vector3.h"

static int randSeed1 = 19191;

std::ostream& operator<< (std::ostream& o, const B4& dt) {
   for (unsigned long i = 0; i < 4; ++i)
      o << dt.m_vec[i];
   return o;
}

B4::B4(void) {
   m_vec.resize(4);
}

B4::B4(const B4& dt) 
   : m_vec(dt.m_vec)
{
   m_vec.resize(4);
}

B4::B4(const G6& v6) {
   m_vec.resize(4);
   (*this) = B4(LRL_Cell(v6));
}

B4::B4(const D7& v7) {
   m_vec.resize(4);
   (*this) = B4(LRL_Cell(v7));
}

B4::B4(const std::string& v7) {
   m_vec.resize(4);
   throw;
}

B4::B4(const Vector_3& v0, const Vector_3& v1, const Vector_3& v2, const Vector_3& v3) {
   m_vec.resize(4);
   m_vec[0] = v0;
   m_vec[1] = v1;
   m_vec[2] = v2;
   m_vec[3] = v3;
}

B4::B4(const LRL_Cell& c) {
   static const double rad = 180.0 / (4 * atan(1.0));
   m_vec.resize(4);
   const Matrix_3x3 amat = Matrix_3x3::Cart(c[0], c[1], c[2], c[3] * rad, c[4] * rad, c[5] * rad);

   (*this).m_vec[0] = amat * Vector_3(1, 0, 0);
   (*this).m_vec[1] = amat * Vector_3(0, 1, 0);
   (*this).m_vec[2] = amat * Vector_3(0, 0, 1);
   (*this).m_vec[3] = -(m_vec[0]+m_vec[1]+m_vec[2]);
}

B4::B4(const S6& ds) {
   m_vec.resize(4);
   (*this) = B4(LRL_Cell(ds));
}

Vector_3 B4::operator[] (const unsigned long n) const {
   return m_vec[n];
}

Vector_3& B4::operator[] (const unsigned long n) {
   return m_vec[n];
}

B4& B4::operator= (const B4& dt)
{
   for (unsigned long i = 0; i < 4; ++i)
      m_vec[i] = dt.m_vec[i];
   return *this;
}

B4& B4::operator= (const S6& s6) {
   m_vec.resize(4);
   *this = B4(s6);
   return *this;
}

B4& B4::operator= (const D7& d7) {
   m_vec.resize(4);
   *this = B4(d7);
   return *this;
}

B4& B4::operator= (const G6& g6) {
   m_vec.resize(4);
   *this = B4(g6);
   return *this;
}

B4& B4::operator= (const LRL_Cell& cl) {
   m_vec.resize(4);
   *this = B4(cl);
   return *this;
}

B4& B4::operator= (const std::string& s)
{
   throw;
   return *this;
}

B4 B4::operator+ (const B4& dt) const {
   B4 temp;
   for (unsigned long i = 0; i<4; ++i)
      temp[i] = (*this)[i] + dt[i];
   return temp;
}

B4 B4::operator- (const B4& dt) const {
   B4 temp;
   for ( unsigned long i=0; i<4; ++i )
   temp[i] = (*this)[i] - dt[i];
   return temp;
}

B4 B4::operator* (const double d) const
{
   B4 dt;
   for (unsigned long i = 0; i < dt.size(); ++i)
      dt[i] = ((*this)[i]) * d;
   return dt;
}

B4 B4::operator/ (const double d) const
{
   B4 dt;
   for (unsigned long i = 0; i < dt.size(); ++i)
      dt[i] = ((*this)[i]) / d;
   return dt;
}

B4 B4::operator- (void) { // unary
   m_vec[0] = -m_vec[0];
   m_vec[1] = -m_vec[1];
   m_vec[2] = -m_vec[2];
   m_vec[3] = -m_vec[3];
   return *this;
}

B4& B4::operator*= (const double d) {
   for (unsigned long i = 0; i < size(); ++i)
      m_vec[i] = d * m_vec[i];
   return *this;
}

B4& B4::operator/= (const double d) {
   for (unsigned long i = 0; i < size(); ++i)
      m_vec[i] = m_vec[i] / d;
   return *this;
}

B4& B4::operator+= (const B4& dt) {
   for (unsigned long i = 0; i < size(); ++i)
      m_vec[i] = m_vec[i] + dt.m_vec[i];
   return *this;
}

B4& B4::operator-= (const B4& dt) {
   for (unsigned long i = 0; i < size(); ++i)
      m_vec[i] = m_vec[i] - dt.m_vec[i];
   return *this;
}


bool B4::operator== (const B4& b4) const {
   const bool b0 = (*this).m_vec[0] == b4.m_vec[0];
   const bool b1 = (*this).m_vec[1] == b4.m_vec[1];
   const bool b2 = (*this).m_vec[2] == b4.m_vec[2];
   const bool b3 = (*this).m_vec[3] == b4.m_vec[3];
   return b0 && b1 && b2 && b3;
}
bool B4::operator!= (const B4& b4) const {
   return !((*this) == b4);
}

B4 operator* (const double d, const B4& dt)
{
   return (dt*d);
}

double DistanceBetween(const B4& v1, const B4& v2) {
   return (v1 - v2).norm();
}

double B4::norm(void) const {
   return norm(*this);
}

double B4::DistanceBetween(const B4& v1, const B4& v2) {
   return ((v1 - v2).norm());
}

double B4::norm(const B4& dt) const {
   const double norm1 = dt[0].Norm();
   const double norm2 = dt[1].Norm();
   const double norm3 = dt[2].Norm();
   const double norm4 = dt[3].Norm();
   return sqrt(norm1*norm1 + norm2*norm2 + norm3*norm3 + norm4*norm4);
}

B4 B4::rand() {
   G6 g6;
   for (int i = 0; i < 6; ++i)
      g6[i] = randfg(randSeed1);

   const D7 d7(g6);
   const double randsNorm = d7.norm();
   return(d7 / randsNorm);
}

B4 B4::randDeloneReduced() {
   S6 vRan;
   S6 v;

   MatS6 m;
   vRan = S6(1000.0 * rand());

   while (!Delone::Reduce(vRan, m, v, 0.0) || !v.IsAllMinus()) {
      vRan = S6(1000.0 * rand());
   }
   return D7(v);
}

B4 B4::randDeloneUnreduced() {
   S6 vRan;
   S6 v;

   MatS6 m;
   vRan = S6(1000.0 * rand());

   while (!Delone::Reduce(vRan, m, v, 0.0) || v.IsAllMinus()) {
      vRan = S6(1000.0 * rand());
   }
   return D7(v);
}

B4 B4::rand(const double d) {
   return d*rand();
}

B4 B4::randDeloneReduced(const double d) {
   return d*randDeloneReduced();
}

B4 B4::randDeloneUnreduced(const double d) {
   return d*randDeloneUnreduced();
}


B4 B4::sort(const B4& din) {
   bool again = true;
   B4 dt(din);
   while (again) {
      again = false;
      for (unsigned long i = 0; i < 3; ++i) {
         if (dt[i + 1].Norm() < dt[i].Norm()) {
            std::swap(dt[i], dt[i + 1]);
            again = true;
         }
      }
   }
   return dt;
}

B4 B4::sort(void) const {
   return sort(*this);
}