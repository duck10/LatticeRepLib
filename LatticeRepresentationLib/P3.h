#ifndef P3_H
#define P3_H

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

#include "LRL_Cell.h"

class P3 {
public:
   friend std::ostream& operator<< (std::ostream& o, const P3& p3);

   P3() = default;
   template<typename T>
   P3(const T& t) {
      LRL_Cell cell(t);
      const double& a(cell[0]);
      const double& b(cell[1]);
      const double& c(cell[2]);
      const double& alpha(cell[3]);
      const double& beta(cell[4]);
      const double& gamma(cell[5]);
      m_p3 = {
         {a * cos(alpha),a * sin(alpha)},
         {b * cos(beta), b * sin(beta)},
         {c * cos(gamma),c * sin(gamma)} };
   }

   std::pair<double, double> operator[] (const size_t n) const {
      return m_p3[n];
   }

   std::pair<double, double>& operator[] (const size_t n) {
      return m_p3[n];
   }

   static P3 rand();
   static P3 RandWork();
   double norm() const;
   double Norm() const { return norm(); }
   static double norm(const P3& p3) { return p3.norm(); }
   static double Norm(const P3& p3) { return p3.norm(); }
   P3 operator-(const P3& p3) const;
   P3 operator+(const P3& p3) const;

   static double DistanceBetween(const P3& p1, const P3& p2) {
      const P3 diff = p1 - p2;
      return diff.norm();
   }

private:
   std::vector<std::pair<double, double> > m_p3;
};

#endif // P3_H