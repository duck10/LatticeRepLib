#include "P3.h"
#include "Polar.h"

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
   Polar_2 p2_1(-1, -1);
   Polar_2 p2_2(-1, -1);
   Polar_2 p2_3(-1, -1);
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
   if (validCell) LRL_Cell cell(p2_1[0], p2_2[0], p2_3[0], p2_1[1], p2_2[1], p2_3[1]);

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
