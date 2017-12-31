
#include "stdafx.h"

#include "LRL_CoordinateConversionMatrices.h"
#include "D7.h"
#include "MatD7.h"
#include "MatG6.h"
#include "MatMN.h"
#include "MatN.h"

#include <algorithm>
#include <string>
#include <vector>

#pragma warning( disable : 4702) //  unreachable code
#pragma warning( disable : 4100) //  unreferenced formal parameter

std::vector<MatD7> MatD7::vD7_Refl = MatD7::GetReflections();

MatD7::MatD7(void)
   : m_mat(49)
{}

//MatD7::MatD7(const LRL_Cell& m)
//   : m_mat(49)
//{throw;}

MatD7::MatD7(const MatS6& m)
   : m_mat(49)
{
   const MatMN mnS6 = m;
   const MatMN mn = D7_FROM_S6 * mnS6 * S6_FROM_D7;
   m_mat.SetVector( mn.GetVector());
   m_mat.SetDim(49);
   m_mat.SetRowDim(7);
}

MatD7::MatD7(const MatD7& m)
   : m_mat(m.m_mat)
{}

MatD7::MatD7(const MatG6& m)
   : m_mat(49)
{
   const MatMN mn = D7_FROM_G6 * m * G6_FROM_D7;
   m_mat.SetVector(mn.GetVector());
}

MatD7::MatD7(const MatN& m)
   : m_mat(m)
{}

MatD7::MatD7(const MatMN& m)
   : m_mat(m.GetDim())
{
   if (m.GetColDim() != m.GetRowDim()) throw;
   const unsigned long n = m.GetRowDim();
   m_mat.resize(n);
   m_mat.SetVector(m.GetVector());
   m_mat.SetDim(n*n);
   m_mat.SetRowDim(n);
}

MatD7::MatD7(const std::string& s)
   : m_mat(49)
{
   MatN mn ( m_mat.FromString(s));
   (*this).m_mat = mn;
}

MatD7::MatD7(const std::vector<double>& v)
   : m_mat(49)
{
   for (unsigned long i = 0; i < 49; ++i) {
      m_mat[i] = v[i];
   }
}

MatD7& MatD7::operator= (const MatB4& m) {
   m_mat.resize(49);
   throw("we don't know how to do this");
   return *this;
}

MatD7& MatD7::operator= (const MatS6& m) {
   m_mat.resize(49);
   m_mat.SetVector((D7_FROM_S6 * m * S6_FROM_D7).GetVector());;
   return *this;
}

MatD7& MatD7::operator= (const MatD7& v) {
   m_mat.resize(49);
   m_mat = v.m_mat;
   return *this;
}

MatD7& MatD7::operator= (const MatG6& m) {
   m_mat.resize(49);
   m_mat.SetVector((D7_FROM_G6 * m * G6_FROM_D7).GetVector());;
   return *this;
}

MatD7& MatD7::operator+= (const MatD7& d) {
   for (unsigned long i = 0; i < 49; ++i) m_mat[i] += d[i];
   return *this;
}

MatD7& MatD7::operator-= (const MatD7& d) {
   for (unsigned long i = 0; i < 49; ++i) m_mat[i] -= d[i];
   return *this;
}

bool MatD7::operator== (const MatD7& m) const {
   for (unsigned long i = 0; i < 49; ++i) if( m_mat[i] != m[i]) return false;
   return true;
}

bool MatD7::operator!= (const MatD7& m) const {
   return !((*this) == m);
}

MatD7& MatD7::operator/= (const double d) {
   for (unsigned long i = 0; i < 49; ++i) m_mat[i] /= d;
   return *this;
}

MatD7& MatD7::operator*= (const double d) {
   for (unsigned long i = 0; i < 49; ++i) m_mat[i] *= d;
   return *this;
}

MatD7 MatD7::operator+ (const MatD7& v) const {
   MatD7 m(*this);
   return (m += v);
}

MatD7 MatD7::operator- (const MatD7& v) const {
   MatD7 m(*this);
   return (m -= v);
}

MatD7 MatD7::operator- (void) const { // unary
   MatD7 m(*this);
   m.m_mat = -m.m_mat;
   return m; // unary
}

MatD7 MatD7::operator* (const MatD7& m2) const {
   MatD7 m(*this);
   m.m_mat *= m2.m_mat;
   return m;
}

MatD7 MatD7::operator* (const double d) const {
   MatD7 m(*this);
   for (unsigned long i = 0; i < 49; ++i) m.m_mat[i] *= d;
   return m;
}

MatD7 MatD7::operator/ (const double d) const {
   MatD7 m(*this);
   for (unsigned long i = 0; i < 49; ++i) m.m_mat[i] /= d;
   return m;
}

