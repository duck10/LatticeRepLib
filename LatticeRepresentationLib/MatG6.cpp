


#include "LRL_CoordinateConversionMatrices.h"
#include "G6.h"
#include "MatS6.h"
#include "MatD7.h"
#include "MatG6.h"
#include "MatMN.h"
#include "MatN.h"

#include <string>
#include <vector>

#pragma warning( disable : 4702) //  unreachable code
#pragma warning( disable : 4100) //  unreferenced formal parameter

std::vector<MatG6> MatG6::vG6_Refl = MatG6::GetReflections();



MatG6::MatG6(void)
   : m_mat(36)
{}

//MatG6::MatG6(const LRL_Cell& m)
//   : m_mat(36)
//{throw;}

MatG6::MatG6(const MatS6& m)
   : MatG6()
{
   MatMN mnS6(6,6);
   for (size_t i = 0; i < m.size(); ++i) mnS6[i] = m[i];
   MatMN mn = LRL_CoordinateConversionMatrices::G6_FROM_S6 * mnS6 * LRL_CoordinateConversionMatrices::S6_FROM_G6;
   m_mat.SetVector(mn.GetVector());
}

MatG6::MatG6(const MatD7& m)
   : MatG6()
{
   MatMN mnD7(7, 7);
   for (size_t i = 0; i < m.size(); ++i) mnD7[i] = m[i];
   MatMN mn = LRL_CoordinateConversionMatrices::G6_FROM_D7 * mnD7 * LRL_CoordinateConversionMatrices::D7_FROM_G6;
   m_mat.SetVector(mn.GetVector());
}

MatG6::MatG6(const MatG6& m)
   : m_mat(m.m_mat)
{
}

MatG6::MatG6(const MatN& m)
   : m_mat(m)
{}

MatG6::MatG6(const MatMN& m)
   : m_mat(m.GetDim())
{
   if (m.GetColDim() != m.GetRowDim()) throw;
   const size_t n = m.GetRowDim();
   m_mat.resize(n*n);
   for (size_t i = 0; i < n; ++i) m_mat[i] = m[i];
   m_mat.SetDim(n*n);
   m_mat.SetRowDim(n);
}

MatG6::MatG6(const std::string& s)
   : MatG6()
{
   MatN mn(m_mat.FromString(s));
   (*this).m_mat = mn;
}

MatG6::MatG6(const std::vector<double>& v)
   : MatG6()
{
   for (size_t i = 0; i < 36; ++i) {
      m_mat[i] = v[i];
   }
}

MatG6& MatG6::operator= (const MatB4& m) {
   m_mat.resize(36);
   throw("we don't know how to do this");
   return *this;
}

MatG6& MatG6::operator= (const MatS6& m) {
   MatMN mn = LRL_CoordinateConversionMatrices::G6_FROM_S6 * m * LRL_CoordinateConversionMatrices::S6_FROM_G6;
   m_mat.SetVector(mn.GetVector());
   return *this;
}

MatG6& MatG6::operator= (const MatG6& m) {
   m_mat.resize(36);
   m_mat = m.m_mat;
   return *this;
}

MatG6& MatG6::operator= (const MatD7& m) {
   MatMN mn = LRL_CoordinateConversionMatrices::G6_FROM_D7 * m * LRL_CoordinateConversionMatrices::D7_FROM_G6;
   m_mat.SetVector(mn.GetVector());
   return *this;
}

MatG6& MatG6::operator+= (const MatG6& d) {
   for (size_t i = 0; i < 36; ++i) m_mat[i] += d[i];
   return *this;
}

MatG6& MatG6::operator-= (const MatG6& d) {
   for (size_t i = 0; i < 36; ++i) m_mat[i] -= d[i];
   return *this;
}

bool MatG6::operator== (const MatG6& m) const {
   for (size_t i = 0; i < 36; ++i) if (m_mat[i] != m[i]) return false;
   return true;
}

bool MatG6::operator!= (const MatG6& m) const {
   return !((*this) == m);
}

MatG6& MatG6::operator/= (const double d) {
   for (size_t i = 0; i < 36; ++i) m_mat[i] /= d;
   return *this;
}

MatG6& MatG6::operator*= (const double d) {
   for (size_t i = 0; i < 36; ++i) m_mat[i] *= d;
   return *this;
}

MatG6 MatG6::operator+ (const MatG6& m) const {
   MatG6 m6(*this);
   return (m6 += m);
}

MatG6 MatG6::operator- (const MatG6& m) const {
   MatG6 m6(*this);
   return (m6 -= m);
}

MatG6 MatG6::operator- (void) const { // unary
   MatG6 m(*this);
   m.m_mat = -m.m_mat;
   return m; // unary
}

MatG6 MatG6::operator* (const MatG6& m2) const {
   MatG6 m(*this);
   m.m_mat *= m2.m_mat;
   return m;
}

MatG6 MatG6::operator* (const double d) const {
   MatG6 m(*this);
   for (size_t i = 0; i < 36; ++i) m.m_mat[i] *= d;
   return m;
}

