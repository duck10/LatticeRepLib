#ifndef P3_H
#define P3_H

#include <array>
#include <cmath>
#include <iostream>
#include <utility>

#include "BasisBase.h"
#include "LRL_Cell.h"
#include "Vector_2.h"
#include "rhrand.h"

class P3 {
public:
   using Vec2 = std::pair<double, double>;
   using Container = std::array<Vec2, 3>;
   using iterator = Container::iterator;
   using const_iterator = Container::const_iterator;

   P3();  // Default constructor
   explicit P3(const LRL_Cell& cell);  // Construct from cell geometry

   explicit P3(const std::vector<Vector_2>& vecs) {
      if (vecs.size() != 3) {
         std::cerr << "P3 constructor requires exactly 3 Vector_2 elements\n";
         const std::pair<double, double> zero2D(0.0, 0.0);
         m_p3 = { zero2D, zero2D, zero2D };
      }
      else
      {
         for (size_t i = 0; i < 3; ++i)
            m_p3[i] = std::make_pair(vecs[i][0], vecs[i][1]);
      }
   }

   P3(const double a, const double b, const double c, const double d,
      const double e, const double f) {
      m_p3[0].first = a;
      m_p3[0].second = b;
      m_p3[1].first = c;
      m_p3[1].second = d;
      m_p3[2].first = e;
      m_p3[2].second = f;
   }

   P3(const P3& p3) {
      m_valid = p3.m_valid;
      m_p3 = p3.m_p3;
   }

   explicit P3(const std::array<Vector_2, 3>& vecs) {
      for (size_t i = 0; i < 3; ++i)
         m_p3[i] = std::make_pair(vecs[i][0], vecs[i][1]);
   }

   static double EuclideanDistance(const P3& p1, const P3& p2);

   std::array<Vector_2, 3> GetVectorTriplet() const {
      std::array<Vector_2, 3> out;
      for (size_t i = 0; i < 3; ++i)
         out[i] = Vector_2(m_p3[i].first, m_p3[i].second);
      return out;
   }



   P3(const Vector_2& v1, const Vector_2& v2, const Vector_2& v3) {
      m_p3[0] = std::make_pair(v1[0], v1[1]);
      m_p3[1] = std::make_pair(v2[0], v2[1]);
      m_p3[2] = std::make_pair(v3[0], v3[1]);
   }


   iterator begin() { return m_p3.begin(); }
   iterator end() { return m_p3.end(); }
   const_iterator begin() const { return m_p3.begin(); }
   const_iterator end()   const { return m_p3.end(); }
   const_iterator cbegin() const { return m_p3.cbegin(); }
   const_iterator cend()   const { return m_p3.cend(); }

   Vec2& operator[](std::size_t i) { return m_p3[i]; }
   const Vec2& operator[](std::size_t i) const { return m_p3[i]; }

   double norm() const;
   static double Norm(const P3& p3) { return p3.norm(); }
   static double DistanceBetween(const P3& p1, const P3& p2) {
      return (p1 - p2).norm();
   }
   static double DistanceBetween(const LRL_Cell& p1, const LRL_Cell& p2) {
      return (P3(p1)-P3(p2)).norm();
   }

   P3 operator-(const P3& other) const;
   P3 operator+(const P3& other) const;

   P3 operator-() const; // unary minus

   std::array<Vec2, 3> GetVector() const {
      return m_p3;
   }

   std::vector<double> GetFlatVector() const {
      std::vector<double> out(6);
      const P3& p(*this);
      out[0] = p[0].first;
      out[1] = p[0].second;
      out[2] = p[1].first;
      out[3] = p[1].second;
      out[4] = p[2].first;
      out[5] = p[2].second;
      return out;
   }

   // --- Dot Product between two P3s ---
   static double Dot(const P3& a, const P3& b) {
      double result = 0.0;
      for (size_t i = 0; i < 3; ++i)
         result += a[i].first * b[i].first + a[i].second * b[i].second;
      return result;
   }

   static double dot(const P3& a, const P3& b) {
      return Dot(a, b);
   }

   double Dot(const P3& v) {
      return Dot((*this), v);
   }

   double dot(const P3& v) {
      return Dot((*this), v);
   }

   // --- Scalar multiplication ---
   P3 operator*(const double scalar) const {
      return P3(
         m_p3[0].first * scalar, m_p3[0].second * scalar,
         m_p3[1].first * scalar, m_p3[1].second * scalar,
         m_p3[2].first * scalar, m_p3[2].second * scalar
      );
   }

   // --- Scalar division ---
   P3 operator/(const double scalar) const {
      return P3(
         m_p3[0].first / scalar, m_p3[0].second / scalar,
         m_p3[1].first / scalar, m_p3[1].second / scalar,
         m_p3[2].first / scalar, m_p3[2].second / scalar
      );
   }

   //// --- Scalar multiplication ---
   //friend P3 operator*(const P3& p3, const double scalar) {
   //   return P3(
   //      p3[0].first * scalar, p3[0].second * scalar,
   //      p3[1].first * scalar, p3[1].second * scalar,
   //      p3[2].first * scalar, p3[2].second * scalar
   //   );
   //}

   // --- Scalar multiplication ---
   friend P3 operator*(const double scalar, const P3& p3) {
      P3 pnew{
       p3[0].first * scalar, p3[0].second * scalar,
       p3[1].first * scalar, p3[1].second * scalar,
       p3[2].first * scalar, p3[2].second * scalar };
      pnew.SetValid(true);
      return pnew;
   }

   // --- Scalar division ---
   friend P3 operator/(const double scalar, const P3& p3) {
      return P3(
         p3[0].first / scalar, p3[0].second / scalar,
         p3[1].first / scalar, p3[1].second / scalar,
         p3[2].first / scalar, p3[2].second / scalar
      );
   }

   friend  bool operator==(const P3& a, const P3& b) {
      return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
  }

   friend bool operator!=(const P3& a, const P3& b) {
      return a[0] != b[0] && a[1] != b[1] && a[2] != b[2];
   }

   // --- Normalization ---
   P3 Normalized() const {
      const double n = this->norm();
      return (n > 0.0) ? (*this) * (1.0 / n) : *this;
   }

   // --- Projection of one P3 onto another ---
   static P3 Project(const P3& onto, const P3& source) {
      const double dot = Dot(onto, source);
      const double normSq = Dot(onto, onto);
      const double scale = (normSq > 0.0) ? (dot / normSq) : 0.0;
      return onto * scale;
   }

   bool GetValid() const { return m_valid; }
   void SetValid(const bool valid) { m_valid = valid; }

   RHrand ranx;

   Vector_2 GeneratePolarProjection() {
      const Vector_2 v(2.0 * (ranx.urand() - 0.5), ranx.urand());
      return v;
   }


    P3 CreateUnitOrthogonalComponent(const P3& p3) {
      const P3 base = p3.Normalized();  // Unit direction
      P3 temp;

      // Generate random P3 vector in [-0.5, 0.5] range
      for (size_t i = 0; i < 3; ++i) {
         const Vector_2 v = GeneratePolarProjection();
         temp[i].first = v[0];
         temp[i].second = v[1];
      }

      temp = temp.Normalized();

      // Compute orthogonal component
      const double dotVal = P3::Dot(base, temp);  // well-known P3 dot product
      P3 ortho = temp - base * dotVal;
      ortho = ortho.Normalized();
      ortho.SetValid(true);

      return ortho;
   }


   friend std::ostream& operator<< (std::ostream& o, const P3& p3);

protected:
   Container m_p3;
   bool m_valid;
};




#endif  // P3_H
