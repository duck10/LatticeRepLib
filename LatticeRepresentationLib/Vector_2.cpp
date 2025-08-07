#include "rhrand.h"
#include "Vector_2.h"

#include <algorithm>

Vector_2::Vector_2()
{
   m_v.resize(2);
   m_v[0] = 19191;
   m_v[1] = 19191;
}

Vector_2::Vector_2(const double d1, const double d2)
{
   m_v.resize(2);
   m_v[0] = d1;
   m_v[1] = d2;
}

Vector_2::Vector_2(const Vector_2& v)
{
   m_v.resize(2);
   m_v[0] = v[0];
   m_v[1] = v[1];
}

Vector_2 Vector_2::AveragePosition(const std::vector<Vector_2>& vv) const
{
   Vector_2 avg;
   for (size_t i = 0; i < vv.size(); ++i) {
      avg += vv[i];
   }
   return avg / double(vv.size());
}

Vector_2& Vector_2::operator=(const Vector_2& v)
{
   m_v = v.m_v;
   return *this;
}

Vector_2 Vector_2::operator+(const Vector_2& v) const
{
   return Vector_2((*this).m_v[0]+v[0], (*this).m_v[1]+v[1]);
}

Vector_2 Vector_2::operator-(const Vector_2& v) const
{
   return Vector_2((*this).m_v[0] - v[0], (*this).m_v[1] - v[1]);
}

Vector_2 Vector_2::operator-() const
{
   const Vector_2& v(*this);
   return { -v[0], -v[1] };
}

Vector_2 Vector_2::operator+=(const Vector_2& v)
{
   (*this).m_v[0] += v[0];
   (*this).m_v[1] += v[1];
   return *this;
}

Vector_2 Vector_2::operator-=(const Vector_2& v)
{
   m_v[0] -= v[0];
   m_v[1] -= v[1];
   return *this;
}

Vector_2 Vector_2::operator*=(const double d)
{
   m_v[0] *= d;
   m_v[1] *= d;
   return *this;
}

Vector_2 Vector_2::operator/=(const double d)
{
   m_v[0] /= d;
   m_v[1] /= d;
   return *this;
}

Vector_2 Vector_2::operator/(const double d) const
{
   const Vector_2& v(*this);
   return Vector_2(v[0]/d, v[1]/d);
}

Vector_2 Vector_2::operator*(const double d) const
{
   const Vector_2& v(*this);
   return Vector_2(v[0] * d, v[1] * d);
}

double Vector_2::operator[](const size_t n) const
{
   return m_v[n];
}

double& Vector_2::operator[](const size_t n)
{
   return m_v[n];
}

std::ostream& operator<<(std::ostream& o, const Vector_2& v)
{
   o << v[0] << ", " << v[1];
   return o;
}

double Vector_2::dot(const Vector_2& v2) const
{
   const Vector_2& v1(*this);
   return v1[0] * v2[0] + v1[1] * v2[1];
}

double Vector_2::angle(const Vector_2& v2) const
{
   const Vector_2& v1(*this);
   const double cosAngle = v1.dot(v2) / v1.norm() / v2.norm();
   return atan2(sqrt(1.0 - cosAngle * cosAngle), cosAngle);
}

double Vector_2::angleDegrees(const Vector_2& v2) const
{
   const Vector_2& v1(*this);
   return v1.angle(v2) * 180.0 / (4.0 * atan(1.0));
}

double Vector_2::norm(const Vector_2& v) const
{
   return sqrt(v[0] * v[0] + v[1] * v[1]);
}

double Vector_2::norm() const
{
   const Vector_2& v(*this);
   return sqrt(v[0] * v[0] + v[1] * v[1]);
}

double Vector_2::Norm() const {
   return (*this).norm();
}

Centroid::Centroid()
   : m_centroid(0,0)
   , m_count(0)
{
}

Centroid::Centroid(const double d)
   : m_centroid(d, d)
   , m_count(1)
{}

