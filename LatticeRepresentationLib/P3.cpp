#include "P3.h"
#include "Polar.h"

#include <cmath>

std::ostream& operator<< (std::ostream& o, const P3& p3) {
   o << "(("
      << p3[0].first << ", " << p3[0].second << "), ("
      << p3[1].first << ", " << p3[1].second << "), ("
      << p3[2].first << ", " << p3[2].second << "))";
   return o;
}

P3::P3() {
   P3& p(*this);
   p[0] = { 19191,19191 };
   p[1] = { 19191,19191 };
   p[2] = { 19191,19191 };
   m_valid = false;
}

P3::P3(const LRL_Cell& cell) {
   P3& p(*this);
   const double& a = cell[0];
   const double& b = cell[1];
   const double& c = cell[2];
   const double& alpha = cell[3];
   const double& beta = cell[4];
   const double& gamma = cell[5];
   p[0] = { a * cos(alpha),a * sin(alpha) };
   p[1] = { b * cos(beta), b * sin(beta) };
   p[2] = { c * cos(gamma), c * sin(gamma) };
   m_valid = cell.CheckValid(p);
}


double P3::norm() const {
   const P3& p(*this);
   return sqrt(
      p[0].first * p[0].first + p[0].second * p[0].second +
      p[1].first * p[1].first + p[1].second * p[1].second +
      p[2].first * p[2].first + p[2].second * p[2].second
   );
}

P3 P3::operator+(const P3& p3) const {
   P3 out;
   for (std::size_t i = 0; i < 3; ++i) {
      out[i].first = m_p3[i].first + p3[i].first;
      out[i].second = m_p3[i].second + p3[i].second;
   }
   out.SetValid(p3.GetValid() && (*this).GetValid());
   return out;
}

P3 P3::operator-(const P3& p3) const {
   P3 out;
   for (std::size_t i = 0; i < 3; ++i) {
      out[i].first = m_p3[i].first - p3[i].first;
      out[i].second = m_p3[i].second - p3[i].second;
   }
   return out;
}

P3 P3::operator-() const {
   P3 result;
   for (std::size_t i = 0; i < 3; ++i) {
      result[i].first = -m_p3[i].first;
      result[i].second = -m_p3[i].second;
   }
   return result;
}

double P3::EuclideanDistance(const P3& p1, const P3& p2) {
   return (p1 - p2).norm();
}