MatG6 MatG6::operator/ (const double d) const {
   MatG6 m(*this);
   for (size_t i = 0; i < 36; ++i) m.m_mat[i] /= d;
   return m;
}

MatG6 operator*(const double d, const MatG6& m) {
   return m*d;
}

MatG6 operator/(const double d, const MatG6& m) {
   return m / d;
}

G6 MatG6::operator* (const G6& v) const {
   G6 g6(v);
   g6.SetVector( (m_mat * v.GetVector()).GetVector());
   g6.SetValid(g6[0]>0.001 && g6[1]>0.001 && g6[2]>0.001);
   return g6;
}


double MatG6::operator[] (const size_t n) const {
   return m_mat[n];
}

double& MatG6::operator[] (const size_t n) {
   return m_mat[n];
}


double MatG6::DistanceBetween(const MatG6& v1, const MatG6& v2) {
   return ((v1 - v2).norm());
}

size_t MatG6::size(void) const {
   return 36;
}

double MatG6::norm() const {
   double sum = 0.0;
   for (size_t i = 0; i < 36; ++i) sum += m_mat[i] * m_mat[i];
   return sqrt(sum);
}

double MatG6::norm(const MatG6& t) const {
   return t.norm();
}

double MatG6::Norm() const {
   return norm();
}

double MatG6::Norm(const MatG6& t) const {
   return t.norm();
}

bool MatG6::IsUnit() const {
   long row = -1;
   for (size_t i = 0; i < 36; ++i) {
      long column = i % 6;
      if (column == 0) ++row;
      if (column == row && m_mat[i] != 1.0) return false;
      if (column != row && m_mat[i] != 0.0) return false;
   }
   return true;
}

MatG6 MatG6::Eye(void) {
   MatG6 m;
   for (size_t i = 0; i < 36; ++i)
      m.m_mat[i] = 0.0;
   for (size_t i = 0; i < 36; i += 7)
      m.m_mat[i] = 1.0;
   return m;
}

MatG6 MatG6::Zero(void) {
   MatG6 m;
   for (size_t i = 0; i < 36; ++i)
      m[i] = 0.0;
   return m;
}

MatG6 MatG6::transpose(const MatG6& m2) const {
   //  transpose a symmetrical matrix
   const int size = 6;
   MatG6 m;
   for (int count = 0; count<size*size; ++count) {
      const int transposeIndex = count / size + size*(count%size); //'=int(rowindex/6) +6*colIndex)
      if (count >= transposeIndex) {
         m[transposeIndex] = m2[count];
         m[count] = m2[transposeIndex];
      }
   }
   return m;
}

void MatG6::transpose(void) {
   //  transpose a symmetrical matrix
   const int size = 6;
   MatG6& m(*this);
   MatG6 m2(*this);
   for (int count = 0; count<size*size; ++count) {
      const int transposeIndex = count / size + size*(count%size); //'=int(rowindex/6) +6*colIndex)
      if (count >= transposeIndex) {
         m[transposeIndex] = m2[count];
         m[count] = m2[transposeIndex];
      }
   }
}


double MatG6::at(const size_t n) const {
   return m_mat[n];
}

MatG6 MatG6::unit(void) {
   m_mat.resize(36);
   for (size_t i = 0; i < 36; ++i)
      m_mat[i] = 0.0;
   for (size_t i = 0; i < 36; i += 6 + 1)
      m_mat[i] = 1.0;
   return *this;
}

MatG6 MatG6::unit(const MatG6& min) {
   MatG6 m(min);
   for (size_t i = 0; i < 36; ++i)
      m.m_mat[i] = 0.0;
   for (size_t i = 0; i < 36; i += 6 + 1)
      m.m_mat[i] = 1.0;
   return m;
}

//static const MatD7 mReducer( "0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0   1 1 1 1 -1 -1 -1   0 0 0 0 0 0 0   0 0 0 0 0 0 0   0 0 0 0 0 0 0");
static std::vector<MatG6> vReducer;

bool IsBetter(const MatG6& m1, const MatG6& m2) {
   return m1.norm() < m2.norm();
}

void MatG6::ReplaceTwosInRows7x7(MatG6& m) {
   throw;
}

MatG6  MatG6::GetReflection(const size_t n) {
   if (vG6_Refl.empty()) GetReflections();
   return vG6_Refl[n];
}