Centroid::Centroid(const double d1, const double d2)
   : m_centroid(d1, d2)
   , m_count(1)
{}

Centroid::Centroid(const Vector_2 & v)
   : m_centroid(v)
   , m_count(1)
{
}

Centroid Centroid::operator+(const Vector_2& v) const
{
   Centroid cent(*this);
   cent[0] += v[0];
   cent[1] += v[1];
   cent.m_count += 1;

   return cent;
}

Centroid Centroid::operator-(const Centroid& v) const
{
   Centroid cent(*this);
   cent.m_centroid[0] -= v[0];;
   cent.m_centroid[1] -= v[1];;
   cent.m_count = 1;
   return cent;
}

Centroid Centroid::operator/(const double d)
{
   m_centroid /= d;
   return *this;
}

double Centroid::operator[](const size_t n) const
{
   return m_centroid[n];
}

double& Centroid::operator[](const size_t n) 
{
   return m_centroid[n];
}

double Centroid::norm(const Centroid& v) const
{
   return sqrt(v[0] * v[0] + v[1] * v[1]);
}

double Centroid::norm() const
{
   const Centroid& v(*this);
   return sqrt(v[0] * v[0] + v[1] * v[1]);
}

Vector_2 Vector_2::Rotate(const double radians) const
{
   const Vector_2& v(*this);
   const double d1 = v[0] * cos(radians) - v[1] * sin(radians);
   const double d2 = v[0] * sin(radians) + v[1] * cos(radians);
   return Vector_2(d1, d2);
}

std::vector<Vector_2> Vector_2::Rotate(const double radians, const std::vector<Vector_2>& vv) {
   std::vector<Vector_2> out;
   for (size_t i = 0; i < vv.size(); ++i) {
      out.emplace_back(vv[i].Rotate(radians));
   }
   return out;
}

Vector_2 Vector_2::RotateDegrees(const double degrees) const {
   return Rotate(degrees * 180.0 / (4 * atan(1.0)));
}

Vector_2 Vector_2::CenterOfMass(const std::vector<Vector_2>& vv)
{
   Vector_2 cm(0.0, 0.0);
   for (size_t i = 0; i < vv.size(); ++i) {
      cm += vv[i];
   }
   return cm / double(vv.size());
}

std::ostream& operator<<(std::ostream& o, const Centroid& v)
{
   o << v[0] << " " << v[1] << std::endl;
   return o;
}


Centroid& Centroid::operator+=(const Centroid& v)
{
   m_centroid[0] += v[0];
   m_centroid[1] += v[1];
   m_count += v.m_count;
   return *this;
}

Matrix_2::Matrix_2()
{
   m_m = { 19191,19191,19191,19191 };
}

Matrix_2::Matrix_2(const Matrix_2& m)
{
   if (m.m_m.size() != 4) throw("incorrect size of Matrix_2");
   m_m = m.m_m;
}

Matrix_2::Matrix_2(const double d1, const double d2, const double d3, const double d4)
{
   m_m = { d1,d2,d3,d4 };
}

Matrix_2 Matrix_2::operator+(const Matrix_2& m) const
{
   Matrix_2 out(*this);
   out.m_m[0] += m.m_m[0];
   out.m_m[1] += m.m_m[1];
   out.m_m[3] += m.m_m[2];
   out.m_m[0] += m.m_m[3];
   return out;
}
Matrix_2 Matrix_2::operator-(const Matrix_2& m) const
{
   Matrix_2 out(*this);
   out.m_m[0] -= m.m_m[0];
   out.m_m[1] -= m.m_m[1];
   out.m_m[2] -= m.m_m[2];
   out.m_m[3] -= m.m_m[3];
   return out;
}

Matrix_2 Matrix_2::operator*(const Matrix_2& m2) const
{
   Matrix_2 out(*this);
   out.m_m = {
      m_m[0] * m2.m_m[0] + m_m[1] * m2.m_m[2],
      m_m[2] * m2.m_m[0] + m_m[3] * m2.m_m[2],
      m_m[0] * m2.m_m[1] + m_m[1] * m2.m_m[3],
      m_m[2] * m2.m_m[1] + m_m[3] * m2.m_m[3]
   };
   return out;
}