MatD7 operator*(const double d, const MatD7& m) {
   return m*d;
}

MatD7 operator/(const double d, const MatD7& m) {
   return m / d;
}

D7 MatD7::operator* (const D7& v) const {
   return m_mat * v.GetVector();
}


double MatD7::operator[] (const unsigned long n) const {
   return m_mat[n];
}

double& MatD7::operator[] (const unsigned long n) {
   return m_mat[n];
}


double MatD7::DistanceBetween(const MatD7& v1, const MatD7& v2) {
   return ((v1 - v2).norm());
}

unsigned long MatD7::size(void) const {
   return 49;
}

double MatD7::norm() const {
   double sum = 0.0;
   for (unsigned long i = 0; i < 49; ++i) sum += m_mat[i];
   return sum;
}

double MatD7::norm(const MatD7& t) const {
   return t.norm();
}

double MatD7::Norm() const {
   return norm();
}

double MatD7::Norm(const MatD7& t) const {
   return t.norm();
}

bool MatD7::IsUnit() const {
   long row = -1;
   for (unsigned long i = 0; i < 49; ++i) {
      long column = i % 7;
      if (column == 0) ++row;
      if (column == row && m_mat[i] != 1.0) return false;
      if (column != row && m_mat[i] != 0.0) return false;
   }
   return true;
}

MatD7 MatD7::Eye() {
   MatD7 m;
   for (unsigned long i = 0; i<49; ++i)
      m.m_mat[i] = 0.0;
   for (unsigned long i = 0; i<49; i += 8)
      m.m_mat[i] = 1.0;
   return m;
}

MatD7 MatD7::Zero(void) {
   MatD7 m;
   for (unsigned long i = 0; i < 49; ++i)
      m[i] = 0.0;
   return m;
}

MatD7 MatD7::transpose(const MatD7& m2) const {
   //  transpose a symmetrical matrix
   const int size = 7;
   MatD7 m;
   for (int count = 0; count<size*size; ++count) {
      const int transposeIndex = count / size + size*(count%size); //'=int(rowindex/6) +6*colIndex)
      if (count >= transposeIndex) {
         m[transposeIndex] = m2[count];
         m[count] = m2[transposeIndex];
      }
   }
   return m;
}

void MatD7::transpose(void) {
   //  transpose a symmetrical matrix
   const int size = 7;
   MatD7& m(*this);
   MatD7 m2(*this);
   for (int count = 0; count<size*size; ++count) {
      const int transposeIndex = count / size + size*(count%size); //'=int(rowindex/6) +6*colIndex)
      if (count >= transposeIndex) {
         m[transposeIndex] = m2[count];
         m[count] = m2[transposeIndex];
      }
   }
}


double MatD7::at(const unsigned long n) const {
   return m_mat[n];
}

MatD7 MatD7::unit(void) {
   m_mat.resize(49);
   for (unsigned long i = 0; i < 49; ++i)
      m_mat[i] = 0.0;
   for (unsigned long i = 0; i < 49; i += 7+1)
      m_mat[i] = 1.0;
   return *this;
}

MatD7 MatD7::unit(const MatD7& min) {
   MatD7 m(min);
   for (unsigned long i = 0; i < 49; ++i)
      m.m_mat[i] = 0.0;
   for (unsigned long i = 0; i < 49; i += 7+1)
      m.m_mat[i] = 1.0;
   return m;
}

//static const MatD7 mReducer( "0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   1 1 1 1 -1 -1 -1   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0");
static std::vector<MatD7> vReducer;

bool IsBetter(const MatD7& m1, const MatD7& m2) {
   return m1.norm() < m2.norm();
}


MatD7 MatD7::Reduce(void) const {
   if (vReducer.empty()) {
      vReducer.push_back(MatD7("1 1 1 1 -1 -1 -1   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0"));
      vReducer.push_back(MatD7("0 0 0 0 0 0 0   1 1 1 1 -1 -1 -1   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0"));
      vReducer.push_back(MatD7("0 0 0 0 0 0 0   0 0 0 0 0 0 0   1 1 1 1 -1 -1 -1   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0"));
      vReducer.push_back(MatD7("0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   1 1 1 1 -1 -1 -1   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0"));
      vReducer.push_back(MatD7("0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   1 1 1 1 -1 -1 -1   0 0 0 0 0 0 0   0 0 0 0 0 0 0"));
      vReducer.push_back(MatD7("0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   1 1 1 1 -1 -1 -1   0 0 0 0 0 0 0"));
      vReducer.push_back(MatD7("0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   1 1 1 1 -1 -1 -1"));
   }
   MatD7 mToReduce(*this);
   double mNorm = mToReduce.norm();
   const double testValue = 1.0E-6 * mNorm;
   double lastNorm = mNorm;

   int count = 0;

   while (count <1 || mNorm - lastNorm > testValue) {
      ++count;
      for (unsigned long nloops = 1; nloops < 4; ++nloops) {
         for (unsigned long i = 0; i < vReducer.size(); ++i) {
            const MatD7 m1 = mToReduce - vReducer[i];
            const MatD7 m2 = mToReduce + vReducer[i];

            if (IsBetter(m1, mToReduce))
               mToReduce = m1;
            if (IsBetter(m2, mToReduce))
               mToReduce = m2;
         }
      }
   }

   return mToReduce;
}

