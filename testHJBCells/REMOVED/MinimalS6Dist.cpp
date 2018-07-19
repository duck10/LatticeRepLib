// MinimalS6Dist.cpp : Defines the entry point for the console application.
//

#include <iostream>

#include "S6.h"
#include "S6Dist.h"
#include "Selling.h"

int main()
{
   const S6 s6_A(S6::rand());
   const S6 s6_B(S6::rand());
   S6 red_A;
   S6 red_B;
   S6Dist s6dist;
   const bool bA = Selling::Reduce(s6_A, red_A);
   const bool bB = Selling::Reduce(s6_B, red_B);
   std::cout << red_A << std::endl;
   std::cout << red_B << std::endl;
   const double dist = s6dist.DistanceBetween(red_A, red_B);
   std::cout << "distance between red_A and red_B  " << dist << std::endl;
   const double distAB = s6dist.DistanceBetween(s6_A, s6_B);
   std::cout << "distance between s6_A and s6_B (incorrect using NON-reduced vectors " << distAB << std::endl;
   std::cout << std::endl;

   std::vector< S6(*)(const S6&)> refl = S6::SetRelectionFunctions();
   for ( size_t i=0; i<24; ++i ) {
      const double dist = s6dist.DistanceBetween(red_A, refl[i](red_A) );
      std::cout << i << "  " << dist << std::endl;
   }
   return 0;
}