Vector_2 Matrix_2::operator*(const Vector_2& v) const
{
   Vector_2 out{
      m_m[0]*v[0] + m_m[1]*v[1],
      m_m[2]*v[0] + m_m[3]*v[1]
      };
   return out;
}

Matrix_2 Matrix_2::operator*(const double d) const
{
   Matrix_2 out(*this);
   out.m_m[0] *= d;
   out.m_m[1] *= d;
   out.m_m[2] *= d;
   out.m_m[3] *= d;
   return out;
}

Matrix_2 Matrix_2::operator+=(const Matrix_2& m2)
{
   Matrix_2& m(*this);
   return {
      m.m_m[0] + m2.m_m[0],
      m.m_m[1] + m2.m_m[1],
      m.m_m[2] + m2.m_m[2],
      m.m_m[3] + m2.m_m[3]
   };
   return *this;
}

Matrix_2 Matrix_2::operator-=(const Matrix_2& m2)
{
   Matrix_2& m(*this);
   return {
      m.m_m[0] - m2.m_m[0],
      m.m_m[1] - m2.m_m[1],
      m.m_m[2] - m2.m_m[2],
      m.m_m[3] - m2.m_m[3]
   };
   return *this;
}

Matrix_2 Matrix_2::operator/(const double d) const
{
   Matrix_2 out(*this);
   out.m_m[0] /= d;
   out.m_m[1] /= d;
   out.m_m[2] /= d;
   out.m_m[3] /= d;
   return out;
}

double Matrix_2::operator[](const size_t n) const
{
   return m_m[n];
}

double& Matrix_2::operator[](const size_t n)
{
   return m_m[n];
}

Matrix_2 Matrix_2::Rotate(const double angle)
{
   return {
      cos(angle),
      -sin(angle),
      sin(angle),
      cos(angle)
   };
}

Vector_2 Matrix_2::Eigen1() const
{
   return Eigen1(*this);
}

Vector_2 Matrix_2::Eigen1(const Matrix_2& m)
{
   Vector_2 out(1, 1);
   for (size_t i = 0; i < 16; ++i) {
      out = m * out / out.norm();
   }
   return out / out.norm();
}

double SumXsq(const std::vector<Vector_2>& vv) {
   double out(0.0);
   for (size_t i = 0; i < vv.size(); ++i) {
      out += vv[i][0] * vv[i][0];
   }
   return out;
}

double SumYsq(const std::vector<Vector_2>& vv) {
   double out(0.0);
   for (size_t i = 0; i < vv.size(); ++i) {
      out += vv[i][1] * vv[i][1];
   }
   return out;
}

double SumXY(const std::vector<Vector_2>& vv) {
   double out(0.0);
   for (size_t i = 0; i < vv.size(); ++i) {
      out += vv[i][0] * vv[i][1];
   }
   return out;
}

Matrix_2 Matrix_2::InertiaTensor(const std::vector<Vector_2>& vv) {
   return {SumYsq(vv), -SumXY(vv), -SumXY(vv), SumXsq(vv)};
}


std::ostream& operator<<(std::ostream& o, const Matrix_2& v)
{
   o << v[0] << " " << v[1] << "    " << v[2] << " " << v[3];
   return o;
}


// Const version of operator[]
double Polar_2::operator[] (const size_t n) const {
   return (n == 0) ? m_mag : m_radians;
}
// Non-const version of operator[]
double& Polar_2::operator[] (const size_t n) {
   if (n == 0)
      return m_mag;
   else
      return m_radians;
}

std::ostream& operator<< (std::ostream& o, const Polar_2& v) {
   o << "magnitude " << v.m_mag << std::endl;
   o << "radians " << v.m_radians << std::endl;
   return o;
}