const double t1[10] = { 2, 0, 0, 0, 0, 1, 1, 1, 0, 0 };
const double t2[10] = { 0, 2, 0, 0, 1, 0, 1, 0, 1, 0 };
const double t3[10] = { 0, 0, 2, 0, 1, 1, 0, 0, 0, 1 };
const double t4[10] = { 0, 0, 0, 2, 0, 0, 0, 1, 1, 1 };

//const double tx[10] = { 1, 1, 1, -1, 1, 1, 1, 0, 0, 0 };

void FixColumnMinus7(MatD7& m, const unsigned long start, const double v[7]) {
   for (unsigned long i = start, j = 0; i<start + 7; ++i, ++j)
      m[i] += v[j];
}
void FixColumnPlus7(MatD7& m, const unsigned long start, const double v[7]) {
   for (unsigned long i = start, j = 0; i<start + 7; ++i, ++j)
      m[i] -= v[j];
}

void FixSixColumnsOneAndOneZero(MatD7& m, const unsigned long start) {
   for (unsigned long i = start; i < start + 7; ++i)
      m[i] = m[i] == 0.0 ? 1.0 : 0.0;
}

// special function for simplifying 7-space transformation matrices
void MatD7::ReplaceTwosInRows7x7( MatD7& m ) {
   for( unsigned long i=0; i<49; i = i+7 ) {
      if ( m[i+0] == -2.0 ) FixColumnMinus7( m, i, t1 );
      if ( m[i+1] == -2.0 ) FixColumnMinus7( m, i, t2 );
      if ( m[i+2] == -2.0 ) FixColumnMinus7( m, i, t3 );
      if ( m[i+3] == -2.0 ) FixColumnMinus7( m, i, t4 );
      if ( m[i+0] == 2.0 ) FixColumnPlus7( m, i, t1 );
      if ( m[i+1] == 2.0 ) FixColumnPlus7( m, i, t2 );
      if ( m[i+2] == 2.0 ) FixColumnPlus7( m, i, t3 );
      if ( m[i+3] == 2.0 ) FixColumnPlus7( m, i, t4 );

      double sumAbs = 0.0;
      double sum = 0.0;
      double maxAbs = 0.0;
      double fourSum = 0;
      for (unsigned long j = i; j < i + 7; ++j) {
         const double mj = m[j];
         sumAbs += std::abs(mj);
         sum += mj;
         maxAbs = std::max(maxAbs, std::abs(mj));
         if (j < i + 4) fourSum += mj;
      }
      const double d = m[i + 6];
      if (sumAbs == 6 && sum == 0 && maxAbs == 1) FixSixColumnsOneAndOneZero(m, i);
   }
}

MatD7  MatD7::GetReflection(const unsigned long n) {
   if (vD7_Refl.empty()) GetReflections();
   return vD7_Refl[n];
}

