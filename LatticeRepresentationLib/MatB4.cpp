
//#include "stdafx.h"

#include "B4.h"
#include "D7.h"
#include "MatB4.h"
#include "MatMN.h"
#include "MatN.h"

#include <string>
#include <vector>

#pragma warning( disable : 4100) //  unreferenced formal parameter
#pragma warning( disable : 4702) //  unreachable code

std::vector<MatB4> MatB4::vB4_Refl = MatB4::GetReflections();

MatB4::MatB4(void)
   : m_mat(16)
{}

//MatB4::MatB4(const LRL_Cell& m)
//   : m_mat(16)
//{throw;}

MatB4::MatB4(const MatS6& m)
   : m_mat(16)
{
   throw("we don't know how to do this");
}

MatB4::MatB4(const MatD7& m)
   : m_mat(16)
{
   throw("we don't know how to do this");
}

MatB4::MatB4(const MatB4& m)
   : m_mat(m.m_mat)
{}

MatB4::MatB4(const MatG6& m)
   : m_mat(16)
{
   throw("we don't know how to do this");
}

MatB4::MatB4(const MatN& m)
   : m_mat(m)
{}

MatB4::MatB4(const std::string& s)
   : m_mat(16)
{
   m_mat = m_mat.FromString(s);
}

MatB4::MatB4(const std::vector<double>& v)
   : m_mat(16)
{
   for (unsigned long i = 0; i < 16; ++i) {
      m_mat[i] = v[i];
   }
}


MatB4& MatB4::operator= (const MatB4& m) {
   m_mat.resize(16);
   m_mat = m.m_mat;
   return *this;
}

MatB4& MatB4::operator= (const MatS6& m) {
   throw("we don't know how to do this");
   return *this;
}

MatB4& MatB4::operator= (const MatD7& m) {
   throw("we don't know how to do this");
   return *this;
}

MatB4& MatB4::operator= (const MatG6& m) {
   m_mat.resize(16);
   throw("we don't know how to do this");
   return *this;
}

MatB4& MatB4::operator+= (const MatB4& d) {
   for (unsigned long i = 0; i < 16; ++i) m_mat[i] += d[i];
   return *this;
}

MatB4& MatB4::operator-= (const MatB4& d) {
   for (unsigned long i = 0; i < 16; ++i) m_mat[i] -= d[i];
   return *this;
}

bool MatB4::operator== (const MatB4& m) const {
   for (unsigned long i = 0; i < 16; ++i) if (m_mat[i] != m[i]) return false;
   return true;
}

bool MatB4::operator!= (const MatB4& m) const {
   return !((*this) == m);
}

MatB4& MatB4::operator/= (const double d) {
   for (unsigned long i = 0; i < 16; ++i) m_mat[i] /= d;
   return *this;
}

MatB4& MatB4::operator*= (const double d) {
   for (unsigned long i = 0; i < 16; ++i) m_mat[i] *= d;
   return *this;
}

MatB4 MatB4::operator+ (const MatB4& m) const {
   MatB4 m4(*this);
   return (m4 += m);
}

MatB4 MatB4::operator- (const MatB4& m) const {
   MatB4 m4(*this);
   return (m4 -= m);
}

MatB4 MatB4::operator- (void) const { // unary
   MatB4 m(*this);
   m.m_mat = -m.m_mat;
   return m; // unary
}

MatB4 MatB4::operator* (const MatB4& m2) const {
   MatB4 m(*this);
   m.m_mat *= m2.m_mat;
   return m;
}

MatB4 MatB4::operator* (const double d) const {
   MatB4 m(*this);
   for (unsigned long i = 0; i < 16; ++i) m.m_mat[i] *= d;
   return m;
}

MatB4 MatB4::operator/ (const double d) const {
   MatB4 m(*this);
   for (unsigned long i = 0; i < 16; ++i) m.m_mat[i] /= d;
   return m;
}

MatB4 operator*(const double d, const MatB4& m) {
   return m*d;
}

MatB4 operator/(const double d, const MatB4& m) {
   return m / d;
}

static B4 B4FromMatrixTimesB4(const MatN& m, const B4& d) {
   const Vector_3 v1 = m[0] * d[0] + m[1] * d[1] + m[2] * d[2] + m[3] * d[3];
   const Vector_3 v2 = m[4] * d[0] + m[5] * d[1] + m[6] * d[2] + m[7] * d[3];
   const Vector_3 v3 = m[8] * d[0] + m[9] * d[1] + m[10] * d[2] + m[11] * d[3];
   const Vector_3 v4 = m[12] * d[0] + m[13] * d[1] + m[14] * d[2] + m[15] * d[3];
   return B4(v1, v2, v3, v4);
}

B4 MatB4::operator* (const B4& m) const {
   return B4FromMatrixTimesB4(m_mat, m);
}


double MatB4::operator[] (const unsigned long n) const {
   return m_mat[n];
}

double& MatB4::operator[] (const unsigned long n) {
   return m_mat[n];
}


double MatB4::DistanceBetween(const MatB4& v1, const MatB4& v2) {
   return ((v1 - v2).norm());
}

unsigned long MatB4::size(void) const {
   return 16;
}

double MatB4::norm() const {
   double sum = 0.0;
   for (unsigned long i = 0; i < 16; ++i) sum += m_mat[i]* m_mat[i];
   return sqrt(sum);
}

double MatB4::norm(const MatB4& t) const {
   return t.norm();
}

double MatB4::Norm() const {
   return norm();
}

double MatB4::Norm(const MatB4& t) const {
   return t.norm();
}

