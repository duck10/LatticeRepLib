#include <iomanip>

#include "LRL_MinMaxTools.h"
#include "rhrand.h"
#include "Polar.h"
#include "StoreResults.h"

RHrand ran;

std::ostream& operator<< (std::ostream& os, const Polar& p) {
   os << std::setprecision(4) << "(" << p[0] << "), (" << p[1] << "), (" << p[2] << ")";
   return os;
}

Polar::Polar() {
   m_vec.resize(3);
   m_vec[0] = Vector_2(19191, 19191);
   m_vec[1] = Vector_2(19192, 19192);
   m_vec[2] = Vector_2(19193, 19193);
}

Polar::Polar(const Vector_2& v1, const Vector_2& v2, const Vector_2& v3) {
   m_vec.resize(3);
   m_vec[0] = v1;
   m_vec[1] = v2;
   m_vec[2] = v3;
}

const double rad = 180.0 / (4 * atan(1.0));
const double radiansPerDegree = 1.0 / rad;
const double fiveDegrees = 5.0 * radiansPerDegree;
const double onehundredseventyDegrees = 170.0 * radiansPerDegree;
const double axialLength = 10.0;


Polar::operator LRL_Cell() const
{
   LRL_Cell_Degrees out;
   const Polar& p(*this);
   const double a = p[0].norm();
   const double b = p[1].norm();
   const double c = p[2].norm();
   const double alpha = atan2(p[0][1], p[0][0]);
   const double beta  = atan2(p[1][1], p[1][0]);
   const double gamma = atan2(p[2][1], p[2][0]);
   const bool b5 = (alpha + beta + gamma - 2.0 * maxNC(alpha, beta, gamma)) >= 0.0 - 0.00001;
   if (b5) {
      return LRL_Cell(a, b, c, alpha, beta, gamma);
   }
   else
   {
      return LRL_Cell();
   }
}



/*
   Vector_2 out(19191, 19191);
   double sum = 19191.0;
   while (sum > 0.99) {
      const double r1 = ran.urand();
      const double r2 = ran.urand();
      out[0] = v[0] * r1;
      out[1] = v[1] * r2 * onehundredseventyDegrees + fiveDegrees;
   }
   return out;
*/

Polar_2 Polar::ConvertUnitSphereProjectionToLengthAndAngle(const Vector_2& v) {
   return Polar_2 ( v.norm() * axialLength, atan2(v[1], v[0])); // radians
}

Vector_2 XYtoV2(const Vector_2& v) {
   Vector_2 out;
   out[0] = axialLength * v[0];
   return out;
}

RHrand ranx;

Vector_2 GeneratePolarProjection() {
   const Vector_2 v(2.0 * (ranx.urand() - 0.5), ranx.urand());
   return v;
}

Polar Polar::RandWork() {
   double sum(19191.0);
   Vector_2 prj1;
   Vector_2 prj2;
   Vector_2 prj3;

   bool validCell = false;
   int count = 0;
   Polar_2 p2_1(-1,-1);
   Polar_2 p2_2(-1,-1);
   Polar_2 p2_3(-1,-1);
   while (!validCell && count < 100)
   {
      ++count;
      validCell = true;
      while (sum > 0.99)
      {
         prj1 = GeneratePolarProjection();
         prj2 = GeneratePolarProjection();
         sum = prj1[0] * prj1[0] + prj1[1] * prj1[1] + prj2[0] * prj2[0] + prj2[1] * prj2[1];
         if (sum > 0.99) continue;
         prj3 = GeneratePolarProjection();
         sum += prj3[0] * prj3[0] + prj3[1] * prj3[1];
         if (sum > 0.99) continue;
      p2_1 = Polar::ConvertUnitSphereProjectionToLengthAndAngle(prj1);
      p2_2 = Polar::ConvertUnitSphereProjectionToLengthAndAngle(prj2);
      p2_3 = Polar::ConvertUnitSphereProjectionToLengthAndAngle(prj3);
      }
   }

   validCell = LRL_Cell::CheckValid(p2_1[0], p2_2[0], p2_3[0], p2_1[1], p2_2[1], p2_3[1]);
   if(validCell) LRL_Cell cell(p2_1[0], p2_2[0], p2_3[0], p2_1[1], p2_2[1], p2_3[1]);

   return Polar(
      Polar::ConvertUnitSphereProjectionToLengthAndAngle(prj1),
      Polar::ConvertUnitSphereProjectionToLengthAndAngle(prj2),
      Polar::ConvertUnitSphereProjectionToLengthAndAngle(prj3));
}

