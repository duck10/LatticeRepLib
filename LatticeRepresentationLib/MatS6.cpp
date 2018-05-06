


#include "LRL_CoordinateConversionMatrices.h"
#include "S6.h"
#include "MatS6.h"
#include "MatD7.h"
#include "MatG6.h"
#include "MatMN.h"
#include "MatN.h"

#include <string>
#include <vector>

#pragma warning( disable : 4100) //  unreferenced formal parameter
#pragma warning( disable : 4702) //  unreachable code

std::vector<MatS6> MatS6::vS6_Refl = MatS6::GetReflections();


MatS6::MatS6(void)
   : m_mat(36)
{}

//MatS6::MatS6(const LRL_Cell& m)
//   : m_mat(36)
//{throw;}

MatS6::MatS6(const MatS6& m)
   : m_mat(m.m_mat)
{}

MatS6::MatS6(const MatD7& m)
   : m_mat(36)
{
   MatMN mn = S6_FROM_D7 * m * D7_FROM_S6;
   m_mat.SetVector(mn.GetVector());
}

MatS6::MatS6(const MatG6& m)
   : m_mat(36)
{
   MatMN mn = S6_FROM_G6 * m * G6_FROM_S6;
   m_mat.SetVector(mn.GetVector());
}

MatS6::MatS6(const MatN& m)
   : m_mat(m)
{}

MatS6::MatS6(const MatMN& m)
   : m_mat(m.GetDim())
{
   if (m.GetColDim() != m.GetRowDim()) throw;
   const unsigned long n = m.GetRowDim();
   m_mat.resize(n);
   for (unsigned long i = 0; i < n; ++i) m_mat[i] = m[i];
   m_mat.SetDim(n*n);
   m_mat.SetRowDim(n);
}

MatS6::MatS6(const std::string& s)
   : m_mat(36)
{
   MatN mn(m_mat.FromString(s));
   (*this) = MatS6(mn);
}

MatS6::MatS6(const std::vector<double>& v)
   : m_mat(36)
{
   for (unsigned long i = 0; i < 36; ++i) {
      m_mat[i] = v[i];
   }
}

MatS6& MatS6::operator= (const MatB4& v) {
   m_mat.resize(36);
   throw("we don't know how to do this");
   return *this;
}

MatS6& MatS6::operator= (const MatS6& v) {
   m_mat.SetVector(v.GetVector());
   m_mat.SetDim(v.size());
   m_mat.SetRowDim((unsigned long)std::sqrt(v.size()));
   return *this;
}

MatS6& MatS6::operator= (const MatD7& m) {
   m_mat.resize(36);
   MatMN mn = S6_FROM_D7 * m * D7_FROM_S6;
   m_mat.SetVector(mn.GetVector());

   return *this;
}

MatS6& MatS6::operator= (const MatG6& m) {
   m_mat.resize(36);
   MatMN mn = S6_FROM_G6 * m * G6_FROM_S6;
   m_mat.SetVector(mn.GetVector());
   return *this;
}

MatS6& MatS6::operator+= (const MatS6& d) {
   for (unsigned long i = 0; i < 36; ++i) m_mat[i] += d[i];
   return *this;
}

MatS6& MatS6::operator-= (const MatS6& d) {
   for (unsigned long i = 0; i < 36; ++i) m_mat[i] -= d[i];
   return *this;
}

bool MatS6::operator== (const MatS6& m) const {
   for (unsigned long i = 0; i < 36; ++i) if (m_mat[i] != m[i]) return false;
   return true;
}

bool MatS6::operator!= (const MatS6& m) const {
   return !((*this) == m);
}

MatS6& MatS6::operator/= (const double d) {
   for (unsigned long i = 0; i < 36; ++i) m_mat[i] /= d;
   return *this;
}

MatS6& MatS6::operator*= (const double d) {
   for (unsigned long i = 0; i < 36; ++i) m_mat[i] *= d;
   return *this;
}

MatS6 MatS6::operator+ (const MatS6& v) const {
   MatS6 m(*this);
   return (m += v);
}

MatS6 MatS6::operator- (const MatS6& v) const {
   MatS6 m(*this);
   return (m -= v);
}

MatS6 MatS6::operator- (void) const { // unary
   MatS6 m(*this);
   m.m_mat = -m.m_mat;
   return m; // unary
}

MatS6 MatS6::operator* (const MatS6& m2) const {
   MatS6 m(*this);
   m.m_mat *= m2.m_mat;
   return m;
}

MatS6 MatS6::operator* (const double d) const {
   MatS6 m(*this);
   for (unsigned long i = 0; i < 36; ++i) m.m_mat[i] *= d;
   return m;
}