std::vector<MatD7> MatD7::GetReflections() {
   if (vD7_Refl.empty()) {
      vD7_Refl.push_back(MatD7("1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1  0 0 0 0 0 0"));
      vD7_Refl.push_back(MatD7("1 0 0 0 0 0  0 1 0 0 0 0  0 0 0 1 0 0  0 0 1 0 0 0  0 0 0 0 0 1  0 0 0 0 1 0  0 0 0 0 0 0"));
      vD7_Refl.push_back(MatD7("1 0 0 0 0 0  0 0 1 0 0 0  0 1 0 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 0  0 0 0 0 0 1"));
      vD7_Refl.push_back(MatD7("1 0 0 0 0 0  0 0 1 0 0 0  0 0 0 1 0 0  0 1 0 0 0 0  0 0 0 0 0 0  0 0 0 0 1 0  0 0 0 0 0 1"));
      vD7_Refl.push_back(MatD7("1 0 0 0 0 0  0 0 0 1 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 0 0 1  0 0 0 0 0 0  0 0 0 0 1 0"));
      vD7_Refl.push_back(MatD7("1 0 0 0 0 0  0 0 0 1 0 0  0 0 1 0 0 0  0 1 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 1  0 0 0 0 1 0"));
      vD7_Refl.push_back(MatD7("0 1 0 0 0 0  1 0 0 0 0 0  0 0 1 0 0 0  0 0 0 1 0 0  0 0 0 0 0 1  0 0 0 0 1 0  0 0 0 0 0 0"));
      vD7_Refl.push_back(MatD7("0 1 0 0 0 0  1 0 0 0 0 0  0 0 0 1 0 0  0 0 1 0 0 0  0 0 0 0 1 0  0 0 0 0 0 1  0 0 0 0 0 0"));
      vD7_Refl.push_back(MatD7("0 1 0 0 0 0  0 0 1 0 0 0  1 0 0 0 0 0  0 0 0 1 0 0  0 0 0 0 0 1  0 0 0 0 0 0  0 0 0 0 1 0"));
      vD7_Refl.push_back(MatD7("0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 1 0 0  1 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 0 1  0 0 0 0 1 0"));
      vD7_Refl.push_back(MatD7("0 1 0 0 0 0  0 0 0 1 0 0  1 0 0 0 0 0  0 0 1 0 0 0  0 0 0 0 1 0  0 0 0 0 0 0  0 0 0 0 0 1"));
      vD7_Refl.push_back(MatD7("0 1 0 0 0 0  0 0 0 1 0 0  0 0 1 0 0 0  1 0 0 0 0 0  0 0 0 0 0 0  0 0 0 0 1 0  0 0 0 0 0 1"));
      vD7_Refl.push_back(MatD7("0 0 1 0 0 0  1 0 0 0 0 0  0 1 0 0 0 0  0 0 0 1 0 0  0 0 0 0 0 0  0 0 0 0 1 0  0 0 0 0 0 1"));
      vD7_Refl.push_back(MatD7("0 0 1 0 0 0  1 0 0 0 0 0  0 0 0 1 0 0  0 1 0 0 0 0  0 0 0 0 1 0  0 0 0 0 0 0  0 0 0 0 0 1"));
      vD7_Refl.push_back(MatD7("0 0 1 0 0 0  0 1 0 0 0 0  1 0 0 0 0 0  0 0 0 1 0 0  0 0 0 0 0 0  0 0 0 0 0 1  0 0 0 0 1 0"));
      vD7_Refl.push_back(MatD7("0 0 1 0 0 0  0 1 0 0 0 0  0 0 0 1 0 0  1 0 0 0 0 0  0 0 0 0 0 1  0 0 0 0 0 0  0 0 0 0 1 0"));
      vD7_Refl.push_back(MatD7("0 0 1 0 0 0  0 0 0 1 0 0  1 0 0 0 0 0  0 1 0 0 0 0  0 0 0 0 1 0  0 0 0 0 0 1  0 0 0 0 0 0"));
      vD7_Refl.push_back(MatD7("0 0 1 0 0 0  0 0 0 1 0 0  0 1 0 0 0 0  1 0 0 0 0 0  0 0 0 0 0 1  0 0 0 0 1 0  0 0 0 0 0 0"));
      vD7_Refl.push_back(MatD7("0 0 0 1 0 0  1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 0 0 0  0 0 0 0 0 1  0 0 0 0 1 0"));
      vD7_Refl.push_back(MatD7("0 0 0 1 0 0  1 0 0 0 0 0  0 0 1 0 0 0  0 1 0 0 0 0  0 0 0 0 0 1  0 0 0 0 0 0  0 0 0 0 1 0"));
      vD7_Refl.push_back(MatD7("0 0 0 1 0 0  0 1 0 0 0 0  1 0 0 0 0 0  0 0 1 0 0 0  0 0 0 0 0 0  0 0 0 0 1 0  0 0 0 0 0 1"));
      vD7_Refl.push_back(MatD7("0 0 0 1 0 0  0 1 0 0 0 0  0 0 1 0 0 0  1 0 0 0 0 0  0 0 0 0 1 0  0 0 0 0 0 0  0 0 0 0 0 1"));
      vD7_Refl.push_back(MatD7("0 0 0 1 0 0  0 0 1 0 0 0  1 0 0 0 0 0  0 1 0 0 0 0  0 0 0 0 0 1  0 0 0 0 1 0  0 0 0 0 0 0"));
      vD7_Refl.push_back(MatD7("0 0 0 1 0 0  0 0 1 0 0 0  0 1 0 0 0 0  1 0 0 0 0 0  0 0 0 0 1 0  0 0 0 0 0 1  0 0 0 0 0 0"));
   }
   return vD7_Refl;
}
