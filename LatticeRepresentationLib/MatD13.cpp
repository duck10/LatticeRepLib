#include "MatD13.h"

#include <iomanip>

MatD13::MatD13() : m_mat(13*13) {}

void MatD13::zeros() {
   for (size_t i = 0; i < 13*13; ++i) m_mat[i] = 0.0; 
}

double MatD13::operator[] (const size_t n) const {
   return m_mat[n];
}

double& MatD13::operator[] (const size_t n) {
   return m_mat[n];
}


double MatD13::norm() const {
   double sum = 0.0;
   for (size_t i = 0; i < 13*13; ++i) sum += m_mat[i] * m_mat[i];
   return sqrt(sum);
}

double MatD13::norm(const MatD13& t) const {
   return t.norm();
}

double MatD13::Norm() const {
   return norm();
}

double MatD13::Norm(const MatD13& t) const {
   return t.norm();
}


MatD13 MatD13::operator+ (const MatD13& m) const {
   MatD13 m6(*this);
   return (m6 += m);
}

MatD13 MatD13::operator- (const MatD13& m) const {
   MatD13 m6(*this);
   return (m6 -= m);
}

MatD13 MatD13::operator- (void) const { // unary
   MatD13 m(*this);
   m.m_mat = -m.m_mat;
   return m; // unary

}
MatD13& MatD13::operator+= (const MatD13& d) {
   for (size_t i = 0; i < 36; ++i) m_mat[i] += d[i];
   return *this;
}

MatD13& MatD13::operator-= (const MatD13& d) {
   for (size_t i = 0; i < 36; ++i) m_mat[i] -= d[i];
   return *this;
}


std::ostream& operator<< (std::ostream& o, const MatD13& dc) {
   std::streamsize oldPrecision = o.precision();
   o << std::fixed << std::setprecision(5);
   for (size_t i = 0; i < dc.size(); ++i) {
      if (i % 13 == 0) o << std::endl;
      o << int(dc[i]) << " ";
      //o << std::setw(9) << dc[i] << " ";
   }
   o << std::setprecision(oldPrecision);
   o.unsetf(std::ios::floatfield);
   return o;
}