MatS6 MatS6::operator/ (const double d) const {
   MatS6 m(*this);
   for (unsigned long i = 0; i < 36; ++i) m.m_mat[i] /= d;
   return m;
}

MatS6 operator*(const double d, const MatS6& m) {
   return m*d;
}

MatS6 operator/(const double d, const MatS6& m) {
   return m / d;
}

S6 MatS6::operator* (const S6& v) const {
   return m_mat * v.GetVector();
}


double MatS6::operator[] (const unsigned long n) const {
   return m_mat[n];
}

double& MatS6::operator[] (const unsigned long n) {
   return m_mat[n];
}


double MatS6::DistanceBetween(const MatS6& v1, const MatS6& v2) {
   return ((v1 - v2).norm());
}

unsigned long MatS6::size(void) const {
   return 36;
}

double MatS6::norm() const {
   double sum = 0.0;
   for (unsigned long i = 0; i < 36; ++i) sum += m_mat[i];
   return sum;
}

double MatS6::norm(const MatS6& t) const {
   return t.norm();
}

double MatS6::Norm() const {
   return norm();
}

double MatS6::Norm(const MatS6& t) const {
   return t.norm();
}

bool MatS6::IsUnit() const {
   long row = -1;
   for (unsigned long i = 0; i < 36; ++i) {
      long column = i % 6;
      if (column == 0) ++row;
      if (column == row && m_mat[i] != 1.0) return false;
      if (column != row && m_mat[i] != 0.0) return false;
   }
   return true;
}

MatS6 MatS6::Eye(void) {
   MatS6 m;
   for (unsigned long i = 0; i < 36; ++i)
      m[i] = 0.0;
   for (unsigned long i = 0; i < 36; i += 7)
      m[i] = 1.0;
   return m;
}

MatS6 MatS6::Zero(void) {
   MatS6 m;
   for (unsigned long i = 0; i < 36; ++i)
      m[i] = 0.0;
   return m;
}

MatS6 MatS6::transpose(const MatS6& m2) const {
   //  transpose a symmetrical matrix
   const int size = 6;
   MatS6 m;
   for (int count = 0; count < size*size; ++count) {
      const int transposeIndex = count / size + size*(count%size); //'=int(rowindex/6) +6*colIndex)
      if (count >= transposeIndex) {
         m[transposeIndex] = m2[count];
         m[count] = m2[transposeIndex];
      }
   }
   return m;
}

void MatS6::transpose(void) {
   //  transpose a symmetrical matrix
   const int size = 6;
   MatS6& m(*this);
   MatS6 m2(*this);
   for (int count = 0; count < size*size; ++count) {
      const int transposeIndex = count / size + size*(count%size); //'=int(rowindex/6) +6*colIndex)
      if (count >= transposeIndex) {
         m[transposeIndex] = m2[count];
         m[count] = m2[transposeIndex];
      }
   }
}


double MatS6::at(const unsigned long n) const {
   return m_mat[n];
}

MatS6 MatS6::unit(void) {
   m_mat.resize(36);
   for (unsigned long i = 0; i < 36; ++i)
      m_mat[i] = 0.0;
   for (unsigned long i = 0; i < 36; i += 6 + 1)
      m_mat[i] = 1.0;
   return *this;
}

MatS6 MatS6::unit(const MatS6& min) {
   MatS6 m(min);
   for (unsigned long i = 0; i < 36; ++i)
      m.m_mat[i] = 0.0;
   for (unsigned long i = 0; i < 36; i += 6 + 1)
      m.m_mat[i] = 1.0;
   return m;
}

MatS6 MatS6::GetReflection(const unsigned long n) {
   if (vS6_Refl.empty()) GetReflections();
   return vS6_Refl[n];
}

