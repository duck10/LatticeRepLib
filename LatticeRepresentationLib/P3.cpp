#include "P3.h"

std::ostream& operator<< (std::ostream& o, const P3& p3) {
   o << "(("
      << p3[0].first << ", " << p3[0].second << "), ("
      << p3[1].first << ", " << p3[1].second << "), ("
      << p3[2].first << ", " << p3[2].second << "))";
   return o;
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
   const P3& p(*this);
   P3 out;
   out.m_p3 = std::vector<std::pair<double, double>>{
      {p[0].first - p3[0].first, p[0].second - p3[0].second},
      {p[1].first - p3[1].first, p[1].second - p3[1].second},
      {p[2].first - p3[2].first, p[2].second - p3[2].second}
   };
   return out;
}

P3 P3::operator-(const P3& p3) const {
   const P3& p(*this);
   P3 out;
   out.m_p3 = std::vector<std::pair<double, double>>{
      {p[0].first - p3[0].first, p[0].second - p3[0].second},
      {p[1].first - p3[1].first, p[1].second - p3[1].second},
      {p[2].first - p3[2].first, p[2].second - p3[2].second}
   };
   return out;
}