Polar Polar::rand() {

   Polar out;
   do {
      out = RandWork();
   } while (!out.CheckValid());

   return out;

   //double sum(19191.0);
   //Vector_2 prj1;
   //Vector_2 prj2;
   //Vector_2 prj3;

   //bool validCell = false;
   //int count = 0;
   //while (!validCell && count < 100)
   //{
   //   ++count;
   //   validCell = true;
   //   while (sum > 0.99)
   //   {
   //      sum = 0.0;
   //      prj1 = GeneratePolarProjection();
   //      prj2 = GeneratePolarProjection();
   //      sum = prj1[0] * prj1[0] + prj1[1] * prj1[1] + prj2[0] * prj2[0] + prj2[1] * prj2[1];
   //      if (sum > 0.99) continue;
   //      prj3 = GeneratePolarProjection();

   //      //std::cout << "in Polar::rand() \n";
   //      //std::cout << "prj1 " << prj1 << std::endl;
   //      //std::cout << "prj2 " << prj2 << std::endl;
   //      //std::cout << "prj3 " << prj3 << std::endl;
   //      sum += prj3[0] * prj3[0] + prj3[1] * prj3[1];
   //   }

   //   const Polar_2 p2_1 = ConvertUnitSphereProjectionToLengthAndAngle(prj1);
   //   const Polar_2 p2_2 = ConvertUnitSphereProjectionToLengthAndAngle(prj2);
   //   const Polar_2 p2_3 = ConvertUnitSphereProjectionToLengthAndAngle(prj3);
   //   //LRL_Cell cell(p2_1[0], p2_2[0], p2_3[0], p2_1[1], p2_2[1], p2_3[1]);
   //   validCell = LRL_Cell::CheckValid(p2_1[0], p2_2[0], p2_3[0], p2_1[1], p2_2[1], p2_3[1]);
   //   //LRL_Cell cell(p2_1[0], p2_2[0], p2_3[0], p2_1[1], p2_2[1], p2_3[1]);

   //   //std::cout << " in Polar::rand() " << validCell << std::endl;

   //   const int i19191 = 19191;
   //}
   //std::cout << count << std::endl;
   //return Polar(
   //   ConvertUnitSphereProjectionToLengthAndAngle(prj1), 
   //   ConvertUnitSphereProjectionToLengthAndAngle(prj2), 
   //   ConvertUnitSphereProjectionToLengthAndAngle(prj3));
}

LRL_Cell_Degrees Polar::ConvertPolarToCell(const Polar& p) {
   const double a = sqrt(p[0][0] * p[0][0] + p[0][1] * p[0][1]);
   const double alpha = atan2(p[0][1], p[0][0]);

   const double b = sqrt(p[1][0] * p[1][0] + p[1][1] * p[1][1]);
   const double beta = atan2(p[1][1], p[1][0]);

   const double c = sqrt(p[2][0] * p[2][0] + p[2][1] * p[2][1]);
   const double gamma = atan2(p[2][1], p[2][0]);
   return LRL_Cell(a, b, c, rad*alpha, rad*beta, rad*gamma);
}


bool Polar::CheckValid() {
   return CheckValid(*this);
}

//bool Polar::CheckValid(const Polar& p) {
//   return CheckValid(*this);
//}

bool Polar::CheckValid(const LRL_Cell& cell) {
   return CheckValid(cell[0], cell[1], cell[2], cell[3], cell[4], cell[5]);
}

bool Polar::CheckValid(const double a, const double b, const double c,
   const double alpha, const double beta, const double gamma) {
   if (a == 0.0) return false;
   static const double pi = 4.0 * atan(1.0);
   static const double twopi = 2.0 * pi;
   static const double rad = 180.0 / pi;

   const bool radianInput = alpha < pi && beta < pi && gamma < pi;

   const double lowerlimit = 0.001;
   const bool b1 = a > lowerlimit && b > lowerlimit && c > lowerlimit;
   const bool b2 = alpha > lowerlimit && beta > lowerlimit && gamma > lowerlimit;
   const bool b3 = alpha < 179.99 && beta < 179.99 && gamma < 179.99;

   const double anglesum = alpha + beta + gamma;
   const bool b4 = (radianInput) ? (anglesum < twopi) : (anglesum < 360.0);

   // b5 checks the triangle inequality for the angles
   const bool b5 = (alpha + beta + gamma - 2.0 * maxNC(alpha, beta, gamma)) >= 0.0 - 0.00001;

   //std::cout << "in CheckValid " << ((b1 && b2 && b3 && b4 && b5) ? "true" : "false") << std::endl;

   const bool check(b1 && b2 && b3 && b4 && b5);
   return check;
}