std::vector<MatS6> MatS6::GetReflections() {
   if (vS6_Refl.empty()) {
      vS6_Refl.push_back(MatS6("1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1"));
      vS6_Refl.push_back(MatS6("0 0 0 0 1 0  0 0 0 1 0 0  0 0 1 0 0 0  0 1 0 0 0 0  1 0 0 0 0 0  0 0 0 0 0 1"));
      vS6_Refl.push_back(MatS6("1 0 0 0 0 0  0 0 1 0 0 0  0 1 0 0 0 0  0 0 0 1 0 0  0 0 0 0 0 1  0 0 0 0 1 0"));
      vS6_Refl.push_back(MatS6("0 0 0 0 0 1  0 0 0 1 0 0  0 1 0 0 0 0  0 0 1 0 0 0  1 0 0 0 0 0  0 0 0 0 1 0"));
      vS6_Refl.push_back(MatS6("0 0 0 0 1 0  0 0 1 0 0 0  0 0 0 1 0 0  0 1 0 0 0 0  0 0 0 0 0 1  1 0 0 0 0 0"));
      vS6_Refl.push_back(MatS6("0 0 0 0 0 1  0 1 0 0 0 0  0 0 0 1 0 0  0 0 1 0 0 0  0 0 0 0 1 0  1 0 0 0 0 0"));
      vS6_Refl.push_back(MatS6("0 1 0 0 0 0  1 0 0 0 0 0  0 0 1 0 0 0  0 0 0 0 1 0  0 0 0 1 0 0  0 0 0 0 0 1"));
      vS6_Refl.push_back(MatS6("0 0 0 1 0 0  0 0 0 0 1 0  0 0 1 0 0 0  1 0 0 0 0 0  0 1 0 0 0 0  0 0 0 0 0 1"));
      vS6_Refl.push_back(MatS6("0 1 0 0 0 0  0 0 1 0 0 0  1 0 0 0 0 0  0 0 0 0 1 0  0 0 0 0 0 1  0 0 0 1 0 0"));
      vS6_Refl.push_back(MatS6("0 0 0 0 0 1  0 0 0 0 1 0  1 0 0 0 0 0  0 0 1 0 0 0  0 1 0 0 0 0  0 0 0 1 0 0"));
      vS6_Refl.push_back(MatS6("0 0 0 1 0 0  0 0 1 0 0 0  0 0 0 0 1 0  1 0 0 0 0 0  0 0 0 0 0 1  0 1 0 0 0 0"));
      vS6_Refl.push_back(MatS6("0 0 0 0 0 1  1 0 0 0 0 0  0 0 0 0 1 0  0 0 1 0 0 0  0 0 0 1 0 0  0 1 0 0 0 0"));
      vS6_Refl.push_back(MatS6("0 0 1 0 0 0  1 0 0 0 0 0  0 1 0 0 0 0  0 0 0 0 0 1  0 0 0 1 0 0  0 0 0 0 1 0"));
      vS6_Refl.push_back(MatS6("0 0 0 1 0 0  0 0 0 0 0 1  0 1 0 0 0 0  1 0 0 0 0 0  0 0 1 0 0 0  0 0 0 0 1 0"));
      vS6_Refl.push_back(MatS6("0 0 1 0 0 0  0 1 0 0 0 0  1 0 0 0 0 0  0 0 0 0 0 1  0 0 0 0 1 0  0 0 0 1 0 0"));
      vS6_Refl.push_back(MatS6("0 0 0 0 1 0  0 0 0 0 0 1  1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 1 0 0"));
      vS6_Refl.push_back(MatS6("0 0 0 1 0 0  0 1 0 0 0 0  0 0 0 0 0 1  1 0 0 0 0 0  0 0 0 0 1 0  0 0 1 0 0 0"));
      vS6_Refl.push_back(MatS6("0 0 0 0 1 0  1 0 0 0 0 0  0 0 0 0 0 1  0 1 0 0 0 0  0 0 0 1 0 0  0 0 1 0 0 0"));
      vS6_Refl.push_back(MatS6("0 0 1 0 0 0  0 0 0 0 1 0  0 0 0 1 0 0  0 0 0 0 0 1  0 1 0 0 0 0  1 0 0 0 0 0"));
      vS6_Refl.push_back(MatS6("0 1 0 0 0 0  0 0 0 0 0 1  0 0 0 1 0 0  0 0 0 0 1 0  0 0 1 0 0 0  1 0 0 0 0 0"));
      vS6_Refl.push_back(MatS6("0 0 1 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1  1 0 0 0 0 0  0 1 0 0 0 0"));
      vS6_Refl.push_back(MatS6("1 0 0 0 0 0  0 0 0 0 0 1  0 0 0 0 1 0  0 0 0 1 0 0  0 0 1 0 0 0  0 1 0 0 0 0"));
      vS6_Refl.push_back(MatS6("0 1 0 0 0 0  0 0 0 1 0 0  0 0 0 0 0 1  0 0 0 0 1 0  1 0 0 0 0 0  0 0 1 0 0 0"));
      vS6_Refl.push_back(MatS6("1 0 0 0 0 0  0 0 0 0 1 0  0 0 0 0 0 1  0 0 0 1 0 0  0 1 0 0 0 0  0 0 1 0 0 0"));
   }
   return vS6_Refl;
}