bool MatB4::IsUnit() const {
   long row = -1;
   for (unsigned long i = 0; i < 16; ++i) {
      long column = i % 7;
      if (column == 0) ++row;
      if (column == row && m_mat[i] != 1.0) return false;
      if (column != row && m_mat[i] != 0.0) return false;
   }
   return true;
}

MatB4 MatB4::Eye(void) {
   MatB4 m;
   for (unsigned long i = 0; i < 16; ++i)
      m.m_mat[i] = 0.0;
   for (unsigned long i = 0; i < 16; i += 5)
      m.m_mat[i] = 1.0;
   return m;
}

MatB4 MatB4::Zero(void) {
   MatB4 m;
   for (unsigned long i = 0; i < 16; ++i)
      m[i] = 0.0;
   return m;
}

MatB4 MatB4::transpose(const MatB4& m2) const {
   //  transpose a symmetrical matrix
   const int size = 4;
   MatB4 m;
   for (int count = 0; count<size*size; ++count) {
      const int transposeIndex = count / size + size*(count%size); //'=int(rowindex/6) +6*colIndex)
      if (count >= transposeIndex) {
         m[transposeIndex] = m2[count];
         m[count] = m2[transposeIndex];
      }
   }
   return m;
}

void MatB4::transpose(void) {
   //  transpose a symmetrical matrix
   const int size = 4;
   MatB4& m(*this);
   MatB4 m2(*this);
   for (int count = 0; count<size*size; ++count) {
      const int transposeIndex = count / size + size*(count%size); //'=int(rowindex/6) +6*colIndex)
      if (count >= transposeIndex) {
         m[transposeIndex] = m2[count];
         m[count] = m2[transposeIndex];
      }
   }
}

double MatB4::at(const unsigned long n) const {
   return m_mat[n];
}

MatB4 MatB4::unit(void) {
   m_mat.resize(16);
   for (unsigned long i = 0; i < 16; ++i)
      m_mat[i] = 0.0;
   for (unsigned long i = 0; i < 16; i += 4 + 1)
      m_mat[i] = 1.0;
   return *this;
}

MatB4 MatB4::unit(const MatB4& min) {
   MatB4 m(min);
   for (unsigned long i = 0; i < 16; ++i)
      m.m_mat[i] = 0.0;
   for (unsigned long i = 0; i < 16; i += 4 + 1)
      m.m_mat[i] = 1.0;
   return m;
}

MatB4  MatB4::GetReflection(const unsigned long n) {
   if (vB4_Refl.empty()) GetReflections();
   return vB4_Refl[n];
}

std::vector<MatB4>   MatB4::GetReflections() {
   if (vB4_Refl.empty()) {
      vB4_Refl.push_back(MatB4("1 0 0 0  0 1 0 0  0 0 1 0  0 0 0 1"));
      vB4_Refl.push_back(MatB4("1 0 0 0  0 1 0 0  0 0 0 1  0 0 1 0"));
      vB4_Refl.push_back(MatB4("1 0 0 0  0 0 1 0  0 1 0 0  0 0 0 1"));
      vB4_Refl.push_back(MatB4("1 0 0 0  0 0 1 0  0 0 0 1  0 1 0 0"));
      vB4_Refl.push_back(MatB4("1 0 0 0  0 0 0 1  0 1 0 0  0 0 1 0"));
      vB4_Refl.push_back(MatB4("1 0 0 0  0 0 0 1  0 0 1 0  0 1 0 0"));
      vB4_Refl.push_back(MatB4("0 1 0 0  1 0 0 0  0 0 1 0  0 0 0 1"));
      vB4_Refl.push_back(MatB4("0 1 0 0  1 0 0 0  0 0 0 1  0 0 1 0"));
      vB4_Refl.push_back(MatB4("0 1 0 0  0 0 1 0  1 0 0 0  0 0 0 1"));
      vB4_Refl.push_back(MatB4("0 1 0 0  0 0 1 0  0 0 0 1  1 0 0 0"));
      vB4_Refl.push_back(MatB4("0 1 0 0  0 0 0 1  1 0 0 0  0 0 1 0"));
      vB4_Refl.push_back(MatB4("0 1 0 0  0 0 0 1  0 0 1 0  1 0 0 0"));
      vB4_Refl.push_back(MatB4("0 0 1 0  1 0 0 0  0 1 0 0  0 0 0 1"));
      vB4_Refl.push_back(MatB4("0 0 1 0  1 0 0 0  0 0 0 1  0 1 0 0"));
      vB4_Refl.push_back(MatB4("0 0 1 0  0 1 0 0  1 0 0 0  0 0 0 1"));
      vB4_Refl.push_back(MatB4("0 0 1 0  0 1 0 0  0 0 0 1  1 0 0 0"));
      vB4_Refl.push_back(MatB4("0 0 1 0  0 0 0 1  1 0 0 0  0 1 0 0"));
      vB4_Refl.push_back(MatB4("0 0 1 0  0 0 0 1  0 1 0 0  1 0 0 0"));
      vB4_Refl.push_back(MatB4("0 0 0 1  1 0 0 0  0 1 0 0  0 0 1 0"));
      vB4_Refl.push_back(MatB4("0 0 0 1  1 0 0 0  0 0 1 0  0 1 0 0"));
      vB4_Refl.push_back(MatB4("0 0 0 1  0 1 0 0  1 0 0 0  0 0 1 0"));
      vB4_Refl.push_back(MatB4("0 0 0 1  0 1 0 0  0 0 1 0  1 0 0 0"));
      vB4_Refl.push_back(MatB4("0 0 0 1  0 0 1 0  1 0 0 0  0 1 0 0"));
      vB4_Refl.push_back(MatB4("0 0 0 1  0 0 1 0  0 1 0 0  1 0 0 0"));
   }
   return vB4_Refl;
}
