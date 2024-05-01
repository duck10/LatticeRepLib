// RandPL.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "LRL_Cell.h"
#include "LRL_Cell_Degrees.h"
#include "Polar.h"
#include "Vector_2.h"


int main(int argc, char* argv[])
{
   Polar test;
   for (size_t i = 0; i < 50; ++i) {
      test = test.rand();
      const Polar_2 v1 = Polar::ConvertUnitSphereProjectionToLengthAndAngle(test[0]);
      const Polar_2 v2 = Polar::ConvertUnitSphereProjectionToLengthAndAngle(test[1]);
      const Polar_2 v3 = Polar::ConvertUnitSphereProjectionToLengthAndAngle(test[2]);
      if (LRL_Cell::CheckValid(v1[0], v2[0], v3[0], v1[1], v2[1], v3[1]))
         std::cout << "P " << Polar::ConvertPolarToCell(test) << std::endl;
   }
}