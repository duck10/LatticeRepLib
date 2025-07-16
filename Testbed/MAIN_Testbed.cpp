#include <iostream>
#include <cmath>

#include "Polar.h"
#include "LRL_Vector3.h"
#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"

//#include "InputHandler.h"
//#include "ProgramSetup.h"

Vector_3 FixZeros(const Vector_3 sarah_vector) {
   Vector_3 out;
   for (int i = 0; i < 3; ++i) {
      if (abs(sarah_vector[i]) < 1.0E-10) {
         out[i] = 0.0;
      }
      else {
         out[i] = sarah_vector[i];
      }
   }
   return out;
}


// Example usage
int main() {

   Vector_3 a(1, 0, 0);
   const Vector_3 b(0, 1, 0);
   const Vector_3 sum = a + b;

   a = a + a;
   a = 2 * a;
   a += a;
   a = 2 * a;

   LRL_Cell random = LRL_Cell::rand();
   std::cout << "random unit cell " << LRL_Cell_Degrees(Polar::rand()) << std::endl;
   std::cout << "random unit cell " << LRL_Cell_Degrees(Polar::rand()) << std::endl;
   std::cout << "random unit cell " << LRL_Cell_Degrees(Polar::rand()) << std::endl;
   std::cout << "random unit cell " << LRL_Cell_Degrees(Polar::rand()) << std::endl;


   //Vector_3 operator* (const double&, const Vector_3&);
   a = 2 * b;


   std::cout << " a " << a << std::endl;
   std::cout << " b " << b << std::endl;
   std::cout << " sum " << sum << std::endl;

   const Matrix_3x3 m1("1 0 0  0 1 0  0 0 1");
   const Matrix_3x3 m2(0, 1, 0,   1, 0, 0,   0, 0, 1);

   std::cout << " m1 " << std::endl << m1 << std::endl;
   std::cout << " m2 " << std::endl << m2 << std::endl;

   std::cout << std::endl;

   std::cout << " m1*a " << m1 * a << std::endl;
   std::cout << " m2*a " << m2 * a << std::endl;
   std::cout << std::endl;
   std::cout << " m1*b " << m1 * b << std::endl;
   std::cout << " m2*b " << m2 * b << std::endl;

   // to convert from degrees to radians. 2pi/180
   const double converter = 4.0 * atan(1.0) / 180.0;
   std::cout << " converter " << converter << std::endl;
   std::cout << " 1/converter " << 1.0/converter << std::endl;
   std::cout << " converter*180 " << converter*180 << std::endl;


   //Matrix_3x3 Rotmat(const double angle) const;
   const Vector_3 axis(1, 0, 0);
   const Matrix_3x3 rotation1 = axis.Rotmat(converter*180.0); // this should have been radians
   std::cout << "rotation1 * b " << FixZeros(rotation1 * b) << std::endl;


}