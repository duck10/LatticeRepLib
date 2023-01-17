#include "MatDC7u.h"

#include <iomanip>

MatDC7u::MatDC7u() : m_mat(7*7) {}

void MatDC7u::zeros() {
   for (size_t i = 0; i < 7*7; ++i) m_mat[i] = 0.0; 
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


MatDC7u MatDC7u::operator+ (const MatDC7u& m) const {
   MatDC7u m6(*this);
   return (m6 += m);
}

MatDC7u MatDC7u::operator- (const MatDC7u& m) const {
   MatDC7u m6(*this);
   return (m6 -= m);
}

MatDC7u MatDC7u::operator- (void) const { // unary
   MatDC7u m(*this);
   m.m_mat = -m.m_mat;
   return m; // unary

}
MatDC7u& MatDC7u::operator+= (const MatDC7u& d) {
   for (size_t i = 0; i < 36; ++i) m_mat[i] += d[i];
   return *this;
}

MatDC7u& MatDC7u::operator-= (const MatDC7u& d) {
   for (size_t i = 0; i < 36; ++i) m_mat[i] -= d[i];
   return *this;
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

