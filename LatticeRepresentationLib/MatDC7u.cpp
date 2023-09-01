#include "MatDC7u.h"
#include "MatMN.h"
#include "MatN.h"

#include <iomanip>
#include <string>
#include <vector>

#pragma warning( disable : 4702) //  unreachable code
#pragma warning( disable : 4100) //  unreferenced formal parameter

//std::vector<MatDC7u> MatDC7u::vDU_Refl = MatDC7u::GetReflections();

MatDC7u::MatDC7u(void)
   : m_mat(49)
{}

MatDC7u& MatDC7u::operator+= (const MatDC7u& d) {
   for (size_t i = 0; i < 49; ++i) m_mat[i] += d[i];
   return *this;
}

MatDC7u& MatDC7u::operator-= (const MatDC7u& d) {
   for (size_t i = 0; i < 49; ++i) m_mat[i] -= d[i];
   return *this;
}

bool MatDC7u::operator== (const MatDC7u& m) const {
   for (size_t i = 0; i < 49; ++i) if (m_mat[i] != m[i]) return false;
   return true;
}

bool MatDC7u::operator!= (const MatDC7u& m) const {
   return !((*this) == m);
}

MatDC7u& MatDC7u::operator/= (const double d) {
   for (size_t i = 0; i < 49; ++i) m_mat[i] /= d;
   return *this;
}

MatDC7u& MatDC7u::operator*= (const double d) {
   for (size_t i = 0; i < 49; ++i) m_mat[i] *= d;
   return *this;
}

MatDC7u MatDC7u::operator+ (const MatDC7u& m) const {
   MatDC7u m7(*this);
   return (m7 += m);
}

MatDC7u MatDC7u::operator- (const MatDC7u& m) const {
   MatDC7u m7(*this);
   return (m7 -= m);
}

MatDC7u MatDC7u::operator- (void) const { // unary
   MatDC7u m(*this);
   m.m_mat = -m.m_mat;
   return m; // unary
}

MatDC7u MatDC7u::operator* (const MatDC7u& m2) const {
   MatDC7u m(*this);
   m.m_mat *= m2.m_mat;
   return m;
}

MatDC7u MatDC7u::operator* (const double d) const {
   MatDC7u m(*this);
   for (size_t i = 0; i < 49; ++i) m.m_mat[i] *= d;
   return m;
}

MatDC7u MatDC7u::operator/ (const double d) const {
   MatDC7u m(*this);
   for (size_t i = 0; i < 49; ++i) m.m_mat[i] /= d;
   return m;
}

MatDC7u operator*(const double d, const MatDC7u& m) {
   return m*d;
}

MatDC7u operator/(const double d, const MatDC7u& m) {
   return m / d;
}

DC7u MatDC7u::operator* (const DC7u& v) const {
   DC7u g7(v);
   g7.SetVector( (m_mat * v.GetVector()).GetVector());
   g7.SetValid(g7[0]>0.001 && g7[1]>0.001 && g7[2]>0.001);
   return g7;
}

double MatDC7u::operator[] (const size_t n) const {
   return m_mat[n];
}

double& MatDC7u::operator[] (const size_t n) {
   return m_mat[n];
}


double MatDC7u::norm() const {
   double sum = 0.0;
   for (size_t i = 0; i < 7*7; ++i) sum += m_mat[i] * m_mat[i];
   return sqrt(sum);
}

double MatDC7u::norm(const MatDC7u& t) const {
   return t.norm();
}

double MatDC7u::Norm() const {
   return norm();
}

double MatDC7u::Norm(const MatDC7u& t) const {
   return t.norm();
}


bool MatDC7u::IsUnit() const {
   long row = -1;
   for (size_t i = 0; i < 49; ++i) {
      long column = i % 6;
      if (column == 0) ++row;
      if (column == row && m_mat[i] != 1.0) return false;
      if (column != row && m_mat[i] != 0.0) return false;
   }
   return true;
}

MatDC7u MatDC7u::Eye(void) {
   MatDC7u m;
   for (size_t i = 0; i < 49; ++i)
      m.m_mat[i] = 0.0;
   for (size_t i = 0; i < 49; i += 7)
      m.m_mat[i] = 1.0;
   return m;
}

MatDC7u MatDC7u::Zero(void) {
   MatDC7u m;
   for (size_t i = 0; i < 49; ++i)
      m[i] = 0.0;
   return m;
}


MatDC7u MatDC7u::transpose(const MatDC7u& m2) const {
   //  transpose a symmetrical matrix
   const int size = 7;
   MatDC7u m;
   for (int count = 0; count<size*size; ++count) {
      const int transposeIndex = count / size + size*(count%size); //'=int(rowindex/6) +6*colIndex)
      if (count >= transposeIndex) {
         m[transposeIndex] = m2[count];
         m[count] = m2[transposeIndex];
      }
   }
   return m;
}

void MatDC7u::transpose(void) {
   //  transpose a symmetrical matrix
   const int size = 7;
   MatDC7u& m(*this);
   MatDC7u m2(*this);
   for (int count = 0; count<size*size; ++count) {
      const int transposeIndex = count / size + size*(count%size); //'=int(rowindex/6) +6*colIndex)
      if (count >= transposeIndex) {
         m[transposeIndex] = m2[count];
         m[count] = m2[transposeIndex];
      }
   }
}

std::ostream& operator<< (std::ostream& o, const MatDC7u& dc) {
   std::streamsize oldPrecision = o.precision();
   o << std::fixed << std::setprecision(5);
   for (size_t i = 0; i < dc.size(); ++i) {
      if (i % 7 == 0) o << std::endl;
      o << int(dc[i]) << " ";
      //o << std::setw(9) << dc[i] << " ";
   }
   o << std::setprecision(oldPrecision);
   o.unsetf(std::ios::floatfield);
   return o;
}