std::vector<MatG6> MatG6::GetReflections() {
   if (vG6_Refl.empty()) {
      vG6_Refl.resize(24);
      size_t i = 0;
      vG6_Refl[i++] = MatG6("1 0 0 0 0 0  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 1 0 0  0 0 0 0 1 0  0 0 0 0 0 1");
      vG6_Refl[i++] = MatG6("1 0 0 0 0 0  0 1 0 0 0 0  1 1 1 1 1 1  0 -2 0 -1 0 -1  -2 0 0 0 -1 -1  0 0 0 0 0 1");
      vG6_Refl[i++] = MatG6("1 0 0 0 0 0  0 0 1 0 0 0  0 1 0 0 0 0  0 0 0 1 0 0  0 0 0 0 0 1  0 0 0 0 1 0");
      vG6_Refl[i++] = MatG6("1 0 0 0 0 0  0 0 1 0 0 0  1 1 1 1 1 1  0 0 -2 -1 -1 0  -2 0 0 0 -1 -1  0 0 0 0 1 0");
      vG6_Refl[i++] = MatG6("1 0 0 0 0 0  1 1 1 1 1 1  0 1 0 0 0 0  0 -2 0 -1 0 -1  0 0 0 0 0 1  -2 0 0 0 -1 -1");
      vG6_Refl[i++] = MatG6("1 0 0 0 0 0  1 1 1 1 1 1  0 0 1 0 0 0  0 0 -2 -1 -1 0  0 0 0 0 1 0  -2 0 0 0 -1 -1");
      vG6_Refl[i++] = MatG6("0 1 0 0 0 0  1 0 0 0 0 0  0 0 1 0 0 0  0 0 0 0 1 0  0 0 0 1 0 0  0 0 0 0 0 1");
      vG6_Refl[i++] = MatG6("0 1 0 0 0 0  1 0 0 0 0 0  1 1 1 1 1 1  -2 0 0 0 -1 -1  0 -2 0 -1 0 -1  0 0 0 0 0 1");
      vG6_Refl[i++] = MatG6("0 1 0 0 0 0  0 0 1 0 0 0  1 0 0 0 0 0  0 0 0 0 1 0  0 0 0 0 0 1  0 0 0 1 0 0");
      vG6_Refl[i++] = MatG6("0 1 0 0 0 0  0 0 1 0 0 0  1 1 1 1 1 1  0 0 -2 -1 -1 0  0 -2 0 -1 0 -1  0 0 0 1 0 0");
      vG6_Refl[i++] = MatG6("0 1 0 0 0 0  1 1 1 1 1 1  1 0 0 0 0 0  -2 0 0 0 -1 -1  0 0 0 0 0 1  0 -2 0 -1 0 -1");
      vG6_Refl[i++] = MatG6("0 1 0 0 0 0  1 1 1 1 1 1  0 0 1 0 0 0  0 0 -2 -1 -1 0  0 0 0 1 0 0  0 -2 0 -1 0 -1");
      vG6_Refl[i++] = MatG6("0 0 1 0 0 0  1 0 0 0 0 0  0 1 0 0 0 0  0 0 0 0 0 1  0 0 0 1 0 0  0 0 0 0 1 0");
      vG6_Refl[i++] = MatG6("0 0 1 0 0 0  1 0 0 0 0 0  1 1 1 1 1 1  -2 0 0 0 -1 -1  0 0 -2 -1 -1 0  0 0 0 0 1 0");
      vG6_Refl[i++] = MatG6("0 0 1 0 0 0  0 1 0 0 0 0  1 0 0 0 0 0  0 0 0 0 0 1  0 0 0 0 1 0  0 0 0 1 0 0");
      vG6_Refl[i++] = MatG6("0 0 1 0 0 0  0 1 0 0 0 0  1 1 1 1 1 1  0 -2 0 -1 0 -1  0 0 -2 -1 -1 0  0 0 0 1 0 0");
      vG6_Refl[i++] = MatG6("0 0 1 0 0 0  1 1 1 1 1 1  1 0 0 0 0 0  -2 0 0 0 -1 -1  0 0 0 0 1 0  0 0 -2 -1 -1 0");
      vG6_Refl[i++] = MatG6("0 0 1 0 0 0  1 1 1 1 1 1  0 1 0 0 0 0  0 -2 0 -1 0 -1  0 0 0 1 0 0  0 0 -2 -1 -1 0");
      vG6_Refl[i++] = MatG6("1 1 1 1 1 1  1 0 0 0 0 0  0 1 0 0 0 0  0 0 0 0 0 1  0 -2 0 -1 0 -1  -2 0 0 0 -1 -1");
      vG6_Refl[i++] = MatG6("1 1 1 1 1 1  1 0 0 0 0 0  0 0 1 0 0 0  0 0 0 0 1 0  0 0 -2 -1 -1 0  -2 0 0 0 -1 -1");
      vG6_Refl[i++] = MatG6("1 1 1 1 1 1  0 1 0 0 0 0  1 0 0 0 0 0  0 0 0 0 0 1  -2 0 0 0 -1 -1  0 -2 0 -1 0 -1");
      vG6_Refl[i++] = MatG6("1 1 1 1 1 1  0 1 0 0 0 0  0 0 1 0 0 0  0 0 0 1 0 0  0 0 -2 -1 -1 0  0 -2 0 -1 0 -1");
      vG6_Refl[i++] = MatG6("1 1 1 1 1 1  0 0 1 0 0 0  1 0 0 0 0 0  0 0 0 0 1 0  -2 0 0 0 -1 -1  0 0 -2 -1 -1 0");
      vG6_Refl[i++] = MatG6("1 1 1 1 1 1  0 0 1 0 0 0  0 1 0 0 0 0  0 0 0 1 0 0  0 -2 0 -1 0 -1  0 0 -2 -1 -1 0");
   }
   return vG6_Refl;
}
