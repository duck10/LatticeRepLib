// DevelopS6Boundaries.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

#include <iostream>

#include "S6_Boundary.h"
#include "S6_BoundaryList.h"
#include "S6.h"
#include "S6Dist.h"

int main()
{
   S6Dist s6dist(1);
   s6dist.SetDebug(true);
   for (unsigned long i = 0; i < 10; ++i) {
      S6::randDeloneReduced();
      S6::randDeloneReduced();
      const double dist = s6dist.DistanceBetween(S6::randDeloneReduced(), S6::randDeloneReduced());
   }
   const std::vector<S6_Boundary> bounds(S6_BoundaryList::GetVectorOfBoundaries());
   const S6 s6(1, 2, 3, 4, 5, 6);
   for (unsigned long i = 0; i < bounds.size(); ++i) {
      std::cout << bounds[i].GetPerp()*s6 << std::endl;
      std::cout << bounds[i].GetProjector()*s6 << std::endl;
      std::cout << bounds[i] << std::endl;
      std::cout << std::endl;
   }

   return 0;
}

