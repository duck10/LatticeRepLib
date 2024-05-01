#ifndef VECTOR_2_H
#define VECTOR_2_H
#include <iostream>
#include <vector>

#include "Radial.h"

class Vector_2 {
public:
   Vector_2();
   Vector_2(const double d1, const double d2);
   Vector_2(const Vector_2& v);
   friend std::ostream& operator<< (std::ostream& o, const Vector_2& v);
   Vector_2 AveragePosition(const std::vector<Vector_2>& vv) const;

   Vector_2& operator = (const Vector_2& v);
   Vector_2 operator+ (const Vector_2& v) const;
   Vector_2 operator+= (const Vector_2& v);
   Vector_2 operator-= (const Vector_2& v);
   Vector_2 operator- (const Vector_2& v) const;
   Vector_2 operator- () const;

   Vector_2 operator/ (const double d) const;
   Vector_2 operator* (const double d) const;
   Vector_2 operator*= (const double d);
   Vector_2 operator/= (const double d);
   double operator[] (const size_t n) const;
   double& operator[] (const size_t n);

   double dot(const Vector_2& v2) const;
   double angle(const Vector_2& v2) const;
   double angleDegrees(const Vector_2& v2) const;
   double norm(const Vector_2& v) const;
   double norm() const;
   Vector_2 Rotate(const double radians) const;
   static std::vector<Vector_2> Rotate(const double radians, const std::vector<Vector_2>& vv);
   Vector_2 RotateDegrees(const double degrees) const;
   static Vector_2 CenterOfMass(const std::vector<Vector_2>& vv);



   static std::vector<Vector_2> ShiftList(const Vector_2& v, const std::vector<Vector_2>& vv) {
      std::vector<Vector_2> out;
      for (size_t i = 0; i < vv.size(); ++i) {
         out.emplace_back(v + vv[i]);
      }
      return out;
   }

private:
   std::vector<double> m_v;
};

class Centroid {
public:
   friend std::ostream& operator<< (std::ostream& o, const Centroid& v);
   Centroid();
   Centroid(const double d);
   Centroid(const double d1, const double d2);
   Centroid(const Vector_2& v);
   Centroid operator+ (const Vector_2& v) const;
   Centroid operator- (const Centroid& v) const;
   Centroid operator/(const double d);
   Centroid& operator+= (const Centroid& v);
   double operator[] (const size_t n) const;
   double& operator[] (const size_t n);
   double norm(const Centroid& v) const;
   double norm() const;
   Vector_2 RotateDegrees(const double degrees);

private:
   Vector_2 m_centroid;
   size_t m_count;
};

class Matrix_2 {
public:

   Matrix_2();
   Matrix_2(const Matrix_2& m);
   Matrix_2(const double d1, const double d2, const double d3, const double d4);

   Matrix_2 operator+(const Matrix_2& m) const;
   Matrix_2 operator-(const Matrix_2& m) const;
   Matrix_2 operator*(const Matrix_2& m2) const;
   Vector_2 operator*(const Vector_2& v) const;
   Matrix_2 operator*(const double d) const;
   Matrix_2 operator+=(const Matrix_2& m);
   Matrix_2 operator-=(const Matrix_2& m);
   //Matrix_2 operator*=(const Matrix_2& m);
   Matrix_2 operator/(const double d) const;
   double operator[] (const size_t n) const;
   double& operator[] (const size_t n);
   static Matrix_2 Rotate(const double angle);
   Vector_2 Eigen1()const;
   static Vector_2 Eigen1(const Matrix_2& m);
   static Matrix_2 InertiaTensor(const std::vector<Vector_2>& vv);
   friend std::ostream& operator<< (std::ostream& o, const Matrix_2& v);

private:
   std::vector<double> m_m;

};


class Polar_2 {
public:
   Polar_2(const double magnitude, const double angle)
      : m_mag(magnitude)
      , m_radians(angle) {

   }

   Polar_2(const Vector_2& v)
      : m_mag(v.norm())
      , m_radians(atan2(v[1], v[0])) {}

   friend std::ostream& operator<< (std::ostream& o, const Polar_2& v);


   // Const version of operator[]
   double operator[] (const size_t n) const;

   // Non-const version of operator[]
   double& operator[] (const size_t n);

   operator Vector_2() const {
      const Polar_2& p(*this);
      return Vector_2(p[0] * cos(p[1]), p[0] * sin(p[1]));
   }

public:
   double m_mag;
   double m_radians;
};


#endif // VECTOR_2_H
